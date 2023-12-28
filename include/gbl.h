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
    /* 0x000 */ s32 mode;
    /* 0x004 */ s32 vision;
    /* 0x008 */ s32 flag;
    /* 0x00c */ u32 fr;
    /* 0x010 */ sceGsDBuffDc db;
    /* 0x340 */ s32 inter;
    /* 0x344 */ s32 pause_flag;
    /* 0x348 */ s32 non_pause_flag;
    /* 0x34c */ s32 pause_flag2;
    /* 0x350 */ s32 route_max;
    /* 0x354 */ s32 init_vision;
    /* 0x358 */ s32 st_rtn;
    /* 0x35c */ s32 st_mcn;
    /* 0x360 */ s32 st_muki;
    /* 0x364 */ f32 st_height;
    /* 0x368 */ s32 st_flag;
    /* 0x36c */ s32 st_klomode;
    /* 0x370 */ s32 Re_st_rtn;
    /* 0x374 */ s32 Re_st_mcn;
    /* 0x368 */ s32 Re_st_vision;
    /* 0x37c */ s32 Re_st_klomode;
    /* 0x380 */ s32 Re_st_muki;
    /* 0x384 */ f32 Re_st_height;
    /* 0x388 */ s32 Re_st_zanki_count;
    /* 0x38c */ OBJWORK *klonoa;
    /* 0x390 */ OBJWORK *kazd;
    /* 0x394 */ s32 *dataBuff;
    /* 0x398 */ s32 playdemo_time;
    /* 0x39c */ s32 playdemo_flag;
    /* 0x3a0 */ kPadDATA kpd[2];
    /* 0x3e0 */ nkWIPE_WRK wipe;
    /* 0x400 */ s32 kloLife[2];
    /* 0x408 */ s32 kloZanki[2];
    /* 0x410 */ f32 reverse_cnt;
    /* 0x414 */ s32 stage_flag;
    /* 0x418 */ s32 filter_type;
    /* 0x41c */ f32 filter_cnt;
    /* 0x420 */ s32 filter_prty;
    /* 0x424 */ s32 area_max;
    /* 0x428 */ f32 gus_timer;
    /* 0x42c */ f32 gus_cnt;
    /* 0x430 */ s32 popuka_flag;
    /* 0x434 */ s32 time_atack_cnt;
    /* 0x438 */ s32 k_atck_button;
    /* 0x43c */ s32 k_jump_button;
    /* 0x440 */ s32 vs_clear_cnt;
    /* 0x444 */ s32 time_atack_best;
    /* 0x448 */ f32 fcnt0;
    /* 0x44c */ f32 fcnt1;
    /* 0x450 */ f32 fcnt2;
    /* 0x454 */ s32 area_cnt;
    /* 0x458 */ f32 retry_posy;
    /* 0x45c */ s32 retry_bun1;
    /* 0x460 */ s32 retry_muki;
    /* 0x464 */ s32 tokurom_cnt;
    /* 0x470 */ sceVu0FMATRIX wsm;
    /* 0x4b0 */ sceVu0FMATRIX wvm;
    /* 0x4f0 */ sceVu0FMATRIX vsm;
    /* 0x530 */ sceVu0FMATRIX d_wsm;
    /* 0x570 */ sceVu0FMATRIX d_wvm;
    /* 0x5b0 */ sceVu0FMATRIX d_vsm;
    /* 0x5f0 */ CAM_WORK cam;
    /* 0x620 */ CAM_WORK d_cam;
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
