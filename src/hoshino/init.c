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
SifClientData rpc__003d9718;
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
SifDmaData sifdma_004171c0;
int RpcRecvBuf[2][16] __attribute__((aligned(16)));
int SndMainBuffer[16] __attribute__((aligned(16)));
SifClientData sndRpc;
int boot_flag; // ?

void hInitBoot() {
    SifInitRpc(0);
    while (!sceCdInit(SCECdINIT));
#ifdef SCE
    while (!SifIopReboot("host0:ioprp300.img"));
#else
    while (!SifIopReboot("cdrom:\\M\\IOPRP213.IMG;1"));
#endif
    while (!SifIopSync());
    SifInitRpc(0);
    SifInitIopHeap();
    SifLoadFileInit();
    while (!sceCdInit(SCECdINIT));
    while (!sceCdMmode(SCECdMmodeDvd));
#ifdef SCE
    sceFsReset();
#endif
    for (int i = 0; i < 8; i++) {
        int id = SifLoadModule(modules[i], 0, 0);
        if (id < -1) {
            while (!sceCdInit(SCECdINIT));
            sceCdDiskReady(0);
            while (!sceCdMmode(SCECdMmodeDvd));
#ifdef SCE
            sceFsReset();
#endif
        }
        sce_print("@ Loading module %s: %d\n", modules[i], id);
    }

    hRpcBind();
    hRpc(IOP_IopInit);
    init_config_system();
    hCdInit();
    hMovInit();
    hSndInit();
    
    GameGbl.vision = 0x6300;
    int ret;
    hSnd_0016d710();
    FUN_00167c20(getBuff(1, FUN_00167bd0(1), NULL, &ret));
    FUN_001d31a0();
    htInitRand(0x399);
}

void FUN_00196c00() {
    GsResetGraph(GS_INIT_RESET, GS_INTERLACED, GS_MODE_NTSC, GS_FFMD_FRAME);
#ifdef SCE
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    sceGsSetDefClear(&GameGbl.db.clear0, 3, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
    sceGsSetDefClear(&GameGbl.db.clear1, 3, 1728, 1936, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 128, 0);
#else
    // How do you do this in ps2sdk?
#endif
    FlushCache(WRITEBACK_DCACHE);
}

void hStrInit() {
    bD = &BgmData;
    pD = &PptData;
    aD = &Ac3Data;
    hBgmWorkClear();
    bD->Command = 0;
    bD->bgmVol = 0.78740156f;
    int* iopAddr = hRpc(IOP_StrInit);
    bD->iopAddr[0] = iopAddr;
    bD->iopAddr[1] = iopAddr + 0x20000;
    int* n = bD->iopAddr[1] + 0x20000;
    for (int i = 0; i < 4; i++) {
        pD->pptPlay[i] = 0;
        pD->eeStat[i] = 0;
        pD->eeAddr[i] = pptEeAddrs[i];
        for (int j = 0; j < 2; j++) {
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

void hSndInit() {
    sceSdRemoteInit();
    sD = &SndData;
    sD->PkNum = (u8*)&SndMainBuffer[2];
    sD->PkMax = 0;
    sD->iopBankAddr = hRpc(IOP_SndInit);
    RpcArg[0] = 0xFFFFFF;
    RpcArg[1] = 0xFFFF;
    hRpc(IOP_SndMask);
    
    sD->pad = 0;
    sD->Stereo = SND_MODE_STEREO;
    sD->Mute = 0;
    sD->stageBank = 0;
    sD->dBfader = -30.0f;
    sD->log10Volume = log10f(16367.0f) * 20.0f;
    hSndSetMVol(0.0f);
    hSndPkSetMVol(0, 0);
    sD->effMode = 0;
    sD->effDepth = 0;
    sD->effDelay = 0;
    sD->effFeed = 0;
    sD->effMix = 3;
    hSndPkEffect();
    sD->effVolBak = 0.0f;
    sD->effVol = 0.0f;
    hSndPkSetEVol(0);
    sD->envNum = 0;
    hSeLock(0);
    hSeInitGrp(0);
    sD->seMVol = 1.43f;
    sD->bgmMVol = 0.708661437f;
    sD->pptMVol = 0.314960629f;
    sD->TitleDelayCnt = 0;

    hStrInit();
}

s32 *hSnd_0016d5f0(u8 *param_1, int param_2) {
    u8 *hdaddr;
    u8 *bdaddr;
    s32 hdsize;
    s32 bdsize;
    s32 *rpc;
    u8 stackShiz[16];
    
    if (param_2 != 0) {
        s16 *fhm = (s16 *)GetFHMAddress(param_1, 2);
        s16 tblNum = *fhm;
        sD->stageTblNum = tblNum;
        memcpy(sD->stageTbl, fhm + 1, tblNum * 2);
    }

    hdaddr = GetFHMAddress(param_1, 0);
    bdaddr = GetFHMAddress(param_1, 1);
    hdsize = JamGetHdSize(hdaddr);
    bdsize = JamGetBdSize(hdaddr);
    hRpc_0016c9b8((u8 *)(sD->iopBankAddr + param_2 * 0x2000), hdaddr, hdsize); // addr, data, size
    RpcArg[0] = param_2;
    rpc = hRpc(IOP_JamBankSet);
    
    while (bdsize > 0) {
        s32 transsize;
        if (bdsize < 0x10000) {
            transsize = bdsize;
        } else {
            transsize = 0x10000;
        }
        
        hRpc_0016c9b8((u8 *)(sD->iopBankAddr + 0x4000), bdaddr, transsize);
        bdsize -= 0x10000;
        bdaddr += 0x10000;
        rpc = hRpc(IOP_JamBdTrans);
    }
    
    return rpc;
}

void hSnd_0016d710() {
    int ret;

    u8 *buf = getBuff(1, 0x200000, NULL, &ret);
    hCdReadKlPack(0xC6, buf);
    buf = GetFHMAddress(buf, 2);
    hSnd_0016d5f0(buf, 0);
    FUN_001d37f8(1, 0x200000, NULL);
}

void init_config_system() {
    DevVif0Reset();
    DevVu0Reset();
    GsResetPath();
#ifdef SCE
    sceDmaReset(1);
#else
    dma_reset();
#endif
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

int FUN_001d1c08(const char* name) {
    // Using open/lseek rather than sceOpen/sceLseek
    int ret;
    FILE* file = fopen(name, "r");

    if (file == NULL) {
        fclose(file); // fclose on a null pointer? Is that allowed with sceClose?
        ret = -1;
    } else {
        ret = fseek(file, 0, SEEK_END);
        fclose(file);
    }

    return ret;
}

int FUN_001d1c78(const char* name, u8 *buf) {
    FILE* file = fopen(name, "r");
    if (file != NULL) {
        int size = fseek(file, 0, SEEK_END);
        if (size > 0) {
            fseek(file, 0, SEEK_SET);
            read(file->_file, buf, size);
            fclose(file);
            return size;
        }
    }

    fclose(file);
    return -1;
}

void FUN_001d31a0() {
    buffstagetop = buffstartptr;
}

u8 *getBuff(s32 type, s32 size, const char *name, s32 *ret) {
    u8 *buff = buffstartptr;
    u8 *newBuff = buffstartptr;
    
    if (type == 0) {
        size = FUN_001d1c08(name); // Gets filesize
        *ret = -1;
        if (size == -1)
            return (u8 *)-1;
        *ret = FUN_001d1c78(name, newBuff);
    }
    
    size = ((size + 0x0F) / 0x10) * 0x10;
    newBuff += size;
    buffstartptr = newBuff;
    return buff;
}

void FUN_001d37f8(s32 type, s32 size, const char *name) {
    u8 *buff = buffstartptr;
    
    if (type == 0) {
        size = FUN_001d1c08(name);
        if (size == -1)
            return;
    }

    size = ((size + 0xF) / 16) * 16;
    buff -= size;
    
    buffstartptr = buff;
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
