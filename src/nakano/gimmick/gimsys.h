#ifndef GIMSYS_H
#define GIMSYS_H

#include "common.h"
#include "harada.h"

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

extern GIMWORK* GetGimWork();
extern s32 gmGetGimType(OBJWORK* objw);

#endif
