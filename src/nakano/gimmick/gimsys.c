#include "common.h"

GIMWORK * GetGimWork() {
    GIMWORK *work = (GIMWORK *)malloc(sizeof(GIMWORK));
    memset(work, 0, sizeof(GIMWORK));
    return work;
}

s32 gmGetGimType(OBJWORK *objw) {
    GIMWORK* gim = objw->gimmick;
    if (gim != NULL) {
        if (gim->type & 2 == 0) {
            return (gim->type >> 1) & 2;
        }
        return 1;
    }
    return 0;
}