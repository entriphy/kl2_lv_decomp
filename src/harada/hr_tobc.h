#ifndef HR_TOBC_H
#define HR_TOBC_H

#include "harada.h"
#include "harada/hr_obcon.h"

extern void hr_to_modeldraw(HOT *hobj);
extern void hr_to_modeldrawRL(HOT *hobj, s32 no);
extern void hr_to_modeldrawF(HOT *hobj);
extern void hr_to_motionset(HOT *hobj, s32 motno, s32 loop);
extern void hr_to_motionpause(HOT *hobj, s32 pause);
extern HOT* hr_to_workget();
extern s32 hr_to_motion(HOT *hobj, s32 xyz);
extern void hr_to_workdel(HOT *hobj);
extern void hr_to_modelinit2(HOT *hobj, s32 id);
extern void hr_to_setpos(HOT *hobj, f32 px, f32 py, f32 pz);
extern void hr_to_setrot(HOT *hobj, f32 rx, f32 ry, f32 rz);
extern void hr_to_setscale(HOT *hobj, f32 scale);
extern void hr_to_modelclip(HOT *hobj);
extern f32 hr_to_motionGFMAX(HOT *hobj);
extern void hr_to_motionSFRM(HOT *hobj, f32 frm);
extern void hr_set_hobject2(HOT *hObj, HR_HOBJ *hh);
extern void hr_set_hobject3(HR_OBJW *ow);

#endif
