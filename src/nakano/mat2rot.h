#ifndef MAT2ROT_H
#define MAT2ROT_H

#include "nakano.h"

extern void InvRotMatrixZYX(sceVu0FVECTOR vec, sceVu0FMATRIX rotm);
extern void InvRotMatrixZXY(sceVu0FVECTOR vec, sceVu0FMATRIX rotm);
extern void InvRotMatrixYXZ(sceVu0FVECTOR vec, sceVu0FMATRIX rotm);

#endif