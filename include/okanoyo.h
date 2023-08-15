#ifndef OKANOYO_H
#define OKANOYO_H

#include "common.h"

typedef struct { // 0x30
    /* 0x00 */ u8 l0;
    /* 0x01 */ u8 l1;
    /* 0x02 */ s16 rno;
    /* 0x04 */ f32 lip;
    /* 0x08 */ f32 ypos;
    /* 0x0c */ s32 mcn;
    /* 0x10 */ s32 type;
    /* 0x20 */ sceVu0FVECTOR pos;
} LT_WORK;

typedef struct { // 0xc0
    /* 0x00 */ s16 xzdis;
    /* 0x02 */ s16 ydis;
    /* 0x04 */ u16 timer;
    /* 0x06 */ u16 count;
    /* 0x08 */ s16 flag;
    /* 0x0a */ s16 flag2;
    /* 0x0c */ s16 flag3;
    /* 0x0e */ s16 tekipic;
    /* 0x10 */ void *movetbl;
    /* 0x14 */ s16 look;
    /* 0x16 */ s16 bomtimer;
    /* 0x18 */ s32 motno;
    /* 0x1c */ OBJWORK *prt;
    /* 0x20 */ s32 seno[4];
    /* 0x30 */ s32 sedat[4];
    /* 0x40 */ OBJWORK *movebox;
    /* 0x44 */ f32 gspd;
    /* 0x48 */ OBJWORK *gmp;
    /* 0x4c */ s32 gimspd;
    /* 0x50 */ LT_WORK ltw;
    /* 0x80 */ f32 rt_ofs;
    /* 0x84 */ f32 rt_spd;
    /* 0x88 */ s64 jumpspd;
    /* 0x90 */ s16 res0;
    /* 0x92 */ s16 res1;
    /* 0x94 */ s32 test0;
    /* 0x98 */ s32 test1;
    /* 0xa0 */ s64 test2;
    /* 0xa8 */ s64 juryoku;
    /* 0xb0 */ s64 otoM;
} ZAKO_WORK;

#endif
