#ifndef NAKANO_H
#define NAKANO_H

#include "types.h"
#include "ps2.h"

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

typedef struct {
    float SrcZ;
    float AspectX;
    float AspectY;
    float CenterX;
    float CenterY;
    float MinZ;
    float MaxZ;
    float NearZ;
    float FarZ;
    float pad[3];
    sceVu0FVECTOR camera_p;
    sceVu0FVECTOR camera_zd;
    sceVu0FVECTOR camera_yd;
    sceVu0FMATRIX WvMtx;
    sceVu0FMATRIX VsMtx;
    sceVu0FMATRIX WsMtx;
} SCRENV;

typedef struct {
	u16 bit_buf;
	u16 bit_buf2;
	s16 bit8;
	u16 blocksize;
	u8 *in_data_adrs;
	s32 end_flag;
	s64 block_num;
	u8 p_len[19];
	u8 c_len[510];
	u16 p_table[256];
	u16 c_table[4096];
	u16 left[1019];
	u16 right[1019];
	u16 count[17];
	u16 weight[17];
	u16 start[18];
} HLZWork;

extern u8 *HLZDecode(u8 *src, u8 *dst, HLZWork *wk_adrs);

#define nkSPR (nkQWdata *)0x70000000

#endif
