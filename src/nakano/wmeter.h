#ifndef WMETER_H
#define WMETER_H

#include "nakano.h"

typedef struct { // 0x20
    /* 0x00 */ s32 cnt;
    /* 0x04 */ s32 p0;
    /* 0x08 */ s32 p1;
    /* 0x0c */ s32 p2;
    /* 0x10 */ s32 r;
    /* 0x14 */ s32 g;
    /* 0x18 */ s32 b;
    /* 0x1c */ s32 a;
} nkMETER;

extern void nkResetMeter();
extern void nkSetMeter();
extern void nkDrawWorkMeter();

#endif
