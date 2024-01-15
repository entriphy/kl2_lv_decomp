#include "nakano/objklo1.h"
#include "nakano/k_math.h"
#include "nakano/vu0mac.h"
#include "nakano/klmot.h"
#include "nakano/klohit0.h"
#include "nakano/herosub.h"
#include "nakano/objkazd.h"
#include "nakano/objklo0.h"
#include "nakano/main.h"
#include "nakano/gmiss.h"
#include "nakano/route.h"
#include "harada/hr_pall.h"
#include "hoshino/h_gamesnd.h"
#include "taka/tk_effect.h"

void DrawKloSide(OBJWORK *objw) {
    sceVu0FVECTOR rot;

    if (hr_pt_check() == 1) {
        rot[1] = _atan2(objw->muki[0], objw->muki[2]) + 3.141592f;
        rot[1] = nkRadMask(rot[1]);
        objw->rot[1] = rot[1];
        objw->rot[1] = nkRadMask(objw->rot[1]);
    } else {
        nkSetKlonoaShadow(objw);
        DrawSfxKlonoa_SS(objw);
    }
}

void KloSlideSide(OBJWORK *objw) {
    HERO_WORK *herow;
    s32 spd;
    s32 spdlimit;
    f32 sa;
    sceVu0FVECTOR rot;
    s32 trg2;
    f32 spd_sc;
    sceVu0FVECTOR slant;
    sceVu0FVECTOR vf0;
    sceVu0FVECTOR vf1;
    s32 pitch;
    u32 efmat;
    f32 var_f0;

    spd_sc = 0.0f;
    herow = objw->work;
    herow->gimm_flag &= -0x1000001;
    if (hr_pt_check() == 1) {
        if (herow->se_id != -1) {
            hSeKeyOff(herow->se_id);
            herow->se_id = -1;
        }
        if (herow->se_id2 != -1) {
            hSeKeyOff(herow->se_id2);
            herow->se_id2 = -1;
        }
        // why not 3 lol
        if (herow->se_id4 != -1) {
            hSeKeyOff(herow->se_id4);
            herow->se_id4 = -1;
        }
        if (herow->se_id5 != -1) {
            hSeKeyOff(herow->se_id5);
            herow->se_id5 = -1;
        }
        if (herow->se_id6 != -1) {
            hSeKeyOff(herow->se_id6);
            herow->se_id6 = -1;
        }
        if (herow->se_id7 != -1) {
            hSeKeyOff(herow->se_id7);
            herow->se_id7 = -1;
        }

        return;
    }

    if (herow->gimm_flag & 0x1000) {
        herow->lvl = herow->ex_lvl;
        herow->trg = herow->ex_trg;
        if (!GameGbl.playdemo_flag) {
            objw->hitram = 0;
        }
    } else {
        herow->lvl = GameGbl.kpd[0].lvl;
        herow->trg = GameGbl.kpd[0].trg;
        if (herow->area_cnt < 2) {
            herow->lvl = 0;
            herow->trg = 0;
        }
    }

    if (GameGbl.wipe.intime > 0 && GameGbl.wipe.outtime > 0) {
        herow->lvl = 0;
        herow->trg = 0;
    }

    if (GameGbl.kloLife[herow->con] <= 0) {
        if (objw->bun0 == 34) {
            Hero_LifeLost_Sub(objw, herow);
        }
        objw->hitram = 0;
        herow->lvl = 0;
        herow->trg = 0;
    }

    if (objw->bun0 == 12) {
        Hero_Dead_Sub(objw, herow);
        if (herow->yarare_bun == 2) {
            herow->mispd_buf = 0;
            herow->mispd = 0;
        }
    }

    if (pPAD_TRG_L1(herow)) {
        herow->b_action = 1;
        herow->b_act_time = 0;
    }
    if (pPAD_TRG_L2(herow)) {
        herow->b_action = 2;
        herow->b_act_time = 0;
    }

    nkDG.hitmap_flag = 0;

    sceVu0CopyVector(vf0, objw->posi);
    vf0[1] -= 16.0f;
    vf1[0] = 0.0f;
    vf1[1] = 1.0f;
    vf1[2] = 0.0f;
    sbRayCrsJudgzN(slant, vf0, vf1, 32.0f);
    sceVu0CopyVector(herow->slant, slant);

    if (herow->se_id6 == -1) {
        herow->se_id6 = hSeKeyOn(0x4b04b00da1a100, objw->posi, 0);
    } else {
        sceVu0FVECTOR vf;
        vu0_Square(vf, objw->spd);
        spd_sc = vu0_SquareRoot(vf[0] + vf[2]);
        pitch = spd_sc * 50.0f;
        hSePitch(herow->se_id6, pitch);
    }

    if (herow->se_id7 == -1) {
        switch (GameGbl.vision >> 8) {
            case 5:
                herow->se_id7 = hSeKeyOn(0xc80c80da1a103, objw->posi, 0);
                break;
            case 10:
                herow->se_id7 = hSeKeyOn(0xc80c80da1a102, objw->posi, 0);
                break;
            case 14:
                herow->se_id7 = hSeKeyOn(0xc80c80da1a101, objw->posi, 0);
                break;
            default:
                herow->se_id7 = hSeKeyOn(0xc80c80da1a102, objw->posi, 0);
                break;
        }
    } else {
        f32 vol;
        sceVu0FVECTOR vf;
        vu0_Square(vf, objw->spd);
        spd_sc = vu0_SquareRoot(vf[0] + vf[2]);
        pitch = (spd_sc * 50.0f - 360.0f) / 220.0f * 400.0f - 200.0f;
        vol = 0.5f + (spd_sc * 50.0f - 360.0f) / 220.0f * 0.5f;
        if (herow->yuka_hit == 0 || !(objw->spd[1] >= 0.0f) || herow->f_phcode == 0xFFFF) {
            vol = 0.0f;
        }
        hSePitch(herow->se_id7, pitch);
        hSeSetObjVol(herow->se_id7, vol);
    }

    if (herow->yuka_hit != 0 && objw->spd[1] >= 0.0f && GameGbl.vision >> 8 != 1) {
        sceVu0FVECTOR efnor;
        sceVu0CopyVector(efnor, slant);
        efnor[0] *= -spd_sc * 0.5f;
        efnor[1] *= -spd_sc * 0.5f;
        efnor[2] *= -spd_sc * 0.5f;
        efnor[3] = 1.0f;

        if (herow->f_phcode == 0xFFFF) {
            if (!(SysGbl.irqc & 3)) {
                abSetBoardEffectA(objw->posi, efnor, 3);
            }
        } else {
            if (GameGbl.vision >> 8 == 5) {
                sceVu0CopyVector(EffectParamObj.waterPos, objw->posi);
                sceVu0CopyVector(EffectParamObj.waterDirec, objw->spd);
                func_002FCB60(22);
                func_002FCB60(23);
            } else {
                switch (GameGbl.vision >> 8) {
                    case 5:
                        efmat = 1;
                        break;
                    case 10:
                        efmat = 0;
                        break;
                    case 14:
                        efmat = 2;
                        break;
                    default:
                        efmat = 0;
                        break;
                }

                if (!(SysGbl.irqc & 3)) {
                    abSetBoardEffectA(objw->posi, efnor, efmat);
                }
            }

        }
    }

    if (herow->da_muteki > 0) {
        herow->da_muteki--;
    }
    if (herow->tenmetu > 0) {
        herow->tenmetu--;
    }

    if (objw->hitram & 3) {
        if (herow->da_muteki <= 0) {
            objw->bun0 = 10;
            herow->da_muteki = 120;
            herow->tenmetu = 120;
            herow->count = 0;
            hSeKeyOn(0xc80c80ca20107, objw->posi, 0);
            if (GameGbl.kloLife[herow->con] > 0) {
                GameGbl.kloLife[herow->con]--;
            }
        }
        objw->hitram &= -4;
    }

    if (objw->bun0 == 10 && herow->count > 91) {
        objw->bun0 = 11;
    }

    if (objw->hitram & 0x300) {
        if (herow->da_muteki <= 0) {
            herow->tenmetu = herow->da_muteki;
            hSeKeyOn(0xc80c80ca20107, objw->posi, 0);
            if (GameGbl.kloLife[herow->con] > 0) {
                GameGbl.kloLife[herow->con] = 0;
            }
        }
        objw->hitram &= -0x301;
    }

    if (objw->hitram & 0xC) {
        if (herow->da_muteki <= 0) {
            herow->tenmetu = 0;
            herow->da_muteki = 30;
            hSeKeyOn(0xc80c80ca20106, objw->posi, 0);
        }
        objw->hitram &= -0xD;
    }

    if (herow->gimm_flag & 0x100) {
        if (herow->se_id4 != -1) {
            hSeKeyOff(herow->se_id4);
            herow->se_id4 = -1;
        }
        herow->rakka_flag &= -2;
        if (herow->se_id3 == -1) {
            herow->se_id3 = hSeKeyOn(0x4b04b00da00804, objw->posi, 0);
        }
        herow->gimm_flag &= -0x101;

        switch (rand() % 4) {
            case 0:
                hSeKeyOn(0xc80c80ca20112, objw->posi, 0);
                break;
            case 1:
                hSeKeyOn(0xc80c80ca20113, objw->posi, 0);
                break;
            case 2:
                hSeKeyOn(0xc80c80ca20111, objw->posi, 0);
                break;
            case 3:
                hSeKeyOn(0x4b04b00ca2a002, objw->posi, 0);
                break;
        }
    } else {
        if (objw->spd[1] > 0.0f && herow->se_id3 != -1) {
            hSeKeyOff(herow->se_id3);
            herow->se_id3 = -1;
        }
    }

    if (herow->gimm_flag & 1) {
        objw->bun0 = 2;
        if (herow->gimm_flag & 0x10) {
            herow->timer = 0;
            herow->gimm_flag &= -0x11;
            herow->gimm_flag |= 0x40;
            if (herow->se_id != -1) {
                hSeKeyOff(herow->se_id);
                herow->se_id = -1;
            }
            if (herow->se_id2 != -1) {
                hSeKeyOff(herow->se_id2);
                herow->se_id2 = -1;
            }
        }
        if (herow->gimm_flag & 0x40 && herow->timer < 10) {
            herow->Scale[0] = herow->Scale[1] = herow->Scale[2] = (10.0f - herow->timer) / 10.0f;
        }
        if (herow->gimm_flag & 0x20) {
            herow->se_id = hSeKeyOn(0x4b04b00da00800, objw->posi, 0);
            hSeKeyOn(0xc80c80ca2010c, objw->posi, 0);
            herow->timer = 0;
            herow->gimm_flag &= -0x21;
            herow->gimm_flag &= -0x41;
            herow->gimm_flag |= 0x80;
        }
        if (herow->gimm_flag & 0x80) {
            if (herow->timer < 16) {
                herow->Scale[0] = herow->Scale[1] = herow->Scale[2] = herow->timer / 16.0f;
            } else {
                herow->gimm_flag &= -0x81;
                herow->Scale[0] = herow->Scale[1] = herow->Scale[2] = 1.0f;
            }
        }

        herow->timer++;
        return;
    }

    if (herow->se_id != -1) {
        hSeKeyOff(herow->se_id);
        herow->se_id = -1;
    }
    if (herow->se_id2 != -1) {
        hSeKeyOff(herow->se_id2);
        herow->se_id2 = -1;
    }

    herow->Muki = 0;
    herow->buki_muki = 0;
    GetMichiVec(&objw->rtw, objw->muki);
    sa = objw->muki[1];
    var_f0 = GetVectorLenXZ(objw->muki);
    spd = 0.5110887f * sa * 4096.0f;
    spd /= 16.0f;
    rot[0] = -_atan2(var_f0, sa) + 1.570796f;
    if (rot[0] < -3.141592f) {
        rot[0] += 6.283184f;
    } else if (rot[0] > 3.141592f) {
        rot[0] -= 6.283184f;
    }
    rot[0] -= objw->rot[0];
    if (rot[0] < -3.141592f) {
        rot[0] += 6.283184f;
    } else if (rot[0] > 3.141592f) {
        rot[0] -= 6.283184f;
    }
    objw->rot[0] += rot[0] / 16.0f;
    if (objw->rot[0] < -3.141592f) {
        objw->rot[0] += 6.283184f;
    } else if (objw->rot[0] > 3.141592f) {
        objw->rot[0] -= 6.283184f;
    }
    spdlimit = (s32)(sa * 2.0f * 4096.0f) + 0x3000;
    if (spdlimit > 0x4000) {
        spdlimit = 0x4000;
    }

    if (objw->bun0 != 20) {
        if (objw->spd[1] != 0.0f) {
            if (pPAD_LVL_RIGHT(herow)) {
                herow->mispd_buf += 0x500;
                if (herow->mispd_buf > 0x6800) {
                    herow->mispd_buf = 0x6800;
                }
                if (objw->bun0 != 10 && objw->bun0 != 35) {
                    herow->buki_muki = 0;
                }
            } else if (pPAD_LVL_LEFT(herow) || objw->bun0 == 10) {
                herow->mispd_buf -= 0x400;
                if (herow->mispd_buf < 0) {
                    herow->mispd_buf = 0;
                }
                if (objw->bun0 != 10 && objw->bun0 != 35) {
                    herow->buki_muki = 1;
                }
            } else {
                if (herow->mispd_buf > 0x4000) {
                    herow->mispd_buf -= 0x80;
                }
                if (herow->mispd_buf < 0x4000) {
                    herow->mispd_buf += 0x100;
                }
            }
        } else {
            if (pPAD_LVL_RIGHT(herow)) {
                herow->mispd_buf += 0x500;
                if (herow->mispd_buf > 0x6800) {
                    herow->mispd_buf = 0x6800;
                }
                if (objw->bun0 != 10 && objw->bun0 != 35) {
                    herow->buki_muki = 0;
                }
            } else if (pPAD_LVL_LEFT(herow) || objw->bun0 == 10) {
                herow->mispd_buf -= 0x200;
                if (herow->mispd_buf < 0x2000) {
                    herow->mispd_buf = 0x2000;
                }
                if (objw->bun0 != 10 && objw->bun0 != 35) {
                    herow->buki_muki = 1;
                }
            } else {
                if (herow->mispd_buf > 0x4000) {
                    herow->mispd_buf -= 0x80;
                }
                if (herow->mispd_buf < 0x4000) {
                    herow->mispd_buf += 0x100;
                }
            }
        }

        if (herow->mispd < 0x3000) {
            herow->mispd = 0x3000;
        }
    }

    if (herow->mispd > spdlimit) {
        herow->mispd = spdlimit;
    }

    switch (herow->Muki) {
        case 0:
            objw->reg0 &= -0x11;
            break;
        case 1:
            herow->buki_muki = 0;
            break;
        case 2:
            herow->buki_muki = 0;
            break;
    }

    if (herow->trg & GameGbl.k_jump_button && objw->spd[1] == 0.0f) {
        herow->count = 0;
        hSeKeyOn(0x4b04b00ca0a206, objw->posi, 0);
        objw->spd[1] = -9.75f;
        if (objw->bun0 != 40) {
            objw->bun0 = objw->bun0 == 6 ? 7 : 2;
        }
        trg2 = 0;
    } else {
        trg2 = herow->trg;
    }

    if (herow->lvl & GameGbl.k_jump_button && objw->bun0 != 40 && !(herow->spflag2 & 1) && objw->spd[1] > 2.0f && !(objw->reg0 & 1)) {
        herow->se_id9 = hSeKeyOn(0xc80c80da10004, objw->posi, 0);
        objw->bun0 = 20;
        herow->spflag2 |= 1;
        objw->spd[1] = 1.0483398f;
        herow->count = 0;
    }
    if (herow->se_id9 != -1 && objw->bun0 != 20) {
        hSeKeyOff(herow->se_id9);
        herow->se_id9 = -1;
    }

    Hero_Jump_Sub(objw, herow);

    if (objw->spd[1] < 0.0f || objw->spd[1] > 0.6f || objw->reg0 & 0x100 || objw->bun0 == 40) {
        if (objw->reg0 & 1) {
            if (objw->bun0 != 10 || !(objw->spd[1] < 0.0f)) {
                if (herow->trg & GameGbl.k_atck_button && !(herow->gimm_flag & 0x10000)) {
                    herow->spflag &= -2;
                    objw->bun0 = 9;
                    herow->mochifuku->reg2 |= 0x40;
                    herow->timer = 0;
                    herow->count = 0;
                    hSeKeyOn(0xc80c80ca20104, objw->posi, 0);
                } else if ((trg2 & GameGbl.k_jump_button || objw->reg0 & 0x20) && !(herow->spflag & 1) && !(objw->reg0 & 8) && !(herow->gimm_flag & 0x8000)) {
                    objw->bun0 = 27;
                    herow->sjump_flag = objw->posi[1];
                    SayWahoo(objw, herow);
                    objw->reg0 &= -0x125;
                    objw->spd[1] = -1.0f;
                    herow->sjump_yspd = -7.2087765f;
                    herow->timer = 0;
                    herow->count = 0;
                } else if (objw->bun0 == 2) {
                    objw->bun0 = 15;
                    herow->count = 0;
                }
            }

        } else {
            if (objw->bun0 == 15) {
                objw->bun0 = 2;
            }
            if ((objw->bun0 != 10 || !(objw->spd[1] < 0.0f)) && objw->bun0 != 7 && objw->bun0 != 27 && objw->bun0 != 40 && herow->trg & GameGbl.k_atck_button && !(herow->gimm_flag & 0x2000)) {
                SetKazd(objw);
                objw->bun0 = 7;
                herow->timer = 0;
            }
        }

        if (objw->bun0 == 27) {
            Hero_SJump_Sub(objw, herow);
        }
    } else if (objw->bun0 != 6 && objw->bun0 != 35 && objw->bun0 != 40 && herow->trg & GameGbl.k_atck_button && !(herow->gimm_flag & 0x2000)) {
        herow->timer = 0;
        if (!(objw->reg0 & 1)) {
            SetKazd(objw);
            objw->bun0 = 6;
        } else {
            hSeKeyOn(0xc80c80ca20104, objw->posi, 0);
            herow->count = 0;
            herow->mochifuku->reg2 |= 0x40;
        }
    }

    if (objw->bun0 == 6) {
        Hero_Shot_Sub(objw, herow);
    }
    if (objw->bun0 == 7) {
        Hero_Shot2_Sub(objw, herow);
    }
    if (objw->bun0 == 20) {
        if (pPAD_LVL_RIGHT(herow)) {
            herow->buki_muki = 0;
            spd = 0;
            herow->mispd_buf = 0x200;
            if (herow->mispd > 0x3800) {
                herow->mispd = 0x3800;
            }
        } else if (pPAD_LVL_LEFT(herow)) {
            herow->buki_muki = 1;
            spd = 0;
            herow->mispd_buf = -0x200;
            if (herow->mispd < 0x800) {
                herow->mispd = 0x800;
            }
        } else {
            herow->buki_muki = 0;
            spd = 0;
            if (herow->mispd > 0x2000) {
                herow->mispd_buf = -0x100;
            }
            if (herow->mispd < 0x2000) {
                herow->mispd_buf = 0x100;
            }
        }

        if (herow->count >= 25) {
            herow->mispd_buf = spd;
            herow->mispd = 0x4000;
        }
    }

    if ((herow->buki_muki == 0 && herow->mispd < 0) || (herow->buki_muki == 1 && herow->mispd > 0)) {
        objw->ang[0] = -objw->muki[0];
        objw->ang[2] = -objw->muki[2];
    } else {
        objw->ang[0] = objw->muki[0];
        objw->ang[2] = objw->muki[2];
    }
    objw->ang[1] = 0.0f;

    if (herow->mispd != 0) {
        if ((objw->reg0 & 3) == 3) {
            herow->mispd_buf = 0;
            herow->mispd = 0;
            spd = 0;
        } else {
            HeroKabeCheck3(objw, herow);
            if (herow->mispd == 0) {
                spd = 0;
                herow->mispd_buf = 0;
            }
        }
    }

    if (objw->reg6 != 0) {
        MichiMove(&objw->rtw, objw->reg6);
    }
    objw->reg6 = 0;
    objw->posi[1] += objw->spd[1];
    if (objw->spd[1] > 0.0f) {
        Hero_JumpDown_Sub(objw, herow);
    } else if (objw->spd[1] < 0.0f) {
        TenjoHit(objw, herow);
    }
    if (objw->spd[1] == 0.0) {
        herow->mispd += spd;
    }
    herow->mispd += herow->mispd_buf;
    if (objw->spd[1] <= 0.0f) {
        herow->rakka_cnt = 0;
    }

    if (!herow->yuka_hit_old && herow->yuka_hit) {
        s32 obj = -1;
        switch (GameGbl.vision >> 8) {
            case 5:
                if (herow->f_phcode != 0xFFFF) {
                    obj = hSeKeyOn(0x4b04b00ca0a207, objw->posi, 0);
                }
                break;
            case 10:
                obj = hSeKeyOn(0x4b04b00ca0a208, objw->posi, 0);
                break;
            case 14:
                obj = hSeKeyOn(0x4b04b00ca0a209, objw->posi, 0);
                break;
        }

        if (obj != -1) {
            sceVu0FVECTOR vf;
            f32 var_f12;
            vu0_Square(vf, objw->spd);
            var_f12 = vu0_SquareRoot(vf[0] + vf[2]) * 50.0f;
            var_f12 = 0.7f + (var_f12 - 360.0f) / 220.0f * 0.3f;
            hSeSetObjVol(obj, var_f12);
        }
    }

    herow->yuka_hit_old = herow->yuka_hit;
    if (objw->bun0 == 12 && herow->yarare_bun == 2) {
        herow->mispd /= 4;
    }

    {
        sceVu0FVECTOR vf;

        MichiMove(&objw->rtw, herow->mispd);
        sceVu0CopyVector(vf, objw->posi);
        MichiPosiXZ(&objw->rtw, objw->posi);
        objw->spd[0] = objw->posi[0] - vf[0];
        objw->spd[2] = objw->posi[2] - vf[2];
        vf[0] = objw->spd[0];
        vf[2] = objw->spd[2];
        vf[3] = vu0_SquareRoot(vf[0] * vf[0] + vf[2] * vf[2]);
    }

    herow->count++;
    herow->dead_cnt++;
    herow->area_cnt++;
    herow->jmp_cnt++;
    herow->rakka_cnt++;
    objw->rtw.mcn = GetMichinori(&objw->rtw);
}