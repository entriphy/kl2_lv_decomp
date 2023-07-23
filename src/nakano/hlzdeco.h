#ifndef HLZDECO_H
#define HLZDECO_H

#include "nakano.h"

typedef struct {
    u16 bit_buf;
    u16 bit_buf2;
    s16 bit8;
    u16 blocksize;
    u8 *in_data_adrs;
    s32 end_flag;
    s64 block_num;
    u8  p_len[19];
    u8  c_len[510];
    u16 p_table[256];
    u16 c_table[4096];
    u16 left[1019];
    u16 right[1019];
    u16 count[17];
    u16 weight[17];
    u16 start[18];
} HLZWork;

extern u8 *HLZDecode(u8 *src, u8 *dst, HLZWork *wk_adrs);

#endif
