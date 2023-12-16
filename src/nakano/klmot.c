#include "nakano/klmot.h"
#include "nakano/route.h"
#include "nakano/k_math.h"
#include "nakano/wipe.h"
#include "nakano/vu0mac.h"
#include "okanoyo/okio.h"
#include "take/object.h"
#include "take/motsys.h"
#include "take/camera.h"
#include "hoshino/h_gamesnd.h"
#include "take/view.h"

SFXOBJ* nkGetKloPSFXOBJ() {
    klMODEL *klmodel;

    klmodel = (klMODEL *)GameGbl.klonoa->prim;
    return klmodel->klm.pObj;
}

void nkSetKlonoaShadow(OBJWORK *objw) {
    sceVu0FVECTOR v0;
    sceVu0FVECTOR v1;
    klMODEL *klmodel;
    SFXOBJ *pObj;
    HERO_WORK *herow;

    herow = objw->work;
    klmodel = (klMODEL *)objw->prim;
    pObj = klmodel->klm.pObj;

    sceVu0CopyVector(v0, objw->posi);
    if (objw->bun1 == 4) {
        v1[0] = herow->slant[0];
        v1[1] = herow->slant[1];
        v1[2] = herow->slant[2];
    } else if (objw->reg0 & 0x400) {
        v1[0] = 0.0f;
        v1[1] = -1.0f;
        v1[2] = 0.0f;
    } else {
        v1[0] = 0.0f;
        v1[1] = 1.0f;
        v1[2] = 0.0f;
    }

    abSetShadow(v0, v1, pObj);
}

void InitKloModel(OBJWORK *objw) {
    SFXOBJ *pObj;
    SFXOBJ *pObj2;
    klMODEL *klmodel;

    objw->prim = getmem(sizeof(klMODEL));
    klmodel = objw->prim;
    *(s32 *)klmodel = 0; // idk

    klmodel->klm.ActNum = 2;
    klmodel->klm.wip_timer = 0;
    klmodel->klm.Rot[0] = 0.0f;
    klmodel->klm.Rot[1] = 0.0f;
    klmodel->klm.Rot[2] = 0.0f;
    klmodel->klm.Rot[3] = 0.0f;
    klmodel->klm.Trans[0] = 0.0f;
    klmodel->klm.Trans[1] = 25.0f;
    klmodel->klm.Trans[2] = 200.0f;
    klmodel->klm.Trans[3] = 1.0f;
    klmodel->klm.Scale[0] = 1.0f;
    klmodel->klm.Scale[1] = 1.0f;
    klmodel->klm.Scale[2] = 1.0f;
    klmodel->klm.Scale[3] = 1.0f;
    klmodel->klm.pObj = GetSfxObjectData(371);
    pObj = klmodel->klm.pObj;
    abAttachPoolA_Klonoa(objw, pObj);
    pObj->Condition = 1;
    SetAct(pObj, klmodel->klm.ActNum);
    SetBaseMatrix(pObj, klmodel->klm.Rot, klmodel->klm.Trans, 0);

    klmodel->shm.pObj = GetSfxObjectData(371);
    pObj = klmodel->shm.pObj;
    SetObjEffDraw(pObj);
    SetAct(pObj, klmodel->klm.ActNum);
    SetBaseMatrix(pObj, klmodel->klm.Rot, klmodel->klm.Trans, 0);
    pObj->Condition = 1;
    nkWipeInit();

    klmodel->ksm.ActNum = 1;
    klmodel->ksm.wip_timer = 0;
    klmodel->ksm.Rot[0] = 0.0f;
    klmodel->ksm.Rot[1] = 0.0f;
    klmodel->ksm.Rot[2] = 0.0f;
    klmodel->ksm.Rot[3] = 0.0f;
    klmodel->ksm.Trans[0] = 0.0f;
    klmodel->ksm.Trans[1] = 25.0f;
    klmodel->ksm.Trans[2] = 200.0f;
    klmodel->ksm.Trans[3] = 1.0f;
    klmodel->ksm.Scale[0] = 1.0f;
    klmodel->ksm.Scale[1] = 1.0f;
    klmodel->ksm.Scale[2] = 1.0f;
    klmodel->ksm.Scale[3] = 1.0f;
    klmodel->ksm.pObj = pObj2 = GetSfxObjectData(378);
    if (pObj2 != NULL) {
        pObj2->Condition = 2;
        SetAct(pObj2, klmodel->ksm.ActNum);
        SetBaseMatrix(pObj2, klmodel->ksm.Rot, klmodel->ksm.Trans, 0);
    }

    klmodel->kmm.pObj = GetSfxObjectData(372);
    if (klmodel->kmm.pObj != NULL) {
        pObj = klmodel->kmm.pObj;
        pObj->Condition = 2;
        klmodel->kmm.ActNum = 1;
        SetAct(pObj, 1);
        SetBaseMatrix(pObj, klmodel->kmm.Rot, klmodel->kmm.Trans, 0);
    }

    klmodel->shm2.pObj = GetSfxObjectData(655);
    if (klmodel->shm2.pObj != NULL) {
        klmodel->shm2.Rot[0] = 0.0f;
        klmodel->shm2.Rot[1] = 0.0f;
        klmodel->shm2.Rot[2] = 0.0f;
        klmodel->shm2.Rot[3] = 0.0f;
        klmodel->shm2.Trans[0] = 0.0f;
        klmodel->shm2.Trans[1] = 0.0f;
        klmodel->shm2.Trans[2] = 0.0f;
        klmodel->shm2.Trans[3] = 1.0f;
        klmodel->shm2.Scale[0] = 1.0f;
        klmodel->shm2.Scale[1] = 1.0f;
        klmodel->shm2.Scale[2] = 1.0f;
        klmodel->shm2.Scale[3] = 1.0f;
        pObj = klmodel->shm2.pObj;
        pObj->Condition = 2;
        klmodel->shm2.ActNum = 1;
        SetAct(pObj, 1);
        SetBaseMatrix(pObj, klmodel->shm2.Rot, klmodel->shm2.Trans, 0);
    }

    GetWsMtx();
}

void nkBaseMove(SFXOBJ *pObj, sceVu0FVECTOR Rot, sceVu0FVECTOR Trans) {
    if (PAD_LVL_DOWN(GameGbl.kpd[0])) {
        Rot[0] += 0.04908739;
        if (Rot[0] > 3.141592653589793) {
            Rot[0] -= 6.283185;
        }
    }
    if (PAD_LVL_UP(GameGbl.kpd[0])) {
        Rot[0] -= 0.04908739;
        if (Rot[0] < 3.141592653589793) {
            Rot[0] += 6.283185;
        }
    }
    if (PAD_LVL_RIGHT(GameGbl.kpd[0])) {
        Rot[1] += 0.04908739;
        if (Rot[1] > 3.141592653589793) {
            Rot[1] -= 6.283185;
        }
    }
    if (PAD_LVL_LEFT(GameGbl.kpd[0])) {
        Rot[1] -= 0.04908739;
        if (Rot[1] < 3.141592653589793) {
            Rot[1] += 6.283185;
        }
    }

    if (PAD_LVL_R1(GameGbl.kpd[0])) {
        Trans[2] -= 5.0;
    }
    if (PAD_LVL_R2(GameGbl.kpd[0])) {
        Trans[2] += 5.0;
    }

    SetBaseMatrix(pObj, Rot, Trans, 0);
}

s32 nkCallMotion(SFXOBJ *pObj, s32 *now, s32 after) {
    s32 ret = 0;

    if (*now != after) {
        *now = after;
        SetActIp(pObj, after);
        ret = 1;
    }

    return ret;
}

void DrawMirSfxKlonoa(OBJWORK *objw) {
    SFXOBJ *pObj;
    sceVu0FMATRIX *nl;
    sceVu0FMATRIX *lc;
    klMODEL *klmodel;

    klmodel = objw->prim;
    pObj = klmodel->shm.pObj;
    SetSfxMotionSync(pObj, klmodel->klm.pObj);
    pObj->Condition = 4;
    SetObjPause(pObj, 1);
    nkSetLight2(&objw->rtw, objw->posi, &nl, &lc);
    SetObjNormalLight(pObj, nl);
    SetObjLightColor(pObj, lc);
    ModelDraw(pObj);
}

void nk_ptget_klonoa(sceVu0FVECTOR pos, sceVu0FVECTOR rot, RT_WRK *rt, s16 *rtno, s32 *mc, s32 *yuka) {
    OBJWORK *objw = GameGbl.klonoa;
    HERO_WORK *herow;
    klMODEL *klmodel = objw->prim;

    sceVu0CopyVector(pos, klmodel->klm.Trans);
    sceVu0CopyVector(rot, klmodel->klm.Rot);
    pos[3] = rot[3] = 1.0f;
    *rt = objw->rtw;
    *rtno = GetMichiNo(rt->rtp);
    *mc = rt->mcn;
    herow = objw->work;
    *yuka = herow->yuka_hit;
}

void DrawSfxKlonoa(OBJWORK *objw) {
    SFXOBJ *pObj;
    HERO_WORK *herow;
    sceVu0FVECTOR rot;
    sceVu0FVECTOR muki;
    f32 work;
    s32 worldip;
    s32 turn_flag;
    sceVu0FMATRIX *nl;
    sceVu0FMATRIX *lc;
    klMODEL *klmodel;
    sceVu0FVECTOR vf;

    worldip = 0;
    turn_flag = 0;
    klmodel = objw->prim;
    herow = objw->work;
    if (klmodel->klm.pObj == NULL) {
        return;
    }
    pObj = klmodel->klm.pObj;

    SetObjPause(pObj, GameGbl.pause_flag);
    if (objw->reg0 & 0x200) {
        nkSetLight2(&herow->okuyuka->rtw, objw->posi, &nl, &lc);
    } else {
        nkSetLight2(&objw->rtw, objw->posi, &nl, &lc);
    }
    SetObjNormalLight(pObj, nl);
    SetObjLightColor(pObj, lc);
    if (objw->reg0 & 0x400) {
        klmodel->klm.Rot[2] = 3.141592f;
    } else {
        klmodel->klm.Rot[2] = 0.0f;
    }

    if (objw->bun0 == 41) {
        klmodel->klm.Rot[1] = klmodel->klm.Rot[1] + 0.26179934f - (objw->spd[1] - herow->jumpspd_limit) / 40.0f;
        klmodel->klm.Rot[1] = nkRadMask(klmodel->klm.Rot[1]);
    } else {
        rot[1] = _atan2(objw->muki[0], objw->muki[2]) + 3.141592f;
        if (herow->Muki == 1 || herow->Muki == 2) {
            if (objw->reg0 & 0x200) {
                GimGetMichiVec(&objw->rtw, muki, herow->okuyuka);
                if (herow->Muki == 1) {
                    rot[1] = _atan2(muki[0], muki[2]) - 1.570796f;
                } else {
                    rot[1] = _atan2(muki[0], muki[2]) + 1.570796f;
                }
            } else {
                GetTaniVector(objw->posi, GameGbl.cam.posi, muki);
                if (herow->Muki == 1) {
                    rot[1] = _atan2(muki[0], muki[2]) + 3.141592f;
                } else {
                    rot[1] = _atan2(muki[0], muki[2]);
                }
            }
        }
        rot[1] = nkRadMask(rot[1]);
        work = rot[1] - klmodel->klm.Rot[1];
        work = nkRadMask(work);
        if (herow->trg & 0xF000) {
            if (klmodel->klm.old_muki != herow->trg) {
                worldip = 1;
            }
            klmodel->klm.old_muki = herow->trg & 0xF000;
        }
        if (work > 0.392699f) {
            klmodel->klm.Rot[1] += 0.392699f;
            turn_flag = 1;
        } else if (work < -0.392699f) {
            klmodel->klm.Rot[1] -= 0.392699f;
            turn_flag = 1;
        } else {
            klmodel->klm.Rot[1] = rot[1];
        }
        klmodel->klm.Rot[1] = nkRadMask(klmodel->klm.Rot[1]);
    }

    vu0_LoadMtx(GameGbl.d_wsm);
    vu0_Thing(vf, objw->posi);
    if (vf[3] < 128.0f) {
        pObj->Condition = 1;
    } else if (vf[3] < 256.0f) {
        pObj->Condition = 2;
    } else {
        pObj->Condition = 4;
    }
    GetActiveSfx(pObj);

    if (herow->taiho_bun != 1 && !(herow->gimm_flag & 0x400000)) {
        klmodel->klm.Rot[0] = objw->rot[0] = 0.0f;
    }
    if (!(herow->gimm_flag & 1)) {
        herow->taiho_bun = 0;
    }

    if (herow->gimm_flag & 0x400000) {
        objw->rot[0] -= 0.392699f;
        objw->rot[0] = nkRadMask(objw->rot[0]);
        klmodel->klm.Rot[0] = objw->rot[0];
        rot[1] = _atan2(objw->spd[0], objw->spd[2]) + 3.141592f;
        rot[1] = nkRadMask(rot[1]);
        klmodel->klm.Rot[0] = objw->rot[0];
        klmodel->klm.Rot[1] = rot[1];
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 39);
    } else if (herow->gimm_flag & 1) {
        sceVu0CopyVector(klmodel->klm.Scale, herow->Scale);
        rot[0] = objw->rot[0];
        if (herow->taiho_bun == 1) {
            objw->rot[0] -= 0.392699f;
            objw->rot[0] = nkRadMask(objw->rot[0]);
        } else {
            objw->rot[0] = 0.0f;
        }
        rot[1] = _atan2(objw->spd[0], objw->spd[2]) + 3.141592f;
        rot[1] = nkRadMask(rot[1]);
        klmodel->klm.Rot[0] = objw->rot[0];
        klmodel->klm.Rot[1] = rot[1];

        if (pPAD_TRG_L2(herow) && herow->b_action == 0) {
            hSeKeyOn(0xc80c80ca20111, objw->posi, 0);
            herow->b_action = 2;
            herow->b_act_rflag = 0;
            worldip = 1;
        }
        if (herow->b_action == 2) {
            herow->b_act_rot += 0.392699f;
            herow->b_act_rot = nkRadMask(herow->b_act_rot);
            herow->b_act_rflag++;
            if (herow->b_act_rflag > 16) {
                herow->b_act_rot = 0.0f;
                herow->b_act_rflag = 0;
                herow->b_action = 0;
            }
            klmodel->klm.Rot[2] = herow->b_act_rot;
        }

        if (pPAD_TRG_L1(herow) && herow->b_action == 0 && klmodel->klm.ActNum != 26) {
            hSeKeyOn(0xc80c80ca1000d, objw->posi, 0);
            herow->b_action = 1;
            herow->b_act_rflag = 0;
            worldip = 1;
        }
        if (herow->b_action == 1) {
            herow->b_act_rot += 0.392699f;
            herow->b_act_rot = nkRadMask(herow->b_act_rot);
            herow->b_act_rflag++;
            if (herow->b_act_rflag > 16) {
                herow->b_act_rot = 0.0f;
                herow->b_act_rflag = 0;
                herow->b_action = 0;
            }
            klmodel->klm.Rot[0] = herow->b_act_rot;
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 39);
        }

        if (herow->norigimm->reg0 & 0x1000) {
            if (herow->taiho_bun == 0) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 25);
                herow->taiho_bun = 1;
                if (herow->se_id != -1) {
                    hSeKeyOff(herow->se_id);
                    herow->se_id = -1;
                }
                herow->se_id2 = hSeKeyOn(0x8da00803, objw->posi, 0);
            }
            if (klmodel->klm.ActNum == 25 && herow->taiho_bun == 1 && GetActStopFlag(pObj) != 0) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 26);
            }
        } else if (klmodel->klm.ActNum != 25 && klmodel->klm.ActNum != 26) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 24);
        }
    } else if (GameGbl.wipe.flag & 2 && GameGbl.wipe.fdata > 0) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 2);
    } else if (GameGbl.wipe.outtime > 0 && !(GameGbl.wipe.flag & 4)) {
        if (GameGbl.vision == 0x0705 || GameGbl.vision == 0x0F08) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 2);
        } else {
            switch (GameGbl.wipe.imuki) {
                case 1:
                case 2:
                    if (objw->reg0 & 1) {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 15);
                    } else {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 13);
                    }
                    break;
                case 3:
                case 4:
                    if (objw->reg0 & 1) {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 12);
                    } else {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 11);
                    }
                    break;
            }
        }
    } else if (herow->gimm_flag & 0x200 && objw->bun0 == 11) {
        if (herow->Muki == 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 46);
            klmodel->klm.Rot[1] = nkRadMask(_atan2(objw->muki[0], objw->muki[2]) + 3.141592f);
        } else if (herow->Muki == 2) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 47);
            klmodel->klm.Rot[1] = nkRadMask(_atan2(objw->muki[0], objw->muki[2]) + 3.141592f);
        } else {
            if (herow->buki_muki == 1) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 2);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 45);
            }
        }
    } else if (GameGbl.wipe.intime > 0 && !(GameGbl.wipe.flag & 8)) {
        switch (GameGbl.wipe.omuki) {
            case 1:
            case 2:
                if (objw->reg0 & 1) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 15);
                } else {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 13);
                }
                break;
            case 3:
            case 4:
                if (objw->reg0 & 1) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 12);
                } else {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 11);
                }
                break;
        }
    } else {
        switch (objw->bun0) {
            case 8:
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 36);
                break;
            case 9:
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 20);
                break;
            case 6:
                if (herow->timer == 1) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 33);
                } else if (herow->timer > 10) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 34);
                }
                break;
            case 7:
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 19);
                break;
            case 18:
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 16);
                break;
            case 1:
                if (pPAD_TRG_L1(herow) && herow->b_action == 0 && klmodel->klm.ActNum != 14) {
                    hSeKeyOn(0xc80c80ca20203, objw->posi, 0);
                    herow->b_action = 1;
                    herow->rupu_cnt = 0;
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 14);
                } else if (klmodel->klm.ActNum == 14) {
                    herow->rupu_cnt++;
                    if (herow->rupu_cnt > 32) {
                        herow->b_action = 0;
                        herow->rupu_cnt = 0;
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 13);
                    }
                } else if (pPAD_TRG_L2(herow) && herow->b_action == 0 && klmodel->klm.ActNum != 14) {
                    hSeKeyOn(0xc80c80ca20202, objw->posi, 0);
                    herow->b_action = 2;
                    herow->rupu_cnt = 0;
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 14);
                } else if (klmodel->klm.ActNum == 14) {
                    herow->rupu_cnt++;
                    if (herow->rupu_cnt > 32) {
                        herow->b_action = 0;
                        herow->rupu_cnt = 0;
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 13);
                    }
                } else {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 13);
                }
                break;
            case 13:
                if (turn_flag) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 15);
                } else {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 3);
                }
                break;
            case 16:
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 15);
                break;
            case 2:
                if (herow->se_id5 != -1) {
                    if (pPAD_TRG_L1(herow) && herow->b_action == 0) {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 51);
                        hSeKeyOn(0xc80c80ca2010e, objw->posi, 0);
                        herow->b_action = 1;
                        herow->b_act_rflag = 0;
                    }
                    if (herow->b_action == 1) {
                        herow->b_act_rflag++;
                        if (herow->b_act_rflag > 32) {
                            herow->b_act_rflag = 0;
                            herow->b_action = 0;
                        }
                        break;
                    }

                    if (pPAD_TRG_L2(herow) && herow->b_action == 0) {
                        hSeKeyOn(0xc80c80ca1000d, objw->posi, 0);
                        herow->b_action = 2;
                        herow->b_act_rflag = 0;
                        worldip = 1;
                    }
                    if (herow->b_action == 2) {
                        herow->b_act_rot += 0.392699f;
                        herow->b_act_rot = nkRadMask(herow->b_act_rot);
                        herow->b_act_rflag++;
                        if (herow->b_act_rflag > 16) {
                            herow->b_act_rot = 0.0f;
                            herow->b_act_rflag = 0;
                            herow->b_action = 0;
                        }
                        klmodel->klm.Rot[1] = rot[1] + herow->b_act_rot;
                        klmodel->klm.Rot[1] = nkRadMask(klmodel->klm.Rot[1]);
                    }
                } else {
                    if (pPAD_TRG_L2(herow) && herow->b_action == 0) {
                        hSeKeyOn(0xc80c80ca1000d, objw->posi, 0);
                        herow->b_action = 2;
                        herow->b_act_rflag = 0;
                        worldip = 1;
                    }
                    if (herow->b_action == 2) {
                        herow->b_act_rot += 0.392699f;
                        herow->b_act_rot = nkRadMask(herow->b_act_rot);
                        herow->b_act_rflag++;
                        if (herow->b_act_rflag > 16) {
                            herow->b_act_rot = 0.0f;
                            herow->b_act_rflag = 0;
                            herow->b_action = 0;
                        }
                        klmodel->klm.Rot[1] = rot[1] + herow->b_act_rot;
                        klmodel->klm.Rot[1] = nkRadMask(klmodel->klm.Rot[1]);
                    }

                    if (pPAD_TRG_L1(herow) && herow->b_action == 0) {
                        hSeKeyOn(0xc80c80ca1000d, objw->posi, 0);
                        herow->b_action = 1;
                        herow->b_act_rflag = 0;
                        worldip = 1;
                    }
                    if (herow->b_action == 1) {
                        herow->b_act_rot += 0.392699f;
                        herow->b_act_rot = nkRadMask(herow->b_act_rot);
                        herow->b_act_rflag++;
                        if (herow->b_act_rflag > 16) {
                            herow->b_act_rot = 0.0f;
                            herow->b_act_rflag = 0;
                            herow->b_action = 0;
                        }
                        klmodel->klm.Rot[1] = rot[1] + herow->b_act_rot;
                        klmodel->klm.Rot[1] = nkRadMask(klmodel->klm.Rot[1]);
                    }
                }

                if (objw->spd[1] < 0.0f) {
                    if (herow->gimm_flag & 8) {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 23);
                    } else {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 17);
                    }
                } else if (objw->spd[1] > 0.0f) {
                    nkCallMotion(pObj, &klmodel->klm.ActNum, 31);
                }
                break;
            case 27:
                if (herow->count < 10) {
                    if (herow->gimm_flag & 4) {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 22);
                    } else {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 21);
                    }
                }
                break;
            case 3:
                if (objw->reg0 & 1) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 30);
                } else {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 29);
                }
                break;
            case 15:
                if (objw->spd[1] < 0.0f) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 18);
                } else if (objw->spd[1] > 0.0f) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 32);
                }
                break;
            case 20:
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 43);
                break;
            case 34:
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 40);
                break;
            case 12:
                if (klmodel->klm.ActNum != 41) {
                    klmodel->klm.ActNum = 41;
                    SetActIp(pObj, 41);
                    worldip = 1;
                }
                break;
            case 41:
                if (objw->reg0 & 1) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 28);
                } else {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 27);
                }
                break;
            case 10:
                if (objw->reg0 & 1) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 38);
                } else {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 37);
                }
                break;
            case 14:
            case 40:
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 44);
                break;
            case 30:
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 8);
                nkCallMotion(klmodel->shm2.pObj, &klmodel->shm2.ActNum, 2);
                break;
            default:
                if (objw->reg0 & 1) {
                    break;
                }

                if (pPAD_TRG_L2(herow)) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 6);
                }
                if (pPAD_TRG_L1(herow)) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 7);
                }

                GetActCnt(pObj);
                if (klmodel->klm.ActNum == 7 && GetActCnt(pObj) == 70.0f && GameGbl.pause_flag == 0) {
                    hSeKeyOn(0xc80c80ca20110, objw->posi, 0);
                }
                if (klmodel->klm.ActNum == 7 && GetActStopFlag(pObj) == 0) {
                    break;
                }

                GetActCnt(pObj);
                if (klmodel->klm.ActNum == 6 && GetActCnt(pObj) == 40.0f && GameGbl.pause_flag == 0) {
                    hSeKeyOn(0xc80c80ca20204, objw->posi, 0);
                }
                if (klmodel->klm.ActNum == 6 && GetActStopFlag(pObj) == 0) {
                    break;
                }

                if (turn_flag) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 13);
                } else if (herow->hima_cnt > 3600 && GameGbl.pause_flag == 0) {
                    herow->hima_cnt = 0;
                    if (rand() & 1) {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 4);
                        hSeKeyOn(0xc80c80ca20201, objw->posi, 0);
                    } else {
                        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 5);
                        hSeKeyOn(0xc80c80ca20200, objw->posi, 0);
                    }
                } else if ((klmodel->klm.ActNum != 4 && klmodel->klm.ActNum != 5) || GetActStopFlag(pObj) != 0) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 2);
                }
                break;
        }
    }

    if (objw->bun0 != 2 && !(herow->gimm_flag & 1)) {
        herow->b_action = 0;
    }
    if (klmodel->klm.wip_timer < 50) {
        klmodel->klm.wip_timer++;
    }
    if (worldip && klmodel->klm.wip_timer > 10) {
        SetMotionWorldIp(pObj->pMot, klwipsp);
        klmodel->klm.wip_timer = 0;
    }

    sceVu0CopyVector(klmodel->klm.Trans, objw->posi);
    SetBaseMatrix(pObj, klmodel->klm.Rot, klmodel->klm.Trans, 1);

    if (!(herow->tenmetu & 1) || GameGbl.pause_flag) {
        SetObjScale(pObj, klmodel->klm.Scale);
        ModelDraw(pObj);
        herow->motcnt = GetActCnt(pObj);
        herow->motcnt_end = GetActEndCnt(pObj);
        herow->motstop_flag = GetActStopFlag(pObj);
        herow->mot_actno = klmodel->klm.ActNum;

        if (objw->bun0 == 30) {
            pObj = klmodel->shm2.pObj;
            SetObjPause(pObj, GameGbl.pause_flag);
            pObj->Condition = 1;
            SetObjNormalLight(pObj, nl);
            SetObjLightColor(pObj, lc);
            sceVu0CopyVector(klmodel->shm2.Rot, klmodel->klm.Rot);
            sceVu0CopyVector(klmodel->shm2.Trans, klmodel->klm.Trans);
            SetBaseMatrix(pObj, klmodel->shm2.Rot, klmodel->shm2.Trans, 1);
            ModelDraw(pObj);
        }
    }
}

void DrawSfxKlonoa_SS(OBJWORK *objw) {
    SFXOBJ *pObj;
    SFXOBJ *pObj_ks;
    HERO_WORK *herow;
    sceVu0FVECTOR rot;
    s32 worldip;
    sceVu0FMATRIX *nl;
    sceVu0FMATRIX *lc;
    klMODEL *klmodel;
    sceVu0FVECTOR vf;
    f32 *v1;

    worldip = 0;
    klmodel = objw->prim;
    herow = objw->work;
    if (klmodel->klm.pObj == NULL || klmodel->ksm.pObj == NULL) {
        return;
    }
    pObj = klmodel->klm.pObj;
    pObj_ks = klmodel->ksm.pObj;

    nkSetLight2(&objw->rtw, objw->posi, &nl, &lc);
    SetObjNormalLight(pObj, nl);
    SetObjLightColor(pObj, lc);
    SetObjNormalLight(pObj_ks, nl);
    SetObjLightColor(pObj_ks, lc);
    SetObjPause(pObj, GameGbl.pause_flag);
    SetObjPause(pObj_ks, GameGbl.pause_flag);

    rot[1] = _atan2(objw->muki[0], objw->muki[2]) + 3.141592f;
    rot[1] = nkRadMask(rot[1]);
    rot[1] -= objw->rot[1];
    rot[1] = nkRadMask(rot[1]);
    objw->rot[1] += rot[1] / 16.0f;
    objw->rot[1] = nkRadMask(objw->rot[1]);
    klmodel->klm.Rot[1] = objw->rot[1];
    klmodel->ksm.Rot[1] = objw->rot[1];
    klmodel->klm.Rot[0] = objw->rot[0];
    klmodel->ksm.Rot[0] = objw->rot[0];
    klmodel->klm.Rot[2] = objw->rot[2];
    klmodel->ksm.Rot[2] = objw->rot[2];

    if (pPAD_TRG_UP(herow) | pPAD_TRG_DOWN(herow) | pPAD_TRG_LEFT(herow) | pPAD_TRG_RIGHT(herow)) {
        if (klmodel->klm.old_muki != herow->trg) {
            worldip = 1;
        }
        klmodel->klm.old_muki = pPAD_TRG_UP(herow) | pPAD_TRG_DOWN(herow) | pPAD_TRG_LEFT(herow) | pPAD_TRG_RIGHT(herow);
    }

    vu0_LoadMtx(GameGbl.wsm);
    vu0_Thing(vf, objw->posi);
    if (vf[3] < 128.0f) {
        pObj->Condition = 1;
        pObj_ks->Condition = 2;
    } else if (vf[3] < 256.0f) {
        pObj->Condition = 2;
        pObj_ks->Condition = 2;
    } else {
        pObj->Condition = 4;
        pObj_ks->Condition = 4;
    }

    if (herow->gimm_flag & 1) {
        sceVu0CopyVector(klmodel->klm.Scale, herow->Scale);
        sceVu0CopyVector(klmodel->ksm.Scale, herow->Scale);
        rot[1] = _atan2(objw->spd[0], objw->spd[2]) + 3.141592f;
        rot[1] = nkRadMask(rot[1]);
        klmodel->klm.Rot[1] = rot[1];
        klmodel->ksm.Rot[1] = rot[1];
    }

    if (objw->bun0 == 8) {
        herow->b_action = 0;
        if (herow->buki_muki == 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 71);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 20);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 70);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 19);
        }
    } else if (objw->bun0 == 9) {
        herow->b_action = 0;
        if (herow->buki_muki == 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 71);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 32);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 70);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 31);
        }
    } else if (objw->bun0 == 6) {
        herow->b_action = 0;
        if (herow->buki_muki == 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 65);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 14);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 64);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 13);
        }
    } else if (objw->bun0 == 7) {
        herow->b_action = 0;
        if (herow->buki_muki == 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 79);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 28);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 78);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 27);
        }
    } else if (herow->b_action == 2) {
        if (klmodel->klm.ActNum == 97 || klmodel->klm.ActNum == 101) {
            if (GetActStopFlag(pObj) != 0) {
                herow->b_action = 0;
            }
        } else if (herow->yuka_hit == 0 || !(objw->spd[1] >= 0.0f)) {
            if (herow->gimm_flag & 1 || objw->spd[1] < -9.75f) {
                hSeKeyOn(0x4b04b00ca2a002, objw->posi, 0);
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 97);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 46);
            } else {
                hSeKeyOn(0x4b04b00ca2a000, objw->posi, 0);
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 101);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 50);
            }
        } else {
            hSeKeyOn(0x4b04b00ca2a000, objw->posi, 0);
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 97);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 46);
        }
    } else if (herow->b_action == 1) {
        if (klmodel->klm.ActNum == 100 || klmodel->klm.ActNum == 96) {
            if (GetActStopFlag(pObj) != 0) {
                herow->b_action = 0;
            }
        } else if (herow->yuka_hit == 0 || !(objw->spd[1] >= 0.0f)) {
            if (herow->gimm_flag & 1 || objw->spd[1] < -9.75f) {
                hSeKeyOn(0xc80c80ca20113, objw->posi, 0);
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 96);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 45);
            } else {
                hSeKeyOn(0x4b04b00ca2a001, objw->posi, 0);
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 100);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 49);
            }
        } else if (klmodel->klm.ActNum == 54) {
            if (GetActStopFlag(pObj) != 0) {
                herow->b_action = 0;
            }
        } else if (klmodel->klm.ActNum == 53) {
            hSeKeyOn(0xc80c80ca20202, objw->posi, 0);
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 54);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 3);
        } else {
            herow->b_action = 0;
        }
    } else if (objw->bun0 == 34) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 99);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 48);
    } else if (objw->bun0 == 10) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 98);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 47);
    } else if (objw->bun0 == 20) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 88);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 37);
    } else if (objw->bun0 == 27) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 93);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 42);
    } else if (objw->spd[1] < -2.0f) {
        if (objw->reg0 & 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 91);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 40);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 76);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 25);
        }
    } else if (objw->spd[1] > 2.0f) {
        if (objw->reg0 & 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 94);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 43);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 89);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 38);
        }
    } else if (pPAD_LVL_RIGHT(herow)) {
        if (objw->reg0 & 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 60);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 9);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 55);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 4);
        }
    } else if (pPAD_LVL_LEFT(herow)) {
        if (objw->reg0 & 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 63);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 12);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 58);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 7);
        }
    } else if (objw->reg0 & 1) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 59);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 8);
    } else {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 53);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 3);
    }

    if (klmodel->klm.wip_timer < 50) {
        klmodel->klm.wip_timer++;
    }
    if (worldip && klmodel->klm.wip_timer > 10) {
        SetMotionWorldIp(pObj->pMot, klwipsp);
        klmodel->klm.wip_timer = 0;
    }

    sceVu0CopyVector(klmodel->klm.Trans, objw->posi);
    sceVu0CopyVector(klmodel->ksm.Trans, objw->posi);
    if (!(herow->tenmetu & 1) || GameGbl.pause_flag) {
        klmodel->klm.Trans[1] -= 16.0f;
        klmodel->ksm.Trans[1] -= 16.0f;
        SetBaseMatrix(pObj, klmodel->klm.Rot, klmodel->klm.Trans, 1);
        SetObjScale(pObj, klmodel->klm.Scale);
        ModelDraw(pObj);
        SetBaseMatrix(pObj_ks, klmodel->ksm.Rot, klmodel->ksm.Trans, 1);
        SetObjScale(pObj_ks, klmodel->ksm.Scale);
        ModelDraw(pObj_ks);
    }
}

void nkCallBoardFlear(OBJWORK *objw) {
    HERO_WORK *herow = objw->work;
    klMODEL *klmodel = objw->prim;
    SFXOBJ *pObj = klmodel->ksm.pObj;

    if (herow->kasokuLvL == 1) {
        sceVu0IVECTOR v0 = { 32, 128, 0,   0 };
        sceVu0IVECTOR v1 = { 0,  128, 128, 0 };
        abDrawBoardFlaer(pObj, 1, 8.0f, 12.0f, 60, v0, v1);
    } else if (herow->kasokuLvL == 2) {
        sceVu0IVECTOR v0 = { 192, 192, 32, 0 };
        sceVu0IVECTOR v1 = { 64,  64,  0,  0 };
        abDrawBoardFlaer(pObj, 1, 12.0f, 16.0f, 50, v0, v1);
    } else if (herow->kasokuLvL == 3) {
        sceVu0IVECTOR v0 = { 96, 96, 96 };
        sceVu0IVECTOR v1 = { 96 };
        sceVu0IVECTOR v2 = { 192, 96, 96 };
        sceVu0IVECTOR v3 = { 192 };
        abDrawBoardFlaer(pObj, 1, 8.0f, 10.0f, 40, v0, v1);
        abDrawBoardFlaer(pObj, 1, 16.0f, 18.0f, 40, v2, v3);
    }
}

void DrawSfxKlonoa_BH(OBJWORK *objw) {
    SFXOBJ *pObj;
    SFXOBJ *pObj_ks;
    HERO_WORK *herow;
    sceVu0FVECTOR rot;
    s32 worldip;
    sceVu0FMATRIX *nl;
    sceVu0FMATRIX *lc;
    klMODEL *klmodel;
    sceVu0FVECTOR vf;
    f32 *v1;
    sceVu0FMATRIX mat;
    MOTION *m;

    worldip = 0;
    klmodel = objw->prim;
    herow = objw->work;
    if (klmodel->klm.pObj == NULL || klmodel->ksm.pObj == NULL) {
        return;
    }
    pObj = klmodel->klm.pObj;
    if (GameGbl.vision >> 8 == 1) {
        pObj_ks = klmodel->kmm.pObj;
    } else {
        pObj_ks = klmodel->ksm.pObj;
    }

    nkSetLight2(&objw->rtw, objw->posi, &nl, &lc);
    SetObjNormalLight(pObj, nl);
    SetObjLightColor(pObj, lc);
    SetObjNormalLight(pObj_ks, nl);
    SetObjLightColor(pObj_ks, lc);
    SetObjPause(pObj, GameGbl.pause_flag);
    SetObjPause(pObj_ks, GameGbl.pause_flag);

    if (objw->bun1 == 4) {
        rot[1] = -objw->rot[1] + 3.141592f;
    } else {
        rot[1] = -objw->rot[1] + 3.141592f - objw->freg1;
    }
    rot[1] = nkRadMask(rot[1]);
    klmodel->klm.Rot[0] = objw->rot[0];
    klmodel->ksm.Rot[0] = objw->rot[0];
    klmodel->klm.Rot[1] = rot[1];
    klmodel->ksm.Rot[1] = rot[1];
    klmodel->klm.Rot[2] = objw->rot[2];
    klmodel->ksm.Rot[2] = objw->rot[2];

    vu0_LoadMtx(GameGbl.wsm);
    vu0_Thing(vf, objw->posi);
    if (vf[3] < 128.0f) {
        pObj->Condition = 1;
        pObj_ks->Condition = 2;
    } else if (vf[3] < 256.0f) {
        pObj->Condition = 2;
        pObj_ks->Condition = 2;
    } else {
        pObj->Condition = 4;
        pObj_ks->Condition = 4;
    }

    if (herow->gimm_flag & 1) {
        sceVu0CopyVector(klmodel->klm.Scale, herow->Scale);
        sceVu0CopyVector(klmodel->ksm.Scale, herow->Scale);
        rot[1] = _atan2(objw->spd[0], objw->spd[2]) + 3.141592f;
        rot[1] = nkRadMask(rot[1]);
        klmodel->klm.Rot[1] = rot[1];
        klmodel->ksm.Rot[1] = rot[1];
    }

    if (GameGbl.vision >> 8 == 1) {
        if (objw->bun0 == 6 || objw->bun0 == 7) {
            if (herow->lvl & herow->down_move) {
                if (herow->lvl & herow->right_move) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 69);
                    nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 5);
                } else if (herow->lvl & herow->left_move) {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 68);
                    nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 4);
                } else {
                    worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 65);
                    nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 2);
                }
            } else if (herow->lvl & herow->right_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 67);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 5);
            } else if (herow->lvl & herow->left_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 66);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 4);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 64);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 2);
            }
        } else if (herow->yuka_hit == 0 || !(objw->spd[1] >= 0.0f) || objw->bun0 == 20) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 53);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 3);
        } else if (!(herow->lvl & (herow->right_move | herow->left_move))) {
            if (herow->lvl & herow->up_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 55);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 2);
            } else if (herow->lvl & herow->down_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 53);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 3);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 53);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 2);
            }
        } else if (herow->lvl & herow->right_move) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 57);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 5);
        } else if (herow->lvl & herow->left_move) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 56);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 4);
        }
    } else if (objw->bun0 == 6) {
        herow->b_action = 0;
        if (herow->lvl & herow->down_move) {
            if (herow->lvl & herow->right_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 69);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 18);
            } else if (herow->lvl & herow->left_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 68);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 17);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 65);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 14);
            }
        } else if (herow->lvl & herow->right_move) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 67);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 16);
        } else if (herow->lvl & herow->left_move) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 66);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 15);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 64);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 13);
        }
    } else if (objw->bun0 == 7) {
        herow->b_action = 0;
        if (herow->lvl & herow->down_move) {
            if (herow->lvl & herow->right_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 81);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 30);
            } else if (herow->lvl & herow->left_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 80);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 29);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 65);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 14);
            }
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 78);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 27);
        }
    } else if (herow->b_action == 2) {
        if (klmodel->klm.ActNum == 97 || klmodel->klm.ActNum == 101) {
            if (GetActStopFlag(pObj) != 0) {
                herow->b_action = 0;
            }
        } else if (herow->yuka_hit == 0 || !(objw->spd[1] >= 0.0f)) {
            if (herow->gimm_flag & 1 || objw->spd[1] < -9.75f) {
                hSeKeyOn(0x4b04b00ca2a002, objw->posi, 0);
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 97);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 46);
            } else {
                hSeKeyOn(0x4b04b00ca2a000, objw->posi, 0);
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 101);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 50);
            }
        } else {
            hSeKeyOn(0x4b04b00ca2a000, objw->posi, 0);
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 97);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 46);
        }
    } else if (herow->b_action == 1) {
        if (klmodel->klm.ActNum == 100 || klmodel->klm.ActNum == 96) {
            if (GetActStopFlag(pObj) != 0) {
                herow->b_action = 0;
            }
        } else if (herow->yuka_hit == 0 || !(objw->spd[1] >= 0.0f)) {
            if (herow->gimm_flag & 1 || objw->spd[1] < -9.75f) {
                hSeKeyOn(0xc80c80ca20113, objw->posi, 0);
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 96);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 45);
            } else {
                hSeKeyOn(0x4b04b00ca2a001, objw->posi, 0);
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 100);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 49);
            }
        } else if (klmodel->klm.ActNum == 54) {
            if (GetActStopFlag(pObj) != 0) {
                herow->b_action = 0;
            }
        } else if (klmodel->klm.ActNum == 53) {
            hSeKeyOn(0xc80c80ca20202, objw->posi, 0);
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 54);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 3);
        } else {
            herow->b_action = 0;
        }
    } else if (objw->bun0 == 34) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 99);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 48);
    } else if (objw->bun0 == 10) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 98);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 47);
    } else if (objw->bun0 == 27) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 93);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 42);
    } else if (objw->bun0 == 20) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 88);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 37);
    } else if (objw->bun0 == 8) {
        if (herow->lvl & herow->down_move) {
            if (herow->lvl & herow->right_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 75);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 24);
            } else if (herow->lvl & herow->left_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 74);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 23);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 71);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 20);
            }
        } else if (herow->lvl & herow->right_move) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 73);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 22);
        } else if (herow->lvl & herow->left_move) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 72);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 21);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 70);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 19);
        }
    } else if (objw->bun0 == 9) {
        if (herow->lvl & herow->down_move) {
            if (herow->lvl & herow->right_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 87);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 36);
            } else if (herow->lvl & herow->left_move) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 86);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 35);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 83);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 32);
            }
        } else if (herow->lvl & herow->right_move) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 85);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 34);
        } else if (herow->lvl & herow->left_move) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 84);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 33);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 82);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 31);
        }
    } else if (objw->spd[1] != 0.0f) {
        if (objw->spd[1] < -2.0f) {
            if (objw->reg0 & 1) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 91);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 40);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 76);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 25);
            }
        } else if (objw->spd[1] > 2.0f) {
            if (objw->reg0 & 1) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 94);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 43);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 89);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 38);
            }
        } else if (objw->reg0 & 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 92);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 41);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 77);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 26);
        }
    } else if (!(herow->lvl & (herow->right_move | herow->left_move))) {
        if (herow->lvl & herow->up_move) {
            if (objw->reg0 & 1) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 60);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 9);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 55);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 4);
            }
        } else if (herow->lvl & herow->down_move) {
            if (objw->reg0 & 1) {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 63);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 12);
            } else {
                worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 58);
                nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 7);
            }
        } else if (objw->reg0 & 1) {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 59);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 8);
        } else {
            worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 53);
            nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 3);
        }
    } else if (herow->lvl & herow->right_move) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 57);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 6);
    } else if (herow->lvl & herow->left_move) {
        worldip |= nkCallMotion(pObj, &klmodel->klm.ActNum, 56);
        nkCallMotion(pObj_ks, &klmodel->ksm.ActNum, 5);
    }

    if (klmodel->klm.wip_timer < 50) {
        klmodel->klm.wip_timer++;
    }
    if (worldip && klmodel->klm.wip_timer > 10) {
        SetMotionWorldIp(pObj->pMot, klwipsp);
        klmodel->klm.wip_timer = 0;
    }

    sceVu0CopyVector(klmodel->klm.Trans, objw->posi);
    sceVu0CopyVector(klmodel->ksm.Trans, objw->posi);
    if (!(herow->tenmetu & 1) || GameGbl.pause_flag) {
        if (objw->bun1 == 4) {
            klmodel->klm.Trans[0] -= herow->slant[0] * 16.0f;
            klmodel->klm.Trans[1] -= herow->slant[1] * 16.0f;
            klmodel->klm.Trans[2] -= herow->slant[2] * 16.0f;
            klmodel->ksm.Trans[0] -= herow->slant[0] * 16.0f;
            klmodel->ksm.Trans[1] -= herow->slant[1] * 16.0f;
            klmodel->ksm.Trans[2] -= herow->slant[2] * 16.0f;
        } else {
            klmodel->klm.Trans[1] -= 16.0f;
            klmodel->ksm.Trans[1] -= 16.0f;
        }

        if (objw->bun1 == 4) {
            sceVu0UnitMatrix(mat);
            sceVu0RotMatrixY(mat, mat, -objw->freg1);
            sceVu0RotMatrixZ(mat, mat, klmodel->klm.Rot[2]);
            sceVu0RotMatrixX(mat, mat, klmodel->klm.Rot[0]);
            sceVu0RotMatrixY(mat, mat, klmodel->klm.Rot[1]);
            sceVu0TransMatrix(mat, mat, klmodel->klm.Trans);
            SetBaseMatrix2(pObj, mat);
        } else {
            SetBaseMatrix(pObj, klmodel->klm.Rot, klmodel->klm.Trans, 1);
        }
        SetObjScale(pObj, klmodel->klm.Scale);
        ModelDraw(pObj);

        if (objw->bun1 == 4) {
            m = pObj_ks->pMot;
            m->pBaseCoord->Rot[0] = klmodel->klm.Rot[0];
            m->pBaseCoord->Rot[1] = klmodel->klm.Rot[1];
            m->pBaseCoord->Rot[2] = klmodel->klm.Rot[2];
            m->pBaseCoord->Rot[3] = 0.0f;
            m->pBaseCoord->Trans[0] = klmodel->klm.Trans[0];
            m->pBaseCoord->Trans[1] = klmodel->klm.Trans[1];
            m->pBaseCoord->Trans[2] = klmodel->klm.Trans[2];
            m->pBaseCoord->Trans[3] = 1.0f;
            m->pBaseCoord->Flag = -1;
            sceVu0UnitMatrix(m->pBaseCoord->Mtx);
            sceVu0RotMatrixY(m->pBaseCoord->Mtx, m->pBaseCoord->Mtx, -objw->freg1);
            sceVu0RotMatrixZ(m->pBaseCoord->Mtx, m->pBaseCoord->Mtx, klmodel->klm.Rot[2]);
            sceVu0RotMatrixX(m->pBaseCoord->Mtx, m->pBaseCoord->Mtx, klmodel->klm.Rot[0]);
            sceVu0RotMatrixY(m->pBaseCoord->Mtx, m->pBaseCoord->Mtx, klmodel->klm.Rot[1]);
            sceVu0TransMatrix(m->pBaseCoord->Mtx, m->pBaseCoord->Mtx, klmodel->klm.Trans);
        } else {
            SetBaseMatrix(pObj_ks, klmodel->ksm.Rot, klmodel->ksm.Trans, 1);
        }
        SetObjScale(pObj_ks, klmodel->ksm.Scale);
        ModelDraw(pObj_ks);
    }
}