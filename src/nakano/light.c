#include "nakano/light.h"
#include "nakano/nkpad.h"
#include "nakano/nk2pcon.h"
#include "nakano/gssub.h"
#include "nakano/vu0mac.h"
#include "nakano/main.h"
#include "nakano/route.h"

static _nkLightData nkLgt0100_00 = {
    {
        { -7, 16 },
        { 112, -48 },
        { 0, -89 }
    },
    {
        { 48, 52, 30, 0 },
        {  9, 10,  3, 0 },
        {  0,  0,  0, 0 }
    },
    { 28, 35, 48, 0 }
};
static _nkLightData nkLgt0200_00 = {
    {
        { -46, 31 },
        { 128,  0 },
        { 0, -89 }
    },
    {
        { 121, 112, 100, 0 },
        {  37,  51,  50, 0 },
        {   0,   0,   0, 0 }
    },
    { 64, 64, 64, 0 }
};
static _nkLightData nkLgt0201_00 = {
    {
        { -98, 145 },
        { -94, -59 },
        {   0,   0 }
    },
    {
        { 27,  48,  48, 0 },
        { 50, 109, 105, 0 },
        {  0,   0,   0, 0 }
    },
    { 51, 95, 66, 0 }
};
static _nkLightData nkLgt0400_00 = {
    {
        { 69, 112 },
        { 92,   0 },
        { 0,  -89 }
    },
    {
        { 135, 112, 60, 0 },
        {  12,  54, 55, 0 },
        {  89,  36, 19, 0 }
    },
    { 32, 32, 32, 0 }
};
static _nkLightData nkLgt0700_00 = {
    {
        { -58, -90 },
        { -79,  86 },
        {   0,   0 }
    },
    {
        { 31, 44, 100, 0 },
        { 32, 43,  45, 0 },
        {  0,  0,   0, 0 }
    },
    { 39, 41, 57, 0 }
};
static _nkLightData nkLgt0701_00 = {
    {
        { -38, 17 },
        { 155,  0 },
        {   0,  0 }
    },
    {
        { 117, 83, 58, 0 },
        {  12, 22, 23, 0 },
        {   0,  0,  0, 0 }
    },
    { 64, 64, 64, 0 }
};
static _nkLightData nkLgt0703_01 = {
    {
        { 66,   16 },
        { 73, -125 },
        {  0,    0 }
    },
    {
        { 113, 85, 58, 0 },
        {  22,  9,  0, 0 },
        {   0,  0,  0, 0 }
    },
    { 64, 64, 64, 0 }
};
static _nkLightData nkLgt1100_00 = {
    {
        { 75,   85 },
        { 128, -89 },
        {   0,   0 }
    },
    {
        { 102, 127, 131, 0 },
        {  53,  39,  23, 0 },
        {   0,  0,  0,   0 }
    },
    { 37, 38, 42, 0 }
};
static nkLgtMNG *lgtMng[64];
static LgtWorkDat *lgtLwd;
static sceVu0FMATRIX nkNormalLight;
static sceVu0FMATRIX nkLightColor;
static nkLightWrk nkLightTest[3];
static sceVu0FVECTOR nkAmbient;
static nkLeMenu menu;

void nkLeInit() {
    menu.csl = 0;
    menu.mode = 0;
    menu.lightno = 0;
}

static void nkLeVec(nkLightWrk *lline) {
    s32 lvl = nkGetPlvl(1);
    nkGetPtrg(1);
    nkGetPrep(1);

    if (lvl & 0x2000) {
        lline->dir.x += 0.052359868f;
    } else if (lvl & 0x8000) {
        lline->dir.x -= 0.052359868f;
    }
    if (lline->dir.x > 3.141592f) {
        lline->dir.x -= 6.283184f;
    } else if (lline->dir.x < -3.141592f) {
        lline->dir.x += 6.283184f;
    }

    if (lvl & 0x1000) {
        lline->dir.y += 0.052359868f;
    } else if (lvl & 0x4000) {
        lline->dir.y -= 0.052359868f;
    }
    if (lline->dir.y > 3.141592f) {
        lline->dir.y -= 6.283184f;
    } else if (lline->dir.y < -3.141592f) {
        lline->dir.y += 6.283184f;
    }
}

static void nkLeCol(nkLightWrk *lline) {
    s32 rep;
    s32 trg;

    nkGetPlvl(1);
    trg = nkGetPtrg(1);
    rep = nkGetPrep(1);

    if (trg & 0x8000) {
        menu.colc--;
        if (menu.colc < 0) {
            menu.colc = 0;
        }
    } else if (trg & 0x2000) {
        menu.colc++;
        if (menu.colc > 2) {
            menu.colc = 2;
        }
    }

    if (rep & 0x1000) {
        lline->col[menu.colc] += 0.00390625f;
        if (lline->col[menu.colc] >= 1.0f) {
            lline->col[menu.colc] = 0.0f;
        }
    } else if (rep & 0x4000) {
        lline->col[menu.colc] -= 0.00390625f;
        if (lline->col[menu.colc] < 0.0f) {
            lline->col[menu.colc] = 0.99609375f;
        }
    }
}

static void nkLeAmb() {
    s32 rep;
    s32 trg;

    nkGetPlvl(1);
    trg = nkGetPtrg(1);
    rep = nkGetPrep(1);

    if (trg & 0x8000) {
        menu.colc--;
        if (menu.colc < 0) {
            menu.colc = 0;
        }
    } else if (trg & 0x2000) {
        menu.colc++;
        if (menu.colc > 2) {
            menu.colc = 2;
        }
    }

    if (rep & 0x1000) {
        nkAmbient[menu.colc] += 0.00390625f;
        if (nkAmbient[menu.colc] >= 1.0f) {
            nkAmbient[menu.colc] = 0.0f;
        }
    } else if (rep & 0x4000) {
        nkAmbient[menu.colc] -= 0.00390625f;
        if (nkAmbient[menu.colc] < 0.0f) {
            nkAmbient[menu.colc] = 0.99609375f;
        }
    }
}

static void nkLeLgtNo(nkLightWrk *lline) {
    s32 rep;
    s32 trg;

    nkGetPlvl(1);
    trg = nkGetPtrg(1);
    rep = nkGetPrep(1);

    if (trg & 0x8000) {
        menu.colc--;
        if (menu.colc < 0) {
            menu.colc = 0;
        }
    } else if (trg & 0x2000) {
        menu.colc++;
        if (menu.colc > 1) {
            menu.colc = 1;
        }
    }

    if (menu.colc == 0) {
        if (rep & 0x1000) {
            menu.lightno++;
            if (menu.lightno > 2) {
                menu.lightno = 2;
            }
        } else if (rep & 0x4000) {
            menu.lightno--;
            if (menu.lightno < 0) {
                menu.lightno = 0;
            }
        }
    } else if (menu.colc == 1) {
        if (rep & 0x5000) {
            lline->on++;
            lline->on &= 1;
        }
    }
}

void nkLightMenu(nkLightWrk *lline) {
    s32 trg;

    nkGetPlvl(1);
    trg = nkGetPtrg(1);
    nkGetPrep(1);

    if (menu.mode == 0) {
        if (trg & 0x1000) {
            menu.csl--;
            if (menu.csl < 0) {
                menu.csl = 3;
            }
        } else if (trg & 0x4000) {
            menu.csl++;
            if (menu.csl > 3) {
                menu.csl = 0;
            }
        }

        if (trg & 0x20) {
            if (menu.csl == 1) {
                menu.mode = menu.csl;
            }
            if (menu.csl == 2) {
                menu.colc = 0;
                menu.mode = menu.csl;
            }
            if (menu.csl == 3) {
                menu.colc = 0;
                menu.mode = menu.csl;
            }
            if (menu.csl == 0) {
                menu.mode = 5;
            }
        }
    }
}

void nkLightEdit() {
    s32 trg;
    nkLightWrk *lline;
    sceVu0FVECTOR light;
    s32 lp;
    OBJWORK *objw;
    sceVu0FVECTOR vf0;
    sceVu0FVECTOR vf1;
    u32 col0;
    u32 col1;

    lline = &nkLightTest[menu.lightno];
    nkGetPlvl(1);
    trg = nkGetPtrg(1);
    nkGetPrep(1);
    nkLightMenu(lline);

    switch (menu.mode) {
        case 1:
            nkLeVec(lline);
            nkDrawFbBar(2, 11, 11);
            break;
        case 2:
            nkLeCol(lline);
            nkDrawFbBar(2, 12, 5);
            nkDrawFbBar(menu.colc * 5 + 10, 12, 4);
            break;
        case 3:
            nkLeAmb();
            nkDrawFbBar(2, 13, 7);
            nkDrawFbBar(menu.colc * 5 + 10, 13, 4);
            break;
        case 5:
            nkLeLgtNo(lline);
            nkDrawFbBar(2, 10, 5);
            nkDrawFbBar(menu.colc * 2 + 10, 10, 3);
            break;
    }

    if (trg & 0x80) {
        menu.mode = 0;
    }

    for (lp = 0; lp < 3; lp++) {
        lline = &nkLightTest[lp];
        light[0] = sinf(lline->dir.x);
        light[0] *= cosf(lline->dir.y);
        light[2] = cosf(lline->dir.x);
        light[2] *= cosf(lline->dir.y);
        light[1] = sinf(lline->dir.y);
        light[3] = 0;
        objw = GameGbl.klonoa;
        sceVu0CopyVector(vf0, objw->posi);

        vf0[1] -= 32.0f;
        vf0[0] -= light[0] * 32.0f;
        vf0[1] -= light[1] * 32.0f;
        vf0[2] -= light[2] * 32.0f;
        vf1[0] = vf0[0] - light[0] * 64.0f;
        vf1[1] = vf0[1] - light[1] * 64.0f;
        vf1[2] = vf0[2] - light[2] * 64.0f;

        col0 = (u32)(lline->col[0] * 256.0f) & 0xFF;
        col0 |= ((u32)(lline->col[1] * 256.0f) & 0xFF) << 8;
        col0 |= ((u32)(lline->col[2] * 256.0f) & 0xFF) << 16;
        col1 = (u32)(lline->col[0] * 128.0f) & 0xFF;
        col1 |= ((u32)(lline->col[1] * 128.0f) & 0xFF) << 8;
        col1 |= ((u32)(lline->col[2] * 128.0f) & 0xFF) << 16;
        _nkDraw3DLine(vf0, vf1, col0, col1);
    }
}

void LgtDataNone() {
    s32 i;

    for (i = 0; i < 64; i++) {
        lgtMng[i] = NULL;
    }
}

void LgtDataInit(s32 *adrs) {
    nkLgtMNG *mng;
    s32 lp1;
    s32 n;

    adrs++;
    n = *adrs;
    adrs++;
    adrs += n;
    mng = (nkLgtMNG *)adrs;
    for (lp1 = 0; lp1 < 64; lp1++) {
        lgtMng[lp1] = mng;
        while (!(mng->flg & 0x40)) {
            mng++;
        }
        if (mng->flg & 0x80) {
            break;
        }
        mng++;
    }
    mng++;
    for (lp1++; lp1 < 64; lp1++) {
        lgtMng[lp1] = NULL;
    }
    lgtLwd = (LgtWorkDat *)mng;
}

static void nkKazdLight(sceVu0FVECTOR light, sceVu0FVECTOR col, sceVu0FVECTOR posi) {
    OBJWORK *kazd = GameGbl.kazd;
    sceVu0FVECTOR vf;
    sceVu0FVECTOR wk;
    f32 dis;

    if (kazd->bun0 == 1 || kazd->bun0 == 2) {
        vf[0] = posi[0] - kazd->posi[0];
        vf[0] = posi[1] - (kazd->posi[1] + 25.0f);
        vf[2] = posi[2] - kazd->posi[2];
        vu0_Square(wk, vf);
        dis = vu0_SquareRoot(wk[0] + wk[1] + wk[2]);
        if (!(dis > 64.0f) && !(dis < 1.0f)) {
            sceVu0Normalize(light, vf);
            col[0] = (64.0f - dis) / 64.0f * 1.0f;
            col[1] = (64.0f - dis) / 64.0f * 8.0f;
            col[2] = (64.0f - dis) / 64.0f * 8.0f;
        }
    }
}

void nkCalcLightMatrix(sceVu0FVECTOR posi) {
    sceVu0FVECTOR light[3];
    sceVu0FVECTOR color[3];
    sceVu0FVECTOR dummy;
    s32 lp;
    nkLightWrk *lline = nkLightTest;

    dummy[0] = 0.0f;
    dummy[1] = 0.0f;
    dummy[2] = 0.0f;

    for (lp = 0; lp < 3; lp++, lline++) {
        light[lp][0] = sinf(lline->dir.x);
        light[lp][0] *= cosf(lline->dir.y);
        light[lp][2] = cosf(lline->dir.x);
        light[lp][2] *= cosf(lline->dir.y);
        light[lp][1] = sinf(lline->dir.y);
        light[lp][3] = 0.0f;

        if (lline->on != 0) {
            color[lp][0] = lline->col[0];
            color[lp][1] = lline->col[1];
            color[lp][2] = lline->col[2];
        } else {
            color[lp][0] = 0.0f;
            color[lp][1] = 0.0f;
            color[lp][2] = 0.0f;
        }
    }

    nkKazdLight(light[2], color[2], posi);
    sceVu0NormalLightMatrix(nkNormalLight, light[0], light[1], light[2]);
    sceVu0LightColorMatrix(nkLightColor,   color[0], color[1], color[2], nkAmbient);
}

void _nkLightInit() {
    _nkLightData *lgtdata;
    sceVu0FVECTOR light[3];
    sceVu0FVECTOR color[4];
    s32 i;
    s32 j;

    lgtdata = &nkLgt0701_00;
    if (GameGbl.vision / 256 == 1) {
        lgtdata = &nkLgt0100_00;
    }
    if (GameGbl.vision / 256 == 2) {
        switch (GameGbl.vision & 0xFF) {
            case 0:
                lgtdata = &nkLgt0200_00;
                break;
            case 1:
                lgtdata = &nkLgt0201_00;
                break;
            case 2:
                lgtdata = &nkLgt0200_00;
                break;
            case 3:
                lgtdata = &nkLgt0201_00;
                break;
            case 4:
                lgtdata = &nkLgt0200_00;
                break;
            case 5:
                lgtdata = &nkLgt0201_00;
                break;
            case 6:
                lgtdata = &nkLgt0200_00;
                break;
        }
    }
    if (GameGbl.vision / 256 == 4 && (GameGbl.vision & 0xFF) == 0) {
        lgtdata = &nkLgt0400_00;
    }
    if (GameGbl.vision / 256 == 7) {
        switch (GameGbl.vision & 0xFF) {
            case 0:
                lgtdata = &nkLgt0700_00;
                break;
            case 3:
                lgtdata = &nkLgt0703_01;
                break;
        }
    }
    if (GameGbl.vision / 256 == 11) {
        lgtdata = &nkLgt1100_00;
    }

    for (i = 0; i < 3; i++) {
        light[i][0] = sinf(6.283184f * lgtdata->dir[i][0] / 360.0f);
        light[i][0] *= cosf(6.283184f * lgtdata->dir[i][1] / 360.0f);
        light[i][2] = cosf(6.283184f * lgtdata->dir[i][0] / 360.0f);
        light[i][2] *= cosf(6.283184f * lgtdata->dir[i][1] / 360.0f);
        light[i][1] = sinf(6.283184f * lgtdata->dir[i][1] / 360.0f);
        light[i][3] = 0.0f;
        nkLightTest[i].dir.x = 6.283184f * lgtdata->dir[i][0] / 360.0f;
        nkLightTest[i].dir.y = 6.283184f * lgtdata->dir[i][1] / 360.0f;
        nkLightTest[i].on = 1;
    }

    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            color[i][j] = lgtdata->col[i][j] / 255.0f;
        }
    }
    for (j = 0; j < 4; j++) {
        color[3][j] = lgtdata->amb[j] / 255.0f;
    }

    sceVu0CopyVector(nkLightTest[0].col, color[0]);
    sceVu0CopyVector(nkLightTest[1].col, color[1]);
    sceVu0CopyVector(nkLightTest[2].col, color[2]);
    sceVu0CopyVector(nkAmbient,          color[3]);

    sceVu0NormalLightMatrix(nkNormalLight, light[0], light[1], light[2]);
    sceVu0LightColorMatrix(nkLightColor,   color[0], color[1], color[2], color[3]);
}

void nkLightIp(LgtWorkF *lgt_r, LgtWorkF *lgt_a, LgtWorkF *lgt_b, f32 w) {
    s32 i;
    LgtFIF *a = lgt_a;
    LgtFIF *b = lgt_b;
    LgtFIF *r = lgt_r;

    for (i = 0; i < 3; a++, b++, r++, i++) {
        r->dir_x = a->dir_x + (b->dir_x - a->dir_x) * w;
        if (r->dir_x < -3.141592f) {
            r->dir_x += 6.283184f;
        } else if (r->dir_x > 3.141592f) {
            r->dir_x -= 6.283184f;
        }

        r->dir_y = a->dir_y + (b->dir_y - a->dir_y) * w;
        if (r->dir_y < -3.141592f) {
            r->dir_y += 6.283184f;
        } else if (r->dir_y > 3.141592f) {
            r->dir_y -= 6.283184f;
        }

        r->col[0] = a->col[0] + (b->col[0] - a->col[0]) * w;
        r->col[1] = a->col[1] + (b->col[1] - a->col[1]) * w;
        r->col[2] = a->col[2] + (b->col[2] - a->col[2]) * w;
        r->col[3] = a->col[3] + (b->col[3] - a->col[3]) * w;
    }

    lgt_r->amb[0] = lgt_a->amb[0] + (lgt_b->amb[0] - lgt_a->amb[0]) * w;
    lgt_r->amb[1] = lgt_a->amb[1] + (lgt_b->amb[1] - lgt_a->amb[1]) * w;
    lgt_r->amb[2] = lgt_a->amb[2] + (lgt_b->amb[2] - lgt_a->amb[2]) * w;
    lgt_r->amb[3] = lgt_a->amb[3] + (lgt_b->amb[3] - lgt_a->amb[3]) * w;
}

void nkLightWork2F(LgtWorkF *out, LgtWorkDat *in) {
    s32 i;
    LgtINFO *lgti;
    LgtFIF *lgtf;

    for (lgti = in->lgt, lgtf = out->lgt, i = 0; i < 3; lgti++, lgtf++, i++) {
        lgtf->dir_x = 6.283184f * lgti->dir_x / 360.0f;
        lgtf->dir_y = 6.283184f * lgti->dir_y / 360.0f;
        lgtf->col[0] = lgti->r / 255.0f;
        lgtf->col[1] = lgti->g / 255.0f;
        lgtf->col[2] = lgti->b / 255.0f;
        lgtf->col[3] = 0.0f;
    }

    out->amb[0] = in->amb[0] / 255.0f;
    out->amb[1] = in->amb[1] / 255.0f;
    out->amb[2] = in->amb[2] / 255.0f;
    out->amb[3] = in->amb[3] / 255.0f;
}

void nkMngLight(LgtWorkF *lgtw, sceVu0FVECTOR posi, RT_WRK *rtw) {
    nkLgtMNG *mng;
    nkLgtMNG *mng_n;
    nkLgtMNG *b_mng;
    s32 rtcnt;
    f32 height;
    LgtWorkDat *lwd_top;
    LgtWorkDat *lwd;
    s32 i;
    s32 j;
    s32 cnt;
    f32 weight;
    LgtWorkF flgt_a;
    LgtWorkF flgt_b;
    LgtWorkF flgt_a0;
    LgtWorkF flgt_a1;
    LgtWorkF flgt_b0;
    LgtWorkF flgt_b1;

    lwd_top = lgtLwd;
    cnt = 0;
    mng = lgtMng[GetMichiNo(rtw->rtp)];
    if (mng == NULL) {
        if (!(nkDG.flag & 0x10)) {
            nkLightWork2F(lgtw, lwd_top);
        }
        return;
    }
    rtcnt = rtw->mcn >> 12;
    height = posi[1];

    while (1) {
        b_mng = NULL;
        if (mng->flg & 0x40) {
            break;
        }
        if (rtcnt < (mng + 1)->rtcnt) {
            goto cringe;
        }
        mng++;
        cnt++;
        if (mng->rtcnt == (mng + 1)->rtcnt && height > mng->height) {
            for (b_mng = mng; mng->rtcnt == b_mng->rtcnt; b_mng++);
            if (b_mng->rtcnt > rtcnt) {
                break;
            }
        }
    }

cringe: // cringe
    if (b_mng != NULL) {
        mng_n = b_mng;
        if (cnt > 0 && mng->rtcnt == (mng - 1)->rtcnt) {
            i = (s32)height - mng->height;
            j = (mng - 1)->height - mng->height;
            weight = (f32)i / (f32)j;
            lwd = lwd_top + mng->idx;
            nkLightWork2F(&flgt_a0, lwd);
            lwd = lwd_top + (mng - 1)->idx;
            nkLightWork2F(&flgt_a1, lwd);
            nkLightIp(&flgt_a, &flgt_a0, &flgt_a1, weight);
        } else {
            lwd = lwd_top + mng->idx;
            nkLightWork2F(&flgt_a, lwd);
        }
    } else {
        mng_n = mng + 1;
        lwd = lwd_top + mng->idx;
        nkLightWork2F(&flgt_a, lwd);
    }

    if (mng_n->height == -0x80000000 || (mng_n + 1)->height == -0x80000000) {
        lwd = lwd_top + mng_n->idx;
        nkLightWork2F(&flgt_b, lwd);
    } else {
        while ((mng_n + 1)->height != -0x80000000 && mng_n->rtcnt == (mng_n + 1)->rtcnt && !(height > (mng_n + 1)->height)) {
            mng_n++;
        }
        i = (s32)height - (mng_n + 1)->height;
        j = mng_n->height - (mng_n + 1)->height;
        if (i < 0) {
            weight = 0.0f;
        } else {
            weight = (f32)i / (f32)j;
        }
        lwd = lwd_top + (mng_n + 1)->idx;
        nkLightWork2F(&flgt_b0, lwd);
        lwd = lwd_top + mng_n->idx;
        nkLightWork2F(&flgt_b1, lwd);
        nkLightIp(&flgt_b, &flgt_b0, &flgt_b1, weight);
    }

    i = rtcnt - mng->rtcnt;
    j = mng_n->rtcnt - mng->rtcnt;
    weight = (f32)i / (f32)j;
    nkLightIp(lgtw, &flgt_a, &flgt_b, weight);
}

void nkSetLight(OBJWORK *objw) {
    // Empty function
}

void nkSetLight2(RT_WRK *rtw, sceVu0FVECTOR posi, sceVu0FMATRIX **nl, sceVu0FMATRIX **lc) {
    LgtWorkF flgt;

    if (GameGbl.cam.mode == 5) {
        nkCalcLightMatrix(posi);
    } else {
        nkMngLight(&flgt, posi, rtw);
        nkCalcMatLgtWork(nkNormalLight, nkLightColor, &flgt, posi);
    }

    *nl = &nkNormalLight;
    *lc = &nkLightColor;
}

void nkSetLight3(RT_WRK *rtw, sceVu0FVECTOR posi, sceVu0FMATRIX **nl, sceVu0FMATRIX **lc) {
    LgtWorkF flgt;

    if (GameGbl.cam.mode == 5) {
        nkCalcLightMatrix(posi);
    } else {
        nkMngLight(&flgt, posi, rtw);
        nkCalcMatLgtWork2(nkNormalLight, nkLightColor, &flgt, posi);
    }

    *nl = &nkNormalLight;
    *lc = &nkLightColor;
}

void nkGetRouteLgtWork(LgtWorkF *lgtw, RT_WRK *rtw, sceVu0FVECTOR posi) {
    if (GameGbl.cam.mode == 5) {
        nkCalcLightMatrix(posi);
    } else {
        nkMngLight(lgtw, posi, rtw);
    }
}

void nkGetLgtWorkF(LgtWorkF *lgt, s32 light_no) {
    LgtWorkDat *lwd_top;

    if (nkDG.flag & 0x10) {
        return;
    }
    lwd_top = lgtLwd;
    nkLightWork2F(lgt, lwd_top + light_no);
}

void nkCalcMatLgtWork(sceVu0FMATRIX normal_light, sceVu0FMATRIX light_color,  LgtWorkF *lgtw, sceVu0FVECTOR posi) {
    LgtFIF *fl;
    sceVu0FVECTOR light[3];
    sceVu0FVECTOR color[3];
    sceVu0FVECTOR amb;
    s32 i;

    for (fl = lgtw->lgt, i = 0; i < 3; i++, fl++) {
        light[i][0] = sinf(fl->dir_x);
        light[i][0] *= cosf(fl->dir_y);
        light[i][2] = cosf(fl->dir_x);
        light[i][2] *= cosf(fl->dir_y);
        light[i][1] = sinf(fl->dir_y);
        light[i][3] = 0.0f;
        color[i][0] = fl->col[0];
        color[i][1] = fl->col[1];
        color[i][2] = fl->col[2];
        color[i][3] = 0.0f;
    }

    amb[0] = lgtw->amb[0];
    amb[1] = lgtw->amb[1];
    amb[2] = lgtw->amb[2];
    amb[3] = 0.0f;

    if (posi != NULL) {
        if (GameGbl.vision >> 8 == 13) {
            nkKuLightObj(light[0], color[0], posi, amb);
            nkKuLightObj(light[1], color[1], posi, amb);
            nkKuLightObj(light[2], color[2], posi, amb);
        }
        nkKazdLight(light[2], color[2], posi);
    }

    sceVu0NormalLightMatrix(normal_light, light[0], light[1], light[2]);
    sceVu0LightColorMatrix(light_color, color[0], color[1], color[2], amb);
}

void nkCalcMatLgtWork2(sceVu0FMATRIX normal_light, sceVu0FMATRIX light_color,  LgtWorkF *lgtw, sceVu0FVECTOR posi) {
    LgtFIF *fl;
    sceVu0FVECTOR light[3];
    sceVu0FVECTOR color[3];
    sceVu0FVECTOR amb;
    s32 i;

    for (fl = lgtw->lgt, i = 0; i < 3; i++, fl++) {
        light[i][0] = sinf(fl->dir_x);
        light[i][0] *= cosf(fl->dir_y);
        light[i][2] = cosf(fl->dir_x);
        light[i][2] *= cosf(fl->dir_y);
        light[i][1] = sinf(fl->dir_y);
        light[i][3] = 0.0f;
        color[i][0] = fl->col[0];
        color[i][1] = fl->col[1];
        color[i][2] = fl->col[2];
        color[i][3] = 0.0f;
    }

    amb[0] = lgtw->amb[0];
    amb[1] = lgtw->amb[1];
    amb[2] = lgtw->amb[2];
    amb[3] = 0.0f;

    if (posi != NULL) {
        if (GameGbl.vision >> 8 == 13) {
            nkKuLightObj(light[0], color[0], posi, amb);
            nkKuLightObj(light[1], color[1], posi, amb);
            nkKuLightObj(light[2], color[2], posi, amb);
        }
    }

    sceVu0NormalLightMatrix(normal_light, light[0], light[1], light[2]);
    sceVu0LightColorMatrix(light_color, color[0], color[1], color[2], amb);
}

void nkSetLightNo(sceVu0FMATRIX normal_light, sceVu0FMATRIX light_color, s32 light_no, sceVu0FVECTOR posi) {
    LgtWorkDat *lwd_top;
    LgtWorkF flgt;
    LgtFIF *fl;
    sceVu0FVECTOR light[3];
    sceVu0FVECTOR color[3];
    sceVu0FVECTOR amb;
    s32 i;

    if (nkDG.flag & 0x10) {
        sceVu0CopyMatrix(normal_light, nkNormalLight);
        sceVu0CopyMatrix(light_color, nkLightColor);
        return;
    }

    lwd_top = lgtLwd;
    nkLightWork2F(&flgt, lwd_top + light_no);
    for (fl = flgt.lgt, i = 0; i < 3; fl++, i++) {
        light[i][0] = sinf(fl->dir_x);
        light[i][0] *= cosf(fl->dir_y);
        light[i][2] = cosf(fl->dir_x);
        light[i][2] *= cosf(fl->dir_y);
        light[i][1] = sinf(fl->dir_y);
        light[i][3] = 0.0f;
        color[i][0] = fl->col[0];
        color[i][1] = fl->col[1];
        color[i][2] = fl->col[2];
        color[i][3] = 0.0f;
    }

    amb[0] = flgt.amb[0];
    amb[1] = flgt.amb[1];
    amb[2] = flgt.amb[2];
    amb[3] = 0.0f;

    if (posi != NULL) {
        nkKazdLight(light[2], color[2], posi);
    }

    sceVu0NormalLightMatrix(normal_light, light[0], light[1], light[2]);
    sceVu0LightColorMatrix(light_color, color[0], color[1], color[2], amb);
}
