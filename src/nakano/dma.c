#include "common.h"

#define setCHCR(ch, m, t) *((u16 *)&(ch)->chcr) = (m) << D_CHCR_MOD_O | (t) << D_CHCR_TTE_O

s32 nkDmaCdNum = 0;
static char *nk_dma_mode_name[] = { "REFE", "CNT", "NEXT", "REF", "REFS", "CALL", "RET", "END" };

void jDmaPauseOn() {
    while (!(DGET_D_ENABLER() & D_ENABLER_CPND_M))
        DPUT_D_ENABLEW(D_ENABLER_CPND_M);
}

void jDmaPauseOff() {
    while (DGET_D_ENABLER() & D_ENABLER_CPND_M)
        DPUT_D_ENABLEW(0);
}

void nkInitDma() {
    sceDmaReset(1);
    sceDevGifPutImtMode(0);
    DmaChVIF0 = sceDmaGetChan(SCE_DMA_VIF0);
    setCHCR(DmaChVIF0, 1, 1);
    DmaChVIF1 = sceDmaGetChan(SCE_DMA_VIF1);
    setCHCR(DmaChVIF1, 1, 1);
    DmaChGIF = sceDmaGetChan(SCE_DMA_GIF);
    setCHCR(DmaChGIF, 1, 0);
    DmaChfromSPR = sceDmaGetChan(SCE_DMA_fromSPR);
    setCHCR(DmaChfromSPR, 0, 0);
    DmaChtoSPR = sceDmaGetChan(SCE_DMA_toSPR);
    setCHCR(DmaChtoSPR, 0, 0);
}

void nk_PrintOT(qword *tag) {
    u32 mode;
    u32 qwc;
    qword *ret_tag[2];

    memset(ret_tag, 0, sizeof(ret_tag));

    do {
        mode = (*tag)[0] & 0x70000000;
        qwc = (*tag)[0] & 0xFFFF;

        switch (mode) {
            case 0x00000000: // REFE
                mode = 0x70000000;
                break;
            case 0x10000000: // CNT
                tag = (qword *)tag[qwc + 1];
                break;
            case 0x20000000:
                tag = (qword *)(*tag)[1];
                break;
            case 0x50000000: // CALL
                if (ret_tag[1] == NULL) {
                    if (ret_tag[0] != NULL)
                        ret_tag[1] = tag + 1;
                    else
                        ret_tag[0] = tag + 1;
                    tag = (qword *)(*tag)[1];
                } else {
                    mode = 0x70000000;
                }
                break;
            case 0x60000000: // RET
                if (ret_tag[1] != NULL) {
                    tag = ret_tag[1];
                    ret_tag[1] = NULL;
                } else {
                    if (ret_tag[0] != NULL) {
                        tag = ret_tag[0];
                        ret_tag[0] = NULL;
                    } else {
                        mode = 0x70000000;
                    }
                }
                break;
            case 0x30000000: // REF
            case 0x40000000: // REFS
                tag++;
                break;
            default:
                break;
        }
    } while (mode != 0x70000000); // END
}

void* nkAddRef(void * p1) {
    memcpy(p1_packet, p1, 0x10);
    p1_packet++;
    return p1;
}

qword* nkMakeUnpackVif1(qword **pbp, void *data, s32 vu_addr, s32 qwc) {
    qword *pb;

    pb = *pbp;
    pb[0][0] = (qwc + 1) | 0x10000000;
    pb[0][2] = pb[0][3] = pb[0][1] = 0;
    pb[1][0] = pb[1][1] = pb[1][2] = 0;
    pb[1][3] = vu_addr | qwc << 0x10 | 0x6C000000;
    pb += 2;

    while (qwc-- > 0) {
        pb[0][0] = *((u32 *)data)++;
        pb[0][1] = *((u32 *)data)++;
        pb[0][2] = *((u32 *)data)++;
        pb[0][3] = *((u32 *)data)++;
        pb++;
    }

    *pbp = pb;
    return pb;
}

void nkDmaSendGif(qword *addr) {
    sceDmaSync(DmaChGIF, 0, 0);
    sceDmaSend(DmaChGIF, addr);
}

void nkLoadGms(qword *gms) {
    FlushCache(WRITEBACK_DCACHE);
    nkDmaSendGif(gms);
    sceGsSyncPath(0, 0);
}

void nkClearOT(qword *tagp, s32 otn, s32 pce) {
    qword *uncache_tagp;
    u32 mode;

    uncache_tagp = (qword *)((u32)tagp | 0x20000000);
    mode = 0x20000000;
    while (otn-- > 0) {
        (*uncache_tagp)[0] = mode;
        (*uncache_tagp)[1] = (u32)(tagp + 1);
        (*uncache_tagp)[2] = (*uncache_tagp)[3] = 0;

        tagp++;
        uncache_tagp++;
    }

    (*uncache_tagp)[0] = 0x70000000;
    (*uncache_tagp)[1] = 0;
    (*uncache_tagp)[2] = (*uncache_tagp)[3] = 0;
}

void nkPathReset() {
    draw_id = !draw_id;
    p1_ot = p1_ot_buff[draw_id];
    p1_packet_top = p1_packet_buff[draw_id];
    p1_packet = (qword *)((u32)p1_packet_top | 0x20000000);
    p1_packet_cnt = p1_packet;
    nkClearOT(p1_ot, 4, -1);
}

void nkP1Flush(qword *otp) {
    (*p1_packet)[0] = (*otp)[0];
    (*p1_packet)[1] = (*otp)[1];
    (*p1_packet)[2] = (*p1_packet)[3] = 0;
    (*otp)[0] = 0x20000000;
    (*otp)[1] = (u32)p1_packet_cnt & 0xDFFFFFFF;
    p1_packet++;
    p1_packet_cnt = p1_packet;
}

void nkDebFlush() {
    nkP1Flush(&p1_ot[4]);
}

void nkDmaSendVif1(qword *addr) {
    sceDmaSync(DmaChVIF1, 0, 0);
    sceDmaSend(DmaChVIF1, addr);
}

void nkDmaSendVif0(qword *addr) {
    sceDmaSync(DmaChVIF0, 0, 0);
    sceDmaSend(DmaChVIF0, addr);
}

void nkPathFlush() {
    FlushCache(WRITEBACK_DCACHE);
    nkDmaSendVif1(p1_ot);
}

void nkPathBlock() {
    DisableDmac(DMAC_VIF1);
}
