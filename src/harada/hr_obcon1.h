#ifndef HR_OBCON1_H
#define HR_OBCON1_H

#include "harada.h"

typedef struct { // 0x10
    /* 0x0 */ s32 bun0;
    /* 0x4 */ s32 *ptr;
    /* 0x8 */ s32 reg0;
    /* 0xc */ s32 pad0;
} HR_OBJSW;

typedef enum {
    HROBJ_SW_TRG,
    HROBJ_SW_END,
    HR0402_CURSW,
    HROBJ_SW_NUM,
} HR_OBCT1;

typedef enum {
    PCAMW_PIA,
    PCAMW_IPA,
    PCAMW_PAI,
    PCAMW_IAP,
    PCAMW_NUM,
} PCAMW; // TODO: Put this in the correct header

extern void hr_swtrg_getset(OBJWORK *objw, s32 *tbl);
extern void hr_switch_trg(OBJWORK *objw);

#endif
