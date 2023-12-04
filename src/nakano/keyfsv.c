#include "nakano/keyfsv.h"

static nkKEY_DATA *key_data_top;
// static s32 key_data_top_index; // ?
static s32 key_frame_cnt;
static s32 key_frame_max;
static s32 kf_mode;

void nkKeyFrameSet(u32 *adr) {
    key_frame_max = *adr;
    adr += 2;
    key_data_top = (nkKEY_DATA *)adr;
    kf_mode = 0;
}

void nkKeyFrameStart() {
    key_frame_cnt = 0;
    kf_mode = 1;
}

void nkKeyFramePlay() {
    /* a0 4 */ HERO_WORK *herow;
    /* v1 3 */ nkKEY_DATA *now_keyf;

    herow = GameGbl.klonoa->work;
    if (key_frame_cnt < key_frame_max) {
        now_keyf = &key_data_top[key_frame_cnt];
        herow->gimm_flag |= 0x1000;
        herow->ex_lvl = now_keyf->lvl;
        herow->ex_trg = now_keyf->trg;
        key_frame_cnt++;
    }
}