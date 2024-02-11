#include "nakano/nk2pcon.h"
#include "nakano/main.h"
#include "nakano/camera.h"
#include "nakano/light.h"

static s32 nkCeCarsol;

void nkDrawFbBar(s32 x, s32 y, s32 w) {
    nkGifPacket pack;
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y2; // they couldn't name this y1 because there's a function called y1 lmaooo

    x0 = x * 0xC0 + 0x6C00;
    y0 = y * 0x60 + 0x7900;
    x1 = x0 + w * 0xC0;
    y2 = y0 + 0x60;

    pack.size = 0;
    pack.buf = (nkQWdata *)0x70000000;
    pack.buf[pack.size].ul128 = 0;
    pack.buf[pack.size++].ul32[0] = 0x70000009;
    pack.buf[pack.size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 0, 0, 0, 1, 0), 0, 2);
    pack.buf[pack.size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 4;

    pack.buf[pack.size].ul32[0] = 0x00;
    pack.buf[pack.size].ul32[1] = 0x00;
    pack.buf[pack.size].ul32[2] = 0x80;
    pack.buf[pack.size].ul32[3] = 0x80;
    pack.size++;
    pack.buf[pack.size].ul32[0] = x0;
    pack.buf[pack.size].ul32[1] = y0;
    pack.buf[pack.size].ul32[2] = 0xFFFE;
    pack.buf[pack.size].ul32[3] = 0x0000;
    pack.size++;

    pack.buf[pack.size].ul32[0] = 0x80;
    pack.buf[pack.size].ul32[1] = 0x00;
    pack.buf[pack.size].ul32[2] = 0x80;
    pack.buf[pack.size].ul32[3] = 0x80;
    pack.size++;
    pack.buf[pack.size].ul32[0] = x1;
    pack.buf[pack.size].ul32[1] = y0;
    pack.buf[pack.size].ul32[2] = 0xFFFE;
    pack.buf[pack.size].ul32[3] = 0x0000;
    pack.size++;

    pack.buf[pack.size].ul32[0] = 0x00;
    pack.buf[pack.size].ul32[1] = 0x00;
    pack.buf[pack.size].ul32[2] = 0x80;
    pack.buf[pack.size].ul32[3] = 0x80;
    pack.size++;
    pack.buf[pack.size].ul32[0] = x0;
    pack.buf[pack.size].ul32[1] = y2;
    pack.buf[pack.size].ul32[2] = 0xFFFE;
    pack.buf[pack.size].ul32[3] = 0x0000;
    pack.size++;

    pack.buf[pack.size].ul32[0] = 0x80;
    pack.buf[pack.size].ul32[1] = 0x00;
    pack.buf[pack.size].ul32[2] = 0x80;
    pack.buf[pack.size].ul32[3] = 0x80;
    pack.size++;
    pack.buf[pack.size].ul32[0] = x1;
    pack.buf[pack.size].ul32[1] = y2;
    pack.buf[pack.size].ul32[2] = 0xFFFE;
    pack.buf[pack.size].ul32[3] = 0x0000;
    pack.size++;

    FlushCache(WRITEBACK_DCACHE);
    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void NkDeb2PTblInit() {
    nkCeCarsol = 0;
}

void NkDeb2PTbl() {
    kPadDATA *kpd1;

    kpd1 = &GameGbl.kpd[1];
    if (GameGbl.flag & 4 || GameGbl.vision >> 8 >= 30) {
        return;
    }

    if (PAD_TRG_START(GameGbl.kpd[1])) {
        nkDG.cam_debug_mode ^= 1;
    }

    if (nkDG.cam_debug_mode == 1) {
        nkDrawFbBar(2, nkCeCarsol + 7, 12);
        if (pPAD_TRG_UP(kpd1)) {
            nkCeCarsol--;
        }
        if (pPAD_TRG_DOWN(kpd1)) {
            nkCeCarsol++;
        }
        if (pPAD_TRG_CIRCLE(kpd1)) {
            nkDG.cam_debug_mode ^= 1;
            switch (nkCeCarsol) {
                case 0:
                    GameGbl.cam.mode = 2;
                    break;
                case 1:
                    GameGbl.cam.mode = 1;
                    break;
                case 2:
                    GameGbl.cam.mode = 3;
                    break;
                case 4:
                    GameGbl.cam.mode = 5;
                    nkLeInit();
                    break;
                default:
                    break;
            }
        }
    } else {
        switch (GameGbl.cam.mode) {
            case 2:
                CamPadCtrl(GameGbl.klonoa, &GameGbl.cam);
                CamFollow(GameGbl.klonoa, &GameGbl.cam);
                break;
            case 3:
                CamDebug(&GameGbl.cam);
                break;
            case 5:
                nkLightEdit();
                break;
        }
    }

    CamMonOut(GameGbl.klonoa, &GameGbl.cam);
}
