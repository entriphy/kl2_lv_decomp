#ifndef MAP01_H
#define MAP01_H

#include "nakano.h"

typedef struct { // 0x70
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword t00;
    /* 0x20 */ sceVu0FVECTOR fixednum;
    /* 0x30 */ qword t01;
    /* 0x40 */ sceVu0FVECTOR clipnum;
    /* 0x50 */ qword dblbuf;
    /* 0x60 */ qword mscal;
} ATR_VU1O_IV;

extern void DebugVIFcode(qword *prim);
extern void nkDrawHitModel();

#endif
