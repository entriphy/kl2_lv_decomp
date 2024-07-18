#ifndef TK_STAR_H
#define TK_STAR_H

#include "take.h"

typedef struct {
    /* 0x000 */ sceVu0FMATRIX BaseMatrix;
    /* 0x040 */ SFXOBJ *pObj;
    qword_uni GsEnv[16];
    s32 GsEnvInd;
    s16 px;
    s16 py;
    s16 ph;
    s16 pw;
    f32 Angle[5];
    f32 Scale[5];
    f32 Radius;
    f32 SpinAngle[5];
    s32 PartsNum;
} STAR;

extern OBJWORK* SetStar(SFXOBJ *pObj);
extern OBJWORK* SetPopStar(SFXOBJ *pObj);
extern void EraseStar(OBJWORK *pObjw);

#endif