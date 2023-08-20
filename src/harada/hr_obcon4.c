#include "harada/hr_obcon4.h"
#include "harada/hr_obcon.h"
#include "harada/hr_main.h"

static void hr_0100_tree(OBJWORK *objw);
static void hr_0900_oksave(OBJWORK *objw);

static HR_RTC hr_0100_treeRC[4] = {
    {0, 0xA80000, 0xB20000,  0},
    {0, 0x900000, 0xD30000,  0},
    {0, 0xEE0000, 0x15B0000, 0},
    {1, 0x480000, 0x720000,  0}
};
static void (*hrObjExFuncTbl[2])(OBJWORK *) = {
    hr_0100_tree,
    hr_0900_oksave
};
static s16 hrExResTbl[2] = {};

static void hr_obcex_draw2(OBJWORK *objw) {
    if (objw->reg2 != NULL) {
        hr_to_modeldrawRL(objw->reg2, objw->reg4);
    }
}

static void hr_tree0100_setG(OBJWORK *objw) {
    objw->reg2 = (s32)hr_to_workget();
    if (objw->reg2 != NULL) {
        hr_to_modelinit2(objw->reg2, 73);
        hr_to_motionset(objw->reg2, 2, 0);
        hr_to_modelclip(objw->reg2);
        hr_to_setpos(objw->reg2, -1300.6832f, -321.7744f, 1567.7184f);
        hr_to_setrot(objw->reg2, 0.0f, 0.0f, 0.0f);
        hr_to_setscale(objw->reg2, 6.4f);
    }
}

static void hr_0100_tree(OBJWORK *objw) {
    OBJWORK *kw;
    s32 ret;
    sceVu0FVECTOR v;

    kw = GameGbl.klonoa;
    objw->reg0 = hr_route_clipM(hr_0100_treeRC, 4, GetMichiNo(kw->rtw.rtp), kw->rtw.mcn);
    objw->draw = hr_obcex_draw2;
    objw->reg4 = 4;
    if (objw->reg5 != 0) {
        hr_tree0100_setG(objw);
        objw->reg5 = 0;
        objw->bun0 = 3;
        if (objw->reg2 != NULL) {
            ret = hr_to_motionGFMAX((void *)objw->reg2);
            ret--;
            hr_to_motionSFRM((void *)objw->reg2, (f32)ret);
        }
    }

    switch (objw->bun0) {
        case 0:
            if (objw->reg0 == 0) {
                objw->draw = NULL;
                if (objw->reg2 != NULL) {
                    hr_to_workdel(objw->reg2);
                    objw->reg2 = NULL;
                }
            } else {
                if (objw->reg2 == NULL) {
                    objw->reg2 = hr_to_workget();
                    if (objw->reg2 != NULL) {
                        hr_to_modelinit2(objw->reg2, 72);
                        hr_to_motionset(objw->reg2, 2, 0);
                        hr_to_setpos(objw->reg2, -1300.6832f, -321.7744f, 1567.7184f);
                        hr_to_setrot(objw->reg2, 0.0f, 0.0f, 0.0f);
                        hr_to_setscale(objw->reg2, 6.4f);
                    } else {
                        objw->draw = NULL;
                    }
                }

                if (objw->reg0 == 1 && objw->reg2 != NULL) {
                    objw->reg3 = 40;
                    objw->bun0 += 1;
                    abSetExplosion(objw->reg1, v);
                    hr_restart_set(0);
                }
            }
            break;
        case 1:
            objw->reg3--;
            if (objw->reg3 == 0) {
                hr_to_workdel(objw->reg2);
                hr_tree0100_setG(objw);
                objw->bun0 += 1;
                objw->reg3 = 240;
            }
            break;
        case 2:
            objw->reg3--;
            if (objw->reg3 == 0) {
                objw->bun0 += 1;
            }
            break;
        default:
            if (objw->reg0 == 0) {
                objw->draw = NULL;
            }
            break;
    }

    if (objw->reg2 != NULL) {
        hr_to_motion(objw->reg2, 0);
    }
}

static void hr_0900_oksave(OBJWORK *objw) {
    OkSaveforReStart(0, 880, 2605.0f, 0, 0);
    hr_resSV |= 1;
    objw->stat0 = 0;
}

void hr_obcex_getset(OBJWORK *objw, s32 id) {
    objw->reg1 = id;
    objw->bun0 = 0;
    objw->reg0 = objw->reg2 = objw->reg3 = objw->reg4 = 0;
    objw->reg5 = hr_restart_check(hrExResTbl[id - 512]);
}

void hr_obcex_work(OBJWORK *objw) {
    hrObjExFuncTbl[objw->reg1 - 512](objw);
}
