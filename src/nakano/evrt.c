#include "nakano/evrt.h"
#include "nakano/vu0mac.h"
#include "nakano/main.h"

static ROUTE *pEvRt[64];

s32 nkInitEvRt(u32 *adr) {
    s32 lp1;
    s32 n;
    u32 *ar;

    ar = adr;
    n = *ar++;

    for (lp1 = 0; lp1 < 64; lp1++) {
        pEvRt[lp1] = NULL;
    }
    for (lp1 = 0; lp1 < n; lp1++, ar++) {
        pEvRt[lp1] = (ROUTE *)((u32)adr + *ar);
    }
    pEvRt[lp1] = NULL;

    return n;
}

ROUTE* GetEvRtAdrs(s32 rtn) {
    return pEvRt[rtn];
}

s32 GetEvRtNo(ROUTE *rtp) {
    s32 lp1;

    lp1 = 0;
    while (pEvRt[lp1] != NULL) {
        if (pEvRt[lp1] == rtp) {
            return lp1;
        }
        lp1++;
    }

    return -1;
}

void DrawEvRt() {
    ROUTE *rtp;
    nkGifPacket pack;
    nkGifPacket *pp;
    sceVu0IVECTOR vi;
    sceVu0FVECTOR vf;
    sceVu0FVECTOR vf0;
    sceVu0FVECTOR vf1;
    sceVu0IVECTOR v0;
    sceVu0IVECTOR v1;
    s32 LP1;
    s32 cnt;

    pp = &pack;
    LP1 = 0;
    cnt = 0;
    while (pEvRt[LP1] != NULL) {
        rtp = pEvRt[LP1];
        while (rtp->co - 0x7FFEU > 1) {
            vi[0] = rtp->posi.x;
            vi[1] = rtp->posi.y;
            vi[2] = rtp->posi.z;
            vu0_ITOF12Vector(vf, vi);
            vf[3] = 1.0f;
            vu0_LoadMtx(GameGbl.wsm);
            vu0_Thing(vf0, vf);
            if (VertClip(vf0) == 0) {
                vu0_FTOI4Vector(v0, vf0);
                vi[0] = rtp->vec.x * rtp->co + rtp->posi.x;
                vi[1] = rtp->vec.y * rtp->co + rtp->posi.y;
                vi[2] = rtp->vec.z * rtp->co + rtp->posi.z;
                vu0_ITOF12Vector(vf, vi);
                vf[3] = 1.0f;
                vu0_LoadMtx(GameGbl.wsm);
                vu0_Thing(vf1, vf);
                if (VertClip(vf1) == 0) {
                    vu0_FTOI4Vector(v1, vf1);
                    pp->size = 0;
                    pp->buf = (nkQWdata *)0x70000000;
                    if (cnt & 1) {
                        pp->buf = (nkQWdata *)0x70002000;
                    }
                    pp->buf[pp->size].ul128 = 0;
                    pp->buf[pp->size++].ul32[0] = 0x70000005;
                    pp->buf[pp->size].ul64[0] = 0x2104c00000008002;
                    pp->buf[pp->size++].ul64[1] = 0x51;
                    pp->buf[pp->size].ul32[0] = 0;
                    pp->buf[pp->size].ul32[1] = 0xFF;
                    pp->buf[pp->size].ul32[2] = 0;
                    pp->buf[pp->size].ul32[3] = 0x80;
                    pp->size++;
                    pp->buf[pp->size].ul32[0] = v0[0];
                    pp->buf[pp->size].ul32[1] = v0[1];
                    pp->buf[pp->size].ul32[2] = v0[2];
                    pp->buf[pp->size].ul32[3] = 0;
                    pp->size++;
                    pp->buf[pp->size].ul32[0] = 0;
                    pp->buf[pp->size].ul32[1] = 0x40;
                    pp->buf[pp->size].ul32[2] = 0x40;
                    pp->buf[pp->size].ul32[3] = 0x80;
                    pp->size++;
                    pp->buf[pp->size].ul32[0] = v1[0];
                    pp->buf[pp->size].ul32[1] = v1[1];
                    pp->buf[pp->size].ul32[2] = v1[2];
                    pp->buf[pp->size].ul32[3] = 0;
                    pp->size++;
                    FlushCache(0);
                    DmaChGIF->chcr.TTE = 1;
                    sceDmaSend(DmaChGIF, (u32)pp->buf | 0x80000000);
                    cnt++;
                }
            }

            rtp++;
        }

        LP1++;
    }
}