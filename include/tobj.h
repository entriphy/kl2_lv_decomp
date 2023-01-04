#ifndef TOBJ_H
#define TOBJ_H

#include <libvux.h>
#include "hoshino.h"
#include "p.h"
#include <tamtypes.h>

typedef struct tCOORD {
    VU_VECTOR Rot;
    VU_VECTOR Trans;
    VU_MATRIX LcMtx;
    VU_MATRIX LwMtx;
    VU_MATRIX LsMtx;
    VU_MATRIX LvMtx;
    VU_MATRIX LcLightMtx;
    struct tCOORD* Super;
    int Flag;
} tCOORD;

typedef struct {
    tCOORD Base;
    float pad[2];
    VU_MATRIX* pNormalLight;
    VU_MATRIX* pLightColor;
    float Scale;
    hKEI Size;
    int Fuku;
    int OutLine;
    PSFXOBJ* pSfx;
    PMOTION* pMot;
    int ret;
} tOBJECT;

#endif