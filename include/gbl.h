#ifndef GBL_H
#define GBL_H

#include "types.h"
#include "objwork.h"

typedef struct {
    u32 cnt;
    u32 status;
    u32 lvl;
    u8 r3h;
    u8 r3v;
    u8 l3h;
    u8 l3v;
    u32 lvl2;
    u16 trg;
    u16 trgbuf;
    u16 rep;
    u16 reptimer;
    u32 looks;
} kPadDATA;

typedef struct {
    sceVu0FVECTOR posi;
    sceVu0FVECTOR ang;
    f32 scr_z;
    s32 mode;
} CAM_WORK;

typedef struct {
    s32 outtime;
    s32 intime;
    s32 next_vision;
    s32 rtn;
    s32 mcn;
    s32 imuki;
    s32 omuki;
    u16 flag;
    s16 fdata;
} nkWIPE_WRK;

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

typedef struct { // 0x50
    /* 0x00 */ u32 irqc;
    /* 0x04 */ u32 drawhc;
    /* 0x08 */ u32 proc_hcnt;
    /* 0x0c */ s32 cp;
    /* 0x10 */ u32 *cotf;
    /* 0x14 */ u32 *cot;
    /* 0x18 */ u32 *cotb;
    /* 0x1c */ u32 *cpkt;
    /* 0x20 */ u32 *sysbuf;
    /* 0x24 */ u32 *sysbufbase;
    /* 0x28 */ s32 nmode;
    /* 0x2c */ s32 nsmode;
    /* 0x30 */ s32 fmode;
    /* 0x34 */ s32 smode;
    /* 0x38 */ u32 modorg;
    /* 0x3c */ u32 modorgend;
    /* 0x40 */ OBJWORK *objwork;
    /* 0x44 */ s32 n_objw;
    /* 0x48 */ s32 Language;
    /* 0x4C */ s32 TVSystem;
} SYSGBL;

extern SYSGBL SysGbl;
extern GAME_WORK GameGbl;


#endif
