#include <sifdev.h>
#include "kazuya/bios.h"
#include "kazuya/cvar.h"
#include "nakano/main.h"
#include "harada/hr_mapdr.h"
#include "nakano/light.h"
#include "hoshino/h_gamesnd.h"
#include "nakano/hlzdeco.h"
#include "nakano/dma.h"
#include "hoshino/h_file.h"

static u64 kz_tex0;
GAMEDATA gamdat = {}; // TODO: where does this go?
sceVu0FMATRIX kz_normal_light = {};
sceVu0FMATRIX kz_light_color = {};
u8 *D_3509A0[18] = {};

// TODO: ???
SE_INFO SeInfo_Mono    = { 0, 0xC80C80DA0E702, 0, 1.0f, 1.0f };
SE_INFO SeInfo_StereoL = { 0, 0xC80C80DA0E700, 0, 1.0f, 0.0f };
SE_INFO SeInfo_StereoR = { 0, 0xC80C80DA0E701, 0, 0.0f, 1.0f };
SE_INFO *SoundEffects[3] = { &SeInfo_Mono, &SeInfo_StereoL, &SeInfo_StereoR };

// TODO: This doesn't go here, also fill these in
BGM_LIST_INFO BgmListInfo_Half = { };
BGM_LIST_INFO BgmListInfo_All = { };
BGM_LIST_INFO *BgmLists[] = { &BgmListInfo_Half, &BgmListInfo_All };

void kz_Set_Line(DVECTOR *sxy0, DVECTOR *sxy1, u32 sz, KZCOLOR *col) {
    GS_LINE *pp;

    pp = (GS_LINE *)0x70000000;
    pp->DMAtag = 0x70000005;
    pp->GIFtag = SCE_GIF_SET_TAG(1, 0, 0, 0, 1, 1);
    pp->GIFtag_regs = SCE_GS_PRIM;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_LINESTRIP, 1, 0, 0, 1, 0, 0, 1, 0);
    pp->tag1 = SCE_GIF_SET_TAG(2, 1, 0, 0, 1, 2);
    pp->tag1_regs = SCE_GS_XYZ2 << 4 | SCE_GS_RGBAQ;

    pp->p[0].x0 = KZOFFX.vx + (sxy0->vx << 4);
    pp->p[1].x0 = KZOFFX.vx + (sxy1->vx << 4);
    pp->p[0].y0 = KZOFFX.vx + (sxy0->vy << 3);
    pp->p[1].y0 = KZOFFX.vx + (sxy1->vy << 3);
    pp->p[0].z0 = pp->p[1].z0 = sz;
    pp->p[0].r0 = pp->p[1].r0 = col->r;
    pp->p[0].g0 = pp->p[1].g0 = col->g;
    pp->p[0].b0 = pp->p[1].b0 = col->b;
    pp->p[0].a0 = pp->p[1].a0 = col->a;

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_SprtUV(DVECTOR *sxy, u32 sz, DVECTOR *swh, DVECTOR *uv, KZCOLOR *col) {
    GS_SPRT_UV *pp;

    // stdump didn't list these variables...
    u16 v1;
    u16 u1;
    u16 v0;
    u16 y1;
    u16 x1;
    u16 u0;
    u16 y0;
    u16 x0;

    pp = (GS_SPRT_UV *)0x70000000;
    x0 = KZOFFX.vx + (sxy->vx << 4) - 4;
    x1 = KZOFFX.vx + ((sxy->vx + swh->vx - 1) << 4) + 4;
    y0 = KZOFFY.vx + (sxy->vy << 3) - 4;
    y1 = KZOFFY.vx + ((sxy->vy + swh->vy - 1) << 3) + 4;
    u0 = (uv->vx << 4) + 4;
    u1 = ((uv->vx + swh->vx) << 4) - 4;
    v0 = (uv->vy << 4) + 4;
    v1 = ((uv->vy + swh->vy) << 4) - 4;

    pp->DMAtag = 0x70000005;
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 7);
    pp->GIFtag_regs = \
        SCE_GS_XYZ2 << 24 | SCE_GS_UV     << 20 | \
        SCE_GS_XYZ2 << 16 | SCE_GS_UV     << 12 | \
        SCE_GS_RGBAQ << 8 | SCE_GS_TEX0_2 << 4  | SCE_GS_PRIM;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y1;
    pp->z0 = sz;
    pp->z1 = sz;
    pp->u0 = u0;
    pp->v0 = v0;
    pp->u1 = u1;
    pp->v1 = v1;
    pp->r0 = col->r;
    pp->g0 = col->g;
    pp->b0 = col->b;
    pp->a0 = col->a;
    pp->q0 = 1.0f;
    pp->tex0 = kz_tex0;

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_SprtUV_Eff(DVECTOR *sxy, u32 sz, DVECTOR *swh, DVECTOR *uv, KZCOLOR *col) {
    GS_POLY_FT4_UV *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;

    pp = (GS_POLY_FT4_UV *)0x70000000;
    x0 = KZOFFX.vx + (sxy->vx << 4) - 4;
    x1 = KZOFFX.vx + ((sxy->vx + swh->vx - 1) << 4) + 4;
    y0 = KZOFFY.vx + (sxy->vy * 16) - 4;
    y1 = KZOFFY.vx + (((sxy->vy + swh->vy) * 2 - 1) << 3) + 4;
    u0 = (uv->vx << 4) + 4;
    u1 = ((uv->vx + swh->vx) << 4) - 4;
    v0 = (uv->vy << 4) + 4;
    v1 = ((uv->vy + swh->vy) << 4) - 4;

    pp->DMAtag = 0x70000007;
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 11);
    pp->GIFtag_regs = 0x53535353170;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 0, 1, 0, 1, 0, 1, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y0;
    pp->x2 = x0;
    pp->y2 = y1;
    pp->x3 = x1;
    pp->y3 = y1;
    pp->z0 = sz;
    pp->z1 = sz;
    pp->z2 = sz;
    pp->z3 = sz;
    pp->u0 = u0;
    pp->v0 = v0;
    pp->u1 = u1;
    pp->v1 = v0;
    pp->u2 = u0;
    pp->v2 = v1;
    pp->u3 = u1;
    pp->v3 = v1;
    pp->r0 = col->r;
    pp->g0 = col->g;
    pp->b0 = col->b;
    pp->a0 = col->a;
    pp->q0 = 1.0f;
    pp->tex0 = kz_tex0;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_TileF4(DVECTOR *sxy, u32 sz, DVECTOR *swh, KZCOLOR *col) {
    GS_POLY_F4 *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;

    pp = (GS_POLY_F4 *)0x70000000;
    x0 = KZOFFX.vx + (sxy->vx << 4) - 4;
    x1 = KZOFFX.vx + ((sxy->vx + swh->vx - 1) << 4) + 4;
    y0 = KZOFFY.vx + (sxy->vy << 3) - 4;
    y1 = KZOFFY.vx + ((sxy->vy + swh->vy - 1) << 3) + 4;

    pp->DMAtag = 0x70000004;
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 6);
    pp->GIFtag_regs = 0x555510;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 0, 0, 0, 1, 0, 0, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y0;
    pp->x2 = x0;
    pp->y2 = y1;
    pp->x3 = x1;
    pp->y3 = y1;
    pp->z0 = sz;
    pp->z1 = sz;
    pp->z2 = sz;
    pp->z3 = sz;
    pp->r0 = col->r;
    pp->g0 = col->g;
    pp->b0 = col->b;
    pp->a0 = col->a;
    pp->q0 = 1.0f;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_TileG4(DVECTOR *sxy, u32 sz, DVECTOR *swh, KZCOLOR *col0, KZCOLOR *col1, KZCOLOR *col2, KZCOLOR *col3) {
    GS_POLY_G4 *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;

    pp = (GS_POLY_G4 *)0x70000000;
    x0 = KZOFFX.vx + (sxy->vx << 4) - 4;
    x1 = KZOFFX.vx + ((sxy->vx + swh->vx - 1) << 4) + 4;
    y0 = KZOFFY.vx + (sxy->vy << 3) - 4;
    y1 = KZOFFY.vx + ((sxy->vy + swh->vy - 1) << 3) + 4;

    pp->DMAtag = 0x70000006;
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 9);
    pp->GIFtag_regs = 0x515151510;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 1, 0, 0, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y0;
    pp->x2 = x0;
    pp->y2 = y1;
    pp->x3 = x1;
    pp->y3 = y1;
    pp->z0 = sz;
    pp->z1 = sz;
    pp->z2 = sz;
    pp->z3 = sz;
    pp->r0 = col0->r;
    pp->g0 = col0->g;
    pp->b0 = col0->b;
    pp->a0 = col0->a;
    pp->r1 = col1->r;
    pp->g1 = col1->g;
    pp->b1 = col1->b;
    pp->a1 = col1->a;
    pp->r2 = col2->r;
    pp->g2 = col2->g;
    pp->b2 = col2->b;
    pp->a2 = col2->a;
    pp->r3 = col3->r;
    pp->g3 = col3->g;
    pp->b3 = col3->b;
    pp->a3 = col3->a;
    pp->q0 = 1.0f;
    pp->q1 = 1.0f;
    pp->q2 = 1.0f;
    pp->q3 = 1.0f;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_TileFT4UV(DVECTOR *sxy, u32 sz, DVECTOR *swh, DVECTOR *uv, DVECTOR *twh, KZCOLOR *col) {
    GS_SPRT_UV *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;

    pp = (GS_SPRT_UV *)0x70000000;
    x0 = KZOFFX.vx + (sxy->vx << 4) - 4;
    x1 = KZOFFX.vx + ((sxy->vx + swh->vx - 1) << 4) + 4;
    y0 = KZOFFY.vx + (sxy->vy << 3) - 4;
    y1 = KZOFFY.vx + ((sxy->vy + swh->vy - 1) << 3) + 4;
    u0 = (uv->vx << 4) + 4;
    u1 = ((uv->vx + twh->vx) << 4) - 4;
    v0 = (uv->vy << 4) + 4;
    v1 = ((uv->vy + twh->vy) << 4) - 4;

    pp->DMAtag = 0x70000005;
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 7);
    pp->GIFtag_regs = \
        SCE_GS_XYZ2 << 24 | SCE_GS_UV     << 20 | \
        SCE_GS_XYZ2 << 16 | SCE_GS_UV     << 12 | \
        SCE_GS_RGBAQ << 8 | SCE_GS_TEX0_2 << 4  | SCE_GS_PRIM;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y1;
    pp->z0 = sz;
    pp->z1 = sz;
    pp->u0 = u0;
    pp->v0 = v0;
    pp->u1 = u1;
    pp->v1 = v1;
    pp->r0 = col->r;
    pp->g0 = col->g;
    pp->b0 = col->b;
    pp->a0 = col->a;
    pp->q0 = 1.0f;
    pp->tex0 = kz_tex0;

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_TileGT4UV(DVECTOR *sxy, u32 sz, DVECTOR *swh, DVECTOR *uv, DVECTOR *twh, KZCOLOR *col0, KZCOLOR *col1, KZCOLOR *col2, KZCOLOR *col3) {
    GS_POLY_GT4_UV *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;

    pp = (GS_POLY_GT4_UV *)0x70000000;
    x0 = KZOFFX.vx + (sxy->vx << 4) - 4;
    x1 = KZOFFX.vx + ((sxy->vx + swh->vx - 1) << 4) + 4;
    y0 = KZOFFY.vx + (sxy->vy << 3) - 4;
    y1 = KZOFFY.vx + (((sxy->vy + swh->vy) - 1) << 3) + 4;
    u0 = (uv->vx << 4) + 4;
    u1 = ((uv->vx + twh->vx) << 4) - 4;
    v0 = (uv->vy << 4) + 4;
    v1 = ((uv->vy + twh->vy) << 4) - 4;

    pp->DMAtag = 0x70000008;
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 14);
    pp->GIFtag_regs = 0x51351351351370;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y0;
    pp->x2 = x0;
    pp->y2 = y1;
    pp->x3 = x1;
    pp->y3 = y1;
    pp->z0 = sz;
    pp->z1 = sz;
    pp->z2 = sz;
    pp->z3 = sz;
    pp->u0 = u0;
    pp->v0 = v0;
    pp->u1 = u1;
    pp->v1 = v0;
    pp->u2 = u0;
    pp->v2 = v1;
    pp->u3 = u1;
    pp->v3 = v1;
    pp->r0 = col0->r;
    pp->g0 = col0->g;
    pp->b0 = col0->b;
    pp->a0 = col0->a;
    pp->r1 = col1->r;
    pp->g1 = col1->g;
    pp->b1 = col1->b;
    pp->a1 = col1->a;
    pp->r2 = col2->r;
    pp->g2 = col2->g;
    pp->b2 = col2->b;
    pp->a2 = col2->a;
    pp->r3 = col3->r;
    pp->g3 = col3->g;
    pp->b3 = col3->b;
    pp->a3 = col3->a;
    pp->q0 = 1.0f;
    pp->q1 = 1.0f;
    pp->q2 = 1.0f;
    pp->q3 = 1.0f;
    pp->tex0 = kz_tex0;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_PolyF3(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, KZCOLOR *col) {
    GS_POLY_F3 *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;

    pp = (GS_POLY_F3 *)0x70000000;
    x0 = KZOFFX.vx + (*crd0)[0];
    y0 = KZOFFY.vx + (*crd0)[1];
    x1 = KZOFFX.vx + (*crd1)[0];
    y1 = KZOFFY.vx + (*crd1)[1];
    x2 = KZOFFX.vx + (*crd2)[0];
    y2 = KZOFFY.vx + (*crd2)[1];

    pp->DMAtag = 0x70000000 + (sizeof(GS_POLY_F3) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 5);
    pp->GIFtag_regs = SCE_GS_XYZ2 << 16 | SCE_GS_XYZ2 << 12 | SCE_GS_XYZ2 << 8 | SCE_GS_RGBAQ << 4 | SCE_GS_PRIM;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 0, 0, 0, 1, 0, 0, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y1;
    pp->x2 = x2;
    pp->y2 = y2;
    pp->z0 = (*crd0)[2];
    pp->z1 = (*crd1)[2];
    pp->z2 = (*crd2)[2];
    pp->r0 = col->r;
    pp->g0 = col->g;
    pp->b0 = col->b;
    pp->a0 = col->a;
    pp->q0 = 1.0f;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_PolyF4(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, KZCOLOR *col) {
    GS_POLY_F4 *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
    u16 x3;
    u16 y3;

    pp = (GS_POLY_F4 *)0x70000000;
    x0 = KZOFFX.vx + (*crd0)[0] - 4;
    x1 = KZOFFX.vx + (*crd1)[0] + 4;
    x2 = KZOFFX.vx + (*crd2)[0] - 4;
    x3 = KZOFFX.vx + (*crd3)[0] + 4;
    y0 = KZOFFY.vx + (*crd0)[1] - 4;
    y1 = KZOFFY.vx + (*crd1)[1] - 4;
    y2 = KZOFFY.vx + (*crd2)[1] + 4;
    y3 = KZOFFY.vx + (*crd3)[1] + 4;

    pp->DMAtag = 0x70000000 + (sizeof(GS_POLY_F4) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 6);
    pp->GIFtag_regs = SCE_GS_XYZ2 << 20 | SCE_GS_XYZ2 << 16 | SCE_GS_XYZ2 << 12 | SCE_GS_XYZ2 << 8 | SCE_GS_RGBAQ << 4 | SCE_GS_PRIM;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 0, 0, 0, 1, 0, 0, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y1;
    pp->x2 = x2;
    pp->y2 = y2;
    pp->x3 = x3;
    pp->y3 = y3;
    pp->z0 = (*crd0)[2];
    pp->z1 = (*crd1)[2];
    pp->z2 = (*crd2)[2];
    pp->z3 = (*crd3)[2];
    pp->r0 = col->r;
    pp->g0 = col->g;
    pp->b0 = col->b;
    pp->a0 = col->a;
    pp->q0 = 1.0f;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_PolyG4(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, KZCOLOR *col0, KZCOLOR *col1, KZCOLOR *col2, KZCOLOR *col3) {
    GS_POLY_G4 *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
    u16 x3;
    u16 y3;

    pp = (GS_POLY_G4 *)0x70000000;
    x0 = KZOFFX.vx + (*crd0)[0] - 4;
    x1 = KZOFFX.vx + (*crd1)[0] + 4;
    x2 = KZOFFX.vx + (*crd2)[0] - 4;
    x3 = KZOFFX.vx + (*crd3)[0] + 4;
    y0 = KZOFFY.vx + (*crd0)[1] - 4;
    y1 = KZOFFY.vx + (*crd1)[1] - 4;
    y2 = KZOFFY.vx + (*crd2)[1] + 4;
    y3 = KZOFFY.vx + (*crd3)[1] + 4;

    pp->DMAtag = 0x70000000 + (sizeof(GS_POLY_G4) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 9);
    pp->GIFtag_regs = \
        (u64)SCE_GS_XYZ2 << 32 | (u64)SCE_GS_RGBAQ << 28 | \
        (u64)SCE_GS_XYZ2 << 24 | (u64)SCE_GS_RGBAQ << 20 | \
        (u64)SCE_GS_XYZ2 << 16 | (u64)SCE_GS_RGBAQ << 12 | \
        (u64)SCE_GS_XYZ2 << 8  | (u64)SCE_GS_RGBAQ << 4  | \
        (u64)SCE_GS_PRIM;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 1, 0, 0, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y1;
    pp->x2 = x2;
    pp->y2 = y2;
    pp->x3 = x3;
    pp->y3 = y3;
    pp->z0 = (*crd0)[2];
    pp->z1 = (*crd1)[2];
    pp->z2 = (*crd2)[2];
    pp->z3 = (*crd3)[2];
    pp->r0 = col0->r;
    pp->g0 = col0->g;
    pp->b0 = col0->b;
    pp->a0 = col0->a;
    pp->r1 = col1->r;
    pp->g1 = col1->g;
    pp->b1 = col1->b;
    pp->a1 = col1->a;
    pp->r2 = col2->r;
    pp->g2 = col2->g;
    pp->b2 = col2->b;
    pp->a2 = col2->a;
    pp->r3 = col3->r;
    pp->g3 = col3->g;
    pp->b3 = col3->b;
    pp->a3 = col3->a;
    pp->q0 = 1.0f;
    pp->q1 = 1.0f;
    pp->q2 = 1.0f;
    pp->q3 = 1.0f;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_PolyFT4UV(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, DVECTOR *uv0, DVECTOR *uv1, DVECTOR *uv2, DVECTOR *uv3, KZCOLOR *col) {
    GS_POLY_FT4_UV *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
    u16 x3;
    u16 y3;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
    u16 u2;
    u16 v2;
    u16 u3;
    u16 v3;

    pp = (GS_POLY_FT4_UV *)0x70000000;
    x0 = KZOFFX.vx + (*crd0)[0] - 4;
    x1 = KZOFFX.vx + (*crd1)[0] + 4;
    x2 = KZOFFX.vx + (*crd2)[0] - 4;
    x3 = KZOFFX.vx + (*crd3)[0] + 4;
    y0 = KZOFFY.vx + (*crd0)[1] - 4;
    y1 = KZOFFY.vx + (*crd1)[1] - 4;
    y2 = KZOFFY.vx + (*crd2)[1] + 4;
    y3 = KZOFFY.vx + (*crd3)[1] + 4;
    u0 = (uv0->vx << 4) + 4;
    u1 = (uv1->vx << 4) - 4;
    u2 = (uv2->vx << 4) + 4;
    u3 = (uv3->vx << 4) - 4;
    v0 = (uv0->vy << 4) + 4;
    v1 = (uv1->vy << 4) + 4;
    v2 = (uv2->vy << 4) - 4;
    v3 = (uv3->vy << 4) - 4;

    pp->DMAtag = 0x70000000 + (sizeof(GS_POLY_FT4_UV) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 11);
    pp->GIFtag_regs = \
        (u64)SCE_GS_XYZ2  << 40 | (u64)SCE_GS_UV     << 36 | \
        (u64)SCE_GS_XYZ2  << 32 | (u64)SCE_GS_UV     << 28 | \
        (u64)SCE_GS_XYZ2  << 24 | (u64)SCE_GS_UV     << 20 | \
        (u64)SCE_GS_XYZ2  << 16 | (u64)SCE_GS_UV     << 12 | \
        (u64)SCE_GS_RGBAQ << 8  | (u64)SCE_GS_TEX0_2 << 4 | (u64)SCE_GS_PRIM;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 0, 1, 0, 1, 0, 1, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y1;
    pp->x2 = x2;
    pp->y2 = y2;
    pp->x3 = x3;
    pp->y3 = y3;
    pp->z0 = (*crd0)[2];
    pp->z1 = (*crd1)[2];
    pp->z2 = (*crd2)[2];
    pp->z3 = (*crd3)[2];
    pp->u0 = u0;
    pp->v0 = v0;
    pp->u1 = u1;
    pp->v1 = v1;
    pp->u2 = u2;
    pp->v2 = v2;
    pp->u3 = u3;
    pp->v3 = v3;
    pp->r0 = col->r;
    pp->g0 = col->g;
    pp->b0 = col->b;
    pp->a0 = col->a;
    pp->q0 = 1.0f;
    pp->tex0 = kz_tex0;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_PolyFT4UV_NC(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, DVECTOR *uv0, DVECTOR *uv1, DVECTOR *uv2, DVECTOR *uv3, KZCOLOR *col) {
    GS_POLY_FT4_UV *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
    u16 x3;
    u16 y3;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
    u16 u2;
    u16 v2;
    u16 u3;
    u16 v3;

    pp = (GS_POLY_FT4_UV *)0x70000000;
    x0 = KZOFFX.vx + (*crd0)[0];
    x1 = KZOFFX.vx + (*crd1)[0];
    x2 = KZOFFX.vx + (*crd2)[0];
    x3 = KZOFFX.vx + (*crd3)[0];
    y0 = KZOFFY.vx + (*crd0)[1];
    y1 = KZOFFY.vx + (*crd1)[1];
    y2 = KZOFFY.vx + (*crd2)[1];
    y3 = KZOFFY.vx + (*crd3)[1];
    u0 = (uv0->vx << 4) + 4;
    u1 = (uv1->vx << 4) - 4;
    u2 = (uv2->vx << 4) + 4;
    u3 = (uv3->vx << 4) - 4;
    v0 = (uv0->vy << 4) + 4;
    v1 = (uv1->vy << 4) + 4;
    v2 = (uv2->vy << 4) - 4;
    v3 = (uv3->vy << 4) - 4;

    pp->DMAtag = 0x70000000 + (sizeof(GS_POLY_FT4_UV) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 11);
    pp->GIFtag_regs = \
        (u64)SCE_GS_XYZ2  << 40 | (u64)SCE_GS_UV     << 36 | \
        (u64)SCE_GS_XYZ2  << 32 | (u64)SCE_GS_UV     << 28 | \
        (u64)SCE_GS_XYZ2  << 24 | (u64)SCE_GS_UV     << 20 | \
        (u64)SCE_GS_XYZ2  << 16 | (u64)SCE_GS_UV     << 12 | \
        (u64)SCE_GS_RGBAQ << 8  | (u64)SCE_GS_TEX0_2 << 4 | (u64)SCE_GS_PRIM;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 0, 1, 0, 1, 0, 1, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y1;
    pp->x2 = x2;
    pp->y2 = y2;
    pp->x3 = x3;
    pp->y3 = y3;
    pp->z0 = (*crd0)[2];
    pp->z1 = (*crd1)[2];
    pp->z2 = (*crd2)[2];
    pp->z3 = (*crd3)[2];
    pp->u0 = u0;
    pp->v0 = v0;
    pp->u1 = u1;
    pp->v1 = v1;
    pp->u2 = u2;
    pp->v2 = v2;
    pp->u3 = u3;
    pp->v3 = v3;
    pp->r0 = col->r;
    pp->g0 = col->g;
    pp->b0 = col->b;
    pp->a0 = col->a;
    pp->q0 = 1.0f;
    pp->tex0 = kz_tex0;

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_PolyGT4UV(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, DVECTOR *uv0, DVECTOR *uv1, DVECTOR *uv2, DVECTOR *uv3, KZCOLOR *col0, KZCOLOR *col1, KZCOLOR *col2, KZCOLOR *col3) {
    GS_POLY_GT4_UV *pp;

    u16 x0;
    u16 y0;
    u16 x1;
    u16 y1;
    u16 x2;
    u16 y2;
    u16 x3;
    u16 y3;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
    u16 u2;
    u16 v2;
    u16 u3;
    u16 v3;

    pp = (GS_POLY_GT4_UV *)0x70000000;
    x0 = KZOFFX.vx + (*crd0)[0] - 4;
    x1 = KZOFFX.vx + (*crd1)[0] + 4;
    x2 = KZOFFX.vx + (*crd2)[0] - 4;
    x3 = KZOFFX.vx + (*crd3)[0] + 4;
    y0 = KZOFFY.vx + (*crd0)[1] - 4;
    y1 = KZOFFY.vx + (*crd1)[1] - 4;
    y2 = KZOFFY.vx + (*crd2)[1] + 4;
    y3 = KZOFFY.vx + (*crd3)[1] + 4;
    u0 = (uv0->vx << 4) + 4;
    u1 = (uv1->vx << 4) - 4;
    u2 = (uv2->vx << 4) + 4;
    u3 = (uv3->vx << 4) - 4;
    v0 = (uv0->vy << 4) + 4;
    v1 = (uv1->vy << 4) + 4;
    v2 = (uv2->vy << 4) - 4;
    v3 = (uv3->vy << 4) - 4;

    pp->DMAtag = 0x70000000 + (sizeof(GS_POLY_GT4_UV) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 14);
    pp->GIFtag_regs = \
        (u64)SCE_GS_XYZ2   << 52 | (u64)SCE_GS_RGBAQ << 48 | (u64)SCE_GS_UV << 44 | \
        (u64)SCE_GS_XYZ2   << 40 | (u64)SCE_GS_RGBAQ << 36 | (u64)SCE_GS_UV << 32 | \
        (u64)SCE_GS_XYZ2   << 28 | (u64)SCE_GS_RGBAQ << 24 | (u64)SCE_GS_UV << 20 | \
        (u64)SCE_GS_XYZ2   << 16 | (u64)SCE_GS_RGBAQ << 12 | (u64)SCE_GS_UV << 8 | \
        (u64)SCE_GS_TEX0_2 << 4  | (u64)SCE_GS_PRIM;
    pp->prim = SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0);

    pp->x0 = x0;
    pp->y0 = y0;
    pp->x1 = x1;
    pp->y1 = y1;
    pp->x2 = x2;
    pp->y2 = y2;
    pp->x3 = x3;
    pp->y3 = y3;
    pp->z0 = (*crd0)[2];
    pp->z1 = (*crd1)[2];
    pp->z2 = (*crd2)[2];
    pp->z3 = (*crd3)[2];
    pp->u0 = u0;
    pp->v0 = v0;
    pp->u1 = u1;
    pp->v1 = v1;
    pp->u2 = u2;
    pp->v2 = v2;
    pp->u3 = u3;
    pp->v3 = v3;
    pp->r0 = col0->r;
    pp->g0 = col0->g;
    pp->b0 = col0->b;
    pp->a0 = col0->a;
    pp->r1 = col1->r;
    pp->g1 = col1->g;
    pp->b1 = col1->b;
    pp->a1 = col1->a;
    pp->r2 = col2->r;
    pp->g2 = col2->g;
    pp->b2 = col2->b;
    pp->a2 = col2->a;
    pp->r3 = col3->r;
    pp->g3 = col3->g;
    pp->b3 = col3->b;
    pp->a3 = col3->a;
    pp->q0 = 1.0f;
    pp->q1 = 1.0f;
    pp->q2 = 1.0f;
    pp->q3 = 1.0f;
    pp->tex0 = kz_tex0;

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Alpha(u32 ctxt, u64 A, u64 B, u64 C, u64 D, u64 FIX) {
    GS_ALPHA *pp;

    pp = (GS_ALPHA *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_ALPHA) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = ctxt == 1 ? SCE_GS_ALPHA_1 : SCE_GS_ALPHA_2;
    pp->alpha = SCE_GS_SET_ALPHA(A, B, C, D, FIX);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Clamp(u32 ctxt, u64 WMS, u64 WMT, u64 MINU, u64 MAXU, u64 MINV, u64 MAXV) {
    GS_CLAMP *pp;

    pp = (GS_CLAMP *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_CLAMP) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = ctxt == 1 ? SCE_GS_CLAMP_1 : SCE_GS_CLAMP_2;
    pp->clamp = SCE_GS_SET_CLAMP(WMS, WMT, MINU, MAXU, MINV, MAXV);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Fba(u32 ctxt, u64 FBA) {
    GS_FBA *pp;

    pp = (GS_FBA *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_FBA) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = ctxt == 1 ? SCE_GS_FBA_1 : SCE_GS_FBA_2;
    pp->fba = SCE_GS_SET_FBA(FBA);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Frame(u32 ctxt, u64 FBP, u64 FBW, u64 PSM, u64 FBMSK) {
    GS_FRAME *pp;

    pp = (GS_FRAME *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_FRAME) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = ctxt == 1 ? SCE_GS_FRAME_1 : SCE_GS_FRAME_2;
    pp->frame = SCE_GS_SET_FRAME(FBP, FBW, PSM, FBMSK);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Pabe(u64 PABE) {
    GS_PABE *pp;

    pp = (GS_PABE *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_PABE) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = SCE_GS_PABE;
    pp->pabe = SCE_GS_SET_PABE(PABE);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Test(u32 ctxt, u64 ATE, u64 ATST, u64 AREF, u64 AFAIL, u64 DATE, u64 DATM, u64 ZTE, u64 ZTST) {
    GS_TEST *pp;

    pp = (GS_TEST *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_TEST) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = ctxt == 1 ? SCE_GS_TEST_1 : SCE_GS_TEST_2;
    pp->test = SCE_GS_SET_TEST(ATE, ATST, AREF, AFAIL, DATE, DATM, 1, ZTST);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Tex0(u64 TBP0, u64 TBW, u64 PSM, u64 TW, u64 TH, u64 TCC, u64 TFX, u64 CBP, u64 CPSM, u64 CSM, u64 CSA, u64 CLD) {
    kz_tex0 = SCE_GS_SET_TEX0(TBP0, TBW, PSM, TW, TH, TCC, TFX, CBP, CPSM, CSM, CSA, CLD);
    FlushCache(WRITEBACK_DCACHE);
}

void kz_Set_Tex0_Imd(u64 tex0) {
    kz_tex0 = tex0;
    FlushCache(WRITEBACK_DCACHE);
}

void kz_Set_Tex1(u32 ctxt, u64 LCM, u64 MXL, u64 MMAG, u64 MMIN, u64 MTBA, u64 L, u64 K) {
    GS_TEX1 *pp;

    pp = (GS_TEX1 *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_TEX1) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = ctxt == 1 ? SCE_GS_TEX1_1 : SCE_GS_TEX1_2;
    pp->tex1 = SCE_GS_SET_TEX1(LCM, MXL, MMAG, MMIN, MTBA, L, K);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Texa(u64 TA0, u64 AEM, u64 TA1) {
    GS_TEXA *pp;

    pp = (GS_TEXA *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_TEXA) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = SCE_GS_TEXA;
    pp->texa = SCE_GS_SET_TEXA(TA0, AEM, TA1);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Texflush() {
    GS_TEXFLUSH *pp;

    pp = (GS_TEXFLUSH *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_TEXFLUSH) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = SCE_GS_TEXFLUSH;
    pp->texflush = 1;

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Xyoffset(u32 ctxt, u64 OFX, u64 OFY) {
    GS_XYOFFSET *pp;

    pp = (GS_XYOFFSET *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_XYOFFSET) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = ctxt == 1 ? SCE_GS_XYOFFSET_1 : SCE_GS_XYOFFSET_2;
    pp->xyoffset = SCE_GS_SET_XYOFFSET(OFX, OFY);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_Zbuf(u32 ctxt, u64 ZBP, u64 PSM, u64 ZMSK) {
    GS_ZBUF *pp;

    pp = (GS_ZBUF *)0x70000000;
    pp->DMAtag = 0x70000000 + (sizeof(GS_ZBUF) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->GIFtag_regs = SCE_GIF_PACKED_AD;
    pp->addr = ctxt == 1 ? SCE_GS_ZBUF_1 : SCE_GS_ZBUF_2;
    pp->zbuf = SCE_GS_SET_ZBUF(ZBP, PSM, ZMSK);

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_DrawPixel(u64 DBP, u64 DSAX, u64 DSAY, KZCOLOR *col0, KZCOLOR *col1) {
    GS_DRAWPIXEL *pp;
    u64 col0_w;
    u64 col1_w;

    pp = (GS_DRAWPIXEL *)0x70000000;
    col0_w = (u64)*(u32 *)col0;
    col1_w = (u64)*(u32 *)col1 << 32;
    pp->DMAtag = 0x70000000 + (sizeof(GS_DRAWPIXEL) / sizeof(qword) - 1);
    pp->GIFtag = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 6);
    pp->GIFtag_regs = \
        SCE_GIF_PACKED_AD << 24 | SCE_GIF_PACKED_AD << 20 | SCE_GIF_PACKED_AD << 16 | \
        SCE_GIF_PACKED_AD << 12 | SCE_GIF_PACKED_AD << 8  | SCE_GIF_PACKED_AD << 4  | SCE_GIF_PACKED_AD;

    pp->bitbltbuf_addr = SCE_GS_BITBLTBUF;
    pp->trxpos_addr = SCE_GS_TRXPOS;
    pp->trxreg_addr = SCE_GS_TRXREG;
    pp->trxdir_addr = SCE_GS_TRXDIR;
    pp->hwreg_addr = SCE_GS_HWREG;
    pp->texflush = 0;
    pp->texflush_addr = SCE_GS_TEXFLUSH;

    pp->bitbltbuf = SCE_GS_SET_BITBLTBUF(0, 1, 0, DBP, 1, 0);
    pp->trxpos = SCE_GS_SET_TRXPOS(0, 0, DSAX, DSAY, 0);
    pp->trxreg = SCE_GS_SET_TRXREG(2, 1);
    pp->trxdir = SCE_GS_SET_TRXDIR(0);
    pp->hwreg = col0_w | col1_w;

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pp | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void kz_Set_FogDist(s32 near, s32 far) {
    KVM_FOGDIST *pp;
    f32 f_near;
    f32 f_far;

    pp = (KVM_FOGDIST *)p1_packet;
    pp->DMAtag[0] = 0x10000000 | (sizeof(KVM_FOGDIST) / sizeof(qword) - 1);
    pp->DMAtag[1] = pp->DMAtag[2] = pp->DMAtag[3] = 0;
    pp->VIFtag0[0] = pp->VIFtag0[1] = pp->VIFtag0[2] = 0;
    pp->VIFtag0[3] = SCE_VIF1_SET_UNPACK(0x394, 1, 0xC, 0);
    pp->foga[0] = pp->foga[1] = pp->foga[2] = 0;
    if (near != far) {
        f_near = (f32)near;
        f_far = (f32)far;
        pp->foga[3] = (f_near * 255.0f) / (f_near - f_far);
    } else {
        pp->foga[3] = 0;
    }
    pp->VIFtag1[0] = pp->VIFtag1[1] = pp->VIFtag1[2] = 0;
    pp->VIFtag1[3] = SCE_VIF1_SET_UNPACK(0x395, 1, 0xC, 0);
    pp->fogb[0] = pp->fogb[1] = pp->fogb[2] = 0;
    if (near != far) {
        f_near = (f32)near;
        f_far = (f32)far;
        pp->fogb[3] = -(f_far * 255.0f * f_near) / (f_near - f_far);
    } else {
        pp->fogb[3] = 0;
    }
    pp++;
    p1_packet = (qword *)pp;
}

void kz_Set_FogCol(sceVu0IVECTOR col) {
    KGS_FOGCOL *pp;

    pp = (KGS_FOGCOL *)p1_packet;
    pp->DMAtag[0] = 0x10000000 | (sizeof(KGS_FOGCOL) / sizeof(qword) - 1);
    pp->DMAtag[1] = pp->DMAtag[2] = pp->DMAtag[3] = 0;
    pp->VIFtag[0] = pp->VIFtag[1] = pp->VIFtag[2] = 0;
    pp->VIFtag[3] = SCE_VIF1_SET_DIRECT(2, 0);
    pp->GIFtag[0] = 0x8001;
    pp->GIFtag[1] = 0x10000000;
    pp->GIFtag[2] = SCE_GIF_PACKED_AD;
    pp->GIFtag[3] = 0;
    pp->GSreg = SCE_GS_FOGCOL;
    pp->fogcol = (col[2] << 16) + (col[1] << 8) + col[0];
    pp++;
    p1_packet = (qword *)pp;
}

void kz_Set_Box(DVECTOR *sxy, u32 sz, DVECTOR *swh, s32 thick, KZCOLOR *col) {
    static DVECTOR sxy0;
    static DVECTOR sxy1;
    static DVECTOR sxy2;
    static DVECTOR swh0;
    static DVECTOR swh1;

    sxy0.vx = sxy2.vx = sxy->vx;
    sxy1.vx = (sxy->vx + swh->vx) - thick;
    sxy0.vy = sxy1.vy = sxy->vy;
    sxy2.vy = (sxy->vy + swh->vy) - thick;
    swh0.vx = swh->vx;
    swh1.vy = swh->vy;
    swh0.vy = swh1.vx = thick;
    kz_Set_TileF4(&sxy0, sz, &swh0, col);
    kz_Set_TileF4(&sxy0, sz, &swh1, col);
    kz_Set_TileF4(&sxy1, sz, &swh1, col);
    kz_Set_TileF4(&sxy2, sz, &swh0, col);
}

void kz_Set_Box_VGrad(DVECTOR *sxy, u32 sz, DVECTOR *swh, s32 thick, KZCOLOR *col0,KZCOLOR *col1) {
    static DVECTOR sxy0;
    static DVECTOR sxy1;
    static DVECTOR sxy2;
    static DVECTOR swh0;
    static DVECTOR swh1;

    sxy0.vx = sxy2.vx = sxy->vx;
    sxy1.vx = (sxy->vx + swh->vx) - thick;
    sxy0.vy = sxy1.vy = sxy->vy;
    sxy2.vy = (sxy->vy + swh->vy) - thick;
    swh0.vx = swh->vx;
    swh1.vy = swh->vy;
    swh0.vy = swh1.vx = thick;
    kz_Set_TileF4(&sxy0, sz, &swh0, col0);
    kz_Set_TileG4(&sxy0, sz, &swh1, col0, col0, col1, col1);
    kz_Set_TileG4(&sxy1, sz, &swh1, col0, col0, col1, col1);
    kz_Set_TileF4(&sxy2, sz, &swh0, col1);
}

void kz_Set_Box_HGrad(DVECTOR *sxy, u32 sz, DVECTOR *swh, s32 thick, KZCOLOR *col0, KZCOLOR *col1) {
    static DVECTOR sxy0;
    static DVECTOR sxy1;
    static DVECTOR sxy2;
    static DVECTOR swh0;
    static DVECTOR swh1;

    sxy0.vx = sxy2.vx = sxy->vx;
    sxy1.vx = (sxy->vx + swh->vx) - thick;
    sxy0.vy = sxy1.vy = sxy->vy;
    sxy2.vy = (sxy->vy + swh->vy) - thick;
    swh0.vx = swh->vx;
    swh1.vy = swh->vy;
    swh0.vy = swh1.vx = thick;
    kz_Set_TileG4(&sxy0, sz, &swh0, col0, col1, col0, col1);
    kz_Set_TileF4(&sxy0, sz, &swh1, col0);
    kz_Set_TileF4(&sxy1, sz, &swh1, col1);
    kz_Set_TileG4(&sxy2, sz, &swh0, col0, col1, col0, col1);
}

s32 kz_Get_KazuyaStageNo() {
    s32 vision = GameGbl.vision >> 8;
    switch (vision) {
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
            return vision;
        default:
            return -1;
    }
}

s32 kz_Get_Cont_NormalStageNo() {
    s32 vision = GameGbl.vision >> 8;
    switch (vision) {
        case 1:
            return STAGE_01;
        case 2:
            return STAGE_02;
        case 4:
            return STAGE_04;
        case 5:
            return STAGE_05;
        case 6:
            return STAGE_06;
        case 7:
            return STAGE_07;
        case 8:
            return STAGE_08;
        case 9:
            return STAGE_09;
        case 10:
            return STAGE_10;
        case 11:
            return STAGE_11;
        case 12:
            return STAGE_12;
        case 13:
            return STAGE_13;
        case 14:
            return STAGE_14;
        case 15:
            return STAGE_15;
        case 16:
            return STAGE_16;
        case 17:
            return STAGE_17;
        case 18:
            return STAGE_EX1;
        case 19:
            return STAGE_EX2;
        default:
            return -1;
    }
}

s32 kz_Get_Cont_BossStageNo() {
    s32 vision = GameGbl.vision >> 8;
    switch (vision) {
        case 21:
            return BOSS_1;
        case 22:
            return BOSS_2;
        case 23:
            return BOSS_3;
        case 24:
            return BOSS_4;
        case 26:
            return BOSS_5;
        case 27:
            return BOSS_6;
        default:
            return -1;
    }
}

s32 kz_Get_AreaNo() {
    return GameGbl.vision & 0xFF;
}

s32 kz_Get_StageMode() {
    s32 vision = GameGbl.vision >> 8;
    if (vision > 0 && vision < 20) {
        return STG_MODE_GAME;
    } else if (vision > 20 && vision < 29) {
        return STG_MODE_BOSS;
    } else {
        switch (vision) {
            case 33:
                return STG_MODE_SROLL;
            case 30:
            case 31:
            case 32:
            case 34:
            case 35:
                return STG_MODE_SELECT;
            default:
                return STG_MODE_OTHER;
        }
    }
}

s32 kz_Get_RetryVision() {
    s32 vision = GameGbl.vision >> 8;
    switch (vision) {
        case 1:
            return 0x100;
        case 2:
            return 0x200;
        case 4:
            return 0x400;
        case 5:
            return 0x500;
        case 6:
            return 0x600;
        case 7:
            return 0x700;
        case 8:
            return 0x800;
        case 9:
            return 0x908;
        case 0xA:
            return 0xA00;
        case 0xB:
            return 0xB00;
        case 0xC:
            return 0xC00;
        case 0xD:
            return 0xD00;
        case 0xE:
            return 0xE00;
        case 0xF:
            return 0xF02;
        case 0x10:
            return 0x1000;
        case 0x11:
            return 0x1100;
        case 0x12:
            return 0x1200;
        case 0x13:
            return 0x1300;
        case 0x15:
            return 0x1500;
        case 0x16:
            return 0x1601;
        case 0x17:
            return 0x1700;
        case 0x18:
            return 0x1800;
        case 0x1A:
            return 0x1A00;
        case 0x1B:
            return 0x1B00;
        default:
            return 0x100;
    }
}

static inline s32 _check_clear(u64 flag) {
    return gamdat.clear & flag ? 1 : 0;
}
#define check_clear(flag) (gamdat.clear & (u64)flag ? 1 : 0);
s32 kz_Check_Clear_CurrentStage() {
    switch (GameGbl.vision >> 8) {
        case 1:
            return _check_clear(0x1);
        case 2:
            return check_clear(0x4);
        case 4:
            return check_clear(0x40);
        case 5:
            return check_clear(0x80);
        case 6:
            return check_clear(0x800);
        case 7:
            return check_clear(0x1000);
        case 8:
            return check_clear(0x4000);
        case 9:
            return check_clear(0x10000);
        case 10:
            return check_clear(0x20000);
        case 11:
            return check_clear(0x40000);
        case 12:
            return check_clear(0x200000);
        case 13:
            return check_clear(0x400000);
        case 14:
            return check_clear(0x800000);
        case 15:
            return check_clear(0x2000000);
        case 16:
            return check_clear(0x4000000);
        case 17:
            return check_clear(0x8000000);
        case 18:
            return check_clear(0x20000000);
        case 19:
            return check_clear(0x40000000);
        case 21:
            return check_clear(0x8);
        case 22:
            return check_clear(0x100);
        case 23:
            return check_clear(0x2000);
        case 24:
            return check_clear(0x80000);
        case 26:
            return check_clear(0x1000000);
        case 27:
            return check_clear(0x10000000);
        default:
            return 0;
    }
}

s32 kz_Get_PlaceStrNo() {
    s32 point = gamdat.point;
    POINT_INFO *info = kzWM_Get_PointInfo(point);
    s32 idx = 0;

    switch (point) {
        case POINT_STG_01:
            if ((gamdat.clear & 0x600000) == 0x200000) {
                idx = 1;
            }
            break;
        case POINT_STG_02:
            if ((gamdat.clear & 0x300000) == 0x100000) {
                idx = 1;
            }
            break;
        case POINT_STG_07:
            if ((gamdat.clear & 0x6000) == 0x2000) {
                idx = 1;
            }
            break;
    }

    return info->place[idx];
}

s32 kz_Check_GenjuTotalTime() {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (gamdat.genju_time[i] == 0) {
            return 0;
        }
    }

    return 1;
}

s32 kz_Get_FinishedDolls() {
    s32 dolls = 0;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (gamdat.stg_mmd[i] >= 6) {
            dolls++;
        }
    }

    return dolls;
}

s32 kz_Get_YumeCompleteStages() {
    s32 stages = 0;
    s32 i;

    for (i = 0; i < 18; i++) {
        if (gamdat.stg_yume[i] >= 150) {
            stages++;
        }
    }

    return stages;
}

void kz_Set_KL2System_Light(s32 light_no) {
    LgtWorkF lgt;

    nkGetLgtWorkF(&lgt, light_no);
    nkCalcMatLgtWork(kz_normal_light, kz_light_color, &lgt, NULL);
}

BGM_LIST_INFO* kz_Get_Ptr_BgmListInfo() {
    return BgmLists[bgm_list_no];
}

BGM_INFO* kz_Get_Ptr_BgmInfo(s32 index) {
    return kz_Get_Ptr_BgmListInfo()->list[index];
}

s32 kz_Get_BgmIndex_Prev() {
    s32 list_no = kzGetBGMListNumber();
    s32 prev = bgm_index - 1;
    if (prev < 0) {
        prev = list_no - 1;
    }
    return prev;
}

s32 kz_Get_BgmIndex_Next() {
    s32 list_no = kzGetBGMListNumber();
    s32 next = bgm_index + 1;
    if (next >= list_no) {
        next = 0;
    }
    return next;
}

s32 kz_Get_BgmNameStr(s32 index) {
    return kz_Get_Ptr_BgmInfo(index)->name;
}

void kz_Init_SoundEffect(s32 se_no) {
    SE_INFO *se = SoundEffects[se_no];
    if (se->flag != 0) {
        hSeKeyOff(se->obj_no);
    }
    se->flag = 0;
}

void kz_KeyOn_SoundEffect(s32 se_no) {
    SE_INFO *se = SoundEffects[se_no];
    s32 obj_no = hSeKeyOn(se->se_no, NULL, 0);
    hSeSetObjPan(obj_no, se->panl, se->panr);
    if (se->flag != 0) {
        hSeKeyOff(se->obj_no);
    }
    se->obj_no = obj_no;
    se->flag = 1;
}

void kz_Load_CompGms(s32 gms_no) {
    HLZDecode(D_3509A0[gms_no], D_3FAD70, (HLZWork *)0x70000000);
    nkLoadGms((qword *)D_3FAD70);
    sceGsSyncPath(0, 0);
}

f32 kz_Get_Coef_Intpl(s32 iNow, s32 iStart, s32 iEnd) {
    f32 fNow, fStart, fEnd;

    if (iNow <= iStart) {
        return 0.0f;
    }
    if (iNow >= iEnd || iEnd == iStart) {
        return 1.0f;
    }

    fNow = iNow;
    fStart = iStart;
    fEnd = iEnd;
    return (fNow - fStart) / (fEnd - fStart);
}

void kz_Intpl_Color(f32 coef, KZCOLOR *dst, KZCOLOR *src0, KZCOLOR *src1) {
    dst->r = src1->r * coef + src0->r * (1.0f - coef);
    dst->g = src1->g * coef + src0->g * (1.0f - coef);
    dst->b = src1->b * coef + src0->b * (1.0f - coef);
    dst->a = src1->a * coef + src0->a * (1.0f - coef);
}

void func_00173BA0(void *out, s32 size, char *filename) {
    char name[256];
    s32 fp;

    if (out == NULL) {
        s32 *addr = (s32 *)hGetSysDataAddr(7);
        if (addr == NULL) {
            return;
        }
        out = (void *)((u32)addr + addr[1]);
    }

    sprintf(name, "host0:./%s.bin", filename);
    fp = sceOpen(name, SCE_CREAT | SCE_TRUNC | SCE_WRONLY);
    if (fp > -1) {
        sceWrite(fp, out, size);
        sceClose(fp);
    }
}

