#ifndef NK_WIPE_H
#define NK_WIPE_H

#include "nakano.h"

extern void nkWipeInit();
extern void nkWipeSetMatrix();
extern void nkWipeEffBClear();

typedef struct { // 0x80
    /* 0x00 */ s32 size_clear;
    /* 0x10 */ sceGifTag gif_clear;
    /* 0x20 */ sceGsClear clear;
} nkWipeEffBC;

typedef struct { // 0x80
    /* 0x00 */ s32 size_texenv;
    /* 0x10 */ sceGifTag gif_texenv;
    /* 0x20 */ sceGsTexEnv2 texenv;
    /* 0x60 */ u64 alpha;
    /* 0x68 */ u64 alp_addr;
    /* 0x70 */ u64 test;
    /* 0x78 */ u64 test_addr;
} nkWipeDrawEnv;

#endif
