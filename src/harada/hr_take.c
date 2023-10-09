#include "harada/hr_take.h"
#include "okanoyo/okio.h"
#include "take/object.h"
#include "take/motsys.h"
#include "take/motsys2.h"
#include "take/mimesys.h"

void hr_init_tobject(tOBJECT *tobj) {
    tobj->pLightColor = NULL;
    tobj->pNormalLight = NULL;
    tobj->Scale = 1.0f;
    tobj->ret = 0;
    tobj->OutLine = 0;
    tobj->Fuku = 0;
    tobj->Size = hKEI_L;
}

void hr_take_init() {
    u32 *addr;
    s32 i;

    addr = getBuff(1, 0x100000, "hr_take_init", &i);
    if ((s32)addr != -1) {
        SfxSystemInit(addr);
    }
}

void hr_take_initF() {
    Vu0ProgTrans();
    OutLineEnvInit(GameGbl.fr, GameGbl.inter);
}

void hr_take_setM() {
    sceVu0CopyMatrix(Scr.WvMtx, GameGbl.wvm);
    sceVu0CopyMatrix(Scr.VsMtx, GameGbl.vsm);
    sceVu0CopyMatrix(Scr.WsMtx, GameGbl.wsm);
}

HOT* hr_take_workGet() {
    HOT *ho;

    ho = getmem(sizeof(HOT));
    if ((s32)ho == -1) {
        ho = NULL;
    } else {
        memset(ho, 0, sizeof(HOT));
    }

    return ho;
}

void hr_take_workDel(HOT *ho) {
    if (ho == NULL) {
        return;
    }
    if (ho->pSfx != NULL) {
        EraseSfxObject(ho->pSfx);
    }
    freemem(ho);
}

void hr_take_modelinit(HOT *hot, u32 *addr) {
    hr_init_tobject(hot);
    hot->pSfx = SetSfxObject(addr);
}

void hr_take_modelinit2(HOT *hot, s32 id) {
    hr_init_tobject(hot);
    hot->pSfx = GetSfxObjectData(id);
}

void hr_take_motionPause(HOT *hot, s32 pause) {
    SetObjPause(hot->pSfx, pause);
}

s32 hr_take_modeldraw(HOT *hot, s32 mode) {
    s32 ret;
    SFXOBJ *sfx;

    sfx = hot->pSfx;
    if (hot->pNormalLight != NULL) {
        SetObjNormalLight(sfx, hot->pNormalLight);
    }
    if (hot->pLightColor != NULL) {
        SetObjLightColor(sfx, hot->pLightColor);
    }
    if (mode == 0) {
        SetObjPause(sfx, GameGbl.pause_flag);
    }
    ModelDraw(sfx);

    ret = GetActStopFlag(sfx);;
    hot->ret = ret;
    return ret;
}

s32 hr_take_modeldraw2(HOT *hot) {
    s32 ret;
    SFXOBJ *sfx;

    sfx = hot->pSfx;
    if (hot->pNormalLight != NULL) {
        SetObjNormalLight(sfx, hot->pNormalLight);
    }
    if (hot->pLightColor != NULL) {
        SetObjLightColor(sfx, hot->pLightColor);
    }
    SetObjPause(sfx, GameGbl.pause_flag);
    ModelDraw(sfx);

    ret = GetActStopFlag(sfx);;
    hot->ret = ret;
    return ret;
}

void hr_take_modeldrawMir(HOT *hot, s32 id) {
    SFXOBJ *sfx;

    sfx = GetSfxObjectData(id);
    SetSfxMotionSync(sfx, hot->pSfx);
    SetObjEffDraw(sfx);
    SetObjPause(sfx, 1);
    if (hot->pNormalLight != NULL) {
        SetObjNormalLight(sfx, hot->pNormalLight);
    }
    if (hot->pLightColor != NULL) {
        SetObjLightColor(sfx, hot->pLightColor);
    }
    ModelDraw(sfx);
    EraseSfxObject(sfx);
}

s16 hr_take_size(HOT *hot) {
    s16 size;

    switch (hot->Size) {
        case hKEI_L:
            size = 1;
            break;
        case hKEI_M:
            size = 2;
            break;
        case hKEI_S:
            size = 4;
            break;
        default:
            size = 8;
            break;
    }

    return size;
}

void hr_take_modelsizeCHG(HOT *hot, s32 size) {
    hot->Size = size;
    SetObjCondition(hot->pSfx, hr_take_size(hot));
}

void hr_take_modelscale(HOT *hot, f32 scale) {
    sceVu0FVECTOR v;

    v[0] = v[1] = v[2] = scale;
    v[3] = 0.0f;
    hot->Scale = scale;
    SetObjScale(hot->pSfx, v);
}

void hr_take_modelclip(HOT *hot, s32 flag) {
    SetObjClipFlag(hot->pSfx, flag);
}

void hr_take_motionSet(HOT *hot, s32 motno, s32 loop) {
    SFXOBJ *pSfx;

    pSfx = hot->pSfx;
    SetObjCondition(pSfx, hr_take_size(hot));
    SetActIp(hot->pSfx, motno);
}

void hr_take_motionSetN(HOT *hot, s32 motno, s32 loop) {
    SFXOBJ *pSfx;

    pSfx = hot->pSfx;
    SetObjCondition(pSfx, hr_take_size(hot));
    SetAct(hot->pSfx, motno);
}

void hr_take_GetLCMatYXZ(COLD *cd) {
    sceVu0FMATRIX work;

    sceVu0UnitMatrix(work);
    sceVu0RotMatrixZ(work, work, cd->Rot[2]);
    sceVu0RotMatrixX(work, work, cd->Rot[0]);
    sceVu0RotMatrixY(work, work, cd->Rot[1]);
    sceVu0TransMatrix(cd->LcMtx, work, cd->Trans);
}

s32 hr_take_motion(HOT *hot, s32 xyz) {
    if (xyz == 0) {
        hr_take_GetLCMatYXZ(&hot->Base);
    }
    SetBaseMatrix(hot->pSfx, hot->Base.Rot, hot->Base.Trans, 1);
    return hot->ret;
}

f32 hr_take_motionGetFMAX(HOT *hot) {
    return GetActEndCnt(hot->pSfx);
}

f32 hr_take_motionGetFrm(HOT *hot) {
    return GetActCnt(hot->pSfx);
}

void hr_take_motionSetFrm(HOT *hot, f32 frm) {
    SetActCnt(hot->pSfx, frm);
}

void hr_take_matrixtrans(HOT *hot, sceVu0FVECTOR v, s32 num) {
    GetObjMatrixTrans(hot->pSfx, num, v);
}

void hr_take_lips(HOT *hot, s16 *addr) {
    SetVoiceMime(hot->pSfx, addr);
}

void hr_take_lipsOFF(HOT *hot) {
    EraseVoiceMime(hot->pSfx);
}

void hr_take_rinkaku(HOT *hot) {
    SetObjOutlineOff(hot->pSfx);
}

void hr_take_shadow(HOT *hot, sceVu0FVECTOR pos) {
    abSetShadow(pos, hot->pSfx);
}
