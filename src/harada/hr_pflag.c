#include "common.h"

static PTDATA scene_table[90] = {};

void hr_pflag_initAr() {
    memset(ptflag_ar, 0, 0x10);
}

void hr_pflag_initSt() {
    memset(ptflag_st, 0, 0x10);
    hr_pflag_initAr();
}

void hr_pflag_init() {
    memset(ptflag_buff, 0, 0x10);
    memset(ptflag_th, 0, 0x10);
    hr_pflag_initSt();
}

PT hr_pflag_get_id() {
    int i;

    for (i = 0; i < 90; i++) {
        if (scene_table[i].area == GameGbl.vision && scene_table[i].scene == hrpt_scene)
            return (PT)i;
    }
    return -1;
}
