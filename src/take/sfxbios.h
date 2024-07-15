#ifndef TAKE_SFXBIOS_H
#define TAKE_SFXBIOS_H

#include "take.h"

extern u32* Fadr(u32 *pAddr, s32 nNum);
extern s32 DataRead(char *name, char *buff);
extern void GmsLoad(qword *gms);
extern void AlphaEnvClear();
extern s32 ReadFile(char *fn, qword *addr);
extern void Vu0ProgTrans();
extern void WorkMemInit();
extern u32 UnitAlign(u32 Fig);
extern s32 CheckMem();
extern void func_00213088();
extern u32 GetMem(u32 Fig);
extern void OpenMem(u32 *Adrs);
extern void SfxSystemInit(u32 *Buff);
extern void GimLoad(u32 *addr, GIMINFO *info);

#endif
