#include "gssub.h"
#include "main.h"

void nkGsInitFrm() {
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    sceGsSetDefClear(&GameGbl.db.clear0, 3, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    sceGsSetDefClear(&GameGbl.db.clear1, 3, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    FlushCache(WRITEBACK_DCACHE);
}

void nkGsSetNormalFZ2_0() {
    nkGifPacket pk;
    sceGsDrawEnv2 *drawEnv;

    drawEnv = &GameGbl.db.draw02;
        
    pk.size = 0;
    pk.buf = nkSPR;
    pk.buf->ul128 = 0;

    pk.buf[pk.size++].ul32[0] = 0x70000004;
    
    pk.buf[pk.size].ul64[0] = SCE_GIF_SET_TAG(3, 1, 0, 0, 0, 1);
    pk.buf[pk.size++].ul64[1] = 0xE;

    pk.buf[pk.size].ul64[1] = SCE_GS_FRAME_2;
    pk.buf[pk.size++].ul64[0] = *(u64 *)&drawEnv->frame2;

    pk.buf[pk.size].ul64[1] = SCE_GS_XYOFFSET_2;
    pk.buf[pk.size++].ul64[0] = *(u64 *)&drawEnv->xyoffset2;

    pk.buf[pk.size].ul64[1] = SCE_GS_ZBUF_2;
    pk.buf[pk.size++].ul64[0] = *(u64 *)&drawEnv->zbuf2;

    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pk.buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void nkGsSetNormalFZ2_1() {
    nkGifPacket pk;
    sceGsDrawEnv2 *drawEnv;

    drawEnv = &GameGbl.db.draw12;
        
    pk.size = 0;
    pk.buf = nkSPR;
    pk.buf->ul128 = 0;

    pk.buf[pk.size++].ul32[0] = 0x70000004;
    
    pk.buf[pk.size].ul64[0] = SCE_GIF_SET_TAG(3, 1, 0, 0, 0, 1);
    pk.buf[pk.size++].ul64[1] = 0xE;

    pk.buf[pk.size].ul64[1] = SCE_GS_FRAME_2;
    pk.buf[pk.size++].ul64[0] = *(u64 *)&drawEnv->frame2;

    pk.buf[pk.size].ul64[1] = SCE_GS_XYOFFSET_2;
    pk.buf[pk.size++].ul64[0] = *(u64 *)&drawEnv->xyoffset2;

    pk.buf[pk.size].ul64[1] = SCE_GS_ZBUF_2;
    pk.buf[pk.size++].ul64[0] = *(u64 *)&drawEnv->zbuf2;

    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pk.buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void nkGsSetNormalFZ2() {
    nkGifPacket packet;
    nkGifPacket *pk;
    sceGsDrawEnv2 *env2;

    pk = &packet;
    if ((GameGbl.fr - 1 & 1) != 0) {
        env2 = &GameGbl.db.draw12;
    } else {
        env2 = &GameGbl.db.draw02;
    }
        
    pk->size = 0;
    pk->buf = nkSPR;
    pk->buf->ul128 = 0;

    pk->buf[pk->size++].ul32[0] = 0x70000004;
    
    pk->buf[pk->size].ul64[0] = SCE_GIF_SET_TAG(3, 1, 0, 0, 0, 1);
    pk->buf[pk->size++].ul64[1] = 0xE;

    pk->buf[pk->size].ul64[1] = SCE_GS_FRAME_2;
    pk->buf[pk->size++].ul64[0] = *(u64 *)&env2->frame2;

    pk->buf[pk->size].ul64[1] = SCE_GS_XYOFFSET_2;
    pk->buf[pk->size++].ul64[0] = *(u64 *)&env2->xyoffset2;

    pk->buf[pk->size].ul64[1] = SCE_GS_ZBUF_2;
    pk->buf[pk->size++].ul64[0] = *(u64 *)&env2->zbuf2;

    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pk->buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void nkGsSetFBA1(s32 fba) {
    nkGifPacket pk;
    
    pk.size = 0;
    pk.buf = nkSPR;
    
    SCE_GIF_CLEAR_TAG(pk.buf);
    pk.buf[pk.size++].ul32[0] = 0x70000000 | 0x2;
    pk.buf[pk.size].ul64[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pk.buf[pk.size++].ul64[1] = SCE_GIF_PACKED_AD;
    pk.buf[pk.size].ul64[1] = SCE_GS_FBA_1;
    pk.buf[pk.size++].ul64[0] = fba;
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pk.buf | 0x80000000));
}

void nkGsInitEnv() {
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ16S, SCE_GS_CLEAR);
    GameGbl.db.clear0.rgbaq.A = 128;
    GameGbl.db.clear1.rgbaq.A = 128;
    nkGsSetFBA1(1);
    sceGsSetDefClear(&GameGbl.db.clear0, SCE_GS_ZGREATER, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    sceGsSetDefClear(&GameGbl.db.clear1, SCE_GS_ZGREATER, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    FlushCache(WRITEBACK_DCACHE);
}
