#include "common.h"

sceDmaChan *DmaChVIF0;
sceDmaChan *DmaChVIF1;
sceDmaChan *DmaChGIF;
sceDmaChan *DmaChfromSPR;
sceDmaChan *DmaChtoSPR;
OBJWORK ObjWorkBuff[128];

void nkInitDma() {
    sceDmaReset(1);
    sceDevGifPutImtMode(0);
    DmaChVIF0 = sceDmaGetChan(0);
    DmaChVIF0->chcr = (tD_CHCR){ .MOD = 1, .TTE = 1 };
    DmaChVIF1 = sceDmaGetChan(1);
    DmaChVIF1->chcr = (tD_CHCR){ .MOD = 1, .TTE = 1 };
    DmaChGIF = sceDmaGetChan(2);
    DmaChGIF->chcr = (tD_CHCR){ .MOD = 1 };
    DmaChfromSPR = sceDmaGetChan(8);
    DmaChfromSPR->chcr = (tD_CHCR){ };
    DmaChtoSPR = sceDmaGetChan(9);
    DmaChtoSPR->chcr = (tD_CHCR){ };
}

void nkInitPS2() {
    SysGbl.objwork = ObjWorkBuff;
    SysGbl.n_objw = 0x80;
    nkDG.cam_debug_mode = 0;
    nkDG.flag = 0;
    GsResetPath();
    sceDevGifReset();
    DevVif1Reset();
    DevVu1Reset();
    DevVif0Reset();
    DevVu0Reset();
    nkInitDma();
    GsResetGraph(GS_INIT_RESET, GS_INTERLACED, GS_MODE_NTSC, GS_FFMD_FRAME);
    FUN_00197a88();
}

void FUN_001979b0(int fba) {
    nkGifPacket pk;
    
    pk.size = 0;
    pk.buf = (nkQWdata *)0x70000000;
    
    SCE_GIF_CLEAR_TAG(pk.buf);
    pk.buf[pk.size++].ul32[0] = 0x70000000 | 0x2;
    pk.buf[pk.size].ul64[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pk.buf[pk.size++].ul64[1] = SCE_GIF_PACKED_AD;
    pk.buf[pk.size].ul64[1] = SCE_GS_FBA_1;
    pk.buf[pk.size++].ul64[0] = fba;
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pk.buf | 0x80000000));
}

void FUN_00197a88() {
    GsResetGraph(GS_INIT_RESET, GS_INTERLACED, GS_MODE_NTSC, GS_FFMD_FRAME);
#ifdef SCE
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ16S, SCE_GS_CLEAR);
    GameGbl.db.clear0.rgbaq.A = 128;
    GameGbl.db.clear1.rgbaq.A = 128;
#endif
    FUN_001979b0(1);
#ifdef SCE
    sceGsSetDefClear(&GameGbl.db.clear0, SCE_GS_ZGREATER, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    sceGsSetDefClear(&GameGbl.db.clear1, SCE_GS_ZGREATER, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
#endif
    FlushCache(WRITEBACK_DCACHE);
}

void nkInitSys() {
    // TODO
    nkDG.capture_num = 0;
    nkDG.map_draw_flag = 0;
    nkDG.opflag = 0;
    hrpt_deb = 0;
}
