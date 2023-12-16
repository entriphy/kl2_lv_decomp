#ifndef CVAR_H
#define CVAR_H

#include "kazuya.h"

extern KZ_MC_INFO kzMcInfo;
extern KZ_MC_INFO *mci;
extern kPadDATA *kpd0;
extern kPadDATA *kpd1;
extern s32 wm_jobcnt;
extern s32 wm_jobcnt_current;
extern s32 wm_jobcnt_next0;
extern s32 wm_jobcnt_next1;
extern s32 wm_jobcnt_sub;
extern s32 wm_jobcnt_ret;
extern s32 td_jobcnt;
extern s32 td_jobcnt_current;
extern s32 td_jobcnt_next0;
extern s32 td_jobcnt_next1;
extern s32 td_jobcnt_sub;
extern s32 td_jobcnt_ret;
extern s32 fade_count;
extern s32 fade_mode;
extern s32 vision_count;
extern s32 saveornot_flag;
extern s32 aftersave_goto;
extern s32 status_return;
extern s32 pause_return;
extern s32 pause_interval;
extern s32 cloud_flag;
extern s32 retry_vision;
extern s32 point_vision;
extern s32 route_anime_no;
extern s32 clear_event_no;
extern s32 wmap_start_act_flag;
extern s32 mmd_complete_stage;
extern s32 bgm_list_no;
extern s32 bgm_index;
extern u8 *D_3FAD70;
extern s32 kz_colsys_mode;
extern DVECTOR KZOFFX;
extern DVECTOR KZOFFY;

#endif
