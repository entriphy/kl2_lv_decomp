#ifndef GSCREEN_H
#define GSCREEN_H

#include "nakano.h"

typedef struct { // 0x20
    /* 0x00 */ s32 k0heart;
    /* 0x04 */ s32 k0yume;
    /* 0x08 */ s32 k0zanki;
    /* 0x0c */ s32 k0exitm;
    /* 0x10 */ f32 htime;
    /* 0x14 */ f32 ytime;
    /* 0x18 */ f32 ztime;
    /* 0x1c */ f32 ztime2;
    /* 0x20 */ f32 R1time;
    /* 0x24 */ f32 extime;
    /* 0x28 */ f32 mm_rot;
    /* 0x2c */ f32 gus_cnt;
    /* 0x30 */ s32 yume150_flag;
} nkGameScrInfo;

typedef struct { // 0x18
    /* 0x00 */ u64 tex0;
    /* 0x08 */ s32 px;
    /* 0x0c */ s32 py;
    /* 0x10 */ s32 pw;
    /* 0x14 */ s32 ph;
} nkTEXIF;

extern void nkGameDisp(s32 flag);
extern void SetGScreenLight();
extern void InitStageGScreen();
extern void InitGScreen();
extern void nkGScreenMatrix();
extern void nkGScreenMatrixXY(f32 x, f32 y);
extern void DrawGameScrHeart();
extern void DrawGameScrZanki(s32 boss_flag);
extern void DrawMomePanel(nkGifPacket *pp, s32 x, s32 y, nkTEXIF *ti, s32 n);
extern void DrawKagiPanel(nkGifPacket *pp, s32 x, s32 y, nkTEXIF *ti, s32 n);
extern void DrawGameScrKagi();
extern void DrawGusMeterPanel(nkGifPacket *pp, s32 x, s32 y, nkTEXIF *ti, s32 n);
extern void DrawGusMeterBar(nkGifPacket *pp);
extern void DrawEngSysMeterPanel(nkGifPacket *pp, s32 x, s32 y, nkTEXIF *ti, s32 n);
extern void DrawEngSysMeterBar(nkGifPacket *pp);
extern void DrawGameScreen();

#endif