#include "common.h"
#include "view.h"
#include "motsys.h"

void SetKlWorldIp(SFXOBJ *pObj) {
    SetMotionWorldIp(pObj->pMot, klwipsp);
}

void Vu0EffProgTrans() {
    // TODO
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