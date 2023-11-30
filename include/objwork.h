#ifndef OBJWORK_H
#define OBJWORK_H

#include "route.h"
#include "types.h"

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

// Most likely incorrect
typedef struct {
    /* 0x00 */ RT_WRK rtw;
    s32 field_0x10;
    /* 0x14 */ s32 miexspd;
    /* 0x18 */ s32 tumspd;
    /* 0x1c */ s32 gimspd;
    /* 0x20 */ u32 bun_buf;
    /* 0x24 */ u32 Muki;
    /* 0x28 */ u32 buki_muki;
    /* 0x2c */ u32 timer;
    /* 0x30 */ u32 timer2;
    /* 0x34 */ s32 yarare_bun;
    /* 0x38 */ s32 tenmetu;
    /* 0x3c */ u32 Muki_buf;
    /* 0x40 */ u32 door_muki;
    /* 0x44 */ s32 total_rakka_time;
    /* 0x48 */ s32 count;
    /* 0x4c */ s32 muki2;
    /* 0x50 */ s32 michinori;
    /* 0x54 */ s32 michispd;
    /* 0x58 */ s32 mispd_buf;
    /* 0x5c */ s32 mispd;
    /* 0x60 */ s32 spflag;
    /* 0x64 */ s32 spflag2;
    /* 0x68 */ s32 da_muteki;
    /* 0x6c */ s32 noyukatime;
    /* 0x70 */ OBJWORK* mochifuku;
    s32 jmp_cnt;
    f32 saka;
    f32 yarare_yposi;
    u32 f_phcode;
    s32 lvl;
    s32 trg;
    s32 ex_lvl;
    s32 ex_trg;
    f32 sjump_flag;
    f32 sjump_yspd;
    f32 tenjo_hosei;
    s32 retflag;
    s32 left_move;
    s32 right_move;
    s32 up_move;
    s32 down_move;
    u32 gimm_flag;
    OBJWORK* norigimm;
    OBJWORK* movebox;
    s32 yuka_hit;
    s32 yuka_hit_buf;
    s32 ottoto;
    s32 rakka_flag;
    s32 wahoo_cnt;
    s32 wahoo_timer;
    /* 0xd8 */ OBJWORK *okuyuka;
    /* 0xdc */ s32 con;
    s32 b_action;
    s32 b_act_time;
    sceVu0FVECTOR Scale;
    s32 se_id;
    s32 se_id2;
    s32 se_id3;
    s32 se_id4;
    s32 se_id5;
    s32 se_id6;
    s32 se_id7;
    s32 se_id8;
    s32 se_id9;
    s32 dai_se_cnt;
    s32 yuka_hit_old;
    s32 b_act_rflag;
    f32 b_act_rot;
    f32 motcnt;
    f32 motcnt_end;
    s32 motstop_flag;
    s32 mot_actno;
    /* 0x144 */ s32 kasokuLvL;
    /* 0x148 */ f32 kasokuTime;
    /* 0x14c */ f32 kasokuMTime;
    /* 0x150 */ f32 center_time;
    s32 rupu_cnt;
    s32 run_ef_cnt;
    /* 0x158 */ s32 dead_cnt;
    s32 hima_cnt;
    s32 taiho_bun;
    s32 area_cnt;
    s32 kage_flag;
    /* 0x16c */ f32 jumpspd_limit;
    s32 rakka_cnt;
    s32 pad;
    /* 0x180 */ sceVu0FVECTOR slant;
    OBJWORK *effBoardFlear;
} HERO_WORK;

#endif
