#ifndef TARO_H
#define TARO_H

#include "common.h"

#define SET_DMACNT(size) (DMAcnt | size)
#define SET_DMAREF(size, address) (DMAref | size | (u64)(address & 0x0FFFFFFF) << 32)
#define SET_DMAREF2(size, address) (DMAref | size | (u64)(address) << 32)

#endif
