#ifndef HR_PREAD_H
#define HR_PREAD_H

#include "harada.h"

extern void pt_flag_work(HR_PSYS *ps, PT96A *sfg);
extern void comm_p128b_ft(PCAMSC *pcc, s32 ft);
extern void comm_jigen_spd(sceVu0FVECTOR spd, sceVu0FVECTOR acc, f32 ft, u8 uc0, u8 uc1);
extern void pt_set_mkmat(HR_CALL *ca, HR_PSYS *ps);
extern void hr_pcamsl_spdclr(PCAMSL *pcf);
extern void comm_getft_mvF(HR_CALL *ca, PCAMSL *pcf);
extern void comm_getft_mvpF(HR_CALL *ca, PCAMSL *pcf);

extern s32 (**hrPtFuncTbl[5])(HR_CALL *ca, HR_PSYS *ps);

#endif
