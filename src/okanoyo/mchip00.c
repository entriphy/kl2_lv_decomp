#include "okanoyo/mchip00.h"
#include "okanoyo/okio.h"
#include "okanoyo/okzdraw.h"
#include "okanoyo/okzminit.h"
#include "nakano/objfunc.h"
#include "take/object.h"

void OkChipDel(OBJWORK *objw) {
    s32 i;
    ZM_WORK *zmw = (ZM_WORK *)objw->reg5;

    for (i = 0; i < objw->reg4; i++) {
        EraseSfxObject(zmw->hOp->pSfx);
        EraseSfxObject(zmw->hOp->pMir);
        if (zmw->hOp != NULL) { // didn't you already dereference hOp lol
            freemem(zmw->hOp);
        }
        zmw++;
    }

    freemem((void *)objw->reg5);
}

void OkSetChip(OBJWORK *oya, s32 type) {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    ZAKO_WORK *zakow;

    if (objw != NULL) {
        InitObjWork(objw);
        objw->stat0 = 2;
        objw->stat1 = 0x4080;
        objw->work = getmem(sizeof(ZAKO_WORK));
        objw->prim = getmem(0x100);
        objw->reg0 = type;
        objw->posi[0] = oya->posi[0];
        objw->posi[1] = oya->posi[1];
        objw->posi[2] = oya->posi[2];
        objw->posi[3] = 1.0f;

        zakow = objw->work;
        zakow->motno = 0;
        objw->option = 0;
        objw->htblid = (-(s64)(oya->option + 1) << 16) | objw->reg2;
        zakow->xzdis = 2048;
        zakow->ydis = 2048;
        zakow->flag = 0;
        objw->reg6 = oya->reg6;
        objw->reg7 = oya->reg7;
    }
}

void M_Chip00(OBJWORK *objw) {
    s32 i;
    ZAKO_COMMON *zakow = objw->work;
    f32 xmov[4] = { 1.0f, 1.0f, -1.0f, -1.0f };
    f32 zmov[4] = { 1.0f, -1.0f, 1.0f, -1.0f };
    ZM_WORK *zmw;

    if (objw->bun0 == 0) {
        switch (objw->reg0) {
            case 20:
            case 21:
            case 31:
            case 32:
                objw->reg4 = 4;
                break;
            case 12:
            case 13:
            case 45:
            default:
                objw->reg4 = 1;
                break;
        }
        objw->reg5 = (s32)getmem(objw->reg4 * sizeof(ZM_WORK));

        zmw = (ZM_WORK *)objw->reg5;
        for (i = 0; i < objw->reg4; i++) {
            zmw->hOp = NULL;
            switch (objw->reg0) {
                case 31:
                    OkModelSetNT(objw, 243 + i, &zmw->hOp);
                    OkTObjScaleSet(zmw->hOp, 3.0f);
                    zmw->vec[1] = -10.0f;
                    zmw->vec[0] = xmov[i] * 6.0f;
                    zmw->vec[2] = zmov[i] * 6.0f;
                    zmw->rot[3] = M_PI_8f;
                    break;
                case 20:
                    OkModelSetNT(objw, 243 + i, &zmw->hOp);
                    zmw->vec[1] = -10.0f;
                    zmw->vec[0] = xmov[i] * 2.0f;
                    zmw->vec[2] = zmov[i] * 2.0f;
                    zmw->rot[3] = M_PI_8f;
                    break;
                case 21:
                    OkModelSetNT(objw, 247 + i, &zmw->hOp);
                    zmw->vec[1] = -10.0f;
                    zmw->vec[0] = xmov[i] * 2.0f;
                    zmw->vec[2] = zmov[i] * 2.0f;
                    zmw->rot[3] = M_PI_8f;
                    break;
                case 32:
                    OkModelSetNT(objw, 247 + i, &zmw->hOp);
                    OkTObjScaleSet(zmw->hOp, 3.0f);
                    zmw->vec[1] = -10.0f;
                    zmw->vec[0] = xmov[i] * 6.0f;
                    zmw->vec[2] = zmov[i] * 6.0f;
                    zmw->rot[3] = M_PI_8f;
                    break;
                case 12:
                case 13:
                    OkModelSetNT(objw, 254, &zmw->hOp);
                    zmw->vec[1] = -6.0f;
                    zmw->vec[0] = xmov[i] * 4.0f;
                    zmw->vec[2] = zmov[i] * 4.0f;
                    zmw->rot[3] = M_PI_4f;
                    break;
                case 22:
                    OkModelSetNT(objw, 128, &zmw->hOp);
                    zmw->vec[1] = -12.0f;
                    zmw->vec[0] = xmov[i] * 2.0f;
                    zmw->vec[2] = zmov[i] * 2.0f;
                    zmw->rot[3] = M_PI_4f;
                    break;
                case 45:
                    OkModelSetNT(objw, 134, &zmw->hOp);
                    zmw->vec[1] = -12.0f;
                    zmw->vec[0] = xmov[i] * 2.0f;
                    zmw->vec[2] = zmov[i] * 2.0f;
                    zmw->rot[3] = M_PI_4f;
                    break;
                default:
                    OkModelSetNT(objw, 129, &zmw->hOp);
                    zmw->vec[1] = -14.0f;
                    zmw->vec[0] = xmov[i] * 3.0f;
                    zmw->vec[2] = zmov[i] * 2.0f;
                    zmw->rot[3] = M_PI_4f;
                    break;
            }

            COPYVECTOR(zmw->pos, objw->posi);
            SETVECTOR(zmw->rot, 0.0f, 0.0f, 0.0f, 1.0f);
            zmw++;
        }

        objw->draw = OkDrawGeneralN2T;
        objw->bun0++;
        zakow->timer = 0;
    }

    zmw = (ZM_WORK *)objw->reg5;
    for (i = 0; i < objw->reg4; i++) {
        zmw->pos[0] += zmw->vec[0];
        zmw->pos[2] += zmw->vec[2];
        zmw->pos[1] += zmw->vec[1];
        zmw->vec[1] += 0.5;
        zmw->rot[0] += 0.049087375f;
        zmw->rot[1] += 0.049087375f;
        NORMRAD(zmw->rot[0]);
        NORMRAD(zmw->rot[1]);
        NORMRAD(zmw->rot[2]);
        zmw++;
    }

    if (zakow->timer > 60) {
        OkKillObj(objw, OkChipDel);
    } else {
        zakow->timer++;
    }
}