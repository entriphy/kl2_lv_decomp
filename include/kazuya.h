#ifndef KAZUYA_H
#define KAZUYA_H

#include "types.h"

// This not might be a Kazuya struct
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

#endif