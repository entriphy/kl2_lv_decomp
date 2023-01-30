#ifndef GBL_H
#define GBL_H

#ifdef SCE
#include <libgraph.h>
#else
#include <libgs.h>
#endif

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
    OBJWORK* objwork;
    s32 n_objw;
    s32 Language;
    s32 TVSystem;
} SYSGBL;

#ifdef SCE
#define GsDBuffDc sceGsDBuffDc
#else
typedef struct {
    GS_PMODE pmode;
    GS_SMODE2 smode2;
    GS_DISPFB dispfb;
    GS_DISPLAY display;
    GS_BGCOLOR bgcolor;
} GsDispEnv;

typedef struct {
    GS_FRAME frame;
    u64 frame_addr;
    GS_ZBUF zbuf;
    s64 zbuf_addr;
    GS_XYOFFSET xyoffset;
    s64 xyoffset_addr;
    GS_SCISSOR scissor;
    s64 scissor_addr;
    GS_PRMODECONT prmodecont;
    s64 prmodecont_addr;
    GS_COLCLAMP colclamp;
    s64 colclamp_addr;
    GS_DTHE dthe;
    s64 dthe_addr;
    GS_TEST test;
    s64 test_addr;
} GsDrawEnv;

typedef struct {
    GS_DISPENV disp[2];
    GS_GIF_TAG giftag0;
    GsDrawEnv draw0[2];
    void* clear0;
    GS_GIF_TAG giftag1;
    GsDrawEnv draw1[2];
    void* clear1;
} GsDBuffDc;
#endif

typedef struct {
    s32 mode;
    s32 vision;
    s32 flag;
    u32 fr;
    GsDBuffDc db;
    s32 inter;
    s32 pause_flag;
    s32 non_pause_flag;
    s32 pause_flag2;
    s32 route_max;
    s32 init_vision;
    s32 st_rtn;
    s32 st_mcn;
    s32 st_muki;
    float st_height;
    s32 st_flag;
    s32 st_klomode;
    s32 Re_st_rtn;
    s32 Re_st_mcn;
    s32 Re_st_vision;
    s32 Re_st_klomode;
    s32 Re_st_muki;
    float Re_st_height;
    s32 Re_st_zanki_flag;
    s32 Re_st_zanki_count;
    OBJWORK* klonoa;
    OBJWORK* kazd;
    s32 playdemo_time;
    s32 playdemo_flag;
    kPadDATA kpd[2];
    nkWIPE_WRK wipe;
    s32 kloLife[2];
    s32 kloZanki[2];
    float reverse_cnt;
    s32 stage_flag;
    s32 filter_type;
    float filter_cnt;
    s32 filter_prty;
    s32 area_max;
    float gus_timer;
    float gus_cnt;
    s32 popuka_flag;
    s32 time_atack_cnt;
    s32 vs_clear_cnt;
    s32 time_atack_best;
    float fcnt0;
    float fcnt1;
    float fcnt2;
    s32 area_cnt;
    float retry_posy;
    s32 retry_bun1;
    s32 retry_muki;
    s32 tokurom_cnt;
    FMATRIX wsm;
    FMATRIX wvm;
    FMATRIX vsm;
    FMATRIX d_wsm;
    FMATRIX d_wvm;
    FMATRIX d_vsm;
    CAM_WORK cam;
    CAM_WORK d_cam;
} GAME_WORK;

#endif
