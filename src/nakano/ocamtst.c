#include "nakano/ocamtst.h"
#include "nakano/gssub.h"
#include "nakano/camera.h"
#include "nakano/objfunc.h"
#include "nakano/route.h"

static void ObjDraw(OBJWORK *objw) {
    sceVu0FVECTOR vf0 = { 0.0f, 0.0f, 0.0f, 1.0f };
    sceVu0FVECTOR vf1 = { -32.0f, -64.0f, 0.0f, 1.0f };
    sceVu0FVECTOR vf2 = { 32.0f, -64.0f, 0.0f, 1.0f };
    sceVu0IVECTOR rgba = { 0x80, 0x20, 0xFF, 0x7F };

    objw->rot[1] += 0.049087375f;
    if (objw->rot[1] < -3.141592f) {
        objw->rot[1] += 6.283184f;
    } else if (objw->rot[1] > 3.141592f) {
        objw->rot[1] -= 6.283184f;
    }

    _nkDrawTTriangle(vf0, vf1, vf2, objw->posi, objw->rot, rgba);
}

static void ObjInit(OBJWORK *objw) {
    objw->draw = ObjDraw;
    objw->reg1 = 0;
    objw->bun0 += 1;
}

static void ObjMain(OBJWORK *objw) {
    OBJWORK *objklo = GameGbl.klonoa;

    if (objw->rtw.rtp == objklo->rtw.rtp && abs(objw->rtw.mcn - objklo->rtw.mcn) < 0x20000) {
        if (objw->reg1 == 0) {
            GameGbl.cam.mode = 7;
            objw->reg1 = (s32)objklo;
        }
    } else if (objw->reg1 == (s32)objklo) {
        CamRelease(120.0f);
        objw->reg1 = 0;
    }
}

static void (*ObjFuncTbl[])(OBJWORK *objw) = {
    ObjInit,
    ObjMain
};

void nkOCamTest(OBJWORK *objw) {
    ObjFuncTbl[objw->bun0](objw);
}

void nkSetOCamTest() {
    OBJWORK *objklo = GameGbl.klonoa;
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);

    if (objw != NULL) {
        objw->stat0 = 1;
        objw->stat1 = 3;
        objw->rtw = objklo->rtw;
        MichiPosi(&objw->rtw, objw->posi);
    }
}