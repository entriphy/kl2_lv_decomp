#include "okanoyo/debug/i_alc.h"
#include "okanoyo/okimain.h"

void CopySI_WORK(SI_WORK *dist, SI_WORK *sorc) {
    dist->pos[0] = sorc->pos[0];
    dist->pos[1] = sorc->pos[1];
    dist->pos[2] = sorc->pos[2];
    dist->pos[3] = sorc->pos[3];
    dist->rot[0] = sorc->rot[0];
    dist->rot[1] = sorc->rot[1];
    dist->rot[2] = sorc->rot[2];
    dist->rot[3] = sorc->rot[3];
}

void OkItemDataInport(s32 *buf, SI_WORK *sibuf) {
    ITEM_DATA_HEAD *idhp = (ITEM_DATA_HEAD *)buf;
    SI_WORK *sp = (SI_WORK *)(idhp + 1);
    s32 i;
    s32 *ip;

    for (i = 0; i < idhp->itemcnt; i++, sp++, sibuf++) {
        CopySI_WORK(sibuf, sp);
    }

    ip = (s32 *)sp;

    for (i = 0; i < 9; i++, ip++) {
        okitemflagX[i] = *ip;
    }

    for (i = 0; i < 33; i++, ip++) {
        okitemflag2[i] = *ip;
    }
}
