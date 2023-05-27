#ifndef CAM_H
#define CAM_H

#include "types.h"

typedef struct {
    sceVu0FVECTOR posi;
    sceVu0FVECTOR ang;
    f32 scr_z;
    s32 mode;
} CAM_WORK;

#endif
