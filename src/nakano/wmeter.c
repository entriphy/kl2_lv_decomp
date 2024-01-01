#include "nakano/wmeter.h"
#include "nakano/main.h"

static qword wm_col[8] = {
    { 0x00, 0x00, 0xFF, 0x80 },
    { 0xFF, 0x00, 0x00, 0x80 },
    { 0x00, 0xFF, 0x00, 0x80 },
    { 0xFF, 0xFF, 0x00, 0x80 },
    { 0x00, 0xFF, 0xFF, 0x80 },
    { 0xFF, 0x00, 0xFF, 0x80 },
    { 0x00, 0x00, 0x7F, 0x80 },
    { 0x00, 0x00, 0x00, 0x00 }
};
static nkMETER nkMeterWork[10];

void nkResetMeter() {
    s32 lp;

    DPUT_T0_COUNT(0);
    nkDG.meter_num = 0;
    nkDG.meter_cnt = 0;
    for (lp = 0; lp < 16; lp++) {
        nkDG.work_path[lp] = 0;
    }
}

static nkMETER* nkMeter(s32 r, s32 g, s32 b) {
    nkMETER *m;

    if (nkDG.meter_num > 19) {
        return NULL;
    }

    m = &nkMeterWork[nkDG.meter_num++];
    m->cnt = DGET_T0_COUNT() - nkDG.meter_cnt;
    nkDG.meter_cnt = DGET_T0_COUNT();
    m->r = r;
    m->g = g;
    m->b = b;
    return m;
}

void nkSetMeter() {
    nkMeter(0, 0, 255);
}

void nkDrawWorkMeter() {
    s32 lp0;
    nkMETER *m;
    nkGifPacket pack_buff;
    nkGifPacket *pack = &pack_buff;
    s32 wmx;

    if (nkDG.meter_num <= 0) {
        return;
    }
    if (nkDG.meter_num >= 20) {
        return;
    }

    m = nkMeterWork;

    pack->size = 0;
    pack->buf = (nkQWdata *)0x70000000;
    pack->buf[pack->size].ul128 = 0;
    pack->buf[pack->size++].ul32[0] = 0x70000000 | nkDG.meter_num * 4 + 1;
    pack->buf[pack->size].ul64[0] = SCE_GIF_SET_TAG(nkDG.meter_num * 2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0), 0, 2);
    pack->buf[pack->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 4;
    wmx = 0x6E00;
    for (lp0 = 0; lp0 < nkDG.meter_num; lp0++) {
        pack->buf[pack->size].ul32[0] = wm_col[lp0][0];
        pack->buf[pack->size].ul32[1] = wm_col[lp0][1];
        pack->buf[pack->size].ul32[2] = wm_col[lp0][2];
        pack->buf[pack->size].ul32[3] = wm_col[lp0][3];
        pack->size++;
        pack->buf[pack->size].ul32[0] = wmx;
        pack->buf[pack->size].ul32[1] = 0x8620;
        pack->buf[pack->size].ul32[2] = 0xFFFFFE;
        pack->buf[pack->size].ul32[3] = 0;
        pack->size++;

        wmx += m->cnt * 16;

        pack->buf[pack->size].ul32[0] = wm_col[lp0][0];
        pack->buf[pack->size].ul32[1] = wm_col[lp0][1];
        pack->buf[pack->size].ul32[2] = wm_col[lp0][2];
        pack->buf[pack->size].ul32[3] = wm_col[lp0][3];
        pack->size++;
        pack->buf[pack->size].ul32[0] = wmx;
        pack->buf[pack->size].ul32[1] = 0x8640;
        pack->buf[pack->size].ul32[2] = 0xFFFFFE;
        pack->buf[pack->size].ul32[3] = 0;
        pack->size++;
        m++;
    }
    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pack->buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);

    pack->size = 0;
    pack->buf = (nkQWdata *)0x70000000;
    pack->buf[pack->size].ul128 = 0;
    pack->buf[pack->size++].ul32[0] = 0x70000000 | 5;
    pack->buf[pack->size].ul64[0] = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0), 0, 2);
    pack->buf[pack->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 4;
    wmx = 0x6E00;
    for (lp0 = 0; lp0 < 1; lp0++) {
        pack->buf[pack->size].ul32[0] = wm_col[lp0][0];
        pack->buf[pack->size].ul32[1] = wm_col[lp0][1];
        pack->buf[pack->size].ul32[2] = wm_col[lp0][2];
        pack->buf[pack->size].ul32[3] = wm_col[lp0][3];
        pack->size++;
        pack->buf[pack->size].ul32[0] = wmx;
        pack->buf[pack->size].ul32[1] = 0x8660;
        pack->buf[pack->size].ul32[2] = 0xFFFFFE;
        pack->buf[pack->size].ul32[3] = 0;
        pack->size++;

        wmx += nkDG.work_path[lp0] * 16;

        pack->buf[pack->size].ul32[0] = wm_col[lp0][0];
        pack->buf[pack->size].ul32[1] = wm_col[lp0][1];
        pack->buf[pack->size].ul32[2] = wm_col[lp0][2];
        pack->buf[pack->size].ul32[3] = wm_col[lp0][3];
        pack->size++;
        pack->buf[pack->size].ul32[0] = wmx;
        pack->buf[pack->size].ul32[1] = 0x8680;
        pack->buf[pack->size].ul32[2] = 0xFFFFFE;
        pack->buf[pack->size].ul32[3] = 0;
        pack->size++;
    }
    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pack->buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);

    pack->size = 0;
    pack->buf = (nkQWdata *)0x70000000;
    pack->buf[pack->size].ul128 = 0;
    pack->buf[pack->size++].ul32[0] = 0x70000000 | 53;
    pack->buf[pack->size].ul64[0] = SCE_GIF_SET_TAG(26, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_LINE, 0, 0, 0, 0, 0, 0, 0, 0), 0, 2);
    pack->buf[pack->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 4;

    pack->buf[pack->size].ul32[0] = 0x00;
    pack->buf[pack->size].ul32[1] = 0x40;
    pack->buf[pack->size].ul32[2] = 0x00;
    pack->buf[pack->size].ul32[3] = 0x80;
    pack->size++;
    pack->buf[pack->size].ul32[0] = 0x6E00;
    pack->buf[pack->size].ul32[1] = 0x8630;
    pack->buf[pack->size].ul32[2] = 0xFFFFFE;
    pack->buf[pack->size].ul32[3] = 0x00;
    pack->size++;

    pack->buf[pack->size].ul32[0] = 0x00;
    pack->buf[pack->size].ul32[1] = 0x40;
    pack->buf[pack->size].ul32[2] = 0x00;
    pack->buf[pack->size].ul32[3] = 0x80;
    pack->size++;
    pack->buf[pack->size].ul32[0] = 0x7D00;
    pack->buf[pack->size].ul32[1] = 0x8630;
    pack->buf[pack->size].ul32[2] = 0xFFFFFE;
    pack->buf[pack->size].ul32[3] = 0x00;
    pack->size++;

    for (lp0 = 0; lp0 < 13; lp0++) {
        pack->buf[pack->size].ul32[0] = 0x00;
        pack->buf[pack->size].ul32[1] = 0x40;
        pack->buf[pack->size].ul32[2] = 0x00;
        pack->buf[pack->size].ul32[3] = 0x80;
        pack->size++;
        pack->buf[pack->size].ul32[0] = 0x6E00 + lp0 * 0x140;
        pack->buf[pack->size].ul32[1] = 0x8610;
        pack->buf[pack->size].ul32[2] = 0xFFFFFF;
        pack->buf[pack->size].ul32[3] = 0x00;
        pack->size++;

        pack->buf[pack->size].ul32[0] = 0x00;
        pack->buf[pack->size].ul32[1] = 0x40;
        pack->buf[pack->size].ul32[2] = 0x00;
        pack->buf[pack->size].ul32[3] = 0x80;
        pack->size++;
        pack->buf[pack->size].ul32[0] = 0x6E00 + lp0 * 0x140;
        pack->buf[pack->size].ul32[1] = 0x8650;
        pack->buf[pack->size].ul32[2] = 0xFFFFFF;
        pack->buf[pack->size].ul32[3] = 0x00;
        pack->size++;
    }

    FlushCache(0);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)((u32)pack->buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}
