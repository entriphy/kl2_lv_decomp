#include "nakano/nkmirr.h"
#include "nakano/gssub.h"
#include "nakano/dma.h"
#include "nakano/drawof.h"
#include "harada/hr_mirr.h"
#include "take/object.h"
#include "harada/hr_mapdr.h"

static sceVu0FMATRIX mrvsm;
static sceVu0FMATRIX mrwvm;
static sceVu0FMATRIX mrwsm;

s32 nkMirClip(sceVu0FVECTOR pos) {
    return h_vpo_vclip(&hfmcam.vco, pos, hfmcam.wvm);
}

void nkMirPushMatrix() {
    sceVu0CopyMatrix(mrvsm, GameGbl.vsm);
    sceVu0CopyMatrix(mrwvm, GameGbl.wvm);
    sceVu0CopyMatrix(mrwsm, GameGbl.wsm);
    sceVu0CopyMatrix(GameGbl.vsm, hfmcam.vsm);
    sceVu0CopyMatrix(GameGbl.wvm, hfmcam.wvm);
    sceVu0CopyMatrix(GameGbl.wsm, hfmcam.wsm);
}

void nkMirPopMatrix() {
    sceVu0CopyMatrix(GameGbl.vsm, mrvsm);
    sceVu0CopyMatrix(GameGbl.wvm, mrwvm);
    sceVu0CopyMatrix(GameGbl.wsm, mrwsm);
}

// TODO: fix
s32 nkGetMirrFlag() {
    return hfmircnt > 0 || hcmircnt > 0;
}

void nkMirDraw() {
    if (nkGetMirrFlag() && hfmcam.type != 0) {
        nkGsSetEffectFZ2();
        hrDrawMir(0);
        nkPathFlush();
        nkMirPushMatrix();
        sceVu0CopyMatrix(Scr.WvMtx, hfmcam.wvm);
        sceVu0CopyMatrix(Scr.VsMtx, hfmcam.vsm);
        sceVu0CopyMatrix(Scr.WsMtx, hfmcam.wsm);
        Vu0ProgTrans();
        DrMirObjFunc(SysGbl.objwork);
        sceGsSyncPath(0,0);
        abMirrorBegin();
        DrMirEffObjFunc(SysGbl.objwork);
        abMirrorEnd();
        nkMirPopMatrix();
        nkGsSetNormalFZ2();
        hrDrawMirT();
    }
}
