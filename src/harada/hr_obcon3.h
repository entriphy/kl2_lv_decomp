#ifndef HR_OBCON3_H
#define HR_OBCON3_H

#include "harada.h"

typedef struct HRPT_EFC { // 0x40
    /* 0x00 */ sceVu0FVECTOR pos;
    /* 0x10 */ sceVu0FVECTOR rot;
    /* 0x20 */ s32 id;
    /* 0x24 */ void (*draw)(struct HRPT_EFC *);
    /* 0x28 */ void (*work)(struct HRPT_EFC *);
    /* 0x2c */ s32 bun;
    /* 0x30 */ s32 reg0;
    /* 0x34 */ s32 reg1;
    /* 0x38 */ s32 reg2;
    /* 0x3c */ s32 reg3;
} HRPT_EFC;

#endif
