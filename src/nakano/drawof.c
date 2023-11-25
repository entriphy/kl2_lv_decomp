#include "nakano/drawof.h"

void DrawObjFunc(OBJWORK *objwtop) {
    s32 i;
    OBJWORK *objw;

    for (i = 0; i < 128; i++) {
        objw = &objwtop[i];
        if (objw->draw != NULL && objw->stat0 != 0) {
            objw->draw(objw);
        }
    }
}

void DrMirObjFunc(OBJWORK *objwtop) {
    s32 i;
    OBJWORK *objw;

    for (i = 0; i < 128; i++) {
        objw = &objwtop[i];
        if (objw->drmir != NULL && objw->stat0 != 0) {
            objw->drmir(objw);
        }
    }
}

void DrMirEffObjFunc(OBJWORK *objwtop) {
    s32 i;
    OBJWORK *objw;

    for (i = 0; i < 128; i++) {
        objw = &objwtop[i];
        if (objw->drmiref != NULL && objw->stat0 != 0) {
            objw->drmiref(objw);
        }
    }
}

void DrEffObjFunc(OBJWORK *objwtop) {
    s32 i;
    OBJWORK *objw;

    for (i = 0; i < 128; i++) {
        objw = &objwtop[i];
        if (objw->dreff != NULL && objw->stat0 != 0) {
            objw->dreff(objw);
        }
    }
}