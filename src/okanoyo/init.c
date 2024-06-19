#include "okanoyo/init.h"
#include "okanoyo/okbman.h"
#include "okanoyo/okio.h"
#include "okanoyo/okmain.h"
#include "okanoyo/okimain.h"
#include "okanoyo/okzfk00.h"
#include "hoshino/h_game.h"
#include "nakano/nkgf.h"
#include "harada/hr_main.h"
#include "kazuya.h"
#include "hoshino/h_file.h"
#include "nakano/objfunc.h"
#include "okanoyo/okzmain.h"
#include "okanoyo/okzbios.h"

RESTARTDATA restartData = {};
s32 okRestartTrg = 0;
s32 okRestart = 0;
s32 visbak = 0;
s32 OkDPStart = 0;
s32 OkjumpTrg = 0;

void camtest() {
    // Empty function
}

s32 OkReStartFlag() {
    return restartData.flag;
}

s32 OkReStartKloRouteNo() {
    return restartData.rno;
}

s64 OkReStartKloMichinori() {
    return restartData.mcn << 12;
}

f32 OkReStartKloYposition() {
    return restartData.ypos;
}

s32 OkKloMode() {
    return restartData.klomode;
}

s32 OkReStartVision() {
    return restartData.vision;
}

s32 OkReStartKloDir() {
    return restartData.klodir;
}

void OkReStartJob() {
    LastBalloonPic = -1;
    okRestartTrg = 1;
}

void OkReStartJobMain() {
    s32 i;
    s32 j;
    char *dstc;
    char *srcc;
    s32 *dsti;
    s32 *srci;

    if (restartData.flag == 1) {
        OkDebDPcnt = restartData.OkDPcnt;
        OkDebEXcnt = restartData.OkEXcnt;
        OkDebDPStcnt = restartData.OkDPStcnt;
        OkDebEXId = restartData.OkExId;

        for (i = 0; i < 24; i++) {
            srcc = restartData.zoutflgBufp[i];
            dstc = zoutflgBufp[i];
            for (j = 0; j < 256; j++) {
                *dstc = *srcc & ~2;
                dstc++;
                srcc++;
            }

            srci = restartData.ioutflgBufp[i];
            dsti = ioutflgBufp[i];
            for (j = 0; j < 8; j++) {
                *dsti = *srci;
                dsti++;
                srci++;
            }
        }

        srci = OkSTGBufp;
        for (i = 0; i < 64; i++) {
            *srci = restartData.OkSTGBuf[i];
            srci++;
        }

        okitemKeyflag = restartData.okitemKeyflag;
        okitemKeyusedflag = restartData.okitemKeyusedflag;
        okitemKeyID = restartData.okitemKeyID;
        OkFurorLightCount = 0;
        OkFurorLightSetCount = 0;
        OkFurorLightPtr = NULL;
    }
}

void ReStartItemFlagSet() {
    if (restartData.flag == 1) {
        okitemKeyflag = restartData.okitemKeyflag;
        okitemSwitchflag = restartData.okitemSwitchflag;
    }
}

void ReStartZakoSet() {
    // Empty function
}

void OkSaveforReStart(s32 rno, s32 mcn, f32 ypos, s32 bun1, s32 dir) {
    s32 i;
    s32 j;
    char *dstc;
    char *srcc;
    s32 *dsti;
    s32 *srci;

    restartData.flag = 1;
    if (rno == -1) {
        OBJWORK *klo = OkGetKloPtr();
        restartData.rno = klo->rtw.rtn;
        restartData.mcn = klo->rtw.mcn >> 12;
        restartData.ypos = klo->posi[1];
        restartData.klomode = klo->bun1;
    } else {
        restartData.rno = rno;
        restartData.mcn = mcn;
        restartData.ypos = ypos;
        restartData.klomode = bun1;
    }

    restartData.vision = GameGbl.vision;
    restartData.okitemKeyflag = okitemKeyflag;
    restartData.okitemKeyusedflag = okitemKeyusedflag;
    restartData.okitemSwitchflag = okitemSwitchflag;
    restartData.OkDPcnt = OkDebDPcnt;
    restartData.OkEXcnt = OkDebEXcnt;
    restartData.OkExId = OkDebEXId;
    restartData.OkDPStcnt = OkDebDPStcnt;
    restartData.okitemKeyID = okitemKeyID;
    restartData.klodir = dir;

    if (zakodataflag != -1) {
        for (i = 0; i < 24; i++) {
            srcc = zoutflgBufp[i];
            dstc = restartData.zoutflgBufp[i];
            for (j = 0; j < 256; j++) {
                *dstc = *srcc;
                dstc++;
                srcc++;
            }

            srci = ioutflgBufp[i];
            dsti = restartData.ioutflgBufp[i];
            for (j = 0; j < 8; j++) {
                *dsti = *srci;
                dsti++;
                srci++;
            }
        }

        srci = OkSTGBufp;
        for (i = 0; i < 64; i++) {
            restartData.OkSTGBuf[i] = *srci;
            srci++;
        }
    }

    nkGfReStartSave();
    hPushRestart();
    hr_restart_keep();
}

s32 OkDev2GameST() {
    switch(GameGbl.vision >> 8) {
        case 1:
            return STAGE_01;
        case 2:
            return STAGE_02;
        case 4:
            return STAGE_04;
        case 5:
            return STAGE_05;
        case 6:
            return STAGE_06;
        case 7:
            return STAGE_07;
        case 8:
            return STAGE_08;
        case 9:
            return STAGE_09;
        case 10:
            return STAGE_10;
        case 11:
            return STAGE_11;
        case 12:
            return STAGE_12;
        case 13:
            return STAGE_13;
        case 14:
            return STAGE_14;
        case 15:
            return STAGE_15;
        case 16:
            return STAGE_16;
        case 17:
            return STAGE_17;
        case 18:
            return STAGE_EX1;
        case 19:
            return STAGE_EX2;
        default:
            return -1;
    }
}

// TODO: delete these once kz is implemented
OKDATA* kzLoadOkData(s32 stage_no);
u8 kzLoadCovYume();

void OkReTryInit() {
    s32 i;
    s32 j;
    s32 num;
    char stack[256];

    if (GameGbl.vision / 256 >= 30) {
        OkZakoFuncOn = 0;
    } else {
        OkZakoFuncOn = 1;
    }

    okRestartTrg = 0;
    restartData.flag = 0;
    OkFurorLightCount = 0;
    OkFurorLightSetCount = 0;
    OkFurorLightPtr = NULL;
    okitemKeyflag = 0;
    restartData.okitemKeyflag = 0;
    okitemKeyusedflag = 0;

    for (i = 0; i < 64; i++) {
        OkSIDBufp[i] = 0;
    }

    for (i = 0; i < 64; i++) {
        OkSTGBufp[i] = 0;
    }

    if (!OkZakoFuncOn) {
        OkDebSIDflag = -1;
        zakodataflag = -1;
        return;
    }

    if (hGetDataAddr(1) == 0) {
        OkZakoFuncOn = 0;
    }

    if (!OkZakoFuncOn) {
        OkDebSIDflag = -1;
        zakodataflag = -1;
        return;
    }

    restartData.klomode = -1;
    OkDebSIDflag = -1;
    
    num = OkDev2GameST();
    if (num != -1) {
        if (kzCheckGameClear()) {
            OkDebDPcnt = 0;
        } else {
            OkDebDPcnt = kzLoadCovYume() % 100;
        }
        
        OkDPStart = OkDebDPcnt;
        OkDebEXcnt = kzLoadStgDolls(num);
        OkDebDPStcnt = 0;
        OkDebEXId = kzLoadOkData(num)->ExID & 0xFF;
        restartData.OkEXcnt = OkDebEXcnt;
        restartData.OkExId = OkDebEXId;
        restartData.OkDPStcnt = OkDebDPStcnt;
    } else {
        OkDebDPcnt = 0;
        OkDebEXcnt = 0;
        OkDebDPStcnt = 0;
        OkDebEXId = 0;
    }

    LastBalloonPic = -1;
    LastFuror = (OBJWORK *)-1;
    okitemKeyID = -1;
    for (i = 0; i < 24; i++) {
        teki_shutugen = zoutflgBufp[i];
        for (j = 0; j < 256; j++) {
            teki_shutugen[j] = 0;
        }

        teki_shutugen = restartData.zoutflgBufp[i];
        for (j = 0; j < 256; j++) {
            teki_shutugen[j] = 0;
        }

        okitemflag = ioutflgBufp[i];
        for (j = 0; j < 8; j++) {
            okitemflag[j] = 0;
        }

        okitemflag = restartData.ioutflgBufp[i];
        for (j = 0; j < 8; j++) {
            okitemflag[j] = 0;
        }
    }

    OkDebSIDflag = 1;
    restartData.flag = 0;
}

void OkZakoInitStage() {
    char FileName[256];
	s32 flg;
	s32 i;
	s32 j;
    s32 num;

    if (GameGbl.vision / 256 >= 34) {
        OkDebDPcnt = kzLoadCovYume() % 100;
    }

    if (GameGbl.vision / 256 >= 30) {
        OkZakoFuncOn = 0;
    } else {
        OkZakoFuncOn = 1;
    }

    okRestartTrg = 0;
    restartData.flag = 0;
    OkFurorLightCount = 0;
    OkFurorLightSetCount = 0;
    OkFurorLightPtr = NULL;
    okitemKeyflag = 0;
    restartData.okitemKeyflag = 0;
    okitemKeyusedflag = 0;
    OkSIDBufp = getBuff(1, sizeof(s32) * 64, NULL, &flg);
    OkSTGBufp = getBuff(1, sizeof(s32) * 64, NULL, &flg);
    siworkp = getBuff(1, sizeof(SI_WORK) * 256, NULL, &flg);

    for (i = 0; i < 64; i++) {
        OkSIDBufp[i] = 0;
    }

    for (i = 0; i < 64; i++) {
        OkSTGBufp[i] = 0;
    }

    if (!OkZakoFuncOn) {
        OkDebSIDflag = -1;
        zakodataflag = -1;
        return;
    }

    if (hGetDataAddr(1) == 0) {
        OkZakoFuncOn = 0;
    }

    if (!OkZakoFuncOn) {
        OkDebSIDflag = -1;
        zakodataflag = -1;
        return;
    }

    restartData.klomode = -1;
    OkDebSIDflag = -1;
    
    num = OkDev2GameST();
    if (num != -1) {
        if (kzCheckGameClear()) {
            OkDebDPcnt = 0;
        } else {
            OkDebDPcnt = kzLoadCovYume() % 100;
        }
        
        OkDPStart = OkDebDPcnt;
        OkDebEXcnt = kzLoadStgDolls(num);
        OkDebDPStcnt = 0;
        OkDebEXId = kzLoadOkData(num)->ExID & 0xFF;
        restartData.OkEXcnt = OkDebEXcnt;
        restartData.OkExId = OkDebEXId;
        restartData.OkDPStcnt = OkDebDPStcnt;
    } else {
        OkDebDPcnt = 0;
        OkDebEXcnt = 0;
        OkDebDPStcnt = 0;
        OkDebEXId = 0;
    }

    LastBalloonPic = -1;
    LastFuror = (OBJWORK *)-1;
    DebmemFormat(2);
    okitemKeyID = -1;
    for (i = 0; i < 24; i++) {
        teki_shutugen = zoutflgBufp[i] = getBuff(1, 256, NULL, &flg);
        for (j = 0; j < 0x100; j++) {
            teki_shutugen[j] = 0;
        }

        
        teki_shutugen = restartData.zoutflgBufp[i] = getBuff(1, 256, NULL, &flg);
        for (j = 0; j < 0x100; j++) {
            teki_shutugen[j] = 0;
        }
        
        okitemflag = ioutflgBufp[i] = getBuff(1, sizeof(s32) * 64, NULL, &flg);
        for (j = 0; j < 8; j++) {
            okitemflag[j] = 0;
        }
        okitemflag = restartData.ioutflgBufp[i] = getBuff(1, sizeof(s32) * 64, NULL, &flg);
        for (j = 0; j < 8; j++) {
            okitemflag[j] = 0;
        }
    }

    ZAKODataBuffTop = (char *)hGetDataAddr(1);
    OkDebSIDTop = FADR(ZAKODataBuffTop, 24);
    OkDebSIDflag = 1;
    restartData.flag = 0;
    OkVpoStageInit();
    OkHpoInit();
}

s32 OkZakoAliveArea(s32 vision) {
    s32 arr[] = {-1, 4, 6, 0, 12, 5, 8, 5, 5, 7, 4, 16, 6, 4, 4, 8, 10, 2, 5, 8, -1, 0, 0, 0, -1, -1, 0, -1, 1};
    s32 vs0 = vision / 256;
    s32 vs1 = vision & 0xFF;

    if (vs0 >= 30) {
        return -1;
    } else if (arr[vs0] == -1) {
        return -1;
    } else if (arr[vs0] < vs1) {
        return -1;
    } else {
        return 1;
    }
}

void OkZakoInitArea() {
	char FileName[256];
	s32 i;
	OBJWORK *objw;
	OKDEBDATAEX *debp = okDebData.debp;
	u32 *rtp;
	s32 arnum = GameGbl.vision & 0xFF;

    OkZakoFuncOn = 1;
    if (hGetDataAddr(1) == 0) {
        OkZakoFuncOn = 0;
    }
    if (OkZakoAliveArea(GameGbl.vision) == -1) {
        OkZakoFuncOn = 0;
    }

    OkKeyDoorOpen = -1;
    OkjumpTrg = 0;

    if (!OkZakoFuncOn) {
        OkDebSIDflag = -1;
        zakodataflag = -1;
        return;
    }

    OkDebSIDflag = 1;
    OkReBalloon = -1;
    OkReFuror = -1;
    OkBossFlag = 0;
    OkDebugCamCurMode = 0;
    SETVECTOR(klposb, 0.0f, 0.0f, 0.0f, 1.0f);
    SETVECTOR(klspd, 0.0f, 0.0f, 0.0f, 1.0f);
    okCwork.trgptr = NULL;
    okCwork.DirectCam = 0;
    teki_shutugen = zoutflgBufp[GameGbl.vision & 0xFF];
    teki_Rshutugen = restartData.zoutflgBufp[GameGbl.vision & 0xFF];
    okitemflag = ioutflgBufp[GameGbl.vision & 0xFF];
    zakodataflag = 1;
    itemdataflag = 1;
    ZAKODataBuffp = FADR(ZAKODataBuffTop, 25 + arnum * 3);
    ITEMDataBuffp = FADR(ZAKODataBuffTop, 26 + arnum * 3);
    if ((s32)FADR(ZAKODataBuffTop, 27 + arnum * 3) != -1) {
        OkDebZKRTno = OkInitMichi(FADR(ZAKODataBuffTop, 27 + arnum * 3));
    } else {
        OkDebZKRTno = 0;
    }
    if (zakodataflag != -1) {
        setTEKI_DATA(ZAKODataBuffp);
    }

    objw = SearchOBJWORK(SysGbl.objwork,SysGbl.n_objw);
    objw->stat0 = 2;
    objw->stat1 = 3;
    objw->bun0 = 0;
    objw = SearchOBJWORK(SysGbl.objwork,SysGbl.n_objw);
    objw->stat0 = 2;
    objw->stat1 = 0x1000;
    objw->bun0 = 0;
    objw = SearchOBJWORK(SysGbl.objwork,SysGbl.n_objw);
    objw->stat0 = 2;
    objw->stat1 = 2;
    objw->bun0 = 0;
    
    debp->csr_keyhis = 0;
    debp->csr_dbkey = 0;
    debp->csr_wtkey = 0;
    debp->csr_dbcnt = 0;

    okitemSwitchflag = 0;
    okRestart = okRestartTrg;
    if (okRestartTrg == 1) {
        OkReStartJobMain();
        okRestartTrg = 0;
    }

    for (i = 0; i < 256; i++) {
        OkOutFlagClean(i);
    }

    OkDebEXId = 0;
    OkDebEXcnt = 0;
    OkDebDPTrg = 0;
    OkDebKLTrg = 0;
    OkDebHTTrg = 0;
    OkDebEXTrg = 0;
    OkDebM_Elf = 0;
    OkM_ElfCnt = 0;
    OkJumpDaiSpd = 0.0f;
    okkldmhit = &okkldmhitN;
    okkzathit = &okkzathitN;
    okitgthit = okdefhittbl2I;
    OkVpoAreaInit();
    OkGrtInit();

    for (i = 0; i < 64; i++) {
        OkSIDBufp[i] = 0;
    }
}

