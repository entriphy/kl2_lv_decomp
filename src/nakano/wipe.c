#include "nakano/wipe.h"
#include "nakano/main.h"
#include "nakano/light.h"
#include "nakano/klmot.h"
#include "nakano/gssub.h"
#include "take/object.h"
#include "take/motsys.h"

static PsfxMODEL PsfxWipeKlo;

void nkWipeInit() {
    PsfxMODEL *model = &PsfxWipeKlo;
    SFXOBJ *pObj;

    model->ActNum = 13;
    model->wip_timer = 0;
    model->Rot[0] = 0.0f;
    model->Rot[1] = 0.0f;
    model->Rot[2] = 0.0f;
    model->Rot[3] = 0.0f;
    model->Trans[0] = 0.0f;
    model->Trans[1] = 25.0f;
    model->Trans[2] = 200.0f;
    model->Trans[3] = 1.0f;
    pObj = GetSfxObjectData(371);
    model->pObj = pObj;
    SetObjEffDraw(pObj);
    pObj->Condition = 1;
    SetAct(pObj, model->ActNum);
    SetBaseMatrix(pObj, model->Rot, model->Trans, 0);
}

void nkWipeSetMatrix() {
    sceVu0FVECTOR vec;
    sceVu0FMATRIX mat;
    sceVu0FMATRIX vsm;
    sceVu0FMATRIX wvm;
    sceVu0FMATRIX wsm;

    sceVu0UnitMatrix(mat);
    vec[0] = 0.0f;
    vec[1] = 0.0f;
    vec[2] = 200.0f;
    vec[3] = 1.0f;
    sceVu0TransMatrix(wvm, mat, vec);
    sceVu0ViewScreenMatrix(vsm, 2560.0f, 1.0f, 0.47f, 2048.0f, 2048.0f, 1.0f, 16777215.0f, 0.1f, 65535.0f);
    sceVu0MulMatrix(wsm, vsm, wvm);
    sceVu0CopyMatrix(Scr.WvMtx, wvm);
    sceVu0CopyMatrix(Scr.VsMtx, vsm);
    sceVu0CopyMatrix(Scr.WsMtx, wsm);
}

void nkWipeEffBClear() {
    nkWipeEffBC clear;
    
    clear.size_clear = sceGsSetDefClear2(&clear.clear, SCE_GS_ZGREATER, 2048 - 320, 2048 - 112, 640, 224, 0, 0, 0, 0, 0);
    clear.gif_clear.NLOOP = clear.size_clear;
    clear.gif_clear.EOP = 1;
    clear.gif_clear.PRE = 0;
    clear.gif_clear.PRIM = 0;
    clear.gif_clear.FLG = 0;
    clear.gif_clear.NREG = 1;
    clear.gif_clear.REGS0 = SCE_GIF_PACKED_AD;
    
    FlushCache(WRITEBACK_DCACHE);
    sceDmaSendN(DmaChGIF, &clear.gif_clear, clear.size_clear + 1);
    sceDmaSync(DmaChGIF, 0, 0);
}

void nkDrawWipeEff() {
    nkWipeDrawEnv de;
    nkGifPacket packet;
    nkGifPacket *pp;

    sceDmaSync(DmaChGIF, 0, 0);
    de.size_texenv = sceGsSetDefTexEnv2(&de.texenv, 1, 0x1680, 10, SCE_GS_PSMCT32, 10, 8, 0, 0, 0, 0, 1);
    de.alpha = 5;
    de.alp_addr = SCE_GS_ALPHA_2;
    de.test = 0x38009;
    de.test_addr = SCE_GS_TEST_2;
    de.size_texenv += 2;

    de.gif_texenv.NLOOP = de.size_texenv;
    de.gif_texenv.EOP = 1;
    de.gif_texenv.PRE = 0;
    de.gif_texenv.PRIM = 0;
    de.gif_texenv.FLG = 0;
    de.gif_texenv.NREG = 1;
    de.gif_texenv.REGS0 = SCE_GIF_PACKED_AD;

    FlushCache(0);
    sceDmaSendN(DmaChGIF, &de.gif_texenv, de.size_texenv + 1);
    sceDmaSync(DmaChGIF, 0, 0);

    pp = &packet;
    pp->size = 0;
    pp->buf = (nkQWdata *)0x70000000;

    pp->buf[pp->size].ul128 = 0;
    pp->buf[pp->size++].ul32[0] = 0x70000000 | 13;
    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    pp->buf[pp->size++].ul64[1] = SCE_GS_UV | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;

    pp->buf[pp->size].ul32[0] = 0;
    pp->buf[pp->size].ul32[1] = 0xE00;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x88;
    pp->buf[pp->size].ul32[1] = 0x88;
    pp->buf[pp->size].ul32[2] = 0x88;
    pp->buf[pp->size].ul32[3] = 0x40;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00;
    pp->buf[pp->size].ul32[1] = 0x8700;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0x00;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 0;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x88;
    pp->buf[pp->size].ul32[1] = 0x88;
    pp->buf[pp->size].ul32[2] = 0x88;
    pp->buf[pp->size].ul32[3] = 0x40;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00;
    pp->buf[pp->size].ul32[1] = 0x7900;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0x00;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 0x2800;
    pp->buf[pp->size].ul32[1] = 0xE00;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x88;
    pp->buf[pp->size].ul32[1] = 0x88;
    pp->buf[pp->size].ul32[2] = 0x88;
    pp->buf[pp->size].ul32[3] = 0x40;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x9400;
    pp->buf[pp->size].ul32[1] = 0x8700;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0x00;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 0x2800;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x88;
    pp->buf[pp->size].ul32[1] = 0x88;
    pp->buf[pp->size].ul32[2] = 0x88;
    pp->buf[pp->size].ul32[3] = 0x40;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x9400;
    pp->buf[pp->size].ul32[1] = 0x7900;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0x00;
    pp->size++;

    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void nkDrawWhiteOut(s32 alpha) {
    nkWipeDrawEnv de;
    nkGifPacket packet;
    nkGifPacket *pp = &packet;

    sceDmaSync(DmaChGIF, 0, 0);
    de.size_texenv = sceGsSetDefTexEnv2(&de.texenv, 1, 0x1680, 10, SCE_GS_PSMCT32, 10, 8, 0, 0, 0, 0, 1);
    de.alpha = 0x48;
    de.alp_addr = SCE_GS_ALPHA_2;
    de.test = 0x39003;
    de.test_addr = SCE_GS_TEST_2;
    de.size_texenv += 2;

    de.gif_texenv.NLOOP = de.size_texenv;
    de.gif_texenv.EOP = 1;
    de.gif_texenv.PRE = 0;
    de.gif_texenv.PRIM = 0;
    de.gif_texenv.FLG = 0;
    de.gif_texenv.NREG = 1;
    de.gif_texenv.REGS0 = SCE_GIF_PACKED_AD;

    FlushCache(0);
    sceDmaSendN(DmaChGIF, &de.gif_texenv, de.size_texenv + 1);
    sceDmaSync(DmaChGIF, 0, 0);

    pp->size = 0;
    pp->buf = (nkQWdata *)0x70000000;

    pp->buf[pp->size].ul128 = 0;
    pp->buf[pp->size++].ul32[0] = 0x70000000 | 9;
    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 1, 0, 1, 1, 0), 0, 2);
    pp->buf[pp->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZF2 << 4;

    pp->buf[pp->size].ul32[0] = 0x88;
    pp->buf[pp->size].ul32[1] = 0x88;
    pp->buf[pp->size].ul32[2] = 0x88;
    pp->buf[pp->size].ul32[3] = alpha;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00;
    pp->buf[pp->size].ul32[1] = 0x8700;
    pp->buf[pp->size].ul32[2] = 0x00;
    pp->buf[pp->size].ul32[3] = 0x00;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 0x88;
    pp->buf[pp->size].ul32[1] = 0x88;
    pp->buf[pp->size].ul32[2] = 0x88;
    pp->buf[pp->size].ul32[3] = alpha;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00;
    pp->buf[pp->size].ul32[1] = 0x7900;
    pp->buf[pp->size].ul32[2] = 0x00;
    pp->buf[pp->size].ul32[3] = 0x00;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 0x88;
    pp->buf[pp->size].ul32[1] = 0x88;
    pp->buf[pp->size].ul32[2] = 0x88;
    pp->buf[pp->size].ul32[3] = alpha;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x9400;
    pp->buf[pp->size].ul32[1] = 0x8700;
    pp->buf[pp->size].ul32[2] = 0x00;
    pp->buf[pp->size].ul32[3] = 0x00;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 0x88;
    pp->buf[pp->size].ul32[1] = 0x88;
    pp->buf[pp->size].ul32[2] = 0x88;
    pp->buf[pp->size].ul32[3] = alpha;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x9400;
    pp->buf[pp->size].ul32[1] = 0x7900;
    pp->buf[pp->size].ul32[2] = 0x00;
    pp->buf[pp->size].ul32[3] = 0x00;
    pp->size++;

    DmaChGIF->chcr.TTE = 1;
    sceDmaSend(DmaChGIF, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}

void nkDrawWipe() {
    f32 oy;
    f32 timer;
    sceVu0FVECTOR vec;
    SFXOBJ *pObj;
    PsfxMODEL *model = &PsfxWipeKlo;
    sceVu0FMATRIX nlm;
    sceVu0FMATRIX lcm;

    if ((!(GameGbl.wipe.flag & 2) || GameGbl.wipe.fdata <= 0) && model->pObj != NULL) {
        nkWipeSetMatrix();
        Vu0ProgTrans();
        pObj = model->pObj;
        SetObjPause(model->pObj, GameGbl.pause_flag);
        nkSetLightNo(nlm, lcm, 0, NULL);
        SetObjNormalLight(pObj, &nlm);
        SetObjLightColor(pObj, &lcm);
        pObj->Condition = 2;
        vec[1] = GameGbl.cam.ang[1];
        model->Rot[0] = 0.0f;
        model->Rot[1] = vec[1];
        model->Rot[2] = 0.0f;
        model->Rot[3] = 1.0f;
        if (GameGbl.wipe.outtime > 0) {
            timer = GameGbl.wipe.outtime;
        } else {
            timer = 64 - GameGbl.wipe.intime;
        }
        model->Scale[0] = model->Scale[1] = model->Scale[2] = timer / 64.0f;
        oy = 28.0f * timer / 64.0f;
        model->Trans[0] = 0.0f;
        model->Trans[1] = oy;
        model->Trans[2] = 0.0f;
        model->Trans[3] = 1.0f;
        if (GameGbl.wipe.outtime == 64) {
            nkCallMotion(pObj, &model->ActNum, 13);
        }
        SetBaseMatrix(pObj, model->Rot, model->Trans, 1);
        SetObjScale(pObj, model->Scale);
        sceDmaSync(DmaChGIF, 0, 0);
        nkGsSetEffectFZ2();
        sceDmaSync(DmaChGIF, 0, 0);
        nkWipeEffBClear();
        ModelDraw(pObj);
        sceDmaSync(DmaChGIF, 0, 0);
        nkGsSetNormalFZ2();
        nkDrawWhiteOut((64 - GameGbl.wipe.outtime) / 2);
        sceDmaSync(DmaChGIF, 0, 0);
        nkDrawWipeEff();
        sceDmaSync(DmaChGIF, 0, 0);
    }
}