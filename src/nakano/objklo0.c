#include "nakano/objklo0.h"
#include "nakano/klohit0.h"
#include "nakano/objkazd.h"
#include "nakano/herosub.h"
#include "nakano/vu0mac.h"
#include "nakano/gmiss.h"
#include "nakano/main.h"
#include "nakano/nkgf.h"
#include "nakano/dma.h"
#include "nakano/klmot.h"
#include "nakano/k_math.h"
#include "nakano/route.h"
#include "nakano/klopipe.h"
#include "nakano/camera.h"
#include "harada/hr_main.h"
#include "kazuya/disp.h"
#include "harada/hr_pall.h"
#include "okanoyo/okio.h"
#include "hoshino/h_gamesnd.h"
#include "hoshino/h_game.h"
#include "okanoyo/okmain.h"


void Init_ReTry() {
    OBJWORK *klo = GameGbl.klonoa;

    GameGbl.flag |= 8;
    GameGbl.kloLife[0] = 3;
    klo->drmir = NULL;
    klo->draw = NULL;
    klo->bun0 = 0;
    klo->bun1 = 0;
    klo->reg0 &= -0x401;
    nkGfInit();
    hInitStage1();
    hr_retry_set();
    htRetryBossSet();
    OkReTryInit();
    if (GameGbl.vision >> 8 < 30) {
        nkSetRtConn(GameGbl.klonoa);
    }
    GameGbl.kloZanki[0] = kzLoadRemain();
    GameGbl.stage_flag = 0;
    GameGbl.Re_st_rtn = -1;
    GameGbl.Re_st_mcn = 0;
    GameGbl.Re_st_klomode = 0;
    GameGbl.Re_st_height = 0.0f;
    GameGbl.filter_cnt = 0.0f;
    GameGbl.filter_type = 1;
    GameGbl.reverse_cnt = 0.0f;
    GameGbl.time_atack_cnt = 0;
    GameGbl.vs_clear_cnt = 0;
    GameGbl.fcnt0 = 0.0f;
    GameGbl.fcnt1 = 0.0f;
    GameGbl.fcnt2 = 0.0f;
}

void Init_Restart() {
    OBJWORK *klo = GameGbl.klonoa;
    s32 vision = GameGbl.vision >> 8;

    GameGbl.kloLife[0] = 3;
    if (vision == 18 || vision == 19) {
        GameGbl.kloZanki[0] = 1;
    }
    if (GameGbl.kloZanki[0] < 0) {
        kzSetDispMaskOn();
        kzReturnVOver();
        nkPathBlock();
        TkRemoveAllEffect();
        hSndReset();
    } else {
        GameGbl.init_vision = GameGbl.Re_st_vision;
        klo->drmir = NULL;
        klo->draw = NULL;
        klo->bun0 = 0;
        klo->bun1 = 0;
        klo->reg0 &= -0x401;
        nkGfReStartInit();
        OkReStartJob();
        hPopRestart();
        hr_restart_Tmpclear();
        nkEngSysReStart();
        if (GameGbl.Re_st_rtn != -1) {
            GameGbl.st_rtn = GameGbl.Re_st_rtn;
            GameGbl.st_muki = GameGbl.Re_st_muki;
            GameGbl.st_mcn = GameGbl.Re_st_mcn;
            GameGbl.st_height = GameGbl.Re_st_height;
            GameGbl.st_klomode = GameGbl.Re_st_klomode;
        }
        GameGbl.flag |= 0x10;
    }
}

static void DrawKloMir(OBJWORK *objw) {
    DrawMirSfxKlonoa(objw);
}

static void DrawKlonoa(OBJWORK *objw) {
    HERO_WORK *herow = objw->work;

    if (objw->reg0 & 0x200) {
        GetMichiNo(herow->okuyuka->rtw.rtp);
    } else {
        GetMichiNo(objw->rtw.rtp);
    }

    if (hr_pt_check() != 1 && GameGbl.vision >> 8 < 30 && objw->bun0 != 12) {
        nkSetKlonoaShadow(objw);
        DrawSfxKlonoa(objw);
    }
}

void InitKlonoa(OBJWORK *objw, s32 imuki) {
    HERO_WORK *herow = objw->work;

    objw->draw = DrawKlonoa;
    objw->drmir = DrawKloMir;
    herow->miexspd = 0;
    objw->reg0 &= 0x401;
    herow->jumpspd_limit = 8.0f;
    herow->da_muteki = 0;
    herow->tenmetu = 0;
    objw->bun0 = 11;
    objw->rot[0] = objw->rot[2] = 0.0f;

    if (objw->bun1 == 2) {
        herow->mispd = 0x2800;
        objw->draw = NULL; // TODO: DrawKloSide
        objw->rot[1] = _atan2(objw->muki[0], objw->muki[2]) + 3.141592f;
    } else if (objw->bun1 == 3 || objw->bun1 == 4) {
        if (GameGbl.vision == 0x102) {
            GetMichiVec(&objw->rtw, objw->muki);
        }
        objw->rot[1] = -_atan2(objw->muki[0], objw->muki[2]);
        if (objw->bun1 == 4) {
            nkKpInitJyuryoku();
        }
        objw->draw = NULL; // TODO: DrawKloBhd
        herow->jumpspd_limit = 20.0f;
        switch (imuki) {
            case 3:
                herow->right_move = 0x2000;
                herow->left_move = 0x8000;
                herow->up_move = 0x1000;
                herow->down_move = 0x4000;
                break;
            case 4:
                herow->right_move = 0x8000;
                herow->left_move = 0x2000;
                herow->up_move = 0x4000;
                herow->down_move = 0x1000;
                break;
            default:
                herow->right_move = 0x2000;
                herow->left_move = 0x8000;
                herow->up_move = 0x1000;
                herow->down_move = 0x4000;
                break;
        }
    }
    GameGbl.st_flag = 0;
}

static void SetKlonoaMode(s32 mode, s32 imuki) {
    OBJWORK *objw = GameGbl.klonoa;
    objw->bun1 = mode;
    InitKlonoa(objw, imuki);
}

static void ObjInit(OBJWORK *objw) {
    HERO_WORK *herow;

    if (GameGbl.flag & 4) {
        return;
    }

    objw->prty = 0;
    objw->draw = DrawKlonoa;
    objw->drmir = DrawKloMir;
    objw->dreff = NULL;
    objw->drmiref = NULL;
    objw->spd[0] = objw->spd[1] = objw->spd[2] = 0.0f;

    objw->work = getmem(sizeof(HERO_WORK));
    herow = objw->work;
    memset(herow, 0, sizeof(HERO_WORK));
    herow->yarare_bun = 0;
    herow->left_move = 0;
    herow->right_move = 0;
    herow->up_move = 0;
    herow->down_move = 0;
    herow->b_action = 0;
    herow->se_id = -1;
    herow->se_id2 = -1;
    herow->se_id3 = -1;
    herow->se_id4 = -1;
    herow->se_id5 = -1;
    herow->se_id6 = -1;
    herow->se_id7 = -1;
    herow->se_id8 = -1;
    herow->se_id9 = -1;
    herow->kasokuLvL = 0;
    herow->kasokuTime = 0.0f;
    herow->kasokuMTime = 0.0f;
    herow->center_time = 0.0f;
    herow->yuka_hit_old = 2;
    herow->yuka_hit_buf = 0;
    herow->dai_se_cnt = 0;
    herow->b_act_rflag = 0;
    herow->b_act_rot = 0.0f;
    herow->okuyuka = NULL;
    herow->wahoo_cnt = 0;
    herow->wahoo_timer = 0;
    herow->rupu_cnt = 0;
    herow->dead_cnt = 0;
    herow->hima_cnt = 0;
    herow->taiho_bun = 0;
    herow->area_cnt = 0;
    herow->rakka_cnt = 0;
    herow->slant[0] = 0.0f;
    herow->slant[1] = 0.0f;
    herow->slant[2] = 0.0f;
    herow->slant[3] = 1.0f;
    if (GameGbl.playdemo_flag && GameGbl.vision == 0x501) {
        GameGbl.st_mcn = 0x59000;
    }
    objw->rtw.rtp = GetMichiAdrs(GameGbl.st_rtn);
    SetMichinori(&objw->rtw, GameGbl.st_mcn);
    objw->reg0 &= 0x400;
    herow->Scale[0] = 1.0f;
    herow->Scale[1] = 1.0f;
    herow->Scale[2] = 1.0f;
    herow->Scale[3] = 0.0f;
    InitKloModel(objw);
    objw->hitram = 0;
    herow->Muki = 0;
    herow->miexspd = 0;
    herow->gimspd = 0;
    herow->rakka_flag = 0;

    MichiPosi(&objw->rtw, objw->posi);
    if (GameGbl.Re_st_rtn != -1 || GameGbl.st_flag & 8) {
        objw->posi[1] = GameGbl.st_height;
    }
    GetMichiVec(&objw->rtw, objw->muki);
    switch (GameGbl.st_muki) {
        case 1:
            herow->buki_muki = 0;
            break;
        case 2:
            herow->buki_muki = 1;
            objw->muki[0] = -objw->muki[0];
            objw->muki[1] = -objw->muki[1];
            objw->muki[2] = -objw->muki[2];
            herow->Muki = 0;
            break;
        case 4:
            herow->buki_muki = 2;
            herow->Muki = 1;
            break;
        case 3:
            herow->buki_muki = 3;
            herow->Muki = 2;
            break;
    }

    if (hr_pt_check() != 1) {
        CamVWorkInit(objw, &GameGbl.cam);
    }

    if (GameGbl.klonoa == objw) {
        herow->con = 0;
    } else {
        herow->con = 1;
    }

    objw->reg3 = 0;
    herow->gimm_flag = 0;
    herow->norigimm = NULL;
    herow->movebox = NULL;
    herow->mochifuku = NULL;
    herow->jumpspd_limit = 8.0f;
    objw->bun0 = 11;
    objw->bun1 = GameGbl.st_klomode;
    if (objw->bun1 == 0) {
        objw->bun1++;
    }
    InitKlonoa(objw, GameGbl.st_muki);
    objw->freg3 = 0.0f;
}

static void InitDamageJump(OBJWORK *objw, HERO_WORK *herow) {
    herow->da_muteki = 60;
    if ((objw->bun0 != 40 && objw->bun0 != 14) || GameGbl.kloLife[herow->con] <= 1) {
        objw->bun0 = 10;
        herow->yarare_yposi = objw->posi[1];
        if (objw->hitram & 0x145) {
            objw->reg3 = -0x3000;
            herow->buki_muki = 0;
        } else {
            objw->reg3 = 0x3000;
            herow->buki_muki = 1;
        }
        herow->michispd = objw->reg3;
        objw->spd[1] = -6.5f;
    }
}

void SetKlonoaPadData(s32 lvl, s32 trg, s32 con) {
    HERO_WORK *herow = GameGbl.klonoa->work;
    herow->ex_lvl = lvl;
    herow->ex_trg = trg;
}

void PtKlonoaJump(f32 spd, s32 flag) {
    OBJWORK *objw = GameGbl.klonoa;

    objw->spd[1] = spd;
    if (flag == 1) {
        hSeKeyOn(0x4b04b00ca10001, objw->posi, 0);
    }
    if (flag == 2) {
        hSeKeyOn(0xc80c80ca20102, objw->posi, 0);
    }
}

void SayWahoo(OBJWORK *objw, HERO_WORK *herow) {
    switch (herow->wahoo_cnt) {
        case 0:
            hSeKeyOn(0xc80c80ca20102, objw->posi, 0);
            break;
        case 1:
            hSeKeyOn(0xc80c80ca20112, objw->posi, 0);
            break;
        case 2:
            hSeKeyOn(0xc80c80ca20111, objw->posi, 0);
            break;
        case 3:
            hSeKeyOn(0xc80c80ca20113, objw->posi, 0);
            break;
    }
}

static void ObjMain(OBJWORK *objw) {
    HERO_WORK *herow;
    f32 wk0;
    f32 wk1;
    s32 work;
    s32 sw0;
    s32 sw1;
    s32 trg2 = 0;
    s32 klPadRight;
    s32 klPadLeft;
    void (*Hero_Kodo[44])(OBJWORK *objw, HERO_WORK *herow) = {
        NULL,
        Hero_Run_Sub,
        Hero_Jump_Sub2,
        Hero_Chaku_Sub,
        NULL,
        NULL,
        Hero_Shot_Sub,
        Hero_Shot2_Sub,
        Hero_TShot_Sub,
        Hero_TShot2_Sub,
        Hero_Yarare_Sub,
        Hero_Taiki_Sub,
        Hero_Dead_Sub,
        NULL,
        NULL, // TODO: Hero_Hakobare_OkSub
        NULL,
        NULL,
        NULL,
        Hero_TShot3_Sub,
        Hero_Fuwafuwa_Sub,
        NULL,
        NULL,
        Hero_Run_Sub,
        NULL,
        NULL,
        NULL,
        NULL,
        Hero_SJump_Sub,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        Hero_LifeLost_Sub,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL, // TODO: Hero_TumujiJump_OkSub
        NULL,
        NULL
    };
    s32 rtcnt;
    s32 pitch;
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;
    f32 dy;

    herow = objw->work;
    herow->gimm_flag &= -0x1000001;

    if (hr_pt_check() == 1) {
        if (herow->se_id4 != -1) {
            hSeKeyOff(herow->se_id4);
            herow->se_id4 = -1;
        }
        if (herow->se_id5 != -1) {
            hSeKeyOff(herow->se_id5);
            herow->se_id5 = -1;
        }
        objw->hitram = 0;
        objw->spd[1] = 0.0f;
        herow->mispd = 0;
        herow->michispd = 0;
        herow->mispd_buf = 0;
        return;
    }

    if (GameGbl.vision >> 8 >= 30) {
        return;
    }

    sw0 = 0;
    work = GetMichiNo(objw->rtw.rtp);
    rtcnt = objw->rtw.mcn >> 12;
    if (GameGbl.vision == 0x0202 && work == 0 && rtcnt > 0x940 && rtcnt < 0xF809)  {
        sw0 = 1;
    }
    if (GameGbl.vision == 0x0C02 && work == 0 && rtcnt > 0x940 && rtcnt < 0xFB0)  {
        sw0 = 1;
    }
    if (GameGbl.vision == 0x0F00 && work == 1 && rtcnt > 0x0 && rtcnt < 0x500)  {
        sw0 = 1;
    }
    if (GameGbl.vision == 0x0600 && work == 4 && rtcnt > 0x680 && rtcnt < 0x860)  {
        sw0 = 1;
    }
    if ((GameGbl.vision == 0x0904 || GameGbl.vision == 0x0F04) && work == 2 && rtcnt > 0xB80 && rtcnt < 0xCD0)  {
        sw0 = 1;
    }
    if ((GameGbl.vision == 0x0702 || GameGbl.vision == 0x0802) && work == 4 && rtcnt > 0x0 && rtcnt < 0x290)  {
        sw0 = 1;
    }
    if ((GameGbl.vision == 0x0703 || GameGbl.vision == 0x0803) && work == 1 && rtcnt > 0x0 && rtcnt < 0xA20)  {
        sw0 = 1;
    }

    if (sw0 == 1) {
        herow->jumpspd_limit = 12.0f;
        herow->gimm_flag |= 0x20000;
    } else if (sw0 == 2) {
        herow->jumpspd_limit = 20.0f;
        herow->gimm_flag |= 0x20000;
    } else {
        if (herow->gimm_flag & 0x20000) {
            herow->gimm_flag &= -0x20001;
            herow->jumpspd_limit = 8.0f;
        }
    }

    objw->rtw.rtn = GetMichiNo(objw->rtw.rtp);
    objw->rtw.mcn = GetMichinori(&objw->rtw);

    herow->wahoo_timer++;
    if (herow->wahoo_timer > 90 || herow->yuka_hit || objw->hitram & 3) {
        herow->wahoo_cnt = 0;
        herow->wahoo_timer = 0;
    }

    if (herow->gimm_flag & 0x1000) {
        herow->lvl = herow->ex_lvl;
        herow->trg = herow->ex_trg;
        if (!GameGbl.playdemo_flag) {
            objw->hitram = 0;
        }
    } else {
        herow->lvl = GameGbl.kpd[herow->con].lvl;
        herow->trg = GameGbl.kpd[herow->con].trg;
        if (herow->area_cnt < 2) {
            herow->lvl = 0;
            herow->trg = 0;
        }
        if (herow->lvl != 0) {
            herow->hima_cnt = 0;
        }
    }

    if (objw->reg0 & 0x400) {
        herow->gimm_flag |= 0x800;
        objw->reg0 |= 0x400;
    } else {
        herow->gimm_flag &= -0x801;
        objw->reg0 &= -0x401;
    }

    if (herow->da_muteki > 0) {
        herow->da_muteki--;
    }
    if (herow->tenmetu > 0) {
        herow->tenmetu--;
    }

    if (objw->reg0 & 0x400) {
        klPadRight = 0x8000;
        klPadLeft = 0x2000;
    } else {
        klPadRight = 0x2000;
        klPadLeft = 0x8000;
    }

    herow->michispd = 0;
    herow->saka = 0.0f;
    herow->yuka_hit = 0;
    herow->tenjo_hosei = 0.0f;

    if (DebTestOn == 1) {
        return;
    }

    if (herow->gimm_flag & 8 && objw->spd[1] > 0.0f) {
        herow->gimm_flag &= -9;
    }
    if (herow->gimm_flag & 0x400 && objw->spd[1] > 0.0f) {
        herow->gimm_flag &= -0x401;
    }
    if (herow->se_id5 != -1) {
        pitch = objw->spd[1] * 60.0f;
        pitch = -pitch;
        if (pitch < -1200) {
            pitch = -1200;
        }
        hSePitch(herow->se_id5, pitch);
    }
    if (herow->gimm_flag & 0x100) {
        if (herow->se_id4 != -1) {
            hSeKeyOff(herow->se_id4);
            herow->se_id4 = -1;
        }
        if (herow->se_id5 != -1) {
            hSeKeyOff(herow->se_id5);
            herow->se_id5 = -1;
        }
        herow->rakka_flag &= -2;
        if (herow->se_id3 == -1) {
            herow->se_id3 = hSeKeyOn(0x4b04b00da00804, objw->posi, 0);
        }
        herow->gimm_flag &= -0x101;

        switch (herow->dai_se_cnt % 3) {
            case 0:
                hSeKeyOn(0xc80c80ca20112, objw->posi, 0);
                break;
            case 1:
                hSeKeyOn(0xc80c80ca20111, objw->posi, 0);
                break;
            case 2:
                hSeKeyOn(0xc80c80ca20113, objw->posi, 0);
                break;
        }
        herow->dai_se_cnt++;
    } else {
        if (objw->spd[1] > 0.0f && herow->se_id3 != -1) {
            hSeKeyOff(herow->se_id3);
            herow->se_id3 = -1;
        }
    }
    if (herow->gimm_flag & 0x200000 || herow->gimm_flag & 0x400000) {
        objw->bun0 = 2;
        return;
    }
    if (herow->gimm_flag & 1) {
        if (herow->se_id4 != -1) {
            hSeKeyOff(herow->se_id4);
            herow->se_id4 = -1;
        }
        if (herow->se_id5 != -1) {
            hSeKeyOff(herow->se_id5);
            herow->se_id5 = -1;
        }
        objw->hitram = 0;
        if (herow->gimm_flag & 0x10) {
            herow->timer = 0;
            herow->taiho_bun = 0;
            herow->gimm_flag &= -0x11;
            herow->gimm_flag |= 0x40;
            if (herow->se_id != -1) {
                hSeKeyOff(herow->se_id);
                herow->se_id = -1;
            }
            if (herow->se_id2 != -1) {
                herow->b_action = 0;
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

    if (GameGbl.kloLife[herow->con] <= 0) {
        objw->hitram = 0;
        herow->lvl = 0;
        herow->trg = 0;
    }

    if (objw->reg0 & 0x200) {
        GimGetMichiVec(&objw->rtw, objw->muki, herow->okuyuka);
    } else {
        GetMichiVec(&objw->rtw, objw->muki);
    }

    if (GameGbl.reverse_cnt > 0.0f) {
        herow->michispd = herow->mispd = 0;
        herow->mispd_buf = 0;
        return;
    }

    if (OkGetKeyDorOpen() != -1) {
        objw->bun0 = 30;
        herow->count = 0;
        OkSetKeyDoorOpen(-1);
    }
    if (objw->bun0 == 30) {
        if (herow->count == 70) {
            hSeKeyOn(0xc80c80ca20114, objw->posi, 0);
        }
        if (herow->count >= 120) {
            objw->bun0 = 11;
        }
        herow->count++;

        if (herow->buki_muki == 1) {
            objw->muki[0] = -objw->muki[0];
            objw->muki[1] = -objw->muki[1];
            objw->muki[2] = -objw->muki[2];
        }
        herow->michispd = herow->mispd = 0;
        herow->mispd_buf = 0;
        if (herow->door_muki == 2) {
            herow->Muki = 1;
        } else if (herow->door_muki == 3) {
            herow->Muki = 2;
        }
        return;
    }

    if (GameGbl.wipe.flag & 2 && GameGbl.wipe.fdata > 0) {
        return;
    }
    if (GameGbl.wipe.outtime == 60 && GameGbl.wipe.flag & 4) {
        herow->se_id4 = hSeKeyOn(0x4b04b00da00805, objw->posi, 0);
        herow->se_id5 = hSeKeyOn(0x4b04b00da00802, objw->posi, 0);
        if (!(herow->gimm_flag & 0x1000)) {
            herow->trg = 0;
            herow->lvl = 0;
        }
    }
    if (GameGbl.wipe.outtime > 0 && !(GameGbl.wipe.flag & 4)) {
        if (GameGbl.vision != 0x0705 && GameGbl.vision != 0x0F08) {
            if (herow->motcnt == 10.0f || herow->motcnt == 30.0f) {
                hSeKeyOn(0x4b04b018a10009, NULL, 0);
            }

            switch (GameGbl.wipe.imuki) {
                case 1:
                    MichiMove(&objw->rtw, 8192);
                    break;
                case 2:
                    MichiMove(&objw->rtw, -8192);
                    objw->muki[0] = -objw->muki[0];
                    objw->muki[1] = -objw->muki[1];
                    objw->muki[2] = -objw->muki[2];
                    break;
                case 4:
                    herow->door_muki = 2;
                    herow->Muki = 1;
                    break;
                case 3:
                {
                    f32 var_f21 = -_atan2(objw->muki[0], objw->muki[2]);
                    objw->posi[0] -= _cos(var_f21) * 0.75f;
                    objw->posi[2] -= _sin(var_f21) * 0.75f;
                    herow->door_muki = 3;
                    herow->Muki = 2;
                    return;
                }
            }
            objw->rtw.mcn = GetMichinori(&objw->rtw);
            MichiPosiXZ(&objw->rtw, objw->posi);
        }
        return;
    }
    if (GameGbl.wipe.intime > 0 && !(GameGbl.wipe.flag & 8)) {
        switch (GameGbl.wipe.omuki) {
            case 1:
                MichiMove(&objw->rtw, 8192);
                herow->Muki = 0;
                herow->door_muki = 0;
                herow->buki_muki = 0;
                break;
            case 2:
                MichiMove(&objw->rtw, -8192);
                herow->Muki = 0;
                herow->door_muki = 1;
                herow->buki_muki = 1;
                objw->muki[0] = -objw->muki[0];
                objw->muki[1] = -objw->muki[1];
                objw->muki[2] = -objw->muki[2];
                break;
            case 4:
            {
                f32 var_f21;
                herow->door_muki = 2;
                herow->Muki = 1;
                MichiPosi(&objw->rtw, objw->posi);
                var_f21 = -_atan2(objw->muki[0], objw->muki[2]);
                objw->posi[0] -= _cos(var_f21) * 0.75f * GameGbl.wipe.intime;
                objw->posi[2] -= _sin(var_f21) * 0.75f * GameGbl.wipe.intime;
                return;
            }
            case 3:
                herow->door_muki = 3;
                herow->Muki = 3;
                break;
        }
        objw->rtw.mcn = GetMichinori(&objw->rtw);
        MichiPosi(&objw->rtw, objw->posi);
        return;
    }

    nkDG.hitmap_flag = 0;

    if (herow->spflag & 1) {
        objw->bun0 = 14;
        objw->spd[1] += 1E-7;
    }

    if (objw->hitram & 3) {
        if (herow->da_muteki <= 0) {
            InitDamageJump(objw, herow);
            herow->tenmetu = herow->da_muteki;
            hSeKeyOn(0xc80c80ca20107, objw->posi, 0);
            if (GameGbl.kloLife[herow->con] > 0) {
                GameGbl.kloLife[herow->con]--;
            }
        }
        objw->hitram &= -4;
    }
    if (objw->hitram & 0x300) {
        InitDamageJump(objw, herow);
        herow->tenmetu = herow->da_muteki;
        hSeKeyOn(0xc80c80ca20107, objw->posi, 0);
        if (GameGbl.kloLife[herow->con] > 0) {
            GameGbl.kloLife[herow->con] = 0;
        }
        objw->hitram &= -0x301;
    }
    if (objw->hitram & 0xC) {
        if (herow->da_muteki <= 0) {
            InitDamageJump(objw, herow);
            herow->tenmetu = 0;
            herow->da_muteki = 30;
            hSeKeyOn(0xc80c80ca20106, objw->posi, 0);
        }
        objw->hitram &= -0xD;
    }

    if (pPAD_LVL_DOWN(herow)) {
        herow->door_muki = 2;
        if (objw->bun0 != 6) {
            herow->Muki = 1;
        }
    } else if (pPAD_LVL_UP(herow)) {
        herow->door_muki = 3;
        if (objw->bun0 != 6) {
            herow->Muki = 2;
        }
    }
    if (herow->lvl & klPadRight) {
        herow->door_muki = 0;
        if (objw->bun0 == 40) {
            herow->Muki = 0;
            herow->buki_muki = 0;
        } else if (objw->bun0 != 10 && objw->bun0 != 6 && objw->bun0 != 35 && objw->bun0 != 40) {
            herow->Muki = 0;
            herow->michispd = 0x3400;
            if (objw->bun0 != 10 && objw->bun0 != 35) {
                herow->buki_muki = 0;
            }
        }
    } else if (herow->lvl & klPadLeft) {
        herow->door_muki = 1;
        if (objw->bun0 == 40) {
            herow->Muki = 0;
            herow->buki_muki = 1;
        } else if (objw->bun0 != 10 && objw->bun0 != 6 && objw->bun0 != 35 && objw->bun0 != 40) {
            herow->Muki = 0;
            herow->michispd = -0x3400;
            if (objw->bun0 != 10 && objw->bun0 != 35) {
                herow->buki_muki = 1;
            }
        }
    }

    switch (herow->Muki) {
        case 0:
            objw->reg0 &= -0x11;
            break;
        case 1:
            herow->buki_muki = 2;
            break;
        case 2:
            herow->buki_muki = 3;
            break;
    }
    if ((objw->reg0 & 0x11) == 0x11) {
        objw->reg0 &= -0x11;
        herow->michispd = 0;
    }
    if (herow->buki_muki == 1 || (herow->buki_muki >= 2 && herow->mispd_buf < 0)) {
        objw->muki[0] = -objw->muki[0];
        objw->muki[1] = -objw->muki[1];
        objw->muki[2] = -objw->muki[2];
    }

    herow->saka = GetMichiSaka(&objw->rtw, herow->saka);

    if (objw->spd[1] != 0.0f) {
        if (herow->michispd != 0) {
            if (herow->michispd > 0) {
                if (herow->mispd_buf < 0x1000) {
                    herow->mispd_buf = 0x1000;
                }
            } else if (herow->mispd_buf > -0x1000) {
                herow->mispd_buf = -0x1000;
            }
            if (herow->mispd_buf < herow->michispd) {
                herow->mispd = herow->mispd_buf + 0x200;
            } else {
                herow->mispd = herow->mispd_buf - 0x200;
            }
        } else {
            herow->mispd = 0;
        }
    } else {
        wk0 = herow->saka;
        sw0 = wk0 * 4096.0f / 6.283184f;
        wk1 = _cos(wk0);
        if (wk1 == 0.0f) {
            wk1 = 1.0f;
        }
        wk1 = _sin(wk0) / wk1;
        wk1 = wk1 * 3.0f / 4.0f;
        sw1 = (s32)(wk1 * 4096.0f) / 4;

        if (herow->michispd != 0) {
            if (herow->michispd > 0) {
                if (sw0 < 0) {
                    if (sw1 < -128) {
                        sw1 = -128;
                    }
                }
                else if (sw1 > 1792) {
                    sw1 = 1792;
                }
                sw0 = sw1;
                if (sw1 == 0) {
                    if (herow->mispd_buf - 96 > 13312) {
                        sw0 = -96;
                    }
                    if (herow->mispd_buf < 13312) {
                        sw0 = 25;
                    }
                }
                if (herow->mispd_buf < 10240) {
                    if (herow->mispd_buf >= -10240) {
                        herow->mispd_buf = 10240;
                    }
                    else if ((sw0 += 768) < 0) {
                        sw0 = 512;
                    }
                } else if (sw1 < 0) {
                    if (herow->mispd_buf < 13312) {
                        sw0 = 25;
                    }
                    else if (herow->mispd_buf + sw0 < 13312) {
                        sw0 = 0;
                        herow->mispd_buf = 13312;
                    }
                }
                herow->mispd = herow->mispd_buf + sw0;
                if (herow->mispd > -10240 && herow->mispd < 10240) {
                    herow->mispd = 10240;
                }
                if (herow->mispd > 20480) {
                    herow->mispd = 20480;
                }
            } else {
                if (sw0 > 0) {
                    if (sw1 > 128) {
                        sw1 = 128;
                    }
                } else if (sw1 < -1792) {
                    sw1 = -1792;
                }
                sw0 = sw1;
                if (sw1 == 0) {
                    if (herow->mispd_buf + 96 < -13312) {
                        sw0 = 96;
                    }
                    if (herow->mispd_buf > -13312) {
                        sw0 = -25;
                    }
                }
                if (herow->mispd_buf > -10240) {
                    if (herow->mispd_buf <= 10240) {
                        herow->mispd_buf = -10240;
                    } else if ((sw0 -= 768) > 0) {
                        sw0 = -512;
                    }
                }
                else if (sw1 > 0) {
                    if (herow->mispd_buf > -13312) {
                        sw0 = -25;
                    }
                    else if (herow->mispd_buf + sw0 > -13312) {
                        sw0 = 0;
                        herow->mispd_buf = -13312;
                    }
                }
                herow->mispd = herow->mispd_buf + sw0;
                if (herow->mispd < 10240 && herow->mispd > -10240) {
                    herow->mispd = -10240;
                }
                if (herow->mispd < -20480) {
                    herow->mispd = -20480;
                }
            }

            if (abs(herow->mispd) >= 13312) {
                herow->spflag |= 64;
            } else {
                herow->spflag &= -65;
            }
            herow->spflag &= -129;
        } else {
            if (!(herow->spflag & 0x80)) {
                if (herow->spflag & 0x40) {
                    herow->spflag &= -0x41;
                } else {
                    herow->mispd = herow->mispd_buf = 0;
                }
            }
            herow->spflag |= 128;
            if (herow->mispd_buf > 0 && (herow->mispd = herow->mispd_buf - 512) < 0) {
                herow->mispd = 0;
            }
            if (herow->mispd_buf < 0 && (herow->mispd = herow->mispd_buf + 512) > 0) {
                herow->mispd = 0;
            }
        }
    }
    herow->mispd += herow->miexspd;

    if ((herow->buki_muki == 0 && herow->mispd < 0) || (herow->buki_muki == 1 && herow->mispd > 0)) {
        objw->ang[0] = -objw->muki[0];
        objw->ang[2] = -objw->muki[2];
    } else {
        objw->ang[0] = objw->muki[0];
        objw->ang[2] = objw->muki[2];
    }
    objw->ang[1] = 0.0f;

    klW_Slider(objw, herow);
    objw->reg6 = 0;
    HeroKabeCheck2(objw, herow);
    if ((objw->reg0 & 3) == 3) {
        herow->michispd = 0;
        herow->mispd = 0;
    } else {
        HeroKabeCheck3(objw, herow);
    }

    MichiMove(&objw->rtw, objw->reg6);

    herow->mispd_buf = herow->mispd;
    if (herow->mispd != 0) {
        vi[0] = abs(herow->mispd >> 12);
        vu0_ITOF12Vector(vf, vi);
        objw->spd[0] = objw->muki[0] * vf[0];
        objw->spd[2] = objw->muki[2] * vf[0];
    }

    if (herow->trg & GameGbl.k_jump_button && herow->spflag & 4 && objw->reg7 == 0) {
        objw->spd[1] = OkJumpDaiSpd;
        herow->spflag |= 0x10;
    }
    if (herow->trg & GameGbl.k_jump_button && objw->spd[1] == 0.0f) {
        if (herow->spflag & 4) {
            objw->spd[1] = -29.25f;
            herow->spflag |= 0x10;
        } else {
            hSeKeyOn(0x4b04b00ca10001, objw->posi, 0);
            herow->count = 0;
            objw->spd[1] = -9.75f;
            if (herow->movebox != NULL && gmGetGimType(herow->movebox) == 2) {
                objw->spd[1] += herow->movebox->freg3;
            }
            if (objw->bun0 != 40) {
                objw->bun0 = objw->bun0 == 6 ? 7 : 2;
            }
            trg2 = 0;
        }
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

    herow->saka = GetMichiSaka(&objw->rtw, herow->saka);

    if (objw->reg0 & 0x40) {
        objw->bun0 = 27;
        herow->sjump_flag = objw->posi[1];
        SayWahoo(objw, herow);
        objw->reg0 &= -0x145;
        objw->spd[1] = -1.0f;
        herow->sjump_yspd = -8.666667f;
        herow->timer = 0;
        herow->count = 0;
    } else if (objw->spd[1] != 0.0f || objw->reg0 & 0x100 || objw->bun0 == 40) {
        if (objw->bun0 != 14 && objw->bun0 != 28 && objw->bun0 != 31) {
            Hero_Jump_Sub(objw, herow);
        }

        if (objw->reg0 & 1 && herow->mochifuku != NULL) {
            if (objw->bun0 != 10 || !(objw->spd[1] < 0.0f)) {
                if (herow->trg & GameGbl.k_atck_button && !(herow->gimm_flag & 0x10000)) {
                    if (herow->Muki == 0) {
                        hSeKeyOn(0xc80c80ca20104, objw->posi, 0);
                    } else {
                        hSeKeyOn(0xc80c80ca20105, objw->posi, 0);
                    }
                    herow->spflag &= -2;
                    objw->bun0 = 9;
                    herow->mochifuku->reg2 |= 0x40;
                    herow->timer = 0;
                    herow->count = 0;
                } else if ((trg2 & GameGbl.k_jump_button || objw->reg0 & 0x20) && !(herow->spflag & 1) && !(objw->reg0 & 8) && !(herow->gimm_flag & 0x8000)) {
                    objw->bun0 = 27;
                    herow->sjump_flag = objw->posi[1];
                    if (herow->gimm_flag & 4) {
                        hSeKeyOn(0xc80c80ca2010f, objw->posi, 0);
                    } else {
                        SayWahoo(objw, herow);
                    }
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

        if (Hero_Kodo[objw->bun0] != NULL) {
            Hero_Kodo[objw->bun0](objw, herow);
        }

        if (objw->spd[1] < 0.0f) {
            TenjoHit(objw, herow);
        }
    } else {
        if (objw->bun0 != 6 && objw->bun0 != 35 && objw->bun0 != 40 && herow->trg & GameGbl.k_atck_button && !(herow->gimm_flag & 0x2000)) {
            herow->timer = 0;
            if (!(objw->reg0 & 1)) {
                SetKazd(objw);
                objw->bun0 = 6;
            } else {
                herow->count = 0;
                herow->mochifuku->reg2 |= 0x40;
                if (herow->Muki == 0) {
                    hSeKeyOn(0xc80c80ca20104, objw->posi, 0);
                } else {
                    hSeKeyOn(0xc80c80ca20105, objw->posi, 0);
                }
                if (herow->michispd != 0) {
                    objw->bun0 = 18;
                } else {
                    objw->bun0 = 8;
                }
            }
        }
        if (objw->bun0 != 6 && objw->bun0 != 40) {
            if (herow->michispd != 0) {
                switch (objw->bun0) {
                    case 12:
                    case 18:
                    case 22:
                    case 30:
                    case 34:
                    case 35:
                    case 38:
                    case 39:
                        break;
                    default:
                        objw->bun0 = 1;
                        break;
                }
            } else {
                if (objw->bun0 == 18) {
                    objw->bun0 = 8;
                }
                switch (objw->bun0) {
                    case 3:
                    case 8:
                    case 12:
                    case 28:
                    case 30:
                    case 31:
                    case 34:
                    case 35:
                    case 38:
                    case 39:
                        break;
                    default:
                        objw->bun0 = 11;
                        break;
                }
            }
        }

        if (Hero_Kodo[objw->bun0] != NULL) {
            Hero_Kodo[objw->bun0](objw, herow);
        }
    }

    if (objw->reg0 & 0x400) {
        objw->posi[1] -= objw->spd[1];
    } else {
        objw->posi[1] += objw->spd[1];
    }
    MichiMove(&objw->rtw, herow->mispd);

    if (objw->spd[1] + herow->tenjo_hosei > 0.0f) {
        Hero_JumpDown_Sub(objw, herow);
    } else if (objw->spd[1] > 0.0f) {
        Hero_JumpUp_Sub(objw, herow);
    } else if (objw->spd[1] == 0.0f) {
        s32 spd;
        RT_WRK rtw = objw->rtw;
        dy = Ashi_HitCheck(objw, &herow->retflag, herow->saka, &herow->kage_flag);

        switch (herow->kage_flag) {
            case 1:
                spd = 0x23000;
                goto cringe2;
            case 3:
                spd = -0x23000;
            cringe2: MichiMove(&rtw, spd);
                break;
            default:
                if (dy == 65535.0f) {
                    herow->noyukatime++;
                } else {
                    objw->posi[1] = dy;
                }
                goto cringe;
        }


        herow->noyukatime = 0;
        objw->posi[1] = dy;
cringe:
        if (herow->noyukatime >= 6 && objw->bun0 != 40 && objw->bun0 != 14 && objw->bun0 != 12 && objw->bun0 != 34) {
            objw->spd[1] = 5.0f;
            herow->noyukatime = 0;
            if (objw->bun0 == 6) {
                objw->bun0 = 7;
            } else {
                objw->bun0 = 2;
            }
        }
    }

    if (objw->spd[1] <= 0.0f || objw->bun0 == 40) {
        herow->rakka_cnt = 0;
    }

    herow->yuka_hit_buf = herow->yuka_hit;
    herow->yuka_hit_old = herow->yuka_hit;

    if (herow->movebox != NULL && herow->yuka_hit != 3) {
        herow->movebox = NULL;
    }

    if (!(herow->rakka_flag & 1) && objw->spd[1] > 9.0f) {
        herow->se_id4 = hSeKeyOn(0x4b04b00da00805, objw->posi, 0);
        herow->se_id5 = hSeKeyOn(0x4b04b00da00802, objw->posi, 0);
        herow->b_action = 0;
        herow->rakka_flag |= 1;
    }

    if (herow->yuka_hit != 0) {
        herow->rakka_flag &= -2;
        if (herow->se_id4 != -1) {
            hSeKeyOff(herow->se_id4);
            herow->se_id4 = -1;
        }
        if (herow->se_id5 != -1) {
            hSeKeyOff(herow->se_id5);
            herow->se_id5 = -1;
        }
    } else {
        herow->rakka_flag |= 2;
    }

    if (herow->movebox != NULL) {
        MichiMove(&objw->rtw, herow->gimspd);
        if (gmGetGimType(herow->movebox) != 1) {
            objw->posi[1] += herow->movebox->freg3;
        }
    }

    herow->gimspd = 0;
    if (objw->reg0 & 0x200) {
        GimMichiPosiXZ(&objw->rtw, objw->posi, herow->okuyuka);
    } else {
        MichiPosiXZ(&objw->rtw, objw->posi);
    }

    objw->reg6 = 0;
    herow->timer++;
    herow->count++;
    herow->dead_cnt++;
    herow->hima_cnt++;
    herow->area_cnt++;
    herow->jmp_cnt++;
    herow->rakka_cnt++;
    objw->rtw.mcn = GetMichinori(&objw->rtw);
    herow->spflag &= -0xD;
}

static void (*ObjFuncTbl[5])(OBJWORK *objw) = {
    ObjInit,
    ObjMain,
    NULL, // TODO: KloSlideSide
    NULL, // TODO: KloSlideBehind
    KloSlidePipe
};

void ObjKlo(OBJWORK *objw) {
    ObjFuncTbl[objw->bun1](objw);
}

void SetRtwKlonoa(s32 rtn, s32 rtcnt, s32 muki) {
    OBJWORK *objw = GameGbl.klonoa;
    HERO_WORK *herow = objw->work;

    SetRouteNo(&objw->rtw, rtn);
    SetMichinori(&objw->rtw, rtcnt << 12);
    MichiPosi(&objw->rtw, objw->posi);
    GetMichiVec(&objw->rtw, objw->muki);
    herow->door_muki = muki;
    herow->buki_muki = muki;
}

void SetRtwKlonoaY(s32 rtn, s32 rtcnt, s32 muki, f32 height) {
    OBJWORK *objw = GameGbl.klonoa;
    HERO_WORK *herow = objw->work;

    SetRouteNo(&objw->rtw, rtn);
    SetMichinori(&objw->rtw, rtcnt << 12);
    MichiPosiXZ(&objw->rtw, objw->posi);
    objw->posi[1] = height;
    GetMichiVec(&objw->rtw, objw->muki);
    herow->door_muki = muki;
    herow->buki_muki = muki;
}
