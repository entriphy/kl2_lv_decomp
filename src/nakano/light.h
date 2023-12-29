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

typedef struct { // 0x8
    /* 0x0 */ f32 x;
    /* 0x4 */ f32 y;
} nkLightDir;

typedef struct { // 0x30
    /* 0x00 */ nkLightDir dir;
    /* 0x10 */ sceVu0FVECTOR col;
    /* 0x20 */ s32 on;
} nkLightWrk;

typedef struct { // 0x10
    /* 0x0 */ s32 csl;
    /* 0x4 */ s32 colc;
    /* 0x8 */ s32 mode;
    /* 0xc */ s32 lightno;
} nkLeMenu;

typedef struct { // 0x28
    /* 0x00 */ s32 dir[3][2];
    /* 0x18 */ u8 col[3][4];
    /* 0x24 */ u8 amb[4];
} _nkLightData;

extern void nkLeInit();
extern void nkLightMenu(nkLightWrk *lline);
extern void nkLightEdit();
extern void LgtDataNone();
extern void LgtDataInit(s32 *adrs);
extern void nkCalcLightMatrix(sceVu0FVECTOR posi);
extern void _nkLightInit();
extern void nkLightIp(LgtWorkF *lgt_r, LgtWorkF *lgt_a, LgtWorkF *lgt_b, f32 w);
extern void nkLightWork2F(LgtWorkF *out, LgtWorkDat *in);
extern void nkMngLight(LgtWorkF *lgtw, sceVu0FVECTOR posi, RT_WRK *rtw);
extern void nkSetLight(OBJWORK *objw);
extern void nkSetLight2(RT_WRK *rtw, sceVu0FVECTOR posi, sceVu0FMATRIX **nl, sceVu0FMATRIX **lc);
extern void nkSetLight3(RT_WRK *rtw, sceVu0FVECTOR posi, sceVu0FMATRIX **nl, sceVu0FMATRIX **lc);
extern void nkGetRouteLgtWork(LgtWorkF *lgtw, RT_WRK *rtw, sceVu0FVECTOR posi);
extern void nkGetLgtWorkF(LgtWorkF *lgt, s32 light_no);
extern void nkCalcMatLgtWork(sceVu0FMATRIX normal_light, sceVu0FMATRIX light_color,  LgtWorkF *lgtw, sceVu0FVECTOR posi);
extern void nkCalcMatLgtWork2(sceVu0FMATRIX normal_light, sceVu0FMATRIX light_color,  LgtWorkF *lgtw, sceVu0FVECTOR posi);
extern void nkSetLightNo(sceVu0FMATRIX normal_light, sceVu0FMATRIX light_color, s32 light_no, sceVu0FVECTOR posi);

#endif
