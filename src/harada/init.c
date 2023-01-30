#include "common.h"

int ptflag_buff[4];
int ptflag_st[4];
int ptflag_ar[4];
int ptflag_th[4];
PTDATA scene_table[90];
int hr_abeoff;
void* hrpt_addr;
void* hrptm_addr;
short hrpt_flag;
short hrpt_scene;
short hrpt_view;
short hrpt_th;
short hrpt_vt;
PT hrpt_id;
short hrptm_res;
int hrpt_sbgm;

void hr_cold_start() {
    hr_pflag_init();
    hr_pt_fclear();
    hrpt_deb = 0;
    hr_abeoff = 0;
    hrpt_sbgm = 0;
}

void hr_pt_set(short flag, short scene, short view, short th) {
    hrpt_flag = flag;
    hrpt_scene = scene;
    hrpt_view = view;
    hrpt_th = th;

    if (flag != 0) {
        if (hrpt_flag == 4)
            hrpt_id = -1;
        else
            hrpt_id = hr_pflag_get_id();
    }
}

void hr_pt_fclear() {
    hr_pt_set(0, 0, 0, 0);
    hrpt_vt = 0;
    hrpt_addr = (void *)-1;
}

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
    for (int i = 0; i < 90; i++) {
        if (scene_table[i].area == GameGbl.vision && scene_table[i].scene == hrpt_scene)
            return (PT)i;
    }
    return -1;
}
