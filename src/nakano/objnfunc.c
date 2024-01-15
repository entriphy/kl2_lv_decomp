#include "nakano/objnfunc.h"
#include "nakano/objkazd.h"
#include "nakano/objklo0.h"
#include "nakano/ocamtst.h"
#include "nakano/popuka.h"
#include "nakano/popeff.h"

static void (*ObjJTbl[27])(OBJWORK *objw) = {
    NULL,
    ObjKlo,
    ObjKazd,
    nkOCamTest,
    NULL,
    NULL, // ObjGimTrain
    NULL, // ObjGimViking
    NULL, // ObjGimBridgeB7
    NULL, // ObjGimS2Bridge
    NULL, // ObjGimGpDoor0
    NULL, // ObjGimGpDoor1
    NULL, // ObjGimDoor04Pazzle
    NULL, // ObjSpotLight
    NULL, // ObjGimTest2
    NULL, // SmplMyu
    nkPopuka, // nkPopuka
    NULL, // nkFilterCtrlObj
    NULL,
    NULL, // ObjGimPendulum
    NULL, // ObjGimBigRock
    NULL, // ObjKuLight
    NULL, // ObjGusSystem
    NULL, // ObjEngSystem
    NULL, // ObjGimRtDoor0
    NULL, // ObjGimRtDoor1
    nkPopFlear, // nkPopFlear
    NULL, // nkYume150Flear
};

void ObjNFunc(OBJWORK *objw) {
    if (objw->stat1 != 0) {
        ObjJTbl[objw->stat1](objw);
    }
}
