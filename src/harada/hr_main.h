#ifndef HR_MAIN_H
#define HR_MAIN_H

#include "harada.h"

typedef struct { // 0x10
    /* 0x0 */ f32 s;
    /* 0x4 */ f32 spds;
    /* 0x8 */ f32 t;
    /* 0xc */ f32 spdt;
} HRSCRST;

extern MAPVWORK mapvw;
extern HRSCRST hrmapst;
extern HRSCRST hrbgst;
extern u32 *hrd_pack;
extern u32 *hrg_pack;
extern u32 *hrf_pack;
extern u32 *hrse_pack;
extern s32 hr_abeoff;
extern s32 hr_objtype;
extern u32 *hr_mt_addr;
extern vpmINFO hr_mtexi;
extern s32 hr_resKeep;
extern s32 hr_resTmp;
extern u32 *hr_obcdata;
extern u32 *hr_obcvpf;
extern VPOINFO *hr_obcvpo;

extern void hr_cold_start();
extern void hr_hpmk_init(u32 *top);
extern u32* func_001028E0(u32 *top);
extern u32* hr_hpmk_blk(u16 gx, u16 gy, u16 gz, u32 *top);
extern u32* hr_hpmk(f32 x, f32 y, f32 z, u32 *top);
extern u32* hr_hpmk_deb(f32 x, f32 y, f32 z, u32 *top, HR_HPDEB *hpd);
extern void hr_stage_no(char *name, s32 fg);
extern s32 hr_check_dmy(qword *addr);
extern s32 func_00103398(u32 *pack, s32 param_2, s32 param_3);
extern void func_00103508();
extern void hrPtSeLoad();
extern void hrStageDataLoad(s32 fg);
extern void hr_set_defMVC();
extern void func_00103E00(u32 *param_1);
extern void MapConfFileRead();
extern void hrDataLoad();
extern void func_00104508();
extern void hrAreaEnd();
extern void hrStageEnd();
extern void hr_set_vpmfog(vpmINFO *info);
extern s32* hr_get_draw2();
extern void hr_scrst_init(HRSCRST *st);
extern void hr_scrst_set(HRSCRST *st, s16 s, s16 t);
extern void hr_scrst_work(HRSCRST *st);
extern void hrInitWork();
extern void hrInitWorkDeb();
extern void hrMainWork();
extern void hrMainDraw();
extern void hr_retry_set();
extern void hr_restart_clear();
extern void hr_restart_Tmpclear();
extern void hr_restart_set(s32 id);
extern s32 hr_restart_check(s32 id);

#endif
