#include "common.h"

static void hr_anm_v0100i(HRANMV *av);
static void hr_anm_v0100w(HRANMV *av);

static HRANMVS hr_v_avt_tbl[1] = { {hr_anm_v0100i, hr_anm_v0100w, 3, (vpmINFO2 *)&VpmInfo} };

static void hr_anm_v0100i(HRANMV *av) {
    OBJWORK *obj;

    obj = GameGbl.klonoa;
    hr_set_vlight(&hrvlight[0], obj->posi[0], obj->posi[1], obj->posi[2], 1.0f, 1.0f, 2.0f, 20.0f, 300.0f);
    hr_set_vlight(&hrvlight[1], obj->posi[0], obj->posi[1] - 200.0f, obj->posi[2], 2.0f, 1.0f, 1.0f, 20.0f, 300.0f);
    hr_set_vlightMini(&hrvlight[0], 0.1f, 0.1f, 0.1f);
    hr_set_vlightMini(&hrvlight[1], 0.0f, 0.0f, 0.0f);
}

static void hr_anm_v0100w(HRANMV *av) {
    OBJWORK *obj;

    // Empty function
}

void hr_vision_anmVPM_set() {
    switch (GameGbl.vision) {
        case 0x0D00:
        case 0x0D01:
        case 0x0D02:
        case 0x0D03:
        case 0x0D04:
            hr_anmVPM_set(&hr_v_avt_tbl[0]);
            hrmapoff = 1;
            break;
    }
}

void hr_change_anmVPM(s32 id) {
    HRANMV *anmv;

    anmv = &hravbuf[0];
    switch (id) {
        case HRAV_SINT:
            anmv->drawno = id;
            anmv->prog = hr_anm_v0100w;
            break;
        case HRAV_SINT2:
            anmv->drawno = id;
            anmv->prog = hr_anm_v0100w;
            break;
        default:
            return;
    }
}