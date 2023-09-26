#include "harada/hr_preak.h"
#include "harada/hr_pread.h"
#include "harada/hr_nak.h"
#include "harada/hr_pcam.h"
#include "harada/hr_pall.h"
#include "harada/hr_pbgm.h"
#include "harada/hr_take.h"

static PT32A *p32a;
static PT32B *p32b;
static PT64A *p64a;
static PT64B *p64b;
static PT64C *p64c;
static PT64D *p64d;
static PT64E *p64e;
static PT96A *p96a;
static PT96B *p96b;
static PT96D *p96d;
static PT128C *p128c;

void hr_rt_movework(HR_CALL *ca) {
    ca->route.spd += ca->route.acc;
    ca->route.p += ca->route.spd;
    if (ca->route.spd != 0) {
        MichiMove(&ca->rtw, ca->route.spd);
    }
    hr_rt_posrot(ca);
}

static s32 pt_k_rtdata(HR_CALL *ca,  HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    if (p32a->ss0 != 0) {
        ca->route.addr = pt_al_data(ca, p32a->ss0);
    }
    ca->read++;
    return 1;
}

static s32 pt_k_rtpos(HR_CALL *ca, HR_PSYS *ps) {
    p64b = (PT64B *)ca->read;
    pt_set_rtpos(ca, p64b->si0, p64b->us0);
    ca->read += 2;
    return 1;
}

static s32 pt_k_back(HR_CALL *ca, HR_PSYS *ps) {
    p64b = (PT64B *)ca->read;
    ps->klono = p64b->us0;
    ps->klomc = p64b->si0;
    ps->flag |= 0x10;
    ca->read += 2;
    return 1;
}

static s32 pt_k_rtspd(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ca->route.spd = p32a->ss0;
    ca->route.acc = 0;
    ca->read++;
    return 1;
}

static s32 pt_k_rtacc(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ca->route.acc = p32a->ss0;
    ca->read++;
    return 1;
}

static s32 pt_k_rtoff(HR_CALL *ca, HR_PSYS *ps) {
    hr_pcall_rtinit(ca);
    ca->read++;
    return 1;
}

static void comm_p64d_read(HR_CALL *ca, s32 *p) {
    p64d = (PT64D *)ca->read;
    *p = p64d->si0;
    ca->read += 2;
}

static void comm_p64d_spd(HR_CALL *ca, PCAMSR *pcr) {
    sceVu0FVECTOR spd;
    sceVu0FVECTOR acc;

    pcr->time = ca->getft;
    pcr->flag |= 1;
    spd[0] = spd[1] = spd[2] = pcr->spd;
    spd[3] = 0.0f;
    sceVu0SubVector(acc, spd, spd);
    comm_jigen_spd(spd, acc, ca->getft, p64d->uc0, p64d->uc1);
    pcr->spd = spd[0];
    pcr->acc = acc[0];
}

void hr_pcamsr_spdclr(PCAMSR *pcr) {
    pcr->acc = 0;
    pcr->spd = 0;
    pcr->flag &= -2;
}

static void comm_getft_mvR(HR_CALL *ca, PCAMSR *pcr) {
    comm_p64d_read(ca, &pcr->spd);
    if (ca->getft == 0) {
        hr_pcamsr_spdclr(pcr);
    } else {
        pcr->moku = pcr->p + pcr->spd;
        comm_p64d_spd(ca, pcr);
    }
}

static void comm_getft_mvpR(HR_CALL *ca, PCAMSR *pcr) {
    comm_p64d_read(ca, &pcr->moku);
    if (ca->getft == 0) {
        hr_pcamsr_spdclr(pcr);
    } else {
        pcr->spd = pcr->moku - pcr->p;
        comm_p64d_spd(ca, pcr);
    }
}

static s32 pt_k_rtmv(HR_CALL *ca, HR_PSYS *ps) {
    if (!(ca->flag & 0x20)) {
        ca->read += 2;
    } else {
        comm_getft_mvR(ca, &ca->route);
    }
    return 1;
}

static s32 pt_k_rtmvp(HR_CALL *ca, HR_PSYS *ps) {
    if (!(ca->flag & 0x20)) {
        ca->read += 2;
    } else {
        comm_getft_mvpR(ca, &ca->route);
    }
    return 1;
}

static s32 pt_k_strt(HR_CALL *ca, HR_PSYS *ps) {
    p96a = (PT96A *)ca->read;
    ca->read += 3;
    return 1;
}

static s32 pt_k_stsw(HR_CALL *ca, HR_PSYS *ps) {
    p64a = (PT64A *)ca->read;
    ca->read += 2;
    return 1;
}

static s32 pt_k_rtwait(HR_CALL *ca, HR_PSYS *ps) {
    if (hr_strt_check((PT96A *)ca->read, ps->rtno, ps->rtmc, ps->yuka)) {
        ca->read += 3;
        return 1;
    } else {
        return 0;
    }
}

static s32 pt_k_copy(HR_CALL *ca, HR_PSYS *ps) {
    ps->flag |= 0x80000000;
    nk_ptget_klonoa(ca->pos.p, ca->rot.p, &ca->rtw, &ps->rtno,& ps->rtmc, &ps->yuka);
    pt_set_mkmat(ca, ps);
    ca->read += 1;
    return 1;
}

static s32 pt_k_camcat(HR_CALL *ca, HR_PSYS *ps) {
    ptcat_camera();
    ps->flag |= 0x2000;
    ca->read++;
    return 1;
}

static s32 pt_k_camrel(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ptrel_camera(p32a->ss0);
    ps->flag &= -0x2001;
    ca->read++;
    return 1;
}

static s32 pt_k_w2rt(HR_CALL *ca, HR_PSYS *ps) {
    sceVu0FVECTOR v;

    hr_pcall_rtinit(ca);
    pt_k_rtpos(ca, ps);
    hr_rt_movework(ca);
    sceVu0SubVector(v, ca->pos.p, ca->rtpos);
    v[3] = 1.0f;
    hr_pcam_cinit(&ca->pos);
    sceVu0CopyVector(ca->pos.p, v);
    sceVu0ScaleVectorXYZ(v, ca->rtrot, 180.0f);
    sceVu0DivVectorXYZ(v, v, M_PIf);
    sceVu0SubVector(v, ca->rot.p, v);
    v[3] = 1.0f;
    hr_pcam_cinit(&ca->rot);
    sceVu0CopyVector(ca->rot.p, v);
    return 1;
}

static s32 pt_k_rt2w(HR_CALL *ca, HR_PSYS *ps) {
    sceVu0FVECTOR v;

    p32b = (PT32B *)ca->read;
    if (ca->flag & 0x20) {
        hr_pcamsr_spdclr(&ca->route);
        hr_rt_movework(ca);
        sceVu0AddVector(v, ca->rtpos, ca->pos.p);
        v[3] = 1.0f;
        hr_pcam_cinit(&ca->pos);
        sceVu0CopyVector(ca->pos.p, v);
        sceVu0ScaleVectorXYZ(v, ca->rtrot, 180.0f);
        sceVu0DivVectorXYZ(v, v, M_PIf);
        sceVu0AddVector(v, v, ca->rot.p);
        v[3] = 1.0f;
        hr_pcam_cinit(&ca->rot);
        sceVu0CopyVector(ca->rot.p, v);
        hr_pcall_rtinit(ca);
    }
    ca->read++;
    return 1;
}

static s32 pt_k_gamej(HR_CALL *ca, HR_PSYS *ps) {
    s32 fg;

    p64a = (PT64A *)ca->read;
    if (hrpt_flag & 2) {
        fg = 1;
    } else {
        fg = 0;
    }

    if (p64a->ss0 == 1) {
        fg ^= 1;
    }

    if (fg) {
        ca->read += p64a->ss1;
    } else {
        ca->read += 2;
    }

    return 1;
}

static s32 pt_k_gamer(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    pt_get_routeK(ca);
    ca->read++;
    return 1;
}

static s32 pt_k_posj(HR_CALL *ca, HR_PSYS *ps) {
    f32 pos;
    s32 fg;

    p96b = (PT96B *)ca->read;
    fg = 0;
    pos = ca->pos.p[p96b->us0];

    switch (p96b->us1) {
        case PTPOSJ_G:
            if (pos > p96b->f0) {
                fg++;
            }
            break;
        case PTPOSJ_GE:
            if (pos >= p96b->f0) {
                fg++;
            }
            break;
        case PTPOSJ_E:
            if (pos == p96b->f0) {
                fg++;
            }
            break;
        case PTPOSJ_LE:
            if (pos <= p96b->f0) {
                fg++;
            }
            break;
        case PTPOSJ_L:
            if (pos < p96b->f0) {
                fg++;
            }
            break;
        case PTPOSJ_NE:
            if (pos != p96b->f0) {
                fg++;
            }
            break;
    }

    if (fg) {
        ca->read += p96b->ss0;
    } else {
        ca->read += 3;
    }

    return 1;
}

static s32 pt_k_key(HR_CALL *ca, HR_PSYS *ps) {
    p64b = (PT64B *)ca->read;

    switch (p64b->us0) {
        case PTKEY_SYS:
            if (p64b->si0 == 0x40000000) {
                if (!ps->key.open) {
                    hr_pkey_init(ps->key);
                    ps->key.open++;
                    hr_open_klokey(1);
                }
            } else if (ps->key.open) {
                ps->key.open = 0;
                hr_open_klokey(0);
            }
            break;
        case PTKEY_TRG:
            hr_pkey_trg(&ps->key, p64b->si0);
            break;
        case PTKEY_LVL:
            hr_pkey_lvl(&ps->key, p64b->si0);
            break;
    }

    ca->read += 2;
    return 1;
}

static s32 pt_k_rtjmp(HR_CALL *ca, HR_PSYS *ps) {
    s32 fg;

    fg = hr_strt_check((PT96A *)ca->read, ps->rtno, ps->rtmc, ps->yuka);

    p128c = (PT128C *)ca->read;
    if (p128c->ss2 == 1) {
        fg ^= 1;
    }

    if (fg) {
        ca->read += p128c->ss1;
    } else {
        ca->read += 4;
    }

    return 1;
}

static s32 pt_k_cent(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    if (ps->flag < 0) {
        hr_pklo_center(p32b->us0);
    }

    ca->read++;
    return 1;
}

static s32 pt_k_keyj(HR_CALL *ca, HR_PSYS *ps) {
    kPadDATA *kpd;
    s32 k;
    s32 i;

    kpd = &GameGbl.kpd[0];
    p96a = (PT96A *)ca->read;
    switch (p96a->us0) {
        case PTKEY_TRG:
            k = kpd->trg;
            break;
        case PTKEY_LVL:
            k = kpd->lvl;
            break;
        default:
            k = 0;
            break;
    }

    i = hr_pkey_change(p96a->ui0);
    k &= i;
    if ((p96a->ui0 == 0x4000 || p96a->ui0 == 0x8000) && k) {
        k = i;
    }
    if (i == k) {
        i = 1;
    } else {
        i = 0;
    }
    if (p96a->us1 == 1) {
        i ^= 1;
    }

    if (i) {
        ca->read += p96a->ss0;
    } else {
        ca->read += 3;
    }
    return 1;
}

static s32 pt_k_jump(HR_CALL *ca, HR_PSYS *ps) {
    p96d = (PT96D *)ca->read;
    hr_pklo_jump(p96d->f0, p96d->si0);
    ca->read += 3;
    return 1;
}

static s32 pt_k_shadow(HR_CALL *ca, HR_PSYS *ps) {
    p64c = (PT64C *)ca->read;
    if (p64c->us0 == 1) {
        ca->flag &= -0x401;
    } else if (ca->hObj != NULL) {
        ca->flag |= 0x400;
    }

    ca->read += 2;
    return 1;
}

static s32 pt_k_mir(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    if (p32b->us0 == 0) {
        if (p32b->code == 0x31A) {
            ca->flag |= 0x800;
        } else {
            ca->flag |= 0x1000;
            if (ca->hObj != NULL) {
                hr_take_rinkaku(ca->hObj);
            }
        }
        ps->mircnt++;
    } else {
        ca->flag &= -0x1801;
        if (ps->mircnt != 0) {
            ps->mircnt--;
        }
    }

    ca->read++;
    return 1;
}

static s32 pt_k_mesp(HR_CALL *ca, HR_PSYS *ps) {
    p64a = (PT64A *)ca->read;
    if (p64a->ss0 == 0) {
        pt_set_mesp(ca, ps, pt_al_data(ca, p64a->ss1), p64a->ss2);
    } else {
        pt_del_mesp(ca, ps, pt_al_data(ca, p64a->ss1), p64a->ss2);
    }

    ca->read += 2;
    return 1;
}

static s32 pt_k_backv(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    ps->klov = p32b->us0;
    ca->read++;
    return 1;
}

static s32 pt_k_backy(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    ps->kloy = p32a->ss0;
    ps->flag |= 0x100;
    ca->read++;
    return 1;
}

static s32 pt_k_switch(HR_CALL *ca, HR_PSYS *ps) {
    p64a = (PT64A *)ca->read;
    if (p64a->ss0 == 0) {
        OkSetSwitchFlag(4, p64a->ss1);
    }

    ca->read += 2;
    return 1;
}

static s32 pt_k_vclr(HR_CALL *ca, HR_PSYS *ps) {
    ps->flag |= 0x200;
    hrpt_patch |= 0x10;
    hrpt_patcnt = 0xEE;
    hrpt_patch |= 0x20;
    kzVisionClearOn();
    ca->read++;
    return 1;
}

static s32 pt_k_reset(HR_CALL *ca, HR_PSYS *ps) {
    hrptm_res = 1;
    ca->read++;
    return 1;
}

static s32 pt_k_nskip(HR_CALL *ca, HR_PSYS *ps) {
    ps->flag |= 0x200;
    p32a = (PT32A *)ca->read;
    if (p32a->ss0 == 0) {
        hr_hse_areaend();
        hr_pt_setBGM(ps, -1, 0);
    }

    ca->read++;
    return 1;
}

static s32 pt_k_lcol(HR_CALL *ca, HR_PSYS *ps) {
    p64e = (PT64E *)ca->read;
    ps->flag |= 0x800;
    ps->r = p64e->uc0;
    ps->g = p64e->uc1;
    ps->b = p64e->uc2;
    ps->a = 0x80;

    ca->read += 2;
    return 1;
}

s32 (*HrPtKl2Tbl[36])(HR_CALL *ca, HR_PSYS *ps) = {
    pt_k_rtdata,
    pt_k_rtpos,
    pt_k_rtspd,
    pt_k_rtacc,
    pt_k_rtoff,
    pt_k_rtmv,
    pt_k_rtmvp,
    pt_k_strt,
    pt_k_stsw,
    pt_k_rtwait,
    pt_k_copy,
    pt_k_camcat,
    pt_k_camrel,
    pt_k_w2rt,
    pt_k_rt2w,
    pt_k_gamej,
    pt_k_gamer,
    pt_k_back,
    pt_k_posj,
    pt_k_key,
    pt_k_rtjmp,
    pt_k_cent,
    pt_k_keyj,
    pt_k_jump,
    pt_k_shadow,
    pt_k_stsw,
    pt_k_mir,
    pt_k_mesp,
    pt_k_mir,
    pt_k_backv,
    pt_k_backy,
    pt_k_switch,
    pt_k_vclr,
    pt_k_reset,
    pt_k_nskip,
    pt_k_lcol,
};
