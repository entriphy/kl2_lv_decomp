#include "common.h"

// static nkKEY_DATA *key_data_top;
// static s32 key_data_top_index; // ?
static s32 key_frame_cnt;
static s32 key_frame_max;
static s32 kf_mode;

void nkKeyFrameStart() {
    key_frame_cnt = 0;
    kf_mode = 1;
}
