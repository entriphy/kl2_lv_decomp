#ifndef HRMENU_H
#define HRMENU_H

#include "harada.h"

typedef struct { // 0x10
    /* 0x0 */ s32 mode;
    /* 0x4 */ s32 vision;
    /* 0x8 */ s32 flag;
    /* 0xc */ s32 vscnt;
} MENU_WORK;

extern s32 (*hrMenuFuncTbl[2])();

#endif
