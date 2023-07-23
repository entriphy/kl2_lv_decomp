#ifndef ABE_H
#define ABE_H

#include "common.h"

#pragma region Defines

#define abSPR (abQWdata *)0x70000000

#pragma endregion Defines

#pragma region Structs

typedef union { // 0x10
	/* 0x0 */ u128 ul128;
	/* 0x0 */ u64  ul64[2];
	/* 0x0 */ u32  ul32[4];
	/* 0x0 */ s32  vect[4];
} abQWdata;

typedef struct { // 0xc
	/* 0x0 */ abQWdata *buf;
	/* 0x4 */ u32 size;
	/* 0x8 */ u32 *pBase;
} abGifPacket;

#pragma endregion Structs

#endif


