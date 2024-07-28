#include "take/view.h"
#include "take/berny.h"
#include "take/motip.h"
#include "take/motsys.h"
#include "take/object.h"
#include "take/sfxbios.h"
#include "take/viewsub.h"
#include "abe/ab_init.h"
#include "harada/hr_mapdr.h"
#include "nakano/dma.h"
#include "nakano/map00.h"
#include "okanoyo/okio.h"

static void ModelSet();
static void ModelSetAll();

void ViewInit() {
    sceVu0FVECTOR v[5]; // i have no idea what they're smoking here
    s32 size;
    s32 i;
	s32 ret;
    u32 idk; // ?
    
    SETVEC(v[0], 0.0f, 0.0f, 1.0f, 0.0f);
    GetJetMatrix(&v[1], v[1], v[0], v[0]);
    sceVu0MulMatrix(&v[0], &v[1], &v[0]);
    memoryStageFormat();
    memoryAreaptrSet();
    memoryAreaFormat();
    DataBuffPtr = getBuff(1, 0x1100000, NULL, &ret);
    SfxSystemInit(DataBuffPtr);
    DataBuffPtr += 0x40000;
    SfxInitDebugCamera();
    nkPathReset();
    nkInitAtrMain();
    sceGsSyncPath(0, 0);
    nkDebFlush();
    nkPathFlush();
    nkPathReset();
    size = ReadFile("host0:../../dat/chr/chr.lst", (qword *)DataBuffPtr);
    pBinh = (BINH *)DataBuffPtr;
    DataBuffPtr = (u32 *)((u32)DataBuffPtr + size);
    ModelIndex = 0;
    for (i = 0; i < 4; i++) {
        vObj[i].pObj = NULL;
        vObj[i].ActNum = 0;
        vObj[i].AllFlag = 0;
    }

    idk = (u32)ALIGN(DataBuffPtr);
    SfxDataBuff[0] = (u8 *)idk;
    SfxDataBuff[1] = (u8 *)(idk + 0x300000);
    SfxDataBuff[2] = (u8 *)(idk + 0x600000);
    SfxDataBuff[3] = (u8 *)(idk + 0x900000);
    DataBuffPtr = (u32 *)(idk + 0xC00000);
    DataBuffPtr = ALIGN(DataBuffPtr);

    GameGbl.vision = 0x100;
    abGimInit();

    size = ReadFile("host0:../../dat/chr/clip.bin", (qword *)DataBuffPtr);
    pClip = (f32 *)DataBuffPtr;
    DataBuffPtr = ALIGN((u32)DataBuffPtr + size);

    DirNum = 0;
    ChrNum = 0;
    ViewMode = 0;
    ViewSubMode = 0;
}

void View() {
	s32 i;
	char ActName[9];
	char Name[3];

    nkPathFlush();
    nkPathReset();
    
    // OkPrint(0, 2, 1,scr_p[0],"ModelViewer Ver 0.00001");
    // OkPrint(0, 2, 2, scr_p[0], "Mode = ");
    // OkPrint(0, 9, 2, scr_p[0], "%s", ModeName[ViewMode]);
    
    Vu0ProgTrans();
    BgDraw();
    
    if (PAD_TRG_SELECT(GameGbl.kpd[0])) {
        ViewMode++;
        if (ViewMode > 2) {
            ViewMode = 0;
        }
    }

    for (i = 0; i < 4; i++) {
        if (vObj[i].pObj != NULL) {
            strcpy(ReadFileName, pBinh->FileName[vObj[i].DirNum][vObj[i].ChrNum]);
            // OkPrint(0, i * 11 + 2, 4, scr_p[0], "DATA%d %s", i, ReadFileName);
        } else {
            // OkPrint(0, i * 11 + 2, 4, scr_p[0], "DATA%d NOP", i);
        }

        if (vObj[i].AllFlag != 0) {
            // OkPrint(0, i * 11 + 7, 4, scr_p[0], "*", i);
        }
    }
    // OkPrint(0, ModelIndex * 11 + 1, 4, scr_p[0], ">");

    if (PAD_TRG_START(GameGbl.kpd[0])) {
        ModelIndex++;
        if (ModelIndex > 3) {
            ModelIndex = 0;
        }
    }

    hrDrawVpo_st();
    OutLineEnvInit(GameGbl.fr, GameGbl.inter);
    pnfPrg[ViewMode]();
    hrDrawVpo_end();
    sceGsSyncPath(0, 0);
    // OkPrint(0, 3, 0x22, scr_p[0], "TotalCnt %d", *T0_COUNT);
}

static void ViewRead_() {
	s32 i;
	char TmpName[16];

    for (i = 0; i < pBinh->DirNum; i++) {
        // OkPrint(0, i / 24 * 8 + 2, i % 24 + 7, scr_p[0], "%s", pBinh->DirName[i]);
    }
    // OkPrint(0, DirNum / 24 * 8 + 1, DirNum % 24 + 7, scr_p[0], ">");
    
    for (i = 0; i < pBinh->FileNum[DirNum]; i++) {
        // OkPrint(0, i / 24 * 4 + 27, i % 24 + 7, scr_p[0], "%s", pBinh->FileName[DirNum][i]);
    }
    // OkPrint(0, ChrNum / 24 * 4 + 26, ChrNum % 24 + 7, scr_p[0], ">");

    if (DirNum >= pBinh->DirNum) {
        DirNum = pBinh->DirNum - 1;
    }
    if (PAD_TRG_UP(GameGbl.kpd[0])) {
        DirNum--;
        if (DirNum < 0) {
            DirNum = pBinh->DirNum - 1;
        }
    }
    if (PAD_TRG_DOWN(GameGbl.kpd[0])) {
        DirNum++;
        if (DirNum >= pBinh->DirNum) {
            DirNum = 0;
        }
    }
    if (PAD_TRG_LEFT(GameGbl.kpd[0])) {
        DirNum -= 24;
        if (DirNum < 0) {
            s32 yeet = DirNum; // ?
            yeet = (pBinh->DirNum / 24) * 24 + 24;
            DirNum += yeet;
        }
    }
    if (PAD_TRG_RIGHT(GameGbl.kpd[0])) {
        DirNum += 24;
        if (DirNum >= pBinh->DirNum) {
            DirNum %= 24;
        }
    }

    if (ChrNum >= pBinh->FileNum[DirNum]) {
        ChrNum = pBinh->FileNum[DirNum] - 1;
    }
    if (PAD_TRG_TRIANG(GameGbl.kpd[0])) {
        ChrNum--;
        if (ChrNum < 0) {
            ChrNum = pBinh->FileNum[DirNum] - 1;
        }
    }
    if (PAD_TRG_CROSS(GameGbl.kpd[0])) {
        ChrNum++;
        if (ChrNum >= pBinh->FileNum[DirNum]) {
            ChrNum = 0;
        }
    }
    if (PAD_TRG_SQUARE(GameGbl.kpd[0])) {
        ChrNum -= 24;
        if (ChrNum < 0) {
            s32 yeet = ChrNum; // ?
            yeet = (pBinh->FileNum[DirNum] / 24) * 24 + 24;
            ChrNum += yeet;
        }
    }
    if (PAD_TRG_CIRCLE(GameGbl.kpd[0])) {
        ChrNum += 24;
        if (ChrNum >= pBinh->FileNum[DirNum]) {
            ChrNum %= 24;
        }
    }
    if (ChrNum >= pBinh->FileNum[DirNum]) {
        ChrNum = pBinh->FileNum[DirNum] - 1;
    }


    if (PAD_TRG_L1(GameGbl.kpd[0])) {
        if (PAD_LVL_L2(GameGbl.kpd[0])) {
            strcpy(ChrNameTmp, pBinh->FileName[DirNum][ChrNum]);
            ModelSetAll();
        } else {
            strcpy(ChrNameTmp, pBinh->FileName[DirNum][ChrNum]);
            strcpy(ReadFileName, "host0:../../dat/chr/");
            strcat(ReadFileName, pBinh->FileName[DirNum][ChrNum]);
            strcat(ReadFileName, "data.bin");
            ModelSet();
        }

        vObj[ModelIndex].DirNum = DirNum;
        vObj[ModelIndex].ChrNum = ChrNum;
    }
    
    if (PAD_TRG_R1(GameGbl.kpd[0])) {
        EraseSfxObject(vObj[ModelIndex].pObj);
        vObj[ModelIndex].pObj = NULL;
    }

    if (vObj[ModelIndex].pObj != NULL) {
        if (PAD_TRG_R2(GameGbl.kpd[0])) {
            SetObjCondition(vObj[ModelIndex].pObj, ((u16)vObj[ModelIndex].pObj->Condition << 0x11) >> 0x10); // this should be a sign extension
            if (vObj[ModelIndex].pObj->Condition > 8) {
                SetObjCondition(vObj[ModelIndex].pObj, 1);
            }
        }
    }

    for (i = 0; i < 4; i++) {
        if (vObj[i].pObj != NULL) {
            ModelDraw(vObj[i].pObj);
        }
    }
}

static void ModelSet() {
	s32 ChrNumTmp;
    SFXOBJ *pObj;
    f32 var_f0;
    f32 var_f1;

    if (vObj[ModelIndex].pObj != NULL) {
        EraseSfxObject(vObj[ModelIndex].pObj);
    }

    ReadFile(ReadFileName, (qword *)SfxDataBuff[ModelIndex]);
    pObj = vObj[ModelIndex].pObj = SetSfxObject((u32 *)SfxDataBuff[ModelIndex]);
    ChrNumTmp = GetChrNum(ChrNameTmp);
    pObj->ObjNum = ChrNumTmp;
    if (pObj->ObjNum == 436) {
        SetBernyFire(pObj);
    }
    SetObjClipZone(pObj, (pClip + ChrNumTmp * 6)[0], (pClip + ChrNumTmp * 6)[1]);
    vObj[ModelIndex].ActNum = 1;
    vObj[ModelIndex].AllFlag = 0;
    SETVEC(vObj[ModelIndex].Rot, 0.0f, 0.0f, 0.0f, 0.0f);
    var_f0 = 200.0f;
    var_f1 = 1.0f;
    SETVEC(vObj[ModelIndex].Trans, 0.0f, 0.0f, var_f0, var_f1);
    SetBaseMatrix(pObj, vObj[ModelIndex].Rot, vObj[ModelIndex].Trans, 0);
}

static void ModelSetAll() {
    s32 ChrNumTmp;
    SFXOBJ *pObj;
    f32 var_f0;
    f32 var_f1;

    if (vObj[ModelIndex].pObj != NULL) {
        EraseSfxObject(vObj[ModelIndex].pObj);
    }

    pObj = vObj[ModelIndex].pObj = GetSfxObjectData(GetChrNum(ChrNameTmp));
    vObj[ModelIndex].ActNum = 1;
    vObj[ModelIndex].AllFlag = 1;
    SETVEC(vObj[ModelIndex].Rot, 0.0f, 0.0f, 0.0f, 0.0f);
    var_f0 = 200.0f;
    var_f1 = 1.0f;
    SETVEC(vObj[ModelIndex].Trans, 0.0f, 0.0f, var_f0, var_f1);
    SetBaseMatrix(pObj, vObj[ModelIndex].Rot, vObj[ModelIndex].Trans, 0);
}

static void ViewAction_() {
	SFXOBJ *pObj;
	SFXOBJ *pObjAct;
	s32 i;
	s32 ii;
	char TmpName[16];
	s32 TmpCnt;
	char ActName[9];
	sceVu0FVECTOR DistVector;
	s32 frame0;
	s32 frame1;
	f32 Weight;
	ACT_HEADER *pAct;
    
    if (vObj[ModelIndex].pObj != NULL) {
        pObj = vObj[ModelIndex].pObj;
        frame0 = (GameGbl.fr % 500) / 100;
        frame1 = ((GameGbl.fr + 100) % 500) / 100;
        Weight = (GameGbl.fr % 100) / 100.0f;
        SetSfxVariationClut(pObj, frame0, frame1, Weight);
        if (GameGbl.kpd[0].trg & 0x5) { // L1 | L2
            pObjAct = GetActiveSfx(pObj);
            
            if (PAD_TRG_L1(GameGbl.kpd[0])) {
                vObj[ModelIndex].ActNum--;
                if (vObj[ModelIndex].ActNum < 1) {
                    vObj[ModelIndex].ActNum = pObjAct->pMot->ActNumMax - 1;
                }
            }
            if (PAD_TRG_L2(GameGbl.kpd[0])) {
                vObj[ModelIndex].ActNum++;
                if (vObj[ModelIndex].ActNum >= pObjAct->pMot->ActNumMax) {
                    vObj[ModelIndex].ActNum = 1;
                }
            }

            SetActIp(pObj, vObj[ModelIndex].ActNum);
        }

        BaseMove(pObj, vObj[ModelIndex].Rot, vObj[ModelIndex].Trans);
    }

    for (i = 0; i < 4; i++) {
        if (vObj[i].pObj != NULL) {
            TmpCnt = DGET_T0_COUNT();
            pObj = vObj[i].pObj;
            ModelDraw(pObj);
            TmpCnt = DGET_T0_COUNT() - TmpCnt;
            // OkPrint(0, i * 11 + 2, 5, scr_p[0], "CNT=%d", TmpCnt);

            pObjAct = GetActiveSfx(pObj);
            if (pObjAct != NULL && pObjAct->pMot->CoordNum != 0) {
                pAct = (ACT_HEADER *)pObjAct->pMot->Mb[pObjAct->pMot->BaseIndex].pAct;
                for (ii = 0; ii < 8; ii++) {
                    ActName[ii] = pAct->ActName[ii];
                }
                ActName[8] = 0;

                // OkPrint(0, i * 11 + 2, 6, scr_p[0], "%s", ActName);
                // OkPrint(0, i * 11 + 2, 7, scr_p[0], "Number=%d", vObj[i].ActNum);
                // OkPrint(0, i * 11 + 2, 8, scr_p[0], "%d", (s32)GetActEndCnt(pObj));
                // OkPrint(0, i * 11 + 2, 9, scr_p[0], "%d", (s32)GetActCnt(pObj));
                GetActEndCnt(pObj);
                GetActCnt(pObj);
            }
        }
    }

    SfxDebugCameraWork();
}

static void ViewClip_() {
	SFXOBJ *pObj;
	SFXOBJ *pObjAct;
	s32 i;
	s32 ii;
	char TmpName[16];
	s32 TmpCnt;
	char ActName[9];
	sceVu0FVECTOR DistVector;
	f32 Offset;
	f32 Zone;
	f32 Add;
    ACT_HEADER *pAct;

    if (vObj[ModelIndex].pObj != NULL) {
        pObj = vObj[ModelIndex].pObj;

        if (GameGbl.kpd[0].trg & 0x5) { // L1 | L2
            pObjAct = GetActiveSfx(pObj);
            
            if (PAD_TRG_L1(GameGbl.kpd[0])) {
                vObj[ModelIndex].ActNum--;
                if (vObj[ModelIndex].ActNum < 1) {
                    vObj[ModelIndex].ActNum = pObjAct->pMot->ActNumMax - 1;
                }
            }
            if (PAD_TRG_L2(GameGbl.kpd[0])) {
                vObj[ModelIndex].ActNum++;
                if (vObj[ModelIndex].ActNum >= pObjAct->pMot->ActNumMax) {
                    vObj[ModelIndex].ActNum = 1;
                }
            }

            SetAct(pObj, vObj[ModelIndex].ActNum);
        }

        BaseMove(pObj, vObj[ModelIndex].Rot, vObj[ModelIndex].Trans);

        Offset = pObj->ClipOffset;
        Zone = pObj->ClipZone;
        Add = PAD_LVL_TRIANG(GameGbl.kpd[1]) ? 1.0f : 0.1f;

        if (PAD_LVL_R1(GameGbl.kpd[1])) {
            Zone += Add;
            SetObjClipZone(pObj, Offset, Zone);
        }
        if (PAD_LVL_R2(GameGbl.kpd[1])) {
            Zone -= Add;
            SetObjClipZone(pObj, Offset, Zone);
        }
        if (PAD_LVL_L1(GameGbl.kpd[1])) {
            Offset += Add;
            SetObjClipZone(pObj, Offset, Zone);
        }
        if (PAD_LVL_L2(GameGbl.kpd[1])) {
            Offset -= Add;
            SetObjClipZone(pObj, Offset, Zone);
        }
    }

     for (i = 0; i < 4; i++) {
        if (vObj[i].pObj != NULL) {
            TmpCnt = DGET_T0_COUNT();
            pObj = vObj[i].pObj;
            ModelDraw(pObj);
            DrawClipZone(pObj);
            TmpCnt = DGET_T0_COUNT() - TmpCnt;
            // OkPrint(0, i * 11 + 2, 5, scr_p[0], "CNT=%d", TmpCnt);

            pObjAct = GetActiveSfx(pObj);
            if (pObjAct != NULL && pObjAct->pMot->CoordNum != 0) {
                pAct = (ACT_HEADER *)pObjAct->pMot->Mb[pObjAct->pMot->BaseIndex].pAct;
                for (ii = 0; ii < 8; ii++) {
                    ActName[ii] = pAct->ActName[ii];
                }
                ActName[8] = 0;

                // OkPrint(0, i * 11 + 2, 6, scr_p[0], "%s", ActName);
                // OkPrint(0, i * 11 + 2, 7, scr_p[0], "Number=%d", vObj[i].ActNum);
                // OkPrint(0, i * 11 + 2, 8, scr_p[0], "%d", (s32)GetActEndCnt(pObj));
                // OkPrint(0, i * 11 + 2, 9, scr_p[0], "%d", (s32)GetActCnt(pObj));
                GetActEndCnt(pObj);
                GetActCnt(pObj);
            }

            // OkPrint(0, i * 11 + 2, 10, scr_p[0], "Ypos=%d", (s32)(pObj->ClipOffset / -2.4));
            // OkPrint(0, i * 11 + 2, 11, scr_p[0], "Zone=%d", (s32)(pObj->ClipZone / 2.4));
        }
    }
}

f32 klwipsp[48] = {
    10.0f,
    20.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    10.0f,
    20.0f,
    30.0f,
    40.0f,
    5.0f,
    5.0f,
    10.0f,
    10.0f,
    25.0f,
    25.0f,
    35.0f,
    35.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f,
    0.0f
};

void (*pnfPrg[])() = {
    ViewRead_,
    ViewAction_,
    ViewClip_
};
char ModeName[][16] = { "READ", "ACTION", "CLIP" };
qword_uni XYZ[4] = {};
qword_uni RGB[4] = {};
qword_uni UV[4] = {};
u8 *SfxDataBuff[4] = {};
sceVu0FVECTOR Trans = {};
sceVu0FVECTOR Rot = {};
char fname[32] = {};
s32 TypeNumMax = 0;
s32 ModelIndex = 0;
s32 TypeNum = 0;
VIEWOBJ vObj[4] = {};
char ReadFileName[64] = {};
char ChrNameTmp[64] = {};
BINH *pBinh = NULL;
s32 ViewMode = 0;
s32 ViewSubMode = 0;
s32 DirNum = 0;
s32 ChrNum = 0;
s16 *pSpecGim = NULL;
s32 ChrNum2 = 0;
f32 *pClip = NULL;