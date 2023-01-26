#ifndef OBJWORK_H
#define OBJWORK_H

#include "gim.h"
#include "tobj.h"
#include "types.h"

typedef struct {} GeneralWork;
typedef struct {} Prim;

// Size: 0xF0
typedef struct OBJWORK {
    void (*pers)(struct OBJWORK *); // 0x00
    void (*draw)(struct OBJWORK *); // 0x04
    void (*drmir)(struct OBJWORK *); // 0x08
    void (*drmiref)(struct OBJWORK *); // 0x0C
    void (*dreff)(struct OBJWORK *); // 0x10
    Prim* prim; // 0x14, klMODEL, 
    GeneralWork* work; // 0x18
    s16 stat0; // 0x1C
    s16 stat1; // 0x1E
    s16 prty; // 0x20
    s16 hitram; // 0x22
    s16 bun0; // 0x24
    s16 bun1; // 0x26
    s16 pad0; // 0x28
    s16 pad1; // 0x2A
    s16 live; // 0x2C
    s16 stat; // 0x2E
    FVECTOR posi; // 0x30
    FVECTOR spd; // 0x40
    FVECTOR muki; // 0x50
    FVECTOR ang; // 0x60
    FVECTOR rot; // 0x70
    FVECTOR rtw; // 0x80
    s32 reg0; // 0x90
    s32 reg1; // 0x94
    s32 reg2; // 0x98
    s32 reg3; // 0x9C
    s32 reg4; // 0xA0
    s32 reg5; // 0xA4
    s32 reg6; // 0xA8
    s32 reg7; // 0xAC
    float freg0; // 0xB0
    float freg1; // 0xB4
    float freg2; // 0xB8
    float freg3; // 0xBC
    float freg4; // 0xC0
    float freg5; // 0xC4
    float freg6; // 0xC8
    float freg7; // 0xCC
    s64 htblid; // 0xD0
    s64 dummy; // 0xD8
    s16 option; // 0xE0
    s16 sflag; // 0xE2
    u8* hpo; // 0xE4
    GIMWORK* gimmick; // 0xE8
    tOBJECT* _hOp; // 0xEC
} OBJWORK;

#endif