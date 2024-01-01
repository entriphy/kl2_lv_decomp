#ifndef OBJWORK_H
#define OBJWORK_H

#include "types.h"

typedef struct {
    SVECTOR vec;
    IVECTOR posi;
    u16 co;
    u16 DUMMY;
} ROUTE;

typedef struct {
    s32 cnt;
    ROUTE *rtp;
    s16 plc;
    s16 rtn;
    s32 mcn;
} RT_WRK;

// Size: 0xF0
typedef struct OBJWORK {
    void (*pers)(struct OBJWORK *); // 0x00
    void (*draw)(struct OBJWORK *); // 0x04
    void (*drmir)(struct OBJWORK *); // 0x08
    void (*drmiref)(struct OBJWORK *); // 0x0C
    void (*dreff)(struct OBJWORK *); // 0x10
    void *prim; // 0x14
    void *work; // 0x18
    s16 stat0; // 0x1C
    s16 stat1; // 0x1E
    s16 prty; // 0x20
    s16 hitram; // 0x22
    s16 bun0; // 0x24
    s16 bun1; // 0x26
    s16 pad0; // 0x28
    s16 pad1; // 0x2A
    s16 live; // 0x2C
    s16 stat; // 0x2E
    sceVu0FVECTOR posi; // 0x30
    sceVu0FVECTOR spd; // 0x40
    sceVu0FVECTOR muki; // 0x50
    sceVu0FVECTOR ang; // 0x60
    sceVu0FVECTOR rot; // 0x70
    RT_WRK rtw; // 0x80
    s32 reg0; // 0x90
    s32 reg1; // 0x94
    s32 reg2; // 0x98
    s32 reg3; // 0x9C
    s32 reg4; // 0xA0
    s32 reg5; // 0xA4
    s32 reg6; // 0xA8
    s32 reg7; // 0xAC
    f32 freg0; // 0xB0
    f32 freg1; // 0xB4
    f32 freg2; // 0xB8
    f32 freg3; // 0xBC
    f32 freg4; // 0xC0
    f32 freg5; // 0xC4
    f32 freg6; // 0xC8
    f32 freg7; // 0xCC
    s64 htblid; // 0xD0
    s64 dummy; // 0xD8
    s16 option; // 0xE0
    s16 sflag; // 0xE2
    u32 *hpo; // 0xE4
    u32 *gimmick; // 0xE8
    s32 *_hOp; // 0xEC
} OBJWORK;

typedef struct {
    /* 0x000 */ RT_WRK rtw;
    /* 0x010 */ void (*Main)(/* ??? */);
    /* 0x014 */ s32 miexspd;
    /* 0x018 */ s32 tumspd;
    /* 0x01c */ s32 gimspd;
    /* 0x020 */ u32 bun_buf;
    /* 0x024 */ u32 Muki;
    /* 0x028 */ u32 buki_muki;
    /* 0x02c */ u32 timer;
    /* 0x030 */ u32 timer2;
    /* 0x034 */ s32 yarare_bun;
    /* 0x038 */ s32 tenmetu;
    /* 0x03c */ u32 Muki_buf;
    /* 0x040 */ u32 door_muki;
    /* 0x044 */ s32 total_rakka_time;
    /* 0x048 */ s32 count;
    /* 0x04c */ s32 muki2;
    /* 0x050 */ s32 michinori;
    /* 0x054 */ s32 michispd;
    /* 0x058 */ s32 mispd_buf;
    /* 0x05c */ s32 mispd;
    /* 0x060 */ s32 spflag;
    /* 0x064 */ s32 spflag2;
    /* 0x068 */ s32 da_muteki;
    /* 0x06c */ s32 noyukatime;
    /* 0x070 */ OBJWORK* mochifuku;
    /* 0x074 */ s32 jmp_cnt;
    /* 0x078 */ f32 saka;
    /* 0x07c */ f32 yarare_yposi;
    /* 0x080 */ u32 f_phcode;
    /* 0x084 */ s32 lvl;
    /* 0x088 */ s32 trg;
    /* 0x08c */ s32 ex_lvl;
    /* 0x090 */ s32 ex_trg;
    /* 0x094 */ f32 sjump_flag;
    /* 0x098 */ f32 sjump_yspd;
    /* 0x09c */ f32 tenjo_hosei;
    /* 0x0a0 */ s32 retflag;
    /* 0x0a4 */ s32 left_move;
    /* 0x0a8 */ s32 right_move;
    /* 0x0ac */ s32 up_move;
    /* 0x0b0 */ s32 down_move;
    /* 0x0b4 */ u32 gimm_flag;
    /* 0x0b8 */ OBJWORK *norigimm;
    /* 0x0bc */ OBJWORK *movebox;
    /* 0x0c0 */ s32 yuka_hit;
    /* 0x0c4 */ s32 yuka_hit_buf;
    /* 0x0c8 */ s32 ottoto;
    /* 0x0cc */ s32 rakka_flag;
    /* 0x0d0 */ s32 wahoo_cnt;
    /* 0x0d4 */ s32 wahoo_timer;
    /* 0x0d8 */ OBJWORK *okuyuka;
    /* 0x0dc */ s32 con;
    /* 0x0e0 */ s32 b_action;
    /* 0x0e4 */ s32 b_act_time;
    /* 0x0f0 */ sceVu0FVECTOR Scale;
    /* 0x100 */ s32 se_id;
    /* 0x104 */ s32 se_id2;
    /* 0x108 */ s32 se_id3;
    /* 0x10c */ s32 se_id4;
    /* 0x110 */ s32 se_id5;
    /* 0x114 */ s32 se_id6;
    /* 0x118 */ s32 se_id7;
    /* 0x11c */ s32 se_id8;
    /* 0x120 */ s32 se_id9;
    /* 0x124 */ s32 dai_se_cnt;
    /* 0x128 */ s32 yuka_hit_old;
    /* 0x12c */ s32 b_act_rflag;
    /* 0x130 */ f32 b_act_rot;
    /* 0x134 */ f32 motcnt;
    /* 0x138 */ f32 motcnt_end;
    /* 0x13c */ s32 motstop_flag;
    /* 0x140 */ s32 mot_actno;
    /* 0x144 */ s32 kasokuLvL;
    /* 0x148 */ f32 kasokuTime;
    /* 0x14c */ f32 kasokuMTime;
    /* 0x150 */ f32 center_time;
    /* 0x154 */ s32 rupu_cnt;
    /* 0x158 */ s32 dead_cnt;
    /* 0x15c */ s32 hima_cnt;
    /* 0x160 */ s32 taiho_bun;
    /* 0x164 */ s32 area_cnt;
    /* 0x168 */ s32 kage_flag;
    /* 0x16c */ f32 jumpspd_limit;
    /* 0x170 */ s32 rakka_cnt;
    /* 0x180 */ sceVu0FVECTOR slant;
} HERO_WORK;

#endif
