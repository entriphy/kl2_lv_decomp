#ifndef VPO_H
#define VPO_H

#include "types.h"

typedef struct {
    u32 ntag;
    IVECTOR* dmatag;
    u8* data_top;
    u32 pad0;
} VPOINFO;

#endif
