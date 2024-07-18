#ifndef TK_KLSFXSUB_H
#define TK_KLSFXSUB_H

#include "take.h"

extern void SpecEnvInit();
extern void SetDefaultDrawEnv();
extern void EffBuffClear();
extern void SetEffBuff();
extern void func_00211C28(u64 Tex0, s32 u, s32 v, s32 x, s32 y);
extern void func_00211F50(sceVu0IVECTOR *Xyz, s32 col, u64 Tex0, s32 u0, s32 v0, s32 u1, s32 v1);

#endif