#include "hr_anmvp.h"
#include "h_vpm2.h"
#include "nakano/dma.h"
#include "nakano/main.h"

static s32 hr_avp_sint(HRANMV *av, s32 mno);
static s32 hr_avp_sint2(HRANMV *av, s32 mno);

s32 (*hrAnmVpmTbl[5])(HRANMV *, s32) = {
    NULL,
    hr_avp_sint,
    hr_avp_sint2,
    hr_avp_sint,
    hr_avp_sint2
};
HRANMV hravbuf[1] = {};
s32 hravcnt = 0;
s32 hrmapoff = 0;
HRAVL hrvlight[2] = {};

extern u32 hr_vu1m_av1 __attribute__((section(".vudata")));
extern u32 hr_vu1m_av2 __attribute__((section(".vudata")));
extern u32 hr_vu1m_av1o __attribute__((section(".vudata")));

static void hr_cross(HRAVL *avl, u32 wh) {
    sceVu0FVECTOR v;
    u32 r;
    u32 g;
    u32 b; // ?

    if (!PAD_LVL_TRIANG(GameGbl.kpd[0])) {
        v[0] = avl->x;
        v[1] = avl->y;
        v[2] = avl->z;
        v[3] = 1.0f;

        r = avl->r * 8.0f;
        if (r > 0xFF) {
            r = 0xFF;
        }
        g = avl->g * 8.0f;
        if (g > 0xFF) {
            g = 0xFF;
        }
        b = avl->b * 8.0f;
        if (b > 0xFF) {
            b = 0xFF;
        }

        hr_crossline(v, wh, r, g, b, 0);
    }
}

u32 hr_float2int(f32 f) {
    return *(u32 *)&f;
}

void hr_anmVPM_allinit() {
    s32 i;
    HRANMV *av;

    hravcnt = 0;
    hrmapoff = 0;

    for (i = 0, av = hravbuf; i < 1; i++, av++) {
        av->prog = NULL;
        av->flag = 0;
        av->work0 = av->work1 = av->work2 = av->work3 = 0;
    }
}

void hr_anmVPM_set(HRANMVS *avs) {
    s32 i;
    HRANMV *av;

    for (i = 0, av = hravbuf; i < 1; i++, av++) {
        if (av->flag == 0) {
            hravcnt++;
            av->flag = 1;
            av->info = avs->info;
            av->prog = avs->prog;
            av->drawno = avs->drawno;
            if (avs->init != NULL) {
                avs->init(av);
            }
            break;
        }
    }
}

void hr_set_anmVPMtbl(HRANMVS *avs) {
    do {
        if (avs->drawno == -1) {
            break;
        }
        hr_anmVPM_set(avs++);
    } while (hravcnt <= 0);
}

void hr_anmVPM_work() {
    HRANMV *av;
    s32 i;

    for (i = 0, av = hravbuf; i < 1; i++, av++) {
        if (av->flag != 0 && av->prog != NULL) {
            av->prog(av);
        }
    }
}

void hr_set_vlight(HRAVL *vlight, f32 x, f32 y, f32 z, f32 r, f32 g, f32 b, f32 n, f32 f) {
    vlight->x = x;
    vlight->y = y;
    vlight->z = z;
    vlight->r = r;
    vlight->g = g;
    vlight->b = b;
    vlight->n = n;
    vlight->f = f;
}

void hr_set_vlightMini(HRAVL *vlight, f32 r, f32 g, f32 b) {
    vlight->ri = r;
    vlight->gi = g;
    vlight->bi = b;
}

static void hr_sint_fogab(f64 n, f64 f, f64 nf, f64 maxp, f64 minp, u32 *a, u32 *b) {
   f32 size;

    // ?
    f32 f_n;
    f32 f_maxp;
    f32 f_minp;
    f32 f_nf;
    f32 f_f;

    f_n = n;
    f_f = f;
    f_nf = nf;
    f_maxp = maxp;
    f_minp = minp;

    size = f_maxp - f_minp;
    if (size != 0.0f && f_nf != 0.0f) {
        *b = hr_float2int(size / f_nf);
        *a = hr_float2int(f_minp - (size * f_f) / f_nf);
    } else {
        *a = hr_float2int(f_minp);
        *b = hr_float2int(0.0f);
    }
}

static void hr_avp_vlight(HRAVL *vlight, ATR_LIGHTP *pp2) {
    f32 nf;

    nf = vlight->n - vlight->f;
    hr_sint_fogab(vlight->n, vlight->f, nf, vlight->r, vlight->ri, (u32 *)&pp2->foga[0], (u32 *)&pp2->fogb[0]);
    hr_sint_fogab(vlight->n, vlight->f, nf, vlight->r, vlight->ri, (u32 *)&pp2->foga[1], (u32 *)&pp2->fogb[1]);
    hr_sint_fogab(vlight->n, vlight->f, nf, vlight->r, vlight->ri, (u32 *)&pp2->foga[2], (u32 *)&pp2->fogb[2]);

    pp2->max[0] = *(s32 *)&vlight->r;
    pp2->max[1] = *(s32 *)&vlight->g;
    pp2->max[2] = *(s32 *)&vlight->b;
    pp2->min[0] = *(s32 *)&vlight->ri;
    pp2->min[1] = *(s32 *)&vlight->gi;
    pp2->min[2] = *(s32 *)&vlight->bi;

    pp2->max[3] = 0;
    pp2->min[3] = 0;

    pp2->spot[0] = *(s32 *)&vlight->x;
    pp2->spot[1] = *(s32 *)&vlight->y;
    pp2->spot[2] = *(s32 *)&vlight->z;
    pp2->spot[3] = 0;
}

static s32 hr_avp_sint(HRANMV *av, s32 mno) {
    ATR_AVPSINT0 *pp;
    u32 cnt;
    u32 cnt2;
    vpmINFO *info;
    u32 *list0;
    u32 *list1;
    sceVu0FVECTOR pos;
    HRAVL *vli;

    vli = hrvlight;
    pos[0] = vli->x;
    pos[1] = vli->y;
    pos[2] = vli->z;
    pos[3] = 1.0f;

    cnt = hrSetBlockListSL(av->info, &cvpm, GameGbl.wvm, vli->f + 1773.568f, pos);
    if (av->drawno == HRAV_SINTD) {
        hr_cross(vli, 32);
    }
    nkAddRef(&hr_vu1m_av1);

    pp = (ATR_AVPSINT0 *)p1_packet;
    pp->dmatag[0] = 0x10000008;
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    hrSetObjAttrUnpack(pp->vif0, 0x3AF, 6);
    *(s64 *)pp->tag = 0x302E400000000000;
    hr_avp_vlight(vli, &pp->light0);
    pp->light0.max[3] = hr_float2int(255.0f);
    hrSetObjAttrMscal(pp->mscal, 6);
    FlushCache(0);
    p1_packet = (qword *)++pp;

    info = av->info;
    cnt2 = info->block_list[0];
    if (cnt != 0) {
        info->block_list[0] = cnt;
        hr_set_vpmblock(info);
        FlushCache(0);
    }
    if (cnt != cnt2) {
        hr_dmavif(0, 0x10000000);
        nkAddRef(&hr_vu1m_av1o);
        cnt2 -= cnt;
        info->block_list[0] = cnt2;
        list0 = info->block_list + 1;
        list1 = info->block_list + 1 + cnt;
        while (--cnt2 != 0xFFFFFFFF) {
            *list0++ = *list1++;
        }
        hr_set_vpmblock(info);
        FlushCache(0);
    }

    return 0;
}

static s32 hr_avp_sint2(HRANMV *av, s32 mno) {
    ATR_AVPSINT2 *pp;
    ATR_AVPSINT2Z *ppz;
    u32 cnt;
    u32 cnt2;
    vpmINFO *info;
    u32 *list0;
    u32 *list1;
    sceVu0FVECTOR pos;
    sceVu0FVECTOR pos2;
    HRAVL *vli;

    vli = hrvlight;
    pos[0] = vli->x;
    pos[1] = vli->y;
    pos[2] = vli->z;
    pos[3] = 1.0f;
    vli++;
    pos2[0] = vli->x;
    pos2[1] = vli->y;
    pos2[2] = vli->z;
    pos2[3] = 1.0f;

    if (vli->f < (vli + 1)->f) {
        vli--;
    }

    cnt = hrSetBlockListSL2(av->info, &cvpm, GameGbl.wvm, vli->f + 1773.568f, pos, pos2);
    vli = hrvlight;
    if (av->drawno == HRAV_SINT2D) {
        hr_cross(vli, 32);
        hr_cross(vli + 1, 32);
    }
    nkAddRef(&hr_vu1m_av2);

    pp = (ATR_AVPSINT2 *)p1_packet;
    pp->dmatag[0] = 0x1000000D;
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    hrSetObjAttrUnpack(pp->vif0, 0x3AF, 11);
    *(s64 *)pp->tag = 0x302E400000000000;
    hr_avp_vlight(vli++, &pp->light0);
    hr_avp_vlight(vli, &pp->light1);
    pp->light0.max[3] = hr_float2int(255.0f);
    hrSetObjAttrMscal(pp->mscal, 6);
    FlushCache(0);
    p1_packet = (qword *)++pp;

    info = av->info;
    cnt2 = info->block_list[0];
    if (cnt != 0) {
        info->block_list[0] = cnt;
        hr_set_vpmblock(info);
        FlushCache(0);
    }
    if (cnt != cnt2) {
        hr_dmavif(0, 0x10000000);
        nkAddRef(&hr_vu1m_av1o);
        ppz = (ATR_AVPSINT2Z *)p1_packet;
        ppz->dmatag[0] = 0x10000001;
        ppz->dmatag[1] = ppz->dmatag[2] = ppz->dmatag[3] = 0;
        hrSetObjAttrMscal(ppz->mscal, 6);
        FlushCache(0);
        p1_packet = (qword *)++ppz;
        cnt2 -= cnt;
        info->block_list[0] = cnt2;
        list0 = info->block_list + 1;
        list1 = info->block_list + 1 + cnt;
        while (--cnt2 != 0xFFFFFFFF) {
            *list0++ = *list1++;
        }
        hr_set_vpmblock(info);
        FlushCache(0);
    }

    return 0;
}