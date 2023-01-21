#include <dma.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include <kernel.h>
#include <libcdvd.h>
#include <loadfile.h>
#include <math.h>
#include <sifrpc.h>
#include <sifdma.h>
#include <sifcmd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vif_registers.h>

#include "hoshino.h"
#include "common.h"
#include "gbl.h"
#include "ps2.h"

void* hReadFile(const char* name) {
    sceCdlFILE file;
    int ret;

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&file, name));
    void* buf = getBuff(1, roundSizeToSector(file.size), nullptr, &ret);

    while (!sceCdRead(file.lsn, file.size >> 0xB, buf, &cD->mode));
    do {
        for (int i = 0; i < 0x100000; i++) {
            // Do nothing
        }
    } while (sceCdSync(1));

    return buf;
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
    kldataHead = (int*)((char*)addr + 4);
    addr = GetFHMAddress(buf, 1);
    pptpackHead = (int*)((char*)addr + 4);
    addr = GetFHMAddress(buf, 2);
    bgmpackHead = (int*)((char*)addr + 4);

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&cD->file, "\\KLDATA.BIN;1"));
    sce_print("%08x %08x %08x", kldataHead, pptpackHead, bgmpackHead);
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
    while (!SifIopReboot("cdrom:\\M\\IOPRP213.IMG;1"));
    while (!SifIopSync());
    SifInitRpc(0);
    SifInitIopHeap();
    SifLoadFileInit();
    while (!sceCdInit(SCECdINIT));
    while (!sceCdMmode(SCECdMmodeDvd));
    for (int i = 0; i < 8; i++) {
        int id = SifLoadModule(modules[i], 0, 0);
        if (id < -1) {
            while (!sceCdInit(SCECdINIT));
            sceCdDiskReady(0);
            while (!sceCdMmode(SCECdMmodeDvd));
            // sceFsReset?
        }
        sce_print("@ %s: %d\n", modules[i], id);
    }

    hIopDispatch(0x8000000); // Call IopInit
    init_config_system();
    hCdInit();
    hStrInit();
    hSndInit();
    
    GameGbl.vision = 0x6300;
}

int FUN_0016c778() {
    return SifCheckStatRpc(&sndRpc);
}

void FUN_0016c798() {
    do {
        if (SifBindRpc(&sndRpc, 0x12346, 0) < 0) {
            while (true);
        }
        for (int i = 10000; i > 0; i--) {
            // Do nothing
        }
    } while (!sndRpc.server);
}

void hSndSetMVol(float vol) {
    sD->MVol = hSndFader(vol);
}

int hSndFader(float vol) {
    if (vol == 0.0f) {
        return 0;
    }
    if (vol > 1.0f) {
        return 1.0;
    }

    int n = (int)(powf(10.0f, ((1.0f - vol) * sD->dBfader + sD->log10Volume) / 20.0f));
    if (n > 0x3FFF) {
        n = 0x3FFF;
    }
    if (n < -0x4000) {
        n = -0x4000;
    }

    return n;
}

void hSndPkSetMVol(int voll, int volr) {
    sD->PkMax++;
    *sD->PkNum++ = 7;
    *sD->PkNum++ = (u8)voll;
    *sD->PkNum++ = (u8)(((u32)voll >> 8) & 0x7F);
    *sD->PkNum++ = (u8)volr;
    *sD->PkNum++ = (u8)(((u32)volr >> 8) & 0x7F);
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

void hSndPkEffect() {
    sD->effChange = 1;
    for (int i = 0; i < 2; i++) {
        sD->PkMax++;
        *sD->PkNum++ = 8;
        *sD->PkNum++ = (u8)i;
        *sD->PkNum++ = (u8)sD->effMode;
        *sD->PkNum++ = (u8)sD->effDepth;
        *sD->PkNum++ = (u8)((u32)sD->effDepth >> 8);
        *sD->PkNum++ = (u8)sD->effDepth;
        *sD->PkNum++ = (u8)((u32)sD->effDepth >> 8);
        *sD->PkNum++ = (u8)sD->effDelay;
        *sD->PkNum++ = (u8)sD->effFeed;
    }
}

void hSndPkSetEVol(int vol) {
    sD->PkMax++;
    *sD->PkNum++ = 9;
    *sD->PkNum++ = (u8)vol;
    *sD->PkNum++ = (u8)((u32)vol >> 8);
}

void FUN_00196c00() {
    GsResetGraph(GS_INIT_RESET, GS_INTERLACED, GS_MODE_NTSC, GS_FFMD_FRAME);
    // TODO: Setup display environment
    FlushCache(WRITEBACK_DCACHE);
}

void hBgmWorkClear() {
    bD->nextNo = (BGM)~BGM000;
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

void hSndInit() {
    FUN_0030dad0();
    sD = &SndData;
    sD->PkNum = (u8*)&SndMainBuffer[2];
    sD->PkMax = 0;
    sD->iopBankAddr = hIopDispatch(0x24000000);
    RpcArg[0] = 0xFFFFFF;
    RpcArg[1] = 0xFFFF;
    hIopDispatch(0x22000002);
    
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
    bD = &BgmData;
    pD = &PptData;
    aD = &Ac3Data;
    hBgmWorkClear();
    bD->Command = 0;
    bD->bgmVol = 0.78740156f;
    int* iopAddr = hIopDispatch(0x14000000);
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
    // TODO: hStrInfo();
    while (FUN_0016c778());

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
    DevVif0Reset();
    DevVu0Reset();
    GsResetPath();
    dma_reset();
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

    if (file == nullptr) {
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
    if (file != nullptr) {
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

void* getBuff(int type, int byte_, const char* name, int* ret) {
    if (type == 0) {
        byte_ = FUN_001d1c08(name);
        *ret = -1;
        if (byte_ == -1) {
            return nullptr;
        }
        *ret = FUN_001d1c78(name, buffstartptr);
    }
    void* ptr = buffstartptr;
    buffstartptr = (void*)((char*)buffstartptr + byte_);
    sce_print("%08x %08x %d\n", ptr, buffstartptr, byte_);
    return ptr;
}

void hStrInit() {
    strD = &StrData;
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&strD->file, "\\SR.SMV;1"));
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
    } while (!rpc__003d9718.server);
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