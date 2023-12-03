#include "nakano/gssub.h"
#include "nakano/main.h"

nkSCREEN nkScr = {};
nkFFDRAW nkff = {};

void nkGsDrawFF(s32 inter) {
    sceDmaSync(DmaChGIF, 0, 0);
    FlushCache(0);
    sceDmaSendN(DmaChGIF, &nkScr.gif_view, nkScr.size_view + 1);
    sceDmaSync(DmaChGIF, 0, 0);

    nkff.size_texenv = sceGsSetDefTexEnv(&nkff.texenv, 1, nkScr.drawenv1.frame1.FBP, 10, nkScr.drawenv1.frame1.PSM, 10, 9, 0, 0, 0, 0, 1);
    nkff.alpha = 5;
    nkff.alp_addr = 0x42;
    nkff.size_texenv++;
    nkff.gif_texenv.NLOOP = nkff.size_texenv;
    nkff.gif_texenv.EOP = 1;
    nkff.gif_texenv.PRE = 0;
    nkff.gif_texenv.PRIM = 0;
    nkff.gif_texenv.FLG = 0;
    nkff.gif_texenv.NREG = 1;
    nkff.gif_texenv.REGS0 = SCE_GIF_PACKED_AD;
    FlushCache(0);
    sceDmaSendN(DmaChGIF, &nkff.gif_texenv, nkff.size_texenv + 1);
    sceDmaSync(DmaChGIF, 0, 0);

    {
        nkGifPacket packet;
        nkGifPacket *pp;

        pp = &packet;
        pp->size = 0;
        pp->buf = (nkQWdata *)0x70000000;
        pp->buf[pp->size].ul128 = 0;
        pp->buf[pp->size++].ul32[0] = 0x7000000D;
        pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 0, 0, 0, 0, 0), 0, 3);
        pp->buf[pp->size++].ul64[1] = SCE_GS_ST | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;

        pp->buf[pp->size].fvct[0] = -2.0f / 4096.0f; // 0xBA000000
        pp->buf[pp->size].fvct[1] = inter == 0 ? 448.0f / 512.0f : 449.0f / 512.0f; // 0x3F600000 : 0x3F608000
        pp->buf[pp->size].fvct[2] = 1.0f;
        pp->buf[pp->size].fvct[3] = 0.0f;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x88;
        pp->buf[pp->size].ul32[1] = 0x88;
        pp->buf[pp->size].ul32[2] = 0x88;
        pp->buf[pp->size].ul32[3] = 0x80;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x6C00;
        pp->buf[pp->size].ul32[1] = 0x8700;
        pp->buf[pp->size].ul32[2] = 0xFFFFF;
        pp->buf[pp->size].ul32[3] = 0;
        pp->size++;

        pp->buf[pp->size].fvct[0] = -2.0f / 4096.0f; // 0xBA000000
        pp->buf[pp->size].fvct[1] = inter == 0 ? 0.0f : 1.0f / 512.0f; // 0x00000000 : 0x3B000000
        pp->buf[pp->size].fvct[2] = 1.0f;
        pp->buf[pp->size].fvct[3] = 0.0f;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x88;
        pp->buf[pp->size].ul32[1] = 0x88;
        pp->buf[pp->size].ul32[2] = 0x88;
        pp->buf[pp->size].ul32[3] = 0x80;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x6C00;
        pp->buf[pp->size].ul32[1] = 0x7900;
        pp->buf[pp->size].ul32[2] = 0xFFFFF;
        pp->buf[pp->size].ul32[3] = 0;
        pp->size++;

        pp->buf[pp->size].fvct[0] = 1279.0f / 2048.0f; // 0x3F1FE000
        pp->buf[pp->size].fvct[1] = inter == 0 ? 448.0f / 512.0f : 449.0f / 512.0f; // 0x3F600000 : 0x3F608000
        pp->buf[pp->size].fvct[2] = 1.0f;
        pp->buf[pp->size].fvct[3] = 0.0f;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x88;
        pp->buf[pp->size].ul32[1] = 0x88;
        pp->buf[pp->size].ul32[2] = 0x88;
        pp->buf[pp->size].ul32[3] = 0x80;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x9400;
        pp->buf[pp->size].ul32[1] = 0x8700;
        pp->buf[pp->size].ul32[2] = 0xFFFFF;
        pp->buf[pp->size].ul32[3] = 0;
        pp->size++;

        pp->buf[pp->size].fvct[0] = 1279.0f / 2048.0f; // 0x3F1FE000
        pp->buf[pp->size].fvct[1] = inter == 0 ? 0.0f : 1.0f / 512.0f; // 0x000000 : 0x3B000000
        pp->buf[pp->size].fvct[2] = 1.0f;
        pp->buf[pp->size].fvct[3] = 0.0f;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x88;
        pp->buf[pp->size].ul32[1] = 0x88;
        pp->buf[pp->size].ul32[2] = 0x88;
        pp->buf[pp->size].ul32[3] = 0x80;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x9400;
        pp->buf[pp->size].ul32[1] = 0x7900;
        pp->buf[pp->size].ul32[2] = 0xFFFFF;
        pp->buf[pp->size].ul32[3] = 0;
        pp->size++;

        FlushCache(0);
        DmaChGIF->chcr.TTE = 1;
        sceDmaSend(DmaChGIF, (void *)(0x70000000 | 0x80000000));
        sceDmaSync(DmaChGIF, 0, 0);
    }

    {
        nkGifPacket packet;
        nkGifPacket *pp;

        pp = &packet;
        pp->size = 0;
        pp->buf = (nkQWdata *)0x70000000;
        pp->buf[pp->size].ul128 = 0;
        pp->buf[pp->size++].ul32[0] = 0x7000000D;
        pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 0, 0, 0), 0, 3);
        pp->buf[pp->size++].ul64[1] = SCE_GS_ST | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;

        pp->buf[pp->size].fvct[0] = -2.0f / 4096.0f; // 0xBA000000
        pp->buf[pp->size].fvct[1] = inter == 0 ? 447.0f / 512.0f : 448.0f / 512.0f; // 0x3F5F8000 : 0x3F600000
        pp->buf[pp->size].fvct[2] = 1.0f;
        pp->buf[pp->size].fvct[3] = 0.0f;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x88;
        pp->buf[pp->size].ul32[1] = 0x88;
        pp->buf[pp->size].ul32[2] = 0x88;
        pp->buf[pp->size].ul32[3] = 0x40;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x6C00;
        pp->buf[pp->size].ul32[1] = 0x8700;
        pp->buf[pp->size].ul32[2] = 0xFFFFF;
        pp->buf[pp->size].ul32[3] = 0;
        pp->size++;

        pp->buf[pp->size].fvct[0] = -2.0f / 4096.0f; // 0xBA000000
        pp->buf[pp->size].fvct[1] = inter == 0 ? -1.0f / 512.0f : 0.0f; // 0xBB000000 : 0x00000000
        pp->buf[pp->size].fvct[2] = 1.0f;
        pp->buf[pp->size].fvct[3] = 0.0f;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x88;
        pp->buf[pp->size].ul32[1] = 0x88;
        pp->buf[pp->size].ul32[2] = 0x88;
        pp->buf[pp->size].ul32[3] = 0x40;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x6C00;
        pp->buf[pp->size].ul32[1] = 0x7900;
        pp->buf[pp->size].ul32[2] = 0xFFFFF;
        pp->buf[pp->size].ul32[3] = 0;
        pp->size++;

        pp->buf[pp->size].fvct[0] = 1279.0f / 2048.0f; // 0x3F1FE000
        pp->buf[pp->size].fvct[1] = inter == 0 ? 447.0f / 512.0f : 448.0f / 512.0f; // 0x3F5F8000 : 0x3F600000
        pp->buf[pp->size].fvct[2] = 1.0f;
        pp->buf[pp->size].fvct[3] = 0.0f;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x88;
        pp->buf[pp->size].ul32[1] = 0x88;
        pp->buf[pp->size].ul32[2] = 0x88;
        pp->buf[pp->size].ul32[3] = 0x40;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x9400;
        pp->buf[pp->size].ul32[1] = 0x8700;
        pp->buf[pp->size].ul32[2] = 0xFFFFF;
        pp->buf[pp->size].ul32[3] = 0;
        pp->size++;

        pp->buf[pp->size].fvct[0] = 1279.0f / 2048.0f; // 0x3F1FE000
        pp->buf[pp->size].fvct[1] = inter == 0 ? -1.0f / 512.0f : 0.0f; // 0xBB000000 : 0x00000000
        pp->buf[pp->size].fvct[2] = 1.0f;
        pp->buf[pp->size].fvct[3] = 0.0f;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x88;
        pp->buf[pp->size].ul32[1] = 0x88;
        pp->buf[pp->size].ul32[2] = 0x88;
        pp->buf[pp->size].ul32[3] = 0x40;
        pp->size++;
        pp->buf[pp->size].ul32[0] = 0x9400;
        pp->buf[pp->size].ul32[1] = 0x7900;
        pp->buf[pp->size].ul32[2] = 0xFFFFF;
        pp->buf[pp->size].ul32[3] = 0;
        pp->size++;

        FlushCache(0);
        DmaChGIF->chcr.TTE = 1;
        sceDmaSend(DmaChGIF, (void *)(0x70000000 | 0x80000000));
        sceDmaSync(DmaChGIF, 0, 0);
    }

    FlushCache(0);
    sceDmaSendN(DmaChGIF, &nkScr.gif_draw, nkScr.size_draw + 1);
    sceDmaSync(DmaChGIF, 0, 0);
}

void nkGsInitFrm() {
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    sceGsSetDefClear(&GameGbl.db.clear0, 3, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    sceGsSetDefClear(&GameGbl.db.clear1, 3, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    FlushCache(WRITEBACK_DCACHE);
}

void nkGsInitFF() {
    s32 xsize = SCR_WIDTH;

    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    nkScr.dispenv.pmode.EN1 = 1;
    nkScr.dispenv.pmode.EN2 = 0;
    nkScr.dispenv.pmode.CRTMD = 1;
    nkScr.dispenv.pmode.MMOD = 1;
    nkScr.dispenv.pmode.AMOD = 0;
    nkScr.dispenv.pmode.SLBG = 1;
    nkScr.dispenv.pmode.ALP = 255;
    nkScr.dispenv.dispfb.FBP = 0x140;
    nkScr.dispenv.dispfb.FBW = xsize / 64;
    nkScr.dispenv.dispfb.PSM = 1;
    nkScr.dispenv.dispfb.DBX = 0;
    nkScr.dispenv.dispfb.DBY = 0;
    nkScr.dispenv.display.DX = 0x27C + (14 * (2560 / xsize));
    nkScr.dispenv.display.DY = 50 + 4;
    nkScr.dispenv.display.MAGH = (2560 + xsize - 1) / xsize - 1;
    nkScr.dispenv.display.MAGV = 0;
    nkScr.dispenv.display.DW = 0x9FF;
    nkScr.dispenv.display.DH = SCR_HEIGHT * 2 - 1;
    nkScr.dispenv.bgcolor.R = 0;
    nkScr.dispenv.bgcolor.G = 0;
    nkScr.dispenv.bgcolor.B = 0;

    nkScr.size_draw = sceGsSetDefDrawEnv(&nkScr.drawenv1, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT * 2, SCE_GS_ZGEQUAL, SCE_GS_PSMZ16S);
    nkScr.size_draw += sceGsSetDefDrawEnv2(&nkScr.drawenv2, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT * 2, SCE_GS_ZGEQUAL, SCE_GS_PSMZ16S);
    sceGsSetDefClear(&nkScr.gsclear, SCE_GS_ZGEQUAL, 0x6C0, 0x720, SCR_WIDTH, SCR_HEIGHT * 2, 0, 0, 0, 0, 0);
    nkScr.size_draw += 6;
    nkScr.drawenv1.frame1.FBP = 0x0000;
    nkScr.drawenv1.zbuf1.ZBP = 0x8C;
    nkScr.drawenv2.frame2.FBP = 0x0000;
    nkScr.drawenv2.zbuf2.ZBP = 0x8C;
    nkScr.gif_draw.NLOOP = nkScr.size_draw;
    nkScr.gif_draw.EOP = 1;
    nkScr.gif_draw.PRE = 0;
    nkScr.gif_draw.PRIM = 0;
    nkScr.gif_draw.FLG = 0;
    nkScr.gif_draw.NREG = 1;
    nkScr.gif_draw.REGS0 = SCE_GIF_PACKED_AD;

    nkScr.size_view = sceGsSetDefDrawEnv(&nkScr.viewenv1, SCE_GS_PSMCT24, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZNOUSE, SCE_GS_PSMCT32);
    nkScr.size_view += sceGsSetDefDrawEnv2(&nkScr.viewenv2, SCE_GS_PSMCT24, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZNOUSE, SCE_GS_PSMCT32);
    nkScr.viewenv1.frame1.FBP = 0x140;
    nkScr.viewenv1.zbuf1.ZBP = 0x140;
    nkScr.viewenv1.zbuf1.ZMSK = 1;
    nkScr.viewenv2.frame2.FBP = 0x140;
    nkScr.viewenv2.zbuf2.ZBP = 0x140;
    nkScr.viewenv2.zbuf2.ZMSK = 1;
    nkScr.gif_view.NLOOP = nkScr.size_view;
    nkScr.gif_view.EOP = 1;
    nkScr.gif_view.PRE = 0;
    nkScr.gif_view.PRIM = 0;
    nkScr.gif_view.FLG = 0;
    nkScr.gif_view.NREG = 1;
    nkScr.gif_view.REGS0 = SCE_GIF_PACKED_AD;

    DPUT_GS_PMODE(*(u64 *)&nkScr.dispenv.pmode);
    DPUT_GS_DISPFB2(*(u64 *)&nkScr.dispenv.dispfb);
    DPUT_GS_DISPFB1(*(u64 *)&nkScr.dispenv.dispfb);
    DPUT_GS_DISPLAY2(*(u64 *)&nkScr.dispenv.display);
    DPUT_GS_DISPLAY1(*(u64 *)&nkScr.dispenv.display);
    DPUT_GS_BGCOLOR(*(u64 *)&nkScr.dispenv.bgcolor);
    sceGsSyncV(0);
}

void nkGsSetDrawEnv() {
    nkGifPacket packet;

    packet.size = 0;
    packet.buf = (nkQWdata *)0x70000000;
    packet.buf[packet.size].ul128 = 0;
    packet.buf[packet.size++].ul32[0] = 0x7000000A;
    packet.buf[packet.size].ul64[0] = SCE_GIF_SET_TAG(9, 1, 0, 0, 0, 1);
    packet.buf[packet.size++].ul64[1] = SCE_GIF_PACKED_AD;

    packet.buf[packet.size].ul64[1] = SCE_GS_FRAME_1;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_FRAME(0, 10, SCE_GS_PSMCT32, 0);
    packet.buf[packet.size].ul64[1] = SCE_GS_XYOFFSET_1;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_XYOFFSET(0x6C00, 0x7900);
    packet.buf[packet.size].ul64[1] = SCE_GS_ZBUF_1;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_ZBUF(0xB4, SCE_GS_PSMZ16S, 0);

    packet.buf[packet.size].ul64[1] = SCE_GS_FRAME_2;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_FRAME(0, 10, SCE_GS_PSMCT32, 0);
    packet.buf[packet.size].ul64[1] = SCE_GS_XYOFFSET_2;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_XYOFFSET(0x6C00, 0x7900);
    packet.buf[packet.size].ul64[1] = SCE_GS_ZBUF_2;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_ZBUF(0xB4, SCE_GS_PSMZ16S, 0);

    packet.buf[packet.size].ul64[1] = SCE_GS_RGBAQ;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_RGBAQ(0, 0, 0, 0, 0);
    packet.buf[packet.size].ul64[1] = SCE_GS_XYZ3;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_XYZ(0x6C00, 0x7900, 0x0000);
    packet.buf[packet.size].ul64[1] = SCE_GS_XYZ2;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_XYZ(0x9400, 0x9500, 0x0000);

    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)packet.buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void nkGsSetEffectFZ2() {
    nkGifPacket packet;

    packet.size = 0;
    packet.buf = (nkQWdata *)0x70000000;
    packet.buf[packet.size].ul128 = 0;
    packet.buf[packet.size++].ul32[0] = 0x70000004;
    packet.buf[packet.size].ul64[0] = SCE_GIF_SET_TAG(3, 1, 0, 0, 0, 1);
    packet.buf[packet.size++].ul64[1] = SCE_GIF_PACKED_AD;

    packet.buf[packet.size].ul64[1] = SCE_GS_FRAME_2;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_FRAME(0xB4, 10, SCE_GS_PSMCT32, 0);
    packet.buf[packet.size].ul64[1] = SCE_GS_XYOFFSET_2;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_XYOFFSET(0x6C00, 0x7900);
    packet.buf[packet.size].ul64[1] = SCE_GS_ZBUF_2;
    packet.buf[packet.size++].ul64[0] = SCE_GS_SET_ZBUF(0x8C, SCE_GS_PSMZ16S, 0);

    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)packet.buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
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
    
    pk.buf[pk.size].ul128 = 0;
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

void _nkDrawTTriangle(sceVu0FVECTOR vf0, sceVu0FVECTOR vf1, sceVu0FVECTOR vf2, sceVu0FVECTOR posi, sceVu0FVECTOR rot, sceVu0IVECTOR rgba) {
    sceVu0FMATRIX lmat;
    sceVu0IVECTOR vi0;
    sceVu0IVECTOR vi1;
    sceVu0IVECTOR vi2;
    qword packet_buff[8];
    nkGifPacket pack;
    nkGifPacket *pp;

    pp = &pack;
    sceVu0UnitMatrix(lmat);
    sceVu0RotMatrixX(lmat, lmat, rot[0]);
    sceVu0RotMatrixY(lmat, lmat, rot[1]);
    sceVu0RotMatrixZ(lmat, lmat, rot[2]);
    sceVu0TransMatrix(lmat, lmat, posi);
    sceVu0MulMatrix(lmat, GameGbl.wsm, lmat);
    sceVu0RotTransPers(vi0, lmat, vf0, 0);
    sceVu0RotTransPers(vi1, lmat, vf1, 0);
    sceVu0RotTransPers(vi2, lmat, vf2, 0);
    sceVu0ITOF4Vector(vf0, vi0);
    sceVu0ITOF4Vector(vf1, vi1);
    sceVu0ITOF4Vector(vf2, vi2);

    if (!sceVu0ClipScreen3(vf0, vf1, vf2)) {
        pp->size = 0;
        pp->buf = (nkQWdata *)packet_buff;
        pp->buf[pp->size].ul128 = 0;
        pp->buf[pp->size++].ul32[0] = 0x70000007;
        pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(3, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRI, 1, 0, 0, 0, 0, 0, 0, 0), 0, 2);
        pp->buf[pp->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZF2 << 4;

        rgba[3] = 0xA0;
        pack.buf[2].ul32[0] = rgba[0];
        pack.buf[2].ul32[1] = rgba[1];
        pack.buf[2].ul32[2] = rgba[2];
        pack.buf[2].ul32[3] = rgba[3];
        pack.buf[3].ul32[0] = vi0[0];
        pack.buf[3].ul32[1] = vi0[1];
        pack.buf[3].ul32[2] = vi0[2];
        pack.buf[3].ul32[3] = vi0[3];
        pack.buf[4].ul32[0] = rgba[0];
        pack.buf[4].ul32[1] = rgba[1];
        pack.buf[4].ul32[2] = rgba[2];
        pack.buf[4].ul32[3] = rgba[3];
        pack.buf[5].ul32[0] = vi1[0];
        pack.buf[5].ul32[1] = vi1[1];
        pack.buf[5].ul32[2] = vi1[2];
        pack.buf[5].ul32[3] = vi1[3];
        pack.buf[6].ul32[0] = rgba[0];
        pack.buf[6].ul32[1] = rgba[1];
        pack.buf[6].ul32[2] = rgba[2];
        pack.buf[6].ul32[3] = rgba[3];
        pack.buf[7].ul32[0] = vi2[0];
        pack.buf[7].ul32[1] = vi2[1];
        pack.buf[7].ul32[2] = vi2[2];
        pack.buf[7].ul32[3] = vi2[3];

        FlushCache(WRITEBACK_DCACHE);
        sceDmaSend(DmaChGIF, pack.buf);
        sceDmaSync(DmaChGIF, 0, 0);
    }
}

void _nkDraw3DLine(sceVu0FVECTOR vf0, sceVu0FVECTOR vf1, u32 c0, u32 c1) {
    sceVu0IVECTOR vi0;
    sceVu0IVECTOR vi1;
    nkGifPacket pack;
    nkGifPacket *pp;

    pp = &pack;
    vf0[3] = 1.0f;
    vf1[3] = 1.0f;
    sceVu0RotTransPers(vi0, GameGbl.wsm, vf0, 1);
    sceVu0RotTransPers(vi1, GameGbl.wsm, vf1, 1);
    vi0[2] = 0xffff;
    vi1[2] = 0xffff;

    pp->size = 0;
    pp->buf = (nkQWdata *)0x70000000;
    pp->buf[pp->size].ul128     = 0;
    pp->buf[pp->size++].ul32[0] = 0x70000005;
    pp->buf[pp->size].ul64[0]   = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_LINE, 1, 0, 0, 0, 0, 0, 0, 0), 0, 2);
    pp->buf[pp->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 4;

    pp->buf[pp->size].ul32[0] = (c0 >> 0)  & 0xFF;
    pp->buf[pp->size].ul32[1] = (c0 >> 8)  & 0xFF;
    pp->buf[pp->size].ul32[2] = (c0 >> 16) & 0xFF;
    pp->buf[pp->size].ul32[3] = (c0 >> 24) & 0xFF;
    pp->size++;
    pp->buf[pp->size].ul32[0] = vi0[0];
    pp->buf[pp->size].ul32[1] = vi0[1];
    pp->buf[pp->size].ul32[2] = vi0[2];
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = (c1 >> 0)  & 0xFF;
    pp->buf[pp->size].ul32[1] = (c1 >> 8)  & 0xFF;
    pp->buf[pp->size].ul32[2] = (c1 >> 16) & 0xFF;
    pp->buf[pp->size].ul32[3] = (c1 >> 24) & 0xFF;
    pp->size++;
    pp->buf[pp->size].ul32[0] = vi1[0];
    pp->buf[pp->size].ul32[1] = vi1[1];
    pp->buf[pp->size].ul32[2] = vi1[2];
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}