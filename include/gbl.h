#ifndef GBL_H
#define GBL_H

#include <libgraph.h>
#include "cam.h"
#include "kazuya.h"
#include "nakano.h"
#include "objwork.h"
#include "structs.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 224

typedef struct {
    u32 irqc;
    u32 drawhc;
    u32 proc_hcnt;
    s32 cp;
    s32 nmode;
    s32 nsmode;
    s32 fmode;
    s32 smode;
    u32 modorg;
    u32 modorgend;
    OBJWORK *objwork;
    s32 n_objw;
    s32 Language;
    s32 TVSystem;
} SYSGBL;

typedef struct {
    s32 mode;
    s32 vision;
    s32 flag;
    u32 fr;
    sceGsDBuffDc db;
    s32 inter;
    s32 pause_flag;
    s32 non_pause_flag;
    s32 pause_flag2;
    s32 route_max;
    s32 init_vision;
    s32 st_rtn;
    s32 st_mcn;
    s32 st_muki;
    f32 st_height;
    s32 st_flag;
    s32 st_klomode;
    s32 Re_st_rtn;
    s32 Re_st_mcn;
    s32 Re_st_vision;
    s32 Re_st_klomode;
    s32 Re_st_muki;
    f32 Re_st_height;
    s32 Re_st_zanki_flag;
    s32 Re_st_zanki_count;
    OBJWORK *klonoa;
    OBJWORK *kazd;
    s32 playdemo_time;
    s32 playdemo_flag;
    kPadDATA kpd[2];
    nkWIPE_WRK wipe;
    s32 kloLife[2];
    s32 kloZanki[2];
    f32 reverse_cnt;
    s32 stage_flag;
    s32 filter_type;
    f32 filter_cnt;
    s32 filter_prty;
    s32 area_max;
    f32 gus_timer;
    f32 gus_cnt;
    s32 popuka_flag;
    s32 time_atack_cnt;
    s32 vs_clear_cnt;
    s32 time_atack_best;
    f32 fcnt0;
    f32 fcnt1;
    f32 fcnt2;
    s32 area_cnt;
    f32 retry_posy;
    s32 retry_bun1;
    s32 retry_muki;
    s32 tokurom_cnt;
    sceVu0FMATRIX wsm;
    sceVu0FMATRIX wvm;
    sceVu0FMATRIX vsm;
    sceVu0FMATRIX d_wsm;
    sceVu0FMATRIX d_wvm;
    sceVu0FMATRIX d_vsm;
    CAM_WORK cam;
    CAM_WORK d_cam;
} GAME_WORK;

#endif
