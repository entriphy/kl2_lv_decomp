#ifndef SP_JUMP_H
#define SP_JUMP_H

#include "take.h"

typedef struct {
    sceVu0FVECTOR Vector[180];
    sceVu0FVECTOR Pos[180];
    s8 OnFlag[180];
    f32 Size[180];
    s8 type[180];
    u64 tex0[8];
    s16 px[8];
    s16 py[8];
    s16 ph[8];
    s16 pw[8];
    s16 Cnt[180];
    u8 Alpha[180];
    s32 Num;
    SFXOBJ *pObj;
    qword_uni GsEnv[16];
    s32 GsEnvInd;
    s32 cnt;
    sceVu0FVECTOR LastPos;
    s16 TotalCnt;
} SPJUMP;

extern OBJWORK* SetSpJump(SFXOBJ *pObj);

#endif