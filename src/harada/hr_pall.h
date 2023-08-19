#ifndef HR_PALL_H
#define HR_PALL_H

#include "harada.h"

extern HRPMOJI ppmes;
extern HRPMWAKU ppwaku;
extern s32 ptflag_buff[4];
extern s32 ptflag_st[4];
extern s32 ptflag_ar[4];
extern s32 ptflag_th[4];
extern sceVu0FMATRIX hrpum;
extern sceVu0FMATRIX hrpm;
extern sceVu0FVECTOR hrvi;
extern sceVu0FVECTOR hrvo;
extern s32 *hrpt_addr;
extern s32 *hrptm_addr;
extern s16 hrpt_flag;
extern s16 hrpt_scene;
extern s16 hrpt_view;
extern s16 hrpt_th;
extern s16 hrpt_vt;
extern s16 hrpt_deb;
// extern PCAMSC *hrpcc;
extern u32 *hrpt_pack;
extern u32 *hrpt_gms;
extern s16 hrpt_patch;
extern s16 hrpt_patcnt;
extern s16 hrpt_stnk;
extern HR_PSYS ppsys;
extern HR_CALL *hrpt_call;
extern s32 hrpt_mini;
extern s32 *hrpt_mini_addr;

extern void hr_pt_set(s16 flag, s16 scene, s16 view, s16 th);
extern void hr_pt_fclear();

#endif
