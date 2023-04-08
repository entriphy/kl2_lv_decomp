#include <libcdvd.h>
#include "common.h"

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

void hCdCue_00165c60(int *arr) {
    s32 i = cQ->Exe;
    arr[0] = cQ->Arg[i][0];
    arr[1] = cQ->Arg[i][1];
    arr[2] = cQ->Arg[i][2];
    arr[3] = cQ->Arg[i][3];
    arr[4] = cQ->Arg[i][4];

    cQ->Exe++;
    if (cQ->Exe == 32) {
        cQ->Exe = 0;
    }
    cQ->Num--;
}

s32 hCdCueNum() {
    return cQ->Num;
}

void hCdCueFlushBGM() {
    s32 i;
    s32 iVar2;
    s32 cue;
    u32 arr[32][5];

    i = 0;
    while ((cue = hCdCueNum()) != 0) {
        hCdCue_00165c60(arr[i++]);
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
        hCdCue_00165c60(arr[i++]);
    }
    
    for (iVar2 = 0; iVar2 < i; iVar2++) {
        if (arr[iVar2][3] != 3) { // this is the only change from hCdCueFlushBGM lol
            hCdCuePush(arr[iVar2][0], arr[iVar2][1], arr[iVar2][2], arr[iVar2][3], arr[iVar2][4]);
        }
    }
}

int FUN_00166128(int index) {
    return KlTable[index].count;
}

void hCdReadKlPack(int index, u8 *buf) {
    sce_print("@@@ reading file %d to 0x%08x\n", index, buf);

    sceCdDiskReady(0);
    while (!sceCdRead(KlTable[index].offset + cD->file.lsn, KlTable[index].count, buf, &cD->mode));
    do {
        for (int i = 0; i < 0x10000; i++) {
            // Do nothing
        }
    } while (sceCdSync(1));
}

void FUN_001661e0(int param_1, u8 *param_2) {
    hCdCuePush((cD->file).lsn + KlTable[param_1].offset, KlTable[param_1].count, (int)param_2, 4, cD->eeCnt);
}

int isLoading() {
    int ret = 0;
    if (cD->dataFlag == CDREAD_DATA) {
        ret = cD->dataStat == 2;
    }
    return ret;
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
    u8 *buf = hCdReadFile("\\HEADPACK.BIN;1");
    u8 *addr = GetFHMAddress(buf, 0);
    KlTable = (KLTABLE *)(addr + 4);
    addr = GetFHMAddress(buf, 1);
    PptTable = (PPTTABLE *)(addr + 4);
    addr = GetFHMAddress(buf, 2);
    BgmTable = (BGMTABLE *)(addr + 4);

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&cD->file, "\\KLDATA.BIN;1"));
}
