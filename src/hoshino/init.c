#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "common.h"

hCDDATA CdData;
u8 EeCb[0x2000];
hGAMEDATA GameData;
const char* modules[8] = {
    "cdrom:\\M\\SIFCMD.IRX;1",
    "cdrom:\\M\\SIO2MAN.IRX;1",
    "cdrom:\\M\\MCMAN.IRX;1",
    "cdrom:\\M\\MCSERV.IRX;1",
    "cdrom:\\M\\PADMAN.IRX;1",
    "cdrom:\\M\\LIBSD.IRX;1",
    "cdrom:\\M\\SDRDRV.IRX;1",
    "cdrom:\\M\\KL2.IRX;1"
};
hSNDDATA SndData;
hBGMDATA BgmData;
hPPTDATA PptData;
hAC3DATA Ac3Data;
hMOVDATA MovData;
sceSifClientData rpc__003d9718;
hCDDATA* cD;
hCDCUE* cQ;
hGAMEDATA *gD;
KLTABLE* KlTable;
PPTTABLE* PptTable;
BGMTABLE* BgmTable;
hSNDDATA* sD;
hBGMDATA* bD;
hPPTDATA* pD;
hAC3DATA* aD;
u8 pptEeAddrs[4][0x40000] __attribute__((aligned(16)));
u8 *tblMax;
u8 *areaBuff;
hMOVDATA* mD;
int RpcArg[16] __attribute__((aligned(16)));
sceSifDmaData sifdma_004171c0;
int RpcRecvBuf[2][16] __attribute__((aligned(16)));
u8 SndMainBuffer[0x400] __attribute__((aligned(16)));
sceSifClientData sndRpc;
int boot_flag; // ?

void hInitBoot() {
    u32 i;
    s32 ret;

    sceSifInitRpc(0);
    while (!sceCdInit(SCECdINIT));
    while (!sceSifRebootIop("host0:"IOP_IMAGE_file));
    while (!sceSifSyncIop());
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sceSifLoadFileReset();
    while (!sceCdInit(SCECdINIT));
    while (!sceCdMmode(SCECdDVD));
    for (i = 0; i < 8; i++) {
        while ((ret = sceSifLoadModule(modules[i], 0, NULL)) < 0) {
            while (!sceCdInit(0));
            sceCdDiskReady(0);
            while (!sceCdMmode(SCECdDVD));
            sceFsReset();
        }
    }

    hRpcBind();
    hRpc(IOP_IopInit);
    init_config_system();
    hCdInit();
    hMovInit();
    hSndInit();
    
    GameGbl.vision = 0x6300;
    hSndBankSetCommon();
    FUN_00167c20(getBuff(1, FUN_00167bd0(1), NULL, &ret));
    memoryStageptrSet();
    htInitRand(0x399);
}

void FUN_00196c00() {
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    sceGsSetDefClear(&GameGbl.db.clear0, 3, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    sceGsSetDefClear(&GameGbl.db.clear1, 3, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    FlushCache(WRITEBACK_DCACHE);
}

void hStrInit() {
    int i, j;
    int *iopAddr;
    int *n;

    bD = &BgmData;
    pD = &PptData;
    aD = &Ac3Data;
    hBgmWorkClear();
    bD->Command = 0;
    bD->bgmVol = 0.78740156f;
    iopAddr = hRpc(IOP_StrInit);
    bD->iopAddr[0] = iopAddr;
    bD->iopAddr[1] = iopAddr + 0x20000;
    n = bD->iopAddr[1] + 0x20000;
    for (i = 0; i < 4; i++) {
        pD->pptPlay[i] = 0;
        pD->eeStat[i] = 0;
        pD->eeAddr[i] = pptEeAddrs[i];
        for (j = 0; j < 2; j++) {
            pD->iopAddr[i][j] = n;
            n += 0x1000;
        }
    }
    aD->field_0xB4 = n;
    aD->field_0xB8 = n + 0x3000;
    hStrInfo();
    while (hRpcStat());
    hStr_0016c6e8();

    bD->iopID = bD->iopNext;
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&bD->file, "\\BGMPACK.BIN;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&pD->file, "\\PPTPACK.BIN;1"));

    aD->field_0x90 = -1;
    aD->field_0x80 = 0;
    aD->field_0x84 = 0;
    aD->field_0x88 = 0;
    aD->field_0xA8 = 0;
    aD->field_0x9C = 0;
    aD->field_0xD0 = pptEeAddrs[0];
    aD->field_0xD4 = pptEeAddrs[2];

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[0], "\\BGM000.AC3;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[1], "\\BGM001.AC3;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[2], "\\BGM002.AC3;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[3], "\\BGM003.AC3;1"));
}

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
    FUN_00196c00();
    SysGbl.Language = 1;
    SysGbl.TVSystem = 2;
}

void hMovInit() {
    mD = &MovData;
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&mD->file, "\\SR.SMV;1"));
}

void FUN_00318a80() {
    if (!boot_flag) {
        boot_flag = 1;
        FUN_003189d0();
    }
}

void FUN_003189d0() {
    // TODO, seems like it calls a few constructors?
}
