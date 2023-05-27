#ifndef STRUCTS_H
#define STRUCTS_H

#include "types.h"

typedef struct {
    s32 (**func)();
    s32 mod;
} FUNCTBL;

#endif
