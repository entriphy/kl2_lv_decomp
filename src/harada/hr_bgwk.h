#ifndef HR_BGWK_H
#define HR_BGWK_H

#include "harada.h"

typedef struct { // 0x90
    /* 0x00 */ sceVu0FMATRIX m;
    /* 0x40 */ sceVu0FMATRIX m0;
    /* 0x80 */ mINFO *info;
    /* 0x84 */ s32 rx;
    /* 0x88 */ s32 ry;
    /* 0x8c */ s32 fg;
} BGWK;

extern BGWK *hrbgbuff;
extern s32 hrcntbg;
extern s32 hrcntdrawbg;
extern mINFO hrbgi[6];
extern u32 *hrbgbin[6];

extern void hr_bg_onoff(s32 no, s32 fg);
extern void hr_bginit();
extern void hr_bg_workclip();

#endif
