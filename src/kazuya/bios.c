#include "bios.h"

s32 kz_Get_KazuyaStageNo() {
    s32 vision = GameGbl.vision >> 8;
    return vision > 29 && vision < 36 ? vision : -1;
}
