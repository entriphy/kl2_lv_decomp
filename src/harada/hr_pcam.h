#ifndef HR_PCAM_H
#define HR_PCAM_H

#include "harada.h"

extern void hr_pcam_pr2inte(sceVu0FVECTOR pos, sceVu0FVECTOR ang, sceVu0FVECTOR inte, f32 len);
extern void hr_pcam_pi2rot(sceVu0FVECTOR pos, sceVu0FVECTOR inte, sceVu0FVECTOR ang);
extern void hr_pcam_cinit(PCAMSC *pcc);
extern void hr_pcam_ang_init();
extern void hr_pcam_set_pos(f32 x, f32 y, f32 z);
extern void hr_pcam_set_inte(f32 x, f32 y, f32 z);
extern void hr_pcam_init(sceVu0FVECTOR pos, sceVu0FVECTOR ang, f32 proj);
extern void hr_pcam_mhold(PCAMSC *pcc, PCAMSM *pcm, sceVu0FMATRIX *m, s32 fg);
extern void hr_pcam_vhold(PCAMSC *pcc, PCAMSM *pcm, sceVu0FVECTOR *v, s32 fg);
extern void hr_setEnvelopeMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FMATRIX m2[4], sceVu0FMATRIX m3);
extern void hr_Envelope1vert(sceVu0FVECTOR *v, sceVu0FVECTOR *w, sceVu0FVECTOR *vo);
extern void hr_pcam_spdacc(PCAMSC *pcc);
extern void hr_pcam_spdclr(PCAMSC *pcc);
extern void hr_pcam_set_mode(s32 mode);
extern void hr_pcam_set_beta(s32 *addr);
extern void hr_pcam_ang2wa();
extern void hr_pcam_pi2len();
extern void hr_pcam_anglen();
extern void hr_pcam_set_ang(f32 x, f32 y, f32 z, s32 fg);
extern void hr_pcam_gyakusan(sceVu0FVECTOR *pi, PCAMSC *pcc, PCAMSM *pcm);
extern void hr_pcam_calc();
extern s32 hr_pcam_tmv_end(s32 no);
extern void hr_pcam_pos_tmv(s32 id, f32 x, f32 y, f32 z, s32 time, u8 c0, u8 c1);
extern void hr_pcam_inte_tmv(s32 id, f32 x, f32 y, f32 z, s32 time, u8 c0, u8 c1);
extern void hr_pcam_ang_tmv(s32 id, f32 x, f32 y, f32 z, s32 time, u8 c0, u8 c1);
extern void hr_pcam_work_mae();
extern void hr_pcam_work();

#endif
