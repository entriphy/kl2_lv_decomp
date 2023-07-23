#include "gimsys.h"

GIMWORK* GetGimWork() {
    GIMWORK *work = (GIMWORK *)getmem(sizeof(GIMWORK));
    memset(work, 0, sizeof(GIMWORK));
    return work;
}

s32 gmGetGimType(OBJWORK* objw) {
    GIMWORK *gim;

    gim = (GIMWORK *)objw->gimmick;
    if (gim == 0x0)
        return 0;

    if ((gim->type & 2) != 0) {
        return 1;
    } else {
        if ((gim->type & 4) == 0) {
            return 0;
        } else {
            return 2;
        }
    }
}
