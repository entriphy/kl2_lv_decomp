#ifndef GIM_H
#define GIM_H

#include <libvux.h>
#include "route.h"
#include "vpo.h"

// Size: 0x80
typedef struct {
    VPOINFO* vpo;
    VU_MATRIX hpo_mat;
    VU_VECTOR scale;
    s32 tblidx;
    s32 time;
    u8* hpo;
    ROUTE* rtp;
    s32 type;
    float ph_dist;
    s32 pad0;
    s32 pad1;
} GIMWORK;

#endif