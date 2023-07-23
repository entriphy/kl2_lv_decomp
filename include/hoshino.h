#ifndef HOSHINO_H
#define HOSHINO_H

#include "common.h"

#pragma region Structs

typedef struct {
    s32 start;
    s32 chsize;
    s32 chnum;
} BGMTABLE;

typedef struct {
    s32 top;
    s32 nsector;
} PPTTABLE;

typedef struct {
    s32 lsn;
    s32 sectors;
} KLTABLE; // ?

typedef struct { // 0x10
    /* 0x0 */ u16 TBP : 14;
    /* 0x1 */ u32 : 2;
    /* 0x2 */ u32 TBW : 6;
    /* 0x2 */ u32 : 2;
    /* 0x3 */ u32 PSM : 6;
    /* 0x3 */ u32 : 2;
    /* 0x4 */ u32 : 32;
    /* 0x8 */ u32 PX : 11;
    /* 0x9 */ u32 : 5;
    /* 0xa */ u32 PY : 11;
    /* 0xb */ u32 : 5;
    /* 0xc */ u32 PW : 12;
    /* 0xd */ u32 : 4;
    /* 0xe */ u32 PH : 12;
    /* 0xf */ u32 : 4;
} hGIM_PHEAD;

typedef struct { // 0x10
    /* 0x0 */ u16 CBP : 14;
    /* 0x1 */ u32 : 2;
    /* 0x2 */ u32 CBW : 6;
    /* 0x2 */ u32 : 2;
    /* 0x3 */ u32 CPSM : 6;
    /* 0x3 */ u32 : 2;
    /* 0x4 */ u32 : 32;
    /* 0x8 */ u32 CX : 11;
    /* 0x9 */ u32 : 5;
    /* 0xa */ u32 CY : 11;
    /* 0xb */ u32 : 5;
    /* 0xc */ u32 CW : 12;
    /* 0xd */ u32 : 4;
    /* 0xe */ u32 CH : 12;
    /* 0xf */ u32 : 4;
} hGIM_CHEAD;

#pragma endregion Structs

#endif
