#include "drawsub.h"
#include "object.h"

void DrawTexPoly(GIMINFO *giminfo, s32 num, u128 *pRGB, u128 *pUV, u128 *pXYZ) {
    s32 ind;
    s32 i;
    qword_uni *pPacket;
    u128 PacketBuff[64];
    u32 dmatag; // Need this to get it to match

    pPacket = (qword_uni *)PacketBuff;

    pPacket[2].u_u64[0] = SCE_GS_SET_TEX0_2(0x3E40, 1, SCE_GS_PSMCT32, 9, 5, 1, 0, 0x0000, SCE_GS_PSMCT32, 0, 0, 1);
    pPacket[2].u_u64[1] = SCE_GS_TEX0_2;
    pPacket[3].u_u64[0] = SCE_GS_SET_TEST_2(1, 1, 0, 0, 0, 0, 1, 1);
    pPacket[3].u_u64[1] = SCE_GS_TEST_2;
    pPacket[4].u_u64[0] = SCE_GS_SET_TEX1_2(0, 0, 0, 0, 0, 0, 0);
    pPacket[4].u_u64[1] = SCE_GS_TEX1_2;
    pPacket[5].u_u64[0] = SCE_GS_SET_ALPHA_2(1, 1, 2, 1, 128);
    pPacket[5].u_u64[1] = SCE_GS_ALPHA_2;
    pPacket[6].u_u64[0] = SCE_GS_SET_TEXA(64, 0, 64);
    pPacket[6].u_u64[1] = SCE_GS_TEXA;
    pPacket[7].u_u64[0] = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMZ16S, 1);
    pPacket[7].u_u64[1] = SCE_GS_ZBUF_2;
    pPacket[8].u_u64[0] = SCE_GS_SET_FBA_2(0);
    pPacket[8].u_u64[1] = SCE_GS_FBA_2;
    pPacket[9].u_u64[0] = SCE_GS_SET_PABE(0);
    pPacket[9].u_u64[1] = SCE_GS_PABE;

    pDma.Gif->chcr.TTE = 0;
    SCE_GIF_CLEAR_TAG(pPacket);
    dmatag = 0x70000000;
    pPacket[0].u_u32[0] = dmatag | 9;
    pPacket[1].u_u64[0] = SCE_GIF_SET_TAG(8, 1, 0, 0, 0, 1);
    pPacket[1].u_u64[1] = SCE_GIF_PACKED_AD;

    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, PacketBuff);
    sceDmaSync(pDma.Gif, 0, 0);

    pPacket = (qword_uni *)PacketBuff;
    SCE_GIF_CLEAR_TAG(pPacket);
    pPacket[0].u_u32[0] = dmatag | num * 3 + 1;
    pPacket[1].u_u64[0] = SCE_GIF_SET_TAG(num, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 0, 0, 1, 1, 0), 0, 3);
    pPacket[1].u_u64[1] = SCE_GS_RGBAQ | SCE_GS_UV << 4 | SCE_GS_XYZF2 << 8;

    ind = 2;
    for (i = 0; i < num; i++) {
        pPacket[ind++].u_u128 = *pRGB++;
        pPacket[ind++].u_u128 = *pUV++;
        pPacket[ind++].u_u128 = *pXYZ++;
    }

    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, pPacket);
    sceDmaSync(pDma.Gif, 0, 0);
}