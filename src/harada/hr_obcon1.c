#include "harada/hr_obcon1.h"
#include "okanoyo/okio.h"

static void hr_check_switchTrg(HR_OBJSW *sw, s32 id, s32 on);
static void hr_sw_0402(HR_OBJSW *sw, s32 id, s32 on);

static void (*hrSwitchTrgTbl[3])(HR_OBJSW *, s32, s32) = {
    hr_check_switchTrg,
    NULL,
    hr_sw_0402
};

void hr_swtrg_getset(OBJWORK *objw, s32 *tbl) {
    HR_OBJSW *sw;
    s32 i;

    objw->reg0 = *tbl++;
    if ((s32)(objw->work = getmem(objw->reg0 * sizeof(HR_OBJSW))) == -1) {
        objw->stat0 = 0;
    } else {
        objw->prty = 0x7F;
        for (i = 0, sw = (HR_OBJSW *)objw->work; i < objw->reg0; sw++, i++, tbl += 2) {
            sw->bun0 = sw->reg0 = sw->pad0 = 0;
            sw->ptr = tbl;
        }
    }
}

static void hr_check_switchTrg(HR_OBJSW *sw, s32 id, s32 on) {
    s32 *ptr;

    ptr = sw->ptr;
    if (*ptr == id && on == 1) {
        sw->bun0 = ptr[1];
    }
}

static void hr_sw_0402(HR_OBJSW *sw, s32 id, s32 on) {
    CAM_WORK *cam;

    cam = &GameGbl.cam;
    if (sw->reg0 == 0) {
        GameGbl.cam.mode = 7;
        hr_pcam_init(cam->posi, cam->ang, cam->scr_z);
        hr_pcam_set_mode(PCAMW_PAI);
        hr_pcam_set_pos(718.5f, -185.0f, -422.625f);
        hr_pcam_set_ang(0.0f, 53.0f, 0.0f, 0);
        hr_pcam_pos_tmv(0, 260.0f, -185.0f, -599.875f, 120, 100, 100);
        hr_pcam_ang_tmv(0, 0.0f, 27.0f, 0.0f, 120, 100, 100);
        hr_pcam_work();
        sw->reg0++;
    } else {
        hr_pcam_work_mae();
        hr_pcam_work();
        if (hr_pcam_tmv_end(HRTMV_WP) == 0 || hr_pcam_tmv_end(HRTMV_WA) == 0)
            return;
        sw->bun0 = HROBJ_SW_END;
        CamRelease(60.0f);
    }
}

void hr_switch_trg(OBJWORK *objw) {
    s32 id;
    s32 on;
    HR_OBJSW *sw;
    s32 i;

    id = OkgetSwitchID();
    on = id >> 12 & 15;
    id &= 0xFF;
    for (i = 0, sw = (HR_OBJSW *)objw->work; i < objw->reg0; i++, sw++) {
        if (sw->bun0 != HROBJ_SW_END) {
            hrSwitchTrgTbl[sw->bun0](sw, id, on);
        }
    }
}

