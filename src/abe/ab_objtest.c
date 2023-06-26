#include "common.h"

static void DrawObjTest(OBJWORK *objw) {
    abGifPacket pack;
    sceVu0IVECTOR vi0;
    sceVu0IVECTOR vi1;
    sceVu0IVECTOR vi2;
    sceVu0FVECTOR vf0;
    sceVu0FVECTOR vf1;
    sceVu0FVECTOR vf2;
    sceVu0FMATRIX lmat;
    
    vf0[0] = 0.0f;
    vf0[1] = 0.0f;
    vf0[2] = 0.0f;
    vf0[3] = 1.0f;

    vf1[0] = 100.0f;
    vf1[1] = 0.0f;
    vf1[2] = 0.0f;
    vf1[3] = 1.0f;

    vf2[0] = 0.0f;
    vf2[1] = 100.0f;
    vf2[2] = 0.0f;
    vf2[3] = 1.0f;

    sceVu0UnitMatrix(lmat);
    sceVu0RotMatrixX(lmat, lmat, objw->rot[0]);
    sceVu0RotMatrixY(lmat, lmat, objw->rot[1]);
    sceVu0RotMatrixZ(lmat, lmat, objw->rot[2]);
    sceVu0TransMatrix(lmat, lmat, objw->posi);
    sceVu0MulMatrix(lmat, GameGbl.wsm, lmat);
    sceVu0RotTransPers(vi0, lmat, vf0, 1);
    sceVu0RotTransPers(vi1, lmat, vf1, 1);
    sceVu0RotTransPers(vi2, lmat, vf2, 1);

    pack.size = 0;
    pack.buf = abSPR;
    pack.buf->ul128 = 0;

    pack.buf[pack.size++].ul32[0] = 0x70000007;
    
    pack.buf[pack.size].ul64[0] = 0x2005c00000008003;
    pack.buf[pack.size++].ul64[1] = 0x51;
    
    pack.buf[pack.size].ul32[0] = 0xFF;
    pack.buf[pack.size].ul32[1] = 0x0;
    pack.buf[pack.size].ul32[2] = 0x0;
    pack.buf[pack.size].ul32[3] = 0x80;
    pack.size++;

    pack.buf[pack.size].ul32[0] = vi0[0];
    pack.buf[pack.size].ul32[1] = vi0[1];
    pack.buf[pack.size].ul32[2] = vi0[2];
    pack.buf[pack.size].ul32[3] = 0;
    pack.size++;

    pack.buf[pack.size].ul32[0] = 0x0;
    pack.buf[pack.size].ul32[1] = 0xFF;
    pack.buf[pack.size].ul32[2] = 0x0;
    pack.buf[pack.size].ul32[3] = 0x80;
    pack.size++;

    pack.buf[pack.size].ul32[0] = vi1[0];
    pack.buf[pack.size].ul32[1] = vi1[1];
    pack.buf[pack.size].ul32[2] = vi1[2];
    pack.buf[pack.size].ul32[3] = 0;
    pack.size++;

    pack.buf[pack.size].ul32[0] = 0x0;
    pack.buf[pack.size].ul32[1] = 0x0;
    pack.buf[pack.size].ul32[2] = 0xFF;
    pack.buf[pack.size].ul32[3] = 0x80;
    pack.size++;

    pack.buf[pack.size].ul32[0] = vi2[0];
    pack.buf[pack.size].ul32[1] = vi2[1];
    pack.buf[pack.size].ul32[2] = vi2[2];
    pack.buf[pack.size].ul32[3] = 0;
    pack.size++;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

static void ObjInit(OBJWORK *objw) {
    objw->reg0 = 0;
    objw->draw = DrawObjTest;
    objw->bun0 = 1;
}

static void ObjMain(OBJWORK *objw) {
    objw->posi[0] += objw->spd[0];
    objw->posi[1] += objw->spd[1];
    
    if (objw->posi[0] < -1000.0f || objw->posi[0] > 1000.0f) {
        objw->spd[0] = -objw->spd[0];
    }

    if (objw->posi[1] < -1000.0f || objw->posi[1] > 1000.0f) {
        objw->spd[1] = -objw->spd[1];
    }

    objw->rot[0] += M_PIf / 100.0f;
    objw->rot[1] += M_TWOPIf / 100.0f;
    objw->rot[2] += 3 * M_PIf / 10.0f;

    if (objw->rot[0] < -M_PIf)
        objw->rot[0] += M_TWOPIf;
    else if (objw->rot[0] > M_PIf)
        objw->rot[0] -= M_TWOPIf;

    if (objw->rot[1] < -M_PIf)
        objw->rot[1] += M_TWOPIf;
    else if (objw->rot[1] > M_PIf)
        objw->rot[1] -= M_TWOPIf;

    if (objw->rot[2] < -M_PIf)
        objw->rot[2] += M_TWOPIf;
    else if (objw->rot[2] > M_PIf)
        objw->rot[2] -= M_TWOPIf;
}

static void (*ObjFuncTbl[2])(OBJWORK *objw) = {
    ObjInit,
    ObjMain
};

void abObjTest(OBJWORK *objw) {
    ObjFuncTbl[objw->bun0](objw);
}

void abSetObjTest(sceVu0FVECTOR vf) {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);

    objw->stat0 = 6;
    objw->stat1 = 1;
    objw->bun0 = 0;
    objw->posi[0] = vf[0];
    objw->posi[1] = vf[1];
    objw->posi[2] = vf[2];
    objw->spd[0] = 4.0f;
    objw->spd[1] = -6.0f;
    objw->rot[0] = 0.0f;
    objw->rot[1] = 0.0f;
    objw->rot[2] = 0.0f;
}
