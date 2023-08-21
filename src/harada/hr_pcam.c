#include "harada/hr_pcam.h"
#include "harada/hr_pall.h"
#include "harada/hr_obcon1.h"

void hr_pcam_pr2inte(sceVu0FVECTOR pos, sceVu0FVECTOR ang, sceVu0FVECTOR inte, f32 len) {
    hrvi[0] = hrvi[1] = 0.0f;
    hrvi[2] = len;
    hrvi[3] = 1.0f;
    sceVu0UnitMatrix(hrpum);
    sceVu0RotMatrixX(hrpm, hrpum, -ang[0]);
    sceVu0RotMatrixY(hrpm, hrpm, -ang[1]);
    sceVu0RotMatrixZ(hrpm, hrpm, ang[2]);
    sceVu0ApplyMatrix(hrvo, hrpm, hrvi);
    sceVu0AddVector(inte, pos, hrvo);
    inte[3] = 1.0f;
}

void hr_pcam_pi2rot(sceVu0FVECTOR pos, sceVu0FVECTOR inte, sceVu0FVECTOR ang) {
    f32 var;

    hrvi[0] = inte[0] - pos[0];
    hrvi[1] = inte[1] - pos[1];
    hrvi[2] = inte[2] - pos[2];
    var = sqrtf(hrvi[0] * hrvi[0] + hrvi[2] * hrvi[2]);
    ang[1] = -hr_pall_atan2(hrvi[0], hrvi[2]);
    ang[0] = hr_pall_atan2(hrvi[1], var);
}

void hr_pcam_cinit(PCAMSC *pcc) {
    pcc->p[0] = pcc->p[1] = pcc->p[2] = 0.0f;
    pcc->p[3] = 1.0f;
    pcc->spd[0] = pcc->spd[1] = pcc->spd[2] = pcc->spd[3] = 0.0f;
    sceVu0CopyVector(pcc->acc, pcc->spd);
    sceVu0CopyVector(pcc->moku, pcc->p);
    pcc->time = 0;
    pcc->flag = 0;
}

static void hr_pcam_minit(PCAMSM *pcm) {
    sceVu0ScaleVectorXYZ(pcm->v, pcm->v, 0.0f);
    pcm->v[3] = 1.0f;
    sceVu0CopyVector(pcm->hv, pcm->v);
    pcm->hdmk = NULL;
    pcm->hdv = NULL;
    sceVu0CopyVector(pcm->rr, pcm->v);
    hr_pcam_cinit(&pcm->r);
    pcm->flag = 0;
}

void hr_pcam_ang_init() {
    hr_pcam_cinit(&hrpcam->wa);
    sceVu0ScaleVectorXYZ(hrpcam->wa.p, hrpcam->ang, 180.0f);
    sceVu0DivVectorXYZ(hrpcam->wa.p, hrpcam->wa.p, M_PIf);
}

static void hr_pcam_linit(PCAMSL *pcl, f32 p) {
    memset(pcl, 0, sizeof(PCAMSL));
    pcl->p = p;
}

static void hr_pcam_world(PCAMSC *pcc, PCAMSM *pcm, sceVu0FVECTOR v, f32 x, f32 y, f32 z) {
    hr_pcam_cinit(pcc);
    v[0] = x;
    v[1] = y;
    v[2] = z;
    v[3] = 1.0f;
    sceVu0CopyVector(pcc->p, v);
    pcm->flag |= 0x10;
}

void hr_pcam_set_pos(f32 x, f32 y, f32 z) {
    hr_pcam_world(&hrpcam->wp, &hrpcam->mp, hrpcam->pos, x, y,z );
}

void hr_pcam_set_inte(f32 x, f32 y, f32 z) {
    hr_pcam_world(&hrpcam->wi, &hrpcam->mi, hrpcam->inte, x, y, z);
}

void hr_pcam_init(sceVu0FVECTOR pos, sceVu0FVECTOR ang, f32 proj) {
    hrpcam = &ppcam;
    sceVu0CopyVector(ppcam.pos, pos);
    sceVu0CopyVector(hrpcam->ang, ang);
    hr_pcam_pr2inte(hrpcam->pos, hrpcam->ang, hrpcam->inte, proj);
    hr_pcam_linit(&hrpcam->proj,proj);
    hr_pcam_linit(&hrpcam->len,proj);
    hrpcam->flag = PCAMW_PIA;
    hrpcam->bnow = 0;
    hrpcam->bmax = 0;
    hrpcam->bptr = NULL;
    hr_pcam_cinit(&hrpcam->wp);
    hr_pcam_cinit(&hrpcam->wi);
    hr_pcam_ang_init();
    hr_pcam_minit(&hrpcam->mp);
    hr_pcam_minit(&hrpcam->mi);
    hr_pcam_set_pos(pos[0], pos[1], pos[2]);
}

void hr_pcam_mhold(PCAMSC *pcc, PCAMSM *pcm, sceVu0FMATRIX *m, s32 fg) {
    if (fg != 0) {
        hr_pcam_cinit(pcc);
    }
    pcm->hdmk = m;
    sceVu0CopyMatrix(pcm->hm, *m);
    pcm->flag |= 0xB;
    pcm->flag &= -0x71;
}

void hr_pcam_vhold(PCAMSC *pcc, PCAMSM *pcm, sceVu0FVECTOR *v, s32 fg) {
    if (fg != 0) {
        hr_pcam_cinit(pcc);
    }
    pcm->hdv = v;
    sceVu0CopyVector(pcm->hv, *v);
    pcm->flag |= 9;
    pcm->flag &= -3;
    pcm->flag &= -0x71;
    if (fg == 2 || fg == 3) {
        pcm->flag |= 0x30;
        if (fg == 3) {
            pcm->flag |= 0x40;
        }
    }
}

void hr_setEnvelopeMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FMATRIX m2[4], sceVu0FMATRIX m3) {
    __asm__ volatile(
        "lqc2 $vf16,0x00(%0)\n"
        "lqc2 $vf17,0x10(%0)\n"
        "lqc2 $vf18,0x20(%0)\n"
        "lqc2 $vf19,0x30(%0)\n"
        "lqc2 $vf20,0x00(%1)\n"
        "lqc2 $vf21,0x10(%1)\n"
        "lqc2 $vf22,0x20(%1)\n"
        "lqc2 $vf23,0x30(%1)\n"
        "lqc2 $vf24,0x00(%2)\n"
        "lqc2 $vf25,0x10(%2)\n"
        "lqc2 $vf26,0x20(%2)\n"
        "lqc2 $vf27,0x30(%2)\n"
        "lqc2 $vf28,0x00(%3)\n"
        "lqc2 $vf29,0x10(%3)\n"
        "lqc2 $vf30,0x20(%3)\n"
        "lqc2 $vf31,0x30(%3)\n"
    : : "r" (m0), "r" (m1), "r" (m2), "r" (m3));
}

void hr_Envelope1vert(sceVu0FVECTOR *v, sceVu0FVECTOR *w, sceVu0FVECTOR *vo) {
    __asm__ volatile(
        "lqc2         $vf1,0x00(%1)\n"
        "lqc2         $vf2,0x00(%2)\n"
        "vmulax.xyzw  ACC,$vf16,$vf1x\n"
        "vmadday.xyzw ACC,$vf17,$vf1y\n"
        "vmaddaz.xyzw ACC,$vf18,$vf1z\n"
        "vmaddw.xyzw  $vf4,$vf19,$vf1w\n"
        "vmulax.xyzw  ACC,$vf20,$vf1x\n"
        "vmadday.xyzw ACC,$vf21,$vf1y\n"
        "vmaddaz.xyzw ACC,$vf22,$vf1z\n"
        "vmaddw.xyzw  $vf5,$vf23,$vf1w\n"
        "vmulax.xyzw  ACC,$vf24,$vf1x\n"
        "vmadday.xyzw ACC,$vf25,$vf1y\n"
        "vmaddaz.xyzw ACC,$vf26,$vf1z\n"
        "vmaddw.xyzw  $vf6,$vf27,$vf1w\n"
        "vmulax.xyzw  ACC,$vf28,$vf1x\n"
        "vmadday.xyzw ACC,$vf29,$vf1y\n"
        "vmaddaz.xyzw ACC,$vf30,$vf1z\n"
        "vmaddw.xyzw  $vf7,$vf31,$vf1w\n"
        "vmulax.xyzw  ACC,$vf4,$vf2x\n"
        "vmadday.xyzw ACC,$vf5,$vf2y\n"
        "vmaddaz.xyzw ACC,$vf6,$vf2z\n"
        "vmaddw.xyzw  $vf1,$vf7,$vf2w\n"
    : "=r" (vo) : "r" (v), "r" (w));
}

void hr_pcam_spdacc(PCAMSC *pcc) {
    sceVu0AddVector(pcc->spd, pcc->spd, pcc->acc);
    sceVu0AddVector(pcc->p, pcc->p, pcc->spd);
}

void hr_pcam_spdclr(PCAMSC *pcc) {
    sceVu0SubVector(pcc->spd, pcc->spd, pcc->spd);
    sceVu0CopyVector(pcc->acc, pcc->spd);
    pcc->flag &= -2;
}

void hr_pcam_set_mode(s32 mode) {
    hrpcam->flag = mode;
    switch (mode) {
        case PCAMW_PAI:
            hrpcam->mi.flag &= -14;
            break;
        case PCAMW_IAP:
            hrpcam->mp.flag &= -14;
            break;
    }
}

void hr_pcam_set_beta(s32 *addr) {
    hrpcam->flag = -1;
    hrpcam->bmax = *addr++;
    hrpcam->bptr = (f32 *)addr;
    hrpcam->bnow = 0;
}

static void hr_pcam_matrix(PCAMSM *pcm, sceVu0FMATRIX m) {
    sceVu0UnitMatrix(m);
    sceVu0RotMatrix(m, m, pcm->rr);
    if (pcm->flag & 2) {
        if (pcm->flag & 0xC) {
            sceVu0CopyMatrix(pcm->hm, *pcm->hdmk);
        }
        sceVu0MulMatrix(m, pcm->hm, m);
    } else {
        if (pcm->flag & 0xC) {
            sceVu0CopyVectorXYZ(pcm->hv, *pcm->hdv);
        }
        sceVu0TransMatrix(m, m, pcm->hv);
    }
    pcm->flag &= -9;
}

static void hr_pcam_ang(PCAMSC *pcc, sceVu0FVECTOR v) {
    hr_pcam_spdacc(pcc);
    hr_pall_rot180V(pcc->p);
    sceVu0ScaleVectorXYZ(v, pcc->p, M_PIf);
    sceVu0DivVectorXYZ(v, v, 180.0f);
}

static void hr_pcam_posinte(PCAMSC *pcc, PCAMSM *pcm, sceVu0FVECTOR v) {
    sceVu0FMATRIX m;

    if (!(pcm->flag & 1) && !(pcc->flag & 1) && !(pcm->r.flag & 1)) {
        return;
    }

    hr_pcam_spdacc(pcc);
    if (!(pcm->flag & 0x10)) {
        hr_pcam_ang(&pcm->r, pcm->rr);
        hr_pcam_matrix(pcm, m);
        sceVu0ApplyMatrix(pcm->v, m, pcc->p);
        pcm->v[3] = 1.0f;
        sceVu0CopyVector(v, pcm->v);
    } else if (pcm->flag & 0x60) {
        if (pcm->flag & 0xC) {
            sceVu0CopyVectorXYZ(pcm->hv, *pcm->hdv);
        }
        sceVu0CopyVector(v, pcm->hv);
        if (pcm->flag & 0x40) {
            sceVu0AddVector(v, v, pcc->p);
            v[3] = 1.0f;
        }
    } else {
        sceVu0CopyVector(v, pcc->p);
    }

    if (!(pcm->flag & 4)) {
        pcm->flag &= -2;
    }
}

void hr_pcam_ang2wa() {
    sceVu0FVECTOR v;

    sceVu0ScaleVectorXYZ(v, hrpcam->ang, 180.0f);
    sceVu0DivVectorXYZ(v, v, M_PIf);
    hrpcam->wa.p[0] = v[0];
    hrpcam->wa.p[1] = v[1];
    hrpcam->wa.p[2] = v[2];
}

void hr_pcam_pi2len() {
    sceVu0FVECTOR v;

    sceVu0SubVector(v, hrpcam->pos, hrpcam->inte);
    sceVu0MulVector(v, v, v);
    hrpcam->len.p = sqrtf(v[0] + v[1] + v[2]);
    if (hrpcam->len.p < 1.0f) {
        hrpcam->len.p = 1.0f;
    }
}

void hr_pcam_anglen() {
    hr_pcam_ang2wa();
    hr_pcam_pi2len();
}

void hr_pcam_set_ang(f32 x, f32 y, f32 z, s32 fg) {
    PCAMSC *pcc;

    pcc = &hrpcam->wa;
    hr_pcam_cinit(pcc);
    if (fg != 0) {
        hrpcam->ang[0] = x;
        hrpcam->ang[1] = y;
        hrpcam->ang[2] = z;
        hrpcam->ang[3] = 1.0f;
        hr_pcam_ang2wa();
    } else {
        pcc->p[0] = x;
        pcc->p[1] = y;
        pcc->p[2] = z;
        pcc->p[3] = 1.0f;
        hr_pcam_ang(pcc, hrpcam->ang);
    }
}

void hr_pcam_gyakusan(sceVu0FVECTOR *pi, PCAMSC *pcc, PCAMSM *pcm) {
    sceVu0FMATRIX m;
    sceVu0FVECTOR v;

    if (pcm->flag & 0x10) {
        return;
    }

    if (pcm->flag & 2) {
        sceVu0CopyVector(v, pcm->hm[3]);
    } else {
        sceVu0CopyVector(v, pcm->hv);
    }
    sceVu0SubVector(v, *pi, v);
    v[3] = 1.0f;
    hr_pcam_matrix(pcm, m);
    sceVu0TransposeMatrix(m, m);
    sceVu0ApplyMatrix(v, m, v);
    sceVu0CopyVectorXYZ(pcc->p, v);
}

static void hr_pcam_len(PCAMSL *pcl) {
    pcl->spd += pcl->acc;
    pcl->p += pcl->spd;
    if (pcl->p < 1.0f) {
        pcl->p = 1.0f;
    }
}

static void hr_pcam_beta_frm() {
    f32 *f;

    f = hrpcam->bptr + hrpcam->bnow * 6;
    hrpcam->pos[0] = *f++;
    hrpcam->pos[1] = *f++;
    hrpcam->pos[2] = *f++;
    hrpcam->inte[0] = *f++;
    hrpcam->inte[1] = *f++;
    hrpcam->inte[2] = *f++;
    hrpcam->pos[1] = -hrpcam->pos[1];
    hrpcam->pos[2] = -hrpcam->pos[2];
    hrpcam->inte[1] = -hrpcam->inte[1];
    hrpcam->inte[2] = -hrpcam->inte[2];
    if (hrpcam->bnow + 1 < hrpcam->bmax) {
        hrpcam->bnow++;
    }
}

void hr_pcam_calc() {
    switch (hrpcam->flag) {
        case PCAMW_PIA:
            hr_pcam_ang(&hrpcam->wa, hrpcam->ang);
            hr_pcam_posinte(&hrpcam->wp, &hrpcam->mp, hrpcam->pos);
            hr_pcam_posinte(&hrpcam->wi, &hrpcam->mi, hrpcam->inte);
            hr_pcam_pi2rot(hrpcam->pos, hrpcam->inte, hrpcam->ang);
            hr_pcam_anglen();
            break;
        case PCAMW_IPA:
            hr_pcam_ang(&hrpcam->wa, hrpcam->ang);
            hr_pcam_posinte(&hrpcam->wi, &hrpcam->mi, hrpcam->inte);
            hr_pcam_posinte(&hrpcam->wp, &hrpcam->mp, hrpcam->pos);
            hr_pcam_pi2rot(hrpcam->pos, hrpcam->inte, hrpcam->ang);
            hr_pcam_anglen();
            break;
        case PCAMW_PAI:
            hr_pcam_posinte(&hrpcam->wp, &hrpcam->mp, hrpcam->pos);
            hr_pcam_ang(&hrpcam->wa, hrpcam->ang);
            hr_pcam_len(&hrpcam->len);
            hr_pcam_pr2inte(hrpcam->pos, hrpcam->ang, hrpcam->inte, hrpcam->len.p);
            hr_pcam_gyakusan(&hrpcam->inte, &hrpcam->wi, &hrpcam->mi);
            break;
        case PCAMW_IAP:
            hr_pcam_posinte(&hrpcam->wi, &hrpcam->mi, hrpcam->inte);
            hr_pcam_ang(&hrpcam->wa, hrpcam->ang);
            hr_pcam_len(&hrpcam->len);
            hr_pcam_pr2inte(hrpcam->inte, hrpcam->ang, hrpcam->pos, -hrpcam->len.p);
            hr_pcam_gyakusan(&hrpcam->pos, &hrpcam->wp, &hrpcam->mp);
            break;
        default:
            hr_pcam_beta_frm();
            hr_pcam_pi2rot(hrpcam->pos, hrpcam->inte, hrpcam->ang);
            hr_pcam_anglen();
            break;
    }
    hr_pcam_len(&hrpcam->proj);
}

s32 hr_pcam_tmv_end(s32 no) {
    PCAMSC *pcc;

    switch (no){
        case HRTMV_WP:
            pcc = &hrpcam->wp;
            break;
        case HRTMV_WI:
            pcc = &hrpcam->wi;
            break;
        case HRTMV_WA:
            pcc = &hrpcam->wa;
            break;
        case HRTMV_LRP:
            pcc = &hrpcam->mp.r;
            break;
        case HRTMV_LRI:
            pcc = &hrpcam->mi.r;
            break;
        default:
            return 0;
    }

    if (pcc->time != 0) {
        return 0;
    } else {
        return 1;
    }
}

static void hr_pcam_comm_tmv(PCAMSC *pcc, f32 x, f32 y, f32 z, s32 time, u8 c0, u8 c1) {
    pcc->spd[0] = x;
    pcc->spd[1] = y;
    pcc->spd[2] = z;
    sceVu0AddVector(pcc->moku, pcc->p, pcc->spd);
    pcc->moku[3] = 1.0f;
    comm_p128b_ft(pcc, time);
    comm_jigen_spd(pcc->spd, pcc->acc, time, c0, c1);
}

static void hr_pcam_comm_tmvp(/* s0 16 */ PCAMSC *pcc, /* f12 50 */ f32 x, /* f13 51 */ f32 y, /* f14 52 */ f32 z, /* s2 18 */ s32 time, /* a2 6 */ u8 c0, /* a3 7 */ u8 c1) {
    pcc->moku[0] = x;
    pcc->moku[1] = y;
    pcc->moku[2] = z;
    sceVu0AddVector(pcc->spd, pcc->moku, pcc->p);
    pcc->spd[3] = 0.0f;
    comm_p128b_ft(pcc, time);
    comm_jigen_spd(pcc->spd, pcc->acc, time, c0, c1);
}

void hr_pcam_pos_tmv(s32 id, f32 x, f32 y, f32 z, s32 time, u8 c0, u8 c1) {
    PCAMSC *pcc;

    pcc = &hrpcam->wp;
    if (id != 0) {
        hr_pcam_comm_tmv(pcc, x, y, z, time, c0, c1);
    } else {
        hr_pcam_comm_tmvp(pcc, x, y, z, time, c0, c1);
    }
}

void hr_pcam_inte_tmv(s32 id, f32 x, f32 y, f32 z, s32 time, u8 c0, u8 c1) {
    PCAMSC *pcc;

    pcc = &hrpcam->wi;
    if (id != 0) {
        hr_pcam_comm_tmv(pcc, x, y, z, time, c0, c1);
    } else {
        hr_pcam_comm_tmvp(pcc, x, y, z, time, c0, c1);
    }
}

void hr_pcam_ang_tmv(s32 id, f32 x, f32 y, f32 z, s32 time, u8 c0, u8 c1) {
    PCAMSC *pcc;

    pcc = &hrpcam->wa;
    if (id != 0) {
        hr_pcam_comm_tmv(pcc, x, y, z, time, c0, c1);
    } else {
        hr_pcam_comm_tmvp(pcc, x, y, z, time, c0, c1);
    }
    hr_pall_rot180V(pcc->moku);
}

void hr_pcam_work_mae() {
    hr_check_time_move(&hrpcam->wp);
    hr_check_time_move(&hrpcam->wi);
    hr_check_time_move(&hrpcam->wa);
    hr_check_time_move(&hrpcam->mp.r);
    hr_check_time_move(&hrpcam->mi.r);
    hr_check_time_moveL(&hrpcam->len);
    hr_check_time_moveL(&hrpcam->proj);
}

void hr_pcam_work() {
    CAM_WORK *cam;

    hr_pcam_calc();
    cam = &GameGbl.cam;
    if (hrpt_flag != 4 || ppsys.flag & 0x2000) {
        sceVu0CopyVector(cam->posi, hrpcam->pos);
        sceVu0CopyVector(cam->ang, hrpcam->ang);
        cam->scr_z = hrpcam->proj.p;
    }
}
