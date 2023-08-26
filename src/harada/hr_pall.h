#ifndef HR_PALL_H
#define HR_PALL_H

#include "harada.h"

extern PCAMS ppcam;
extern PCAMS *hrpcam;
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
extern PCAMSC *hrpcc;
extern u32 *hrpt_pack;
extern u32 *hrpt_gms;
extern s16 hrpt_patch;
extern s16 hrpt_patcnt;
extern s16 hrpt_stnk;
extern HR_PSYS ppsys;
extern HR_CALL *hrpt_call;
extern s32 hrpt_mini;
extern s32 *hrpt_mini_addr;

extern f32 hr_pall_atan2(f32 y, f32 x);
extern f32 hr_pall_rotpai(f32 r);
extern void hr_pall_rotpaiV(sceVu0FVECTOR v);
extern f32 hr_pall_rot180(f32 r);
extern void hr_pall_rot180V(sceVu0FVECTOR v);
extern void hr_pall_limitV(sceVu0FVECTOR v, f32 max, f32 min);
extern void hr_pt_set(s16 flag, s16 scene, s16 view, s16 th);
extern void hr_pt_fclear();
extern s32 hr_pt_check();
extern s32 hr_ptmir_check();
extern void hr_ptmir_draw();
extern void hrPtDataLoad();
extern void hrGameInPtDataLoad(s32 sw);
extern void hrPtMiniDataLoad(char *stname);
extern void hrPtMini_init();
extern void hr_ptcall_getbuff();
extern void hr_pall_init();
extern s32 hr_check_skw();
extern s32 hr_pall_work();
extern void hr_pall_ncnt();
extern s32 hr_pall_ncntSN();
extern s32 hr_pall_exitCam();
extern void hr_hse_areaend();
extern void hr_pall_draw();
extern void hr_pall_aftdraw();

#endif
