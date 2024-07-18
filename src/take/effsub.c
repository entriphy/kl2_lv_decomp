#include "take/effsub.h"
#include "take/view.h"
#include "take/motsys.h"
#include "take/object.h"

typedef struct { // 0xc
    /* 0x0 */ SFXOBJ *pObj;
    /* 0x4 */ s32 field_0x4;
    /* 0x8 */ u32 fr;
} D_395CD8_struct;

static s32 D_3FBCD8 = 0;
static D_395CD8_struct D_395CD8[25] = {};

extern u32 Vu0PrgEff __attribute__((section(".vutext")));

void func_00205E38(SFXOBJ *pObj) {
    u32 var_a3 = GameGbl.fr - 1;
    s32 i;

    for (i = 0; i < 25 && D_395CD8[i].fr != GameGbl.fr; i++);
    if (i == 25) {
        D_3FBCD8 = 0;
        for (i = 0; i < 25; i++) {
            if (D_395CD8[i].pObj != NULL && D_395CD8[i].fr != var_a3 && D_395CD8[i].fr != GameGbl.fr) {
                D_395CD8[i].pObj = NULL;
            }
        }
    }

    for (i = 0; i < 25; i++) {
        if (D_395CD8[i].pObj == pObj && D_395CD8[i].fr == var_a3) {
            D_395CD8[i].fr = GameGbl.fr;
            D_3FBCD8 += pObj->DrawWorkCnt;
            return;
        }
    }

    for (i = 0; i < 25; i++) {
        if (D_395CD8[i].pObj == NULL) {
            D_395CD8[i].pObj = pObj;
            D_395CD8[i].fr = GameGbl.fr;
            D_3FBCD8 += pObj->DrawWorkCnt;
            break;
        }
    }
}

void SetKlWorldIp(SFXOBJ *pObj) {
    SetMotionWorldIp(pObj->pMot, klwipsp);
}

void Vu0EffProgTrans() {
    sceDmaSync(pDma.Vif0, 0, 0);
    DPUT_D0_QWC(0);
    DPUT_D0_TADR((u32)&Vu0PrgEff & 0xFFFFFFF);
    DPUT_D_STAT(1);
    FlushCache(WRITEBACK_DCACHE);
    DPUT_D0_CHCR(0x145);
    sceDmaSync(pDma.Vif0, 0, 0);
}

f32 _Sqrt(f32 x) {
    f32 ret;

    __asm__ volatile(
        "mfc1 $6, %0\n"
        "qmtc2 $6, $vf4\n"
        "vsqrt Q, $vf4x\n"
        "vwaitq\n"
        "cfc2 $6, $vi22\n"
        "mtc1 $6, %1"
    : : "f" (x), "f" (ret));

    return ret;
}
