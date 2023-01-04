#ifndef GIM_H
#define GIM_H

#include "route.h"
#include "vpo.h"

// Size: 0x80
typedef struct {
    VPOINFO* vpo;
    FMATRIX hpo_mat;
    FVECTOR scale;
    s32 tblidx;
    s32 time;
    u8* hpo;
    ROUTE* rtp;
    s32 type;
    float ph_dist;
    s32 pad0;
    s32 pad1;
} GIMWORK;

GIMWORK* GetGimWork();

#endif