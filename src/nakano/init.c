#include "common.h"

sceDmaChan *DmaChVIF0;
sceDmaChan *DmaChVIF1;
sceDmaChan *DmaChGIF;
sceDmaChan *DmaChfromSPR;
sceDmaChan *DmaChtoSPR;
nkQWdata *SPAD_ADDR = (nkQWdata *)0x70000000; // Scratchpad
OBJWORK ObjWorkBuff[128];

void nkInitDma() {
    sceDmaReset(1);
    FUN_00302df0(0);
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

void FUN_001979b0(int param_1) {
    SPAD_ADDR[0].ul32[1] = 0;
    SPAD_ADDR[0].ul32[2] = 0;
    SPAD_ADDR[0].ul32[3] = 0;
    SPAD_ADDR[0].ul32[0] = (int)SPAD_ADDR | 2;
    SPAD_ADDR[1].ul64[0] = 0x1000000000008001;
    SPAD_ADDR[1].ul64[1] = SCE_GIF_PACKED_AD;
    SPAD_ADDR[2].ul64[1] = SCE_GS_FBA_1;
    SPAD_ADDR[2].ul64[0] = param_1;
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)0xF0000000);
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

int FUN_00302e58() {
    return (*GIF_STAT & 4) >> 2;
}

void FUN_00302df0(int param_1) {
    if (param_1 == 0) {
        *GIF_MODE = FUN_00302e58() & 0xfffffffb;
    } else {
        *GIF_MODE = FUN_00302e58() | 4;
    }
}

