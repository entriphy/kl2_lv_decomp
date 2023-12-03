#ifndef NK_GSSUB_H
#define NK_GSSUB_H

#include "nakano.h"

typedef struct { // 0x28
    /* 0x00 */ tGS_PMODE pmode;
    /* 0x08 */ tGS_SMODE2 smode2;
    /* 0x10 */ tGS_DISPFB1 dispfb;
    /* 0x18 */ tGS_DISPLAY1 display;
    /* 0x20 */ tGS_BGCOLOR bgcolor;
} nkGsDispEnv;

typedef struct { // 0x2c0
    /* 0x000 */ nkGsDispEnv dispenv;
    /* 0x028 */ s32 size_draw;
    /* 0x030 */ sceGifTag gif_draw;
    /* 0x040 */ sceGsDrawEnv1 drawenv1;
    /* 0x0c0 */ sceGsDrawEnv2 drawenv2;
    /* 0x140 */ sceGsClear gsclear;
    /* 0x1a0 */ s32 size_view;
    /* 0x1b0 */ sceGifTag gif_view;
    /* 0x1c0 */ sceGsDrawEnv1 viewenv1;
    /* 0x240 */ sceGsDrawEnv2 viewenv2;
} nkSCREEN;

typedef struct { // 0x100
    /* 0x00 */ sceGifTag gif_tex0;
    /* 0x10 */ sceGsTex0 tex0;
    /* 0x18 */ sceGsTest test;
    /* 0x20 */ sceGifTag gif_iamge;
    /* 0x30 */ sceGsLoadImage image;
    /* 0x90 */ s32 size_texenv;
    /* 0xa0 */ sceGifTag gif_texenv;
    /* 0xb0 */ sceGsTexEnv texenv;
    /* 0xf0 */ u64 alpha;
    /* 0xf8 */ u64 alp_addr;
} nkFFDRAW;

extern void nkGsDrawFF(s32 inter);
extern void nkGsInitFrm();
extern void nkGsInitFF();
extern void nkGsSetDrawEnv();
extern void nkGsSetEffectFZ2();
extern void nkGsSetNormalFZ2_0();
extern void nkGsSetNormalFZ2_1();
extern void nkGsSetNormalFZ2();
extern void nkGsSetFBA1(s32 fba);
extern void nkGsInitEnv();
extern void _nkDrawTTriangle(sceVu0FVECTOR vf0, sceVu0FVECTOR vf1, sceVu0FVECTOR vf2, sceVu0FVECTOR posi, sceVu0FVECTOR rot, sceVu0IVECTOR rgba);
extern void _nkDraw3DLine(sceVu0FVECTOR vf0, sceVu0FVECTOR vf1, u32 c0, u32 c1);

#endif
