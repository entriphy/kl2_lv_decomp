#include "okanoyo/okzminit.h"
#include "okanoyo/okio.h"
#include "take/object.h"

void OkTObjScaleSet(tOBJECT2 *hOp, f32 scale) {
    SFXOBJ *sfx;
    sceVu0FVECTOR f;

    f[3] = 1.0f;
    f[0] = f[1] = f[2] = scale;
    SetObjScale(hOp->pSfx, f);
}

void OkTObjScaleSet2(tOBJECT2 *hOp, sceVu0FVECTOR scale) {
    SetObjScale(hOp->pSfx, scale);
}

void OkTObjScaleUp(tOBJECT2 *hOp, f32 target, f32 div) {
    // Empty function
}

s32 OkTObjModelSetGeneral(OBJWORK *objw, tOBJECT2 **hOp, s32 p_id, void *Draw, void *Draw2) {
    if (Draw != NULL) {
        objw->draw = Draw;
    }
    if (Draw2 != NULL) {
        objw->drmir = Draw2;
    }

    if (*hOp != NULL) {
        return 1;
    }

    *hOp = getmem(0x10);
    if ((s32)*hOp == -1) {
        *hOp = NULL;
    }
    if (*hOp == NULL) {
        return -1;
    }
    (*hOp)->pSfx = NULL;
    (*hOp)->pMir = NULL;

    if (OktModelInit(*hOp, p_id) == -1) {
        tOBJECT2 *tOp = *hOp;
        if (tOp->pSfx != NULL) {
            EraseSfxObject(tOp->pSfx);
        }
        if (tOp->pMir != NULL) {
            EraseSfxObject(tOp->pMir);
        }
        freemem(*hOp);
        *hOp = NULL;
        return -1;
    }

    OkTObjScaleSet(*hOp, OkZakoDefScale(p_id));
    return (s32)*hOp;
}
