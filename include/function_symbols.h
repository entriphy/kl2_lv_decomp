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

// C++ section?

/* FUN_00278c70 */ extern void abStageInitA();
/* FUN_002da268 */ extern void hMovInit();
/* FUN_002db4a0 */ extern void nkInitPad();
/* FUN_002edfe0 */ extern void TkMainLoop(int tick);

#endif
