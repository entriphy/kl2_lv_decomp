#ifndef TARO_H
#define TARO_H

#include "common.h"

typedef struct { // 0x8c
    /* 0x00 */ s32 type;
    /* 0x04 */ s32 bsDataSize;
    /* 0x08 */ u16 width;
    /* 0x0a */ u16 height;
    /* 0x0c */ u32 nframes;
    /* 0x10 */ u32 frame_cnt;
    /* 0x14 */ u8 thVal0;
    /* 0x15 */ u8 thVal1;
    /* 0x16 */ s16 incnum;
    /* 0x18 */ u16 field_0x18;
    /* 0x1a */ u16 field_0x1a;
    /* 0x1c */ u32 mbx;
    /* 0x20 */ u32 mby;
    /* 0x24 */ u32 texbp;
    /* 0x28 */ u32 texbw;
    /* 0x2c */ u32 tw;
    /* 0x30 */ u32 th;
    /* 0x34 */ u32 cbp;
    /* 0x38 */ u32 cpsm;
    /* 0x3c */ u32 csm;
    /* 0x40 */ u32 csa;
    /* 0x44 */ u32 cld;
    /* 0x48 */ u8 *gsClut;
    /* 0x4c */ u16 *ipuClut;
    /* 0x50 */ u16 killFg;
    /* 0x54 */ u32 *top;
    /* 0x58 */ u8 *bsData;
    /* 0x5c */ u32 bsData1Addr;
    /* 0x60 */ u32 bsData1Size;
    /* 0x64 */ u128 *bsTags;
    /* 0x6c */ u128 *dmaTags[2];
    /* 0x74 */ sceIpuRGB32 *cscBuff[2];
    /* 0x7c */ sceIpuRAW16 *idctBuff[2];
    /* 0x84 */ sceIpuINDX4 *vqBuff[2];
    /* 0x88 */ s32 currentBufNo;
} vtIDEC_MOVIE;

typedef struct { // 0x190
    /* 0x000 */ vtIDEC_MOVIE movie;
    /* 0x088 */ char name[256];
    /* 0x188 */ u32 vram_addr;
} taroMovieStruct;

#define SET_DMACNT(size) (DMAcnt | size)
#define SET_DMAREF(size, address) (DMAref | size | (u64)(address & 0x0FFFFFFF) << 32)
#define SET_DMAREF2(size, address) (DMAref | size | (u64)(address) << 32)

#endif
