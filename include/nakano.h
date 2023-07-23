#ifndef NAKANO_H
#define NAKANO_H

#include "common.h"

#pragma region Defines

#define nkSPR (nkQWdata *)0x70000000

#pragma endregion Defines

#pragma region Structs

typedef union {
    u128 ul128;
    u64 ul64[2];
    u32 ul32[4];
    sceVu0IVECTOR vect;
    sceVu0FVECTOR fvct;
} nkQWdata;

typedef struct {
    nkQWdata *buf;
    u32 size;
} nkGifPacket;

#pragma endregion Structs

#endif
