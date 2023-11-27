#include "nakano/calc3d.h"

s32 nkVu0ClipScreen(sceVu0FVECTOR v0) {
    sceVu0IVECTOR vi;
    s32 flag;

    v0[3] = 1.0f;
    sceVu0RotTransPers(vi, GameGbl.wsm, v0, 1);
    vi[0] = (vi[0] - 0x6C00) >> 4;
    vi[1] = (vi[1] - 0x7900) >> 4;
    if (vi[0] > 0x280U || vi[1] < 0 || vi[1] > 0xF0 || vi[2] - 1 > 0xFFFFFDU) {
        flag = 1;
    } else {
        flag = 0;
    }

    return flag;
}
