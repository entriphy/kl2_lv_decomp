#include "harada/hr_obcon2.h"
#include "harada/hr_obcon.h"
#include "harada/hr_main.h"
#include "harada/hr_nak.h"
#include "harada/hr_pall.h"
#include "harada/hr_pbgm.h"
#include "harada/hr_take.h"
#include "hoshino/h_util.h"

static void hr_gipt_check(OBJWORK *objw);
static void hr_gipt_work(OBJWORK *objw);
static void hr_gipt_tognd(OBJWORK *objw);
static void hr_gipt_skipvc(OBJWORK *objw);
static void hr_gipt_nonpause(OBJWORK *objw);

// TODO: fill these in
static PT96A hr_psw0100s1 = {};
s32 hr_psw_0100[0] = {};
static PT96A hr_psw0104s0 = {};
static PT64A hr_psw0104s1 = {};
s32 hr_psw_0104[0] = {};
static PT96A hr_psw0400s1 = {};
s32 hr_psw_0400[0] = {};
static PT96A hr_psw040bs0 = {};
static PT64A hr_psw040bs1 = {};
static PT64A hr_psw040bs2 = {};
s32 hr_psw_040b[0] = {};
static PT64A hr_psw0505s0 = {};
static PT64A hr_psw0505s1 = {};
s32 hr_psw_0505[0] = {};
static PT96A hr_psw1004s0 = {};
s32 hr_psw_1004[0] = {};
static void (*hrGameInPtTbl[5])(OBJWORK *) = {
    hr_gipt_check,
    hr_gipt_work,
    hr_gipt_tognd,
    hr_gipt_skipvc,
    hr_gipt_nonpause
};

static s32 hr_start_gpt(s32 scene, s16 id) {
    if ((s32)hrpt_pack == -1) {
        return 0;
    }

    if (*hrpt_pack <= scene + 1) {
        return 0;
    }

    hrpt_addr = (s32 *)GetFHMAddress(hrpt_pack, scene);
    ppmes.font = hrpt_gms;
    hrptm_addr = (s32 *)-1;
    GameGbl.cam.mode = 7;
    if (id == 2) {
        hr_pt_set(2, (s16)scene / 2, 0, 0);
        OkBalloonDie();
    }
    hr_pall_init();
    return 1;
}

static s32 hr_start_ptmini(s32 scene, u32 *addr) {
    hrpt_addr = (s32 *)addr;
    ppmes.font = (u32 *)-1;
    hrptm_addr = (s32 *)-1;
    hr_pt_set(4, scene, 0, 0);
    hr_pall_init();
    return 1;
}

void hr_gpt_getset(OBJWORK *objw, s32 *tbl) {
    objw->reg0 = *tbl++;
    objw->reg1 = 0;
    objw->work = tbl;
}

void hr_gpt_resReg1(OBJWORK *objw, s16 *idt, s32 num) {
    s32 i;
    u32 mask;

    if (objw == NULL) {
        return;
    }

    for (i = 0, mask = 0x1; i < num; i++, mask <<= 1, idt++) {
        if (hr_pflag_check(*idt)) {
            objw->reg1 |= mask;
        }
    }
}

s32 hr_ptmini_getset(OBJWORK *objw) {
    if ((s32)hrpt_mini_addr == -1) {
        return 0;
    }

    if (*hrpt_mini_addr > 32) {
        objw->reg0 = 32;
    } else {
        objw->reg0 = *hrpt_mini_addr;
    }
    objw->reg1 = 0;
    return 1;
}

s32 hr_pnt_getset(OBJWORK *objw) {
    if (hr_start_gpt(hrpt_scene << 1, 1) == 0) {
        return 0;
    } else {
        objw->bun0 = 1;
        return 1;
    }
}

static s32 hr_stsw_checkN(PT64A *p64a, s32 stat1) {
    s32 fg;

    fg = (stat1 == 4 ? OkGetSwitchFlag(4, p64a->ss0) : OkGetSwitchFlag2(4, p64a->ss0));

    // i hate you
    if (fg == 1) {
        fg = 1;
    } else {
        fg = 0;
    }

    if (p64a->ss1 == 1) {
        fg ^= 1;
    }

    return fg;
}

static void hr_gipt_check(OBJWORK *objw) {
    u16 *cmdno;
    u32 mc;
    u16 no;
    s32 i;
    s32 flag;
    OBJWORK *kw;
    HERO_WORK *herow;
    s32 *tbl;

    kw = GameGbl.klonoa;
    no = GetMichiNo(kw->rtw.rtp);
    mc = kw->rtw.mcn;
    herow = kw->work;
    tbl = objw->work;

    for (i = 0, flag = 1; i < objw->reg0; i++, flag <<= 1, tbl += 3) {
        if (flag & objw->reg1) {
            continue;
        }

        if (objw->stat1 == 4) {
            tbl = (s32 *)GetFHMAddress((u32 *)hrpt_mini_addr, i);
            cmdno = (u16 *)tbl;
        } else {
            cmdno = (u16 *)*tbl;
        }

        switch (*cmdno) {
            case 0x308:
                if (hr_stsw_checkN((PT64A *)cmdno, objw->stat1) == 0) {
                    continue;
                }
                if (objw->stat1 != 4 && tbl[2] & 6 && hr_ptchk_45(tbl[2] & 6)) {
                    objw->reg1 |= flag;
                    continue;
                }
                break;
            case 0x307:
                if (hr_strt_check(cmdno, no, mc, herow->yuka_hit) == 0) {
                    continue;
                }
                break;
            default:
                continue;
        }

        if (GameGbl.kloLife[0] == 0) {
            objw->stat0 = 0;
            break;
        }
        objw->reg1 |= flag;
        if (objw->stat1 == 2) {
            if (tbl[2] & 1 && herow->yuka_hit == 0) {
                hr_open_klokey(1);
                objw->bun0 = 2;
                objw->reg2 = tbl[1];
            } else if (hr_start_gpt(tbl[1], 2)) {
                objw->bun0 = 1;
            }
        }

        if (objw->stat1 == 4) {
            if (*cmdno == 0x307) {
                tbl += 3;
            } else {
                tbl += 2;
            }
            if (hr_start_ptmini(i, (u32 *)tbl)) {
                objw->bun0 = 1;
            }
        }
        break;
    }
}

static void hr_gipt_draw(OBJWORK *objw) {
    s32 var;

    var = hr_check_skw();
    if (var) {
        hrpt_gipt_work(objw);
        if (var == 1 && GameGbl.pause_flag != 0) {
            GameGbl.pause_flag2 = 0;
        }
    }
    hr_take_initF();
    hr_take_setM();
    hr_pall_draw();
}

static void hr_gipt_drawMir(OBJWORK *objw) {
    hr_ptmir_draw(objw);
}

void hr_skipvc_draw(OBJWORK *objw) {
    hr_skip_drawM(0);
}

void hr_ptback_klo(s32 no, s32 mcn, s32 muki, f32 y, s32 flag) {
    if (flag & 0x100) {
        SetRtwKlonoaY(no, mcn, muki, y);
    } else {
        SetRtwKlonoa(no, mcn, muki);
    }

    switch (hrpt_id)
    {
        case PT0102_S0:
            SetKlonoaMode(1, 0);
            break;
        case PT0100_S0:
        case PT0500_S0:
        case PT1000_S0:
        case PT1100_S0:
        case PT14XX_S0:
        case PT1502_S0:
        case PT16XX_S0:
        case PT17XX_S0:
            OkSaveforReStart(-1, 0, y, 0, 0);
            break;
    }
}

void hr_ptnext_klo(s32 vision, s32 no, s32 mcn, s32 muki, f32 y) {
    nkChangeArea2(y, vision, no, mcn << 12);
    if (hrpt_id == PT0100_S2) {
        hrVT_allwaveErase();
        hrVT_waveSet(0);
    }
}

s32 hr_check_VCmain() {
    switch(hrpt_id) {
        case PT0104_S1:
        case PT0207_S0:
        case PT040B_S1:
        case PT040B_S2:
        case PT0505_S0:
        case PT0505_S1:
        case PT0609_S0:
        case PT0706_S0:
        case PT0706_S1:
        case PT0908_S1:
        case PT1004_S0:
        case PT1111_S0:
        case PT1207_S3:
        case PT13XX_S2:
        case PT14XX_S1:
        case PT1502_S1:
        case PT16XX_S1:
        case PT1805_S0:
        case PT1908_S0:
            return 1;
        case PT2100_S2:
        case PT2201_S0:
        case PT20XX_S1:
        case PT20XX_S4:
        case PT20XX_S7:
        case PT2XXX_S3:
            return 8;
        default:
            return -1;
    }
}

s32 hr_check_VC(OBJWORK *objw, s32 ret) {
    s32 var;

    if (objw->stat1 == 4 || (ret & -0x101) != 1) {
        return 0;
    }

    var = hr_check_VCmain();
    if (var == 1 && ret & 0x100) {
        var = 2;
    }
    return var;
}

s32 hr_check_camRel(s32 stat, s32 nflag) {
    if (stat == HR_GPTMINI) {
        return 0;
    }
    if (kzCheckSelect()) {
        return 0;
    }
    if (nflag) {
        return 0;
    }

    return 1;
}

s32 hr_check_black(s32 ret, s32 skipid) {
    if (skipid != 0) {
        if (ret == 2) {
            return 1;
        }
    } else {
        switch (hrpt_id) {
            case PT0204_S0:
            case PT20XX_S0:
            case PT1100_S0:
            case PT1500_S0:
            case PT16XX_S0:
                return 1;
        }
    }

    return 0;
}

static void hr_gipt_work(OBJWORK *objw) {
    s32 ret;
    f32 tm;
    s32 var_s3;
    s32 var_s2;

    ret = hr_pall_work();
    if (hrptm_res != 0 && objw->stat1 == 4) {
        objw->reg1 = 0;
        hrptm_res = 0;
    }
    if (ret) {
        hr_abeoff = 0;
        var_s3 = hr_check_VC(objw, ret);
        var_s2 = 0;
        if (ret & 0x100) {
            tm = 1.0f;
            ret ^= 0x100;
            hrpt_flag = 8;
            var_s2 = 1;
        } else {
            tm = hr_pall_exitCam() == 0 ? 1.0f : 60.0f;
        }

        if (hrpt_id == PTTITLE) {
            hrpt_flag = 8;
            tm = 1.0f;
        } else if (hr_check_black(ret, var_s2)) {
            hrpt_flag = 8;
            hrpt_patch |= 0x8;
        }

        objw->draw = NULL;
        objw->dreff = NULL;
        objw->drmir = NULL;

        if (hr_check_camRel(objw->stat1, var_s3)) {
            CamRelease(tm);
            if (ret == 3) {
                hr_pall_ncnt();
            }
        } else if (kzCheckSelect() && ret == 3) {
            if (hrpt_id == PTTITLE) {
                if (var_s2 == 0) {
                    var_s2 = 2;
                } else {
                    var_s2 = 3;
                }
                kzReturnTitle(var_s2);
            } else if (GameGbl.vision == 0x2300) {
                kzReturnMommetTop();
            } else {
                hr_pall_ncnt();
            }
        }

        if (objw->stat1 != 4 && (ret != 3 || hr_pall_ncntSN() == -1)) {
            kzLoadCommonGms();
        }

        switch (var_s3) {
            case 2:
                objw->bun0 = 3;
                objw->draw = hr_skipvc_draw;
                objw->reg3 = 0;
                kzVisionClearOn();
                hrpt_flag |= 0x10;
                break;
            default:
                if (var_s3 == 1 || var_s3 == 8) {
                    kzReturnSave();
                } else {
                    hrStageEnd();
                    if (hrpt_id == PTENDING) {
                        kzReturnSRoll();
                    } else {
                        kzReturnWorldMap();
                    }
                }
                break;
            case 0:
                if (ret == 2) {
                    objw->bun0 = 4;
                    objw->reg3 = 0;
                    GameGbl.non_pause_flag = 1;
                } else if (objw->stat1 == 5) {
                    objw->stat0 = 0;
                } else {
                    objw->bun0 = 0;
                }
                break;
        }
    } else {
        objw->draw = hr_gipt_draw;
        objw->dreff = NULL;
        if (hr_ptmir_check()) {
            objw->drmir = hr_gipt_drawMir;
        } else {
            objw->drmir = NULL;
        }
    }
}

s32* hr_ptmir_debobc(s32 *ptr) {
    OBJWORK *objw;

    if (ptr == NULL) {
        objw = hr_get_obcon(HR_DUMMYOBC);
        if (objw == NULL) {
            return NULL;
        }
    } else {
        objw = (OBJWORK *)ptr;
    }

    if (hr_ptmir_check()) {
        objw->drmir = hr_gipt_drawMir;
    } else {
        objw->drmir = NULL;
    }

    return (s32 *)objw;
}

static void hr_gipt_tognd(OBJWORK *objw) {
    HERO_WORK *herow;

    herow = (HERO_WORK *)GameGbl.klonoa->work;
    if (herow->yuka_hit != 0) {
        hr_open_klokey(0);
        if (hr_start_gpt(objw->reg2, 2)) {
            objw->bun0 = 1;
        } else {
            objw->bun0 = 0;
        }
    }
}

static void hr_gipt_skipvc(OBJWORK *objw) {
    objw->reg3++;
    if (objw->reg3 >= 270) {
        hrpt_flag = 0;
        kzReturnSave();
    }
}

static void hr_gipt_nonpause(OBJWORK *objw) {
    objw->reg3++;
    if (objw->reg3 > 4) {
        GameGbl.non_pause_flag = 0;
        if (objw->stat1 == 5) {
            objw->stat0 = 0;
        } else {
            objw->bun0 = 0;
        }
    }
}

void hr_game_puppet(OBJWORK *objw) {
    hrGameInPtTbl[objw->bun0](objw);
}