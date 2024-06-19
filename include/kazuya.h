#ifndef KAZUYA_H
#define KAZUYA_H

#include "common.h"
#include "harada/hr_pflag.h"

typedef struct { // 0x4
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
} KZCOLOR;

typedef struct { // 0x4
    /* 0x0 */ s16 vx;
    /* 0x2 */ s16 vy;
} DVECTOR;

// TODO: This should probably go in an Okano header
typedef struct {
    s32 ExID;
    s32 pad0;
    s32 pad1;
    s32 pad2;
} OKDATA;

// TODO: check this
typedef struct {
    short exist;
    s8 remain;
    u8 diverge;
    s32 genju_time[6];
    s32 ex_time[2];
    u64 clear;
    u8 cov_yume;
    u8 stg_yume[18];
    s8 stg_mmd[18];
    u8 pnt_kind[25]; // PNT_KIND
    s32 point; // POINT_NUM
    s32 clear_string; // STR
    OKDATA ok_data[18];
    HRMEMC hr_data;
} GAMEDATA;

typedef struct {
    s32 bgm_no;
    s32 ch_no;
    s32 name;
} BGM_INFO;

typedef struct {
    BGM_INFO **list;
    s32 num;
} BGM_LIST_INFO;

typedef struct {
    s32 flag;
    u64 se_no;
    s32 obj_no;
    f32 panl;
    f32 panr;
} SE_INFO;

typedef struct { // 0x1a
    /* 0x00 */ s16 name[10];
    /* 0x14 */ s8 pad;
    /* 0x15 */ s8 sound;
    /* 0x16 */ DVECTOR adjust;
} KZ_MC_INFO;

typedef enum {
    STAGE_01,
    STAGE_02,
    STAGE_04,
    STAGE_05,
    STAGE_06,
    STAGE_07,
    STAGE_08,
    STAGE_09,
    STAGE_10,
    STAGE_11,
    STAGE_12,
    STAGE_13,
    STAGE_14,
    STAGE_15,
    STAGE_16,
    STAGE_17,
    STAGE_EX1,
    STAGE_EX2,
    // STAGE_NUM
} STAGE_NUM;

enum POINT_NUM {
    POINT_BUGJI,
    POINT_STG_01,
    POINT_STG_02,
    POINT_STG_04,
    POINT_STG_05,
    POINT_STG_06,
    POINT_STG_07,
    POINT_STG_09,
    POINT_STG_10,
    POINT_STG_11,
    POINT_STG_14,
    POINT_STG_15,
    POINT_STG_16,
    POINT_STG_17,
    POINT_BOSS_1,
    POINT_BOSS_2,
    POINT_BOSS_3,
    POINT_BOSS_4,
    POINT_BOSS_5,
    POINT_BOSS_6,
    POINT_MOMMET,
    POINT_STG_EX1,
    POINT_STG_EX2,
    POINT_WARP1,
    POINT_WARP2,
    POINT_NUM
};

typedef struct {
    VPOINFO *vpo;
    sceVu0FVECTOR *pos;
    u8 neighbor_point[4]; // POINT_NUM
    u8 neighbor_route[4]; // RTDAT
    s16 vision[2];
    s16 clr_ev[2];
    /* 0x18 */ s16 place[2];
} POINT_INFO;

enum BOSS_NUM {
    BOSS_1,
    BOSS_2,
    BOSS_3,
    BOSS_4,
    BOSS_5,
    BOSS_6,
    BOSS_NUM,
};

enum STG_MODE {
    STG_MODE_GAME,
    STG_MODE_BOSS,
    STG_MODE_SROLL,
    STG_MODE_SELECT,
    STG_MODE_OTHER,
    STG_MODE_NUM,
};

#endif
