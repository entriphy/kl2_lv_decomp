#ifndef HR_PREAK_H
#define HR_PREAK_H

#include "harada.h"

typedef enum {
    PTPOSJ_G,
    PTPOSJ_GE,
    PTPOSJ_E,
    PTPOSJ_LE,
    PTPOSJ_L,
    PTPOSJ_NE,
    PTPOSJ_NUM
} PTPOSJ;

typedef enum {
    PTKEY_TRG,
    PTKEY_LVL,
    PTKEY_SYS,
    PTKEY_NUM
} PTKEY;

extern s32 (*HrPtKl2Tbl[36])(HR_CALL *ca, HR_PSYS *ps);

extern void hr_rt_movework(HR_CALL *ca);
extern void hr_pcamsr_spdclr(PCAMSR *pcr);

#endif
