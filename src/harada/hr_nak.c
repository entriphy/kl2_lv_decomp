#include "harada/hr_nak.h"
#include "harada/hr_mapdr.h"
#include "harada/hr_pcam.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"

void hr_nak_init_allobj() {
    InitAllObjWork(SysGbl.objwork, SysGbl.n_objw);
}

void hr_nak_work_allobj() {
    if (GameGbl.pause_flag == 0) {
        SortObjWork(SysGbl.objwork, SysGbl.n_objw);
        ObjFunc(SysGbl.objwork);
    }
}

void hr_nak_draw_allobj() {
    hrDrawVpo_st();
    DrawObjFunc(SysGbl.objwork);
    hrDrawVpo_end();
}

void hr_nak_draw_effobj() {
    if (hr_abeoff == 0) {
        abEff2Begin();
    }
    DrEffObjFunc(SysGbl.objwork);
    if (hr_abeoff == 0) {
        abEff2End();
    }
}

void hr_nak_dummyVK() {
    OBJWORK *ow;

    ow = (OBJWORK *)getmem(sizeof(OBJWORK));
    InitObjWork(ow);
    ow->posi[0] = ow->posi[1] = ow->posi[2] = 0.0f;
    ow->posi[3] = 1.0f;
    GameGbl.klonoa = ow;
}

void ptcat_camera() {
    GameGbl.cam.mode = 7;
    hr_pcam_init(GameGbl.cam.posi, GameGbl.cam.ang, GameGbl.cam.scr_z);
}

void ptrel_camera(f32 tm) {
    CamRelease(tm);
}

RT_WRK* hr_get_klort() {
    OBJWORK *kw;

    kw = GameGbl.klonoa;
    return &kw->rtw;
}

void hr_open_klokey(s32 fg) {
    HERO_WORK *herow;

    herow = (HERO_WORK *)GameGbl.klonoa->work;
    if (fg != 0) {
        if (!(herow->gimm_flag & 0x1000)) {
            herow->gimm_flag |= 0x1000;
            herow->ex_lvl = herow->ex_trg = 0;

        }
    } else {
        herow->gimm_flag &= ~0x1000;
    }
}

void hr_set_klokey(s32 trg, s32 lvl) {
    HERO_WORK *herow;

    herow = (HERO_WORK *)GameGbl.klonoa->work;
    if (herow->gimm_flag & 0x1000) {
        herow->ex_lvl = lvl;
        herow->ex_trg = trg;
    }
}

s32 hr_pkey_change(s32 trg) {
    switch (trg) {
        case 0x1:
            return 0x1000;
        case 0x2:
            return 0x4000;
        case 0x4:
            return 0x8000;
        case 0x8:
            return 0x2000;
        case 0x80:
            return 0x10;
        case 0x20:
            return 0x40;
        case 0x40:
            return 0x80;
        case 0x10:
            return 0x20;
        case 0xC00:
            return 0xC;
        case 0x3000:
            return 0x3;
        case 0x4000:
            if (kzGetPadType() == 0) {
                return 0xA0;
            } else {
                return 0x50;
            }
        case 0x8000:
            if (kzGetPadType() == 0) {
                return 0x50;
            } else {
                return 0xA0;
            }
        default:
            return 0;
    }
}

void hr_pklo_center(f32 time) {
    HERO_WORK *herow;

    herow = (HERO_WORK *)GameGbl.klonoa->work;
    herow->center_time = time;
}

void hr_pklo_jump(f32 spd, s32 flag) {
    PtKlonoaJump(spd, flag);
}
