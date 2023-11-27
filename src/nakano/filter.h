#ifndef FILTER_H
#define FILTER_H

#include "nakano.h"

typedef struct { // 0x80
    /* 0x00 */ s32 size_texenv;
    /* 0x10 */ sceGifTag gif_texenv;
    /* 0x20 */ sceGsTexEnv2 texenv;
    /* 0x60 */ u64 alpha;
    /* 0x68 */ u64 alp_addr;
    /* 0x70 */ u64 test;
    /* 0x78 */ u64 test_addr;
} nkEffFrDrawEnv;

extern u8 nkCalIntpl8bit(u8 a, u8 b, f32 pow);
extern u32 nkCalIntplCol(u32 ca, u32 cb, f32 pow);
extern void nkDrawEffFr();
extern void nkDrawFilterShadeOff(sceGsDrawEnv1 *env1, sceGsDrawEnv1 *env1_d, f32 pow);

#endif