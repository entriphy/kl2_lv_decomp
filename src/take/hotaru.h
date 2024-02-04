#ifndef HOTARU_H
#define HOTARU_H

#include "take.h"

typedef struct {
    f32 Size[32];
    s32 Num;
    s32 Mode;
    SFXOBJ *pObj1;
    SFXOBJ *pObj2;
    qword_uni GsEnv[16];
    s32 GsEnvInd;
    u64 tex0;
    s32 cnt;
    s16 px;
    s16 py;
    s16 ph;
    s16 pw;
    s16 TotalCnt;
    f32 TargetAlpha;
    f32 TargetAlpha1;
    f32 TargetAlpha2;
    u8 Alpha[32];
    s32 OnFlag[32];
    s16 Cnt[32];
    sceVu0FVECTOR Pos[32];
    sceVu0FVECTOR TargetPos[32];
    sceVu0FVECTOR Vector[32];
    f32 TopSpeed;
} HOTARU;

extern OBJWORK* SetHotaruLight(SFXOBJ *pObj1, SFXOBJ *pObj2);
extern void SetHotaruLightMode(OBJWORK *pObjw, s32 mode);
extern void EraseHotaruLight(OBJWORK *pObjw);
extern void SetHotaruLightModeTest(OBJWORK *pObjw);

#endif