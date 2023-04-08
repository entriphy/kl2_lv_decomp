#ifndef NAKANO_H
#define NAKANO_H

#include "types.h"

typedef struct {
    s32 map_draw_flag;
    s32 cam_debug_mode;
    s32 hitmap_flag;
    s32 meter_cnt;
    s32 meter_num;
    s32 flag;
    s32 opflag;
    s32 capture_num;
    s32 work_path[16];
} NKDEBGBL;

typedef struct {
    s32 outtime;
    s32 intime;
    s32 next_vision;
    s32 rtn;
    s32 mcn;
    s32 imuki;
    s32 omuki;
    s32 flag;
    s32 fdata;
} nkWIPE_WRK;

typedef union {
    u128 ul128;
    u64 ul64[2];
    u32 ul32[4];
    IVECTOR vect;
    FVECTOR fvct;
} nkQWdata;

typedef struct {
    nkQWdata *buf;
    u32 size;
} nkGifPacket;

#define nkSPR (nkQWdata *)0x70000000

#endif
