#ifndef FUNCIONS_H
#define FUNCIONS_H

#include "common.h"

/* FUN_001227b0 */ extern void htInitRand(int seed);
/* FUN_001227c8 */ extern int htGetRand();
/* FUN_00165ae0 */ extern void* hReadFile(const char* name); 
/* FUN_00166128 */ extern int FUN_00166128(int param_1);
/* FUN_00166140 */ extern void FUN_00166140(int param_1, void *param_2);
/* FUN_00166248 */ extern void hCdInit();
/* FUN_00166248 */ extern void FUN_00167c20(void *buf);
/* FUN_00167bd0 */ extern int FUN_00167bd0(int param_1);
/* FUN_00168e48 */ extern void hSeLock(int i);
/* FUN_00168fd8 */ extern void hSeInitGrp(int stage);
/* FUN_0016ac00 */ extern void hInitBoot();
/* FUN_0016c778 */ extern int FUN_0016c778();
/* FUN_0016c798 */ extern void FUN_0016c798();
/* FUN_0016c830 */ extern int* hIopDispatch(u32 param);
/* FUN_0016ca30 */ extern void hSndPkEffect();
/* FUN_0016cb40 */ extern void hSndPkSetMVol(int voll, int volr);
/* FUN_0016cbc8 */ extern void hSndPkSetEVol(int vol);
/* FUN_0016cfe0 */ extern void hSndSetMVol(float vol);
/* FUN_0016d008 */ extern int hSndFader(float vol);
/* FUN_0016d0b8 */ extern float hSndFader2(float vol);
/* FUN_0016e878 */ extern int hBgmGetStat();
/* FUN_0016ef10 */ extern void hStrInfo();
/* FUN_0016c6e8 */ extern void FUN_0016c6e8();
/* FUN_0016e350 */ extern void hBgmWorkClear();
/* FUN_0016cec8 */ extern int hSndPkGetSize();
/* FUN_0016d4a8 */ extern void hSndInit();
/* FUN_00196c00 */ extern void FUN_00196c00();
/* FUN_00198a28 */ extern void FUN_00198a28(DecompressionStructure *scratchpad);
/* FUN_001990c0 */ extern u16 FUN_001990c0(DecompressionStructure *scratchpad, s16 param_2);
/* FUN_00199100 */ extern void FUN_00199100(DecompressionStructure *scratchpad, u16 param_2);
/* FUN_001994e0 */ extern void init_config_system();
/* FUN_001d1c08 */ extern int FUN_001d1c08(const char* name);
/* FUN_001d1c78 */ extern int FUN_001d1c78(const char* name, void* buf);
/* FUN_001d31a0 */ extern void FUN_001d31a0();
/* FUN_001d3748 */ extern void* getBuff(int type, int byte_, const char* name, int* ret);
/* FUN_002da268 */ extern void hStrInit();
/* FUN_0030dad0 */ extern int FUN_0030dad0();
/* FUN_00318a80 */ extern void FUN_00318a80();
/* FUN_003189d0 */ extern void FUN_003189d0();

#endif