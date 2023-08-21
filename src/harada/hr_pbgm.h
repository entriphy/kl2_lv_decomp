#ifndef HR_PBGM_H
#define HR_PBGM_H

#include "harada.h"

typedef struct { // 0x8
    /* 0x0 */ s16 sbgm;
    /* 0x2 */ s16 ebgm;
    /* 0x4 */ s16 flag;
    /* 0x6 */ s16 pad0;
} PTBGM;

typedef enum  {
    HRPSFG_0706IKA,
    HRPSFG_0609IKA,
    HRPSFG_CHK040B,
    HRPSFG_CHK0505,
    HRPSFG_CHKTREC,
    HRPSFG_CHKTRG,
    HRPSFG_PNUM
} HRPSFG;

extern s32 hrpt_id;
extern s16 hrptm_res;
extern s32 hrpt_sbgm;

extern s32 hr_ptchk_45(s32 id);
extern void hr_pt_setBGM(HR_PSYS *ps, s16 bgm, s32 mode);
extern void hr_pt_reqBGM(HR_PSYS *ps);
extern void hr_ptbgm_init(HR_PSYS *ps);
extern void hr_ptbgm_end(HR_PSYS *ps, s32 mode);

#endif
