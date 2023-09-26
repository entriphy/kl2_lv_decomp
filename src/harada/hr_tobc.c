#include "harada/hr_tobc.h"
#include "harada/hr_take.h"
#include "take/motsys.h"

// NOTE: all s32* types were changed to HOT*
// No idea why this dev originally used s32*

void hr_to_modeldraw(HOT *hobj) {
    Vu0ProgTrans();
    hr_take_setM();
    hr_take_modeldraw2(hobj);
}

void hr_to_modeldrawRL(HOT *hobj, s32 no) {
    sceVu0FMATRIX nm;
    sceVu0FMATRIX cm;

    Vu0ProgTrans();
    nkSetLightNo(nm, cm, no, hobj->Base.Trans);
    hobj->pNormalLight = &nm;
    hobj->pLightColor = &cm;
    hr_take_setM();
    hr_take_modeldraw2((HOT *)hobj);
}

void hr_to_modeldrawF(HOT *hobj) {
    Vu0ProgTrans();
    hr_take_setM();
    hr_take_rinkaku(hobj);
    hr_take_modeldraw2(hobj);
}

void hr_to_motionset(HOT *hobj, s32 motno, s32 loop) {
    hr_take_motionSet(hobj, motno, loop);
}

void hr_to_motionpause(HOT *hobj, s32 pause) {
    hr_take_motionPause(hobj, pause);
}

HOT* hr_to_workget() {
    return hr_take_workGet();
}

s32 hr_to_motion(HOT *hobj, s32 xyz) {
    return hr_take_motion(hobj, xyz);
}

void hr_to_workdel(HOT *hobj) {
    hr_take_workDel(hobj);
}

void hr_to_modelinit2(HOT *hobj, s32 id) {
    hr_take_modelinit2(hobj, id);
}

void hr_to_setpos(HOT *hobj, f32 px, f32 py, f32 pz) {
    hobj->Base.Trans[0] = px;
    hobj->Base.Trans[1] = py;
    hobj->Base.Trans[2] = pz;
    hobj->Base.Trans[3] = 1.0f;
}

void hr_to_setrot(HOT *hobj, f32 rx, f32 ry, f32 rz) {
    hobj->Base.Rot[0] = rx;
    hobj->Base.Rot[1] = ry;
    hobj->Base.Rot[2] = rz;
    hobj->Base.Rot[3] = 1.0f;
}

void hr_to_setscale(HOT *hobj, f32 scale) {
    hr_take_modelscale(hobj, scale);
}

void hr_to_modelclip(HOT *hobj) {
    hr_take_modelclip(hobj, 0);
}

f32 hr_to_motionGFMAX(HOT *hobj) {
    return hr_take_motionGetFMAX(hobj);
}

void hr_to_motionSFRM(HOT *hobj, f32 frm) {
    hr_take_motionSetFrm(hobj, frm);
}

void hr_set_hobject2(HOT *hObj, HR_HOBJ *hh) {
    hr_take_modelinit2(hObj, hh->fhmno);
    hObj->pNormalLight = NULL;
    hObj->pLightColor = NULL;
    hr_to_motionset(hObj, hh->motno, 0);
    hObj->Base.Trans[0] = hh->px;
    hObj->Base.Trans[1] = hh->py;
    hObj->Base.Trans[2] = hh->pz;
    hObj->Base.Trans[3] = 1.0f;
    hObj->Base.Rot[0] = hh->rx;
    hObj->Base.Rot[1] = hh->ry;
    hObj->Base.Rot[2] = hh->rz;
    hObj->Base.Rot[3] = 1.0f;
}

void hr_set_hobject3(HR_OBJW *ow) {
    HOT *h;

    h = ow->hobj;
    hr_take_modelscale(h, 6.4f);
    nkSetLightNo(ow->pnl, ow->plc,0, h->Base.Trans);
    h->pNormalLight = &ow->pnl;
    h->pLightColor = &ow->plc;
    hr_take_modelclip(h, 0);
    ow->count = 0;
}