#ifndef ROUTE_H
#define ROUTE_H

#include <tamtypes.h>
#include "types.h"

typedef struct {
    VU_SVECTOR vec;
    VU_IVECTOR posi;
    u16 co;
    u16 DUMMY;
} ROUTE;

#endif