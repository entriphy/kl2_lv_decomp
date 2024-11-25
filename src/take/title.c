#include <eetypes.h>
#include <libdev.h>
#include "take/title.h"
#include "nakano/dma.h"
#include "nakano/map00.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"
#include "okanoyo/okprint.h"
#include "take/camera.h"
#include "take/motsys.h"
#include "take/object.h"
#include "take/sfxbios.h"

s32 (*tkTitleFuncTbl[])() = {

};
qword_uni LogoGsEnv[64] = {};
int LogoGsEnvInd = 0;
qword_uni Kumo1GsEnv[64] = {};
int Kumo1GsEnvInd = 0;
qword_uni Kumo2GsEnv[64] = {};
int Kumo2GsEnvInd = 0;
void *pLogo = NULL;
void *pKumo1 = NULL;
void *pKumo2 = NULL;
kPadDATA *kpd0;
kPadDATA *kpd1;
sceVu0FVECTOR SkyRot;
sceVu0FVECTOR DbCameraRot;
sceVu0FVECTOR DbCameraTra;

void SfxInitDebugCamera() {
    sceVu0FMATRIX TmpMtx;

    SETVEC(DbCameraRot, 0.0f, 0.0f, 0.0f, 0.0f);
    SETVEC(DbCameraTra, 0.0f, 0.0f, 0.0f, 1.0f);
    sceVu0UnitMatrix(TmpMtx);
    sceVu0RotMatrix(TmpMtx, TmpMtx, DbCameraRot);
    sceVu0RotMatrix(TmpMtx, TmpMtx, DbCameraRot); // why are you calling this twice
    sceVu0TransMatrix(TmpMtx, TmpMtx, DbCameraTra);
    GetRotTransMatrixZYX(TmpMtx, DbCameraRot, DbCameraTra);
    sceVu0InversMatrix(Scr.WvMtx, TmpMtx);
    sceVu0MulMatrix(Scr.WsMtx, Scr.VsMtx, Scr.WvMtx);
}

void SfxDebugCameraWork() {
	sceVu0FMATRIX TmpMtx;

    if (!(GameGbl.kpd[1].lvl & 0xF0FA)) { // If you || all of the macros below, compiler does ld instead of lw
        return;
    }

    if (PAD_LVL_CROSS(GameGbl.kpd[1])) {
        DbCameraRot[0] += M_PI / 60.0;
        if (DbCameraRot[0] > M_PI) {
            DbCameraRot[0] -= M_PI * 2.0;
        }
    }
    if (PAD_LVL_TRIANG(GameGbl.kpd[1])) {
        DbCameraRot[0] -= M_PI / 60.0;
        if (DbCameraRot[0] < -M_PI) {
            DbCameraRot[0] += M_PI * 2.0;
        }
    }
    if (PAD_LVL_CIRCLE(GameGbl.kpd[1])) {
        DbCameraRot[1] += M_PI / 60.0;
        if (DbCameraRot[1] > M_PI) {
            DbCameraRot[1] -= M_PI * 2.0;
        }
    }
    if (PAD_LVL_SQUARE(GameGbl.kpd[1])) {
        DbCameraRot[1] -= M_PI / 60.0;
        if (DbCameraRot[1] < -M_PI) {
            DbCameraRot[1] += M_PI * 2.0;
        }
    }

    if (PAD_LVL_DOWN(GameGbl.kpd[1])) {
        DbCameraTra[1] += 1.0;
    }
    if (PAD_LVL_UP(GameGbl.kpd[1])) {
        DbCameraTra[1] -= 1.0;
    }
    if (PAD_LVL_RIGHT(GameGbl.kpd[1])) {
        DbCameraTra[0] += 1.0;
    }
    if (PAD_LVL_LEFT(GameGbl.kpd[1])) {
        DbCameraTra[0] -= 1.0;
    }
    if (PAD_LVL_R1(GameGbl.kpd[1])) {
        DbCameraTra[2] -= 1.0;
    }
    if (PAD_LVL_R2(GameGbl.kpd[1])) {
        DbCameraTra[2] += 1.0;
    }

    GetRotTransMatrixZYX(TmpMtx, DbCameraRot, DbCameraTra);
    sceVu0InversMatrix(Scr.WvMtx, TmpMtx);
    sceVu0MulMatrix(Scr.WsMtx, Scr.VsMtx, Scr.WvMtx);
}

void func_00217498() {
    if (!(GameGbl.kpd[1].lvl & 0xF0FA)) {
        return;
    }

    if (PAD_LVL_CROSS(GameGbl.kpd[1])) {
        SkyRot[0] += M_PI / 60.0;
        if (SkyRot[0] > M_PI) {
            SkyRot[0] -= M_PI * 2.0;
        }
    }
    if (PAD_LVL_TRIANG(GameGbl.kpd[1])) {
        SkyRot[0] -= M_PI / 60.0;
        if (SkyRot[0] < -M_PI) {
            SkyRot[0] += M_PI * 2.0;
        }
    }
    if (PAD_LVL_CIRCLE(GameGbl.kpd[1])) {
        SkyRot[1] += M_PI / 60.0;
        if (SkyRot[1] > M_PI) {
            SkyRot[1] -= M_PI * 2.0;
        }
    }
    if (PAD_LVL_SQUARE(GameGbl.kpd[1])) {
        SkyRot[1] -= M_PI / 60.0;
        if (SkyRot[1] < -M_PI) {
            SkyRot[1] += M_PI * 2.0;
        }
    }
}

// int TitleInit() {
// 	/* -0x50(sp) */ s32 ret;

//     SysGbl.objwork = malloc(sizeof(OBJWORK) * 128);
//     SysGbl.n_objw = 128;
//     InitAllObjWork(SysGbl.objwork, 128);
//     sceGsResetPath();
//     sceDevGifReset();
//     sceDevVif1Reset();
//     sceDevVu1Reset();
//     sceDevVif0Reset();
//     sceDevVu0Reset();
//     sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
//     sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ16S, SCE_GS_CLEAR);
//     FlushCache(WRITEBACK_DCACHE);
//     nkInitDma();
//     OkSetIDprofile();
//     OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 12, 6);
//     OkPrintSysInit(7, SCR_WIDTH, SCR_HEIGHT, 12, 6);
//     memoryStageFormat();
//     DataBuffPtr = getBuff(1, 0x100000, NULL, &ret);
//     pDma.Vif0 = sceDmaGetChan(DMAC_VIF0);
//     pDma.Vif1 = sceDmaGetChan(DMAC_VIF1);
//     pDma.Gif = sceDmaGetChan(DMAC_GIF);
//     pDma.ToSpr = sceDmaGetChan(DMAC_TO_SPR);
//     pDma.FromSpr = sceDmaGetChan(DMAC_FROM_SPR);
//     ScreenInit();
//     LightInit();
//     SfxInitDebugCamera();
//     nkPathReset();
//     nkInitAtrMain();
//     sceGsSyncPath(0,0);
//     nkDebFlush();
//     nkPathFlush();
//     nkPathReset();
//     GetWsMtx();
//     iVar1 = ReadFile("host0:../../dat/st30/seltitle.gms",(int *)DataBuffPtr);
//     pLogo = DataBuffPtr;
//     DataBuffPtr = (uint *)((int)DataBuffPtr + iVar1 + 0xf & 0xfffffff0);
//     iVar1 = ReadFile("host0:../../dat/st30/sellogo.gms",(int *)DataBuffPtr);
//     pLogo = DataBuffPtr;
//     DataBuffPtr = (uint *)((int)DataBuffPtr + iVar1 + 0xf & 0xfffffff0);
//     iVar1 = ReadFile("host0:../../dat/st30/selkumo1.gms",(int *)DataBuffPtr);
//     pKumo1 = DataBuffPtr;
//     DataBuffPtr = (uint *)((int)DataBuffPtr + iVar1 + 0xf & 0xfffffff0);
//     iVar1 = ReadFile("host0:../../dat/st30/selkumo2.gms",(int *)DataBuffPtr);
//     pKumo2 = DataBuffPtr;
//     DataBuffPtr = (uint *)((int)DataBuffPtr + iVar1 + 0xf & 0xfffffff0);
//     sceDmaSync(pDma.Gif,0,0);
//     puVar2 = Fadr((uint *)pLogo,0);
//     sceDmaSend(pDma.Gif,puVar2);
//     sceDmaSync(pDma.Gif,0,0);
//     puVar2 = Fadr((uint *)pKumo1,0);
//     sceDmaSend(pDma.Gif,puVar2);
//     sceDmaSync(pDma.Gif,0,0);
//     puVar2 = Fadr((uint *)pKumo2,0);
//     sceDmaSend(pDma.Gif,puVar2);
//     sceDmaSync(pDma.Gif,0,0);
//     TitleLogoInit();
//     CircleInit();
//     KumoInit();
//     SysGbl.smode++;
// }