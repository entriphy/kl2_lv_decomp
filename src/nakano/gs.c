#include "common.h"

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
    nkGifPacket pk;
    sceGsDrawEnv2 *drawEnv;

    if ((GameGbl.fr - 1 & 1) == 0) {
        drawEnv = &GameGbl.db.draw02;
    } else {
        drawEnv = &GameGbl.db.draw12;
    }
        
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
