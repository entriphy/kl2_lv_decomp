#include "harada/hr_mirr.h"
#include "harada/hr_main.h"
#include "taka/tk_effect.h"
#include "nakano/main.h"

s32 hfmircnt = 0;
s32 hcmircnt = 0;
s32 hrmirflush = 0;
s32 hroldflush = 0;
HFMIR hfmirbuf[4] = {};
HCMIR hcmirbuf[1] = {};
HFMIRC hfmcam = {};
u8 *hfm_addr = NULL;
u8 *hcm_addr = NULL;
u32 hfm_size = 0;
u32 hcm_size = 0;
f32 *hfm_scale = NULL;
f32 *hcm_scale = NULL;
V1100MIR *hrm1100 = NULL;
s32 D_3FA510 = 0;

static void hr_mir_setf() {
    HFMIR *fmir;
    K2M_HEADF *head;
    u32 *d;
    u32 cnt;
    s32 i;
    sceVu0FVECTOR *data;

    d = (u32 *)hfm_addr;
    if (*d++ != 0x46)
        return;

    hfmircnt = *d;
    if (hfmircnt > 4) {
        hfmircnt = 4;
    }
    cnt = *d++;

    if (*d == 1) {
        d += 2;
        hfm_scale = (float *)d;
        d += cnt * 2;
    } else {
        d += 2;
    }
    head = (K2M_HEADF *)d;

    for (i = 0, fmir = hfmirbuf; i < hfmircnt; i++, fmir++) {
        data = (sceVu0FVECTOR *)((u32)hfm_addr + (u32)head->addr);
        fmir->count = head->count;
        fmir->rx = head->rx;
        fmir->ry = head->ry;
        fmir->clen = head->len;
        fmir->norm = &data[0];
        fmir->center = &data[1];
        fmir->vert = &data[2];
        head++;
    }
}

static void hr_mir_setc() {
    HCMIR *cmir;
    K2M_HEADC *head;
    u32 *d;
    u32 cnt;
    s32 i;
    sceVu0FVECTOR *data;

    d = (u32 *)hcm_addr;
    if (*d++ != 0x43)
        return;

    hcmircnt = *d;
    if (hcmircnt > 1) {
        hcmircnt = 1;
    }
    cnt = *d++;

    if (*d == 1) {
        d += 2;
        hcm_scale = (f32 *)d;
        d += cnt * 2;
    } else {
        d += 2;
    }
    head = (K2M_HEADC *)d;

    for (i = 0, cmir = hcmirbuf; i < hcmircnt; i++, cmir++) {
        data = (sceVu0FVECTOR *)((u32)hcm_addr + head->addr);
        cmir->count = head->count;
        cmir->poly = head->poly;
        cmir->hc2 = (K2M_HC2 *)((u32)hcm_addr + head->addr2);
        cmir->norm = data;
        cmir->center = &cmir->norm[cmir->poly];
        cmir->vert = &cmir->center[cmir->poly];
        cmir->dst = (s16 *)&cmir->vert[cmir->count];
        head++;
    }
}

void hr_mir_set() {
    hfmircnt = 0;
    hcmircnt = 0;
    hfmcam.mir = NULL;
    hfmcam.type = 0;
    hfmcam.flag = 0;
    hfmcam.scalex = hfmcam.scaley = 1.0f;
    hfmcam.vsmFg = 0;
    hfmcam.vsmSx = hfmcam.vsmSy = 1.0f;
    hfmcam.posmode = 0;
    hfmcam.mpos[0] = hfmcam.mpos[1] = hfmcam.mpos[2] = 0.0f;
    hfmcam.mpos[3] = 1.0f;
    sceVu0CopyVector(hfmcam.mang, hfmcam.mpos);
    if (hfm_addr != NULL) {
        hr_mir_setf();
    }
    if (hcm_addr != NULL) {
        hr_mir_setc();
    }
    if (hfmircnt != 0 || hcmircnt != 0) {
        hrmirflush = 1;
    } else {
        hrmirflush = 0;
    }
    hroldflush = 0;
    if ((s32)hrm1100 != -1) {
        hr_set_vpmfog(&hrm1100->infoA);
        if (hrm1100->flag & 2) {
            hr_set_vpmfog(&hrm1100->infoB);
        }
    }
    if (hfmircnt != 0 && GameGbl.vision != 0x0B11) {
        hfmcam.flag |= 2;
        D_3FA510 = (rand() & 0x3FF) + 120;
    }
}

s32 hr_check_mir() {
    if ((hfmircnt == 0 && hcmircnt == 0) || hfmcam.type == 0) {
        return 0;
    } else {
        return (hfmcam.flag & 1) ^ 1;
    }
}

f32 hr_rot_pai(f32 r) {
    s32 i;
    f32 r0;

    r0 = r;
    i = 0;

    if (r > M_PIf) {
        do {
            i++;
            if (!(M_PIf < r0)) {
                break;
            }
            r0 -= M_TWOPIf;
        } while (i < 5);
    } else if (r0 < -M_PIf) {
        do {
            i++;
            if (!(r0 < -M_PIf)) {
                break;
            }
            r0 += M_TWOPIf;
        } while (i < 5);
    }

    return r0;
}

static f32 hr_atan2f(f32 d0, f32 d1) {
    if (d1 != 0.0f) {
        return atan2f(d0, d1);
    } else {
        if (d0 >= 0.0f) {
            return M_PI_2f;
        } else {
            return -M_PI_2f;
        }
    }
}

s32 hr_mir_cmpx(const void *t0, const void *t1) {
    QWIV *v0;
    QWIV *v1;

    v0 = (QWIV *)t0;
    v1 = (QWIV *)t1;
    if (v0->x == v1->x) {
        if (v0->z > v1->z) {
            return 1;
        } else {
            return -1;
        }
    } else {
        if (v0->x > v1->x) {
            return 1;
        } else {
            return -1;
        }
    }
}

s32 hr_mir_cmpy(const void *t0, const void *t1) {
    QWIV *v0;
    QWIV *v1;

    v0 = (QWIV *)t0;
    v1 = (QWIV *)t1;
    if (v0->y == v1->y) {
        if (v0->z > v1->z) {
            return 1;
        } else {
            return -1;
        }
    } else {
        if (v0->y > v1->y) {
            return 1;
        } else {
            return -1;
        }
    }
}

s32 hr_mir_cmpz(const void *t0, const void *t1) {
    QWIV *v0;
    QWIV *v1;

    v0 = (QWIV *)t0;
    v1 = (QWIV *)t1;
    if (v0->z == v1->z) {
        return 0;
    } else {
        if (v0->z > v1->z) {
            return 1;
        } else {
            return -1;
        }
    }
}

s32 hr_mir_cmpxf(QWFV *v0, QWFV *v1) {
    if (v0->x == v1->x) {
        if (v0->z < v1->z) {
            return 1;
        } else {
            return -1;
        }
    } else {
        if (v0->x > v1->x) {
            return 1;
        } else {
            return -1;
        }
    }
}

s32 hr_mir_cmpyf(QWFV *v0, QWFV *v1) {
    if (v0->y == v1->y) {
        if (v0->z < v1->z) {
            return 1;
        } else {
            return -1;
        }
    } else {
        if (v0->y > v1->y) {
            return 1;
        } else {
            return -1;
        }
    }
}

s32 hr_mir_cmpzf(QWFV *v0, QWFV *v1) {
    if (v0->z == v1->z) {
        return 0;
    } else {
        if (v0->z > v1->z) {
            return 1;
        } else {
            return -1;
        }
    }
}

static s32 hr_mir_rtpc(sceVu0IVECTOR *iv) {
    if ((*iv)[2] >= 0x10000U || (*iv)[0] + 0x10000 > 0x30000U) {
        return 1;
    } else {
        return (*iv)[1] + 0x10000 > 0x30000U;
    }
}

static s32 hr_fmir_rtp(HFMIR *fmir, sceVu0IVECTOR *vert, u32 *sz) {
    sceVu0FVECTOR *v;
    sceVu0IVECTOR iv;
    s32 i;

    for (i = 0, v = fmir->vert; i < fmir->count; i++, v++) {
        sceVu0RotTransPers(*vert, GameGbl.d_wsm, *v, 1);
        if (hr_mir_rtpc(*vert)) {
            return 1;
        }
        vert++;
    }

    sceVu0RotTransPers(iv, GameGbl.d_wsm, *fmir->center, 1);
    fmir->cx = iv[0] / 16.0f;
    fmir->cy = iv[1] / 16.0f;
    *sz = iv[2];
    return 0;
}

u32 hr_mirsys_ver(u8 *addr) {
    return ((u32 *)addr)[2];
}

f32* hr_fmirver1_scale(HFMIR *fmir) {
    u32 size;

    size = ((u32)fmir - (u32)hfmirbuf) / sizeof(HFMIR);
    return hfm_scale + size * 2;
}

f32* hr_cmirver1_scale(HCMIR *cmir) {
    u32 size;

    size = ((u32)cmir - (u32)hcmirbuf) / sizeof(HCMIR);
    return hcm_scale + size * 2;
}

static void hr_mir_cameraC(f32 *d, CAM_WORK *cam,  sceVu0FVECTOR *center, f64 mrx, f64 mry) {
    sceVu0FMATRIX m;
    sceVu0FVECTOR v;
    f32 rx;
    f32 ry;
    f32 *f;

    f32 f_mrx; // ?
    f32 f_mry; // ?

    f_mrx = (f32)mrx;
    f_mry = (f32)mry;

    if (f_mrx == 0.0f) {
        ry = (f_mry - hr_atan2f(d[0], d[2])) * 2 + M_PIf;
        sceVu0UnitMatrix(m);
        sceVu0RotMatrixY(m, m, hr_rot_pai(ry));
        sceVu0TransMatrix(m, m, *center);
        sceVu0ApplyMatrix(hfmcam.mpos, m, d);
        hfmcam.mpos[3] = 1.0f;
    } else {
        sceVu0UnitMatrix(m);
        sceVu0RotMatrixY(m, m, -f_mry);
        sceVu0ApplyMatrix(d, m, d);
        d[2] = -d[2];
        d[3] = 1.0f;
        sceVu0UnitMatrix(m);
        sceVu0RotMatrixY(m, m, f_mry);
        sceVu0RotMatrixX(m, m, f_mrx * 2);
        sceVu0TransMatrix(m, m, *center);
        sceVu0ApplyMatrix(hfmcam.mpos, m, d);
        hfmcam.mpos[3] = 1.0f;
    }

    sceVu0UnitMatrix(m);
    sceVu0UnitMatrix(hfmcam.wvm);
    if (f_mrx == 0.0f) {
        ry = f_mry * 2 + M_PIf + cam->ang[1];
        rx = hr_rot_pai(cam->ang[0]);
        ry = hr_rot_pai(ry);
        hfmcam.mang[0] = rx;
        hfmcam.mang[1] = -ry;
        sceVu0RotMatrixY(m, m, hfmcam.mang[1]);
        sceVu0RotMatrixX(m, m, rx);
    } else {
        sceVu0RotMatrixX(m, m, -f_mrx * 2);
        sceVu0RotMatrixY(m, m, (M_PIf - f_mry) * 2);
        sceVu0RotMatrixY(m, m, hr_rot_pai(M_PIf - cam->ang[1]));
        sceVu0RotMatrixX(m, m, cam->ang[0]);
        hfmcam.mang[0] = hr_rot_pai(cam->ang[0] - (f_mrx * 2));
        hfmcam.mang[1] = hr_rot_pai(((M_PIf - f_mry) * 2 + M_PIf) - cam->ang[1]);
    }

    if (cam->ang[2] != 0.0f) {
        sceVu0RotMatrixZ(m, m, -cam->ang[2]);
        hfmcam.mang[2] = -cam->ang[2];
    } else {
        hfmcam.mang[2] = 0.0f;
    }
    sceVu0MulMatrix(m, hfmcam.wvm, m);
    v[0] = -hfmcam.mpos[0];
    v[1] = -hfmcam.mpos[1];
    v[2] = -hfmcam.mpos[2];
    sceVu0ApplyMatrix(v, m, v);
    v[3] = 1.0f;
    sceVu0TransMatrix(hfmcam.wvm, m, v);
    sceVu0UnitMatrix(m);
    m[0][0] = -m[0][0];

    if (hfmcam.type == 0x43) {
        if (hr_mirsys_ver(hcm_addr) == 1) {
            f = hr_cmirver1_scale(hfmcam.mir);
            hfmcam.scalex = f[0];
            hfmcam.scaley = f[1];
        }
    } else {
        if (hr_mirsys_ver(hfm_addr) == 1) {
            f = hr_fmirver1_scale(hfmcam.mir);
            hfmcam.scalex = f[0];
            hfmcam.scaley = f[1];
        }
    }

    sceVu0MulMatrix(hfmcam.vsm, GameGbl.d_vsm, m);
    sceVu0MulMatrix(hfmcam.wsm, hfmcam.vsm, hfmcam.wvm);
}

static void hr_mir_clipVPM(sceVu0FVECTOR *vs, sceVu0FVECTOR *cent, u32 count) {
    sceVu0FVECTOR v;
    sceVu0FVECTOR *vert;
    VPCLIP *vpc;
    s32 i;
    f32 hz;
    sceVu0FMATRIX m;
    sceVu0FMATRIX m2;
    sceVu0FVECTOR maxx;
    sceVu0FVECTOR minx;
    sceVu0FVECTOR maxy;
    sceVu0FVECTOR miny;
    sceVu0FVECTOR minz;
    sceVu0FVECTOR maxz;
    s32 fg;

    v[0] = (*cent)[0] - hfmcam.mpos[0];
    v[1] = (*cent)[1] - hfmcam.mpos[1];
    v[2] = (*cent)[2] - hfmcam.mpos[2];
    v[3] = 1.0f;

    fg = 0;
    hz = sqrtf(v[0] * v[0] + v[2] * v[2]);
    sceVu0UnitMatrix(m);
    sceVu0UnitMatrix(m2);
    sceVu0RotMatrixY(m, m, -hr_atan2f(v[0], v[2]));
    sceVu0RotMatrixX(m, m, hr_atan2f(v[1], hz));
    sceVu0MulMatrix(m, m2, m);
    v[0] = -hfmcam.mpos[0];
    v[1] = -hfmcam.mpos[1];
    v[2] = -hfmcam.mpos[2];
    sceVu0ApplyMatrix(v, m, v);
    v[3] = 1.0f;
    sceVu0TransMatrix(m, m, v);

    for (i = 0, vert = vs; i < count; i++, vert++) {
        sceVu0ApplyMatrix(v, m, *vert);
        if (!(v[2] <= 0.0f)) {
            if (!fg) {
                sceVu0CopyVector(maxx, v);
                sceVu0CopyVector(minx, v);
                sceVu0CopyVector(maxy, v);
                sceVu0CopyVector(miny, v);
                sceVu0CopyVector(minz, v);
                sceVu0CopyVector(maxz, v);
                fg = 1;
            } else {
                if (minx[0] > v[0]) {
                    sceVu0CopyVector(minx, v);
                } else if (maxx[0] < v[0]) {
                    sceVu0CopyVector(maxx, v);
                }

                if (miny[1] > v[1]) {
                    sceVu0CopyVector(miny, v);
                } else if (maxy[1] < v[1]) {
                    sceVu0CopyVector(maxy, v);
                }

                if (minz[2] > v[2]) {
                    sceVu0CopyVector(minz, v);
                } else if (maxz[2] < v[2]) {
                    sceVu0CopyVector(maxz, v);
                }
            }
        }
    }

    vpc = &hfmcam.vc;
    hz = 1773.62f;
    vpc->cmax[0] = maxx[0] / maxx[2];
    vpc->cmin[0] = minx[0] / minx[2];
    vpc->cmax[1] = maxy[1] / maxy[2];
    vpc->cmin[1] = miny[1] / miny[2];
    vpc->cmax[2] = vpc->cmax[3] = 0.0f;
    vpc->cmin[2] = vpc->cmin[3] = 0.0f;
    vpc->hmax[0] = hz;
    vpc->hmax[1] = hz;
    vpc->hmax[2] = maxz[2] + hz;
    vpc->hmax[3] = 0.0f;
    vpc->hmin[0] = -vpc->hmax[0];
    vpc->hmin[1] = -vpc->hmax[1];
    vpc->hmin[2] = minz[2] - hz;
    vpc->hmin[3] = 0.0f;

    hz = 512.0f;
    hfmcam.vco.cmax[0] = vpc->cmax[0];
    hfmcam.vco.cmin[0] = vpc->cmin[0];
    hfmcam.vco.cmax[1] = vpc->cmax[1];
    hfmcam.vco.cmin[1] = vpc->cmin[1];
    hfmcam.vco.cmax[3] = 0.0f;
    hfmcam.vco.cmax[2] = 0.0f;
    hfmcam.vco.cmin[3] = 0.0f;
    hfmcam.vco.cmin[2] = 0.0f;
    hfmcam.vco.hmax[0] = hz;
    hfmcam.vco.hmax[1] = hz;
    hfmcam.vco.hmax[2] = maxz[2] + hz;
    hfmcam.vco.hmax[3] = 0.0f;
    hfmcam.vco.hmin[0] = -hfmcam.vco.hmax[0];
    hfmcam.vco.hmin[1] = -hfmcam.vco.hmax[1];
    hfmcam.vco.hmin[2] = minz[2] - hz;
    hfmcam.vco.hmin[3] = 0.0f;
}

static void hr_mir_screenclipC(sceVu0IVECTOR *vert, u32 count, f64 cx, f64 cy) {
    QWIV *fv;
    u32 a;

    f32 f_cx; // ?
    f32 f_cy; // ?

    f_cx = (f32)cx;
    f_cy = (f32)cy;

    qsort(vert, count, 16, hr_mir_cmpx);
    fv = (QWIV *)vert;
    hfmcam.smin[0] = fv->x / 16.0f;
    if (fv->x >= 0x9400) {
        hfmcam.type = 0;
        return;
    }

    fv += count - 1;
    hfmcam.smax[0] = fv->x / 16.0f;

    if (fv->x <= 0x6C00) {
        hfmcam.type = 0;
        return;
    }

    qsort(vert, count, 16, hr_mir_cmpy);
    fv = (QWIV *)vert;
    hfmcam.smin[1] = fv->y / 16.0f;

    if (fv->y >= 0x8700) {
        hfmcam.type = 0;
        return;
    }

    fv += count - 1;
    hfmcam.smax[1] = fv->y / 16.0f;

    if (fv->y <= 0x7900) {
        hfmcam.type = 0;
        return;
    }

    qsort(vert, count, 16, hr_mir_cmpz);
    fv = (QWIV *)vert + count;
    hfmcam.smax[2] = (fv - 1)->z;
    hfmcam.smin[2] = 1.0f;

    hfmcam.smax[0] += (hfmcam.smax[0] - f_cx) * 2;
    if (hfmcam.smax[0] > 4096.0f) {
        hfmcam.smax[0] = 4096.0f;
    }

    hfmcam.smax[1] += (hfmcam.smax[1] - f_cy) * 2;
    if (hfmcam.smax[1] > 4096.0f) {
        hfmcam.smax[1] = 4096.0f;
    }

    hfmcam.smax[2] += 512.0f;
    if (hfmcam.smax[2] > 16777215.0f) {
        hfmcam.smax[2] = 16777215.0f;
    }

    hfmcam.smax[3] = 512.0f;
    hfmcam.smin[0] += (hfmcam.smin[0] - f_cx) * 2;
    if (hfmcam.smin[0] < 0.0f) {
        hfmcam.smin[0] = 0.0f;
    }

    hfmcam.smin[1] += (hfmcam.smin[1] - f_cy) * 2;
    if (hfmcam.smin[1] < 0.0f) {
        hfmcam.smin[1] = 0.0f;
    }

    a = 0x380;
    hfmcam.smin[3] = *(f32 *)&a;
}

static u32 hr_fmir_workM(sceVu0IVECTOR *vert, CAM_WORK *cam, u32 maxsz) {
    HFMIR *fmir;
    HFMIR *wkfm;
    s32 i;
    u32 sz;

    wkfm = NULL;
    for (i = 0, fmir = hfmirbuf; i < hfmircnt; i++, fmir++) {
        h_vp_init(&cvpo, cam->scr_z, 1.0f, 65536.0f, fmir->clen, fmir->clen, 0.0f, 1);
        if (h_vpo_vclip(&cvpo, *fmir->center, GameGbl.wvm) == 0) {
            if (!(sceVu0InnerProduct(hfmcam.vn, *fmir->norm) <= 0.0f)) {
                if (hr_fmir_rtp(fmir, vert, &sz) == 0 && sz > maxsz) {
                    hfmcam.count = i;
                    maxsz = sz;
                    wkfm = fmir;
                }
            }
        }
    }

    if (maxsz != 0) {
        hfmcam.mir = wkfm;
        hfmcam.type = 0x46;
    }

    return maxsz;
}

static void hr_fmir_workD(sceVu0IVECTOR *vert, CAM_WORK *cam) {
    HFMIR *wkfm;
    sceVu0FVECTOR d;
    sceVu0FVECTOR *v;
    s32 i;

    wkfm = hfmcam.mir;
    d[0] = cam->posi[0] - (*wkfm->center)[0];
    d[1] = cam->posi[1] - (*wkfm->center)[1];
    d[2] = cam->posi[2] - (*wkfm->center)[2];
    d[3] = 1.0f;

    hr_mir_cameraC(d, cam, wkfm->center, wkfm->rx, wkfm->ry);
    hr_mir_clipVPM(wkfm->vert, wkfm->center, wkfm->count);

    for (i = 0, v = wkfm->vert; i < wkfm->count; i++, v++) {
        sceVu0RotTransPers(vert[i], GameGbl.d_wsm, *v, 1);
    }

    hr_mir_screenclipC(vert, wkfm->count, wkfm->cx, wkfm->cy);
    if (hfmcam.flag & 2) {
        D_3FA510--;
        if (D_3FA510 == 0 && wkfm->count == 4) {
            D_3FA510 = (rand() & 0x3FF) + 480;
            for (i = 0, v = wkfm->vert; i < 4; i++, v++) {
                sceVu0CopyVector(EffectParamObj.mirrorPos[i], *v);
            }
            TkAddEffect(16);
        }
    }
}

static s32 hr_cmir_rtp(HCMIR *cmir, u32 *sz) {
    sceVu0FVECTOR *v;
    sceVu0IVECTOR iv;
    s32 i;

    for (i = 0, v = cmir->vert; i < cmir->count; i++, v++) {
        sceVu0RotTransPers(iv, GameGbl.d_wsm, *v, 1);
        if (hr_mir_rtpc(&iv)) {
            return 1;
        }
    }

    sceVu0RotTransPers(iv, GameGbl.d_wsm, cmir->center[cmir->men], 1);
    cmir->cx = iv[0] / 16.0f;
    cmir->cy = iv[1] / 16.0f;
    *sz = iv[2];
    return 0;
}

static void hr_cmir_hokan(HCMIR *cmir, CAM_WORK *cam, f32 maxinn) {
    K2M_HC2 *hc2;
    K2M_HC2 *hc;
    sceVu0FVECTOR *c;
    sceVu0FVECTOR *vert;
    sceVu0FVECTOR v;
    sceVu0IVECTOR iv;
    u8 fg;
    f32 r;
    f32 dr;
    f32 dr2;
    s32 i;

    hc2 = &cmir->hc2[cmir->men];
    c = &cmir->center[cmir->men];
    hfmcam.cent[0] = (*c)[0];
    hfmcam.cent[1] = (*c)[1];
    hfmcam.cent[2] = (*c)[2];
    hfmcam.cent[3] = 1.0f;
    hfmcam.dlt[0] = hfmcam.dlt[1] = hfmcam.dlt[2] = 0.0f;
    hfmcam.dlt[3] = 1.0f;
    hfmcam.rx = hfmcam.ry = 0.0f;

    if (maxinn >= 0.099989998f) {
        return;
    }

    hc = cmir->hc2;
    fg = 0x59;
    dr = hc[0].ry - hc[1].ry;
    dr2 = hc[0].rx - hc[1].rx;
    if (fabsf(dr) > fabsf(dr2)) {
        r = 3.141592f - cam->ang[1];
        dr = hr_rot_pai(r - hc2->ry);
        if (cmir->men != 0) {
            dr2 = hr_rot_pai(hc2[-1].ry - hc2->ry);
            i = -1;
        } else {
            dr2 = hr_rot_pai(hc2[1].ry - hc2->ry);
            i = 1;
        }
        if (dr * dr2 < 0.0f) {
            i = -i;
        }

        if (cmir->men == 0 && i == -1) {
            return;
        }
        if (cmir->men == cmir->poly - 1 && i == 1) {
            return;
        }

        dr2 = hr_rot_pai(hc2[i].ry - hc2->ry) * 0.5f;
    } else {
        fg = 0x58;
        r = -cam->ang[0];
        dr = hr_rot_pai(r - hc2->rx);
        if (cmir->men != 0) {
            dr2 = hr_rot_pai(hc2[-1].rx - hc2->rx);
            i = -1;
        } else {
            dr2 = hr_rot_pai(hc2[1].rx - hc2->rx);
            i = 1;
        }
        if (dr * dr2 < 0.0f) {
            i = -i;
        }

        if (cmir->men == 0 && i == -1) {
            return;
        }
        if (cmir->men == cmir->poly - 1 && i == 1) {
            return;
        }

        dr2 = hr_rot_pai(hc2[i].rx - hc2->rx) * 0.5f;
    }

    if (fabsf(dr) > fabsf(dr2)) {
        dr = dr2;
    }

    if (fg == 0x59) {
        hfmcam.ry = dr;
    } else {
        hfmcam.rx = dr;
    }

    vert = i == 1 ? cmir->vert + cmir->men * 2 + 2 : cmir->vert + cmir->men * 2;

    v[0] = (vert[0][0] + vert[1][0]) * 0.5f;
    v[1] = (vert[0][1] + vert[1][1]) * 0.5f;
    v[2] = (vert[0][2] + vert[1][2]) * 0.5f;
    r = dr / dr2;
    hfmcam.dlt[0] = (v[0] - (*c)[0]) * r;
    hfmcam.dlt[1] = (v[1] - (*c)[1]) * r;
    hfmcam.dlt[2] = (v[2] - (*c)[2]) * r;
    hfmcam.cent[0] = hfmcam.dlt[0] + (*c)[0];
    hfmcam.cent[1] = hfmcam.dlt[1] + (*c)[1];
    hfmcam.cent[2] = hfmcam.dlt[2] + (*c)[2];
    sceVu0RotTransPers(iv, GameGbl.d_wsm, hfmcam.cent, 1);
    cmir->cx = iv[0] / 16.0f;
    cmir->cy = iv[1] / 16.0f;
}

static void hr_cmir_workM(CAM_WORK *cam, u32 maxsz) {
    HCMIR *cmir;
    K2M_HC2 *hc2;
    sceVu0FVECTOR *n;
    sceVu0FVECTOR *c;
    s32 i;
    s32 j;
    u32 sz;
    f32 inn;
    f32 maxinn;

    maxinn = 0.0f;
    for (i = 0, cmir = hcmirbuf; i < hcmircnt; i++, cmir++) {
        n = cmir->norm;
        c = cmir->center;
        maxinn = 0.0f;
        hc2 = cmir->hc2;
        for (j = 0; j < cmir->poly; j++, hc2++, n++, c++) {
            h_vp_init(&cvpo, cam->scr_z, 1.0f, 65536.0f, hc2->len, hc2->len, 0.0f, 1);
            if (h_vpo_vclip(&cvpo, *c, GameGbl.wvm) == 0) {
                inn = sceVu0InnerProduct(hfmcam.vn, *n);
                if (!(inn <= 0.0f) && inn > maxinn) {
                    maxinn = inn;
                    cmir->men = j;
                }
            }
        }

        if (maxinn != 0.0f && hr_cmir_rtp(cmir, &sz) == 0 && maxsz < sz) {
            hfmcam.count = i;
            maxsz = sz;
            hfmcam.mir = cmir;
            hfmcam.type = 0x43;
        }
    }

    if (hfmcam.type == 0x43) {
        hr_cmir_hokan(hfmcam.mir, cam, maxinn);
    }
}

void hr_make_dum(sceVu0FVECTOR *cent, s16 *dst, u32 men, sceVu0FVECTOR *v, u32 count, s32 d) {
    sceVu0FMATRIX m;
    u32 i;

    sceVu0UnitMatrix(m);
    if (hfmcam.rx != 0.0f) {
        sceVu0RotMatrixX(m, m, hfmcam.rx);
    }
    if (hfmcam.ry != 0.0f) {
        sceVu0RotMatrixY(m, m, hfmcam.ry);
    }

    dst = (s16 *)((u32)dst + (count * (men * 2 + men)) * 2); // aight bro
    for (i = 0; i < count; i++, v += d) {
        (*v)[0] = -hfmcam.dlt[0] + *dst++ / 16.0f;
        (*v)[1] = -hfmcam.dlt[1] + *dst++ / 16.0f;
        (*v)[2] = -hfmcam.dlt[2] + *dst++ / 16.0f;
        (*v)[3] = 1.0f;
        sceVu0ApplyMatrix(*v, m, *v);
        (*v)[0] += (*cent)[0];
        (*v)[1] += (*cent)[1];
        (*v)[2] += (*cent)[2];
        (*v)[3] = 1.0f;
    }
}

static void hr_cmir_workD(CAM_WORK *cam) {
    HCMIR *wkcm;
    sceVu0FVECTOR d;
    sceVu0FVECTOR *v;
    sceVu0IVECTOR *iv;
    K2M_HC2 *hc2;
    s32 i;

    wkcm = hfmcam.mir;
    d[0] = cam->posi[0] - hfmcam.cent[0];
    d[1] = cam->posi[1] - hfmcam.cent[1];
    d[2] = cam->posi[2] - hfmcam.cent[2];
    d[3] = 1.0f;

    hc2 = &wkcm->hc2[wkcm->men];
    hr_mir_cameraC(d, cam, &hfmcam.cent, hr_rot_pai(hc2->rx + hfmcam.rx), hr_rot_pai(hc2->ry + hfmcam.ry));
    hr_make_dum(&hfmcam.cent, wkcm->dst, wkcm->men, (sceVu0FVECTOR *)0x70000000, wkcm->count, 1);
    hr_mir_clipVPM((sceVu0FVECTOR *)0x70000000, &hfmcam.cent, wkcm->count);

    iv = (sceVu0IVECTOR *)0x70000000;
    v = wkcm->vert;

    for (i = 0; i < wkcm->count; i++, v++, iv++) {
        sceVu0RotTransPers(*iv, GameGbl.d_wsm, *v, 1);
    }

    hr_mir_screenclipC((sceVu0IVECTOR *)0x70000000, wkcm->count, wkcm->cx, wkcm->cy);
}

void hr_mir_work() {
    sceVu0IVECTOR vert[8];
    CAM_WORK *cam;
    u32 maxsz;
    sceVu0FMATRIX m;

    hfmcam.mir = NULL;
    hfmcam.type = 0;
    maxsz = 0;
    cam = &GameGbl.cam;
    hfmcam.vn[0] = hfmcam.vn[1] = 0.0f;
    hfmcam.vn[2] = -1.0f;
    hfmcam.vn[3] = 1.0f;
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixX(m, m, -cam->ang[0]);
    sceVu0RotMatrixY(m, m, -cam->ang[1]);
    sceVu0ApplyMatrix(hfmcam.vn, m, hfmcam.vn);
    hfmcam.vn[3] = 1.0f;
    sceVu0Normalize(hfmcam.vn, hfmcam.vn);
    hfmcam.count = 0;
    if (!(hfmcam.flag & 1)) {
        if (hfmircnt != 0) {
            maxsz = hr_fmir_workM(vert, cam, 0);
        }
        if (hcmircnt != 0) {
            hr_cmir_workM(cam, maxsz);
        }
        if (hfmcam.type == 0x46) {
            hr_fmir_workD(vert, cam);
        } else if (hfmcam.type == 0x43) {
            hr_cmir_workD(cam);
        }
    }
}

s32 func_0010DB60(s32 vision) {
    switch (vision) {
        case 0x0B04:
            return 3;
        case 0x0B01:
        case 0x0B02:
        case 0x0B06:
            return 1;
        default:
            return 0;
    }
}

vpmINFO* func_0010DBC0() {
    if ((s32)hrm1100 != -1) {
        if (func_0010DB60(GameGbl.vision) == 1) {
            return &hrm1100->infoA;
        }

        if (GameGbl.vision == 0x0B04) {
            if (hfmcam.type == 0x46) {
                return &hrm1100->infoA;
            } else {
                return &hrm1100->infoB;
            }
        } else {
            return &VpmInfo;
        }
    } else {
        return &VpmInfo;
    }
}