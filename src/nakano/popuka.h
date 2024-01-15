#ifndef POPUKA_H
#define POPUKA_H

#include "nakano.h"

typedef struct { // 0x60
    /* 0x00 */ u32 lvl;
    /* 0x04 */ u32 trg;
    /* 0x08 */ sceVu0FVECTOR *pos_buff;
    /* 0x0c */ u32 pos_cnt;
    /* 0x10 */ s32 mispd;
    /* 0x14 */ f32 jumpspd_limit;
    /* 0x18 */ f32 tenjo_hosei;
    /* 0x1c */ u32 f_phcode;
    /* 0x20 */ s32 muki;
    /* 0x24 */ s32 stat;
    /* 0x28 */ f32 motcnt;
    /* 0x2c */ f32 motcnt_end;
    /* 0x30 */ s32 motstop_flag;
    /* 0x34 */ s32 mot_actno;
    /* 0x38 */ s32 se_id0;
    /* 0x3c */ s32 cnt;
    /* 0x40 */ s32 area_cnt;
    /* 0x50 */ sceVu0FVECTOR old_pos;
} SupWORK;

extern void nkPopuka(OBJWORK *objw);
extern void nkSetPopuka();

#endif
