#include "common.h"

void OkPrintSysInit(s32 personal_id, s32 screenw, s32 screenh, s32 fontw, s32 fonth) {
    PDbuf[personal_id].fsz_x = fontw;
    PDbuf[personal_id].fsz_y = fonth;
    PDbuf[personal_id].scr_x = screenw / fontw;
    PDbuf[personal_id].scr_y = screenh / fonth;
}

void OkDefaultSysInit(s32 screenw, s32 screenh, s32 fontw, s32 fonth) {
    s32 i, j;

    for (i = 0; i < 9; i++) {
        PDbuf[i].fsz_x = fontw;
        PDbuf[i].fsz_y = fonth;
        PDbuf[i].scr_x = screenw;
        PDbuf[i].scr_y = screenh;
        for (j = 0; j < 280; j++) {
            // what
        }
    }
}

void OkPFontFlush(s16 trg) {
    // TODO
}
