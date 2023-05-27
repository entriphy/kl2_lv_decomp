#ifndef KAZUYA_H
#define KAZUYA_H

#include "types.h"

typedef enum {
    DL_STATUS_NML_LOAD,
    DL_STATUS_NML_TRANS,
    DL_STATUS_NML_END,
    DL_STATUS_CMN_LOAD,
    DL_STATUS_CMN_END,
    DL_STATUS_NUM,
} DL_STATUS;

// Functions

// bios.c
extern s32 kz_Get_KazuyaStageNo();

// disp.c
extern void kzSetDispMaskOn();
extern void kzSetDispMaskOff();

// interfac.c
extern void kzLoadCommonGms();
extern void kzInitNowload();
extern s32 kzDrawNowload();

// kzStageInit0;
extern void kzStageInit0();

#endif
