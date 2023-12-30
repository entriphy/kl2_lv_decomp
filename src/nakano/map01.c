#include "nakano/map01.h"
#include "nakano/map00.h"
#include "nakano/main.h"
#include "nakano/dma.h"

extern u32 p_vu1o_iv __attribute__((section(".vudata")));

static s32 vpo_block_num;
static qword *vpo_block_head;

static u32* DecodeUNPACK(u32 *p_pd) {
    u32 code;
    u32 num;
    s32 lp0;
    s16 *wv16;
    u8 *wv8;
    u16 adr;

    code = *p_pd & 0x6F000000;
    num = (*p_pd >> 16) & 0xFF;
    adr = *p_pd & 0xFFFF;
    p_pd++;

    switch (code) {
        case 0x68000000:
            for (lp0 = 0; lp0 < num; p_pd += 3, lp0++);
            break;
        case 0x69000000:
            for (wv16 = (s16 *)p_pd, lp0 = 0; lp0 < num; wv16 += 3, lp0++);
            if (num & 1) wv16++;
            p_pd = (u32 *)wv16;
            break;
        case 0x6A000000:
            for (wv8 = (u8 *)p_pd, lp0 = 0; lp0 < num; wv8 += 3, lp0++);
            if ((num * 3) & 3) wv8 += 4 - ((num * 3) & 3);
            p_pd = (u32 *)wv8;
            break;
        case 0x6C000000:
            for (lp0 = 0; lp0 < num; p_pd += 4, lp0++);
            break;
    }

    return p_pd;
}

void DebugVIFcode(qword *prim) {
    u32 *p_pd;
    s32 lp0;
    u32 vifcd;
    u32 num;
    u32 adr;

    p_pd = (u32 *)(*prim)[1];
    for (lp0 = 0; lp0 < 16; lp0++) {
        vifcd = *p_pd & 0x7F000000;
        num = (*p_pd >> 16) & 0xFF;
        adr = *p_pd & 0xFFFF;

        switch (vifcd) {
            case 0x00000000:
                p_pd++;
                break;
            case 0x01000000:
                p_pd++;
                break;
            case 0x02000000:
                p_pd++;
                break;
            case 0x03000000:
                p_pd++;
                break;
            case 0x04000000:
                p_pd++;
                break;
            case 0x05000000:
                p_pd++;
                break;
            case 0x06000000:
                p_pd++;
                break;
            case 0x07000000:
                p_pd++;
                break;
            case 0x10000000:
                p_pd++;
                break;
            case 0x11000000:
                p_pd++;
                break;
            case 0x13000000:
                p_pd++;
                break;
            case 0x14000000:
                p_pd++;
                break;
            case 0x17000000:
                p_pd++;
                break;
            case 0x15000000:
                p_pd++;
                break;
            case 0x20000000:
                p_pd += 2;
                break;
            case 0x30000000:
                p_pd += 5;
                break;
            case 0x31000000:
                p_pd += 5;
                break;
            case 0x4A000000:
                p_pd++;
                p_pd += num * 2;
                break;
            case 0x50000000:
                p_pd++;
                p_pd += adr * 4;
                break;
            default:
                if ((vifcd >> 24) & 0x60 ) {
                    p_pd = DecodeUNPACK(p_pd);
                } else {
                    p_pd++;
                }
                break;
        }
    }
}

static void nkInitAtrVu1oIv() {
    ATR_VU1O_IV *pp = (ATR_VU1O_IV *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_VU1O_IV) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;

    nkSetObjAttrUnpack(pp->t00, 0x3AC, 1);
    pp->fixednum[0] = 0.5f;
    ((s32 *)pp->fixednum)[1] = 0x8000;
    pp->fixednum[2] = 1.0f;
    pp->fixednum[3] = 1.0f;

    nkSetObjAttrUnpack(pp->t01, 0x3AD, 1);
    pp->clipnum[0] = 2048.0f;
    pp->clipnum[1] = 0.0f;
    pp->clipnum[2] = 0.0f;
    pp->clipnum[3] = 2047.0f;

    nkSetObjAttrDBlbuf(pp->dblbuf, 0x000, 0x1C0);
    nkSetObjAttrMscal(pp->mscal, 2);

    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

void nkDrawHitModel() {
    u32 i;
    u32 nblock;

    nkMakeUnpackVif1(&p1_packet, GameGbl.wsm, 0x390, 4);
    nkAddRef(&p_vu1o_iv);
    nkInitAtrVu1oIv();

    nblock = vpo_block_num;
    if (nblock > 2048) {
        nblock = 2048;
    }
    for (i = 0; i < nblock; i++) {
        nkAddRef(vpo_block_head[i * 2]);
    }

    FlushCache(WRITEBACK_DCACHE);
    nkP1Flush(p1_ot);
}