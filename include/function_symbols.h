#ifndef FUNCIONS_H
#define FUNCIONS_H

#include "common.h"

// Kazuya functions

/* FUN_00173130 */ extern s32 kz_Get_KazuyaStageNo();
/* FUN_00174da0 */ extern void kzLoadCommonGms();
/* FUN_00175630 */ extern void kzInitNowload();
/* FUN_00175690 */ extern int kzDrawNowload();
/* FUN_00175a58 */ extern void kzSetDispMaskOn();
/* FUN_00175a68 */ extern void kzSetDispMaskOff();
/* FUN_0017b7c8 */ extern void kzStageInit0();

// Nakano functions

/* FUN_0018d9e8 */ extern void nkInitDma();
/* FUN_0018dc78 */ extern void FUN_0018dc78(u8 *param_1);
/* FUN_0018dcb0 */ extern void nkLoadGms(u8 *param_1);
/* FUN_0018fc18 */ extern void nkInitPS2();
/* FUN_0018fca0 */ extern int GameInit();
/* FUN_0018fd28 */ extern int GameLoad();
/* FUN_0018ff58 */ extern int GameMain();
/* FUN_00196c00 */ extern void nkGsInitFrm();
/* FUN_00197570 */ extern void nkGsSetNormalFZ2_0();
/* FUN_001976d0 */ extern void nkGsSetNormalFZ2_1();
/* FUN_00197830 */ extern void nkGsSetNormalFZ2();
/* FUN_001979b0 */ extern void FUN_001979b0(int param_1);
/* FUN_00197a88 */ extern void FUN_00197a88();
/* FUN_001994e0 */ extern void init_config_system();
/* FUN_00199560 */ extern void nkInitSys();
/* FUN_00199810 */ extern void nkKeyFrameStart();
/* FUN_001a2e20 */ extern int main(int argc, char *argv[]);
/* FUN_001a4fc0 */ extern int MenuInit(); // i made up this name idk
/* FUN_001a50b0 */ extern int MenuMain();
/* FUN_001a50b8 */ extern void MainFunc();
/* FUN_001a56e0 */ extern int nkInit();
/* FUN_001a5700 */ extern int nkMain();
/* FUN_001b4570 */ extern void nkStageInit0();
/* FUN_001b4ab0 */ extern void nkStageInit1();
/* FUN_001b52a8 */ extern void nkWipeEffBClear();

// C++ section?

/* FUN_00278c70 */ extern void abStageInitA();
/* FUN_002da268 */ extern void hMovInit();
/* FUN_002db4a0 */ extern void nkInitPad();
/* FUN_002edfe0 */ extern void TkMainLoop(int tick);
/* FUN_0030dad0 */ extern int FUN_0030dad0();
/* FUN_00318a80 */ extern void FUN_00318a80();
/* FUN_003189d0 */ extern void FUN_003189d0();

#endif
