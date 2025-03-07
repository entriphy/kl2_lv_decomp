#include "okanoyo/okzdraw.h"
#include "harada/hr_mapdr.h"
#include "okanoyo.h"
#include "okanoyo/okzminit.h"
#include "nakano/k_math.h"
#include "nakano/light.h"
#include "nakano/vu0mac.h"
#include "take/motsys.h"
#include "take/object.h"

// type_override: void* -> tOBJECT2*
SFXOBJ* OkGetSfx(tOBJECT2 *ptr) {
    return ptr->pSfx;
}

// type_override: void* -> tOBJECT2*
SFXOBJ* OkGetMir(tOBJECT2 *ptr) {
    return ptr->pMir;
}

// TODO: not matching https://decomp.me/scratch/iN2dv
void OkGetLight(SFXOBJ *sfx, RT_WRK *rtw, sceVu0FVECTOR pos, ZLT_WORK *zltw, sceVu0FMATRIX *nlx, sceVu0FMATRIX *lcx, s32 flag) {
	sceVu0FMATRIX *nlx2;
	sceVu0FMATRIX *lcx2;
	s32 set = 0;

    if (!flag || zltw == NULL) {
        set |= rtw->rtp != NULL ? 0x100 : 0x400;
    } else {
        if (rtw->rtp != NULL) {
            set |= 0x100;
        }
        if (zltw->rno & 0xFF00) {
            set &= ~0x100;
            set |= 0x200;
        }
        if (zltw->l0 & 0xC0) {
            set |= 0x200;
        }
        if (zltw->type & 2) {
            set |= 0x800;
        }
        if (!(zltw->type & 1)) {
            set |= 0x2;
        }
        if (set == 0) {
            set |= 0x400;
        }
    }
    if (sfx == NULL) {
        set |= 0x1;
    }

    switch (set & 0xFF00) {
        case 0x200:
        {
            LgtWorkF lgtw;
            LgtWorkF lgtw0;
            LgtWorkF lgtw1;

            nkGetLgtWorkF(&lgtw0, zltw->l0 & 0x3F);
            nkGetLgtWorkF(&lgtw1, zltw->l1 & 0x3F);
            nkLightIp(&lgtw, &lgtw0, &lgtw1, zltw->lip / 4096.0f);
            nkCalcMatLgtWork(*nlx, *lcx, &lgtw, pos);
            set |= 0x800;
            break;
        }
        case 0x300:
        {
            RT_WRK rtw2;
        	LgtWorkF lgtw;
        	LgtWorkF lgtw0;
        	LgtWorkF lgtw1;
        	sceVu0FVECTOR tmp;
            s64 var_t0 = zltw->mcn << 12;
            
            tmp[1] = zltw->ypos;
            OkRoute2World2(NULL, &rtw2, tmp, zltw->rno, var_t0, 1);
            nkGetRouteLgtWork(&lgtw1, &rtw2, tmp);
            nkGetLgtWorkF(&lgtw0, zltw->l0 & 0x3F);
            nkLightIp(&lgtw, &lgtw0, &lgtw1, zltw->lip / 4096.0f);
            nkCalcMatLgtWork(*nlx, *lcx, &lgtw, pos);
            set |= 0x800;
            break;
        }
        case 0x100:
        case 0x500:
            nkSetLight2(rtw, pos, &nlx2, &lcx2);
            set |= 0x800;
            set |= 0x10;
            break;
    }

    if (set & 0x2) {
        zltw->type |= 2;
        set |= 0x800;
    }

    switch (set & 0x811) {
        case 0x811:
            hrDrawVpoLt(*nlx2, *lcx2);
            break;
        case 0x801:
            hrDrawVpoLt(*nlx, *lcx);
            break;
        case 0x1:
            sceVu0UnitMatrix(*nlx);
            sceVu0UnitMatrix(*lcx);
            hrDrawVpoLt(*nlx, *lcx);
            break;
        case 0x810:
            SetObjNormalLight(sfx, nlx2);
            SetObjLightColor(sfx, lcx2);
            break;
        case 0x800:
            SetObjNormalLight(sfx, nlx);
            SetObjLightColor(sfx, lcx);
            break;
    }
}

void OkSetLight(OBJWORK *objw, s32 lno0, s32 lno1, s32 rno, s32 mcn, f32 ypos, s32 type) {
	ZSUB_WORK *zsub = objw->prim;
	ZAKO_COMMON *zakow = objw->work;

    zsub->zltw.l0 = lno0;
    zsub->zltw.l1 = lno1;
    zsub->zltw.lip = 0.0f;
    zsub->zltw.rno = rno;
    zsub->zltw.mcn = mcn;
    zsub->zltw.ypos = ypos;
    
    if (lno0 & 0x80) {
        zakow->flag |= 0x80;
    } else {
        zakow->flag &= ~0x80;
    }

    zsub->zltw.type = type;
}

void OkSetShadow(SFXOBJ *sfx, sceVu0FVECTOR pos, f32 rot, f32 yofs, f32 depth) {
	sceVu0FVECTOR tmp;
	sceVu0FVECTOR rad;

    tmp[0] = pos[0];
    tmp[1] = pos[1];
    tmp[2] = pos[2];
    tmp[3] = 1.0f;
    rad[0] = 0.0f;
    rad[1] = 1.0f;
    rad[2] = 0.0f;
    rad[3] = 1.0f;
    abSetShadow(&tmp, &rad, sfx);
}

s32 OkGetModelLevel(sceVu0FVECTOR pos, s32 pic) {
	f32 dis = OkGetDistanceXYZ(GameGbl.cam.posi, pos);
    
    switch (pic) {
        case 65:
        case 70:
        case 71:
        case 87:
        case 88:
        case 228:
        case 291:
        case 297:
        case 545:
        case 546:
        case 547:
            dis = 0.0f;
            break;
        case 29:
        case 31:
        case 32:
        case 47:
            dis /= 3.0f;
            break;
        case 33:
            dis = 100.0f;
            break;
    }

    if (dis < 200.0f) {
        return 1;
    } else if (dis < 512.0f) {
        return 2;
    } else {
        return 4;
    }
}

void GetMichiRotXY4(sceVu0FVECTOR vec, sceVu0FVECTOR rot) {
    sceVu0FVECTOR vf;

    vu0_Square(vf, vec);
    rot[0] = -_atan2(vec[1], vu0_SquareRoot(vf[0] + vf[2])) + M_PI_2f;
    NORMRAD(rot[0]);
    rot[1] = _atan2(vec[0], vec[2]) + M_PIf;
    NORMRAD(rot[1]);
}

// tarns hehe
void OkSfxTarns(OBJWORK *objw, s32 num, sceVu0FVECTOR pos) {
	tOBJECT2 *tOp = (tOBJECT2 *)objw->_hOp;
    GetObjMatrixTrans(tOp->pSfx, num, pos);
}

void OkGetSfxScale(tOBJECT2 *hOp, sceVu0FVECTOR *ret) {
    SFXOBJ *sfx = hOp->pSfx;

    ret[0][0] = sfx->ScaleVector[0];
    ret[1][0] = sfx->ScaleVector[1];
    ret[2][0] = sfx->ScaleVector[2];
}

void OkFreeLightSet(SFXOBJ *sfx, sceVu0FVECTOR pos) {
    sceVu0FMATRIX nlm;
	sceVu0FMATRIX lcm;

    nkSetLightNo(nlm, lcm, 0, pos);
    // Pointing to a stack var..?
    sfx->pNormalLight = &nlm;
    sfx->pLightColor = &lcm;
}

void OkScaleSet(OBJWORK *objw, sceVu0FVECTOR scale) {
    tOBJECT2 *tOp = (tOBJECT2 *)objw->_hOp;
    SetObjScale(tOp->pSfx, scale);
}

void OkhObjScaleUp(tOBJECT2 *hOp, f32 target, f32 div) {
    SFXOBJ *sfx = hOp->pSfx;
    f32 org = sfx->ScaleVector[0];
    sceVu0FVECTOR f;

    org += (target - org) / div;;
    SETVECTOR(f, org, org, org, 0.0f);
    SetObjScale(sfx,  f);
}

void OkhObjScaleUpXZ(tOBJECT2 *hOp, f32 target, f32 div) {
    SFXOBJ *sfx = hOp->pSfx;
    sceVu0FVECTOR org;
    sceVu0FVECTOR f;

    COPYVECTOR(org, sfx->ScaleVector);
    org[0] += (target - org[0]) / div;
    org[1] -= (target - org[1]) / (div * 4.0f);
    SETVECTOR(f, org[0], org[1], org[0], 0.0f);
    SetObjScale(sfx,  f);
}

void OkhObjScaleYxz(tOBJECT2 *hOp, f32 muly, f32 mulxz) {
    SFXOBJ *sfx = hOp->pSfx;
    sceVu0FVECTOR org;

    COPYVECTOR(org, sfx->ScaleVector);
    org[1] *= muly;
    org[0] *= mulxz;
    org[2] *= mulxz;
    SetObjScale(sfx,  org);
}

void OkhObjScaleUp0(tOBJECT2 *hOp, f32 add) {
    SFXOBJ *sfx = hOp->pSfx;
    f32 org = sfx->ScaleVector[0];
    sceVu0FVECTOR f;

    org += add;
    SETVECTOR(f, org, org, org, 0.0f);
    SetObjScale(sfx, f);
}

void OkTObjScaleff(SFXOBJ *sfx, f32 rad, f32 mult) {
	sceVu0FVECTOR f;

    f[1] = rad + M_2PI_3f;
    f[2] = rad + M_4PI_3f;
    f[0] = rad + M_TWOPIf;
    NORMRAD(f[0]);
    NORMRAD(f[1]);
    NORMRAD(f[2]);
    f[1] = 1.0f + sinf(f[1]) * mult;
    f[0] = 1.0f + sinf(f[0]) * mult;
    f[2] = 1.0f + sinf(f[2]) * mult;
    f[3] = 0.0f;
    SetObjScale(sfx, f);
}

void OkTObjScaleffo(sceVu0FVECTOR out, f32 rad, f32 mult, f32 org) {
	sceVu0FVECTOR f;

    f[1] = rad + M_2PI_3f;
    f[2] = rad + M_4PI_3f;
    f[0] = rad + M_TWOPIf;
    NORMRAD(f[0]);
    NORMRAD(f[1]);
    NORMRAD(f[2]);
    out[1] = org + sinf(f[1]) * mult;
    out[0] = org + sinf(f[0]) * mult;
    out[2] = org + sinf(f[2]) * mult;
    out[3] = 0.0f;
}

void OkTObjScaleffXZ(SFXOBJ *sfx, f32 rad, f32 mult) {
	sceVu0FVECTOR f;

    f[2] = 1.0f + sinf(rad) * mult;
    f[0] = 1.0f + cosf(rad) * mult;
    f[1] = f[0];
    f[3] = 0.0f;
    SetObjScale(sfx, f);
}

void OkTObjScaleffXZ21(tOBJECT2 *tOp, f32 mult) {
    SFXOBJ *sfx = tOp->pSfx;
	sceVu0FVECTOR f;

    f[2] = mult;
    f[1] = 1.0f;
    f[0] = mult;
    f[3] = 0.0f;
    SetObjScale(sfx, f);
}

void OkTObjScaleffXZ2(SFXOBJ *sfx, f32 rad, f32 mult, f32 base, f32 itp) {
	sceVu0FVECTOR f;

    f[2] = (base + sinf(rad) * mult) * itp + base * (1.0f - itp);
    f[0] = (base + cosf(rad) * mult) * itp + base * (1.0f - itp);
    f[1] = f[0];
    f[3] = 0.0f;
    SetObjScale(sfx, f);
}

void OkTObjScaleTest(SFXOBJ *sfx, OBJWORK *objw) {
    SetObjScale(sfx, objw->ang);
}

// look closely at the names of the next 2 functions and tell me there's a god

void OkhobjScaleSet(OBJWORK *objw, f32 scale) {
	SFXOBJ *sfx = ((tOBJECT2 *)objw->_hOp)->pSfx;
	sceVu0FVECTOR f;

    SETVECTOR(f, scale, scale, scale, 0.0f);
    SetObjScale(sfx, f);
}

void OkhObjScaleSet(tOBJECT2 *hOp, f32 scale) {
	SFXOBJ *sfx = hOp->pSfx;
	sceVu0FVECTOR f;

    SETVECTOR(f, scale, scale, scale, 0.0f);
    SetObjScale(sfx, f);
}

void OkhobjScaleUp(OBJWORK *objw, f32 target, f32 div) {
    OkhObjScaleUp((tOBJECT2 *)objw->_hOp, target, div);
}

void OkModelSetT(OBJWORK *objw, s32 p_id, void *Draw, void *Draw2) {
    ZAKO_COMMON *zakow = objw->work;
    SFXOBJ *sfx;

    OkTObjModelSetGeneral(objw, (tOBJECT2 **)&objw->_hOp, p_id, Draw, Draw2);
    sfx = ((tOBJECT2 *)objw->_hOp)->pSfx;
    switch (zakow->tekipic) {
        case 38:
            SetSfxVariationClut(sfx, 2, 2, 1.0f);
            break;
        case 17:
        case 68:
            SetSfxVariationClut(sfx, 1, 1, 1.0f);
            break;
        case 56:
            break;
    }
}

void OkModelSetNT(OBJWORK *objw, s32 p_id, tOBJECT2 **hOp) {
    OkTObjModelSetGeneral(objw, hOp, p_id, NULL, NULL);
}

void OkModelSet2T(OBJWORK *objw, s32 p_id, void *Draw, void *Draw2) {
    ZAKO_COMMON *zakow = objw->work;

    OkTObjModelSetGeneral(objw, (tOBJECT2 **)&objw->_hOp, p_id, Draw, Draw2);
    switch (zakow->tekipic) {
        case 30:
            if (zakow->mp0 == NULL) {
                zakow->mp1 = NULL;
            }
            OkModelSetNT(objw, 128, (tOBJECT2 **)&zakow->mp1);
            OkModelSetNT(objw, 129, (tOBJECT2 **)&zakow->mp0);
            break;
        case 22:
            OkModelSetNT(objw, 128, (tOBJECT2 **)&zakow->mp0);
            break;
        case 12:
        case 13:
            OkModelSetNT(objw, 254, (tOBJECT2 **)&zakow->mp0);
            break;
    }
}

extern f32 OkZakoDefScale(s16 tekipic);
void OkModelSetBigT(OBJWORK *objw, s32 p_id, void *Draw, void *Draw2) {
    SFXOBJ *sfx = OkGetSfx((tOBJECT2 *)objw->_hOp);

    OkTObjModelSetGeneral(objw, (tOBJECT2 **)&objw->_hOp, p_id, Draw, Draw2);
    OkTObjScaleSet((tOBJECT2 *)objw->_hOp, OkZakoDefScale(p_id));
    SetObjCondition(sfx, 2);
    SetAct(sfx, 1);
}

void OkModelSetBlnT(/* s4 20 */ OBJWORK *objw, /* s5 21 */ s32 p_id, /* s1 17 */ void *Draw, /* s2 18 */ void *Draw2) {
    ZAKO_WORK4 *zakow = objw->work;
    /* s3 19 */ SFXOBJ *sfx = OkGetSfx((tOBJECT2 *)objw->_hOp);
    /* -0x80(sp) */ sceVu0FVECTOR scale;

    OkResetMotStat(zakow);
    OkModelSetT(objw, p_id, Draw, Draw2);
    switch (zakow->tekipic) {
        default:
            objw->draw = OkZSubstDraw;
            break;
        case 19:
            scale[1] = OkBallonDefScale(p_id);
            scale[0] = scale[1];
            scale[2] = scale[1];
            OkTObjScaleSet2((tOBJECT2 *)objw->_hOp, scale);
            zakow->scale[0] = zakow->scale[1] = scale[1];
            SetAct(sfx, 1);
            SetObjCondition(sfx, 8);
            break;
        case 1:
        case 7:
        case 8:
        case 12:
        case 13:
        case 14:
        case 17:
        case 18:
        case 22:
        case 23:
        case 30:
        case 34:
        case 36:
        case 43:
        case 44:
        case 45:
        case 46:
        case 49:
        case 51:
        case 56:
        case 57:
            SetAct(sfx, 1);
            SetObjCondition(sfx, 8);
            OkTObjScaleSet((tOBJECT2 *)objw->_hOp, 1.0f);
            zakow->scale[0] = zakow->scale[1] = 1.0f;
            break;
        case 37:
            SetSfxVariationClut(sfx, 1, 1, 1.0f);
            SetAct(sfx, 1);
            SetObjCondition(sfx, 8);
            OkTObjScaleSet((tOBJECT2 *)objw->_hOp, 1.0f);
            zakow->scale[0] = zakow->scale[1] = 1.0f;
            break;
        case 38:
            SetSfxVariationClut(sfx, 3, 3, 1.0f);
            SetAct(sfx, 1);
            SetObjCondition(sfx, 8);
            OkTObjScaleSet((tOBJECT2 *)objw->_hOp, 1.0f);
            zakow->scale[0] = zakow->scale[1] = 1.0f;
            break;
        case 29:
            scale[1] = OkBallonDefScale(p_id) * 1.2f;
            scale[0] = scale[1];
            scale[2] = scale[1];
            OkTObjScaleSet2((tOBJECT2 *)objw->_hOp, scale);
            zakow->scale[0] = zakow->scale[1] = scale[1];
        case 47:
            scale[1] = OkBallonDefScale(p_id) * 1.2f;
            scale[0] = scale[1];
            scale[2] = scale[1];
            OkTObjScaleSet2((tOBJECT2 *)objw->_hOp, scale);
            zakow->scale[0] = zakow->scale[1] = scale[1];
            SetObjCondition(sfx, 8);
            break;
        case 11:
            OkTObjScaleSet((tOBJECT2 *)objw->_hOp, 1.0f);
            zakow->scale[0] = zakow->scale[1] = 1.0f;
            SetObjCondition(sfx, 8);
            SetAct(sfx, 2);
            break;
        case 40:
            OkTObjScaleSet((tOBJECT2 *)objw->_hOp, OkZakoDefScale(zakow->tekipic));
            zakow->scale[0] = zakow->scale[1] = OkZakoDefScale(zakow->tekipic);
            if (GameGbl.vision / 256 == 4) {
                SetAct(sfx, 9);
            } else {
                SetAct(sfx, 12);
            }
            break;
    }

    objw->freg4 = M_PI_2f;
}



void OkSetOutLineOff(OBJWORK *objw) {
    if (objw->_hOp != NULL) {
        SetObjOutlineOff(OkGetSfx((tOBJECT2 *)objw->_hOp));
    }
}

void OkDrawGeneralT(OBJWORK *objw) {
    ZAKO_COMMON *zakow = objw->work;
    ZSUB_WORK *zsub = objw->prim;

    if (objw->_hOp != NULL) {
        SFXOBJ *sfx = OkGetSfx((tOBJECT2 *)objw->_hOp);
        s32 ret;
        sceVu0FVECTOR pos;
        sceVu0FVECTOR rad;

        SETVECTOR(pos, objw->posi[0], objw->posi[1], objw->posi[2], 1.0f);
        SETVECTOR(rad, 0.0f, 1.0f * zakow->gspd, 0.0f, 1.0f);
        SetBaseMatrix(sfx, objw->rot, pos, 1);
        ret = SfxObjBallClipCheck(sfx);
        if (!ret) {
            SetObjCondition(sfx, OkGetModelLevel(pos, zakow->tekipic));
            OkGetLight(sfx, &objw->rtw, pos, &zsub->zltw, &zsub->zltw.nlx, &zsub->zltw.lcx, zakow->flag & 0x80);
        }
        SetObjPause(sfx, GameGbl.pause_flag);
        ModelDraw(sfx);
        if (!ret) {
            abSetShadow2(pos, rad, sfx);
        }
    }
}

void OkDrawGeneralFT(OBJWORK *objw) {
    ZAKO_WORK4 *zkw = objw->work;
    SFXOBJ *sfx;
    sceVu0FVECTOR rot;
    static sceVu0FMATRIX nlx;
    static sceVu0FMATRIX lcx;
    sceVu0FVECTOR pos;

    if (objw->_hOp == NULL) {
        return;
    }

    sfx = OkGetSfx((tOBJECT2 *)objw->_hOp);
    pos[0] = objw->posi[0];
    pos[1] = objw->posi[1];
    pos[2] = objw->posi[2];
    pos[3] = 1.0f;

    switch (zkw->tekipic) {
        default:
            if (!GameGbl.pause_flag) {
                objw->freg4 += M_PI_32f;
                NORMRAD(objw->freg4);
                OkTObjScaleff(sfx, objw->freg4, 0.07f);
                OkhObjScaleYxz((tOBJECT2 *)objw->_hOp, zkw->scale[1], zkw->scale[0]);
            }
            break;
        case 29:
        case 47:
            pos[1] -= 40.0f;
            break;
        case 11:
        case 18:
        case 19:
        case 36:
            break;
    }

    SETVECTOR(rot, 0.0f, objw->rot[1], objw->rot[2], 1.0f);
    SetBaseMatrix(sfx, rot, pos, 0);
    OkGetLight(sfx, &objw->rtw, pos, NULL, &nlx, &lcx, 0);
    SetObjPause(sfx, GameGbl.pause_flag);
    ModelDraw(sfx);
}

void OkDrawGeneralTG(OBJWORK *objw) {
    ZAKO_COMMON *zakow = objw->work;
    ZSUB_WORK *zsub = objw->prim;

    if (objw->_hOp != NULL) {
        SFXOBJ *sfx = OkGetSfx((tOBJECT2 *)objw->_hOp);
        s32 ret;
        sceVu0FVECTOR pos;
        sceVu0FVECTOR rad;

        SETVECTOR(pos, objw->posi[0], objw->posi[1], objw->posi[2], 1.0f);
        SETVECTOR(rad, 0.0f, 1.0f, 0.0f, 1.0f);
        SetBaseMatrix(sfx, objw->rot, pos, 1);
        ret = SfxObjBallClipCheck(sfx);
        if (!ret) {
            SetObjCondition(sfx, OkGetModelLevel(pos, zakow->tekipic));
            OkGetLight2(sfx, &objw->rtw, pos, &zsub->zltw, &zsub->zltw.nlx, &zsub->zltw.lcx, zakow->flag & 0x80, objw->rot);
        }
        SetObjPause(sfx, GameGbl.pause_flag);
        ModelDraw(sfx);
        if (!ret) {
            abSetShadow2(pos, rad, sfx);
        }
    }
}

void OkDrawHrVpo(OBJWORK *objw) {
    ZAKO_COMMON *zakow = objw->work;
    ZSUB_WORK *zsub = objw->prim;
    sceVu0FMATRIX nlx;
    sceVu0FMATRIX lcx;
    sceVu0FMATRIX mx;
    sceVu0FVECTOR rot;
    sceVu0FVECTOR vf;
    
    rot[1] = objw->ang[1] + M_PI_2f;
    NORMRAD(rot[1]);
    rot[0] = objw->ang[0];
    vf[1] = objw->ang[1];
    NORMRAD(vf[1]);
    vf[1] = M_PIf;
    vf[0] = objw->ang[0];
    OkGetLight2(NULL, &objw->rtw, objw->posi, &zsub->zltw, &zsub->zltw.nlx, &zsub->zltw.lcx, zakow->flag & 0x80, rot);

    sceVu0UnitMatrix(mx);
    sceVu0RotMatrixZ(mx, mx, rot[0]);
    sceVu0RotMatrixY(mx, mx, rot[1]);
    sceVu0TransMatrix(mx, mx, objw->posi);

    switch (zakow->tekipic) {
        case 2160:
        case 2161:
        case 2162:
        case 2163:
        case 2164:
        case 2165:
        case 2166:
        case 2167:
        case 2168:
        case 2169:
        case 2170:
            break;
        case 2171:
        case 2172:
        case 2173:
        case 2174:
        case 2175:
            sceVu0ScaleVectorXYZ(mx[0], mx[0], objw->muki[0]);
            sceVu0ScaleVectorXYZ(mx[1], mx[1], objw->muki[0]);
            sceVu0ScaleVectorXYZ(mx[2], mx[2], objw->muki[0]);
            break;
        case 2176:
        case 2177:
        case 2178:
        case 2179:
        case 2180:
        case 2181:
        case 2182:
        case 2183:
        case 2184:
        case 2185:
        case 2186:
        case 2187:
        case 2188:
        case 2189:
        case 2190:
        case 2191:
        case 2192:
        case 2193:
        case 2194:
        case 2195:
        case 2196:
        case 2197:
        case 2198:
        case 2199:
            break;
    }

    sceVu0MulMatrix(mx, GameGbl.wsm, mx);
    hrDrawVpoi(mx, hrget_vpoinfo(objw->sflag), 0);   
}

void OkDrawHrVpoX(OBJWORK *objw) {
    ZAKO_COMMON *zakow = objw->work;
    ZSUB_WORK *zsub = objw->prim;
    sceVu0FMATRIX nlx;
    sceVu0FMATRIX lcx;
    sceVu0FMATRIX mx;
    sceVu0FVECTOR rot;
    
    rot[1] = objw->rot[1];
    NORMRAD(rot[1]);
    rot[0] = objw->rot[0];
    OkGetLight2(NULL, &objw->rtw, objw->posi, &zsub->zltw, &zsub->zltw.nlx, &zsub->zltw.lcx, zakow->flag & 0x80, rot);

    sceVu0UnitMatrix(mx);
    sceVu0RotMatrixZ(mx, mx, rot[0]);
    sceVu0RotMatrixY(mx, mx, rot[1]);
    sceVu0TransMatrix(mx, mx, objw->posi);
    sceVu0MulMatrix(mx, GameGbl.wsm, mx);
    hrDrawVpoi(mx, hrget_vpoinfo(objw->sflag), 0);   
}

void OkDrawHrVpo2(OBJWORK *objw) {
    GIMM_COMMON *zakow = objw->work;
    ZSUB_WORK *zsub = objw->prim;
    sceVu0FMATRIX nlx;
    sceVu0FMATRIX lcx;
    sceVu0FMATRIX mx;
    sceVu0FVECTOR rot;
    sceVu0FVECTOR vf;
    
    rot[1] = objw->ang[1] + M_PI_2f;
    NORMRAD(rot[1]);
    rot[0] = objw->ang[0];
    OkGetLight2(NULL, &objw->rtw, objw->posi, &zsub->zltw, &zsub->zltw.nlx, &zsub->zltw.lcx, zakow->flag & 0x80, rot);

    sceVu0UnitMatrix(mx);
    sceVu0RotMatrixZ(mx, mx, rot[0]);
    sceVu0RotMatrixY(mx, mx, rot[1]);
    sceVu0TransMatrix(mx, mx, objw->posi);
    sceVu0MulMatrix(mx, GameGbl.wsm, mx);
    hrDrawVpoi(mx, hrget_vpoinfo(objw->sflag), 0);   

    SETVECTOR(vf, 0.0f, -45.0f, 0.0f, 1.0f);
    sceVu0UnitMatrix(mx);
    sceVu0RotMatrixZ(mx, mx, objw->ang[0]);
    sceVu0RotMatrixY(mx, mx, objw->ang[1]);
    sceVu0TransMatrix(mx, mx, objw->posi);
    sceVu0ApplyMatrix(vf, mx, vf);

    _atan2(vf[1] - zakow->tmp[1], OkGetDistanceXZ(zakow->tmp, vf));
    rot[1] = objw->ang[1] + M_PI_2f;
    NORMRAD(rot[1]);
    rot[0] = objw->ang[0] * 2.0f + M_PIf;
    NORMRAD(rot[0]);
    OkGetLight2(NULL, &objw->rtw, objw->posi, &zsub->zltw, &zsub->zltw.nlx, &zsub->zltw.lcx, zakow->flag & 0x80, rot);

    sceVu0UnitMatrix(mx);
    sceVu0RotMatrixZ(mx, mx, rot[0]);
    sceVu0RotMatrixY(mx, mx, rot[1]);
    sceVu0ScaleVectorXYZ(mx[1], mx[1], (vf[1] - zakow->tmp[1]) / 350.0f);
    sceVu0TransMatrix(mx, mx, vf);
    sceVu0MulMatrix(mx, GameGbl.wsm, mx);
    hrDrawVpoi(mx, hrget_vpoinfo(objw->sflag + 1), 0); 
}

void OkModelSetV(/* s0 16 */ OBJWORK *objw, /* s1 17 */ int p_id, /* a2 6 */ void *Draw, /* a3 7 */ void *Draw2) {}

void OkDrawGeneralN2T(OBJWORK *objw) {
    // TODO
}