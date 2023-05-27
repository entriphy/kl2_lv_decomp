#ifndef GIM_H
#define GIM_H

#include "ps2.h"
#include "route.h"
#include "vpo.h"

// Size: 0x80
typedef struct {
    VPOINFO *vpo;
    sceVu0FMATRIX hpo_mat;
    sceVu0FVECTOR scale;
    s32 tblidx;
    s32 time;
    u8 *hpo;
    ROUTE *rtp;
    s32 type;
    f32 ph_dist;
    s32 pad0;
    s32 pad1;
} GIMWORK;

GIMWORK * GetGimWork();

#endif
