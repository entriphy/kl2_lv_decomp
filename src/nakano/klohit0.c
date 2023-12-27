#include "nakano/klohit0.h"
#include "nakano/vu0mac.h"
#include "nakano/route.h"
#include "nakano/gmiss.h"
#include "nakano/herosub.h"
#include "hoshino/h_gamesnd.h"

void HeroKabeCheck(OBJWORK *objw, HERO_WORK *herow) {
    f32 work;
    f32 work2;
    s32 hosei;
    OBJWORK *kabeobj = NULL;
    sceVu0FVECTOR hitposi;
    sceVu0FVECTOR vec2;
    sceVu0FVECTOR vec3;
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;

    hitposi[0] = objw->ang[0];
    hitposi[1] = 0.0f;
    hitposi[2] = objw->ang[2];
    sceVu0Normalize(vec2, hitposi);

    hitposi[0] = objw->posi[0];
    hitposi[1] = objw->posi[1] - 25.5f;
    hitposi[2] = objw->posi[2];
    work2 = RayCrsJudgzN(hitposi, vec2);
    if (!(work2 <= 12.5f) || !(work2 >= 0.0f)) {
        work2 = 65535.0f;
    }

    vec3[0] = vec2[0] * 32.0f + hitposi[0];
    vec3[1] = hitposi[1];
    vec3[2] = vec2[2] * 32.0f + hitposi[2];
    vec3[3] = hitposi[3] = 1.0f;
    nkObjPolyHit(hitposi, vec3, &kabeobj);

    if (!(work2 < 65535.0f)) {
        hitposi[1] = objw->posi[1] - 40.0f;
        work2 = RayCrsJudgzN(hitposi, vec2);
        if (!(10.0f >= work2) || !(work2 >= 0.0f)) {
            work2 = 65535.0f;
        }
        vec3[1] = hitposi[1];
        nkObjPolyHit(hitposi, vec3, &kabeobj);

        if (work2 < 65535.0f) {
            if (work2 < 65535.0f) {
                vf[0] = work2;
                vu0_FTOI12Vector(vi, vf);
                hosei = vi[0];
                kabeobj = NULL;
            } else {
                vf[0] = 65535.0f;
                vu0_FTOI12Vector(vi, vf);
                hosei = vi[0];
            }
            objw->reg6 = 0x9000 - hosei;
        } else {
            hitposi[1] = objw->posi[1] - 11.0f;
            work2 = RayCrsJudgzN(hitposi, vec2);
            if (!(8.0f >= work2) || !(work2 >= 0.0f)) {
                work2 = 65535.0f;
            }
            vec3[1] = hitposi[1];
            nkObjPolyHit(hitposi, vec3, &kabeobj);
            if (!(work2 < 65535.0f)) {
                return;
            }
            if (work2 < 65535.0f) {
                vf[0] = work2;
                vu0_FTOI12Vector(vi, vf);
                hosei = vi[0];
                kabeobj = NULL;
            } else {
                vf[0] = 65535.0f;
                vu0_FTOI12Vector(vi, vf);
                hosei = vi[0];
            }
            objw->reg6 = 0x7000 - hosei;
        }
    }

    herow->michispd = 0;
    herow->mispd = 0;
    if (objw->bun0 == 1) {
        objw->bun0 = 22;
    }
}

void HeroKabeCheck2(OBJWORK *objw, HERO_WORK *herow) {
    f32 work;
    s32 hosei;
    s32 mispd2;
    OBJWORK *kabeobj;
    sceVu0FVECTOR posi;
    sceVu0FVECTOR muki;
    sceVu0FVECTOR hp0;
    sceVu0FVECTOR hp1;
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;
    s32 i;

    mispd2 = herow->mispd;
    hosei = 0;

    sceVu0CopyVector(posi, objw->posi);
    sceVu0CopyVector(muki, objw->muki);
    muki[3] = 1.0f;
    if (herow->buki_muki == 1) {
        muki[0] = -muki[0];
        muki[2] = -muki[2];
    }
    sceVu0Normalize(muki, muki);

    for (i = 0; i < 2; i++) {
        if (objw->reg0 & 0x400) {
            hp0[0] = posi[0];
            hp0[1] = posi[1] + 25.5f;
            hp0[2] = posi[2];
            hp0[3] = 1.0f;
        } else {
            hp0[0] = posi[0];
            hp0[1] = posi[1] - 25.5f;
            hp0[2] = posi[2];
            hp0[3] = 1.0f;
        }
        hp1[0] = muki[0] * 32.0f + hp0[0];
        hp1[1] = hp0[1];
        hp1[2] = muki[2] * 32.0f + hp0[2];
        hp1[3] = 1.0f;

        work = nkObjPolyHit(hp0, hp1, &kabeobj);
        if (!(work >= -6.0f) || !(work <= 12.5f)) {
            work = 65535.0f;
        } else {
            work = 12.5f - work;
        }
        if (work == 65535.0f) {
            if (objw->reg0 & 0x400) {
                hp0[1] = hp1[1] = posi[1] + 40.0f;
            } else {
                hp0[1] = hp1[1] = posi[1] - 40.0f;
            }

            work = nkObjPolyHit(hp0, hp1, &kabeobj);
            if (!(work >= -6.0f) || !(work <= 11.25f)) {
                work = 65535.0f;
            } else {
                work = 11.25f - work;
            }
            if (work == 65535.0f) {
                if (objw->reg0 & 0x400) {
                    hp0[1] = hp1[1] = posi[1] + 11.0f;
                } else {
                    hp0[1] = hp1[1] = posi[1] - 11.0f;
                }

                work = nkObjPolyHit(hp0, hp1, &kabeobj);
                if (!(work >= -6.0f) || !(work <= 8.75f)) {
                    work = 65535.0f;
                } else {
                    work = 8.75f - work;
                }
            }
        }

        if (work < 65535.0f) {
            vf[0] = work;
            vu0_FTOI12Vector(vi, vf);
            hosei = vi[0];
            if (i == 0) {
                hosei = -hosei;
            }
            break;
        }
        muki[0] = -muki[0];
        muki[2] = -muki[2];
    }

    if (kabeobj == NULL || !(work < 65535.0f)) {
        return;
    }

    if ((i == 0 && mispd2 > 0 && mispd2 > kabeobj->reg3) || (i == 1 && mispd2 < 0 && mispd2 < kabeobj->reg3)) {
        if (kabeobj->reg3 == 0) {
            herow->michispd = 0;
            herow->mispd = 0;
            objw->reg6 = 0;
            herow->gimspd = 0;
        } else {
            herow->michispd = 0;
            herow->mispd = kabeobj->reg3;
            objw->reg6 = hosei;
            herow->gimspd = 0;
        }
    } else if ((mispd2 >= 0 && mispd2 < kabeobj->reg3) || (mispd2 <= 0) && mispd2 > kabeobj->reg3) {
        herow->michispd = herow->mispd = 0;
        objw->reg6 = hosei;
        herow->gimspd = 0;
    } else {
        // what
        herow->michispd = herow->mispd = 0;
        objw->reg6 = hosei;
        herow->gimspd = 0;
    }
}

void HeroKabeCheck3(OBJWORK *objw, HERO_WORK *herow) {
    f32 work;
    s32 hosei;
    s32 mispd2;
    sceVu0FVECTOR posi;
    sceVu0FVECTOR muki;
    sceVu0FVECTOR hp0;
    sceVu0FVECTOR hp1;
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;
    s32 i;

    mispd2 = herow->mispd;
    sceVu0CopyVector(posi, objw->posi);
    sceVu0CopyVector(muki, objw->muki);
    muki[3] = 1.0f;
    if (objw->bun1 != 2 && herow->buki_muki == 1) {
        muki[0] = -muki[0];
        muki[2] = -muki[2];
    }
    if (objw->bun0 == 10) {
        mispd2 += objw->reg3;
    }
    if (herow->Muki != 0) {
        GetMichiVec(&objw->rtw, muki);
    }

    i = 0;
    if (herow->movebox != NULL) {
        if (mispd2 + herow->gimspd + objw->reg6 < 0) {
            i = 1;
            muki[0] = -muki[0];
            muki[2] = -muki[2];
        }
    } else if (mispd2 + objw->reg6 < 0) {
        i = 1;
        muki[0] = -muki[0];
        muki[2] = -muki[2];
    }
    sceVu0Normalize(muki, muki);

    if (objw->reg0 & 0x400) {
        hp0[0] = posi[0];
        hp0[1] = posi[1] + 25.5f;
        hp0[2] = posi[2];
        hp0[3] = 1.0f;
    } else {
        hp0[0] = posi[0];
        hp0[1] = posi[1] - 25.5f;
        hp0[2] = posi[2];
        hp0[3] = 1.0f;
    }
    hp1[0] = muki[0] * 32.0f + hp0[0];
    hp1[1] = muki[1] * 32.0f + hp0[1];
    hp1[2] = muki[2] * 32.0f + hp0[2];
    hp1[3] = 1.0f;

    work = RayCrsJudgzNL(hp0, hp1);
    if (!(work >= -6.0f) || !(work <= 12.5f)) {
        work = 65535.0f;
    } else {
        work = 0.0f;
    }
    if (work == 65535.0f) {
        if (objw->reg0 & 0x400) {
            hp0[1] = hp1[1] = posi[1] + 40.0f;
        } else {
            hp0[1] = hp1[1] = posi[1] - 40.0f;
        }
        hp1[1] += muki[1] * 32.0f;

        work = RayCrsJudgzNL(hp0, hp1);
        if (!(work >= -6.0f) || !(work <= 11.25f)) {
            work = 65535.0f;
        } else {
            work = 11.25f - work;
        }
        if (work == 65535.0f) {
            if (objw->reg0 & 0x400) {
                hp0[1] = hp1[1] = posi[1] + 11.0f;
            } else {
                hp0[1] = hp1[1] = posi[1] - 11.0f;
            }
            hp1[1] += muki[1] * 32.0f;

            work = RayCrsJudgzNL(hp0, hp1);
            if (!(work >= -32.0f) || !(work <= 8.75f)) {
                work = 65535.0f;
            } else {
                work = 8.75f - work;
            }
        }
    }

    if (work < 65535.0f) {
        vf[0] = work;
        vu0_FTOI12Vector(vi, vf);
        hosei = vi[0];
        if (i == 0) {
            hosei = -hosei;
        }
        objw->reg6 = hosei;
        herow->michispd = 0;
        herow->mispd = 0;
        herow->gimspd = 0;
        objw->reg3 = 0;
    }
}

f32 Ashi_HitSub(OBJWORK *objw, sceVu0FVECTOR hitposi, s32 *yflag, OBJWORK **yukaobj, s16 *kage_flag, u32 *phcode) {
    s32 i;
    u32 work2;
    f32 var_f21;
    f32 dy;
    f32 cros;
    f32 work;
    f32 y_kage;
    sceVu0FVECTOR vec;
    sceVu0FVECTOR nor;
    sceVu0FVECTOR hit_nor;
    HERO_WORK *herow;
    OBJWORK *teki2;

    herow = objw->work;
    *yukaobj = NULL;
    objw->reg4 = 0;
    *kage_flag = 0;
    *phcode = 0;

    if (objw->bun1 >= 3) {
        var_f21 = 48.0f;
    } else if (objw->bun1 == 2) {
        var_f21 = 48.0f;
    } else {
        var_f21 = 32.0f;
    }

    vec[0] = hitposi[0];
    if (objw->reg0 & 0x400) {
        vec[1] = hitposi[1] + var_f21;
    } else {
        vec[1] = hitposi[1] - var_f21;
    }
    vec[2] = hitposi[2];
    vec[3] = 1.0f;

    nor[0] = vec[0];
    if (objw->reg0 & 0x400) {
        nor[1] = vec[1] - herow->jumpspd_limit - (var_f21 + herow->tenjo_hosei);
    } else {
        nor[1] = vec[1] + herow->jumpspd_limit + var_f21 + herow->tenjo_hosei;
    }
    nor[2] = vec[2];
    vec[3] = 1.0f; // ?
    cros = nkObjPolyHit(vec, nor, yukaobj);

    i = 0;
    while ((teki2 = OkGetOBJWORKD(3, 0x2000, i, &i)) != NULL) {
        NoriHitCheck4X(hitposi, teki2);
    }
    i = 0;
    while ((teki2 = OkGetOBJWORKD(3, 0x2003, i, &i)) != NULL) {
        NoriHitCheck4X(hitposi, teki2);
    }

    vec[0] = hitposi[0];
    if (objw->reg0 & 0x400) {
        vec[1] = hitposi[1] + var_f21;
    } else {
        vec[1] = hitposi[1] - var_f21;
    }
    vec[2] = hitposi[2];

    nor[0] = vec[0];
    if (objw->reg0 & 0x400) {
        nor[1] = vec[1] - (herow->jumpspd_limit + var_f21 + herow->tenjo_hosei);
    } else {
        nor[1] = vec[1] + herow->jumpspd_limit + var_f21 + herow->tenjo_hosei;
    }
    nor[2] = vec[2];

    dy = RayCrsJudgzNLcol(vec, nor, &work2, hit_nor);
    if (dy != 65535.0f) {
        if (objw->reg0 & 0x400) {
            dy = hitposi[1] - dy + var_f21;
        } else {
            dy = dy + (hitposi[1] - var_f21);
        }
    }
    if (cros != 65535.0f) {
        if (objw->reg0 & 0x400) {
            cros = hitposi[1] - cros + var_f21;
        } else {
            cros = cros + (hitposi[1] - var_f21);
        }
    }

    y_kage = cros;
    if (objw->reg0 & 0x400) {
        if (cros > vec[1]) {
            y_kage = cros = 65535.0f;
        }
    } else {
        if (cros < vec[1]) {
            y_kage = cros = 65535.0f;
        }
    }

    if (objw->spd[1] != 0.0f) {
        if (objw->bun1 == 3) {
            work = herow->jumpspd_limit;
        } else {
            work = herow->jumpspd_limit + herow->tenjo_hosei;
        }
    } else {
        if (objw->bun1 == 2 || objw->bun1 == 3) {
            work = 32.0f;
        } else {
            work = 3.0f;
        }
    }
    if (y_kage <= hitposi[1] + work) {
        *kage_flag = 1;
    }

    i = 0;
    if (objw->reg0 & 0x400) {
        if (dy >= hitposi[1] - work && dy != 65535.0f) {
            i = 1;
        }
    } else {
        if (dy <= hitposi[1] + work) {
            i = 1;
        }
    }
    if (i != 0) {
        *phcode = work2;
    } else {
        dy = 65535.0f;
    }

    i = 0;
    if (objw->reg0 & 0x400) {
        if (cros >= hitposi[1] - work && cros != 65535.0f) {
            i = 1;
        }
    } else {
        if (cros <= hitposi[1] + work) {
            i = 1;
        }
    }
    if (i == 0) {
        cros = 65535.0f;
    }

    *yflag = 0;
    if (dy != 65535.0f) {
        *yflag = 2;
    }

    i = 0;
    if (objw->reg0 & 0x400) {
        if ((dy < cros || dy == 65535.0f) && cros != 65535.0f) {
            i = 1;
        }
    } else {
        if (dy > cros) {
            i = 1;
        }
    }

    if (i != 0) {
        dy = cros;
        *yflag = 3;
        *phcode = 0xFFFFFF;
    }
    if (dy == 65535.0f) {
        objw->posi[1] += herow->tenjo_hosei;
    }
    if (*yflag == 1) {
        *kage_flag = 0;
    }
    return dy;
}

f32 Ashi_HitCheck(OBJWORK *objw, s32 *retflag, f32 saka, s32 *kage_flag) {
    s32 i;
    sceVu0FVECTOR hitposi;
    sceVu0FVECTOR hitposi2;
    f32 retposi;
    f32 retposi2;
    s32 yflag;
    s32 yflag2;
    OBJWORK *yukaobj;
    OBJWORK *yukaobj2;
    s32 hosei;
    u32 phcode1;
    u32 phcode2;
    s16 kage_f;
    s16 kage_f2;
    HERO_WORK *herow = objw->work;
    s32 var_a0;

    hitposi[0] = objw->posi[0] + objw->muki[0] * 8.75f;
    hitposi[2] = objw->posi[2] + objw->muki[2] * 8.75f;
    hitposi[1] = objw->posi[1];
    retposi = Ashi_HitSub(objw, hitposi, &yflag, &yukaobj, &kage_f, &phcode1);
    hitposi2[0] = objw->posi[0] - objw->muki[0] * 8.75f;
    hitposi2[2] = objw->posi[2] - objw->muki[2] * 8.75f;
    hitposi2[1] = objw->posi[1];
    retposi2 = Ashi_HitSub(objw, hitposi2, &yflag2, &yukaobj2, &kage_f2, &phcode2);

    herow->f_phcode = 0;
    var_a0 = 0;
    if (objw->reg0 & 0x400) {
        if (retposi2 == 65535.0f) {
            var_a0 = 1;
        } else if (retposi == 65535.0f) {
            var_a0 = 0;
        } else if (retposi >= retposi2) {
            var_a0 = 1;
        }
    } else {
        if (retposi <= retposi2) {
            var_a0 = 1;
        }
    }
    if (var_a0) {
        *kage_flag = kage_f;
        herow->f_phcode = phcode1;
        if (herow->f_phcode != 0xFFFF00 && herow->f_phcode != 0xFF0000 && retposi != 65535.0f && HeroRootChangeSub(objw, hitposi, yflag)) {
            objw->rtw.mcn = GetMichinori(&objw->rtw);
            if (herow->buki_muki == 1 || (herow->buki_muki >= 2 && herow->mispd_buf < 0)) {
                hosei = 0x8c00;
            } else {
                hosei = -0x8c00;
            }
            if (kage_f == 1 && hosei > 0) {
                *kage_flag = 3;
            }
            objw->rtw.mcn += hosei;
            MichiMove(&objw->rtw, hosei);
        }
    } else {
        *kage_flag = kage_f2;
        yflag = yflag2;
        herow->f_phcode = phcode2;
        yukaobj = yukaobj2;
        var_a0 = HeroRootChangeSub(objw, hitposi2, yflag);
        if (herow->f_phcode != 0xFFFF00 && herow->f_phcode != 0xFF0000 && var_a0) {
            objw->rtw.mcn = GetMichinori(&objw->rtw);
            if (herow->buki_muki == 1 || (herow->buki_muki >= 2 && herow->mispd_buf < 0)) {
                hosei = -0x8c00;
            } else {
                hosei = 0x8c00;
            }
            if (kage_f2 == 1 && hosei > 0) {
                *kage_flag = 3;
            }
            objw->rtw.mcn += hosei;
            MichiMove(&objw->rtw, hosei);
        }
        retposi = retposi2;
    }

    if (yukaobj != NULL) {
        herow->movebox = yukaobj;
    }

    if (yflag != 0) {
        *retflag = yflag;
    }
    herow->yuka_hit = yflag;
    return retposi;
}

void Hero_JumpDown_Sub(OBJWORK *objw, HERO_WORK *herow) {
    f32 dy;

    if (objw->bun0 == 27 || objw->bun0 == 40) {
        dy = 65535.0f;
    } else {
        dy = Ashi_HitCheck(objw, &herow->retflag, herow->saka, &herow->kage_flag);
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
    if (herow->f_phcode == 0xFFFF00) {
        nkKloPhCode03(objw, herow);
        return;
    }

    if (GameGbl.kloLife[herow->con] <= 0) {
        nkKloNoLife(objw, herow);
    } else if (objw->bun0 == 7) {
        objw->bun0 = 6;
    } else {
        herow->timer = 0;
        if (herow->f_phcode != 0xFF0000 && herow->f_phcode != 0xFF && herow->f_phcode != 0xFFFF00 && herow->rakka_flag & 2) {
            hSeKeyOn(0x4b04b00ca10002, objw->posi, 0);
            herow->rakka_flag &= -3;
        }
        if (objw->bun1 == 1) {
            if (herow->michispd != 0) {
                if (objw->bun0 == 10) {
                    herow->mispd = 0;
                    herow->mispd_buf = 0;
                    herow->michispd = 0;
                    objw->bun0 = 11;
                } else if (objw->bun0 == 29) {
                    objw->bun0 = 30;
                    herow->mispd = 0;
                    herow->mispd_buf = 0;
                    herow->count = 0;
                } else if (objw->reg0 & 1) {
                    objw->bun0 = 16;
                } else {
                    objw->bun0 = 1;
                }
            } else if (objw->reg0 & 1) {
                if (objw->spd[1] < 6.0f) {
                    objw->bun0 = 13;
                } else {
                    objw->bun0 = 3;
                    herow->timer = 0;
                    herow->count = 0;
                }
            } else if (objw->bun0 == 10) {
                herow->mispd = 0;
                herow->mispd_buf = 0;
                objw->bun0 = 11;
            } else if (objw->bun0 == 29) {
                objw->bun0 = 30;
                herow->mispd = 0;
                herow->mispd_buf = 0;
                herow->count = 0;
            } else if (objw->spd[1] < 6.0f) {
                objw->bun0 = 11;
            } else {
                objw->bun0 = 3;
                herow->timer = 0;
                herow->count = 0;
            }
        }
    }

    if (herow->f_phcode == 0xFF) {
        nkKloNoLife(objw, herow);
    }
    herow->spflag2 &= -6;
    objw->reg0 &= -9;
    herow->timer = 0;
    objw->posi[1] = dy;
    objw->spd[1] = 0.0f;
}

f32 Ashi_HitSubUp(OBJWORK *objw, sceVu0FVECTOR hitposi, s32 *yflag, OBJWORK **yukaobj, s16 *kage_flag, u32 *phcode) {
    sceVu0FVECTOR vec;
    sceVu0FVECTOR nor;
    sceVu0FVECTOR hit_nor;
    HERO_WORK *herow;
    f32 cros;
    f32 var_f1;
    f32 var_f3;

    herow = objw->work;
    *yukaobj = NULL;
    objw->reg4 = 0;
    *kage_flag = 0;
    *phcode = 0;

    vec[0] = hitposi[0];
    if (objw->reg0 & 0x400) {
        vec[1] = hitposi[1] + 11.25f;
    } else {
        vec[1] = hitposi[1] - 11.25f;
    }
    vec[2] = hitposi[2];
    vec[3] = 1.0f;

    nor[0] = vec[0];
    if (objw->reg0 & 0x400) {
        nor[1] = vec[1] - (herow->jumpspd_limit + 11.25f + herow->tenjo_hosei);
    } else {
        nor[1] = vec[1] + herow->jumpspd_limit + 11.25f + herow->tenjo_hosei;
    }
    nor[2] = vec[2];
    vec[3] = 1.0f; // ?

    cros = nkObjPolyHit(vec, nor, yukaobj);
    if (cros != 65535.0f) {
        cros += hitposi[1] - 11.25f;
    }
    var_f3 = cros;
    if (objw->spd[1] != 0.0f) {
        var_f1 = herow->jumpspd_limit + herow->tenjo_hosei;
    } else {
        var_f1 = 3.0f;
    }
    if (var_f3 <= hitposi[1] + var_f1) {
        *kage_flag = 1;
    }
    if (!(cros <= hitposi[1] + var_f1)) {
        cros = 65535.0f;
    }
    *yflag = 0;
    var_f3 = 65535.0f;
    if (cros != 65535.0f) {
        var_f3 = cros;
        *yflag = 3;
    }
    if (var_f3 == 65535.0f) {
        objw->posi[1] += herow->tenjo_hosei;
    }
    if (*yflag == 1) {
        *kage_flag = 0;
    }
    return var_f3;
}

f32 Ashi_HitCheckUp(OBJWORK *objw, s32 *retflag, f64 saka, s32 *kage_flag) {
    sceVu0FVECTOR hitposi;
    f32 retposi;
    f32 retposi2;
    s32 yflag;
    s32 yflag2;
    OBJWORK *yukaobj;
    s32 hosei;
    u32 phcode1;
    u32 phcode2;
    s16 kage_f;
    s16 kage_f2;
    f32 saka_f = saka;
    HERO_WORK *herow = objw->work;
    s32 var_v1;

    hitposi[0] = objw->posi[0] + objw->muki[0] * 8.75f;
    hitposi[2] = objw->posi[2] + objw->muki[2] * 8.75f;
    hitposi[1] = objw->posi[1];
    retposi = Ashi_HitSubUp(objw, hitposi, &yflag, &yukaobj, &kage_f, &phcode1);
    if (yukaobj != NULL) {
        herow->movebox = yukaobj;
    }
    hitposi[0] = objw->posi[0] - objw->muki[0] * 8.75f;
    hitposi[2] = objw->posi[2] - objw->muki[2] * 8.75f;
    hitposi[1] = objw->posi[1];
    retposi2 = Ashi_HitSubUp(objw, hitposi, &yflag2, &yukaobj, &kage_f2, &phcode2);
    if (yukaobj != NULL) {
        herow->movebox = yukaobj;
    }

    var_v1 = 0;
    if (objw->reg0 & 0x400) {
        if (retposi2 == 65535.0f) {
            var_v1 = 1;
        } else if (retposi == 65535.0f) {
            var_v1 = 0;
        } else if (retposi >= retposi2) {
            var_v1 = 1;
        }
    } else {
        if (retposi <= retposi2) {
            var_v1 = 1;
        }
    }
    if (var_v1) {
        *kage_flag = kage_f;
        herow->f_phcode = phcode1;
        if (retposi != 65535.0f && HeroRootChangeSub(objw, hitposi, yflag)) {
            objw->rtw.mcn = GetMichinori(&objw->rtw);
            if (herow->buki_muki == 1 || (herow->buki_muki >= 2 && herow->mispd_buf < 0)) {
                hosei = -0x8c00;
            } else {
                hosei = 0x8c00;
            }
            if (kage_f == 1 && hosei > 0) {
                *kage_flag = 3;
            }
            objw->rtw.mcn += hosei;
            MichiMove(&objw->rtw, hosei);
        }
    } else {
        *kage_flag = kage_f2;
        yflag = yflag2;
        herow->f_phcode = phcode2;
        if (HeroRootChangeSub(objw, hitposi, yflag)) {
            objw->rtw.mcn = GetMichinori(&objw->rtw);
            if (herow->buki_muki == 1 || (herow->buki_muki >= 2 && herow->mispd_buf < 0)) {
                hosei = -0x8c00;
            } else {
                hosei = 0x8c00;
            }
            if (kage_f2 == 1 && hosei > 0) {
                *kage_flag = 3;
            }
            objw->rtw.mcn += hosei;
            MichiMove(&objw->rtw, hosei);
        }
        retposi = retposi2;
    }

    if (yflag != 0) {
        *retflag = yflag;
    }
    herow->yuka_hit = yflag;
    return retposi;
}

void Hero_JumpUp_Sub(OBJWORK *objw, HERO_WORK *herow) {
    f32 dy;

    if (objw->bun0 == 27 || objw->bun0 == 40) {
        dy = 65535.0f;
    } else {
        dy = Ashi_HitCheckUp(objw, &herow->retflag, herow->saka, &herow->kage_flag);
    }
    if (!(dy < 65535.0f)) {
        return;
    }

    if (objw->bun0 == 7) {
        objw->bun0 = 6;
    } else {
        herow->timer = 0;
        if (herow->f_phcode != 0xFF0000 && herow->rakka_flag & 2) {
            hSeKeyOn(0x4b04b00ca10002, objw->posi, 0);
            herow->rakka_flag &= -3;
        }
        if (objw->bun1 == 1) {
            if (herow->michispd != 0) {
                if (objw->bun0 == 10) {
                    herow->mispd = 0;
                    herow->mispd_buf = 0;
                    herow->michispd = 0;
                    objw->bun0 = 11;
                } else if (objw->reg0 & 1) {
                    objw->bun0 = 16;
                } else {
                    objw->bun0 = 1;
                }
            } else if (objw->reg0 & 1) {
                if (objw->spd[1] < 6.0f) {
                    objw->bun0 = 13;
                } else {
                    objw->bun0 = 3;
                    herow->timer = 0;
                    herow->count = 0;
                }
            } else if (objw->bun0 == 10) {
                herow->mispd = 0;
                herow->mispd_buf = 0;
                objw->bun0 = 11;
            } else if (objw->spd[1] < 6.0f) {
                objw->bun0 = 11;
            } else {
                objw->bun0 = 3;
                herow->timer = 0;
                herow->count = 0;
            }
        }
    }

    herow->spflag2 &= -6;
    objw->reg0 &= -9;
    herow->timer = 0;
    objw->posi[1] = dy;
    objw->spd[1] = 0.0f;
}

void TenjoHit(OBJWORK *objw, HERO_WORK *herow) {
    s16 flag;
    f32 var_f3;
    f32 dis;
    sceVu0FVECTOR hitposi;
    sceVu0FVECTOR hitmuki;
    sceVu0FVECTOR rnor;
    OBJWORK *kabeobj;
    u32 work2;

    kabeobj = NULL;
    flag = 0;
    if (objw->reg0 & 0x400) {
        if (objw->spd[1] > 32.0f) {
            var_f3 = objw->spd[1] + 32.0f;
        } else {
            var_f3 = 32.0f;
        }
    } else {
        if (objw->spd[1] < -32.0f) {
            var_f3 = objw->spd[1] - 32.0f;
        } else {
            var_f3 = -32.0f;
        }
    }

    if (objw->bun0 == 14) {
        return;
    }

    hitposi[0] = objw->posi[0] + objw->muki[0] * 8.75f;
    hitposi[2] = objw->posi[2] + objw->muki[2] * 8.75f;
    if (objw->reg0 & 0x400) {
        hitposi[1] = objw->posi[1] + 28.0f;
        hitmuki[1] = hitposi[1] + var_f3;
    } else {
        hitposi[1] = objw->posi[1] - 28.0f;
        hitmuki[1] = hitposi[1] + var_f3;
    }
    hitmuki[0] = hitposi[0];
    hitmuki[2] = hitposi[2];

    dis = RayCrsJudgzNLcol(hitposi, hitmuki, &work2, rnor);
    if (dis >= 0.0f && dis <= 28.0f) {
        flag = 1;
    } else {
        hitposi[0] = objw->posi[0] - objw->muki[0] * 8.75f;
        hitposi[2] = objw->posi[2] - objw->muki[2] * 8.75f;
        hitmuki[0] = hitposi[0];
        hitmuki[2] = hitposi[2];
        dis = RayCrsJudgzNLcol(hitposi, hitmuki, &work2, rnor);
        if (dis >= 0.0f && dis <= 28.0f) {
            flag = 1;
        }
    }

    if (!flag) {
        hitmuki[0] = hitmuki[2] = 0.0f;
        if (objw->reg0 & 0x400) {
            hitmuki[1] = objw->spd[1] + 16.0f;
            hitposi[1] = objw->posi[1] + 28.0f;
        } else {
            hitmuki[1] = objw->spd[1] - 16.0f;
            hitposi[1] = objw->posi[1] - 28.0f;
        }
        hitposi[0] = objw->posi[0] + objw->muki[0] * 8.75f;
        hitposi[2] = objw->posi[2] + objw->muki[2] * 8.75f;
        sceVu0AddVector(hitmuki, hitposi, hitmuki);
        hitposi[3] = hitmuki[3] = 1.0f;
        dis = nkObjPolyHit(hitposi, hitmuki, &kabeobj);
        if (dis >= 0.0f && dis <= 16.0f) {
            flag = 1;
        } else {
            hitposi[0] = objw->posi[0] - objw->muki[0] * 8.75f;
            hitposi[2] = objw->posi[2] - objw->muki[2] * 8.75f;
            dis = nkObjPolyHit(hitposi, hitmuki, &kabeobj);
            if (dis >= 0.0f && dis <= 16.0f) {
                flag = 1;
            }
        }
    }
    if (!flag) {
        return;
    }

    if (objw->bun0 == 20 || objw->bun0 == 21) {
        objw->spd[1] = 0.1f;
        if (kabeobj != NULL) {
            if (objw->reg0 & 0x400) {
                if (kabeobj->freg3 < 0.0f) {
                    herow->tenjo_hosei = kabeobj->freg3;
                }
            } else {
                if (kabeobj->freg3 > 0.0f) {
                    herow->tenjo_hosei = kabeobj->freg3;
                }
            }
        }
    } else {
        hSeKeyOn(0x4b04b00ca10003, objw->posi, 0);
        if (objw->reg0 & 0x400) {
            objw->spd[1] = -0.1f;
        } else {
            objw->spd[1] = 0.1f;
        }
        herow->tenjo_hosei = 0.0f;
    }

    objw->reg0 |= 4;
    herow->gimm_flag |= 0x1000000;
}