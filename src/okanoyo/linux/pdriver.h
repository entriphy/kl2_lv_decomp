#ifndef PDRIVER_H
#define PDRIVER_H

#include "okanoyo.h"

#define PROTO_SAMPLE	0xe000
#define NODE_EE		'E'
#define NODE_HOST	'H'
#define BUFSZ 0x10000
#define UNCACHED(p) ((u8 *)((u32)p | 0x20000000))

typedef struct { // 0x18
    /* 0x00 */ vs32 s;
    /* 0x04 */ vu32 wlen;
    /* 0x08 */ vu32 rlen;
    /* 0x0c */ u8 *wptr;
    /* 0x10 */ u8 *rptr;
    /* 0x14 */ vs32 rbytes;
} PDRIVER_OPT;

typedef struct { // 0x20
    /* 0x00 */ PDRIVER_OPT opt;
    /* 0x18 */ u8 *rp;
    /* 0x1c */ u8 *wp;
} PDriver;

extern s32 InitProtocolDriver(PDriver *pd);
extern void SetRWBuffAddress(void *_rp, void *_wp, PDriver *pd);
extern s32 SystemOnHost(char *command, char *result, PDriver *pd);

#endif