#include <libcdvd.h>
#include "common.h"

void hCdReadFileToBuf(char *name, void *buf) {
    sceCdlFILE file;
    s32 ret;
    s32 sectors;
    
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&file, name));
    sectors = ((file.size + 0x7FF) / 0x800);
    if (sectors > 0x400) {
        sectors = 0x400;
    }
    while (!sceCdRead(file.lsn, sectors, buf, &cD->mode));
    do {
        s32 i;
        for (i = 0; i < 0x100000; i++);
    } while (sceCdSync(1));
}

u8 * hCdReadFile(const char *name) {
    sceCdlFILE file;
    u8 *buf;
    s32 ret;
    s32 sectors;
    
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&file, name));
    sectors = ((file.size + 0x7FF) / 0x800);
    buf = (u8 *)getBuff(1, sectors * 0x800, NULL, &ret);
    while (!sceCdRead(file.lsn, sectors, buf, &cD->mode));
    do {
        s32 i;
        for (i = 0; i < 0x100000; i++);
    } while (sceCdSync(1));

    return buf;
}

void hCdCuePushExe(int arg0, int arg1, int arg2, int arg3, int arg4) {
    int exe;
    
    cQ->Exe--;
    if (cQ->Exe < 0) {
        cQ->Exe = 31;
    }

    exe = cQ->Exe;
    cQ->Arg[exe][0] = arg0;
    cQ->Arg[exe][1] = arg1;
    cQ->Arg[exe][2] = arg2;
    cQ->Arg[exe][3] = arg3;
    cQ->Arg[exe][4] = arg4;
    cQ->Num++;
}

void hCdCuePush(int arg0, int arg1, int arg2, int arg3, int arg4) {
    int reg = cQ->Reg;

    cQ->Arg[reg][0] = arg0;
    cQ->Arg[reg][1] = arg1;
    cQ->Arg[reg][2] = arg2;
    cQ->Arg[reg][3] = arg3;
    cQ->Arg[reg][4] = arg4;
    if (++cQ->Reg == 32) {
        cQ->Reg = 0;
    }
    cQ->Num++;
}

void hCdCuePop(int *arr) {
    s32 exe = cQ->Exe;
    
    arr[0] = cQ->Arg[exe][0];
    arr[1] = cQ->Arg[exe][1];
    arr[2] = cQ->Arg[exe][2];
    arr[3] = cQ->Arg[exe][3];
    arr[4] = cQ->Arg[exe][4];

    cQ->Exe++;
    if (cQ->Exe == 32) {
        cQ->Exe = 0;
    }
    cQ->Num--;
}

void hCdCuePeek(int *arr) {
    s32 i = cQ->Exe;
    arr[0] = cQ->Arg[i][0];
    arr[1] = cQ->Arg[i][1];
    arr[2] = cQ->Arg[i][2];
    arr[3] = cQ->Arg[i][3];
    arr[4] = cQ->Arg[i][4];
}

s32 hCdCueNum() {
    return cQ->Num;
}

s32 hCdCueArg4() {
    return cQ->Arg[cQ->Exe][4];
}

void hCdCueFlushBGM() {
    s32 i;
    s32 iVar2;
    s32 cue;
    u32 arr[32][5];

    i = 0;
    while ((cue = hCdCueNum()) != 0) {
        hCdCuePop(arr[i++]);
    }
    
    for (iVar2 = 0; iVar2 < i; iVar2++) {
        if (arr[iVar2][3] - 1 > 1) {
            hCdCuePush(arr[iVar2][0], arr[iVar2][1], arr[iVar2][2], arr[iVar2][3], arr[iVar2][4]);
        }
    }
}

void hCdCueFlushPPT() {
    s32 i;
    s32 iVar2;
    s32 cue;
    u32 arr[32][5];

    i = 0;
    while ((cue = hCdCueNum()) != 0) {
        hCdCuePop(arr[i++]);
    }
    
    for (iVar2 = 0; iVar2 < i; iVar2++) {
        if (arr[iVar2][3] != 3) {
            hCdCuePush(arr[iVar2][0], arr[iVar2][1], arr[iVar2][2], arr[iVar2][3], arr[iVar2][4]);
        }
    }
}

void hCdCueFlushBGM2() {
    s32 i;
    s32 iVar2;
    s32 cue;
    u32 arr[32][5];

    i = 0;
    while ((cue = hCdCueNum()) != 0) {
        hCdCuePop(arr[i++]);
    }
    
    for (iVar2 = 0; iVar2 < i; iVar2++) {
        if (arr[iVar2][3] - 1 > 1) {
            hCdCuePush(arr[iVar2][0], arr[iVar2][1], arr[iVar2][2], arr[iVar2][3], arr[iVar2][4]);
        }
    }
}

int hCdRead(u32 lbn, u32 sectors, u8 *buf, sceCdRMode *mode) {
    sceCdDiskReady(0);
    if (sceCdRead(lbn, sectors, buf, mode) == 0) {
        return 0;
    } else {
        cD->dataLSN = lbn;
        cD->dataSectors = sectors;
        cD->buf = buf;
        return 1;
    }
}

int hCdReadIOPm(u32 lbn, u32 sectors, u8 *buf, sceCdRMode *mode) {
    sceCdDiskReady(0);
    if (sceCdReadIOPm(lbn, sectors, buf, mode) == 0) {
        return 0;
    } else {
        cD->dataLSN = lbn;
        cD->dataSectors = sectors;
        cD->buf = buf;
        return 1;
    }
}

int hCdKlPackCount(int index) {
    return KlTable[index].count;
}

void hCdReadKlPack(int index, u8 *buf) {
    int i;

    printf("hCdReadKlPack(%d, 0x%08x)\n", index, buf);

    sceCdDiskReady(0);
    while (!sceCdRead(KlTable[index].offset + cD->file.lsn, KlTable[index].count, buf, &cD->mode));
    do {
        for (i = 0; i < 0x10000; i++) {
            // Do nothing
        }
    } while (sceCdSync(1));
}

void hCdPushKlPack(int index, u8 *buf) {
    hCdCuePush((cD->file).lsn + KlTable[index].offset, KlTable[index].count, (int)buf, 4, cD->eeCnt);
}

int hCdLoading() {
    return cD->dataFlag == CDREAD_DATA && cD->dataStat == 2;
}

void hCdInit() {
    int threadId;
    u8 *buf;
    u8 *addr;

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
    threadId = GetThreadId();
    cD->ThID = threadId;
    ChangeThreadPriority(threadId, 1);

    sceCdDiskReady(0);
    cD->DiscType = sceCdGetDiskType();
    buf = hCdReadFile("\\HEADPACK.BIN;1");
    addr = GetFHMAddress(buf, 0);
    KlTable = (KLTABLE *)(addr + 4);
    addr = GetFHMAddress(buf, 1);
    PptTable = (PPTTABLE *)(addr + 4);
    addr = GetFHMAddress(buf, 2);
    BgmTable = (BGMTABLE *)(addr + 4);

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&cD->file, "\\KLDATA.BIN;1"));
}

// From m2c, not matching
void hCdMain() {
    s32 var_s0;
    s32 var_s1;
    s32 sp[5];
    
    while ((cD->DiscError = sceCdGetError()) == -1);
    cD->Sync = sceCdSync(1);
    if (cD->DiscError >= 0) {
        var_s0 = 1;
        if (cD->DiscError >= 2) {
            var_s0 = 0;
            if (cD->DiscError == 0x13) {

            } else {
                goto loop_7;
            }
        }
    } else {
        var_s0 = 0;
loop_7:
        while (sceCdBreak() == 0);
        sceCdDiskReady(1);
        if (cD->dataStat != 0) {
            hCdCuePushExe(cD->dataLSN, cD->dataSectors, (int)cD->buf, cD->dataFlag, cD->eeCnt);
            cD->dataFlag = CDREAD_IDLE;
            cD->dataStat = 0;
        }
    }
    if (cD->dataStat == 2) {
        cD->dataFlag = CDREAD_IDLE;
        cD->dataStat = 0;
    }
    if ((hCdCueNum() > 0) && (cD->eeCnt >= hCdCueArg4()) && (var_s0 != 0)) {
        if (cD->dataStat == 0) {
            hCdCuePeek(sp);
            cD->dataFlag = sp[3];
            switch (cD->dataFlag) {
                case CDREAD_BGM:
                case CDREAD_BGM2:
                    var_s1 = hCdReadIOPm(sp[0], sp[1], (u8 *)sp[2], &cD->mode);
                    break;
                case CDREAD_PPT:
                case CDREAD_DATA:
                    var_s1 = hCdRead(sp[0], sp[1], (u8 *)sp[2], &cD->mode);
                    break;
                case 5:
                    var_s1 = hCdRead(sp[0], sp[1], (u8 *)sp[2], &cD->mode);
                    break;
            }
            if (var_s1 != 0) {
                hCdCuePop(sp);
                cD->dataStat = 1;
                cD->Sync = 1;
            }
        }
    }
    
    if (cD->dataStat != 0) {
        if (cD->Sync == 0) {
            cD->dataStat = 2;
        }
    }

    // hBgmMain();
    // hPptMain();
    // hStrMain();
    hStrInfo();
    cD->eeCnt++;
}
