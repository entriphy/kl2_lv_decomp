#ifndef VTMRG_H
#define VTMRG_H

#include "nakano.h"

// TODO: move this to kit/VT header
typedef struct { // 0x50
    /* 0x00 */ void *movie_data_buf[8];
    /* 0x20 */ void *wave_data_buf;
    /* 0x24 */ void *spray_data_buf;
    /* 0x28 */ sceGsTex0 tex0[4];
} vtDataPtr;

typedef struct { // 0x30
    /* 0x00 */ s8  field_0x00;
    /* 0x04 */ s32 field_0x04;
    /* 0x08 */ s32 field_0x08;
    /* 0x0c */ s32 field_0x0c;
    /* 0x10 */ s32 field_0x10;
    /* 0x14 */ s32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1c */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2c */ f32 field_0x2c;
} vtUnkStruct;

extern void nkVT_LoadVisionData(vtDataPtr *data, u32 vision);
extern void nkVT_Exec();
extern void nkVT_ExecMovie();
extern void nkVT_InitWave();

#endif
