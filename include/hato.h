#ifndef HATO_H
#define HATO_H

#include "common.h"

typedef struct {
    s64 n_snd;
    s64 l_pad00;
    s32 f_used;
    s32 n_obj;
    s32 pitch;
    f32 vol;
} htSndData;

typedef struct {
    s32 count;
    s32 d_pitch;
    s32 inter;
    f32 d_vol;
} htDelaySetData;

typedef struct {
    s64 n_snd;
    s64 l_pad00;
    s32 pitch;
    s32 d_pitch;
    s32 d_count;
    s32 d_inter;
    f32 vol;
    f32 d_vol;
    f32 f_pad01;
    f32 f_pad00;
} htSoundDelayData;

#endif
