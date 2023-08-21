#ifndef HR_PEFC_H
#define HR_PEFC_H

#include "harada.h"

typedef struct { // 0xd0
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword gif0;
    /* 0x20 */ qword test;
    /* 0x30 */ qword alpha;
    /* 0x40 */ qword xyofs;
    /* 0x50 */ qword tex0;
    /* 0x60 */ qword gif1;
    /* 0x70 */ qword rgb0;
    /* 0x80 */ qword uv0;
    /* 0x90 */ qword xyz0;
    /* 0xa0 */ qword rgb1;
    /* 0xb0 */ qword uv1;
    /* 0xc0 */ qword xyz1;
} ATR_MWAKU;

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

#endif
