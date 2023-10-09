#ifndef HR_PREAM_H
#define HR_PREAM_H

#include "harada.h"

extern s32 (*HrPtMotTbl[10])(HR_CALL *ca, HR_PSYS *ps);
extern s32 (*HrPtSndTbl[17])(HR_CALL *ca, HR_PSYS *ps);

extern void hr_set_motion(HR_CALL *ca, s32 noth);
extern f32* pt_se_panpos(HR_CALL *ca, u16 on);
extern s32 pt_se_changer(s32 id);
extern s32 pt_motlp_secall(HR_CALL *ca);

#endif
