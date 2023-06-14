#include "common.h"

static void ObjDraw(OBJWORK *objw) {
    abJigenDraw(objw, (JIGEN_WORK *)objw->work);
}

static void ObjInit(OBJWORK *objw) {
    // Empty function
}

static void ObjMain(OBJWORK *objw) {
    // Empty function
}

static void ObjDelete(OBJWORK *objw) {
    freemem(objw->work);
}

static void (*ObjFuncTbl[3])(OBJWORK *objw) = {
    ObjInit,
    ObjMain,
    ObjDelete
};

void abObjDummy(OBJWORK *objw) {
    ObjFuncTbl[objw->bun0](objw);
    abJigenAnime(objw, (JIGEN_WORK *)objw->work);
}

void abSetDummy(sceVu0FVECTOR vf) {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    if (objw != NULL) {
        objw->work = getmem(sizeof(JIGEN_WORK) * 0x10);
        ((JIGEN_WORK *)objw->work)->bun0 = 0;
        objw->stat0 = 6;
        objw->stat1 = 21;
        objw->bun0 = 1;
        objw->posi[0] = vf[0];
        objw->posi[1] = vf[1];
        objw->posi[2] = vf[2];
        objw->posi[3] = vf[3];
        objw->rot[0] = objw->rot[1] = objw->rot[2] = objw->rot[3] = 0.0f;
        objw->spd[0] = 0.0f;
        objw->spd[1] = 0.0f;
        objw->spd[2] = -1.0f;
        objw->draw = NULL;
        objw->drmir = NULL;
        objw->dreff = ObjDraw;
    }
}

void abInitDummy(u8 *buffer) {
    // Empty function
}
