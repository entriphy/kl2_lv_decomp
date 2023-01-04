#ifndef CAM_H
#define CAM_H

#include <libvux.h>
#include "types.h"

typedef struct {
    FVECTOR posi;
    FVECTOR ang;
    float scr_z;
    s32 mode;
} CAM_WORK;

#endif