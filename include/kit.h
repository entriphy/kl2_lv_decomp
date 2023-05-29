#ifndef KIT_H
#define KIT_H

#include "ps2.h"
#include "types.h"

typedef struct { // 0x10
	/* 0x0 */ u64 data;
	/* 0x8 */ u64 addr;
} kitADDR_DATA;

typedef struct { // 0x10
	/* 0x0 */ u64 lower;
	/* 0x8 */ u64 upper;
} kitGIFTAG;

typedef struct { // 0x10
	/* 0x0 */ u64 lower;
	/* 0x8 */ u64 upper;
} kitPACKED;

typedef struct { // 0x60
	/* 0x00 */ sceVu0IVECTOR bbox[4];
	/* 0x40 */ u32 fr_page;
	/* 0x44 */ u32 fr_block;
	/* 0x48 */ u64 xyoffset;
	/* 0x50 */ u64 spritez;
	/* 0x58 */ u32 partsnum;
	/* 0x5c */ u8 status;
	/* 0x5d */ u8 alpha;
} kitOutlineDrawEnv;

typedef struct { // 0x8
	/* 0x0 */ u32 partsmax;
	/* 0x4 */ u8 *status;
} kitOutlineObjEnv;

typedef struct { // 0x40
	/* 0x00 */ kitADDR_DATA giftag;
	/* 0x10 */ kitADDR_DATA frame;
	/* 0x20 */ kitADDR_DATA zbuf;
	/* 0x30 */ kitADDR_DATA xyoffset;
} kitEffectBufEnablePacket;

typedef struct { // 0x40
	/* 0x00 */ kitADDR_DATA giftag1;
	/* 0x10 */ kitADDR_DATA frame;
	/* 0x20 */ kitADDR_DATA zbuf;
	/* 0x30 */ kitADDR_DATA xyoffset;
} kitEffectBufDisablePacket;

typedef struct { // 0x80
	/* 0x00 */ kitADDR_DATA giftag1;
	/* 0x10 */ kitADDR_DATA test1;
	/* 0x20 */ kitADDR_DATA zubf1;
	/* 0x30 */ kitADDR_DATA alpha1;
	/* 0x40 */ kitADDR_DATA sprite[2];
	/* 0x60 */ kitADDR_DATA zbuf2;
	/* 0x70 */ kitADDR_DATA test2;
} kitInitEffectBufBPacket;

typedef struct { // 0x4b0
	/* 0x000 */ kitADDR_DATA giftag1;
	/* 0x010 */ kitADDR_DATA test1;
	/* 0x020 */ kitADDR_DATA rgba;
	/* 0x030 */ kitADDR_DATA fba;
	/* 0x040 */ kitADDR_DATA giftag2;
	/* 0x050 */ kitADDR_DATA sprite[70];
} kitInitEffectBufAllPacket;

typedef struct { // 0x70
	/* 0x00 */ kitADDR_DATA giftag1;
	/* 0x10 */ kitADDR_DATA tex0;
	/* 0x20 */ kitADDR_DATA texflush;
	/* 0x30 */ kitADDR_DATA test1;
	/* 0x40 */ kitADDR_DATA giftag2;
	/* 0x50 */ kitADDR_DATA tri[2];
} kitDrawSpritePacket;

typedef struct { // 0x60
	/* 0x00 */ kitADDR_DATA giftag1;
	/* 0x10 */ kitADDR_DATA clamp1;
	/* 0x20 */ kitADDR_DATA tex01;
	/* 0x30 */ kitADDR_DATA frame;
	/* 0x40 */ kitADDR_DATA fba;
	/* 0x50 */ kitADDR_DATA alpha1;
} kitDrawSpriteRegSetPacket;

typedef struct { // 0xa0
	/* 0x00 */ kitADDR_DATA giftag1;
	/* 0x10 */ kitADDR_DATA test1;
	/* 0x20 */ kitADDR_DATA frame1;
	/* 0x30 */ kitADDR_DATA zbuf1;
	/* 0x40 */ kitADDR_DATA alpha1;
	/* 0x50 */ kitADDR_DATA sprite[2];
	/* 0x70 */ kitADDR_DATA frame2;
	/* 0x80 */ kitADDR_DATA zbuf2;
	/* 0x90 */ kitADDR_DATA test2;
} kitInitAlphaPlanePacket;

#endif
