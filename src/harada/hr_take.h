#ifndef HR_TAKE_H
#define HR_TAKE_H

#include "harada.h"

extern void hr_init_tobject(tOBJECT *tobj);
extern void hr_take_init();
extern void hr_take_initF();
extern void hr_take_setM();
extern HOT* hr_take_workGet();
extern void hr_take_workDel(HOT *ho);
extern void hr_take_modelinit(HOT *hot, u32 *addr);
extern void hr_take_modelinit2(HOT *hot, s32 id);
extern void hr_take_motionPause(HOT *hot, s32 pause);
extern s32 hr_take_modeldraw(HOT *hot, s32 mode);
extern s32 hr_take_modeldraw2(HOT *hot);
extern void hr_take_modeldrawMir(HOT *hot, s32 id);
extern s16 hr_take_size(HOT *hot);
extern void hr_take_modelsizeCHG(HOT *hot, s32 size);
extern void hr_take_modelscale(HOT *hot, f32 scale);
extern void hr_take_modelclip(HOT *hot, s32 flag);
extern void hr_take_motionSet(HOT *hot, s32 motno, s32 loop);
extern void hr_take_motionSetN(HOT *hot, s32 motno, s32 loop);
extern void hr_take_GetLCMatYXZ(COLD *cd);
extern s32 hr_take_motion(HOT *hot, s32 xyz);
extern f32 hr_take_motionGetFMAX(HOT *hot);
extern f32 hr_take_motionGetFrm(HOT *hot);
extern void hr_take_motionSetFrm(HOT *hot, f32 frm);
extern void hr_take_matrixtrans(HOT *hot, sceVu0FVECTOR v, s32 num);
extern void hr_take_lips(HOT *hot, s16 *addr);
extern void hr_take_lipsOFF(HOT *hot);
extern void hr_take_rinkaku(HOT *hot);
extern void hr_take_shadow(HOT *hot, sceVu0FVECTOR pos);

#endif
