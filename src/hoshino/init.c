#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "common.h"

hCDDATA CdData;
u8 EeCb[0x2000];
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
KLTABLE* KlTable;
PPTTABLE* PptTable;
BGMTABLE* BgmTable;
hSNDDATA* sD;
hBGMDATA* bD;
hPPTDATA* pD;
hAC3DATA* aD;
u8 pptEeAddrs[4][0x40000];
void* areaBuff;
hMOVDATA* mD;
int RpcArg[16];
SifDmaData sifdma_004171c0;
int RpcRecvBuf[2][16];
int SndMainBuffer[16];
SifClientData sndRpc;
int boot_flag; // ?

void* hReadFile(const char* name) {
    sceCdlFILE file;
    int ret;

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&file, name));
    void* buf = getBuff(1, roundSizeToSector(file.size), NULL, &ret);

    while (!sceCdRead(file.lsn, file.size >> 0xB, buf, &cD->mode));
    do {
        for (int i = 0; i < 0x100000; i++) {
            // Do nothing
        }
    } while (sceCdSync(1));

    return buf;
}

int FUN_00166128(int file) {
    return KlTable[file].count;
}

void hCdInit() {
    cD = &CdData;
    cQ = &cD->Cue;

    cQ->Num = 0;
    cQ->Exe = 0;
    cQ->Reg = 0;

    cD->eeCnt = 0;
    cD->dataFlag = CDREAD_IDLE;
    cD->BGMplay = 0;
    cD->mode.trycount = 0;
    cD->mode.spindlctrl = 0;
    cD->mode.datapattern = 0;
    cD->mode.pad = 0;
    
    sceCdInitEeCB(0, EeCb, 0x2000);
    int threadId = GetThreadId();
    cD->ThID = threadId;
    ChangeThreadPriority(threadId, 1);

    sceCdDiskReady(0);
    cD->DiscType = sceCdGetDiskType();
    void* buf = hReadFile("\\HEADPACK.BIN;1");
    void* addr = GetFHMAddress(buf, 0);
    KlTable = (KLTABLE*)((char*)addr + 4);
    addr = GetFHMAddress(buf, 1);
    PptTable = (PPTTABLE*)((char*)addr + 4);
    addr = GetFHMAddress(buf, 2);
    BgmTable = (BGMTABLE*)((char*)addr + 4);

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&cD->file, "\\KLDATA.BIN;1"));
}

void FUN_00167c20(void *param_1) {
    areaBuff = param_1;
    FUN_00166140(199, param_1);
}

int FUN_00167bd0(int param_1) {
    return FUN_00166128((GameGbl.vision >> 7 & 0x1FE) + param_1) << 0xB;
}

void hSeLock(int i) {
    sD->seLock = i;
}

void hSeInitGrp(int stage) {
    // TODO
}

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

    hIopDispatch(IOP_IopInit);
    init_config_system();
    hCdInit();
    hMovInit();
    hSndInit();
    
    GameGbl.vision = 0x6300;
    int ret;
    FUN_0016d710();
    FUN_00167c20(getBuff(1, FUN_00167bd0(1), NULL, &ret));
    FUN_001d31a0();
    htInitRand(0x399);
}

int FUN_0016c778() {
#ifdef SCE
    return SifCheckStatRpc(&sndRpc.rpcd);
#else
    return SifCheckStatRpc(&sndRpc);
#endif
}

void FUN_0016c798() {
    do {
        if (SifBindRpc(&sndRpc, 0x12346, 0) < 0) {
            while (true);
        }
        for (int i = 10000; i > 0; i--) {
            // Do nothing
        }
    }
#ifdef SCE
    while (!sndRpc.serve);
#else
    while (!sndRpc.server);
#endif
}

// TODO: Fix this
int* hIopDispatch(u32 param) {
    // 0x08000000: IopInit(),        r = 64, s = 0
    // 0x08000001: return RpcInfo,   r = 64, s = 0
    // 0x10000003: StrKick(),        r = 0,  s = 0
    // 0x14000000: StrInit(),        r = 16, s = 0
    // 0x1b000001: StrInfo(data),    r = 64, s = 128
    // 0x20000004: JamBdTrans(),     r = 0,  s = 0
    // 0x21000003: JamBankSet(data), r = 0,  s = 16
    // 0x22000002: SndMask(data),    r = 0,  s = 64
    // 0x24000000: SndInit(),        r = 16, s = 0
    // 0x2a000001: SndMain(data),    r = 64, s = 64

    int rsize;
    switch (param & 0xc000000) {
        case 0x4000000:
            rsize = 16;
            break;
        case 0x8000000:
            rsize = 64;
            break;
        case 0xc000000:
            rsize = 128;
            break;
        default:
            rsize = 0;
            break;
    }

    int ssize;
    switch (param & 0x3000000) {
        case 0x1000000:
            ssize = 16;
            break;
        case 0x2000000:
            ssize = 64;
            break;
        case 0x3000000:
            ssize = 128;
            break;
        default:
            ssize = 0;
            break;
    }

    int mode = 0;
    int* send = RpcArg;
    int* receive = RpcRecvBuf[0];
    if (param == 0x2a000001) {
        send = SndMainBuffer;
        ssize = hSndPkGetSize();
        receive = RpcRecvBuf[1];
    } else {
        mode = SIF_RPC_M_NOWAIT;
    }

    SifCallRpc(&sndRpc, param, mode, send, ssize, receive, rsize, 0, 0);
    if ((param & 0xc000000) == 0x4000000) {
        // TODO
    }
    return receive;
}

s32 FUN_0016c9b8(void *dest, void *src, u32 size) {
    u32 id;

#ifdef SCE
    sifdma_004171c0.data = (int)src;
    sifdma_004171c0.addr = (int)dest;
    sifdma_004171c0.size = size;
    sifdma_004171c0.mode = 0;
#else
    sifdma_004171c0.src = src;
    sifdma_004171c0.dest = dest;
    sifdma_004171c0.size = size;
    sifdma_004171c0.attr = 0;
#endif

    FlushCache(0);
    id = SifSetDma(&sifdma_004171c0, 1);
    if (id != 0) {
        while (SifDmaStat(id) >= 0);
        return 0;
    } else {
        return -1;
    }
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

void hBgmWorkClear() {
    bD->nextNo = -1;
    bD->bgmCh = 0;
    bD->bgmChMax = 2;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
    bD->iopID = 0;
    bD->cdReq = 0;
    bD->fadeFlag = 0;
    bD->bgmVol = 0.0;
    bD->bgmMute = 0;
    cD->BGMplay = 0;
}

void hStrInit() {
    bD = &BgmData;
    pD = &PptData;
    aD = &Ac3Data;
    hBgmWorkClear();
    bD->Command = 0;
    bD->bgmVol = 0.78740156f;
    int* iopAddr = hIopDispatch(IOP_StrInit);
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
    // while (FUN_0016c778());
    FUN_0016c6e8();

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
    FUN_0030dad0();
    sD = &SndData;
    sD->PkNum = (u8*)&SndMainBuffer[2];
    sD->PkMax = 0;
    sD->iopBankAddr = hIopDispatch(IOP_SndInit);
    RpcArg[0] = 0xFFFFFF;
    RpcArg[1] = 0xFFFF;
    hIopDispatch(IOP_SndMask);
    
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

int* FUN_0016d5f0(u8 *param_1, int param_2) {
    // don't feel like doing this rn lol
    return NULL;
}

void FUN_0016d710() {
    int ret;

    void *buf = getBuff(1, 0x200000, NULL, &ret);
    FUN_00166140(0xC6, buf);
    buf = GetFHMAddress(buf, 2);
    FUN_0016d5f0(buf, 0);
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

int FUN_001d1c78(const char* name, void* buf) {
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

void* getBuff(int type, int byte_, const char* name, int* ret) {
    if (type == 0) {
        byte_ = FUN_001d1c08(name);
        *ret = -1;
        if (byte_ == -1) {
            return NULL;
        }
        *ret = FUN_001d1c78(name, buffstartptr);
    }
    void* ptr = buffstartptr;
    buffstartptr = (void*)((char*)buffstartptr + byte_);
    sce_print("@@@ Old buff: %08x, new buff: %08x (%d)\n", ptr, buffstartptr, byte_);
    return ptr;
}

void FUN_001d37f8(int param_1, int param_2, const char *param_3) {
    void *buff = buffstartptr;
    
    if (param_1 == 0) {
        param_2 = FUN_001d1c08(param_3);
        if (param_2 == -1)
            return;
    }

    param_2 = ((param_2 + 0xF) << 4) >> 4;
    buff -= param_2;
    
    buffstartptr = buff;
}

void hMovInit() {
    mD = &MovData;
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&mD->file, "\\SR.SMV;1"));
}

int FUN_0030dad0() {
    SifInitRpc(0);
    do {
        if (SifBindRpc(&rpc__003d9718, 0x80000701, 0) < 0) {
            sce_print("error \n");
            while (true);
        }
        for (int i = 10000; i > 0; i--) {
            // Do nothing
        }
#ifdef SCE
    } while (!rpc__003d9718.serve);
#else
    } while (!rpc__003d9718.server);
#endif
    FlushCache(0);
    return 1;
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
