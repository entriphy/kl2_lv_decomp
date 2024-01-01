#include "nakano/tmddraw.h"

s32 VertClip(sceVu0FVECTOR vf0) {
    s32 ret = 0;
    if (vf0[0] < 1678.0f || vf0[0] > 2418.0f || vf0[1] < 1911.0f || vf0[1] > 2185.0f) {
        ret = 1;
    }
    if (vf0[3] < 32.0f) {
        ret = 1;
    }
    return ret;
}
