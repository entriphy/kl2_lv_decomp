#ifndef COMPRESSION_H
#define COMPRESSION_H

#include "types.h"

typedef struct {
    u16 s0;
    u16 s1;
    u16 s2;
    u16 s3;
    u8 *stream_pos;
    s32 finished;
    u64 count;
    u8 buffer0[20]; // 0x14
    u8 buffer1[510]; // 0x1FE
    u16 buffer2[256]; // 0x100
    u16 buffer3[4096]; // 0x1000
    u16 buffer4[1019]; // 0x3FB
    u16 buffer5[1019]; // 0x3FB
} DecompressionStructure;

#endif
