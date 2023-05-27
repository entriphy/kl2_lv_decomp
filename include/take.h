#ifndef TAKE_H
#define TAKE_H

#include "common.h"

// Structs

typedef struct {
    f32 SrcZ;
    f32 AspectX;
    f32 AspectY;
    f32 CenterX;
    f32 CenterY;
    f32 MinZ;
    f32 MaxZ;
    f32 NearZ;
    f32 FarZ;
    sceVu0FVECTOR camera_p;
    sceVu0FVECTOR camera_zd;
    sceVu0FVECTOR camera_yd;
    sceVu0FMATRIX WvMtx;
    sceVu0FMATRIX VsMtx;
    sceVu0FMATRIX WsMtx;
} SCRENV;

// Data

// object.c
extern SCRENV Scr;

#endif
