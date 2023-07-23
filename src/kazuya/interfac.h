#ifndef KZ_INTERFAC_H
#define KZ_INTERFAC_H

#include "kazuya.h"

typedef enum {
    DL_STATUS_NML_LOAD,
    DL_STATUS_NML_TRANS,
    DL_STATUS_NML_END,
    DL_STATUS_CMN_LOAD,
    DL_STATUS_CMN_END,
    DL_STATUS_NUM,
} DL_STATUS;

extern void kzLoadCommonGms();
extern void kzInitNowload();
extern s32 kzDrawNowload();

#endif
