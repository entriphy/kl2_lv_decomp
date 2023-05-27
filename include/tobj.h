#ifndef TOBJ_H
#define TOBJ_H

#include "hoshino.h"
#include "psfx.h"

typedef struct tCOORD {
    sceVu0FVECTOR Rot;
    sceVu0FVECTOR Trans;
    sceVu0FMATRIX LcMtx;
    sceVu0FMATRIX LwMtx;
    sceVu0FMATRIX LsMtx;
    sceVu0FMATRIX LvMtx;
    sceVu0FMATRIX LcLightMtx;
    struct tCOORD *Super;
    s32 Flag;
} tCOORD;

typedef struct {
    tCOORD Base;
    sceVu0FMATRIX *pNormalLight;
    sceVu0FMATRIX *pLightColor;
    f32 Scale;
    hKEI Size;
    s32 Fuku;
    s32 OutLine;
    PSFXOBJ *pSfx;
    PMOTION *pMot;
    s32 ret;
} tOBJECT;

#endif
