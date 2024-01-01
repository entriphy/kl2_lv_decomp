#include "nakano/vstart.h"
#include "harada/hr_pall.h"

static s32 nkVisionStart_Flag;

void nkVisionStartInit() {
    nkVisionStart_Flag = 0;
}

void nkVisionStartMain() {
    if (GameGbl.playdemo_flag != 0) {
        nkVisionStart_Flag = 1;
    } else {
        if (hr_pt_check() == 1 || nkVisionStart_Flag != 0) {
            return;
        }
        nkVisionStart_Flag = 1;
        kzVisionStartOn();
    }
}
