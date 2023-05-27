#include "common.h"

s32 key_data_top_index;
s32 key_frame_cnt;
s32 key_frame_max;
s32 kf_mode;

void nkKeyFrameStart() {
    key_frame_cnt = 0;
    kf_mode = 1;
}
