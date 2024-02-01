#ifndef KUROSUKE_H
#define KUROSUKE_H

#include "take.h"

typedef struct { // 0x7b0
    /* 0x000 */ SFXOBJ *pObj;
    /* 0x010 */ qword_uni GsEnv[16];
    /* 0x110 */ s32 GsEnvInd;
    /* 0x118 */ u64 Tex0;
    /* 0x120 */ s16 px;
    /* 0x122 */ s16 py;
    /* 0x124 */ s16 ph;
    /* 0x126 */ s16 pw;
    /* 0x128 */ s16 Index;
    /* 0x12a */ s16 Cnt[24];
    /* 0x160 */ sceVu0FVECTOR Pos[24];
    /* 0x2e0 */ sceVu0FVECTOR TargetPos[24];
    /* 0x460 */ sceVu0FVECTOR Vector[24];
    /* 0x5e0 */ f32 Xrot[24];
    /* 0x640 */ f32 Yrot[24];
    /* 0x6a0 */ f32 Zrot[24];
    /* 0x700 */ f32 Range[24];
    /* 0x760 */ s16 Mode[24];
    /* 0x790 */ f32 InRange;
    /* 0x794 */ f32 OutRange;
    /* 0x798 */ f32 TopSpeed;
    /* 0x79c */ s16 TotalMode;
    /* 0x79e */ u8 Alpha;
    /* 0x7a0 */ f32 Speed;
} KUROSUKE;

extern OBJWORK* SetKurosuke(SFXOBJ *pObj);
extern void SetKurosukeMode(OBJWORK *pObjw, s32 mode);

#endif