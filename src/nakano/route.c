#include "nakano/route.h"
#include "nakano/vu0mac.h"
#include "nakano/k_math.h"
#include "nakano/main.h"

static ROUTE *pRoute[64];
static s32 testid;

s32 InitMichi(u32 *adr) {
    s32 lp1;
    s32 n;
    u32 *ar;

    testid = 0;
    ar = adr;
    n = *ar;
    ar++;
    for (lp1 = 0; lp1 < 64; lp1++) {
        pRoute[lp1] = NULL;
    }
    for (lp1 = 0; lp1 < n; lp1++, ar++) {
        pRoute[lp1] = (ROUTE *)((u32)adr + *ar);
    }
    pRoute[lp1] = NULL;
    return n;
}

ROUTE* GetMichiAdrs(s32 rtn) {
    return pRoute[rtn];
}

void SetRouteNo(RT_WRK *rtw, s32 rtn) {
    rtw->rtn = rtn;
    rtw->rtp = GetMichiAdrs(rtn);
}

s32 GetMichiNo(ROUTE *rtp) {
    s32 lp1;

    for (lp1 = 0; pRoute[lp1] != NULL; lp1++) {
        if (pRoute[lp1] == rtp) {
            return lp1;
        }
    }
    return -1;
}

void MichiPosi(RT_WRK *rtw, sceVu0FVECTOR vf) {
    s32 cnt;
    ROUTE *rtp;
    sceVu0IVECTOR vi;

    cnt = (s16)(rtw->cnt >> 12);
    rtp = &rtw->rtp[rtw->plc];
    vi[0] = rtp->posi.x + rtp->vec.x * cnt;
    vi[1] = rtp->posi.y + rtp->vec.y * cnt;
    vi[2] = rtp->posi.z + rtp->vec.z * cnt;
    vu0_ITOF12Vector(vf, vi);
}

void MichiPosiXZ(RT_WRK *rtw, sceVu0FVECTOR vf) {
    s32 cnt;
    ROUTE *rtp;
    sceVu0IVECTOR vi;
    sceVu0FVECTOR vfw;

    cnt = (s16)(rtw->cnt >> 12);
    rtp = &rtw->rtp[rtw->plc];
    vi[0] = rtp->posi.x + rtp->vec.x * cnt;
    vi[2] = rtp->posi.z + rtp->vec.z * cnt;
    vu0_ITOF12Vector(vfw, vi);
    vf[0] = vfw[0];
    vf[2] = vfw[2];
}

f32 MichiPosiY(RT_WRK *rtw) {
    s32 cnt;
    ROUTE *rtp;
    sceVu0FVECTOR vf;
    sceVu0IVECTOR vi;

    cnt = (s16)(rtw->cnt >> 12);
    rtp = &rtw->rtp[rtw->plc];
    vi[1] = rtp->posi.y + rtp->vec.y * cnt;
    vu0_ITOF12Vector(vf, vi);
    return vf[1];
}

void GetMichiRotXY(sceVu0FVECTOR vec, sceVu0FVECTOR rot) {
    f32 sa = vec[1];

    rot[0] = -_atan2(GetVectorLenXZ(vec), sa) + M_PI_2f;
    if (rot[0] < -M_PIf) {
        rot[0] += M_TWOPIf;
    } else if (rot[0] > M_PIf) {
        rot[0] -= M_TWOPIf;
    }

    rot[1] = _atan2(vec[0], vec[2]) + M_PIf;
    if (rot[1] < -M_PIf) {
        rot[1] += M_TWOPIf;
    } else if (rot[1] > M_PIf) {
        rot[1] -= M_TWOPIf;
    }
}

void MichiMove(RT_WRK *rtw, s32 Spd) {
    s32 cnt;
    s16 plc;
    vs32 flag; // no idea why this is volatile
    ROUTE *mapr;

    flag = 0;
    rtw->cnt += Spd;
    mapr = &rtw->rtp[rtw->plc];
    if (Spd > 0) {
        while (!flag) {
            cnt = rtw->cnt >> 12;
            if (cnt > mapr->co) {
                if ((mapr + 1)->co != 0x7FFF) {
                    rtw->cnt -= mapr->co << 12;
                    rtw->plc++;
                    mapr++;
                    if (mapr->co == 0x7FFE) {
                        mapr = rtw->rtp;
                        rtw->plc = 0;
                    } else if (rtw->cnt < mapr->co << 12) {
                        flag = 1;
                    }
                } else {
                    flag = 1;
                }
            } else {
                flag = 1;
            }
        }
    } else {
        if (Spd >= 0) {
            return;
        }
        while (!flag) {
            cnt = (s16)(rtw->cnt >> 12);
            if (cnt < 0) {
                if (rtw->plc > 0) {
                    rtw->plc--;
                    mapr--;
                    rtw->cnt += mapr->co << 12;
                    if (rtw->cnt >= 0) {
                        flag = 1;
                    }
                } else if (GetRootEndCode(rtw, &plc) == 0x7FFE) {
                    rtw->plc = plc;
                    mapr = &rtw->rtp[plc];
                    rtw->cnt += mapr->co << 12;
                } else {
                    flag = 1;
                }
            } else {
                flag = 1;
            }
        }
    }
}

s32 GetRouteLength(ROUTE *rtp) {
    s32 rtln = 0;
    while (rtp->co < 0x7FFE) {
        rtln += rtp->co;
        rtp++;
    }
    return rtln;
}

u16 GetRootEndCode(RT_WRK *rtw, s16 *plcp) {
    ROUTE *rtp = rtw->rtp;
    while (rtp->co < 0x7FFE) {
        rtp++;
    }
    *plcp = (rtp - 1) - rtw->rtp;
    return rtp->co;
}

s32 GetMichinori(RT_WRK *rtw) {
    s16 i;
    s32 len;

    len = 0;
    for (i = 0; i < rtw->plc; i++) {
        len += rtw->rtp[i].co;
    }
    len <<= 12;
    return len + rtw->cnt;
}

void SetMichinori(RT_WRK *rtw, s32 mcn) {
    rtw->cnt = 0;
    rtw->plc = 0;
    MichiMove(rtw, mcn);
    rtw->mcn = mcn;
}

void GetMichiVec(RT_WRK *rtw, sceVu0FVECTOR spd) {
    sceVu0IVECTOR vi;
    ROUTE *mapr;

    mapr = &rtw->rtp[rtw->plc];
    vi[0] = mapr->vec.x;
    vi[1] = mapr->vec.y;
    vi[2] = mapr->vec.z;
    vu0_ITOF12Vector(spd, vi);
}

f32 GetVectorLenXZ(sceVu0FVECTOR vf) {
    sceVu0FVECTOR vfw;

    vu0_Square(vfw, vf);
    vfw[3] = vu0_SquareRoot(vfw[0] + vfw[2]);
    return vfw[3];
}

f32 GetMichiSaka(RT_WRK *rtw, s32 michimuki) {
    f32 xzlen;
    sceVu0FVECTOR vf;

    GetMichiVec(rtw, vf);
    xzlen = GetVectorLenXZ(vf);
    if (michimuki){
        vf[1] = -vf[1];
    }
    vf[2] = xzlen;
    vf[0] = _atan2(vf[1], vf[2]);
    return vf[0];
}

f32 GetDistanceXZ(sceVu0FVECTOR pos1, sceVu0FVECTOR pos2) {
    sceVu0FVECTOR vf;
    sceVu0FVECTOR vw;

    vf[0] = pos1[0] - pos2[0];
    vf[2] = pos1[2] - pos2[2];
    vu0_Square(vw, vf);
    return vu0_SquareRoot(vw[0] + vw[2]);
}

f32 GetDistanceXYZ(sceVu0FVECTOR pos1, sceVu0FVECTOR pos2) {
    sceVu0FVECTOR vf;
    sceVu0FVECTOR vw;

    vf[0] = pos1[0] - pos2[0];
    vf[1] = pos1[1] - pos2[1];
    vf[2] = pos1[2] - pos2[2];
    vu0_Square(vw, vf);
    vf[3] = vu0_SquareRoot(vw[0] + vw[1] + vw[2]);
    return vf[3];
}

f32 GetTaniVector(sceVu0FVECTOR pos1, sceVu0FVECTOR pos2, sceVu0FVECTOR vec) {
    f32 i;

    i = GetDistanceXZ(pos1, pos2);
    if (i == 0.0f) {
        i = 1.0f;
    }
    vec[0] = (pos2[0] - pos1[0]) / i;
    vec[1] = 0.0f;
    vec[2] = (pos2[2] - pos1[2]) / i;
    return i;
}

f32 GetTaniVector2(sceVu0FVECTOR pos1, sceVu0FVECTOR pos2, sceVu0FVECTOR vec) {
    f32 i;

    i = GetDistanceXYZ(pos1, pos2);
    if (i == 0.0f) {
        i = 1.0f;
    }
    vec[0] = (pos2[0] - pos1[0]) / i;
    vec[1] = (pos2[1] - pos1[1]) / i;
    vec[2] = (pos2[2] - pos1[2]) / i;
    return i;
}

void SetTaniVector(sceVu0FVECTOR vfo, sceVu0FVECTOR vfi) {
    vfo[0] = vfi[0];
    vfo[2] = vfi[2];
}

s32 noploop() {
    // Empty function
}

s32 MichiBunki(RT_WRK *rtw, const sceVu0FVECTOR pos) {
    volatile sceVu0FVECTOR vw1;
    volatile sceVu0FVECTOR vw2;
    volatile sceVu0FVECTOR vf;
    volatile sceVu0FVECTOR vf2;
    volatile sceVu0FVECTOR rpf;
    volatile sceVu0FVECTOR rvf;
    volatile sceVu0IVECTOR vi;
    volatile ROUTE *lrtp;
    volatile ROUTE *crtp;
    vs32 lp1;
    vs32 lp2;
    vs32 plc;
    vs32 rtn;
    vf32 cntf;
    vf32 workf;
    vf32 minf;
    vf32 lcof;
    vf32 af;
    vf32 bf;
    vf32 cf;
    vf32 cof;

    crtp = rtw->rtp;
    plc = rtw->plc;
    minf = 100000000.0f;
    vi[3] = 4096;
    vf[3] = 1.0f;
    for (lp1 = 0; pRoute[lp1] != NULL; lp1++) {
        lrtp = pRoute[lp1];
        for (lp2 = 0; lrtp->co != 0x7FFF && lrtp->co != 0x7FFE; lp2++, lrtp++) {
            vi[0] = lrtp->posi.x;
            vi[1] = lrtp->posi.y;
            vi[2] = lrtp->posi.z;
            vu0_ITOF12Vector(rpf, vi);
            vi[0] = lrtp->vec.x;
            vi[1] = lrtp->vec.y;
            vi[2] = lrtp->vec.z;
            vu0_ITOF12Vector(rvf, vi);
            sceVu0SubVector(vf, pos, rpf);
            vu0_Square(vw1, vf);
            bf = vw1[0] + vw1[1] + vw1[2];
            if (!(bf > 1048576.0f)) {
                cof = lrtp->co;
                vf[0] = pos[0] - (rpf[0] + rvf[0] * cof);
                vf[1] = pos[1] - (rpf[1] + rvf[1] * cof);
                vf[2] = pos[2] - (rpf[2] + rvf[2] * cof);
                vu0_Square(vw1, vf);
                af = vw1[0] + vw1[1] + vw1[2];
                cf = cof;
                lcof = (bf + cf * cf - af) / (2.0f * cf);
                if (!(lcof < -16.0f) && !(lcof > cof + 16.0f)) {
                    vf2[0] = pos[0] - (rpf[0] + rvf[0] * lcof);
                    vf2[1] = pos[1] - (rpf[1] + rvf[1] * lcof);
                    vf2[2] = pos[2] - (rpf[2] + rvf[2] * lcof);
                    vw2[0] = vf2[0] * vf2[0];
                    vw2[1] = vf2[1] * vf2[1];
                    vw2[2] = vf2[2] * vf2[2];
                    workf = vw2[0] + vw2[1] + vw2[2];
                    if (workf < minf) {
                        rtn = lp1;
                        plc = lp2;
                        cntf = lcof;
                        minf = workf;
                        crtp = pRoute[lp1];
                    }
                }
            }
        }
    }

    if (crtp != rtw->rtp) {
        rtw->plc = plc;
        vf[0] = cntf;
        vi[0] = vf[0] * 4096.0f;
        rtw->cnt = vi[0];
        rtw->rtp = crtp;
        return rtn;
    } else {
        return -1;
    }
}

s32 MichiBunki1(RT_WRK *rtw, sceVu0FVECTOR pos, ROUTE *rtp) {
    volatile sceVu0FVECTOR vw1;
    volatile sceVu0FVECTOR vw2;
    volatile sceVu0FVECTOR vf;
    volatile sceVu0FVECTOR vf2;
    volatile sceVu0FVECTOR rpf;
    volatile sceVu0FVECTOR rvf;
    volatile sceVu0IVECTOR vi;
    volatile ROUTE *lrtp;
    volatile ROUTE *crtp;
    vs32 lp2;
    vs32 plc;
    vf32 cntf;
    vf32 workf;
    vf32 minf;
    vf32 lcof;
    vf32 af;
    vf32 bf;
    vf32 cf;
    vf32 cof;

    crtp = rtw->rtp;
    plc = rtw->plc;
    minf = 100000000.0f;
    vi[3] = 4096;
    vf[3] = 1.0f;
    lrtp = rtp;
    for (lp2 = 0; lrtp->co != 0x7FFF && lrtp->co != 0x7FFE; lp2++, lrtp++) {
        vi[0] = lrtp->posi.x;
        vi[1] = lrtp->posi.y;
        vi[2] = lrtp->posi.z;
        rpf[0] = vi[0] / 4096.0f;
        rpf[1] = vi[1] / 4096.0f;
        rpf[2] = vi[2] / 4096.0f;
        vi[0] = lrtp->vec.x;
        vi[1] = lrtp->vec.y;
        vi[2] = lrtp->vec.z;
        rvf[0] = vi[0] / 4096.0f;
        rvf[1] = vi[1] / 4096.0f;
        rvf[2] = vi[2] / 4096.0f;
        vf[0] = pos[0] - rpf[0];
        vf[1] = pos[1] - rpf[1];
        vf[2] = pos[2] - rpf[2];
        vw1[0] = vf[0] * vf[0];
        vw1[1] = vf[1] * vf[1];
        vw1[2] = vf[2] * vf[2];
        bf = vw1[0] + vw1[1] + vw1[2];
        if (!(bf > 1048576.0f)) {
            cof = lrtp->co;
            vf[0] = pos[0] - (rpf[0] + rvf[0] * cof);
            vf[1] = pos[1] - (rpf[1] + rvf[1] * cof);
            vf[2] = pos[2] - (rpf[2] + rvf[2] * cof);
            vw1[0] = vf[0] * vf[0];
            vw1[1] = vf[1] * vf[1];
            vw1[2] = vf[2] * vf[2];
            af = vw1[0] + vw1[1] + vw1[2];
            cf = cof;
            lcof = (bf + cf * cf - af) / (2.0f * cf);
            if (!(lcof < -16.0f) && !(lcof > cof + 16.0f)) {
                vf2[0] = pos[0] - (rpf[0] + rvf[0] * lcof);
                vf2[1] = pos[1] - (rpf[1] + rvf[1] * lcof);
                vf2[2] = pos[2] - (rpf[2] + rvf[2] * lcof);
                vw2[0] = vf2[0] * vf2[0];
                vw2[1] = vf2[1] * vf2[1];
                vw2[2] = vf2[2] * vf2[2];
                workf = vw2[0] + vw2[1] + vw2[2];
                if (workf < minf) {
                    plc = lp2;
                    cntf = lcof;
                    minf = workf;
                    crtp = rtp;
                }
            }
        }
    }

    if (crtp != rtw->rtp) {
        rtw->plc = plc;
        vf[0] = cntf;
        vi[0] = vf[0] * 4096.0f;
        rtw->cnt = vi[0];
        rtw->rtp = crtp;
        return 0;
    } else {
        return -1;
    }
}

s32 MichiBunki2(RT_WRK *rtw, sceVu0FVECTOR pos) {
    volatile sceVu0FVECTOR vw1;
    volatile sceVu0FVECTOR vw2;
    volatile sceVu0FVECTOR vf;
    volatile sceVu0FVECTOR vf2;
    volatile sceVu0FVECTOR rpf;
    volatile sceVu0FVECTOR rvf;
    volatile sceVu0IVECTOR vi;
    volatile ROUTE *lrtp;
    volatile ROUTE *crtp;
    vs32 lp1;
    vs32 lp2;
    vs32 plc;
    vs32 rtn;
    vf32 cntf;
    vf32 workf;
    vf32 minf;
    vf32 lcof;
    vf32 af;
    vf32 bf;
    vf32 cf;
    vf32 cof;

    crtp = rtw->rtp;
    plc = rtw->plc;
    minf = 100000000.0f;
    vi[3] = 4096;
    vf[3] = 1.0f;
    for (lp1 = 0; pRoute[lp1] != NULL; lp1++) {
        lrtp = pRoute[lp1];
        for (lp2 = 0; lrtp->co != 0x7FFF && lrtp->co != 0x7FFE; lp2++, lrtp++) {
            vi[0] = lrtp->posi.x;
            vi[1] = lrtp->posi.y;
            vi[2] = lrtp->posi.z;
            vu0_ITOF12Vector(rpf, vi);
            vi[0] = lrtp->vec.x;
            vi[1] = lrtp->vec.y;
            vi[2] = lrtp->vec.z;
            vu0_ITOF12Vector(rvf, vi);
            vf[0] = pos[0] - rpf[0];
            vf[1] = pos[1] - rpf[1];
            vf[2] = pos[2] - rpf[2];
            vu0_Square(vw1, vf);
            bf = vw1[0] + vw1[1] + vw1[2];
            cof = lrtp->co;
            vf[0] = pos[0] - (rpf[0] + rvf[0] * cof);
            vf[1] = pos[1] - (rpf[1] + rvf[1] * cof);
            vf[2] = pos[2] - (rpf[2] + rvf[2] * cof);
            vw1[0] = vf[0] * vf[0];
            vw1[1] = vf[1] * vf[1];
            vw1[2] = vf[2] * vf[2];
            af = vw1[0] + vw1[1] + vw1[2];
            cf = cof;
            lcof = (bf + cf * cf - af) / (2.0f * cf);
            if (!(lcof < -16.0f) && !(lcof > cof + 16.0f)) {
                vf2[0] = pos[0] - (rpf[0] + rvf[0] * lcof);
                vf2[1] = pos[1] - (rpf[1] + rvf[1] * lcof);
                vf2[2] = pos[2] - (rpf[2] + rvf[2] * lcof);
                vu0_Square(vw2, vf2);
                workf = vw2[0] + vw2[1] + vw2[2];
                if (workf < minf) {
                    rtn = lp1;
                    plc = lp2;
                    cntf = lcof;
                    minf = workf;
                    crtp = pRoute[lp1];
                }
            }
        }
    }

    if (minf == 100000000.0f) {
        return -1;
    }

    rtw->plc = plc;
    vf[0] = cntf;
    vi[0] = vf[0] * 4096.0f;
    rtw->cnt = vi[0];
    rtw->rtp = crtp;
    return rtn;
}

s32 MichiBunki2Cir(RT_WRK *rtw, sceVu0FVECTOR pos) {
    volatile sceVu0FVECTOR vw1;
    volatile sceVu0FVECTOR vf;
    volatile sceVu0FVECTOR rpf;
    volatile sceVu0FVECTOR rvf;
    volatile sceVu0IVECTOR vi;
    volatile ROUTE *lrtp;
    volatile ROUTE *crtp;
    vs32 lp1;
    vs32 lp2;
    vs32 plc;
    vs32 rtn;
    vf32 cntf;
    vf32 workf;
    vf32 minf;
    vf32 lcof;
    vf32 af;

    crtp = rtw->rtp;
    plc = rtw->plc;
    minf = 100000000.0f;
    vi[3] = 4096;
    vf[3] = 1.0f;
    for (lp1 = 0; pRoute[lp1] != NULL; lp1++) {
        lrtp = pRoute[lp1];
        for (lp2 = 0; lrtp->co != 0x7FFF && lrtp->co != 0x7FFE; lp2++, lrtp++) {
            vi[0] = lrtp->posi.x;
            vi[1] = lrtp->posi.y;
            vi[2] = lrtp->posi.z;
            vu0_ITOF12Vector(rpf, vi);
            vi[0] = lrtp->vec.x;
            vi[1] = lrtp->vec.y;
            vi[2] = lrtp->vec.z;
            vu0_ITOF12Vector(rvf, vi);
            vf[0] = pos[0] - rpf[0];
            vf[1] = pos[1] - rpf[1];
            vf[2] = pos[2] - rpf[2];
            vu0_Square(vw1, vf);
            workf = vw1[0] + vw1[2];
            af = vu0_SquareRoot(workf);
            workf = af;
            if (workf < minf) {
                rtn = lp1;
                plc = lp2;
                cntf = lcof;
                minf = workf;
                crtp = pRoute[lp1];
            }
        }
    }

    if (minf == 100000000.0f) {
        return -1;
    }

    rtw->plc = plc;
    vf[0] = cntf;
    vi[0] = vf[0] * 4096.0f;
    rtw->cnt = vi[0];
    rtw->rtp = crtp;
    return rtn;
}

void DrawDebugRoute() {
    ROUTE *rtp;
    nkGifPacket pack;
    nkGifPacket *pp;
    sceVu0IVECTOR vi;
    sceVu0FVECTOR vf;
    sceVu0FVECTOR vf0;
    sceVu0FVECTOR vf1;
    sceVu0IVECTOR v0;
    sceVu0IVECTOR v1;
    s32 LP1;
    s32 cnt;

    pp = &pack;
    cnt = 0;
    for (LP1 = 0; pRoute[LP1] != NULL; LP1++) {
        for (rtp = pRoute[LP1]; rtp->co != 0x7FFF && rtp->co != 0x7FFE; rtp++) {
            vi[0] = rtp->posi.x;
            vi[1] = rtp->posi.y;
            vi[2] = rtp->posi.z;
            vu0_ITOF12Vector(vf, vi);
            vf[3] = 1.0f;
            vu0_LoadMtx(GameGbl.wsm);
            vu0_Thing(vf0, vf);
            if (!VertClip(vf0)) {
                vu0_FTOI4Vector(v0, vf0);
                vi[0] = rtp->posi.x + rtp->vec.x * rtp->co;
                vi[1] = rtp->posi.y + rtp->vec.y * rtp->co;
                vi[2] = rtp->posi.z + rtp->vec.z * rtp->co;
                vu0_ITOF12Vector(vf, vi);
                vf[3] = 1.0f;
                vu0_LoadMtx(GameGbl.wsm);
                vu0_Thing(vf1, vf);
                if (!VertClip(vf1)) {
                    vu0_FTOI4Vector(v1, vf1);
                    pp->size = 0;
                    pp->buf = (nkQWdata *)0x70000000;
                    if (cnt & 1) {
                        pp->buf = (nkQWdata *)0x70002000;
                    }
                    pp->buf[pp->size].ul128 = 0;
                    pp->buf[pp->size++].ul32[0] = 0x70000005;
                    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_LINE, 1, 0, 0, 0, 0, 0, 1, 0), 0, 2);
                    pp->buf[pp->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZF2 << 4;

                    pp->buf[pp->size].ul32[0] = 0xFF;
                    pp->buf[pp->size].ul32[1] = 0x00;
                    pp->buf[pp->size].ul32[2] = 0xFF;
                    pp->buf[pp->size].ul32[3] = 0x80;
                    pp->size++;
                    pp->buf[pp->size].ul32[0] = v0[0];
                    pp->buf[pp->size].ul32[1] = v0[1];
                    pp->buf[pp->size].ul32[2] = v0[2];
                    pp->buf[pp->size].ul32[3] = 0;
                    pp->size++;

                    pp->buf[pp->size].ul32[0] = 0x00;
                    pp->buf[pp->size].ul32[1] = 0x40;
                    pp->buf[pp->size].ul32[2] = 0x40;
                    pp->buf[pp->size].ul32[3] = 0x80;
                    pp->size++;
                    pp->buf[pp->size].ul32[0] = v1[0];
                    pp->buf[pp->size].ul32[1] = v1[1];
                    pp->buf[pp->size].ul32[2] = v1[2];
                    pp->buf[pp->size].ul32[3] = 0;
                    pp->size++;

                    FlushCache(0);
                    DmaChGIF->chcr.TTE = 1;
                    sceDmaSend(DmaChGIF, (void *)((u32)pp->buf | 0x80000000));
                    cnt++;
                }
            }
        }
    }
}

void testmove(OBJWORK *objw) {
    ROUTE *lrtp;
    s32 lp2;
    kPadDATA *kpd;
    sceVu0IVECTOR vi;
    f32 *v0;

    kpd = &GameGbl.kpd[0];
    if (pPAD_TRG_CIRCLE(kpd)) {
        testid++;
    }

    lrtp = pRoute[0];
    for (lp2 = 0; lp2 < testid; lp2++) {
        lrtp++;
    }

    vi[0] = lrtp->posi.x;
    vi[1] = lrtp->posi.y;
    vi[2] = lrtp->posi.z;
    v0 = objw->posi;
    vu0_ITOF12Vector(v0, vi);
}
