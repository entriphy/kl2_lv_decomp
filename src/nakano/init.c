#include "common.h"

sceDmaChan *DmaChVIF0;
sceDmaChan *DmaChVIF1;
sceDmaChan *DmaChGIF;
sceDmaChan *DmaChfromSPR;
sceDmaChan *DmaChtoSPR;
OBJWORK ObjWorkBuff[128];

void init_config_system() {
    sceDevVif0Reset();
    sceDevVu0Reset();
    sceGsResetPath();
    sceDmaReset(1);
    SysGbl.nmode = 0;
    SysGbl.nsmode = 0;
    SysGbl.fmode = 0;
    SysGbl.smode = 0;
    GameGbl.kpd[0].cnt = 0;
    GameGbl.kloZanki[0] = 3;
    GameGbl.vs_clear_cnt = 160;
    GameGbl.time_atack_best = 80;
    nkGsInitFrm();
    SysGbl.Language = 1;
    SysGbl.TVSystem = 2;
}

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
    sceGsResetPath();
    sceDevGifReset();
    sceDevVif1Reset();
    sceDevVu1Reset();
    sceDevVif0Reset();
    sceDevVu0Reset();
    nkInitDma();
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    FUN_00197a88();
}

void FUN_001979b0(int fba) {
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

void FUN_00197a88() {
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ16S, SCE_GS_CLEAR);
    GameGbl.db.clear0.rgbaq.A = 128;
    GameGbl.db.clear1.rgbaq.A = 128;
    FUN_001979b0(1);
    sceGsSetDefClear(&GameGbl.db.clear0, SCE_GS_ZGREATER, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    sceGsSetDefClear(&GameGbl.db.clear1, SCE_GS_ZGREATER, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    FlushCache(WRITEBACK_DCACHE);
}

void nkInitSys() {
    // TODO
    nkDG.capture_num = 0;
    nkDG.map_draw_flag = 0;
    nkDG.opflag = 0;
    hrpt_deb = 0;
}

void nkStageInit0() {
    if ((GameGbl.vision >> 8) - 15 > 5) {
        u8 *gms = (u8 *)hGetDataAddr(0);
        if (gms != NULL) {
            FUN_0018dcb0(gms + ((s32 *)gms)[1]);
            sceGsSyncPath(0, 0);
        }
    }
}

void nkStageInit1() {
    // TODO
}
