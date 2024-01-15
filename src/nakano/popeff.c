#include "nakano/popeff.h"
#include "nakano/klmot.h"
#include "nakano/k_math.h"
#include "nakano/objfunc.h"
#include "take/object.h"

extern void abDrawFlaer(sceVu0FVECTOR posi, f32 scale, sceVu0IVECTOR c0, sceVu0IVECTOR c1, f32 priolity);

static void DrawObjEff(OBJWORK *objw) {
    sceVu0FVECTOR vf;
    sceVu0IVECTOR stack_0x50 = { 0x81, 0x81, 0x81, 0xCF };
    sceVu0IVECTOR stack_0x40 = { 0x00, 0x00, 0x80, 0x00 };
    SFXOBJ *pObj = nkGetKloPSFXOBJ();
    f32 scale;

    GetObjMatrixTrans(pObj, 42, vf);
    vf[3] = 1.0f;
    scale = _sin(objw->freg0 * 3.141592f);
    abDrawFlaer(vf, scale * 30.0f, stack_0x50, stack_0x40, -30.0f);
}

static void ObjInit(OBJWORK *objw) {
    objw->draw = NULL;
    objw->drmir = NULL;
    objw->drmiref = DrawObjEff;
    objw->dreff = DrawObjEff;
    objw->freg0 = 0.0f;
    objw->bun0++;
}

static void ObjMain(OBJWORK *objw) {
    objw->freg0 += 1.0f / 30.0f;
    if (objw->freg0 >= 1.0f) {
        InitObjWork(objw);
    }
}

static void (*ObjFuncTbl[])(OBJWORK *objw) = {
    ObjInit,
    ObjMain
};

void nkPopFlear(OBJWORK *objw) {
    ObjFuncTbl[objw->bun0](objw);
}

void nkSetPopFlear() {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    if (objw != NULL) {
        InitObjWork(objw);
        objw->stat0 = 1;
        objw->stat1 = 25;
    }
}
