#ifndef COMMON_H
#define COMMON_H

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Common headers
#include "ps2.h"
#include "defines.h"
#include "structs.h"
#include "types.h"
#include "gbl.h"
#include "objwork.h"
#include "sfx.h"

// Macros
#define SETVEC(vec, x, y, z, w) (vec[0] = x, vec[1] = y, vec[2] = z, vec[3] = w)
#define SPR_MEM ((void *)0x70000000)
#define SPR_MEM_IDX(i) ((void *)(0x70000000 + i * 0x10))
#define SPR_SRC(x) ((void *)((u32)x | 0x80000000))
#define SPR_SEND SPR_SRC(SPR_MEM)
#define UNCACHED(p) ((void *)((u32)p | 0x20000000))
#define VIF_UNPACK_V3_16 9
#define VIF_UNPACK_V4_8 14
#define DMArefe (0<<28)
#define DMAcnt  (1<<28)
#define DMAnext (2<<28)
#define DMAref  (3<<28)
#define DMArefs (4<<28)
#define DMAcall (5<<28)
#define DMAret  (6<<28)
#define DMAend  (7<<28)

#define SCE_GS_FALSE (0)
#define SCE_GS_TRUE  (1)
#define SCE_GS_ALPHA_NEVER		(0)
#define SCE_GS_ALPHA_ALWAYS		(1)
#define SCE_GS_ALPHA_LESS		(2)
#define SCE_GS_ALPHA_LEQUAL		(3)
#define SCE_GS_ALPHA_EQUAL		(4)
#define SCE_GS_ALPHA_GEQUAL		(5)
#define SCE_GS_ALPHA_GREATER	(6)
#define SCE_GS_ALPHA_NOTEQUAL	(7)
#define	SCE_GS_AFAIL_KEEP		(0)
#define	SCE_GS_AFAIL_FB_ONLY	(1)
#define SCE_GS_AFAIL_ZB_ONLY	(2)
#define SCE_GS_AFAIL_RGB_ONLY	(3)
#define SCE_GS_BLEND_RGB_SRC (0)
#define SCE_GS_BLEND_RGB_DST (1)
#define SCE_GS_BLEND_RGB_0   (2)
#define SCE_GS_BLEND_ALPHA_SRC (0)
#define SCE_GS_BLEND_ALPHA_DST (1)
#define SCE_GS_BLEND_ALPHA_FIX (2)
#define SCE_GS_REPEAT (0)
#define SCE_GS_CLAMP  (1)
#define SCE_GS_REGION_CLAMP  (2)
#define SCE_GS_REGION_REPEAT (3)

#endif
