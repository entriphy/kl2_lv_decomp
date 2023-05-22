#ifndef ROUTE_H
#define ROUTE_H

#include "types.h"

typedef struct {
    SVECTOR vec;
    IVECTOR_UA posi;
    u16 co;
    u16 DUMMY;
} ROUTE;

typedef struct {
    s32 cnt;
    ROUTE *rtp;
    s16 plc;
    s16 rtn;
    s32 mcn;
} RT_WRK;

#endif
