#ifndef BERNY_H
#define BERNY_H

#include "take.h"

typedef struct { // 0x290
    /* 0x000 */ sceVu0FMATRIX BaseMatrix;
    /* 0x040 */ SFXOBJ *pObj;
    /* 0x044 */ SFXOBJ *pObjKl;
    /* 0x050 */ qword_uni GsEnv[16];
    /* 0x150 */ s32 GsEnvInd;
    /* 0x154 */ s16 px[4];
    /* 0x15c */ s16 py[4];
    /* 0x164 */ s16 ph[4];
    /* 0x16c */ s16 pw[4];
    /* 0x178 */ u64 tex0[4];
    /* 0x198 */ s32 Cnt[5][4];
    /* 0x1e8 */ s16 x[5][4];
    /* 0x210 */ s16 y[5][4];
    /* 0x238 */ u8 Alpha[5][4];
    /* 0x24c */ f32 Angle[5];
    /* 0x260 */ f32 Scale[5];
    /* 0x274 */ f32 Radius;
    /* 0x278 */ f32 Speed;
    /* 0x27c */ f32 SpinAngle[5];
} BERNY;

extern OBJWORK* SetBernyFire(SFXOBJ *pObj);
extern void SetBernyData(OBJWORK *pObjw, f32 Radius, f32 Speed);
extern void EraseBernyFire(OBJWORK *pObjw);

#endif
