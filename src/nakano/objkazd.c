#include "nakano/objkazd.h"
#include "nakano/vu0mac.h"
#include "nakano/route.h"
#include "nakano/k_math.h"
#include "okanoyo/okio.h"
#include "hoshino/h_gamesnd.h"

static void DrawKazdMir(OBJWORK *objw) {
    // Empty function
}

static void DrawKazd(OBJWORK *objw) {
    // Empty function
}

static void ObjInit(OBJWORK *objw) {
    objw->work = getmem(sizeof(BUKI_WORK));
    GameGbl.kazd = objw;
    objw->pers = NULL;
    objw->draw = DrawKazd;
    objw->drmir = DrawKazdMir;
    objw->hitram = 8;
    objw->bun0 = 4;
}

static void ObjIki(OBJWORK *objw) {
    BUKI_WORK *bukiw = objw->work;
    OBJWORK *klo = GameGbl.klonoa;
    HERO_WORK *herow = klo->work;
    sceVu0FVECTOR vf0;
    sceVu0FVECTOR vf1;
    sceVu0FVECTOR vec;
    f32 dis;
    f32 angy;
    sceVu0FMATRIX mat;
    u32 work2;
    OBJWORK *yukaobj;
    f32 cros;
    f32 work;

    if (klo->bun1 == 3 || klo->bun1 == 4) {
        objw->freg3 = 112.0f;
        vf0[0] = klo->spd[0];
        vf0[1] = 0.0f;
        vf0[2] = klo->spd[2];
        angy = klo->rot[1] + klo->freg1;
        if (herow->lvl & herow->down_move) {
            angy = klo->rot[1] + 3.141592f - klo->freg1;
        }
        if (angy < -3.141592f) {
            angy += 6.283184f;
        } else if (angy > 3.141592f) {
            angy -= 6.283184f;
        }
        vf1[0] = -_sin(angy);
        vf1[1] = 0.0f;
        vf1[2] = _cos(angy);
        objw->spd[0] = vf0[0] + vf1[0] * 7.0f * 2.0f;
        objw->spd[1] = _sin(klo->rot[0]) * 7.0f * 2.0f;
        objw->spd[2] = vf0[2] + vf1[2] * 7.0f * 2.0f;

        if (klo->bun1 == 4) {
            sceVu0UnitMatrix(mat);
            if (herow->lvl & herow->down_move) {
                angy = nkRadMask(-klo->rot[1]);
                sceVu0RotMatrixY(mat, mat, klo->freg1);
                sceVu0RotMatrixZ(mat, mat, -klo->rot[2]);
                sceVu0RotMatrixX(mat, mat, -klo->rot[0]);
            } else {
                angy = nkRadMask(-klo->rot[1] + 3.141592f);
                sceVu0RotMatrixY(mat, mat, -klo->freg1);
                sceVu0RotMatrixZ(mat, mat, klo->rot[2]);
                sceVu0RotMatrixX(mat, mat, klo->rot[0]);
            }
            sceVu0RotMatrixY(mat, mat, angy);
            vf1[0] = 0.0f;
            vf1[1] = 0.0f;
            vf1[2] = -1.0f;
            vf1[3] = 1.0f;
            sceVu0ApplyMatrix(vf1, mat, vf1);
            objw->spd[0] = vf0[0] + vf1[0] * 7.0f * 2.0f;
            objw->spd[1] = vf0[1] + vf1[1] * 7.0f * 2.0f;
            objw->spd[2] = vf0[2] + vf1[2] * 7.0f * 2.0f;
        }
        objw->posi[0] += objw->spd[0];
        objw->posi[1] += objw->spd[1];
        objw->posi[2] += objw->spd[2];
    } else if (!(objw->reg7 & 2)) {
        if (objw->reg7 & 1) {
            bukiw->mispd = -0x6000;
        } else {
            bukiw->mispd = 0x6000;
            if (klo->bun1 == 2) {
                bukiw->mispd += herow->mispd;
            }
        }
        objw->spd[0] = objw->spd[1] = objw->spd[2] = 0.0f;
        MichiMove(&objw->rtw, bukiw->mispd);
        if (klo->bun1 == 2 && klo->spd[1] == 0.0f) {
            MichiPosi(&objw->rtw, objw->posi);
            objw->posi[1] -= 41.0f; // They changed this to 49.0f in KPRS
            objw->posi[3] = 1.0f;
        } else {
            if (klo->reg0 & 0x200) {
                GimMichiPosiXZ(&objw->rtw, objw->posi, herow->okuyuka);
            } else {
                MichiPosiXZ(&objw->rtw, objw->posi);
            }
        }
    } else {
        if (klo->reg0 & 0x200) {
            objw->spd[1] += herow->okuyuka->spd[1];
            objw->posi[1] += klo->spd[1];
            GimGetMichiVec(&klo->rtw, objw->muki, herow->okuyuka);
            if (herow->Muki == 1) {
                dis = _atan2(objw->muki[0], objw->muki[2]);
                dis = nkRadMask(-dis);
                objw->muki[0] = _cos(dis);
                objw->muki[2] = _sin(dis);
            } else {
                dis = _atan2(objw->muki[0], objw->muki[2]) + 3.141592f;
                dis = nkRadMask(-dis);
                objw->muki[0] = _cos(dis);
                objw->muki[2] = _sin(dis);
            }
            objw->posi[0] = klo->posi[0] + objw->muki[0] * 7.0f * objw->freg1;
            objw->posi[2] = klo->posi[2] + objw->muki[2] * 7.0f * objw->freg1;
            objw->freg1 += 1.0f;
        } else {
            objw->spd[0] = objw->muki[0] * 7.0f;
            objw->spd[1] = 0.0f;
            objw->spd[2] = objw->muki[2] * 7.0f;
            objw->posi[0] += objw->spd[0];
            objw->posi[2] += objw->spd[2];
            if (klo->bun1 == 2) {
                objw->posi[0] += klo->spd[0];
                objw->posi[2] += klo->spd[2];
            }
        }
    }

    objw->reg0++;
    dis = GetDistanceXZ(objw->posi, klo->posi);
    if (dis >= objw->freg3 || objw->hitram == 2 || objw->reg0 >= 30) {
        objw->bun0 = 2;
        objw->reg1 = 2;
    } else if (objw->hitram == 1) {
        abKazdTekiHit(objw);
        vf0[0] = klo->posi[0];
        vf0[2] = klo->posi[2];
        if (klo->bun1 == 4) {
            vf0[0] = klo->posi[0] - herow->slant[0] * 41.0f;
            vf0[1] = klo->posi[1] - herow->slant[1] * 41.0f;
            vf0[2] = klo->posi[2] - herow->slant[2] * 41.0f;
        } else {
            if (klo->reg0 & 0x400U) {
                vf0[1] = klo->posi[1] + 25.0f;
            } else {
                vf0[1] = klo->posi[1] - 25.0f;
            }
        }
        objw->spd[0] = objw->spd[1] = objw->spd[2] = 0.0f;
        GetTaniVector2(objw->posi, vf0, objw->muki);
        objw->reg1 = 2;
        objw->bun0 = 2;
    } else if (objw->hitram == 16) {
        abKazdGumiHit(objw);
        vf0[0] = klo->posi[0];
        vf0[2] = klo->posi[2];
        if (klo->bun1 == 4) {
            vf0[0] = klo->posi[0] - herow->slant[0] * 41.0f;
            vf0[1] = klo->posi[1] - herow->slant[1] * 41.0f;
            vf0[2] = klo->posi[2] - herow->slant[2] * 41.0f;
        } else {
            if (klo->reg0 & 0x400U) {
                vf0[1] = klo->posi[1] + 25.0f;
            } else {
                vf0[1] = klo->posi[1] - 25.0f;
            }
        }
        objw->spd[0] = objw->spd[1] = objw->spd[2] = 0.0f;
        GetTaniVector2(objw->posi, vf0, objw->muki);
        objw->reg1 = 2;
        objw->bun0 = 2;
        klo->bun0 = 40;
    }

    vf0[0] = objw->muki[0];
    vf0[1] = 0.0f;
    vf0[2] = objw->muki[2];
    sceVu0Normalize(vf1, vf0);
    vf1[0] *= 32.0f;
    vf1[1] *= 32.0f;
    vf1[2] *= 32.0f;
    sceVu0AddVector(vf1, objw->posi, vf1);
    cros = RayCrsJudgzNLcol(objw->posi, vf1, &work2, vec);
    work = nkObjPolyHit(objw->posi, vf1, &yukaobj);
    if (work < cros) {
        cros = work;
        work2 = 0xFFFFFF;
    }
    if (cros >= 0.0f && cros <= 30.0f && work2 != 0xFF00) {
        objw->bun0 = 2;
        abKazdKabeHit(objw);
    }
    if (objw->bun0 == 2) {
        bukiw->dx = objw->posi[0] - klo->posi[0];
        bukiw->dz = objw->posi[2] - klo->posi[2];
        if (klo->bun1 == 4) {
            bukiw->dx = objw->posi[0] - (klo->posi[0] - herow->slant[0] * 41.0f);
            bukiw->dy = objw->posi[1] - (klo->posi[1] - herow->slant[1] * 41.0f);
            bukiw->dz = objw->posi[2] - (klo->posi[2] - herow->slant[2] * 41.0f);
        } else {
            if (klo->reg0 & 0x400) {
                bukiw->dy = objw->posi[1] - (klo->posi[1] + 25.0f);
            } else {
                bukiw->dy = objw->posi[1] - (klo->posi[1] - 25.0f);
            }
        }
        bukiw->modo_spd_x = bukiw->dx / 8.0f;
        bukiw->modo_spd_y = bukiw->dy / 8.0f;
        bukiw->modo_spd_z = bukiw->dz / 8.0f;
        objw->reg0 = 0;
        objw->spd[0] = objw->spd[1] = objw->spd[2] = 0.0f;
    }
}

static void ObjModori(OBJWORK *objw) {
    BUKI_WORK *bukiw = objw->work;
    OBJWORK *klo = GameGbl.klonoa;
    HERO_WORK *herow = klo->work;
    sceVu0FVECTOR vf0;

    objw->reg1 |= 2;
    sceVu0CopyVector(vf0, klo->posi);
    if (klo->bun1 == 4) {
        vf0[0] -= herow->slant[0] * 41.0f;
        vf0[1] -= herow->slant[1] * 41.0f;
        vf0[2] -= herow->slant[2] * 41.0f;
    } else {
        if (klo->reg0 & 0x400) {
            vf0[1] += 25.0f;
        } else {
            vf0[1] -= 25.0f;
        }
    }

    bukiw->dx -= bukiw->modo_spd_x;
    objw->posi[0] = klo->posi[0] + bukiw->dx;
    bukiw->dz -= bukiw->modo_spd_z;
    objw->posi[2] = klo->posi[2] + bukiw->dz;
    bukiw->dy -= bukiw->modo_spd_y;
    if (klo->bun1 == 4) {
        objw->posi[0] = klo->posi[0] - herow->slant[0] * 41.0f + bukiw->dx;
        objw->posi[1] = klo->posi[1] - herow->slant[1] * 41.0f + bukiw->dy;
        objw->posi[2] = klo->posi[2] - herow->slant[2] * 41.0f + bukiw->dz;
    } else {
        if (klo->reg0 & 0x400) {
            objw->posi[1] = klo->posi[1] + 25.0f + bukiw->dy;
        } else {
            objw->posi[1] = klo->posi[1] - 25.0f + bukiw->dy;
        }
    }

    if (objw->hitram == 1 && !(klo->reg0 & 1)) {
        klo->reg0 |= 1;
    }
    objw->reg0++;
    if (objw->reg0 >= 8) {
        if (objw->hitram == 1) {
            ((HERO_WORK *)klo->work)->spflag2 &= -2;
        }
        objw->bun0 = 4;
        objw->hitram = 8;
    }
}

static void ObjModoWait(OBJWORK *objw) {
    // Empty function
}

static void ObjWait(OBJWORK *objw) {
    if (objw->hitram == 4) {
        objw->bun0 = 5;
        objw->hitram = 0;
    } else {
        objw->posi[1] = 28672.0f;
    }
}

static void ObjInit2(OBJWORK *objw) {
    BUKI_WORK *bukiw = objw->work;

    if (objw->reg7 & 2) {
        objw->freg3 = 80.0f;
    } else {
        objw->freg3 = 48.0f;
    }
    bukiw->kaku_flag = 1;
    bukiw->timer = 0;
    bukiw->count = 0;
    objw->reg0 = objw->reg1 = 0;
    objw->reg2 = 0x5000;
    objw->freg1 = 0.0f;
    objw->bun0 = 1;
    abKazdSet(objw);
}

static void (*ObjFuncTbl[6])(OBJWORK *objw) = {
    ObjInit,
    ObjIki,
    ObjModori,
    ObjModoWait,
    ObjWait,
    ObjInit2
};

void ObjKazd(OBJWORK *objw) {
    ObjFuncTbl[objw->bun0](objw);
}

void SetKazd(OBJWORK *objkl) {
    OBJWORK *buki = GameGbl.kazd;
    HERO_WORK *herow;

    if (buki->hitram != 8) {
        return;
    }

    buki->hitram = 4;
    herow = objkl->work;
    sceVu0CopyVector(buki->posi, objkl->posi);
    if (objkl->bun1 == 4) {
        buki->posi[0] -= herow->slant[0] * 41.0f;
        buki->posi[1] -= herow->slant[1] * 41.0f;
        buki->posi[2] -= herow->slant[2] * 41.0f;
    } else {
        if (objkl->reg0 & 0x400) {
            buki->posi[1] += 25.0f;
        } else {
            buki->posi[1] -= 25.0f;
        }
    }
    buki->reg7 = herow->buki_muki;
    hSeKeyOn(0x4b04b00ca10000, buki->posi, 0);
    if (herow->buki_muki & 2) {
        GetTaniVector(objkl->posi, GameGbl.cam.posi, buki->muki);
        if (herow->buki_muki & 1) {
            buki->muki[0] = -buki->muki[0];
            buki->muki[2] = -buki->muki[2];
        }
    } else {
        SetTaniVector(buki->muki, objkl->muki);
        buki->rtw = objkl->rtw;
    }
}