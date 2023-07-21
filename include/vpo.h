#ifndef VPO_H
#define VPO_H

#include "types.h"

typedef struct {
    u32 ntag;
    qword *dmatag;
    u32 *data_top;
    u32 pad0;
} VPOINFO; // TODO: might go in harada.h?

#endif
