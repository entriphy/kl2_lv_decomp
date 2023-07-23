#ifndef MOTIP_H
#define MOTIP_H

#include "take.h"

extern void TkGetLpMAtrix(sceVu0FMATRIX Mtx, sceVu0FVECTOR Pos, sceVu0FVECTOR Norm, sceVu0FVECTOR Rot);
extern void GetJetMatrix(sceVu0FMATRIX Mtx, sceVu0FVECTOR VecNow);
extern void LinerInterPolateMatrixBuff(sceVu0FMATRIX dm, sceVu0FMATRIX m0, sceVu0FMATRIX m1, s32 num, u64 flag, f32 weight);
extern void LinerInterPolateMatrix(sceVu0FMATRIX dm, sceVu0FMATRIX m0, sceVu0FMATRIX m1, f32 Weight);
extern void InterPolateMatrixLightTmp2(sceVu0FMATRIX dm, sceVu0FMATRIX m0, sceVu0FMATRIX m1, f32 Weight);
//extern void AxisInterPolate(sceVu0FMATRIX DistMtx, sceVu0FMATRIX StartMtx, sceVu0FMATRIX EndMtx, sceVu0FVECTOR Axis, f32 Weight);
extern void GetInterPolateAxis(sceVu0FVECTOR Axis, sceVu0FMATRIX StartMtx, sceVu0FMATRIX EndMtx);
extern f32  __sin(f32 rad);
extern f32  __cos(f32 rad);

#endif
