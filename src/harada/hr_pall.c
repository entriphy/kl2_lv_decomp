#include "common.h"

// PCAMS ppcam = {};
// PCAMS *hrpcam = NULL;
// HRPMOJI ppmes = {};
// HRPMWAKU ppwaku = {};
s32 ptflag_buff[4] = {};
s32 ptflag_st[4] = {};
s32 ptflag_ar[4] = {};
s32 ptflag_th[4] = {};
sceVu0FMATRIX hrpum = {};
sceVu0FMATRIX hrpm = {};
sceVu0FVECTOR hrvi = {};
sceVu0FVECTOR hrvo = {};
s32 *hrpt_addr = NULL;
s32 *hrptm_addr = NULL;
s16 hrpt_flag = 0;
s16 hrpt_scene = 0;
s16 hrpt_view = 0;
s16 hrpt_th = 0;
s16 hrpt_vt = 0;
s16 hrpt_deb = 0;
// PCAMSC *hrpcc = NULL;
u32 *hrpt_pack = NULL;
s16 hrpt_stnk = 0;
// HR_PSYS ppsys = {}
// HR_CALL *hrpt_call = NULL;
s32 hrpt_mini = 0;
s32 *hrpt_mini_addr = NULL;

void hr_pt_set(s16 flag, s16 scene, s16 view, s16 th) {
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
    hrpt_addr = (s32 *)-1;
}
