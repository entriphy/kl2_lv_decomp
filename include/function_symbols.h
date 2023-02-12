#ifndef FUNCIONS_H
#define FUNCIONS_H

#include "common.h"

/* FUN_00102808 */ extern void hr_cold_start();
/* FUN_00110dc0 */ extern void hr_pt_set(short flag, short scene, short view, short th);
/* FUN_00110e08 */ extern void hr_pt_fclear();
/* FUN_00113408 */ extern void hr_pflag_initAr();
/* FUN_00113420 */ extern void hr_pflag_initSt();
/* FUN_00113448 */ extern void hr_pflag_init();
/* FUN_001137f8 */ extern PT hr_pflag_get_id();
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
/* FUN_0016ad90 */ extern void hLoopTop();
/* FUN_0016c778 */ extern int FUN_0016c778();
/* FUN_0016c798 */ extern void FUN_0016c798();
/* FUN_0016c830 */ extern int* hIopDispatch(u32 param);
/* FUN_0016c9b8 */ extern s32 FUN_0016c9b8(void *dest, void *src, u32 size);
/* FUN_0016ca20 */ extern u32 JamGetHdSize(void *hdaddr);
/* FUN_0016ca28 */ extern u32 JamGetBdSize(void *hdaddr);
/* FUN_0016ca30 */ extern void hSndPkEffect();
/* FUN_0016cb40 */ extern void hSndPkSetMVol(int voll, int volr);
/* FUN_0016cbc8 */ extern void hSndPkSetEVol(int vol);
/* FUN_0016cfe0 */ extern void hSndSetMVol(float vol);
/* FUN_0016d008 */ extern int hSndFader(float vol);
/* FUN_0016d0b8 */ extern float hSndFader2(float vol);
/* FUN_0016e878 */ extern int hBgmGetStat();
/* FUN_0016ef10 */ extern void hStrInfo();
/* FUN_0016f360 */ extern void hStrInit();
/* FUN_0016c6e8 */ extern void FUN_0016c6e8();
/* FUN_0016e350 */ extern void hBgmWorkClear();
/* FUN_0016cec8 */ extern int hSndPkGetSize();
/* FUN_0016d4a8 */ extern void hSndInit();
/* FUN_0016d5f0 */ extern int* FUN_0016d5f0(u8 *param_1, int param_2);
/* FUN_0016d710 */ extern void FUN_0016d710();
/* FUN_00175630 */ extern void kzInitNowload();
/* FUN_0018d9e8 */ extern void nkInitDma();
/* FUN_0018fc18 */ extern void nkInitPS2();
/* FUN_0018fca0 */ extern int GameInit();
/* FUN_0018fd28 */ extern int GameLoad();
/* FUN_0018ff58 */ extern int GameMain();
/* FUN_00196c00 */ extern void FUN_00196c00();
/* FUN_00197830 */ extern void nkGsSetNormalFZ2();
/* FUN_001979b0 */ extern void FUN_001979b0(int param_1);
/* FUN_00197a88 */ extern void FUN_00197a88();
/* FUN_00198a28 */ extern void FUN_00198a28(DecompressionStructure *scratchpad);
/* FUN_001990c0 */ extern u16 FUN_001990c0(DecompressionStructure *scratchpad, s16 param_2);
/* FUN_00199100 */ extern void FUN_00199100(DecompressionStructure *scratchpad, u16 param_2);
/* FUN_001994e0 */ extern void init_config_system();
/* FUN_00199560 */ extern void nkInitSys();
/* FUN_00199810 */ extern void nkKeyFrameStart();
/* FUN_001a2e20 */ extern int main(int argc, char *argv[]);
/* FUN_001a4fc0 */ extern int nkInitAtrInit(); // i made up this name idk
/* FUN_001a50b0 */ extern int nkInitAtrMain();
/* FUN_001a50b8 */ extern void MainFunc();
/* FUN_001a56e0 */ extern int nkInit();
/* FUN_001a5700 */ extern int nkMain();
/* FUN_001b52a8 */ extern void nkWipeEffBClear();
/* FUN_001d1a60 */ extern void OkMainInit();
/* FUN_001d1c08 */ extern int FUN_001d1c08(const char* name);
/* FUN_001d1c78 */ extern int FUN_001d1c78(const char* name, void* buf);
/* FUN_001d3170 */ extern void memorySysFormat();
/* FUN_001d31a0 */ extern void FUN_001d31a0();
/* FUN_001d31b0 */ extern void memoryStageFormat();
/* FUN_001d3638 */ extern void FUN_001d3638(int i);
/* FUN_001d3710 */ extern void FUN_001d3710();
/* FUN_001d3748 */ extern void* getBuff(int type, int byte_, const char* name, int* ret);
/* FUN_001d37f8 */ extern void FUN_001d37f8(int param_1, int param_2, const char *param_3);
/* FUN_001da5a0 */ extern void OkPrintSysInit(int personal_id, int screenw, int screenh, int fontw, int fonth);
/* FUN_001da5e8 */ extern void OkDefaultSysInit(int screenw, int screenh, int fontw, int fonth);
/* FUN_002da268 */ extern void hMovInit();
/* FUN_002db4a0 */ extern void nkInitPad();
/* FUN_0030dad0 */ extern int FUN_0030dad0();
/* FUN_00318a80 */ extern void FUN_00318a80();
/* FUN_003189d0 */ extern void FUN_003189d0();

#endif
