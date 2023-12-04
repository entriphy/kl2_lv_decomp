#include "nakano/herosub.h"
#include "hoshino/h_gamesnd.h"

s32 HeroRootChangeSub(OBJWORK *objw, sceVu0FVECTOR hitposi, s32 yflag) {
    s32 rtno;
    s32 kflag;
    HERO_WORK *herow;

    kflag = 0;
    herow = objw->work;

    switch (yflag) {
        case 1:
            break;
        case 2:
            rtno = MichiBunki(&objw->rtw, hitposi);
            if (rtno != -1) {
                objw->reg0 &= -0x201;
                herow->okuyuka = NULL;
                objw->rtw.rtp = GetMichiAdrs(rtno);
                objw->rtw.mcn = GetMichinori(&objw->rtw);
                kflag = 1;
                objw->reg1 = objw->reg2 = 0;
            } else {
                objw->reg1 = objw->reg2;
                if (GetMichiNo(objw->rtw.rtp) >= 0) {
                    objw->reg1 = objw->reg2 = 0;
                }
            }
            break;
        case 3:
            rtno = (s32)nkObjRouteBunki(objw); // idk man
            if (rtno != 0) {
                objw->reg0 |= 0x200;
                herow->okuyuka = (OBJWORK *)rtno;
            }
            break;
        default:
            break;
    }

    return kflag;
}

void Hero_Taiki_Sub(OBJWORK *objw, HERO_WORK *herow) {
    if (objw->reg0 & 1) {
        objw->bun0 = 13;
    } else {
        objw->bun0 = 11;
    }
}

void Hero_Shot_Sub(OBJWORK *objw, HERO_WORK *herow) {
    OBJWORK *kazd;

    kazd = GameGbl.kazd;
    if (kazd->hitram == 8) {
        if (herow->michispd == 0) {
            objw->bun0 = 11;
        } else {
            objw->bun0 = 1;
        }
    } else {
        herow->michispd = 0;
    }
}

void Hero_Shot2_Sub(OBJWORK *objw, HERO_WORK *herow) {
    OBJWORK *kazd;

    kazd = GameGbl.kazd;
    if (kazd->hitram == 8) {
        objw->bun0 = 2;
    }
}

void Hero_Run_Sub(OBJWORK *objw, HERO_WORK *herow) {
    if (objw->reg0 & 1) {
        objw->bun0 = 16;
    }
    if (objw->bun0 != (s16)herow->bun_buf) {
        herow->timer = 0;
    }
}

void Hero_Jump_Sub(OBJWORK *objw, HERO_WORK *herow) {
    objw->spd[1] += 0.5110887f;
    if (objw->spd[1] > herow->jumpspd_limit) {
        objw->spd[1] = herow->jumpspd_limit;
    }
    if (objw->bun0 == 20 || objw->bun0 == 21) {
        Hero_Todomari_Sub(objw, herow);
    } else if (!(objw->spd[1] < -1.75f)) {
        herow->spflag &= -0x11;
    }
}

void Hero_Jump_Sub2(OBJWORK *objw, HERO_WORK *herow) {
    // Empty function
}

void Hero_SJump_Sub(OBJWORK *objw, HERO_WORK *herow) {
    f32 work;

    if (herow->mispd > 0) {
        herow->mispd = 0x1000;
    } else if (herow->mispd < 0) {
        herow->mispd = -0x1000;
    }

    herow->sjump_yspd += 0.5110887f;
    if (herow->sjump_yspd > 0.0f && herow->gimm_flag & 4 && herow->timer < 35) {
        herow->sjump_yspd = 0.0f;
        work = 40 - herow->timer;
        work = work * work / 400.0f;
        herow->sjump_flag += work;
        if (objw->reg0 & 0x800) {
            herow->mochifuku->posi[1] -= work;
        } else {
            herow->mochifuku->posi[1] += work;
        }
    }

    if (objw->reg0 & 0x400) {
        herow->sjump_flag -= herow->sjump_yspd;
    } else {
        herow->sjump_flag += herow->sjump_yspd;
    }
    objw->posi[1] = herow->sjump_flag;
    objw->spd[1] = -1.0f;
    if (!(herow->sjump_yspd > 0.0f) && !(objw->reg0 & 4)) {
        return;
    }

    if (herow->mochifuku != NULL) {
        herow->mochifuku->reg2 |= 0x10;
    }
    objw->reg0 &= -2;
    herow->spflag |= 0x10;
    herow->spflag2 &= -2;
    herow->sjump_flag = 28672.0f;
    if (herow->gimm_flag & 4) {
        objw->spd[1] = -18.525f;
        herow->gimm_flag &= -5;
        herow->gimm_flag |= 8;
        hSeKeyOn(0xC80C80CA2010D, objw->posi, 0);
    } else {
        herow->wahoo_cnt++;
        if (herow->wahoo_cnt > 3) {
            herow->wahoo_cnt = 0;
        }
        herow->wahoo_timer = 0;
        objw->spd[1] = -9.75f;
    }
    objw->bun0 = 2;
}

void Hero_Fuwafuwa_Sub(OBJWORK *objw, HERO_WORK *herow) {
    if (!(objw->reg0 & 1)) {
        objw->bun0 = 2;
        herow->spflag &= -3;
    }
}

void Hero_Chaku_Sub(OBJWORK *objw, HERO_WORK *herow) {
    if (herow->count >= 24) {
        objw->bun0 = 11;
    }
}

void Hero_TShot2_Sub(OBJWORK *objw, HERO_WORK *herow) {
    if (herow->count >= 16) {
        objw->bun0 = 2;
    }
}

void Hero_TShot_Sub(OBJWORK *objw, HERO_WORK *herow) {
    if (herow->count >= 16) {
        objw->bun0 = 11;
    }
}

void Hero_TShot3_Sub(OBJWORK *objw, HERO_WORK *herow) {
    if (herow->count >= 16) {
        objw->bun0 = 1;
    }
}

void Hero_Yarare_Sub(OBJWORK *objw, HERO_WORK *herow) {
    herow->Muki = 0;
    herow->spflag2 &= -2;
    if (objw->spd[1] > herow->jumpspd_limit) {
        objw->spd[1] = herow->jumpspd_limit;
    }
    herow->mispd += objw->reg3;
    if (objw->spd[1] > 0.0f) {
        herow->spflag2 &= -5;
        if (objw->posi[1] > herow->yarare_yposi && herow->tenmetu > 0) {
            objw->bun0 = 2;
        }
    }
}

void Hero_Hajikare_Sub(OBJWORK *objw, HERO_WORK *herow) {
    herow->Muki = 0;
    herow->michispd += objw->reg3;
    if (abs(objw->reg3) <= 0x1800) {
        if (abs(objw->reg3) <= 0x600) {
            objw->bun0 = 11;
            herow->timer = 130;
            objw->reg3 = 0;
        }
    }
    if (objw->reg3 > 0) {
        objw->reg3 -= 0x200;
    } else if (objw->reg3 < 0) {
        objw->reg3 += 0x200;
    }
}

void Hero_Todomari_Sub(OBJWORK *objw, HERO_WORK *herow) {
    sceVu0FVECTOR hitposi;
    sceVu0FVECTOR hitmuki;
    s32 pata0;
    s32 pata1;
    f32 pus;
    f32 pds;

    if (objw->bun1 == 2) {
        pata0 = 20;
        pata1 = 35;
        pus = 0.8360887f;
        pds = 0.48764083f;
    } else {
        pata0 = 30;
        pata1 = 50;
        pus = 0.7060887f;
        pds = 0.5176408f;
    }

    if (objw->reg0 & 1) {
        objw->bun0 = 15;
    } else {
        objw->bun0 = 20;
    }

    if (herow->lvl & GameGbl.k_jump_button && herow->count < pata1) {
        if (herow->count >= pata0) {
            objw->spd[1] -= pus;
        } else {
            objw->spd[1] -= pds;
        }
        hitmuki[0] = hitmuki[2] = 0.0f;
        hitmuki[1] = -0.9992676f;
        hitposi[0] = objw->posi[0] + objw->muki[0] * 8.75f;
        hitposi[1] = objw->posi[1] - 28.0f;
        hitposi[2] = objw->posi[2] + objw->muki[2] * 8.75f;
        hitposi[0] = objw->posi[0] - objw->muki[0] * 8.75f;
        hitposi[1] = objw->posi[1] - objw->muki[1] * 8.75f;
    } else if (objw->bun0 != 15) {
        objw->bun0 = 2;
    }

    if (herow->michispd > 0) {
        herow->michispd = herow->mispd_buf = 4096;
    }
    if (herow->michispd < 0) {
        herow->michispd = herow->mispd_buf = -4096;
    }
}

void Hero_DoorJump_Sub(OBJWORK *objw, HERO_WORK *herow) {
    herow->Muki = 0;
    if (objw->spd[1] > herow->jumpspd_limit) {
        objw->spd[1] = herow->jumpspd_limit;
    }
    herow->michispd += objw->reg3;
}

void Hero_DoorStop_Sub(OBJWORK *objw, HERO_WORK *herow) {
    if (herow->count >= 150) {
        objw->bun0 = 11;
    }
    herow->timer = 130;
}

void Hero_GateInWalk_Sub(OBJWORK *objw, HERO_WORK *herow) {
    // Empty function
}

void Hero_GateOutWalk_Sub(OBJWORK *objw, HERO_WORK *herow) {
    // Empty function
}

void Hero_FireSi_Sub(OBJWORK *objw, HERO_WORK *herow) {
    // Empty function
}
