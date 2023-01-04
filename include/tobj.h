#ifndef TOBJ_H
#define TOBJ_H

#include "hoshino.h"
#include "p.h"

typedef struct tCOORD {
    FVECTOR Rot;
    FVECTOR Trans;
    FMATRIX LcMtx;
    FMATRIX LwMtx;
    FMATRIX LsMtx;
    FMATRIX LvMtx;
    FMATRIX LcLightMtx;
    struct tCOORD* Super;
    int Flag;
} tCOORD;

typedef struct {
    tCOORD Base;
    float pad[2];
    FMATRIX* pNormalLight;
    FMATRIX* pLightColor;
    float Scale;
    hKEI Size;
    int Fuku;
    int OutLine;
    PSFXOBJ* pSfx;
    PMOTION* pMot;
    int ret;
} tOBJECT;

#endif