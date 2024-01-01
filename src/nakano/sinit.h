#ifndef NK_SINIT_H
#define NK_SINIT_H

#include "nakano.h"

extern void InitAllObjWork2(OBJWORK *objwork, s32 n_objw);
extern void nkStageInit0();
extern void nkAreaInit();
extern qword* nkDecodeData(qword *src, qword *dst);
extern void nkStageInit1();
extern void nkChangeArea(u32 vision, s32 rtn, s32 mcn, f32 height);
extern void nkChangeArea2(u32 vision, s32 rtn, s32 mcn, s32 muki, f32 height);

#endif
