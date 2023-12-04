#ifndef KEYFSV_H
#define KEYFSV_H

#include "nakano.h"

typedef struct { // 0x8
    /* 0x0 */ u32 lvl;
    /* 0x4 */ u32 trg;
} nkKEY_DATA;

extern void nkKeyFrameStart();

#endif
