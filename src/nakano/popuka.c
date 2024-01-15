#include "nakano/popuka.h"
#include "nakano/light.h"
#include "nakano/klmot.h"
#include "nakano/k_math.h"
#include "nakano/vu0mac.h"
#include "nakano/popeff.h"
#include "nakano/route.h"
#include "nakano/objfunc.h"
#include "harada/hr_pall.h"
#include "hoshino/h_gamesnd.h"
#include "take/object.h"
#include "take/motsys.h"
#include "okanoyo/okio.h"

static void DrawObjMir(OBJWORK *objw) {
    OBJWORK *objklo = GameGbl.klonoa;
    sceVu0FMATRIX *nl;
    sceVu0FMATRIX *lc;

    if (hr_pt_check() != 1) {
        PsfxMODEL *reg2 = (PsfxMODEL *)objw->reg2;
        SFXOBJ *pObj = reg2->pObj;
        SetSfxMotionSync(pObj, ((PsfxMODEL *)objw->prim)->pObj);
        pObj->Condition = 4;
        SetObjPause(pObj, 1);
        nkSetLight2(&objklo->rtw, objw->posi, &nl, &lc);
        SetObjNormalLight(pObj, nl);
        SetObjLightColor(pObj, lc);
        ModelDraw(pObj);
    }
}

static void DrawObj(OBJWORK *objw) {
    SFXOBJ *pObj;
    SupWORK *supw;
    PsfxMODEL *model;
    OBJWORK *objklo;
    HERO_WORK *herow;
    sceVu0FVECTOR vf;
    sceVu0FVECTOR vec;
    f32 rad;
    sceVu0FMATRIX *nl;
    sceVu0FMATRIX *lc;
    s32 var_s7;

    supw = (SupWORK *)objw->work;
    var_s7 = 0;
    objklo = GameGbl.klonoa;
    herow = objklo->work;
    model = objw->prim;

    if (hr_pt_check() == 1) {
        return;
    }

    pObj = model->pObj;
    SetObjPause(pObj, GameGbl.pause_flag);
    if (objklo->reg0 & 0x200) {
        nkSetLight2(&herow->okuyuka->rtw, objw->posi, &nl, &lc);
    } else {
        nkSetLight2(&objklo->rtw, objw->posi, &nl, &lc);
    }
    SetObjNormalLight(pObj, nl);
    SetObjLightColor(pObj, lc);

    vu0_LoadMtx(GameGbl.d_wsm);
    vu0_Thing(vec, objw->posi);
    if (vec[3] < 128.0f) {
        pObj->Condition = 1;
    } else if (vec[3] < 256.0f) {
        pObj->Condition = 2;
    } else {
        pObj->Condition = 4;
    }

    if (GameGbl.klonoa->reg0 & 0x400) {
        model->Rot[2] = 3.141592f;
    } else {
        model->Rot[2] = 0.0f;
    }
    model->Rot[0] = objw->rot[0];

    if (objw->bun1 == 4) {
        vf[1] = _atan2(objw->muki[0], objw->muki[2]) + 3.141592f;
        if (supw->muki == 2) {
            vf[1] = _atan2(objw->muki[0], objw->muki[2]) + 1.570796f;
        } else if (supw->muki == 3) {
            vf[1] = _atan2(objw->muki[0], objw->muki[2]) - 1.570796f;
        }
        vf[1] = nkRadMask(vf[1]);

        rad = vf[1] - model->Rot[1];
        rad = nkRadMask(rad);
        if (rad > 0.392699f) {
            model->Rot[1] += 0.392699f;
            var_s7 = 1;
        } else if (rad < -0.392699f) {
            model->Rot[1] -= 0.392699f;
            var_s7 = 1;
        } else {
            model->Rot[1] = vf[1];
        }
        model->Rot[1] = nkRadMask(model->Rot[1]);

        switch (supw->stat) {
            case 1:
                nkCallMotion(pObj, &model->ActNum, 3);
                break;
            case 3:
                nkCallMotion(pObj, &model->ActNum, 7);
                break;
            case 2:
                if (objw->spd[1] < 0.0f) {
                    nkCallMotion(pObj, &model->ActNum, 4);
                } else if (objw->spd[1] > 0.0f) {
                    nkCallMotion(pObj, &model->ActNum, 6);
                }
                break;
            case 11:
                if (var_s7) {
                    nkCallMotion(pObj, &model->ActNum, 3);
                } else {
                    nkCallMotion(pObj, &model->ActNum, 2);
                }
                break;
        }
    } else {
        switch (objw->bun1) {
            case 1:
                vf[1] = objw->rot[1];
                rad = vf[1] - model->Rot[1];
                rad = nkRadMask(rad);
                model->Rot[1] += rad / 8.0f;
                model->Rot[1] = nkRadMask(model->Rot[1]);
                objw->rot[1] = model->Rot[1];
                nkCallMotion(pObj, &model->ActNum, 11);
                break;
            case 2:
                nkCallMotion(pObj, &model->ActNum, 9);
                break;
            case 3:
                model->Rot[1] = objw->rot[1];
                nkCallMotion(pObj, &model->ActNum, 10);
                if (objw->reg1 == 0) {
                    objw->reg1 = (s32)SetPopStar(pObj);
                }
                break;
        }
    }

    if (objw->bun1 != 3 && objw->reg1 != 0) {
        EraseStar(objw->reg1);
        objw->reg1 = 0;
    }

    sceVu0CopyVector(model->Trans, objw->posi);
    SetBaseMatrix(pObj, model->Rot, model->Trans, 1);
    model->Scale[0] = model->Scale[1] = model->Scale[2] = objw->freg2;
    SetObjScale(pObj, model->Scale);
    ModelDraw(pObj);
    supw->motcnt = GetActCnt(pObj);
    supw->motcnt_end = GetActEndCnt(pObj);
    supw->motstop_flag = GetActStopFlag(pObj);
    supw->mot_actno = model->ActNum;
}

static void SupOnInit(OBJWORK *objw, SupWORK *supw) {
    SFXOBJ *pObj = nkGetKloPSFXOBJ();
    GetObjMatrixTrans(pObj, 42, objw->posi);
    objw->bun1 = 1;
    objw->freg3 = 0.0f;
    objw->draw = DrawObj;
    objw->drmir = DrawObjMir;
    supw->trg = 0;
    objw->reg3 = 0;
    nkSetPopFlear();
    hSeKeyOn(0xc80c80ca00704, objw->posi, 0);
}

static void ObjInit(OBJWORK *objw) {
    PsfxMODEL *model;
    SupWORK *supw;
    s32 ret;
    OBJWORK *objklo = GameGbl.klonoa;
    SFXOBJ *pObj;
    s32 i;

    objw->prim = getmem(sizeof(PsfxMODEL));
    objw->work = getmem(sizeof(SupWORK));
    objw->reg2 = (s32)getmem(sizeof(PsfxMODEL));
    model = objw->prim;
    supw = objw->work;
    supw->pos_buff = getmem(sizeof(sceVu0FVECTOR) * 32);
    supw->pos_cnt = 0;
    objw->draw = DrawObj;
    objw->drmir = DrawObjMir;
    model->pObj = GetSfxObjectData(13);
    pObj = model->pObj;
    pObj->Condition = 1;
    model->ActNum = 1;
    SetAct(pObj, 1);
    SetBaseMatrix(pObj, model->Rot, model->Trans, 0);
    model->Rot[0] = 0.0f;
    model->Rot[1] = 0.0f;
    model->Rot[2] = 0.0f;
    model->Rot[3] = 0.0f;
    model->Scale[0] = 1.0f;
    model->Scale[1] = 1.0f;
    model->Scale[2] = 1.0f;
    model->Scale[3] = 0.0f;

    model = (PsfxMODEL *)objw->reg2;
    model->pObj = GetSfxObjectData(13);
    pObj = model->pObj;
    SetObjEffDraw(pObj);
    pObj->Condition = 1;
    model->ActNum = 1;
    SetAct(pObj, 1);
    SetBaseMatrix(pObj, model->Rot, model->Trans, 0);
    model->Rot[0] = 0.0f;
    model->Rot[1] = 0.0f;
    model->Rot[2] = 0.0f;
    model->Rot[3] = 0.0f;
    model->Scale[0] = 1.0f;
    model->Scale[1] = 1.0f;
    model->Scale[2] = 1.0f;
    model->Scale[3] = 0.0f;

    supw->lvl = 0;
    supw->trg = 0;
    supw->jumpspd_limit = 8.0f;
    supw->se_id0 = -1;
    supw->cnt = 0;
    supw->area_cnt = 0;
    objw->reg1 = 0;
    objw->reg3 = 30;
    objw->freg3 = 0.0f;
    objw->freg4 = 0.0f;
    objw->bun0++;

    for (i = 0; i < 32; i++) {
        sceVu0FVECTOR *vf = &supw->pos_buff[(supw->pos_cnt - 1) & 0x1F]; // ????
        sceVu0CopyVector(supw->pos_buff[supw->pos_cnt], objklo->posi);
        supw->pos_cnt++;
        if (supw->pos_cnt >= 32) {
            supw->pos_cnt = 0;
        }
    }

    objw->freg2 = 0.0f;
    sceVu0CopyVector(objw->posi, objklo->posi);
    objw->draw = NULL;
    objw->drmir = NULL;
    objw->bun1 = 5;
}

static void SupOff(OBJWORK *objw, SupWORK *supw) {
    if (GameGbl.popuka_flag == 1) {
        return;
    }

    if (supw->trg & 0x800 && objw->reg3 > 30) {
        GameGbl.popuka_flag = 1;
        SupOnInit(objw, supw);
        hSeKeyOn(0xc80c80ca00700, objw->posi, 0);
    }
}

static void SupRet(OBJWORK *objw, SupWORK *supw) {
    sceVu0FVECTOR vec;
    sceVu0FVECTOR posi;
    sceVu0FVECTOR rot;

    GetObjMatrixTrans(nkGetKloPSFXOBJ(), 42, vec);
    sceVu0SubVector(posi, vec, objw->posi);

    if (objw->reg4 >= 16) {
        if (supw->se_id0 != -1) {
            hSeKeyOff(supw->se_id0);
            supw->se_id0 = -1;
        }
        objw->bun1 = 5;
        objw->draw = NULL;
        objw->drmir = NULL;
        objw->reg3 = 0;
        nkSetPopFlear();
        hSeKeyOn(0xc80c80ca00704, objw->posi, 0);
        objw->freg2 = 0.0f;
    } else {

        f32 f = 16 - objw->reg4;
        posi[0] /= f;
        posi[1] /= f;
        posi[2] /= f;
        vu0_Square(rot, posi);
        objw->rot[1] = _atan2(rot[0], rot[2]) + 3.141592f;
        objw->rot[1] = nkRadMask(objw->rot[1]);
        objw->posi[0] += posi[0];
        objw->posi[1] += posi[1];
        objw->posi[2] += posi[2];
        objw->reg4++;
        objw->freg2 -= 1.0f / 30.0f;
    }
}

static void SupGhost(OBJWORK *objw, SupWORK *supw) {
    OBJWORK *klo = GameGbl.klonoa;
    u32 pos_cnt = (supw->pos_cnt + 8) & 0x1F;
    sceVu0FVECTOR v0;
    sceVu0FVECTOR v1;

    sceVu0CopyVector(v0, objw->posi);

    if (objw->reg3 < 30) {
        f32 f;

        sceVu0CopyVector(v1, supw->pos_buff[pos_cnt]);
        sceVu0SubVector(v1, v1, objw->posi);
        f = 30 - objw->reg3;
        objw->posi[0] += v1[0] / f;
        objw->posi[1] += v1[1] / f;
        objw->posi[2] += v1[2] / f;
        objw->freg2 += 1.0f / 30.0f;
    } else {
        objw->freg2 = 1.0f;
        sceVu0CopyVector(objw->posi, supw->pos_buff[pos_cnt]);
        objw->freg3 += 0.052359868f;
        objw->freg3 = nkRadMask(objw->freg3);
        objw->freg4 = -_cos(objw->freg3) * 16.0f + 16.0f;
        if (klo->bun1 == 1) {
            if (klo->reg0 & 0x400) {
                objw->posi[1] += objw->freg4;
            } else {
                objw->posi[1] -= objw->freg4;
            }
        }
    }

    sceVu0SubVector(objw->spd, objw->posi, v0);
    vu0_Square(v0, objw->spd);
    if (vu0_SquareRoot(v0[0] * v0[0] + v0[2] * v0[2]) > 2.0f) {
        objw->rot[1] = _atan2(objw->spd[0], objw->spd[2]) + 3.141592f;
        objw->rot[1] = nkRadMask(objw->rot[1]);
    }
}

static void SupJumpGo(OBJWORK *objw, SupWORK *supw) {
   OBJWORK *klo = GameGbl.klonoa;
    HERO_WORK *herow = klo->work;
    sceVu0FVECTOR vf;

    sceVu0SubVector(vf, klo->posi, objw->posi);
    if (GameGbl.kloLife[0] <= 0 || herow->gimm_flag & 1 || herow->gimm_flag & 0x1000 || klo->bun0 == 14 || klo->bun0 == 40 || herow->gimm_flag & 0x40000) {
        objw->bun1 = 1;
        objw->freg3 = 0.0f;
    }
    if (objw->freg1 > 0.0f) {
        objw->posi[0] += vf[0] / objw->freg1;
        objw->posi[1] += vf[1] / objw->freg1;
        objw->posi[2] += vf[2] / objw->freg1;
        objw->freg1 -= 1.0f;
        return;
    }
    if (GameGbl.kloLife[0] <= 0 || herow->gimm_flag & 1 || herow->gimm_flag & 0x1000 || klo->bun0 == 14 || klo->bun0 == 40) {
        objw->bun1 = 1;
        objw->freg3 = 0.0f;
        return;
    }
    GameGbl.stage_flag |= 1;
    klo->spd[1] = -16.958776f;
    if (klo->bun0 != 27) {
        klo->bun0 = 2;
    }
    herow->gimm_flag |= 0x400;
    herow->spflag2 &= -2;
    objw->bun1 = 3;
    objw->freg1 = 240.0f;
    SetSpJump(nkGetKloPSFXOBJ());
    hSeKeyOn(0xc80c80ca20115, klo->posi, 0);
    hSeKeyOn(0xc80c80ca00701, objw->posi, 0);
    if (supw->se_id0 == -1) {
        supw->se_id0 = hSeKeyOn(0xc80c805a00702, objw->posi, 0);
    }
}

static void SupStiff(OBJWORK *objw, SupWORK *supw) {
    if (objw->freg1 > 0.0f) {
        objw->freg1 -= 1.0f;
    } else {
        objw->bun1 = 1;
        objw->freg3 = 0.0f;
        if (supw->se_id0 != -1) {
            hSeKeyOff(supw->se_id0);
            supw->se_id0 = -1;
        }
    }

    objw->rot[1] -= 0.104719736f;
    objw->rot[1] = nkRadMask(objw->rot[1]);
}

static f32 SupAshiHitSub(OBJWORK *objw, sceVu0FVECTOR hitposi, SupWORK *supw) {
    sceVu0FVECTOR vec;
    sceVu0FVECTOR nor;
    sceVu0FVECTOR hit_nor;
    f32 dy;
    u32 hitcol;
    f32 work;

    vec[0] = hitposi[0];
    vec[1] = hitposi[1] - 11.25f;
    vec[2] = hitposi[2];
    vec[3] = 1.0f;
    nor[0] = vec[0];
    nor[1] = vec[1] + supw->jumpspd_limit + 11.25f + supw->tenjo_hosei;
    nor[2] = vec[2];
    vec[3] = 1.0f;

    dy = RayCrsJudgzNLcol(vec, nor, &hitcol, hit_nor);
    if (dy != 65535.0f) {
        dy += hitposi[1] - 11.25f;
    }

    if (objw->spd[1] != 0.0f) {
        work = supw->jumpspd_limit + supw->tenjo_hosei;
    } else {
        work = 3.0f;
    }

    if (!(dy <= hitposi[1] + work)) {
        dy = 65535.0f;
    }
    if (dy == 65535.0f) {
        objw->posi[1] += supw->tenjo_hosei;
    }

    return dy;
}

static void SupJumpDown(OBJWORK *objw, SupWORK *supw) {
    f32 dy;
    sceVu0FVECTOR hitposi;
    f32 retposi;
    f32 retposi2;

    hitposi[0] = objw->muki[0] * 8.75f + objw->posi[0];
    hitposi[2] = objw->muki[2] * 8.75f + objw->posi[2];
    hitposi[1] = objw->posi[1];
    retposi = SupAshiHitSub(objw, hitposi, supw);

    hitposi[0] = objw->posi[0] - objw->muki[0] * 8.75f;
    hitposi[2] = objw->posi[2] - objw->muki[2] * 8.75f;
    hitposi[1] = objw->posi[1];
    retposi2 = SupAshiHitSub(objw, hitposi, supw);

    if (!(retposi <= retposi2)) {
        retposi = retposi2;
    }

    dy = retposi;
    objw->spd[1] += 0.5110887f;
    if (objw->spd[1] > supw->jumpspd_limit) {
        objw->spd[1] = supw->jumpspd_limit;
    }
    if (objw->spd[1] > 0.0f && dy < 65535.0f) {
        if (!(objw->spd[1] < 6.0f)) {
            supw->stat = 3;
        }
        objw->posi[1] = dy;
        objw->spd[1] = 0.0f;
    }
}

static void SupOperation(OBJWORK *objw, SupWORK *supw) {
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;

    GetMichiVec(&objw->rtw, objw->muki);

    if (pPAD_LVL_RIGHT(supw)) {
        supw->mispd = 0x3400;
        supw->muki = 0;
    } else if (pPAD_LVL_LEFT(supw)) {
        supw->mispd = -0x3400;
        supw->muki = 1;
    } else if (pPAD_LVL_UP(supw)) {
        supw->muki = 2;
    } else if (pPAD_LVL_DOWN(supw)) {
        supw->muki = 3;
    } else {
        supw->mispd = 0;
    }

    if (objw->spd[1] == 0.0f) {
        if (supw->mispd != 0) {
            supw->stat = 1;
        } else if (supw->stat == 3) {
            if (supw->mot_actno == 7 && supw->motstop_flag != 0) {
                supw->stat = 11;
            }
        } else {
            supw->stat = 11;
        }
    }

    if (supw->muki == 1) {
        objw->muki[0] = -objw->muki[0];
        objw->muki[1] = -objw->muki[1];
        objw->muki[2] = -objw->muki[2];
    }

    if (pPAD_TRG_CROSS(supw)) {
        objw->spd[1] = -9.75f;
        supw->stat = 2;
    }

    SupJumpDown(objw, supw);
    objw->posi[1] += objw->spd[1];
    MichiMove(&objw->rtw, supw->mispd);
    MichiPosiXZ(&objw->rtw, objw->posi);

    vf[0] = 0.0f;
    vf[1] = -16.0f;
    vf[2] = 0.0f;
    vf[3] = 0.0f;
    sceVu0AddVector(vf, objw->posi, vf);
    vf[3] = 1.0f;
    sceVu0RotTransPers(vi, GameGbl.wsm, vf, 0);

    if (!nkScrClip(vi) || vi[3] <= 0) {
        vi[3] |= 0x8000;
    }
}

static void ObjMain(OBJWORK *objw) {
    OBJWORK *klo = GameGbl.klonoa;
    HERO_WORK *herow = klo->work;
    void (*SupportSub[7])(OBJWORK *objw, SupWORK *supw) = {
        NULL,
        SupGhost,
        SupJumpGo,
        SupStiff,
        SupOperation,
        SupOff,
        SupRet
    };
    SupWORK *supw = objw->work;

    if (GameGbl.vision == 0x0A01 || GameGbl.vision == 0x0F08 || GameGbl.vision == 0x1101 || GameGbl.vision == 0x1B01 || GameGbl.vision == 0x1B02) {
        return;
    }

    if (herow->gimm_flag & 0x1000) {
        supw->lvl = 0;
        supw->trg &= 0x800;
    } else {
        supw->lvl = GameGbl.kpd[1].lvl;
        supw->trg = GameGbl.kpd[1].trg;
    }

    if (GameGbl.wipe.outtime > 0) {
        if (objw->bun1 != 5 && objw->bun1 != 6) {
            objw->bun1 = 6;
            objw->reg4 = 0;
        }
    } else if (supw->cnt > 64 && GameGbl.popuka_flag == 1 && objw->bun1 == 5) {
        SupOnInit(objw, supw);
    }

    objw->reg3++;
    supw->cnt++;

    if (hr_pt_check() == 1) {
        supw->cnt = 0;
        objw->freg2 = 0.0f;
        objw->draw = NULL;
        objw->drmir = NULL;
        objw->bun1 = 5;
        if (objw->reg1 != 0) {
            EraseStar(objw->reg1);
            objw->reg1 = 0;
        }
        if (supw->se_id0 != -1) {
            hSeKeyOff(supw->se_id0);
            supw->se_id0 = -1;
        }
        return;
    }

    if (supw->area_cnt == 1) {
        s32 i;
        for (i = 0; i < 32; i++) {
            sceVu0FVECTOR *v = supw->pos_buff[(supw->pos_cnt - 1) & 0x1F];
            sceVu0CopyVector(supw->pos_buff[supw->pos_cnt], klo->posi);
            supw->pos_cnt++;
            if (supw->pos_cnt >= 32) {
                supw->pos_cnt = 0;
            }
        }
    } else {
        if (supw->old_pos[0] != klo->posi[0] || supw->old_pos[1] != klo->posi[1] || supw->old_pos[2] != klo->posi[2]) {
            sceVu0FVECTOR v;
            sceVu0FVECTOR *vf;
            sceVu0CopyVector(v, klo->posi);
            vf = supw->pos_buff[(supw->pos_cnt - 1) & 0x1F];
            sceVu0CopyVector(supw->pos_buff[supw->pos_cnt], v);
            supw->pos_cnt++;
            if (supw->pos_cnt >= 32) {
                supw->pos_cnt = 0;
            }
        }
        sceVu0CopyVector(supw->old_pos, klo->posi);
    }
    supw->area_cnt++;

    if (SupportSub[objw->bun1] != NULL) {
        SupportSub[objw->bun1](objw, supw);
    }

    if (objw->bun1 == 5 || objw->bun1 == 3) {
        return;
    }

    if (pPAD_TRG_START(supw) && objw->reg3 > 30) {
        GameGbl.popuka_flag = 0;
        if (objw->bun1 != 6) {
            objw->bun1 = 6;
            objw->reg4 = 0;
        }
    }

    if (GameGbl.cam.mode == 2 || GameGbl.cam.mode == 3 || GameGbl.cam.mode == 5) {
        supw->lvl = 0;
        supw->trg = 0;
    }

    if (GameGbl.vision != 0x0A01 && GameGbl.vision != 0x0F08 && GameGbl.vision != 0x1101 && pPAD_TRG_SQUARE(supw) && objw->freg2 == 1.0f && (objw->bun1 == 1 || objw->bun1 == 4)) {
        objw->bun1 = 2;
        objw->freg1 = 10.0f;
        hSeKeyOn(0xc80c80ca00703, objw->posi, 0);
    }
}

static void (*ObjFuncTbl[])(OBJWORK *objw) = {
    ObjInit,
    ObjMain
};

void nkPopuka(OBJWORK *objw) {
    ObjFuncTbl[objw->bun0](objw);
}

void nkSetPopuka() {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    objw->stat0 = 1;
    objw->stat1 = 15;
    objw->prty = 128;
}
