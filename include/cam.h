#ifndef CAM_H
#define CAM_H

#include <libvux.h>
#include <tamtypes.h>

typedef struct {
    VU_VECTOR posi;
    VU_VECTOR ang;
    float scr_z;
    s32 mode;
} CAM_WORK;

#endif