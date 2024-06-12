#ifndef OKZMINIT_H
#define OKZMINIT_H

#include "okanoyo.h"

extern void OkTObjScaleSet(tOBJECT2 *hOp, f32 scale);
extern void OkTObjScaleSet2(tOBJECT2 *hOp, sceVu0FVECTOR scale);
extern void OkTObjScaleUp(tOBJECT2 *hOp, f32 target, f32 div);
extern s32 OkTObjModelSetGeneral(OBJWORK *objw, tOBJECT2 **hOp, s32 p_id, void *Draw, void *Draw2);

#endif
