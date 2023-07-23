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
    RT_WRK rtw;
    s32 idk;
    s32 miexspd; // 0x14
    s32 tumspd;
    s32 gimspd;
    u32 bun_buf;
    u32 Muki;
    u32 buki_muki;
    u32 timer;
    u32 timer2;
    s32 yarare_bun;
    s32 tenmetu; // 0x38
    u32 Muki_buf;
    u32 door_muki;
    s32 total_rakka_time;
    s32 count;
    s32 muki2;
    s32 michinori;
    s32 michispd;
    s32 mispd_buf;
    s32 mispd; // 0x5C
    s32 spflag;
    s32 spflag2;
    s32 da_muteki; // 0x68
    s32 noyukatime;
    OBJWORK* mochifuku; // 0x70
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
    OBJWORK* okuyuka;
    s32 con;
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
    s32 kasokuLvL; // 0x144
    f32 kasokuTime; // 0x148
    f32 kasokuMTime; // 0x14C
    f32 center_time;
    s32 rupu_cnt;
    s32 run_ef_cnt;
    s32 dead_cnt;
    s32 hima_cnt;
    s32 taiho_bun;
    s32 area_cnt;
    s32 kage_flag;
    f32 jumpspd_limit; // 0x16C
    s32 rakka_cnt;
    s32 pad;
    sceVu0FVECTOR slant; // 0x180
    OBJWORK *effBoardFlear;
} HERO_WORK;

#endif
