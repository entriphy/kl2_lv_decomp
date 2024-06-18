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

#endif
