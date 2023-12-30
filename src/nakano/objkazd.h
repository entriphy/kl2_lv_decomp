#ifndef NK_OBJKAZD_H
#define NK_OBJKAZD_H

#include "nakano.h"

typedef struct { // 0x50
    /* 0x00 */ OBJWORK *yuka;
    /* 0x10 */ sceVu0FVECTOR yuposi;
    /* 0x20 */ s32 mispd;
    /* 0x24 */ f32 dx;
    /* 0x28 */ f32 dy;
    /* 0x2c */ f32 dz;
    /* 0x30 */ f32 modo_spd_x;
    /* 0x34 */ f32 modo_spd_y;
    /* 0x38 */ f32 modo_spd_z;
    /* 0x3c */ s16 timer;
    /* 0x3e */ s16 count;
    /* 0x40 */ s16 kaku_flag;
} BUKI_WORK;

extern void ObjKazd(OBJWORK *objw);
extern void SetKazd(OBJWORK *objkl);

#endif
