#include "nakano/gmiss.h"
#include "nakano/filter2.h"
#include "nakano/gssub.h"
#include "nakano/wipe.h"
#include "hoshino/h_game.h"
#include "take/object.h"

static void nkSetReStartParam() {
    GameGbl.Re_st_rtn = OkReStartKloRouteNo();
    GameGbl.Re_st_mcn = OkReStartKloMichinori();;
    GameGbl.Re_st_height = OkReStartKloYposition();
    GameGbl.Re_st_klomode = OkKloMode();
    GameGbl.Re_st_muki = OkReStartKloDir();
    GameGbl.Re_st_vision = OkReStartVision();
}

void Hero_LifeLost_Sub(OBJWORK *objw, HERO_WORK *herow) {
    s32 black_out_time;

    GameGbl.non_pause_flag = 1;
    if (objw->bun1 == 2 || objw->bun1 == 3 || objw->bun1 == 4) {
        black_out_time = 110;
    } else {
        black_out_time = 160;
    }

    if (herow->dead_cnt == 31) {
        if (objw->reg0 & 0x1 && herow->mochifuku != NULL) {
            objw->reg0 &= -0x2;
            herow->mochifuku->stat = 0;
        }
        GameGbl.kloZanki[herow->con]--;
        herow->da_muteki = 80;
        nkSetFilterObj(3, black_out_time, 0);
    } else if (herow->dead_cnt >= 30 + black_out_time) {
        Init_Restart();
    }
}

void Hero_Dead_Sub(OBJWORK *objw, HERO_WORK *herow) {
    if (herow->yarare_bun == 2) {
        objw->spd[1] /= 2.0f;
        GameGbl.cam.mode = 7;
    }
    if (GameGbl.kloLife[herow->con] != 0) {
        GameGbl.kloLife[herow->con] = 0;
    }

    if (herow->dead_cnt == 31) {
        GameGbl.kloZanki[herow->con]--;
        nkSetFilterObj(3, 60, 0);
    } else if (herow->dead_cnt >= 90) {
        Init_Restart();
    }
}

void nkKloNoLife(OBJWORK *objw, HERO_WORK *herow) {
    s32 vision;
    s32 black_out_time;

    vision = GameGbl.vision >> 8;
    if (!(herow->gimm_flag & 0x40000)) {
        herow->gimm_flag |= 0x40000;
        if (GameGbl.kloLife[herow->con] != 0) {
            GameGbl.kloLife[herow->con] = 0;
        }
        herow->mispd = 0;
        herow->mispd_buf = 0;
        herow->michispd = 0;
        herow->count = 0;
        herow->dead_cnt = 0;
        herow->tenmetu = 0;
        herow->da_muteki = 0x1000;
        objw->bun0 = 34;
        nkSetReStartParam();

        if (objw->bun1 == 2 || objw->bun1 == 3 || objw->bun1 == 4) {
            black_out_time = 110;
        } else {
            black_out_time = 160;
        }

        if (vision == 18 || vision == 19 || kzCheckGenju() == 1) {
            GameGbl.kloZanki[herow->con] = 1;
        }
        if (GameGbl.kloZanki[herow->con] < 0) {
            hKlonoaDead(0x3200, black_out_time + 30, DEAD_DAMAGE);
        } else {
            hKlonoaDead(GameGbl.Re_st_vision, black_out_time + 30, DEAD_DAMAGE);
        }
        nkSetFilterObj(3, 30, 1);
    }
}

void nkKloPhCode01(OBJWORK *objw, HERO_WORK *herow) {
    s32 vision;

    vision = GameGbl.vision >> 8;
    if (!(herow->gimm_flag & 0x40000)) {
        herow->gimm_flag |= 0x40000;
        if (GameGbl.kloLife[herow->con] != 0) {
            GameGbl.kloLife[herow->con] = 0;
        }
        herow->spflag &= -2;
        herow->mispd = 0;
        herow->mispd_buf = 0;
        herow->michispd = 0;
        herow->count = 0;
        herow->dead_cnt = 0;
        herow->tenmetu = 0;
        herow->da_muteki = 0x1000;
        objw->bun0 = 12;
        nkSetReStartParam();

        if (vision == 18 || vision == 19 || kzCheckGenju() == 1) {
            GameGbl.kloZanki[herow->con] = 1;
        }
        if (GameGbl.kloZanki[herow->con] < 0) {
            hKlonoaDead(0x3200, 90, DEAD_DAMAGE);
        } else {
            hKlonoaDead(GameGbl.Re_st_vision, 90, DEAD_FALL);
        }
        nkSetFilterObj(3, 30, 1);
    }
}

void nkKloPhCode03(OBJWORK *objw, HERO_WORK *herow) {
    s32 vision;
    sceVu0FVECTOR v;

    vision = GameGbl.vision >> 8;
    if (!(herow->gimm_flag & 0x40000)) {
        herow->gimm_flag |= 0x40000;

        sceVu0CopyVector(v, objw->posi);
        abSetShibukiB2(v, 3.0f, 5.0f, 0.002f, 2, 5);
        v[1] -= 16.0f;
        abSetShibukiB2(v, 4.0f, 6.0f, 0.002f, 2, 5);

        herow->spflag &= -2;
        if (GameGbl.kloLife[herow->con] != 0) {
            GameGbl.kloLife[herow->con] = 0;
        }
        objw->bun0 = 12;
        herow->mispd = 0;
        herow->mispd_buf = 0;
        herow->michispd = 0;
        herow->count = 0;
        herow->dead_cnt = 0;
        herow->tenmetu = 0;
        herow->da_muteki = 0x1000;
        nkSetReStartParam();

        if (herow->yarare_bun != 2) {
            if (vision == 18 || vision == 19 || kzCheckGenju() == 1) {
                GameGbl.kloZanki[herow->con] = 1;
            }
            if (GameGbl.kloZanki[herow->con] < 0) {
                hKlonoaDead(0x3200, 90, DEAD_DAMAGE);
            } else {
                hKlonoaDead(GameGbl.Re_st_vision, 90, DEAD_DROWN);
            }
            herow->yarare_bun = 2;
            nkSetFilterObj(3, 30, 1);
        }
    }
}

void nkKloMissMain() {
    OBJWORK *klo;
    HERO_WORK *herow;

    klo = GameGbl.klonoa;
    herow = klo->work;

    if (klo->bun0 == 12 || klo->bun0 == 34) {
        if (herow->dead_cnt >= 30) {
            nkGsSetNormalFZ2();
            nkDrawBlackIn(1.0f);
            GameGbl.flag |= 3;
        }

        nkGsSetEffectFZ2();
        nkWipeEffBClear();
        nkGsSetNormalFZ2();
        Vu0ProgTrans();
        OutLineEnvInit(GameGbl.fr, GameGbl.inter);

        sceVu0CopyMatrix(Scr.WvMtx, GameGbl.wvm);
        sceVu0CopyMatrix(Scr.VsMtx, GameGbl.vsm);
        sceVu0CopyMatrix(Scr.WsMtx, GameGbl.wsm);

        switch (klo->bun1) {
            case 1:
                DrawSfxKlonoa(klo);
                break;
            case 2:
                DrawSfxKlonoa_SS(klo);
                break;
            case 3:
                DrawSfxKlonoa_BH(klo);
                break;
            case 4:
                DrawSfxKlonoa_BH(klo);
                break;
        }
    }
}
