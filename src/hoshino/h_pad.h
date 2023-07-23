#ifndef H_PAD_H
#define H_PAD_H

#include "hoshino.h"

typedef struct { // 0xe4
    /* 0x00 */ s32 fd;
    /* 0x04 */ u32 old;
    /* 0x08 */ u32 l;
    /* 0x0c */ u32 on;
    /* 0x10 */ u32 off;
    /* 0x14 */ u32 r;
    /* 0x18 */ u32 r2;
    /* 0x1c */ s32 rep_on[16];
    /* 0x5c */ s32 rcount[16];
    /* 0x9c */ s32 rcount2[16];
    /* 0xdc */ s32 rwait0;
    /* 0xe0 */ s32 rwait1;
} hPAD_DATA;

extern void hPadInit();
extern void hPadRead();
extern void hPadMake(hPAD_DATA *pp);
extern void hPadGet(hPAD_DATA *pad, s32 i);

#endif
