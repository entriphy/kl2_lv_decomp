#ifndef HR_PEFC_H
#define HR_PEFC_H

#include "harada.h"
#include "hr_pmes.h"

typedef struct { // 0x60
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword gif0;
    /* 0x20 */ qword bitblt;
    /* 0x30 */ qword trxpos;
    /* 0x40 */ qword trxreg;
    /* 0x50 */ qword trxdir;
} ATR_VTOV;

typedef struct { // 0x120
    /* 0x000 */ qword dmatag;
    /* 0x010 */ qword gif0;
    /* 0x020 */ qword test;
    /* 0x030 */ qword alpha;
    /* 0x040 */ qword xyofs;
    /* 0x050 */ qword gif1;
    /* 0x060 */ qword rgb0;
    /* 0x070 */ qword xyz0;
    /* 0x080 */ qword rgb1;
    /* 0x090 */ qword xyz1;
    /* 0x0a0 */ qword rgb2;
    /* 0x0b0 */ qword xyz2;
    /* 0x0c0 */ qword rgb3;
    /* 0x0d0 */ qword xyz3;
    /* 0x0e0 */ qword rgb4;
    /* 0x0f0 */ qword xyz4;
    /* 0x100 */ qword rgb5;
    /* 0x110 */ qword xyz5;
} ATR_MSKIP;

extern void hr_pfade_init(HR_FADE *fade);
extern void hr_pfade_sinit();
extern void hr_pfade_set(HR_FADE *fade, u8 r, u8 g, u8 b, u8 a);
extern void hr_pfade_in(HR_FADE *fade, u8 r, u8 g, u8 b, u8 a, u16 time);
extern void hr_pfade_out(HR_FADE *fade, u8 r, u8 g, u8 b, u8 a, u16 time);
extern void hr_pfade_x(HR_FADE *fade, u16 time);
extern void hr_pfade_work(HR_FADE *fade);
extern void hr_pfade_drawN(HR_FADE *fade);
extern void hr_pfade_drawX(HR_FADE *fade);
extern void hr_pfade_draw(HR_FADE *fade);
extern void hr_pt_skipdraw(HR_PSYS *ps);

#endif
