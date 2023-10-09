#ifndef HR_PRM_H
#define HR_PRM_H

#include "harada.h"
#include "harada/hr_mapv.h"

extern s32* pt_al_data(HR_CALL *ca, s16 ptr);
extern s32* pt_al_dataB(s32 *base, s16 ptr);
extern void hr_pcall_rtinit(HR_CALL *ca);
extern HR_CALL* hr_pcall_search(HR_PSYS *ps, HR_CALL *call);
extern HR_CALL* hr_id_search(HR_PSYS *ps, s16 id);
extern HR_CALL* hr_pfg_search(HR_PSYS *ps, s32 flag);
extern s32 hr_efc_why(s32 id);
extern void hr_call_efcdel(HR_CALL *ca, HR_PSYS *ps);
extern void hr_call_efcdelE(HR_PSYS *ps, s32 id);
extern void hr_call_efcon(HR_CALL *ca, HR_PSYS *ps, s32 id, s32 cmd);
extern void hr_callbuff_init(HR_PSYS *ps, s32 fg);
extern void hr_rt_posrot(HR_CALL *ca);
extern void pt_set_rtpos(HR_CALL *ca, s32 si0, u16 us0);
extern void pt_get_routeK(HR_CALL *ca);
extern void hr_psys_init(HR_PSYS *ps, s32 *addr, PCAMS *cam, s32 *addrm, HRPMOJI *pmes, HRPMWAKU *waku);
extern void hr_seloop_on(HR_PTSE *se, u32 splt, f32 *pos);
extern void hr_seloop_off(HR_PTSE *se, u32 splt);
extern HR_PTSE* hr_seloop_search(HR_PSYS *ps, u32 splt);
extern void pt_set_mesp(HR_CALL *ca, HR_PSYS *ps, u16 *addr, s16 time);
extern void pt_del_mesp(HR_CALL *ca, HR_PSYS *ps, u16 *addr, s16 time);
extern s32 hr_pt_checkExit(HR_PSYS *ps);
extern s32 hr_psys_exit(HR_PSYS *ps);
extern void hr_pt_getpos(HR_CALL *ca, sceVu0FVECTOR *v);
extern void hr_pt_getrot(HR_CALL *ca, sceVu0FVECTOR *v);
extern void hr_pt_getrotD(HR_CALL *ca, sceVu0FVECTOR *v);
extern s32 hr_pcall_read(HR_CALL *ca, HR_PSYS *ps);
extern void hr_check_time_move(PCAMSC *pcc);
extern s32 hr_check_time_moveRT(PCAMSR *rt);
extern s32 hr_check_time_moveL(PCAMSL *len);
extern void hr_efc_ofs(HR_CALL *ca, s32 id);
extern void hr_call_efcworkDeb(HR_CALL *ca, HR_PSYS *ps);
extern void hr_pcall_mkm(sceVu0FVECTOR pos, sceVu0FVECTOR rot, sceVu0FMATRIX m);
extern void hr_pt_light(HR_CALL *ca);
extern void hr_pt_motion(HR_CALL *ca);
extern void hr_pt_motionS(HOT *hObj);
extern void hr_pt_motionL(HOT *hObj, sceVu0FMATRIX m, sceVu0FVECTOR nlight, sceVu0FVECTOR lcolor);
extern void hr_psys_motdraw(HR_PSYS *ps, s32 pause);
extern void hr_ptmir_drawMain(HR_PSYS *ps);
extern void hr_pkey_init(HR_PKEY *key);
extern void hr_pkey_trg(HR_PKEY *key, s32 trg);
extern void hr_pkey_lvl(HR_PKEY *key, s32 lvl);
extern void hr_loopse_work(HR_PSYS *ps);
extern void hr_psys_work(HR_PSYS *ps);
extern void hr_ptlips_stop(HR_PSYS *ps);
extern void pt_set_skip(HR_PSYS *ps);
extern void pt_set_skip2(HR_PSYS *ps);
extern void pt_set_exit(HR_PSYS *ps, s16 count);
extern s32 pt_flagj_P(u32 id);
extern s32 hr_strt_check(PT96A *p, u16 no, u32 mc, s32 flag);
extern void hr_ptvoice_call(HR_PSYS *ps, HRPMOJI *pmes);
extern void hr_ptvoice_stop(HR_PSYS *ps, HRPMOJI *pmes);
extern void hr_psys_debugMVP(HPVSEL *pcsel, s32 pause);
extern void hr_psys_debug(HR_PSYS *ps, s32 menu, HPVSEL *pcsel, PCAMS *pcam);
extern void hr_psys_debugP(HR_PSYS *ps, s32 pause);


#endif
