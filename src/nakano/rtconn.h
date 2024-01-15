#ifndef RTCONN_H
#define RTCONN_H

#include "nakano.h"

#define rcSTART 1
#define rcDOOR 2
#define rcZONEE 3
#define rcSTAGEE 4
#define rcKLOMODE 5

typedef struct { // 0x20
    /* 0x00 */ s16 irtn;
    /* 0x02 */ s16 imuki;
    /* 0x04 */ s16 nextst;
    /* 0x06 */ s16 ortn;
    /* 0x08 */ s32 irtcnt;
    /* 0x0c */ s32 ortcnt;
    /* 0x10 */ s16 omuki;
    /* 0x12 */ s16 mode;
    /* 0x14 */ s32 iheight;
    /* 0x18 */ s32 oheight;
    /* 0x1c */ s16 flag;
    /* 0x1e */ s16 fdata;
} RT_CONN;

#endif
