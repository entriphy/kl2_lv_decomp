#include "nakano/klohit2.h"
#include "nakano/vu0mac.h"
#include "nakano/k_math.h"

void sbHeroKabeCheck2(OBJWORK *objw, HERO_WORK *herow) {
    // Empty function
}

void sbHeroKabeCheck(OBJWORK *objw, HERO_WORK *herow) {
    f32 work;
    f32 angy;
    f32 siny;
    f32 cosy;
    sceVu0FVECTOR vec0;
    sceVu0FVECTOR vec1;

    angy = -_atan2(objw->muki[0], objw->muki[2]);
    angy = nkRadMask(angy);
    cosy = _cos(angy);
    siny = _sin(angy);

    vec1[0] = -cosy * 32.0f;
    vec1[1] = 0.0f;
    vec1[2] = -siny * 32.0f;
    vec0[0] = objw->posi[0] + cosy * 16.0f;
    vec0[1] = objw->posi[1] - 11.0f;
    vec0[2] = objw->posi[2] + siny * 16.0f;
    sceVu0AddVector(vec1, vec0, vec1);

    work = RayCrsJudgzNL(vec0, vec1);
    if (work > 0.0f && work < 32.0f) {
        if (!pPAD_LVL_RIGHT(herow)) {
            f32 fspd = objw->spd[0] * objw->spd[0] + objw->spd[2] * objw->spd[2];
            fspd = vu0_SquareRoot(fspd);
            objw->spd[0] = -siny * fspd;
            objw->spd[2] = cosy * fspd;
        }
        objw->posi[0] += (32.0f - work) * cosy;
        objw->posi[2] += (32.0f - work) * siny;
    }

    vec1[0] = cosy * 32.0f;
    vec1[1] = 0.0f;
    vec1[2] = siny * 32.0f;
    vec0[0] = objw->posi[0] - cosy * 16.0f;
    vec0[1] = objw->posi[1] - 11.0f;
    vec0[2] = objw->posi[2] - siny * 16.0f;
    sceVu0AddVector(vec1, vec0, vec1);

    work = RayCrsJudgzNL(vec0, vec1);
    if (work > 0.0f && work < 32.0f) {
        if (!pPAD_LVL_LEFT(herow)) {
            f32 fspd = objw->spd[0] * objw->spd[0] + objw->spd[2] * objw->spd[2];
            fspd = vu0_SquareRoot(fspd);
            objw->spd[0] = -siny * fspd;
            objw->spd[2] = cosy * fspd;
        }
        objw->posi[0] += (-32.0f + work) * cosy;
        objw->posi[2] += (-32.0f + work) * siny;
    }

    vec1[0] = -siny * 32.0f;
    vec1[1] = 0.0f;
    vec1[2] = cosy * 32.0f;
    vec0[0] = objw->posi[0] + siny * 20.0f;
    vec0[1] = objw->posi[1] - 24.0f;
    vec0[2] = objw->posi[2] - cosy * 20.0f;
    sceVu0AddVector(vec1, vec0, vec1);

    work = RayCrsJudgzNL(vec0, vec1);
    if (work > 0.0f && work < 32.0f) {
        objw->posi[0] += (-32.0f + work) * -siny;
        objw->posi[2] += (-32.0f + work) * cosy;
    }
}
