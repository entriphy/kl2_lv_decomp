#include "h_pad.h"
#include "nakano/nkpad.h"

static hPAD_DATA Pad[2];

void hPadInit() {
    s32 i;

    for (i = 0; i < 2; i++) {
        memset(&Pad[i], 0, sizeof(hPAD_DATA));
        Pad[i].rwait0 = 32;
        Pad[i].rwait1 = 8;
    }
}

void hPadRead() {
    nkGetPad();
    GameGbl.kpd[0].lvl = nkGetPlvl(0);
    GameGbl.kpd[1].lvl = nkGetPlvl(1);
    Pad[0].old = Pad[0].l;
    Pad[0].l = GameGbl.kpd[0].lvl & 0xFFFF;
    Pad[1].old = Pad[1].l;
    Pad[1].l = GameGbl.kpd[1].lvl & 0xFFFF;
    hPadMake(&Pad[0]);
    hPadMake(&Pad[1]);
}

void hPadMake(hPAD_DATA *pp) {
    s32 j;
    u32 padr;

    pp->on = ~pp->old & pp->l;
    pp->off = ~pp->l & pp->old;

    padr = 1;
    for (j = 0; j < 16; j++, padr <<= 1) {
        if (padr & pp->l) {
            if (pp->rep_on[j] == 0) {
                if (pp->rcount[j]++ >= pp->rwait0) {
                    pp->rcount[j] = 0;
                    pp->rcount2[j] = 0;
                    pp->rep_on[j] = 1;
                    pp->r |= padr;
                    pp->r2 |= padr;
                }
            } else {
                pp->rep_on[j]++;
                if (pp->rep_on[j] > 64)
                    pp->rep_on[j] = 64;
                if (pp->rcount2[j]++ >= ((64 - pp->rep_on[j]) * pp->rwait1) / 64) {
                    pp->r2 |= padr;
                    pp->rcount2[j] = 0;
                } else {
                    pp->r2 &= ~padr;
                }
                if (pp->rcount[j]++ >= pp->rwait1) {
                    pp->r |= padr;
                    pp->rcount[j] = 0;
                } else {
                    pp->r &= ~padr;
                }
            }
        } else {
            pp->rep_on[j] = 0;
            pp->rcount[j] = 0;
            pp->rcount2[j] = 0;
        }
    }

    pp->r &= pp->l;
    pp->r2 &= pp->l;
}

void hPadGet(hPAD_DATA *pad, s32 i) {
    memcpy(pad, &Pad[i], sizeof(hPAD_DATA));
}
