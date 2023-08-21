#include "harada/hr_pefc.h"
#include "harada/hr_main.h"
#include "harada/hr_pall.h"
#include "nakano/main.h"

void hr_pfade_init(HR_FADE *fade) {
    memset(fade, 0, sizeof(HR_FADE));
    hr_abeoff = 0;
}

void hr_pfade_sinit() {
    hr_pfade_init(&ppsys.fade);
}

void hr_pfade_set(HR_FADE *fade, u8 r, u8 g, u8 b, u8 a) {
    fade->r = r;
    fade->g = g;
    fade->b = b;
    fade->a = fade->ma = a;
    fade->flag |= 1;
}


void hr_pfade_in(HR_FADE *fade, u8 r, u8 g, u8 b, u8 a, u16 time) {
    fade->r = r;
    fade->g = g;
    fade->b = b;
    fade->a = fade->ma = a;
    fade->time = fade->count = time;
    fade->flag |= 3;
}

void hr_pfade_out(HR_FADE *fade, u8 r, u8 g, u8 b, u8 a, u16 time) {
    fade->r = r;
    fade->g = g;
    fade->b = b;
    fade->a = 0;
    fade->ma = a;
    fade->time = time;
    fade->count = 0;
    fade->flag |= 5;
}

void hr_pfade_x(HR_FADE *fade, u16 time) {
    fade->r = fade->g = fade->b = 128;
    fade->a = fade->ma = 128;
    fade->time = fade->count = time;
    fade->flag |= 0x21;
    hr_abeoff = 1;
}

static void hr_pfade_alpha(HR_FADE *fade) {
    fade->a = (fade->ma * fade->count) / fade->time;
}

void hr_pfade_work(HR_FADE *fade) {
    if (!(fade->flag & 1)) {
        return;
    }

    if (fade->flag & 0x12) {
        fade->count--;
        if (fade->count == 0) {
            hr_pfade_init(fade);
        } else {
            hr_pfade_alpha(fade);
        }
    } else if (fade->flag & 4) {
        if (fade->time > fade->count) {
            fade->count++;
            hr_pfade_alpha(fade);
        }
    }
}

void hr_pfade_drawN(HR_FADE *fade) {
    ATR_MWAKU *pp;

    pp = (ATR_MWAKU *)0x70000000;
    pp->dmatag[0] = 0x70000000 | (sizeof(ATR_MWAKU) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;

    ((u64 *)pp->gif0)[0] = SCE_GIF_SET_TAG(4, 0, 0, 0, 0, 1);
    pp->gif0[2] = SCE_GIF_PACKED_AD;
    pp->gif0[3] = 0;
    ((u64 *)pp->test)[0] = SCE_GS_SET_TEST_2(1, 1, 0, 0, 0, 0, 1, 2);
    ((u64 *)pp->test)[1] = SCE_GS_TEST_2;
    if (fade->flag & 8) {
        ((u64 *)pp->alpha)[0] = SCE_GS_SET_ALPHA_2(0, 2, 0, 1, 0);
    } else {
        if (fade->flag & 0x10) {
            ((u64 *)pp->alpha)[0] = SCE_GS_SET_ALPHA_2(0, 1, 2, 1, fade->a);
        } else {
            ((u64 *)pp->alpha)[0] = SCE_GS_SET_ALPHA_2(0, 1, 0, 1, 0);
        }
    }
    ((u64 *)pp->alpha)[1] = SCE_GS_ALPHA_2;
    ((u64 *)pp->xyofs)[0] = *(u64 *)&(((sceGsDrawEnv2 *)hr_get_draw2())->xyoffset2);
    ((u64 *)pp->xyofs)[1] = SCE_GS_XYOFFSET_2;
    ((u64 *)pp->tex0)[0] = SCE_GS_SET_TEX0_2(0x1680, 10, SCE_GS_PSMCT32, 10, 8, 1, 0, 0, 0, 0, 0, 0);
    ((u64 *)pp->tex0)[1] = SCE_GS_TEX0_2;

    if (!(fade->flag & 0x10)) {
        ((u64 *)pp->gif1)[0] = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 1, 0, 1, 1, 0), 0, 3);
    } else {
        ((u64 *)pp->gif1)[0] = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    }
    pp->gif1[2] = SCE_GS_RGBAQ << 0 | SCE_GS_UV << 4 | SCE_GS_XYZ2 << 8;
    pp->gif1[3] = 0;
    pp->rgb0[0] = fade->r;
    pp->rgb0[1] = fade->g;
    pp->rgb0[2] = fade->b;
    pp->uv0[0] = pp->uv0[1] = 0;
    pp->xyz0[0] = 0x6C00;
    pp->xyz0[1] = 0x7900;
    pp->xyz0[2] = -1;
    pp->xyz0[3] = 0;
    pp->rgb1[0] = fade->r;
    pp->rgb1[1] = fade->g;
    pp->rgb1[2] = fade->b;
    if (fade->flag & 0x10) {
        pp->rgb0[3] = pp->rgb1[3] = 128;
    } else {
        pp->rgb0[3] = pp->rgb1[3] = fade->a;
    }
    pp->uv1[0] = 0x2800;
    pp->uv1[1] = 0x0E00;
    pp->xyz1[0] = pp->xyz0[0] + 0x2800;
    pp->xyz1[1] = pp->xyz0[1] + 0x0E00;
    pp->xyz1[2] = -1;
    pp->xyz1[3] = 0;

    sceGsSyncPath(0, 0);
    *((u16 *)&DmaChGIF->chcr) = 4;
    sceDmaSend(DmaChGIF, (u32)pp | 0x80000000);
    sceDmaSync(DmaChGIF, 0, 0);
}

void hr_pfade_drawX(/* a0 4 */ HR_FADE *fade) {
    /* s0 16 */ ATR_VTOV *pp;
    /* v0 2 */ u32 fr;

    pp = (ATR_VTOV *)0x70000000;
    pp->dmatag[0] = 0x70000000 | (sizeof(ATR_VTOV) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = 0;
    pp->dmatag[3] = 0;

    ((u64 *)pp->gif0)[0] = SCE_GIF_SET_TAG(4, 1, 0, 0, 0, 1);
    pp->gif0[2] = SCE_GIF_PACKED_AD;
    pp->gif0[3] = 0;
    fr = *((u8 *)&((sceGsDrawEnv2 *)hr_get_draw2())->frame2) << 5;
    ((u64 *)pp->bitblt)[0] = SCE_GS_SET_BITBLTBUF(fr, 10, 0, 0x1680, 10, 0);
    ((u64 *)pp->bitblt)[1] = SCE_GS_BITBLTBUF;
    ((u64 *)pp->trxpos)[0] = 0;
    ((u64 *)pp->trxpos)[1] = SCE_GS_TRXPOS;
    ((u64 *)pp->trxreg)[0] = SCE_GS_SET_TRXREG(640, 224);
    ((u64 *)pp->trxreg)[1] = SCE_GS_TRXREG;
    ((u64 *)pp->trxdir)[0] = SCE_GS_SET_TRXDIR(2);
    ((u64 *)pp->trxdir)[1] = SCE_GS_TRXDIR;

    sceGsSyncPath(0, 0);
    *((u16 *)&DmaChGIF->chcr) = 4;
    sceDmaSend(DmaChGIF, (u32)pp | 0x80000000);
    sceDmaSync(DmaChGIF, 0, 0);
}

void hr_pfade_draw(HR_FADE *fade) {
    if (fade->flag == 0) {
        return;
    }

    if (!(fade->flag & 0x20)) {
        hr_pfade_drawN(fade);
    } else {
        hr_pfade_drawX(fade);
        fade->flag &= -0x21;
        fade->flag |= 0x10;
    }
}

static void hr_skip_drawM(s32 count) {
    ATR_MSKIP *pp;
    s32 x0;
    s32 x1;
    s32 x2;

    pp = (ATR_MSKIP *)0x70000000;
    count = 80 - count;
    x0 = (count * 0x2A0) / 80;
    x1 = x0 - 32;
    if (x1 < 0) {
        x2 = x1;
    } else {
        x2 = 0;
    }

    pp->dmatag[0] = 0x70000000 | (sizeof(ATR_MSKIP) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;

    ((u64 *)pp->gif0)[0] = SCE_GIF_SET_TAG(3, 0, 0, 0, 0, 1);
    pp->gif0[2] = SCE_GIF_PACKED_AD;
    pp->gif0[3] = 0;
    ((u64 *)pp->test)[0] = SCE_GS_SET_TEST_2(1, 1, 0, 0, 0, 0, 1, 2);
    ((u64 *)pp->test)[1] = SCE_GS_TEST_2;
    ((u64 *)pp->alpha)[0] = SCE_GS_SET_ALPHA_2(0, 1, 0, 1, 0);
    ((u64 *)pp->alpha)[1] = SCE_GS_ALPHA_2;
    ((u64 *)pp->xyofs)[0] = *(u64 *)&(((sceGsDrawEnv2 *)hr_get_draw2())->xyoffset2);
    ((u64 *)pp->xyofs)[1] = SCE_GS_XYOFFSET_2;

    ((u64 *)pp->gif1)[0] = SCE_GIF_SET_TAG(6, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 1, 0, 0, 1, 0), 0, 2);
    pp->gif1[2] = SCE_GS_RGBAQ << 0 | SCE_GS_XYZ2 << 4;
    pp->gif1[3] = 0;

    x0 = (x0 << 4) + 0x6C00;
    x1 = (x1 << 4) + 0x6C00;
    x2 = (x2 << 4) + 0x6C00;

    pp->rgb0[0] = pp->rgb0[1] = pp->rgb0[2] = pp->rgb0[3] = 0;
    pp->xyz0[0] = x0;
    pp->xyz0[1] = 0x7900;
    pp->xyz0[2] = -1;
    pp->xyz0[3] = 0;

    pp->rgb1[0] = pp->rgb1[1] = pp->rgb1[2] = pp->rgb1[3] = 0;
    pp->xyz1[0] = x0;
    pp->xyz1[1] = 0x8700;
    pp->xyz1[2] = -1;
    pp->xyz1[3] = 0;

    pp->rgb2[0] = pp->rgb2[1] = pp->rgb2[2] = 0;
    pp->rgb2[3] = 128;
    pp->xyz2[0] = x1;
    pp->xyz2[1] = 0x7900;
    pp->xyz2[2] = -1;
    pp->xyz2[3] = 0;

    pp->rgb3[0] = pp->rgb3[1] = pp->rgb3[2] = 0;
    pp->rgb3[3] = 128;
    pp->xyz3[0] = x1;
    pp->xyz3[1] = 0x8700;
    pp->xyz3[2] = -1;
    pp->xyz3[3] = 0;

    pp->rgb4[0] = pp->rgb4[1] = pp->rgb4[2] = 0;
    pp->rgb4[3] = 128;
    pp->xyz4[0] = x2;
    pp->xyz4[1] = 0x7900;
    pp->xyz4[2] = -1;
    pp->xyz4[3] = 0;

    pp->rgb5[0] = pp->rgb5[1] = pp->rgb5[2] = 0;
    pp->rgb5[3] = 128;
    pp->xyz5[0] = x2;
    pp->xyz5[1] = 0x8700;
    pp->xyz5[2] = -1;
    pp->xyz5[3] = 0;

    sceGsSyncPath(0, 0);
    *((u16 *)&DmaChGIF->chcr) = 4;
    sceDmaSend(DmaChGIF, (u32)pp | 0x80000000);
    sceDmaSync(DmaChGIF, 0, 0);
}

void hr_pt_skipdraw(HR_PSYS *ps) {
    HR_FADE fade;

    if (hrpt_flag & 8) {
        if (hrpt_patch & 8) {
            hrpt_patch &= -9;
        } else {
            hrpt_flag &= -9;
        }

        if (ps->flag & 0x800) {
            memset(&fade, 0, sizeof(HR_FADE));
            hr_pfade_set(&fade, ps->r, ps->g, ps->b, ps->a);
            hr_pfade_drawN(&fade);
        } else {
            hr_skip_drawM(0);
        }
    } else {
        if (ps->flag & 0x20 && ps->skipcnt != 0) {
            ps->skipcnt--;
            hr_skip_drawM(ps->skipcnt);
        }
    }
}
