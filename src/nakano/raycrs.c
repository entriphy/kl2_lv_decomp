#include "nakano/raycrs.h"
#include "nakano/main.h"
#include "nakano/vu0mac.h"
#include "nakano/tmddraw.h"
#include "harada/hr_main.h"

static HITMAPINFO HitMapInfo;
static u32 *HitBlocksList[32];

s32 GdCheck(f32 rd, f32 v0, f32 bmin, f32 bmax) {
    f32 t0, t1;
    sceVu0FVECTOR v;

    if (rd == 0.0f) {
        if (v0 < bmin || v0 > bmax) {
            return 0;
        }
    } else {
        v[0] = bmin - v0;
        v[1] = bmax - v0;
        sceVu0DivVector(v, v, rd);
        t0 = v[0];
        t1 = v[1];

        if (t0 > t1) {
            f32 work = t0;
            t0 = t1;
            t1 = work;
        }
        if (t0 < bmin) {
            bmin = t0;
        }
        if (t1 > bmax) {
            bmax = t1;
        }

        if (bmin > bmax || bmax < 0.0f) {
            return 0;
        }
    }

    return 1;
}

s32 GetHitBlocks(sceVu0FVECTOR vf0, sceVu0FVECTOR vf1) {
    sceVu0FVECTOR ray;
    sceVu0FVECTOR fw0;
    sceVu0FVECTOR fw1;
    sceVu0FVECTOR vmin;
    sceVu0FVECTOR vmax;
    sceVu0FVECTOR stb; // start block
    sceVu0FVECTOR edb; // end block
    sceVu0FVECTOR grid;
    sceVu0IVECTOR vi;
    sceVu0FVECTOR vf;
    HITMAPINFO *hmi;
    f32 lpx;
    f32 lpy;
    f32 lpz;
    s32 cnt;
    s32 ret;
    u32 *hpadr;

    hmi = &HitMapInfo;

    sceVu0SubVector(ray, vf1, vf0);
    fw0[0] = vf0[0] - hmi->stx;
    fw0[1] = vf0[1] - hmi->sty;
    fw0[2] = vf0[2] - hmi->stz;
    fw1[0] = vf1[0] - hmi->stx;
    fw1[1] = vf1[1] - hmi->sty;
    fw1[2] = vf1[2] - hmi->stz;

    if (fw0[0] > fw1[0]) {
        vmax[0] = fw0[0];
        vmin[0] = fw1[0];
    } else {
        vmax[0] = fw1[0];
        vmin[0] = fw0[0];
    }

    if (fw0[1] > fw1[1]) {
        vmax[1] = fw0[1];
        vmin[1] = fw1[1];
    } else {
        vmax[1] = fw1[1];
        vmin[1] = fw0[1];
    }

    if (fw0[2] > fw1[2]) {
        vmax[2] = fw0[2];
        vmin[2] = fw1[2];
    } else {
        vmax[2] = fw1[2];
        vmin[2] = fw0[2];
    }

    grid[0] = hmi->gdw;
    grid[1] = hmi->gdh;

    stb[0] = vmin[0] / grid[0];
    stb[1] = vmin[1] / grid[1];
    stb[2] = vmin[2] / grid[0];
    sceVu0FTOI0Vector(vi, stb);
    vi[0] *= hmi->gdw;
    vi[1] *= hmi->gdh;
    vi[2] *= hmi->gdw;
    sceVu0ITOF0Vector(stb, vi);

    edb[0] = vmax[0] / grid[0];
    edb[1] = vmax[1] / grid[1];
    edb[2] = vmax[2] / grid[0];
    sceVu0FTOI0Vector(vi, edb);
    vi[0] *= hmi->gdw;
    vi[1] *= hmi->gdh;
    vi[2] *= hmi->gdw;
    sceVu0ITOF0Vector(edb, vi);

    cnt = 0;

    if (stb[0] < 0.0)
        stb[0] = 0.0f;
    if (stb[1] < 0.0)
        stb[1] = 0.0f;
    if (stb[2] < 0.0)
        stb[2] = 0.0f;

    vf[0] = hmi->gdw * hmi->gdx;
    vf[1] = hmi->gdh * hmi->gdy;
    vf[2] = hmi->gdw * hmi->gdz;
    if (edb[0] >= vf[0])
        edb[0] = vf[0] - grid[0];
    if (edb[1] >= vf[1])
        edb[1] = vf[1] - grid[1];
    if (edb[2] >= vf[2])
        edb[2] = vf[2] - grid[0];

    if (hmi->gdw == 0 || hmi->gdh == 0)
        return 0;

    // wtf
    {
        s32 idk0 = (s32)stb[0] / hmi->gdw;
        s32 idk1 = (s32)stb[1] / hmi->gdh;
        s32 idk2 = (s32)stb[2] / hmi->gdw;
    }

    for (lpz = stb[2]; lpz <= edb[2]; lpz += grid[0]) {
        for (lpy = stb[1]; lpy <= edb[1]; lpy += grid[1]) {
            for (lpx = stb[0]; lpx <= edb[0]; lpx += grid[0]) {
                ret = GdCheck(ray[0], fw0[0], lpx, lpx + grid[0]) & GdCheck(ray[1], fw0[1], lpy, lpy + grid[1]);
                ret &= GdCheck(ray[2], fw0[2], lpz, lpz + grid[0]);
                if (ret != 0) {
                    hpadr = hr_hpmk(
                        lpx + grid[0] / 2.0f + hmi->stx,
                        lpy + grid[1] / 2.0f + hmi->sty,
                        lpz + grid[0] / 2.0f + hmi->stz,
                        hmi->prim_top
                    );
                    if (hpadr != NULL) {
                        HitBlocksList[cnt] = hpadr;
                        cnt++;
                    }
                }
            }
        }
    }

    return cnt;
}

f32 CalcNClip(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2) {
    sceVu0FVECTOR va;
    sceVu0FVECTOR vb;
    sceVu0FVECTOR nor;

    sceVu0SubVector(va, v1, v0);
    sceVu0SubVector(vb, v2, v1);
    sceVu0OuterProduct(nor, va, vb);

    return nor[2];
}

void NorMat(sceVu0FMATRIX mat, sceVu0FVECTOR vf) {
    sceVu0FMATRIX m;
    f32 s;
    f32 c;
    f32 ret;

    sceVu0UnitMatrix(mat);
    ret = vu0_SquareRoot(vf[0] * vf[0] + vf[2] * vf[2]);
    s = vf[1];
    sceVu0UnitMatrix(m);
    m[1][1] = ret;
    m[2][2] = ret;
    m[2][1] = -s;
    m[1][2] = s;
    sceVu0MulMatrix(mat, mat, m);
    if (ret != 0.0f) {
        s = -vf[0] / ret;
        c = vf[2] / ret;
        sceVu0UnitMatrix(m);
        m[0][0] = c;
        m[2][2] = c;
        m[2][0] = s;
        m[0][2] = -s;
        sceVu0MulMatrix(mat, mat, m);
    }
}

void vu0_RotTransPers2(sceVu0IVECTOR v0, sceVu0FMATRIX m0, sceVu0FVECTOR v1) {
    __asm__ volatile(
        "lqc2        $vf4, 0x0(%1)\n"
        "lqc2        $vf5, 0x10(%1)\n"
        "lqc2        $vf6, 0x20(%1)\n"
        "lqc2        $vf7, 0x30(%1)\n"
        "lqc2        $vf8, 0x0(%2)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vdiv        Q, $vf0w, $vf12w\n"
        "vwaitq\n"
        "vmulq.xyz   $vf12, $vf12, Q\n"
        "vftoi4.xyz  $vf13, $vf12\n"
        "vftoi0.zw   $vf13, $vf12\n"
        "sqc2        $vf13, 0x0(%0)\n"
    : : "r" (v0), "r" (m0), "r" (v1));
}

void nkMonKloHit(sceVu0FVECTOR posi) {
    // Empty function
}

void DrawBlockGrid() {
    static qword_uni ph_packet_b[32];
    qword_uni *ph_pb;
    static sceDmaTag tag[3];
    sceDmaTag *tp;
    f32 vx;
    f32 vz;
    HITMAPINFO *hmi;
    s32 lpx;
    s32 lpz;
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;

    hmi = &HitMapInfo;

    for (lpz = 0, vz = hmi->stz; lpz < hmi->gdz; lpz++, vz += 512.0f) {
        for (lpx = 0, vx = hmi->stx; lpx < hmi->gdx; lpx++, vx += 512.0f) {
            ph_pb = ph_packet_b;
            tp = tag;

            // NLOOP=4, EOP=1, PRE=1, PRIM={PRIM=4, IIP=1, TME=0, FGE=0, ABE=0, AA1=1, FST=0, CTXT=0, FIX=0}, FLG=0, NREG=2(0x01: SCE_GS_RGBAQ, 0x05: SCE_GS_XYZ2)
            ph_pb->u_u64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 0, 1, 0, 0, 0), SCE_GIF_PACKED, 2);
            ph_pb->u_u64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 0x4;
            ph_pb++;

            ph_pb->u_u32[0] = 0;
            ph_pb->u_u32[1] = 0xFF;
            ph_pb->u_u32[2] = 0;
            ph_pb->u_u32[3] = 0x40;
            ph_pb++;

            vf[0] = vx;
            vf[1] = 0.0f;
            vf[2] = vz;
            vf[3] = 1.0f;
            vu0_RotTransPers2(vi, GameGbl.wsm, vf);
            qword_Copy(ph_pb, vi);
            ph_pb++;

            ph_pb->u_u32[0] = 0;
            ph_pb->u_u32[1] = 0xFF;
            ph_pb->u_u32[2] = 0xFF;
            ph_pb->u_u32[3] = 0x40;
            ph_pb++;

            vf[0] = vx + 512.0f;
            vf[1] = 0.0f;
            vf[2] = vz;
            vf[3] = 1.0f;
            vu0_RotTransPers2(vi, GameGbl.wsm, vf);
            qword_Copy(ph_pb, vi);
            ph_pb++;

            ph_pb->u_u32[0] = 0xFF;
            ph_pb->u_u32[1] = 0;
            ph_pb->u_u32[2] = 0xFF;
            ph_pb->u_u32[3] = 0x40;
            ph_pb++;

            vf[0] = vx;
            vf[1] = 0.0f;
            vf[2] = vz + 512.0f;
            vf[3] = 1.0f;
            vu0_RotTransPers2(vi, GameGbl.wsm, vf);
            qword_Copy(ph_pb, vi);
            ph_pb++;

            ph_pb->u_u32[0] = 0;
            ph_pb->u_u32[1] = 0;
            ph_pb->u_u32[2] = 0xFF;
            ph_pb->u_u32[3] = 0x40;
            ph_pb++;

            vf[0] = vx + 512.0f;
            vf[1] = 0.0f;
            vf[2] = vz + 512.0f;
            vf[3] = 1.0f;
            vu0_RotTransPers2(vi, GameGbl.wsm, vf);
            qword_Copy(ph_pb, vi);

            sceDmaAddRef(&tp, 9, ph_packet_b);
            sceDmaAddEnd(&tp, 0, NULL);

            FlushCache(0);
            sceDmaSync(DmaChGIF, 0, 0);
            tp = tag;
            sceDmaSend(DmaChGIF, tp);
            sceGsSyncPath(0, 0);
        }
    }
}

void DrawHitBlock(sceVu0IVECTOR posi) {
    sceVu0IVECTOR vi[8];
    sceVu0FVECTOR vf[8];
    HITMAPINFO *hmi = &HitMapInfo;
    static qword ph_packet_b[1024];
    qword *ph_pb;
    static sceDmaTag tag[3];
    sceDmaTag *tp = tag;
    s32 qwc;
    s32 lp0;
    u64 giftag[2] = { SCE_GIF_SET_TAG(24, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_LINE, 0, 0, 0, 0, 0, 0, 0, 0), 0, 2), SCE_GS_RGBAQ | SCE_GS_XYZ2 << 4 };

    SETVEC(vi[0], posi[0] * hmi->gdw,        posi[1] * hmi->gdh,       posi[2] * hmi->gdw,       1);
    SETVEC(vi[1], (posi[0] + 1) * hmi->gdw,  posi[1] * hmi->gdh,       posi[2] * hmi->gdw,       1);
    SETVEC(vi[2], posi[0] * hmi->gdw,        posi[1] * hmi->gdh,       (posi[2] + 1) * hmi->gdw, 1);
    SETVEC(vi[3], (posi[0] + 1) * hmi->gdw,  posi[1] * hmi->gdh,       (posi[2] + 1) * hmi->gdw, 1);
    SETVEC(vi[4], posi[0] * hmi->gdw,        (posi[1] + 1) * hmi->gdh, posi[2] * hmi->gdw,       1);
    SETVEC(vi[5], (posi[0] + 1) * hmi->gdw,  (posi[1] + 1) * hmi->gdh, posi[2] * hmi->gdw,       1);
    SETVEC(vi[6], posi[0] * hmi->gdw,        (posi[1] + 1) * hmi->gdh, (posi[2] + 1) * hmi->gdw, 1);
    SETVEC(vi[7], (posi[0] + 1) * hmi->gdw,  (posi[1] + 1) * hmi->gdh, (posi[2] + 1) * hmi->gdw, 1);

    for (lp0 = 0; lp0 < 8; lp0++) {
        vu0_ITOF0Vector(vf[lp0], vi[lp0]);
        vf[lp0][0] += hmi->stx;
        vf[lp0][1] += hmi->sty;
        vf[lp0][2] += hmi->stz;
        sceVu0RotTransPers(vi[lp0], GameGbl.wsm, vf[lp0], 1);
    }

    ph_pb = ph_packet_b;
    qword_Copy(ph_pb, giftag);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[0]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[1]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[1]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[3]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[3]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[2]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[2]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[0]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[4]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[5]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[5]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[7]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[7]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[6]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[6]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[4]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[0]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[4]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[1]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[5]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[2]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[6]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[3]);
    ph_pb++;
    SETVEC((*ph_pb), 0x00, 0xFF, 0xFF, 0x40);
    ph_pb++;
    qword_Copy(ph_pb, vi[7]);
    ph_pb++;

    qwc = 49;
    sceDmaAddRef(&tp, qwc, ph_packet_b);
    sceDmaAddEnd(&tp, 0, NULL);

    FlushCache(0);
    sceDmaSync(DmaChGIF, 0, 0);
    sceDmaSend(DmaChGIF, tag);
    sceGsSyncPath(0, 0);
}

void DrawHitPol(qword *prim) {
    static qword_uni ph_packet_b[1024];
    static sceDmaTag tag[3];

    s32 *p_pd;
    sceVu0FVECTOR trans;
    s32 nloop;
    s32 lp0;
    s16 *vp;
    sceVu0FVECTOR vf;
    sceVu0FVECTOR vf0;
    sceVu0FVECTOR vf1;
    sceVu0IVECTOR vi;
    qword_uni *ph_pb;
    sceDmaTag *tp;
    s32 vflg;
    s32 qwc;

    p_pd = (s32 *)(*prim)[1];
    p_pd++;
    ((s32 *)trans)[0] = *p_pd++;
    ((s32 *)trans)[1] = *p_pd++;
    ((s32 *)trans)[2] = *p_pd++;
    trans[3] = 1.0f;

    vflg = 0;
    if (((*p_pd >> 24) & 0xFF) != 0x69) // SCE_VIF_UNPACK | SCE_VIF_UPK_V3_16
        return;

    while (1) {
        nloop = (*p_pd >> 16) & 0xFF;
        ph_pb = ph_packet_b;
        tp = tag;

        ph_pb->u_u64[0] = SCE_GIF_SET_TAG(nloop, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 1, 1, 0, 0, 0), SCE_GIF_PACKED, 2);
        ph_pb->u_u64[1] = SCE_GS_RGBAQ | SCE_GS_XYZF2 << 0x4;
        ph_pb++;
        qwc = 1;

        vp = (s16 *)p_pd + 2;
        for (lp0 = 0; lp0 < nloop; lp0++) {
            vi[0] = *vp++;
            vi[1] = *vp++;
            vi[2] = *vp++;
            vu0_ITOF4Vector(vf, vi);

            ph_pb->u_u32[0] = 0;
            ph_pb->u_u32[1] = 0;
            ph_pb->u_u32[2] = 0xFF;
            ph_pb->u_u32[3] = 0x40;
            ph_pb++;

            vf[0] += trans[0];
            vf[1] += trans[1];
            vf[2] += trans[2];
            vu0_LoadMtx(GameGbl.wsm);
            vu0_Thing(vf, vf);
            vu0_FTOI4Vector(vi, vf);

            vflg <<= 1;
            if (VertClip(vf) != 0) {
                vflg |= 1;
            }

            if ((vflg & 7) == 0)
                vi[3] = 1;
            else
                vi[3] = 0x8000;
            vi[2] += 0x1000;

            if (lp0 > 1) {
                if ((lp0 & 1) == 0) {
                    if (CalcNClip(vf0, vf1, vf) < 0.0f)
                        vi[3] = 0x8000;
                } else {
                    if (CalcNClip(vf0, vf1, vf) > 0.0f)
                        vi[3] = 0x8000;
                }
            }

            qword_Copy(ph_pb, vi);
            ph_pb++;
            qwc += 2;

            qword_Copy(vf0, vf1);
            qword_Copy(vf1, vf);
        }

        if ((nloop & 1) != 0)
            vp++;

        sceDmaAddRef(&tp, qwc, ph_packet_b);
        sceDmaAddEnd(&tp, 0, NULL);

        FlushCache(0);
        sceDmaSync(DmaChGIF, 0, 0);
        sceDmaSend(DmaChGIF, tag);
        sceGsSyncPath(0, 0);

        p_pd = (s32 *)vp;
        p_pd += 5;
        if (((s8 *)p_pd)[3] != 0x69)
            break;
    }
}

f32 ChkHitPol(sceVu0FVECTOR vec, sceVu0FVECTOR muki, u32 *prim) {
    s32 *p_pd;
    sceVu0FVECTOR trans;
    s32 nloop;
    s32 lp0;
    s16 *vp;
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;
    sceVu0FVECTOR pos;
    f32 dist;
    f32 near = 65535.0f;
    sceVu0FMATRIX mat;
    sceVu0FVECTOR v[3];
    sceVu0FVECTOR va;
    sceVu0FVECTOR vb;
    sceVu0FVECTOR nor;
    HITMAPINFO *hmi = &HitMapInfo;

    sceVu0FTOI0Vector(vi, vec);
    sceVu0CopyVector(pos, vec);

    if (*prim == 0) {
        return near;
    }

    sceVu0Normalize(vf, muki);
    p_pd = (s32 *)(prim[1] + (u32)hmi->prim_top);
    p_pd++;
    NorMat(mat, vf);
    sceVu0ApplyMatrix(pos, mat, pos);
    ((s32 *)trans)[0] = *p_pd++;
    ((s32 *)trans)[1] = *p_pd++;
    ((s32 *)trans)[2] = *p_pd++;
    trans[3] = 1.0f;

    while (1) {
        s32 buf = *p_pd;
        if (((buf >> 24) & 0xFF) != 0x69) {
            break;
        }
        nloop = (buf >> 16) & 0xFF;
        p_pd++;
        vp = (s16 *)p_pd;

        for (lp0 = 0; lp0 < 2; lp0++) {
            vi[0] = *vp++;
            vi[1] = *vp++;
            vi[2] = *vp++;
            vu0_ITOF4Vector(vf, vi);
            vf[3] = 1.0f;
            vf[0] += trans[0];
            vf[1] += trans[1];
            vf[2] += trans[2];
            sceVu0ApplyMatrix(v[lp0], mat, vf);
        }

        for (lp0 = 2; lp0 < nloop; sceVu0CopyVector(v[0], v[1]), sceVu0CopyVector(v[1], v[2]), lp0++) {
            vi[0] = *vp++;
            vi[1] = *vp++;
            vi[2] = *vp++;
            vu0_ITOF4Vector(vf, vi);
            vf[3] = 1.0f;
            vf[0] += trans[0];
            vf[1] += trans[1];
            vf[2] += trans[2];
            sceVu0ApplyMatrix(v[2], mat, vf);
            sceVu0SubVector(va, v[1], v[0]);
            sceVu0SubVector(vb, v[2], v[1]);
            sceVu0OuterProduct(nor, va, vb);
            sceVu0Normalize(nor, nor);

            if (lp0 & 1) {
                nor[0] = -nor[0];
                nor[1] = -nor[1];
                nor[2] = -nor[2];
            }
            if (nor[2] <= 0.0f) {
                continue;
            }

            dist = (pos[0] - v[0][0]) * nor[0] + (pos[1] - v[0][1]) * nor[1] + (pos[2] - v[0][2]) * nor[2];
            dist = -dist / nor[2];

            if (!(lp0 & 1)) {
                if (CalcNClip(v[0], v[1], pos) < 0.0f || CalcNClip(v[1], pos, v[2]) > 0.0f || CalcNClip(pos, v[2], v[0]) < 0.0f) {
                    continue;
                }
            } else {
                if (CalcNClip(v[0], v[1], pos) > 0.0f || CalcNClip(v[1], pos, v[2]) < 0.0f || CalcNClip(pos, v[2], v[0]) > 0.0f) {
                    continue;
                }
            }

            if (dist >= 0.0f && dist < near) {
                near = dist;
            }
        }

        if (nloop & 1) {
            vp++;
        }

        p_pd = (s32 *)vp;
        p_pd += 5;

        if (*p_pd >> 24 != 0x69) {
            break;
        }
    }

    return near;
}

// TODO: not matching
f32 ChkHitPol2(sceVu0FVECTOR vec, sceVu0FMATRIX mat, u32 *prim, u32 *hitcolor, sceVu0FVECTOR rnor) {
    s32 *p_pd;
    sceVu0FVECTOR trans;
    s32 nloop;
    s32 lp0;
    s16 *vp;
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;
    sceVu0FVECTOR pos;
    f32 dist;
    f32 near = 65535.0f;
    sceVu0FVECTOR v[3];
    sceVu0FVECTOR va;
    sceVu0FVECTOR vb;
    sceVu0FVECTOR nor;
    u32 hit_flag;
    u32 col_idx = 0;
    s32 phidx;
    sceVu0FVECTOR VecB[3];
    sceVu0FVECTOR VecR[3];
    HITMAPINFO *hmi = &HitMapInfo;

    sceVu0FTOI0Vector(vi, vec);
    sceVu0CopyVector(pos, vec);

    if (*prim == 0) {
        return near;
    }

    p_pd = (s32 *)(prim[1] + (u32)hmi->prim_top);
    p_pd++;
    sceVu0ApplyMatrix(pos, mat, pos);
    ((s32 *)trans)[0] = *p_pd++;
    ((s32 *)trans)[1] = *p_pd++;
    ((s32 *)trans)[2] = *p_pd++;
    trans[3] = 1.0f;

    while (1) {
        s32 buf = *p_pd;
        if (((buf >> 24) & 0xFF) != 0x69) {
            break;
        }
        nloop = (buf >> 16) & 0xFF;
        p_pd++;
        vp = (s16 *)p_pd;
        hit_flag = 0;

        for (lp0 = 0; lp0 < 2; lp0++) {
            vi[0] = *vp++;
            vi[1] = *vp++;
            vi[2] = *vp++;
            vu0_ITOF4Vector(vf, vi);
            vf[3] = 1.0f;
            vf[0] += trans[0];
            vf[1] += trans[1];
            vf[2] += trans[2];
            sceVu0CopyVector(VecB[lp0], vf);
            sceVu0ApplyMatrix(v[lp0], mat, vf);
        }

        for (lp0 = 2; lp0 < nloop; sceVu0CopyVector(v[0], v[1]), sceVu0CopyVector(v[1], v[2]), sceVu0CopyVector(VecB[0], VecB[1]), sceVu0CopyVector(VecB[1], VecB[2]), lp0++) {
            vi[0] = *vp++;
            vi[1] = *vp++;
            vi[2] = *vp++;
            vu0_ITOF4Vector(vf, vi);
            vf[3] = 1.0f;
            vf[0] += trans[0];
            vf[1] += trans[1];
            vf[2] += trans[2];
            sceVu0CopyVector(VecB[2], vf);
            sceVu0ApplyMatrix(v[2], mat, vf);
            sceVu0SubVector(va, v[1], v[0]);
            sceVu0SubVector(vb, v[2], v[1]);
            sceVu0OuterProduct(nor, va, vb);
            sceVu0Normalize(nor, nor);

            if (lp0 & 1) {
                nor[0] = -nor[0];
                nor[1] = -nor[1];
                nor[2] = -nor[2];
            }
            if (nor[2] <= 0.0f) {
                continue;
            }

            dist = (pos[0] - v[0][0]) * nor[0] + (pos[1] - v[0][1]) * nor[1] + (pos[2] - v[0][2]) * nor[2];
            dist = -dist / nor[2];

            if (!(lp0 & 1)) {
                if (CalcNClip(v[0], v[1], pos) < 0.0f || CalcNClip(v[1], pos, v[2]) > 0.0f || CalcNClip(pos, v[2], v[0]) < 0.0f) {
                    continue;
                }
            } else {
                if (CalcNClip(v[0], v[1], pos) > 0.0f || CalcNClip(v[1], pos, v[2]) < 0.0f || CalcNClip(pos, v[2], v[0]) > 0.0f) {
                    continue;
                }
            }

            if (dist >= 0.0f && dist < near) {
                near = dist;
                hit_flag = 1;
                phidx = lp0;
                sceVu0CopyVector(VecR[0], VecB[0]);
                sceVu0CopyVector(VecR[1], VecB[1]);
                sceVu0CopyVector(VecR[2], VecB[2]);
            }
        }

        if (nloop & 1) {
            vp++;
        }

        p_pd = (s32 *)vp;
        p_pd += 2;
        if (hit_flag) {
            col_idx = *p_pd;
        }
        p_pd += 3;

        if (*p_pd >> 24 != 0x69) {
            break;
        }
    }

    if (near != 65535.0f) {
        HITMAPINFO *hmi = &HitMapInfo;
        *hitcolor = *(u32 *)((u32)hmi->prim_top + col_idx);
        sceVu0SubVector(va, VecR[1], VecR[0]);
        sceVu0SubVector(vb, VecR[2], VecR[1]);
        sceVu0OuterProduct(nor, va, vb);
        sceVu0Normalize(rnor, nor);
        if (phidx & 1) {
            rnor[0] = -rnor[0];
            rnor[1] = -rnor[1];
            rnor[2] = -rnor[2];
        }
    }

    return near;
}

f32 RayCrsJudgz(sceVu0FVECTOR vec, sceVu0FVECTOR muki) {
    return 65535.0f;
}

f32 RayCrsJudgzN(sceVu0FVECTOR vec, sceVu0FVECTOR muki) {
    sceVu0FVECTOR pos1;
    sceVu0FVECTOR vf;
    s32 lp0;
    f32 near;
    f32 dist;
    s32 n_prim;

    near = 65535.0f;
    sceVu0Normalize(vf, muki);
    pos1[0] = vf[0] * 32.0f + vec[0];
    pos1[1] = vf[1] * 32.0f + vec[1];
    pos1[2] = vf[2] * 32.0f + vec[2];

    n_prim = GetHitBlocks(vec, pos1);
    for (lp0 = 0; lp0 < n_prim; lp0++) {
        dist = ChkHitPol(vec, muki, HitBlocksList[lp0]);
        if (dist < near) {
            near = dist;
        }
    }

    return near;
}

f32 RayCrsJudgzNL(sceVu0FVECTOR vec0, sceVu0FVECTOR vec1) {
    sceVu0FVECTOR muki;
    sceVu0FVECTOR vf;
    s32 lp0;
    f32 near;
    f32 dist;
    f32 len;
    s32 n_prim;
    sceVu0FMATRIX mat;
    u32 hitcolor;
    sceVu0FVECTOR nor;

    near = 65535.0f;
    n_prim = GetHitBlocks(vec0, vec1);
    sceVu0SubVector(muki, vec1, vec0);
    sceVu0MulVector(vf, muki, muki);
    vu0_Square(vf, muki);
    len = vu0_SquareRoot(vf[0] + vf[1] + vf[2]);
    sceVu0Normalize(vf, muki);
    NorMat(mat, vf);
    for (lp0 = 0; lp0 < n_prim; lp0++) {
        dist = ChkHitPol2(vec0, mat, HitBlocksList[lp0], &hitcolor, nor);
        if (near > dist) {
            near = dist;
        }
    }

    if (near > len) {
        near = 65535.0f;
    }

    return near;
}

f32 RayCrsJudgzNLcol(sceVu0FVECTOR vec0, sceVu0FVECTOR vec1, u32 *hitcolor, sceVu0FVECTOR rnor) {
    sceVu0FVECTOR muki;
    sceVu0FVECTOR vf;
    s32 lp0;
    f32 near = 65535.0f;
    f32 dist;
    f32 len;
    s32 n_prim;
    sceVu0FMATRIX mat;
    sceVu0FVECTOR nor;
    u32 hitcol = 0;

    n_prim = GetHitBlocks(vec0, vec1);
    sceVu0SubVector(muki, vec1, vec0);
    sceVu0MulVector(vf, muki, muki);
    vu0_Square(vf, muki);
    len = vu0_SquareRoot(vf[0] + vf[1] + vf[2]);
    sceVu0Normalize(vf, muki);
    NorMat(mat, vf);
    for (lp0 = 0; lp0 < n_prim; lp0++) {
        dist = ChkHitPol2(vec0, mat, HitBlocksList[lp0], &hitcol, nor);
        if (near > dist) {
            near = dist;
            sceVu0CopyVector(rnor, nor);
            *hitcolor = hitcol;

        }
    }

    if (near > len)
        near = 65535.0f;;
    return near;
}

f32 sbChkHitPol(sceVu0FVECTOR rnor, sceVu0FVECTOR vec, sceVu0FVECTOR muki, u32 *prim) {
    s32 *p_pd;
    sceVu0FVECTOR trans;
    s32 nloop;
    s32 lp0;
    s16 *vp;
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;
    sceVu0FVECTOR pos;
    f32 dist;
    f32 near = 65535.0f;
    sceVu0FMATRIX mat;
    sceVu0FVECTOR v[3];
    sceVu0FVECTOR va;
    sceVu0FVECTOR vb;
    sceVu0FVECTOR nor;
    s32 phidx;
    sceVu0FVECTOR VecB[3];
    sceVu0FVECTOR VecR[3];
    HITMAPINFO *hmi = &HitMapInfo;

    sceVu0FTOI0Vector(vi, vec);
    sceVu0CopyVector(pos, vec);
    if (*prim == 0)
        return near;

    p_pd = (s32 *)(prim[1] + (u32)hmi->prim_top);
    p_pd++;
    sceVu0Normalize(vf, muki);
    NorMat(mat, vf);
    sceVu0ApplyMatrix(pos, mat, pos);

    ((s32 *)trans)[0] = *p_pd++;
    ((s32 *)trans)[1] = *p_pd++;
    ((s32 *)trans)[2] = *p_pd++;
    trans[3] = 1.0f;

    while (1) {
        s32 read = *p_pd;
        if ((read >> 0x18 & 0xFF) != 0x69) {
            break;
        }
        nloop = read >> 0x10 & 0xFF;
        vp = (s16 *)p_pd + 2;
        for (lp0 = 0; lp0 < 2; lp0++) {
            vi[0] = *vp++;
            vi[1] = *vp++;
            vi[2] = *vp++;
            vu0_ITOF4Vector(vf, vi);
            vf[3] = 1.0f;
            vf[0] += trans[0];
            vf[1] += trans[1];
            vf[2] += trans[2];
            sceVu0CopyVector(VecB[lp0], vf);
            sceVu0ApplyMatrix(v[lp0], mat, vf);
        }

        for (lp0 = 2; lp0 < nloop; lp0++) {
            vi[0] = *vp++;
            vi[1] = *vp++;
            vi[2] = *vp++;
            vu0_ITOF4Vector(vf, vi);
            vf[3] = 1.0f;
            vf[0] += trans[0];
            vf[1] += trans[1];
            vf[2] += trans[2];
            sceVu0CopyVector(VecB[2], vf);
            sceVu0ApplyMatrix(v[2], mat, vf);
            sceVu0SubVector(va, v[1], v[0]);
            sceVu0SubVector(vb, v[2], v[1]);
            sceVu0OuterProduct(nor, va, vb);
            sceVu0Normalize(nor, nor);

            if ((lp0 & 1) != 0) {
                nor[0] = -nor[0];
                nor[1] = -nor[1];
                nor[2] = -nor[2];
            }

            if (nor[2] <= 0.0f)
                goto LAB_00195800;

            dist = -((pos[0] - v[0][0]) * nor[0] + (pos[1] - v[0][1]) * nor[1] + (pos[2] - v[0][2]) * nor[2]) / nor[2];
            if ((lp0 & 1) == 0) {
                if (CalcNClip(v[0], v[1], pos) < 0.0f || CalcNClip(v[1], pos, v[2]) > 0.0f || CalcNClip(pos, v[2], v[0]) < 0.0f)
                    goto LAB_00195800;
            } else {
                if (CalcNClip(v[0], v[1], pos) > 0.0f || CalcNClip(v[1], pos, v[2]) < 0.0f || CalcNClip(pos, v[2], v[0]) > 0.0f)
                    goto LAB_00195800;
            }

            if (dist >= 0.0f && dist < near) {
                near = dist;
                phidx = lp0;
                sceVu0CopyVector(VecR[0], VecB[0]);
                sceVu0CopyVector(VecR[1], VecB[1]);
                sceVu0CopyVector(VecR[2], VecB[2]);
            }

            LAB_00195800:
            sceVu0CopyVector(v[0], v[1]);
            sceVu0CopyVector(v[1], v[2]);
            sceVu0CopyVector(VecB[0], VecB[1]);
            sceVu0CopyVector(VecB[1], VecB[2]);
        }

        if ((nloop & 1) != 0) {
            vp++;
        }
        p_pd = (s32 *)vp;
        p_pd += 5;

        if ((*p_pd >> 0x18) != 0x69)
            break;
    }

    sceVu0SubVector(va, VecR[1], VecR[0]);
    sceVu0SubVector(vb, VecR[2], VecR[1]);
    sceVu0OuterProduct(nor, va, vb);
    sceVu0Normalize(rnor, nor);

    if ((phidx & 1) != 0) {
        rnor[0] = -rnor[0];
        rnor[1] = -rnor[1];
        rnor[2] = -rnor[2];
    }

    return near;
}

f32 sbRayCrsJudgzN(sceVu0FVECTOR rnor, sceVu0FVECTOR vec, sceVu0FVECTOR muki, f32 len) {
    sceVu0FVECTOR pos1;
    sceVu0FVECTOR vf;
    s32 lp0;
    f32 near = 65535.0f;
    f32 dist;
    s32 n_prim;
    sceVu0FVECTOR nor;
    
    sceVu0Normalize(vf, muki);
    pos1[0] = vf[0] * len + vec[0];
    pos1[1] = vf[1] * len + vec[1];
    pos1[2] = vf[2] * len + vec[2];

    n_prim = GetHitBlocks(vec, pos1);
    for (lp0 = 0; lp0 < n_prim; lp0++) {
        u32 *block = HitBlocksList[lp0];
        dist = sbChkHitPol(nor, vec, muki, block);
        if (near > dist) {
            near = dist;
            sceVu0CopyVector(rnor, nor);
        }
    }

    return near;
}

void InitHitPoly(u32 *adr) {
    HITMAPINFO *hmi = &HitMapInfo;

    hmi->stx = ((f32 *)adr)[0];
    hmi->sty = ((f32 *)adr)[1];
    hmi->stz = ((f32 *)adr)[2];
    hmi->gdx = ((u16 *)adr)[6];
    hmi->gdy = ((u16 *)adr)[7];
    hmi->gdz = ((u16 *)adr)[8];
    hmi->gdw = ((s32 *)adr)[5];
    hmi->gdh = ((s32 *)adr)[6];
    hmi->prim_top = adr;
}

void InitHitPolyIndex() {
    // Empty function
}

void SetVu0MemImage() {
    u32 i;
    u32 *sp;
    u32 *dp;

    sceGsSyncPath(0, 0);
    sp = (u32 *)VU0_MEM;
    dp = (u32 *)vu0mem_image;

    for (i = 0; i < 256; i++, sp += 4, dp += 4) {
        dp[0] = sp[0];
        dp[1] = sp[1];
        dp[2] = sp[2];
        dp[3] = sp[3];
    }
}

void StTriFloat(sceVu0IVECTOR v0, sceVu0FMATRIX m0, s32 n) {
    __asm__ volatile(
        "lqc2         $vf4, 0x0(%1)\n"
        "lqc2         $vf5, 0x10(%1)\n"
        "lqc2         $vf6, 0x20(%1)\n"
        "lqc2         $vf7, 0x30(%1)\n"
        "viaddi       $vi1, $vi0, 0x0\n"
        "viaddi       $vi2, $vi0, 0x1\n"
        "vlqi.xyz     $vf22, ($vi1++)\n"
    "_StTriFloat_loop:\n"
        "vlqi.xyz     $vf8, ($vi1++)\n"
        "vitof4.xyz   $vf8, $vf8\n"
        "vmove.w      $vf8, $vf0\n"
        "vadd.xyz     $vf8, $vf8, $vf22\n"
        "vmulax.xyzw  ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw  $vf12, $vf7, $vf8w\n"
        "vdiv         Q, $vf0w, $vf12w\n"
        "vwaitq\n"
        "vmulq.xyzw   $vf12, $vf12, Q\n"
        "vftoi4.xyzw  $vf13, $vf12\n"
        "sqc2         $vf13, 0x0(%0)\n"
        "addi         %0, %0, 0x10\n"
        "addi         %2, %2, -0x1\n"
        "bne          $0, %2, _StTriFloat_loop\n"
    : : "r" (v0), "r" (m0), "r" (n));
}

void TestHitPoly() {
    HITMAPINFO *hmi = &HitMapInfo;
    sceVu0IVECTOR vi;
    sceVu0FVECTOR vf;
    OBJWORK *objw = GameGbl.klonoa;
    s32 areax;
    s32 areay;
    s32 areaz;

    vf[0] = (objw->posi[0] - hmi->stx) / hmi->gdw;
    vf[1] = (objw->posi[1] - hmi->sty) / hmi->gdh;
    vf[2] = (objw->posi[2] - hmi->stz) / hmi->gdw;
    sceVu0FTOI0Vector(vi, vf);
}
