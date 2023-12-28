#include "nakano/klopipe.h"
#include "nakano/gmiss.h"
#include "nakano/vu0mac.h"
#include "nakano/route.h"
#include "nakano/herosub.h"
#include "nakano/k_math.h"
#include "hoshino/h_gamesnd.h"
#include "harada/hr_pall.h"

static sceVu0FVECTOR klpJyuVec;

void nkKpGetJyuryoku(sceVu0FVECTOR vec) {
    sceVu0Normalize(klpJyuVec, vec);
}

void nkKpInitJyuryoku() {
    klpJyuVec[0] = 0.0f;
    klpJyuVec[1] = 1.0f;
    klpJyuVec[2] = 0.0f;
}

static f32 KpAshi_HitSub(OBJWORK *objw, sceVu0FVECTOR hitposi, OBJWORK **yukaobj, s16 *kage_flag, s32 *phcode, s32 jflag) {
    f32 cros;
    f32 work;
    sceVu0FVECTOR vec;
    sceVu0FVECTOR nor;
    sceVu0FMATRIX mat;
    sceVu0FVECTOR hit_nor;
    HERO_WORK *herow = objw->work;
    u32 work2;

    objw->reg4 = 0;
    *kage_flag = 0;
    *phcode = 0;

    vec[0] = 0.0f;
    vec[1] = -64.0f;
    vec[2] = 0.0f;
    vec[3] = 1.0f;
    nor[0] = 0.0f;
    if (!jflag) {
        nor[1] = herow->jumpspd_limit;
    } else {
        nor[1] = -16.0f;
    }
    nor[2] = 0.0f;
    nor[3] = 1.0f;

    sceVu0UnitMatrix(mat);
    sceVu0RotMatrixY(mat, mat, -objw->freg1);
    sceVu0RotMatrixZ(mat, mat, -objw->freg6);
    sceVu0RotMatrixY(mat, mat, -objw->freg5);
    sceVu0ApplyMatrix(vec, mat, vec);
    sceVu0ApplyMatrix(nor, mat, nor);
    sceVu0AddVector(vec, vec, hitposi);
    sceVu0AddVector(nor, nor, hitposi);
    cros = RayCrsJudgzNLcol(vec, nor, &work2, hit_nor);
    work = 80.0f;
    if (cros > work) {
        cros = 65535.0f;
    } else {
        *phcode = work2;
    }
    return cros;
}

static float KpAshi_HitCheck(OBJWORK *objw, s32 *retflag, s32 *kage_flag, s32 jflag) {
    sceVu0FVECTOR hitposi;
    sceVu0FVECTOR hitposi2;
    f32 retposi;
    f32 retposi2;
    OBJWORK *yukaobj;
    s32 phcode1;
    s32 phcode2;
    s16 kage_f;
    sceVu0FMATRIX mat;
    HERO_WORK *herow = objw->work;

    phcode1 = phcode2 = 0;
    
    hitposi[0] = 0.0f;
    hitposi[1] = 0.0f;
    hitposi[2] = 8.75f;
    sceVu0UnitMatrix(mat);
    sceVu0RotMatrixY(mat, mat, -objw->freg1);
    sceVu0RotMatrixZ(mat, mat, -objw->freg6);
    sceVu0RotMatrixY(mat, mat, -objw->freg5);
    sceVu0ApplyMatrix(hitposi, mat, hitposi);
    sceVu0AddVector(hitposi, hitposi, objw->posi);
    retposi = KpAshi_HitSub(objw, hitposi, &yukaobj, &kage_f, &phcode1, jflag);
    
    hitposi2[0] = 0.0f;
    hitposi2[1] = 0.0f;
    hitposi2[2] = -8.75f;
    sceVu0ApplyMatrix(hitposi2, mat, hitposi2);
    sceVu0AddVector(hitposi2, hitposi2, objw->posi);
    retposi2 = KpAshi_HitSub(objw, hitposi2, &yukaobj, &kage_f, &phcode2, jflag);
    
    if (!(retposi <= retposi2)) {
        retposi = retposi2;
        phcode2 = phcode1;
    }
    if (retposi != 65535.0f) {
        herow->f_phcode = phcode1;
        herow->yuka_hit = 2;
    }
    return retposi;
}

static void KpHero_JumpDown_Sub(OBJWORK *objw, HERO_WORK *herow, s32 jflag) {
    f32 dy;
    sceVu0FVECTOR vf;
    sceVu0FMATRIX mat;
    sceVu0FVECTOR v0;
    sceVu0FVECTOR v1;

    if (objw->bun0 == 27 || objw->bun0 == 40) {
        dy = 65535.0f;
    } else {
        dy = KpAshi_HitCheck(objw, &herow->retflag, &herow->kage_flag, jflag);
    }

    if (herow->rakka_cnt > 1800) {
        nkKloPhCode01(objw, herow);
        return;
    }
    if (!(dy < 65535.0f)) {
        return;
    }
    if (herow->f_phcode == 0xFF0000) {
        nkKloPhCode01(objw, herow);
        return;
    }

    if (GameGbl.kloLife[herow->con] <= 0 && objw->bun0 != 34) {
        nkKloNoLife(objw, herow);
    } else if (objw->bun0 == 7) {
        objw->bun0 = 6;
    } else {
        herow->timer = 0;
    }

    herow->spflag2 &= -6;
    objw->reg0 &= -9;
    herow->timer = 0;

    sceVu0UnitMatrix(mat);
    sceVu0RotMatrixY(mat, mat, -objw->freg1);
    sceVu0RotMatrixZ(mat, mat, -objw->freg6);
    sceVu0RotMatrixY(mat, mat, -objw->freg5);
    vf[0] = 0.0f;
    vf[1] = dy - 64.0f;
    vf[2] = 0.0f;
    vf[3] = 1.0f;
    sceVu0ApplyMatrix(vf, mat, vf);
    sceVu0AddVector(objw->posi, objw->posi, vf);
    v0[0] = objw->posi[0];
    v0[1] = objw->posi[1];
    v0[2] = objw->posi[2];
    v0[3] = 1.0f;
    v1[0] = v0[0] - vf[0] * 64.0f;
    v1[1] = v0[1] - vf[1] * 64.0f;
    v1[2] = v0[2] - vf[2] * 64.0f;
    v1[3] = 1.0f;
    objw->spd[1] = 0.0f;
}

void KloSlidePipe(OBJWORK *objw) {
    HERO_WORK *herow;
    sceVu0FVECTOR ang;
    sceVu0FVECTOR slant;
    sceVu0FVECTOR kspd;
    sceVu0FVECTOR vec;
    f32 dist;
    s32 trg2;
    f32 var_f21;
    sceVu0FVECTOR vf0;
    sceVu0FVECTOR vf1;
    f32 wk;
    sceVu0FVECTOR vf;
    sceVu0FMATRIX tm;
    f32 wf;
    sceVu0FMATRIX mat;
    f32 angy;
    f32 angz;
    sceVu0FVECTOR v0;
    sceVu0FVECTOR v1;

    var_f21 = 0.0f;
    herow = objw->work;
    herow->gimm_flag &= -0x1000001;

    if (hr_pt_check() == 1) {
        if (herow->se_id6 != -1) {
            hSeKeyOff(herow->se_id6);
            herow->se_id6 = -1;
        }
        if (herow->se_id7 != -1) {
            hSeKeyOff(herow->se_id7);
            herow->se_id7 = -1;
        }
        return;
    }

    if (herow->se_id6 == -1) {
        if (GameGbl.vision >> 8 != 1) {
            herow->se_id6 = hSeKeyOn(0x4b04b00da1a100, objw->posi, 0);
        }
    } else {
        vu0_Square(vec, objw->spd);
        hSePitch(herow->se_id6, vu0_SquareRoot(vec[0] + vec[2]) * 50.0f);
    }

    if (herow->gimm_flag & 0x1000) {
        herow->lvl = herow->ex_lvl;
        herow->trg = herow->ex_trg;
        objw->hitram = 0;
    } else {
        herow->lvl = GameGbl.kpd[0].lvl;
        herow->trg = GameGbl.kpd[0].trg;
    }

    if (GameGbl.kloLife[herow->con] <= 0) {
        if (objw->bun0 == 34) {
            Hero_LifeLost_Sub(objw, herow);
        }
        objw->hitram = 0;
        herow->lvl = 0;
        herow->trg = 0;
    }

    if (objw->bun0 == 12) {
        Hero_Dead_Sub(objw, herow);
    }

    if (herow->da_muteki > 0) {
        herow->da_muteki--;
    }
    if (herow->tenmetu > 0) {
        herow->tenmetu--;
    }

    if (objw->hitram & 3) {
        if (herow->da_muteki <= 0) {
            objw->bun0 = 10;
            herow->tenmetu = herow->da_muteki = 60;
            herow->count = 0;
            hSeKeyOn(0xc80c80ca20107, objw->posi, 0);
            if (GameGbl.kloLife[herow->con] > 0) {
                GameGbl.kloLife[herow->con]--;
            }
        }
        objw->hitram &= -4;
    }

    if (objw->bun0 == 10 && herow->count > 91) {
        objw->bun0 = 11;
    }

    GetMichiVec(&objw->rtw, objw->muki);
    herow->buki_muki = 0;
    if (herow->lvl & herow->right_move) {
        if (objw->bun0 != 6) {
            herow->buki_muki = 1;
        }
    } else if (herow->lvl & herow->left_move) {
        if (objw->bun0 != 6) {
            herow->buki_muki = 2;
        }
    } else if (pPAD_LVL_DOWN(herow)) {
        if (objw->bun0 != 6) {
            herow->buki_muki = 3;
        }
    }
    sceVu0CopyVector(vf0, objw->posi);
    vf0[0] -= 16.0f * klpJyuVec[0];
    vf0[1] -= 16.0f * klpJyuVec[1];
    vf0[2] -= 16.0f * klpJyuVec[2];
    vf1[0] = klpJyuVec[0];
    vf1[1] = klpJyuVec[1];
    vf1[2] = klpJyuVec[2];
    dist = sbRayCrsJudgzN(slant, vf0, vf1, 256.0f);
    if (dist != 65535.0f) {
        sceVu0CopyVector(klpJyuVec, slant);
        sceVu0CopyVector(herow->slant, slant);
    }
    ang[1] = -_atan2(objw->muki[0], objw->muki[2]);
    if (ang[1] < -3.141592f) {
        ang[1] += 6.283184f;
    } else if (ang[1] > 3.141592f) {
        ang[1] -= 6.283184f;
    }

    if (herow->lvl & herow->right_move) {
        objw->freg1 -= 0.104719736f;
        if (objw->freg1 < -0.6283184f) {
            objw->freg1 = -0.6283184f;
        }
        if (var_f21 != 0.0f) {
            nkKloKsslwt(objw);
        }
    } else if (herow->lvl & herow->left_move) {
        objw->freg1 += 0.104719736f;
        if (objw->freg1 > 0.6283184f) {
            objw->freg1 = 0.6283184f;
        }
        if (var_f21 != 0.0f) {
            nkKloKsslwt(objw);
        }
    } else if (objw->freg1 > 0.026179934f) {
        objw->freg1 -= 0.026179934f;
    } else if (objw->freg1 < -0.026179934f) {
        objw->freg1 += 0.026179934f;
    } else {
        objw->freg1 = 0.0f;
    }
    objw->freg5 = ang[1];

    angy = ang[1] - objw->rot[1];
    if (angy < -3.141592f) {
        angy += 6.283184f;
    } else if (angy > 3.141592f) {
        angy -= 6.283184f;
    }
    objw->rot[1] += angy / 8.0f;
    if (objw->rot[1] < -3.141592f) {
        objw->rot[1] += 6.283184f;
    } else if (objw->rot[1] > 3.141592f) {
        objw->rot[1] -= 6.283184f;
    }

    if (dist != 65535.0f) {
        wf = objw->rot[1] + 3.141592f;
        if (wf < -3.141592f) {
            wf += 6.283184f;
        } else if (wf > 3.141592f) {
            wf -= 6.283184f;
        }
        sceVu0UnitMatrix(tm);
        sceVu0RotMatrixY(tm, tm, wf);
        sceVu0ApplyMatrix(vf, tm, slant);
        ang[2] = _atan2(vf[1], vf[0]) - 1.570796f;
        if (ang[2] < -3.141592f) {
            ang[2] += 6.283184f;
        } else if (ang[2] > 3.141592f) {
            ang[2] -= 6.283184f;
        }

        wf = ang[2] - objw->rot[2];
        if (wf < -3.141592f) {
            wf += 6.283184f;
        } else if (wf > 3.141592f) {
            wf -= 6.283184f;
        }
        objw->rot[2] += wf / 8.0f;
        objw->freg6 = ang[2];

        wf = vf[0] * vf[0] + vf[1] * vf[1];
        wf = vu0_SquareRoot(wf);
        ang[0] = _atan2(vf[2], wf);
        if (ang[0] < -3.141592f) {
            ang[0] += 6.283184f;
        } else if (ang[0] > 3.141592f) {
            ang[0] -= 6.283184f;
        }
        wf = ang[0] - objw->rot[0];
        if (wf < -3.141592f) {
            wf += 6.283184f;
        } else if (wf > 3.141592f) {
            wf -= 6.283184f;
        }
        objw->rot[0] += wf / 8.0f;
        objw->freg4 = 0.0f;
    }

    if (objw->rot[0] < -3.141592f) {
        objw->rot[0] += 6.283184f;
    } else if (objw->rot[0] > 3.141592f) {
        objw->rot[0] -= 6.283184f;
    }
    if (objw->rot[1] < -3.141592f) {
        objw->rot[1] += 6.283184f;
    } else if (objw->rot[1] > 3.141592f) {
        objw->rot[1] -= 6.283184f;
    }
    if (objw->rot[2] < -3.141592f) {
        objw->rot[2] += 6.283184f;
    } else if (objw->rot[2] > 3.141592f) {
        objw->rot[2] -= 6.283184f;
    }

    kspd[0] = 0.0f;
    kspd[1] = 0.0f;
    kspd[2] = 0.4f;
    kspd[3] = 1.0f;

    if (pPAD_LVL_UP(herow)) {
        kspd[2] += 0.3f;
    } else if (pPAD_LVL_DOWN(herow)) {
        kspd[2] -= 0.1f;
    }

    angy = -ang[1];
    if (angy < -3.141592f) {
        angy += 2.0f;
    } else if (angy > 3.141592f) {
        angy -= 2.0f;
    }
    angz = objw->rot[2];
    if (angz < -3.141592f) {
        angz += 6.283184f;
    } else if (angz > 3.141592f) {
        angz -= 6.283184f;
    }
    sceVu0UnitMatrix(mat);
    sceVu0RotMatrixY(mat, mat, -objw->freg1);
    sceVu0RotMatrixX(mat, mat, -objw->rot[0]);
    sceVu0RotMatrixZ(mat, mat, -angz);
    angy = _atan2(objw->muki[0], objw->muki[2]);
    sceVu0RotMatrixY(mat, mat, angy);
    objw->freg1 = nkRadMask(objw->freg1);
    sceVu0ApplyMatrix(kspd, mat, kspd);

    if (herow->trg & GameGbl.k_jump_button && objw->spd[1] == 0.0f) {
        hSeKeyOn(0x4b04b00ca0a206, objw->posi, 0);
        objw->spd[1] = -9.75f;
        trg2 = 0;
    } else {
        trg2 = herow->trg;
    }

    if (herow->lvl & GameGbl.k_jump_button && objw->bun0 != 40 && !(herow->spflag2 & 1) && objw->spd[1] > 2.0f && !(objw->reg0 & 1)) {
        herow->se_id9 = hSeKeyOn(0xc80c80da10004, objw->posi, 0);
        objw->bun0 = 20;
        herow->spflag2 |= 1;
        objw->spd[1] = 1.0483398f;
        herow->count = 0;
    }

    if (herow->se_id9 != -1 && objw->bun0 != 20) {
        hSeKeyOff(herow->se_id9);
        herow->se_id9 = -1;
    }

    if (pPAD_TRG_L1(herow)) {
        herow->b_action = 1;
        herow->b_act_time = 0;
    }
    if (pPAD_TRG_L2(herow)) {
        herow->b_action = 2;
        herow->b_act_time = 0;
    }

    sbbHero_Jump_Sub(objw, herow);

    if (objw->spd[1] != 0.0f || objw->reg0 & 0x100 || objw->bun0 == 40) {
        if (objw->reg0 & 1) {
            if (objw->bun0 != 10 || !(objw->spd[1] < 0.0f)) {
                if (herow->trg & GameGbl.k_atck_button && !(herow->gimm_flag & 0x10000)) {
                    herow->spflag &= -2;
                    objw->bun0 = 9;
                    herow->mochifuku->reg2 |= 0x40;
                    herow->timer = 0;
                    herow->count = 0;
                } else if ((trg2 & GameGbl.k_jump_button || objw->reg0 & 0x20) && !(herow->spflag & 1) && !(objw->reg0 & 8) && !(herow->gimm_flag & 0x8000)) {
                    objw->bun0 = 27;
                    herow->sjump_flag = objw->posi[1];
                    SayWahoo(objw, herow);
                    objw->reg0 &= -0x125;
                    objw->spd[1] = -1.0f;
                    herow->sjump_yspd = -7.2087765f;
                    herow->timer = 0;
                    herow->count = 0;
                } else if (objw->bun0 == 2) {
                    objw->bun0 = 15;
                    herow->count = 0;
                }
            }
        } else {
            if (objw->bun0 == 15) {
                objw->bun0 = 2;
            }
            if ((objw->bun0 != 10 || !(objw->spd[1] < 0.0f)) && objw->bun0 != 7 && objw->bun0 != 27 && objw->bun0 != 40 && herow->trg & GameGbl.k_atck_button) {
                SetKazd(objw);
                objw->bun0 = 7;
                herow->timer = 0;
                herow->count = 0;
            }
        }

        if (objw->bun0 == 27) {
            Hero_SJump_Sub(objw, herow);
        }
    } else if (objw->bun0 != 6 && objw->bun0 != 35 && objw->bun0 != 40 && herow->trg & GameGbl.k_atck_button) {
        herow->timer = 0;
        if (!(objw->reg0 & 1)) {
            SetKazd(objw);
            objw->bun0 = 6;
            herow->count = 0;
        } else {
            herow->count = 0;
            herow->mochifuku->reg2 |= 0x40;
            if (herow->michispd != 0) {
                objw->bun0 = 18;
            } else {
                objw->bun0 = 8;
            }
        }
    }

    if ((objw->bun0 == 7 || objw->bun0 == 9) && herow->count > 15) {
        objw->bun0 = 11;
    }
    if ((objw->bun0 == 6 || objw->bun0 == 8) && herow->count > 15) {
        objw->bun0 = 11;
    }

    MichiBunki2(&objw->rtw, objw->posi);
    objw->spd[0] += kspd[0];
    objw->spd[2] += kspd[2];
    objw->freg2 += kspd[1];
    objw->posi[0] += objw->spd[0];
    objw->posi[1] += objw->freg2;
    objw->posi[2] += objw->spd[2];
    if (objw->spd[1] > 0.0f) {
        KpHero_JumpDown_Sub(objw, herow, 0);
    } else {
        KpHero_JumpDown_Sub(objw, herow, 1);
    }
    if (objw->spd[1] <= 0.0f) {
        herow->rakka_cnt = 0;
    }
    objw->posi[0] += klpJyuVec[0] * objw->spd[1];
    objw->posi[1] += klpJyuVec[1] * objw->spd[1];
    objw->posi[2] += klpJyuVec[2] * objw->spd[1];
    if (objw->bun0 == 20 && herow->count < 25) {
        objw->spd[0] /= 1.08f;
        objw->spd[2] /= 1.08f;
    }
    objw->spd[0] /= 1.03f;
    objw->freg2 /= 1.03f;
    objw->spd[2] /= 1.03f;

    herow->count++;
    herow->dead_cnt++;
    herow->area_cnt++;
    herow->jmp_cnt++;
    herow->rakka_cnt++;
    objw->rtw.mcn = GetMichinori(&objw->rtw);
}
