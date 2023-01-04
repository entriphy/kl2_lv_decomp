#ifndef COMMON_H
#define COMMON_H

#include "gim.h"
#include "tobj.h"
#include "types.h"

typedef struct OBJWORK OBJWORK;

typedef void (*OBJWORK__delegatePers)();
typedef void (*OBJWORK__delegateDraw)(OBJWORK*);
typedef void (*OBJWORK__delegateDrMir)(OBJWORK*);
typedef void (*OBJWORK__delegateDrMirEf)(OBJWORK*);
typedef void (*OBJWORK__delegateDrEff)(OBJWORK*);

typedef struct {} GeneralWork;
typedef struct {} Prim;

// public const int DATA_OKANOYO = 1;
// public const int DATA_NAKANO = 0;
// public const int DATA_OKANO = 1;
// public const int DATA_HOSHINO = 2;
// public const int DATA_HARADA = 3;
// public const int DATA_ABE = 4;
// public const int DATA_HATO = 5;
// public const int DATA_TAKE = 6;
// public const int DATA_KAZUYA = 7;

// Size: 0xF0
struct OBJWORK {
    OBJWORK__delegatePers pers; // 0x00
    OBJWORK__delegateDraw draw; // 0x04
    OBJWORK__delegateDrMir drmir; // 0x08
    OBJWORK__delegateDrMirEf drmiref; // 0x0C
    OBJWORK__delegateDrEff dreff; // 0x10
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
};

void sce_print(const char* fmt, ...);

#endif