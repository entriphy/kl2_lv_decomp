#include "common.h"

KLTABLE *FileData = NULL;
hCDDATA CdData = {};
hCDDATA *cD = NULL;
hCDCUE *cQ = NULL;
u8 cb_stack[8192];

void hCdReadFile(char *filename, u32 *buff) {
    sceCdlFILE file;
    s32 ret;
    s32 sectors;
    
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&file, filename));
    sectors = ((file.size + 0x7FF) / 0x800);
    if (sectors > 0x400) {
        sectors = 0x400;
    }
    while (!sceCdRead(file.lsn, sectors, buff, &cD->mode));
    do {
        s32 i;
        for (i = 0; i < 0x100000; i++);
    } while (sceCdSync(1));
}

u32 * hCdReadFileRet(char *filename) {
    sceCdlFILE file;
    u32 *buf;
    s32 ret;
    s32 sectors;
    
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&file, filename));
    sectors = ((file.size + 0x7FF) / 0x800);
    buf = (u32 *)getBuff(1, sectors * 0x800, NULL, &ret);
    while (!sceCdRead(file.lsn, sectors, buf, &cD->mode));
    do {
        s32 i;
        for (i = 0; i < 0x100000; i++);
    } while (sceCdSync(1));

    return buf;
}

void hCdCuePushLIFO(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 exe;
    
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

void hCdCuePush(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 reg = cQ->Reg;

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

void hCdCuePop(s32 *p) {
    s32 exe = cQ->Exe;
    
    p[0] = cQ->Arg[exe][0];
    p[1] = cQ->Arg[exe][1];
    p[2] = cQ->Arg[exe][2];
    p[3] = cQ->Arg[exe][3];
    p[4] = cQ->Arg[exe][4];

    cQ->Exe++;
    if (cQ->Exe == 32) {
        cQ->Exe = 0;
    }
    cQ->Num--;
}

void hCdCuePopTest(s32 *p) {
    s32 i = cQ->Exe;
    p[0] = cQ->Arg[i][0];
    p[1] = cQ->Arg[i][1];
    p[2] = cQ->Arg[i][2];
    p[3] = cQ->Arg[i][3];
    p[4] = cQ->Arg[i][4];
}

s32 hCdCueNum() {
    return cQ->Num;
}

s32 hCdCueTime() {
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

s32 hCdRead(u32 lsn, u32 sectors, void *buff, sceCdRMode *mode) {
    sceCdDiskReady(0);
    if (sceCdRead(lsn, sectors, buff, mode) == 0) {
        return 0;
    } else {
        cD->dataLSN = lsn;
        cD->dataSectors = sectors;
        cD->buf = buff;
        return 1;
    }
}

s32 hCdReadIOPm(u32 lsn, u32 sectors, void *buff, sceCdRMode *mode) {
    sceCdDiskReady(0);
    if (sceCdReadIOPm(lsn, sectors, buff, mode) == 0) {
        return 0;
    } else {
        cD->dataLSN = lsn;
        cD->dataSectors = sectors;
        cD->buf = buff;
        return 1;
    }
}

s32 hCdGetSize(s32 no) {
    return FileData[no].sectors;
}

void hCdReadDataBlock(s32 no, s32 buff) {
    s32 i;

    sceCdDiskReady(0);
    while (!sceCdRead(FileData[no].lsn + cD->file.lsn, FileData[no].sectors, (void *)buff, &cD->mode));
    do {
        for (i = 0; i < 0x10000; i++) {
            // Do nothing
        }
    } while (sceCdSync(1));
}

void hCdReadData(s32 no, s32 buff) {
    hCdCuePush((cD->file).lsn + FileData[no].lsn, FileData[no].sectors, buff, 4, cD->eeCnt);
}

s32 hCdReadSync() {
    return cD->dataFlag == CDREAD_DATA && cD->dataStat == 2;
}

void hCdInit() {
    s32 threadId;
    u32 *buff;
    s32 *addr;

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
    
    sceCdInitEeCB(0, cb_stack, 0x2000);
    threadId = GetThreadId();
    cD->ThID = threadId;
    ChangeThreadPriority(threadId, 1);

    sceCdDiskReady(0);
    cD->DiscType = sceCdGetDiskType();
    buff = hCdReadFileRet("\\HEADPACK.BIN;1");
    addr = GetFHMAddress(buff, 0);
    FileData = (KLTABLE *)&addr[1];
    addr = GetFHMAddress(buff, 1);
    PptTable = (PPTTABLE *)&addr[1];
    addr = GetFHMAddress(buff, 2);
    BgmTable = (BGMTABLE *)&addr[1];

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
            hCdCuePushLIFO(cD->dataLSN, cD->dataSectors, (s32)cD->buf, cD->dataFlag, cD->eeCnt);
            cD->dataFlag = CDREAD_IDLE;
            cD->dataStat = 0;
        }
    }
    if (cD->dataStat == 2) {
        cD->dataFlag = CDREAD_IDLE;
        cD->dataStat = 0;
    }
    if ((hCdCueNum() > 0) && (cD->eeCnt >= hCdCueTime()) && (var_s0 != 0)) {
        if (cD->dataStat == 0) {
            hCdCuePopTest(sp);
            cD->dataFlag = sp[3];
            switch (cD->dataFlag) {
                case CDREAD_BGM:
                case CDREAD_BGM2:
                    var_s1 = hCdReadIOPm(sp[0], sp[1], (void *)sp[2], &cD->mode);
                    break;
                case CDREAD_PPT:
                case CDREAD_DATA:
                    var_s1 = hCdRead(sp[0], sp[1], (void *)sp[2], &cD->mode);
                    break;
                case 5:
                    var_s1 = hCdRead(sp[0], sp[1], (void *)sp[2], &cD->mode);
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
