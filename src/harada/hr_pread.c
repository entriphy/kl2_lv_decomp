#include "harada/hr_pread.h"
#include "harada/hr_pall.h"
#include "harada/hr_pcam.h"
#include "harada/hr_pmes.h"
#include "harada/hr_pefc.h"
#include "harada/hr_bgwk.h"
#include "harada/hr_main.h"
#include "harada/hr_preak.h"
#include "harada/hr_pream.h"
#include "harada/hr_take.h"
#include "harada/hr_prm.h"
#include "harada/hr_vtprg.h"
#include "hoshino/h_util.h"
#include "nakano/main.h"

static PT32A *p32a;
static PT32B *p32b;
static PT64A *p64a;
static PT64B *p64b;
static PT64C *p64c;
static PT64D *p64d;
static PT64E *p64e;
static PT64F *p64f;
static PT96A *p96a;
static PT96B *p96b;
static PT96D *p96d;
static PT128A *p128a;
static PT128B *p128b;
static PT128C *p128c;
static PT160A *p160a;

static s32 pt_sys_call(HR_CALL *ca, HR_PSYS *ps) {
    HR_CALL *cap;

    p32a = (PT32A *)ca->read;
    cap = hr_pcall_search(ps, ca);
    if (cap != NULL) {
        cap->flag = 1;
        cap->read = ca->read + p32a->ss0;
    }
    ca->read++;
    return 1;
}

static s32 pt_sys_end(HR_CALL *ca, HR_PSYS *ps) {
    return 0;
}

static s32 pt_sys_time(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ca->time = p32a->ss0;
    ca->read++;
    return 0;
}

static s32 pt_sys_sub(HR_CALL *ca, HR_PSYS *ps) {
    if (ca->ret == NULL) {
        ca->ret = ca->read + 1;
        p32a = (PT32A *)ca->read;
        ca->read += p32a->ss0;
    } else {
        ca->read++;
    }
    return 1;
}

static s32 pt_sys_ret(HR_CALL *ca, HR_PSYS *ps) {
    if (ca->ret != NULL) {
        ca->read = ca->ret;
        ca->ret = NULL;
    }
    return 1;
}

static s32 pt_sys_type(HR_CALL *ca, HR_PSYS *ps) {
    HOT *hObj;

    p64a = (PT64A *)ca->read;
    ca->id = p64a->ss0;
    ca->chr = p64a->ss1;
    ca->fhm = p64a->ss2;
    if (hrpt_view == 0 || (s32)ps->model != -1) {
        hObj = hr_take_workGet();
        if (hObj != NULL) {
            if (hrpt_view == 1) {
                hr_take_modelinit(hObj, GetFHMAddress((u32 *)ps->model, ca->fhm));
            } else {
                hr_take_modelinit2(hObj, ca->chr);
            }
            hObj->pNormalLight = &ca->NormalLight;
            hObj->pLightColor = &ca->LightColor;
            ca->hObj = hObj;
        }
    }

    ca->read += 2;
    return 1;
}

static void comm_pt_sys_pos(HR_CALL *ca, sceVu0FVECTOR v) {
    p128a = (PT128A *)ca->read;
    v[0] = p128a->f0;
    v[1] = p128a->f1;
    v[2] = p128a->f2;
    ca->read += 4;
}

static s32 pt_sys_pos(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ca->pos.p);
    return 1;
}

static s32 pt_sys_rot(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ca->rot.p);
    return 1;
}

static s32 pt_sys_draw(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    if (p32b->us0 == 0) {
        ca->flag &= -3;
    } else {
        ca->flag |= 2;
    }
    ca->read++;
    return 1;
}

static s32 pt_sys_jmp(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ca->read += p32a->ss0;
    return 1;
}

void pt_flag_work(HR_PSYS *ps, PT96A *sfg) {
    if (sfg->us0 == 0) {
        ps->flag |= sfg->ui0;
    } else {
        ps->flag &= ~sfg->ui0;
    }
}

static s32 pt_sys_flag(HR_CALL *ca, HR_PSYS *ps) {
    p96a = (PT96A *)ca->read;
    if (ca != &ps->sys) {
        if (ps->nflag < 4) {
            ps->flagptr[ps->nflag] = p96a;
            ps->nflag++;
        }
    } else  {
        pt_flag_work(ps, p96a);
    }
    ca->read += 3;
    return 1;
}

static s32 pt_sys_flagj(HR_CALL *ca, HR_PSYS *ps) {
    s32 fg;

    p96a = (PT96A *)ca->read;
    if (p96a->us1 == 1) {
        fg = pt_flagj_P(p96a->ui0);
    } else {
        if (ps->flag & p96a->ui0) {
            fg = 1;
        } else {
            fg = 0;
        }
    }
    if (p96a->us0 == 1) {
        fg ^= 1;
    }

    if (fg) {
        ca->read += p96a->ss0;
    } else {
        ca->read += 3;
    }
    return 1;
}

static s32 pt_check_xyz0(sceVu0FVECTOR v) {
    if (v[0] != 0.0f) {
        return 0;
    }
    if (v[1] != 0.0f) {
        return 0;
    }
    if (v[2] != 0.0f) {
        return 0;
    }
    return 1;
}

static void pt_check_spdset(PCAMSC *pcc) {
    if (pt_check_xyz0(pcc->spd)) {
        hr_pcam_spdclr(pcc);
    } else {
        pcc->flag |= 1;
        sceVu0ScaleVectorXYZ(pcc->acc, pcc->acc, 0.0f);
    }
}

static s32 pt_sys_pos_spd(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ca->pos.spd);
    pt_check_spdset(&ca->pos);
    return 1;
}

static s32 pt_sys_pos_acc(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ca->pos.acc);
    ca->pos.flag |= 1;
    return 1;
}

static s32 pt_sys_rot_spd(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ca->rot.spd);
    pt_check_spdset(&ca->rot);
    return 1;
}

static s32 pt_sys_rot_acc(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ca->rot.acc);
    ca->rot.flag |= 1;
    return 1;
}

static void comm_p128b_read(HR_CALL *ca, sceVu0FVECTOR v) {
    p128b = (PT128B *)ca->read;
    v[0] = p128b->f0;
    v[1] = p128b->f1;
    v[2] = p128b->f2;
    ca->read += 4;
}

void comm_p128b_ft(PCAMSC *pcc, s32 ft) {
    if (pcc->spd[0] != 0.0f) {
        pcc->flag |= 2;
    }
    if (pcc->spd[1] != 0.0f) {
        pcc->flag |= 4;
    }
    if (pcc->spd[2] != 0.0f) {
        pcc->flag |= 8;
    }
    pcc->time = ft;
    pcc->flag |= 1;
}

void comm_jigen_spd(sceVu0FVECTOR spd, sceVu0FVECTOR acc, f32 ft, u8 uc0, u8 uc1) {
    f32 t2;
    f32 pp;
    f32 pm;

    if (uc0 == uc1) {
        sceVu0ScaleVectorXYZ(acc, acc, 0.0f);
        sceVu0DivVectorXYZ(spd, spd, ft);
    } else if (uc0 == 0) {
        t2 = ft * (ft + 1.0f) / 2.0f;
        sceVu0DivVectorXYZ(acc, spd, t2);
        sceVu0ScaleVectorXYZ(spd, spd, 0.0f);
    } else if (uc1 == 0) {
        t2 = (ft + 1.0f) * (ft - 2.0f) / 2.0f;
        sceVu0DivVectorXYZ(acc, spd, -t2);
        sceVu0ScaleVectorXYZ(spd, acc, -ft);
    } else {
        pp = (f32)uc1 / (f32)uc0;
        pm = pp - 1.0f;
        pp += 1.0f;
        sceVu0ScaleVectorXYZ(spd, spd, 2.0f / (ft * pp));
        sceVu0ScaleVectorXYZ(acc, spd, pm / (ft + 1.0f));
    }
}

static void comm_getft_mv(HR_CALL *ca, PCAMSC *pcc) {
    comm_p128b_read(ca, pcc->spd);
    if (ca->getft == 0) {
        hr_pcam_spdclr(pcc);
    } else {
        sceVu0AddVector(pcc->moku, pcc->p, pcc->spd);
        pcc->moku[3] = 1.0f;
        comm_p128b_ft(pcc, ca->getft);
        comm_jigen_spd(pcc->spd, pcc->acc, ca->getft, p128b->uc0, p128b->uc1);
    }
}

static void comm_getft_mvp(HR_CALL *ca, PCAMSC *pcc) {
    comm_p128b_read(ca, pcc->moku);
    if (ca->getft == 0) {
        hr_pcam_spdclr(pcc);
    } else {
        sceVu0AddVector(pcc->spd, pcc->moku, pcc->p);
        pcc->spd[3] = 0.0f;
        comm_p128b_ft(pcc, ca->getft);
        comm_jigen_spd(pcc->spd, pcc->acc, ca->getft, p128b->uc0, p128b->uc1);
    }
}

static s32 pt_sys_pos_mv(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mv(ca, &ca->pos);
    return 1;
}

static s32 pt_sys_callclr(HR_CALL *ca, HR_PSYS *ps) {
    if (ca == &ps->sys) {
        hr_callbuff_init(ps, 1);
        ps->flag &= 0xFFFF;
        hr_pcam_init(ps->cam->pos, ps->cam->ang, ps->cam->proj.p);
    }
    ca->read++;
    return 1;
}

static s32 pt_sys_rot_mv(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mv(ca, &ca->rot);
    hr_pall_rot180V(ca->rot.moku);
}

static s32 pt_sys_timemp(HR_CALL *ca, HR_PSYS *ps) {
    s32 fmax;

    if (ca->hObj != NULL && ca->flag & 4) {
        p32a = (PT32A *)ca->read;
        fmax = hr_take_motionGetFMAX(ca->hObj);
        ca->time = fmax * p32a->ss0 / 100;
    }
    ca->read++;
    return 0;
}

static void comm_pt_sys_light(HR_CALL *ca, sceVu0FVECTOR v) {
    v[0] = p128a->f0;
    v[1] = p128a->f1;
    v[2] = p128a->f2;
    ca->flag |= 0x10;
    ca->read += 4;
}

static void flag_NL(HR_CALL *ca, s32 no, s32 on) {
    s32 flag;

    flag = 0x40 << no;
    if (on) {
        ca->flag |= flag;
    } else {
        ca->flag &= ~flag;
    }
}

static s32 pt_sys_nlight(HR_CALL *ca, HR_PSYS *ps) {
    p128a = (PT128A *)ca->read;
    comm_pt_sys_light(ca, ca->nlight[p128a->ss0]);
    flag_NL(ca, p128a->ss0, 0);
    return 1;
}

static s32 pt_sys_lcolor(HR_CALL *ca, HR_PSYS *ps) {
    p128a = (PT128A *)ca->read;
    comm_pt_sys_light(ca, ca->lcolor[ca->lightno].p);
    return 1;
}

static s32 pt_sys_ambient(HR_CALL *ca, HR_PSYS *ps) {
    p128a = (PT128A *)ca->read;
    comm_pt_sys_light(ca, ca->ambi);
    return 1;
}

static s32 pt_sys_size(HR_CALL *ca, HR_PSYS *ps) {
    s32 size;

    if (ca->hObj != NULL) {
        p32a = (PT32A *)ca->read;
        size = hKEI_L;
        switch (p32a->ss0) {
            case HRPSIZE_L:
                size = hKEI_L;
                break;
            case HRPSIZE_M:
                size = hKEI_M;
                break;
            case HRPSIZE_S:
                size = hKEI_S;
                break;
            case HRPSIZE_F:
                size = hKEI_F;
                break;
        }
        hr_take_modelsizeCHG(ca->hObj, size);
    }

    ca->read++;
    return 1;
}

static s32 pt_sys_mes_data(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    hr_pmes_set(ps->pmes, pt_al_data(ca, p32a->ss0));
    ca->read++;
    return 1;
}

static s32 pt_sys_mes_start(HR_CALL *ca, HR_PSYS *ps) {
    ps->flag |= 8;
    ps->flag &= -0x40001;
    ca->read++;
    return 1;
}

static s32 pt_sys_mes_window(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    hr_pmes_window(ps->pmes, p32b->us0 != 0);
    ca->read++;
    return 1;
}

static s32 pt_sys_mes_spd(HR_CALL *ca, HR_PSYS *ps) {
    ca->read++;
    return 1;
}

static s32 pt_sys_mes_reta(HR_CALL *ca, HR_PSYS *ps) {
    hr_pmes_reta(ps->pmes);
    ca->read++;
    return 1;
}

static s32 pt_sys_mes_nskip(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    if (p32b->us0 == 0) {
        ps->pmes->flag |= 0x20000;
    } else {
        ps->pmes->flag &= -0x20001;
    }
    ca->read++;
    return 1;
}

static s32 pt_sys_get_ft(HR_CALL *ca, HR_PSYS *ps) {
    s32 t;
    s32 tn;

    p64a = (PT64A *)ca->read;
    if (ca->hObj != NULL) {
        t = hr_take_motionGetFMAX(ca->hObj);
    } else {
        t = 0;
    }

    switch (p64a->ss0) {
        case PGETFT_ML:
            ca->getft = t * ca->motlp;
            break;
        case PGETFT_MP:
            ca->getft = t * p64a->ss1 / 100;
            break;
        case PGETFT_LP:
            ca->getft = t * p64a->ss1;
            break;
        case PGETFT_TM:
            ca->getft = p64a->ss1;
            break;
        case PGETFT_ZN:
            if (t == 0) {
                ca->getft = 0;
            } else {
                tn = hr_take_motionGetFrm(ca->hObj);
                if (tn > t) {
                    tn = t;
                }
                t -= tn;
                ca->getft = t;
            }
            break;
        default:
            ca->getft = 0;
            break;
    }

    ca->read += 2;
    return 1;
}

static s32 pt_sys_pos_mvp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvp(ca, &ca->pos);
    return 1;
}

static s32 pt_sys_rot_mvp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvp(ca, &ca->rot);
    hr_pall_rot180V(ca->rot.moku);
    return 1;
}

static void comm_sys_spd1(HR_CALL *ca, PCAMSC *pcc) {
    p64c = (PT64C *)ca->read;
    pcc->acc[p64c->us0] = 0.0f;
    pcc->spd[p64c->us0] = p64c->f0;
    if (pt_check_xyz0(pcc->spd)) {
        hr_pcam_spdclr(pcc);
    } else if (p64c->f0 != 0.0f) {
        pcc->flag |= 1;
    }
    ca->read += 2;
}

static s32 pt_sys_scale(HR_CALL *ca, HR_PSYS *ps) {
    p64c = (PT64C *)ca->read;
    hr_take_modelscale(ca->hObj, p64c->f0);
    ca->read += 2;
    return 1;
}

static s32 pt_sys_pos_spd1(HR_CALL *ca, HR_PSYS *ps) {
    comm_sys_spd1(ca, &ca->pos);
    return 1;
}

static s32 pt_sys_rot_spd1(HR_CALL *ca, HR_PSYS *ps) {
    comm_sys_spd1(ca, &ca->rot);
    return 1;
}

static s32 pt_sys_light_no(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ca->lightno = p32a->ss0;
    ca->read++;
    return 1;
}

static s32 pt_sys_light_sw(HR_CALL *ca, HR_PSYS *ps) {
    p64a = (PT64A *)ca->read;
    ca->read += 2;
    return 1;
}

static s32 pt_sys_ltcol_spd(HR_CALL *ca, HR_PSYS *ps) {
    hrpcc = &ca->lcolor[ca->lightno];
    comm_pt_sys_pos(ca, hrpcc->spd);
    pt_check_spdset(hrpcc);
    return 1;
}

static s32 pt_sys_ltcol_acc(HR_CALL *ca, HR_PSYS *ps) {
    hrpcc = &ca->lcolor[ca->lightno];
    comm_pt_sys_pos(ca, hrpcc->acc);
    hrpcc->flag |= 1;
    return 1;
}

static s32 pt_sys_ltcol_mv(HR_CALL *ca, HR_PSYS *ps) {
    hrpcc = &ca->lcolor[ca->lightno];
    comm_getft_mv(ca, hrpcc);
    hr_pall_limitV(hrpcc->moku, 1.0f, 0.0f);
    return 1;
}

static s32 pt_sys_ltcol_mvp(HR_CALL *ca, HR_PSYS *ps) {
    hrpcc = &ca->lcolor[ca->lightno];
    comm_getft_mvp(ca, hrpcc);
    hr_pall_limitV(hrpcc->moku, 1.0f, 0.0f);
    return 1;
}

static s32 pt_sys_ltcol_spd1(HR_CALL *ca, HR_PSYS *ps) {
    comm_sys_spd1(ca, &ca->lcolor[ca->lightno]);
    return 1;
}

static s32 pt_sys_ltrot_spd(HR_CALL *ca, HR_PSYS *ps) {
    hrpcc = &ca->nlw[ca->lightno];
    comm_pt_sys_pos(ca, hrpcc->spd);
    pt_check_spdset(hrpcc);
    flag_NL(ca, ca->lightno, 1);
    return 1;
}

static s32 pt_sys_ltrot_acc(HR_CALL *ca, HR_PSYS *ps) {
    hrpcc = &ca->nlw[ca->lightno];
    comm_pt_sys_pos(ca, hrpcc->acc);
    hrpcc->flag |= 1;
    flag_NL(ca, ca->lightno, 1);
    return 1;
}

static s32 pt_sys_ltrot_mv(HR_CALL *ca, HR_PSYS *ps) {
    hrpcc = &ca->nlw[ca->lightno];
    comm_getft_mv(ca, hrpcc);
    hr_pall_rot180V(hrpcc->moku);
    flag_NL(ca, ca->lightno, 1);
    return 1;
}

static s32 pt_sys_ltrot_mvp(HR_CALL *ca, HR_PSYS *ps) {
    hrpcc = &ca->nlw[ca->lightno];
    comm_getft_mvp(ca, hrpcc);
    hr_pall_rot180V(hrpcc->moku);
    flag_NL(ca, ca->lightno, 1);
    return 1;
}

static s32 pt_sys_ltrot_spd1(HR_CALL *ca, HR_PSYS *ps) {
    comm_sys_spd1(ca, &ca->nlw[ca->lightno]);
    flag_NL(ca, ca->lightno, 1);
    return 1;
}

static s32 func_00116E30(s32 param_1) {
    if (param_1 < 0x6E && (0x6B < param_1 || param_1 == 0x26 || param_1 == 0x37)) {
        return 0;
    } else {
        return 1;
    }
}

static s32 pt_sys_effect(HR_CALL *ca, HR_PSYS *ps) {
    p64a = (PT64A *)ca->read;
    if (p64a->ss0 == 0) {
        hr_call_efcdel(ca, ps);
        if (hrpt_view == 0 || func_00116E30(p64a->ss1)) {
            hr_call_efcon(ca, ps, p64a->ss1, 0x30);
        }
    } else {
        hr_call_efcdel(ca, ps);
    }

    ca->read += 2;
    return 1;
}

static s32 pt_sys_econt(HR_CALL *ca, HR_PSYS *ps) {
    p64a = (PT64A *)ca->read;
    if (p64a->ss0 == 0) {
        hr_call_efcon(ca, ps, p64a->ss1, 0x40);
    } else {
        hr_call_efcdelE(ps, p64a->ss1);
    }

    ca->read += 2;
    return 1;
}

static s32 pt_sys_xfade(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    hr_pfade_x(&ps->fade, p32b->us0);
    ca->read++;
    return 1;
}

static s32 pt_sys_xfade_w(HR_CALL *ca, HR_PSYS *ps) {
    if (ps->fade.flag & 0x30) {
        return 0;
    } else {
        ca->read++;
        return 1;
    }
}

static s32 pt_sys_col(HR_CALL *ca, HR_PSYS *ps) {
    p64e = (PT64E *)ca->read;
    if (p64e->uc0 == 0 && p64e->uc1 == 0 && p64e->uc2 == 0) {
        hr_pfade_init(&ps->fade);
    } else {
        hr_pfade_set(&ps->fade, p64e->uc0, p64e->uc1, p64e->uc2, p64e->us0);
    }

    ca->read += 2;
    return 1;
}

static s32 pt_sys_col_in(HR_CALL *ca, HR_PSYS *ps) {
    p64e = (PT64E *)ca->read;
    hr_pfade_in(&ps->fade, p64e->uc0, p64e->uc1, p64e->uc2, 0x80, p64e->us0);
    ca->read += 2;
    return 1;
}

static s32 pt_sys_col_out(HR_CALL *ca, HR_PSYS *ps) {
    p64e = (PT64E *)ca->read;
    hr_pfade_out(&ps->fade, p64e->uc0, p64e->uc1, p64e->uc2, 0x80, p64e->us0);
    ca->read += 2;
    return 1;
}

static s32 pt_sys_pos_cp(HR_CALL *ca, HR_PSYS *ps) {
    HR_CALL *cap;

    p128a = (PT128A *)ca->read;
    hrpcc = &ca->pos;
    hr_pcam_cinit(hrpcc);
    hr_pcall_rtinit(ca);
    ca->flag &= -0x21;

    cap = hr_id_search(ps, p128a->ss0);
    if (cap != NULL) {
        if (p128a->code == 0x34) {
            hr_pt_getpos(cap, &hrpcc->p);
            hrpcc->p[0] += p128a->f0;
            hrpcc->p[1] += p128a->f1;
            hrpcc->p[2] += p128a->f2;
        } else {
            hr_pt_getpos(cap, &hrpcc->p);
            hr_pt_getrot(cap, &hrvi);
            hr_pcall_mkm(hrpcc->p, hrvi, hrpm);
            hrvi[0] = p128a->f0;
            hrvi[1] = p128a->f1;
            hrvi[2] = p128a->f2;
            hrvi[3] = 1.0f;
            sceVu0ApplyMatrix(hrpcc->p, hrpm, hrvi);
            hrpcc->p[3] = 1.0f;
        }
    }

    ca->read += 4;
    return 1;
}

static s32 pt_sys_rot_cp(HR_CALL *ca, HR_PSYS *ps) {
    HR_CALL *cap;

    p128a = (PT128A *)ca->read;
    hrpcc = &ca->rot;
    hr_pcam_cinit(hrpcc);
    hr_pcall_rtinit(ca);
    ca->flag &= -0x21;

    cap = hr_id_search(ps, p128a->ss0);
    if (cap != NULL) {
        hr_pt_getrotD(cap, &hrpcc->p);
        hrpcc->p[0] += p128a->f0;
        hrpcc->p[1] += p128a->f1;
        hrpcc->p[2] += p128a->f2;
        hr_pall_rot180V(hrpcc->p);
    }

    ca->read += 4;
    return 1;
}

static s32 pt_sys_pos_mvpc(HR_CALL *ca, HR_PSYS *ps) {
    HR_CALL *cap;
    sceVu0FVECTOR v;

    p160a = (PT160A *)ca->read;
    cap = hr_id_search(ps, p160a->ss0);
    if (cap != NULL && cap->hObj != NULL && ca->getft != 0) {
        hrpcc = &ca->pos;
        hr_take_matrixtrans(cap->hObj, v, p160a->ss1);
        v[0] += p160a->f0;
        v[1] += p160a->f1;
        v[2] += p160a->f2;
        v[3] = 1.0f;
        sceVu0CopyVector(hrpcc->moku, v);
        sceVu0SubVector(hrpcc->spd, v, hrpcc->p);
        hrpcc->spd[3] = 0.0f;
        comm_p128b_ft(hrpcc, ca->getft);
        comm_jigen_spd(hrpcc->spd, hrpcc->acc, ca->getft, p160a->uc0, p160a->uc1);
    }

    ca->read += 5;
    return 1;
}

static s32 pt_sys_ltrot(HR_CALL *ca, HR_PSYS *ps) {
    hrpcc = &ca->nlw[ca->lightno];
    comm_pt_sys_pos(ca, hrpcc->p);
    flag_NL(ca, ca->lightno, 1);
    return 1;
}

static s32 pt_sys_vtmode(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    if (p32b->us0 == 0) {
        hrpt_vt = 1;
    } else {
        hrpt_vt = 0;
    }

    ca->read++;
    return 1;
}

static s32 pt_sys_pos_cpt(HR_CALL *ca, HR_PSYS *ps) {
    HR_CALL *cap;
    sceVu0FVECTOR v;

    p160a = (PT160A *)ca->read;
    cap = hr_id_search(ps, p160a->ss0);
    if (cap != NULL && cap->hObj != NULL) {
        hrpcc = &ca->pos;
        hr_take_matrixtrans(cap->hObj, v, p160a->ss1);
        v[0] += p160a->f0;
        v[1] += p160a->f1;
        v[2] += p160a->f2;
        v[3] = 1.0f;
        hr_pcam_cinit(hrpcc);
        sceVu0CopyVector(hrpcc->p, v);
    }

    ca->read += 5;
    return 1;
}

static s32 pt_sys_id(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ca->id = p32a->ss0;
    ca->read++;
    return 1;
}

static s32 pt_sys_chr(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ca->chr = p32a->ss0;
    ca->read++;
    return 1;
}

static s32 pt_sys_exit(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    if (!(ps->flag & 0x80)) {
        pt_set_exit(ps, p32a->ss0);
    }
    return 0;
}

static s32 pt_sys_acol(HR_CALL *ca, HR_PSYS *ps) {
    pt_sys_col(ca, ps);
    if (ps->fade.flag & 1) {
        ps->fade.flag |= 8;
    }
    return 1;
}

static s32 pt_sys_acol_in(HR_CALL *ca, HR_PSYS *ps) {
    pt_sys_col_in(ca, ps);
    if (ps->fade.flag & 1) {
        ps->fade.flag |= 8;
    }
    return 1;
}

static s32 pt_sys_acol_out(HR_CALL *ca, HR_PSYS *ps) {
    pt_sys_col_out(ca, ps);
    if (ps->fade.flag & 1) {
        ps->fade.flag |= 8;
    }
    return 1;
}

static void comm_p160a_read(HR_CALL *ca, sceVu0FVECTOR v) {
    p160a = (PT160A *)ca->read;
    v[0] = p160a->f0;
    v[1] = p160a->f1;
    v[2] = p160a->f2;
    ca->read += 5;
}

static void comm_p160a_houb(sceVu0FVECTOR v, PCAMSC *pcc, f32 ft) {
    s32 i;

    sceVu0DivVectorXYZ(v, v, ft);
    for (i = 0; i < 3; i++) {
        if (i != p160a->ss0) {
            pcc->spd[i] = v[i];
            pcc->acc[i] = 0.0f;
        }
    }
}

static void comm_getft_mh(HR_CALL *ca, PCAMSC *pcc) {
    sceVu0FVECTOR v;

    comm_p160a_read(ca, pcc->spd);
    if (ca->getft == 0) {
        hr_pcam_spdclr(pcc);
    } else {
        sceVu0AddVector(pcc->moku, pcc->p, pcc->spd);
        pcc->moku[3] = 1.0f;
        sceVu0CopyVector(v, pcc->spd);
        comm_p128b_ft(pcc, ca->getft);
        comm_jigen_spd(pcc->spd, pcc->acc, ca->getft, p160a->uc0, p160a->uc1);
        comm_p160a_houb(v, pcc, ca->getft);
    }
}

static void comm_getft_mhp(HR_CALL *ca, PCAMSC *pcc) {
    sceVu0FVECTOR v;

    comm_p160a_read(ca, pcc->moku);
    if (ca->getft == 0) {
        hr_pcam_spdclr(pcc);
    } else {
        sceVu0AddVector(pcc->spd, pcc->moku, pcc->p);
        pcc->spd[3] = 0.0f;
        sceVu0CopyVector(v, pcc->spd);
        comm_p128b_ft(pcc, ca->getft);
        comm_jigen_spd(pcc->spd, pcc->acc, ca->getft, p160a->uc0, p160a->uc1);
        comm_p160a_houb(v, pcc, ca->getft);
    }
}

static s32 pt_sys_pos_mh(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mh(ca, &ca->pos);
    return 1;
}

static s32 pt_sys_rot_mh(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mh(ca, &ca->rot);
    hr_pall_rot180V(ca->rot.moku);
    return 1;
}

static s32 pt_sys_pos_mhp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mhp(ca, &ca->pos);
    return 1;
}

static s32 pt_sys_rot_mhp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mhp(ca, &ca->rot);
    hr_pall_rot180V(ca->rot.moku);
    return 1;
}

void pt_set_mkmat(HR_CALL *ca, HR_PSYS *ps) {
    HR_CALL *pfg;

    pfg = hr_pfg_search(ps, 0x200);
    if (pfg != NULL) {
        pfg->flag &= -0x201;
    }
    ca->flag |= 0x200;
}

static s32 pt_sys_mk_mat(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    if (p32b->us0 == 1) {
        ca->flag &= -0x201;
    } else {
        if (ca->hObj == NULL) {
            pt_set_mkmat(ca, ps);
        }
    }

    ca->read++;
    return 1;
}

static s32 pt_sys_the_ft(HR_CALL *ca, HR_PSYS *ps) {
    HR_CALL *cap;

    p32b = (PT32B *)ca->read;
    cap = hr_id_search(ps, p32b->us0);
    if (cap != NULL) {
        ca->getft = cap->getft;
    }

    ca->read++;
    return 1;
}

static s32 pt_sys_time_ft(HR_CALL *ca, HR_PSYS *ps) {
    ca->time = ca->getft;
    ca->read++;
    return 0;
}

static s32 pt_sys_loop(HR_CALL *ca, HR_PSYS *ps) {
    p64a = (PT64A *)ca->read;
    if (p64a->ss0 < 2) {
        if (ca->loop[p64a->ss0] == 0) {
            ca->loop[p64a->ss0] = p64a->ss1;
            ca->read += p64a->ss2;
        } else {
            ca->loop[p64a->ss0]--;
            if (ca->loop[p64a->ss0] == 0) {
                ca->read += 2;
            } else {
                ca->read += p64a->ss2;
            }
        }
    } else {
        ca->read += 2;
    }

    return 1;
}

static s32 pt_sys_bgmode(HR_CALL *ca, HR_PSYS *ps) {
    p64a = (PT64A *)ca->read;
    hr_bg_onoff(p64a->ss1, p64a->ss0 == 0);
    ca->read += 2;
    return 1;
}

static s32 pt_sys_fogset(HR_CALL *ca, HR_PSYS *ps) {
    p64b = (PT64B *)ca->read;
    if (p64b->code == 0x4D) {
        mapvw.fog_near = p64b->si0;
    } else {
        mapvw.fog_far = p64b->si0;
    }
    hr_set_vpmfog(&VpmInfo);

    ca->read += 2;
    return 1;
}

static s32 pt_sys_next(HR_CALL *ca, HR_PSYS *ps) {
    p64a = (PT64A *)ca->read;
    ps->flag |= 0x40;
    ps->ncVi = p64a->ss0 * 256 + p64a->ss1;
    ps->ncSc = p64a->ss2;
    ca->read += 2;
    return 1;
}

static s32 pt_sys_vtwave(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    hrVT_allwaveErase();
    hrVT_waveSet(p32a->ss0);
    ca->read++;
    return 1;
}

static s32 pt_sys_del(HR_CALL *ca, HR_PSYS *ps) {
    return 0;
}

static s32 pt_sys_ntscj(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ca->read += p32a->ss0;
    return 1;
}

static s32 pt_sys_palj(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ca->read++;
    return 1;
}

static s32 pt_cam_pos(HR_CALL *ca, HR_PSYS *ps) {
    p128a = (PT128A *)ca->read;
    hr_pcam_set_pos(p128a->f0, p128a->f1, p128a->f2);
    ca->read += 4;
    return 1;
}

static s32 pt_cam_work(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    hr_pcam_set_mode(p32b->us0);
    ca->read++;
    return 1;
}

static void pt_comm_cam_hold(HR_CALL *ca, PCAMSM *pcm) {
    p32b = (PT32B *)ca->read;
    if (p32b->us0 == 0) {
        pcm->flag |= 5;
    } else {
        pcm->flag &= -5;
    }

    ca->read++;
}

static s32 pt_cam_hold(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_hold(ca, &ps->cam->mp);
    return 1;
}

static void pt_comm_cam_vec(HR_CALL *ca, HR_PSYS *ps, PCAMSC *pcc, PCAMSM *pcm) {
    sceVu0FVECTOR *v;
    s32 mode;
    SFXOBJ *pSfx;

    p128a = (PT128A *)ca->read;
    v = NULL;
    mode = 0;
    if (ca->hObj != NULL) {
        mode = 1;
        if (p128a->ss0 == -1) {
            v = &ca->hObj->Base.LcMtx[3];
        } else {
            pSfx = ca->hObj->pSfx;
            v = &pSfx->pMot->pCoord[p128a->ss0].Mtx[3];
        }
    } else {
        if (p128a->ss0 == -2) {
            mode = 2;
            v = &ca->pos.p;
        } else if (p128a->ss0 == -3) {
            mode = 3;
            v = &ca->pos.p;
        } else if (ca->flag & 0x200) {
            mode = 1;
            hr_pcall_mkm(ca->pos.p, ca->rot.p, ps->wmat);
            v = &ps->wmat[3];
        }
    }

    if (v != NULL) {
        hr_pcam_vhold(pcc, pcm, v, mode);
        pcc->p[0] = p128a->f0;
        pcc->p[1] = p128a->f1;
        pcc->p[2] = p128a->f2;
    }

    ca->read += 4;
}

static void pt_comm_posc(HR_CALL *ca, HR_PSYS *ps) {
    if (ca->hObj != NULL) {
        ps->flag |= 2;
        ps->posc = ca;
    } else {
        ps->flag &= -3;
        ps->posc = NULL;
    }
}

static void pt_comm_intc(HR_CALL *ca, HR_PSYS *ps) {
    if (ca->hObj != NULL) {
        ps->flag |= 4;
        ps->intec = ca;
    } else {
        ps->flag &= -5;
        ps->intec = NULL;
    }
}

static s32 pt_cam_vec(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_vec(ca, ps, &ps->cam->wp, &ps->cam->mp);
    pt_comm_posc(ca, ps);
    return 1;
}

static void pt_comm_cam_mat(HR_CALL *ca, HR_PSYS *ps, PCAMSC *pcc, PCAMSM *pcm) {
    sceVu0FMATRIX *m;
    SFXOBJ *pSfx;

    p128a = (PT128A *)ca->read;
    m = NULL;
    if (ca->hObj != NULL) {
        if (p128a->ss0 == -1) {
            m = &ca->hObj->Base.LcMtx;
        } else {
            pSfx = ca->hObj->pSfx;
            m = &pSfx->pMot->pCoord[p128a->ss0].Mtx;
        }
    } else if (ca->flag & 0x200) {
        hr_pcall_mkm(ca->pos.p, ca->rot.p, ps->wmat);
        m = &ps->wmat;
    }

    if (m != NULL) {
        hr_pcam_mhold(pcc, pcm, m, 1);
        pcc->p[0] = p128a->f0;
        pcc->p[1] = p128a->f1;
        pcc->p[2] = p128a->f2;
    }

    ca->read += 4;
}

static s32 pt_cam_mat(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_mat(ca, ps, &ps->cam->wp, &ps->cam->mp);
    pt_comm_posc(ca, ps);
    return 1;
}

static s32 pt_icam_hold(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_hold(ca, &ps->cam->mi);
    return 1;
}

static s32 pt_icam_vec(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_vec(ca, ps, &ps->cam->wi, &ps->cam->mi);
    pt_comm_intc(ca, ps);
    return 1;
}

static s32 pt_icam_mat(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_mat(ca, ps, &ps->cam->wi, &ps->cam->mi);
    pt_comm_intc(ca, ps);
    return 1;
}

static s32 pt_cam_spd(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ps->cam->wp.spd);
    pt_check_spdset(&ps->cam->wp);
    return 1;
}

static s32 pt_cam_acc(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ps->cam->wp.acc);
    ps->cam->wp.flag |= 1;
    return 1;
}

static s32 pt_icam_pos(HR_CALL *ca, HR_PSYS *ps) {
    p128a = (PT128A *)ca->read;
    hr_pcam_set_inte(p128a->f0, p128a->f1, p128a->f2);
    ca->read += 4;
    return 1;
}

static s32 pt_icam_spd(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ps->cam->wi.spd);
    pt_check_spdset(&ps->cam->wi);
    return 1;
}

static s32 pt_icam_acc(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ps->cam->wi.acc);
    ps->cam->wi.flag |= 1;
    return 1;
}

static s32 pt_ang_set(HR_CALL *ca, HR_PSYS *ps) {
    p128a = (PT128A *)ca->read;
    hr_pcam_set_ang(p128a->f0, p128a->f1, p128a->f2, 0);
    ca->read += 4;
    hr_pcam_spdclr(&ps->cam->wa);
    return 1;
}

static s32 pt_ang_spd(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ps->cam->wa.spd);
    pt_check_spdset(&ps->cam->wa);
    return 1;
}

static s32 pt_ang_acc(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ps->cam->wa.acc);
    ps->cam->wa.flag |= 1;
    return 1;
}

static void comm_pt_clen(HR_CALL *ca, PCAMSL *pcl, sceVu0FVECTOR f, s32 flag) {
    p64c = (PT64C *)ca->read;
    f[0] = p64c->f0;
    if (flag & 1) {
        pcl->spd = 0.0f;
    }
    if (flag & 2) {
        pcl->acc = 0.0f;
    }

    ca->read += 2;
}

static s32 pt_clen_set(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_clen(ca, &ps->cam->len, &ps->cam->len.p, 3);
    return 1;
}

static s32 pt_clen_spd(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_clen(ca, &ps->cam->len, &ps->cam->len.spd, 2);
    return 1;
}

static s32 pt_clen_acc(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_clen(ca, &ps->cam->len, &ps->cam->len.acc, 0);
    return 1;
}

static void pt_comm_cam_matg(HR_CALL *ca, HR_PSYS *ps, sceVu0FVECTOR *pi, PCAMSC *pcc, PCAMSM *pcm, char fg) {
    sceVu0FMATRIX *m;
    SFXOBJ *pSfx;

    p32a = (PT32A *)ca->read;
    if (ca->hObj != NULL) {
        if (p32a->ss0 == -1) {
            hr_pt_getpos(ca, &ca->hObj->Base.Trans);
            hr_pt_getrot(ca, &ca->hObj->Base.Rot);
            hr_take_GetLCMatYXZ(&ca->hObj->Base);
            m = &ca->hObj->Base.LcMtx;
        } else {
            pSfx = ca->hObj->pSfx;
            m = &pSfx->pMot->pCoord[p32a->ss0].Mtx;
        }

        if (fg == 'm') {
            hr_pcam_mhold(pcc, pcm, m, 1);
        } else {
            hr_pcam_vhold(pcc, pcm, (sceVu0FVECTOR *)(*m)[3], 1);
        }

        hr_pcam_gyakusan(pi, pcc, pcm);
    } else if (p32a->ss0 == -2) {
        hr_pcam_vhold(pcc, pcm, &ca->pos.p, 2);
        hr_pcam_gyakusan(pi, pcc, pcm);
    } else if (ca->flag & 0x200) {
        hr_pcall_mkm(ca->pos.p, ca->rot.p, ps->wmat);
        hr_pcam_mhold(pcc, pcm, &ps->wmat, 1);
        hr_pcam_gyakusan(pi, pcc, pcm);
    }

    ca->read++;
}

static s32 pt_cam_matg(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_matg(ca, ps, &ps->cam->pos, &ps->cam->wp, &ps->cam->mp, 'm');
    pt_comm_posc(ca, ps);
    return 1;
}

static s32 pt_icam_matg(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_matg(ca, ps, &ps->cam->inte, &ps->cam->wi, &ps->cam->mi, 'm');
    pt_comm_posc(ca, ps);
    return 1;
}

static s32 pt_cam_mvp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvp(ca, &ps->cam->wp);
    return 1;
}

static s32 pt_icam_mvp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvp(ca, &ps->cam->wi);
    return 1;
}

static s32 pt_cam_vecg(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_matg(ca, ps, &ps->cam->pos, &ps->cam->wp, &ps->cam->mp, 'v');
    pt_comm_posc(ca, ps);
    return 1;
}

static s32 pt_icam_vecg(HR_CALL *ca, HR_PSYS *ps) {
    pt_comm_cam_matg(ca, ps, &ps->cam->inte, &ps->cam->wi, &ps->cam->mi, 'v');
    pt_comm_intc(ca, ps);
    return 1;
}

static s32 pt_cam_lrset(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ps->cam->mp.r.p);
    ps->cam->mp.flag |= 1;
    hr_pcam_spdclr(&ps->cam->mp.r);
    return 1;
}

static s32 pt_cam_lrspd(HR_CALL *ca, HR_PSYS *ps) {
    PCAMSC *pcc;

    pcc = &ps->cam->mp.r;
    comm_pt_sys_pos(ca, pcc->spd);
    pt_check_spdset(pcc);
    return 1;
}

static s32 pt_cam_lracc(HR_CALL *ca, HR_PSYS *ps) {
    PCAMSC *pcc;

    pcc = &ps->cam->mp.r;
    comm_pt_sys_pos(ca, pcc->acc);
    pcc->flag |= 1;
    return 1;
}

static s32 pt_cam_lrmvp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvp(ca, &ps->cam->mp.r);
    return 1;
}

static s32 pt_icam_lrset(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_sys_pos(ca, ps->cam->mi.r.p);
    ps->cam->mi.flag |= 1;
    hr_pcam_spdclr(&ps->cam->mi.r);
    return 1;
}

static s32 pt_icam_lrspd(HR_CALL *ca, HR_PSYS *ps) {
    PCAMSC *pcc;

    pcc = &ps->cam->mi.r;
    comm_pt_sys_pos(ca, pcc->spd);
    pt_check_spdset(pcc);
    return 1;
}

static s32 pt_icam_lracc(HR_CALL *ca, HR_PSYS *ps) {
    PCAMSC *pcc;

    pcc = &ps->cam->mi.r;
    comm_pt_sys_pos(ca, pcc->acc);
    pcc->flag |= 1;
    return 1;
}

static s32 pt_icam_lrmvp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvp(ca, &ps->cam->mi.r);
    return 1;
}

static s32 pt_cam_lrmv(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mv(ca, &ps->cam->mp.r);
    hr_pall_rot180V(ps->cam->mp.r.moku);
    return 1;
}

static s32 pt_icam_lrmv(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mv(ca, &ps->cam->mi.r);
    hr_pall_rot180V(ps->cam->mi.r.moku);
    return 1;
}

static s32 pt_cam_mv(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mv(ca, &ps->cam->wp);
    return 1;
}

static s32 pt_icam_mv(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mv(ca, &ps->cam->wi);
    return 1;
}

static s32 pt_ang_mv(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mv(ca, &ps->cam->wa);
    hr_pall_rot180V(ps->cam->wa.moku);
    return 1;
}

static s32 pt_ang_mvp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvp(ca, &ps->cam->wa);
    hr_pall_rot180V(ps->cam->wa.moku);
    return 1;
}

static s32 pt_cam_spd1(HR_CALL *ca, HR_PSYS *ps) {
    comm_sys_spd1(ca, &ps->cam->wp);
    return 1;
}

static s32 pt_icam_spd1(HR_CALL *ca, HR_PSYS *ps) {
    comm_sys_spd1(ca, &ps->cam->wi);
    return 1;
}

static s32 pt_cam_lrspd1(HR_CALL *ca, HR_PSYS *ps) {
    comm_sys_spd1(ca, &ps->cam->mp.r);
    return 1;
}

static s32 pt_icam_lrspd1(HR_CALL *ca, HR_PSYS *ps) {
    comm_sys_spd1(ca, &ps->cam->mi.r);
    return 1;
}

static s32 pt_ang_spd1(HR_CALL *ca, HR_PSYS *ps) {
    comm_sys_spd1(ca, &ps->cam->wa);
    return 1;
}

static s32 pt_cam_data(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    hr_pcam_set_beta(pt_al_data(ca, p32a->ss0));
    ca->read++;
    return 1;
}

static void comm_p64f_read(HR_CALL *ca, f32 *p) {
    p64f = (PT64F *)ca->read;
    *p = p64f->f0;
}

static void comm_p64f_spd(HR_CALL *ca, PCAMSL *pcf) {
    sceVu0FVECTOR spd;
    sceVu0FVECTOR acc;

    pcf->time = ca->getft;
    pcf->flag |= 1;
    spd[0] = spd[1] = spd[2] = pcf->spd;
    spd[3] = 0.0f;
    sceVu0SubVector(acc, spd, spd);
    comm_jigen_spd(spd, acc, ca->getft, p64f->uc0, p64f->uc1);
    pcf->spd = spd[0];
    pcf->acc = acc[0];
}

void hr_pcamsl_spdclr(PCAMSL *pcf) {
    pcf->spd = pcf->acc = 0.0f;
    pcf->flag &= -2;
}

void comm_getft_mvF(HR_CALL *ca, PCAMSL *pcf) {
    comm_p64f_read(ca, &pcf->spd);
    if (ca->getft == 0) {
        hr_pcamsl_spdclr(pcf);
    } else {
        pcf->moku = pcf->p + pcf->spd;
        comm_p64f_spd(ca, pcf);
    }
}

void comm_getft_mvpF(HR_CALL *ca, PCAMSL *pcf) {
    comm_p64f_read(ca, &pcf->moku);
    if (ca->getft == 0) {
        hr_pcamsl_spdclr(pcf);
    } else {
        pcf->spd = pcf->moku - pcf->p;
        comm_p64f_spd(ca, pcf);
    }
}

static s32 pt_clen_mvp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvpF(ca, &ps->cam->len);
    ca->read += 2;
    return 1;
}

static s32 pt_clen_mv(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvF(ca, &ps->cam->len);
    ca->read += 2;
    return 1;
}

static s32 pt_proj_set(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_clen(ca, &ps->cam->proj, &ps->cam->proj.p, 3);
    return 1;
}

static s32 pt_proj_spd(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_clen(ca, &ps->cam->proj, &ps->cam->proj.spd, 2);
    return 1;
}

static s32 pt_proj_acc(HR_CALL *ca, HR_PSYS *ps) {
    comm_pt_clen(ca, &ps->cam->proj, &ps->cam->proj.acc, 0);
    return 1;
}

static s32 pt_proj_mvp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvpF(ca, &ps->cam->proj);
    ca->read += 2;
    return 1;
}

static s32 pt_proj_mv(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mvF(ca, &ps->cam->proj);
    ca->read += 2;
    return 1;
}

static s32 pt_cam_mhp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mhp(ca, &ps->cam->wp);
    return 1;
}

static s32 pt_icam_mhp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mhp(ca, &ps->cam->wi);
    return 1;
}

static s32 pt_cam_lrmhp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mhp(ca, &ps->cam->mp.r);
    return 1;
}

static s32 pt_icam_lrmhp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mhp(ca, &ps->cam->mi.r);
    return 1;
}

static s32 pt_cam_lrmh(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mh(ca, &ps->cam->mp.r);
    return 1;
}

static s32 pt_icam_lrmh(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mh(ca, &ps->cam->mi.r);
    return 1;
}

static s32 pt_cam_mh(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mh(ca, &ps->cam->wp);
    return 1;
}

static s32 pt_icam_mh(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mh(ca, &ps->cam->wi);
    return 1;
}

static s32 pt_ang_mh(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mh(ca, &ps->cam->wa);
    hr_pall_rot180V(ps->cam->wa.moku);
    return 1;
}

static s32 pt_ang_mhp(HR_CALL *ca, HR_PSYS *ps) {
    comm_getft_mhp(ca, &ps->cam->wa);
    hr_pall_rot180V(ps->cam->wa.moku);
    return 1;
}

static s32 (*HrPtSysTbl[85])(HR_CALL *ca, HR_PSYS *ps) = {
    pt_sys_call,
    pt_sys_end,
    pt_sys_time,
    pt_sys_sub,
    pt_sys_ret,
    pt_sys_type,
    pt_sys_pos,
    pt_sys_rot,
    pt_sys_draw,
    pt_sys_jmp,
    pt_sys_flag,
    pt_sys_flagj,
    pt_sys_pos_spd,
    pt_sys_pos_acc,
    pt_sys_rot_spd,
    pt_sys_rot_acc,
    pt_sys_pos_mv,
    pt_sys_callclr,
    pt_sys_rot_mv,
    pt_sys_timemp,
    pt_sys_time,
    pt_sys_nlight,
    pt_sys_lcolor,
    pt_sys_ambient,
    pt_sys_size,
    pt_sys_mes_data,
    pt_sys_mes_start,
    pt_sys_mes_window,
    pt_sys_mes_spd,
    pt_sys_mes_reta,
    pt_sys_get_ft,
    pt_sys_pos_mvp,
    pt_sys_rot_mvp,
    pt_sys_scale,
    pt_sys_pos_spd1,
    pt_sys_rot_spd1,
    pt_sys_light_no,
    pt_sys_light_sw,
    pt_sys_ltcol_spd,
    pt_sys_ltcol_acc,
    pt_sys_ltcol_mv,
    pt_sys_ltcol_mvp,
    pt_sys_ltcol_spd1,
    pt_sys_ltrot_spd,
    pt_sys_ltrot_acc,
    pt_sys_ltrot_mv,
    pt_sys_ltrot_mvp,
    pt_sys_ltrot_spd1,
    pt_sys_effect,
    pt_sys_col,
    pt_sys_col_in,
    pt_sys_col_out,
    pt_sys_pos_cp,
    pt_sys_rot_cp,
    pt_sys_pos_mvpc,
    pt_sys_ltrot,
    pt_sys_vtmode,
    pt_sys_pos_cpt,
    pt_sys_id,
    pt_sys_chr,
    pt_sys_exit,
    pt_sys_acol,
    pt_sys_acol_in,
    pt_sys_acol_out,
    pt_sys_econt,
    pt_sys_xfade,
    pt_sys_xfade_w,
    pt_sys_pos_mh,
    pt_sys_rot_mh,
    pt_sys_pos_mhp,
    pt_sys_rot_mhp,
    pt_sys_mk_mat,
    pt_sys_pos_cp,
    pt_sys_the_ft,
    pt_sys_time_ft,
    pt_sys_loop,
    pt_sys_bgmode,
    pt_sys_fogset,
    pt_sys_fogset,
    pt_sys_next,
    pt_sys_vtwave,
    pt_sys_del,
    pt_sys_mes_nskip,
    pt_sys_ntscj,
    pt_sys_palj,
};

static s32 (*HrPtCamTbl[62])(HR_CALL *ca, HR_PSYS *ps) = {
    pt_cam_pos,
    pt_cam_work,
    pt_cam_hold,
    pt_cam_vec,
    pt_cam_mat,
    pt_icam_hold,
    pt_icam_vec,
    pt_icam_mat,
    pt_cam_spd,
    pt_cam_acc,
    pt_icam_pos,
    pt_icam_spd,
    pt_icam_acc,
    pt_ang_set,
    pt_ang_spd,
    pt_ang_acc,
    pt_clen_set,
    pt_clen_spd,
    pt_clen_acc,
    pt_cam_matg,
    pt_icam_matg,
    pt_cam_mvp,
    pt_icam_mvp,
    pt_cam_vecg,
    pt_icam_vecg,
    pt_cam_lrset,
    pt_cam_lrspd,
    pt_cam_lracc,
    pt_cam_lrmvp,
    pt_icam_lrset,
    pt_icam_lrspd,
    pt_icam_lracc,
    pt_icam_lrmvp,
    pt_cam_lrmv,
    pt_icam_lrmv,
    pt_cam_mv,
    pt_icam_mv,
    pt_ang_mv,
    pt_ang_mvp,
    pt_cam_spd1,
    pt_icam_spd1,
    pt_cam_lrspd1,
    pt_icam_lrspd1,
    pt_ang_spd1,
    pt_cam_data,
    pt_clen_mvp,
    pt_clen_mv,
    pt_proj_set,
    pt_proj_spd,
    pt_proj_acc,
    pt_proj_mvp,
    pt_proj_mv,
    pt_cam_mhp,
    pt_icam_mhp,
    pt_cam_lrmhp,
    pt_icam_lrmhp,
    pt_cam_lrmh,
    pt_icam_lrmh,
    pt_cam_mh,
    pt_icam_mh,
    pt_ang_mh,
    pt_ang_mhp,
};

s32 (**hrPtFuncTbl[5])(HR_CALL *ca, HR_PSYS *ps) = {
    HrPtSysTbl,
    HrPtMotTbl,
    HrPtCamTbl,
    HrPtKl2Tbl,
    HrPtSndTbl
};