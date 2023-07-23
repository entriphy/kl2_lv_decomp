#ifndef LIGHT_H
#define LIGHT_H

#include "nakano.h"

typedef struct { // 0x10
    /* 0x0 */ s32 rtcnt;
    /* 0x4 */ s32 height;
    /* 0x8 */ u8 flg;
    /* 0x9 */ u8 type;
    /* 0xa */ s16 idx;
    /* 0xc */ s32 dummy;
} nkLgtMNG;

typedef struct { // 0xa
    /* 0x0 */ s16 dir_x;
    /* 0x2 */ s16 dir_y;
    /* 0x4 */ u8 r;
    /* 0x5 */ u8 g;
    /* 0x6 */ u8 b;
    /* 0x7 */ u8 flag;
    /* 0x8 */ s16 dummy;
} LgtINFO;

typedef struct { // 0x18
    /* 0x00 */ f32 dir_x;
    /* 0x04 */ f32 dir_y;
    /* 0x08 */ f32 col[4];
} LgtFIF;

typedef struct { // 0x58
    /* 0x00 */ LgtFIF lgt[3];
    /* 0x48 */ f32 amb[4];
} LgtWorkF;

typedef struct { // 0x22
    /* 0x00 */ LgtINFO lgt[3];
    /* 0x1e */ u8 amb[4];
} LgtWorkDat;

extern void nkLightIp(LgtWorkF *lgt_r, LgtWorkF *lgt_a, LgtWorkF *lgt_b, f32 w);

#endif
