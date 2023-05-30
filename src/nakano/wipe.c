#include "common.h"

static PsfxMODEL PsfxWipeKlo;

void nkWipeInit() {
    // PsfxWipeKlo.ActNum = 13;
    // PsfxWipeKlo.wip_timer = 0;
    // PsfxWipeKlo.Rot[0] = 0.0f;
    // PsfxWipeKlo.Rot[1] = 0.0f;
    // PsfxWipeKlo.Rot[2] = 0.0f;
    // PsfxWipeKlo.Rot[3] = 0.0f;
    // PsfxWipeKlo.Trans[0] = 0.0f;
    // PsfxWipeKlo.Trans[1] = 25.0f;
    // PsfxWipeKlo.Trans[2] = 200.0f;
    // PsfxWipeKlo.Trans[3] = 1.0f;
    // PsfxWipeKlo.pObj = GetSfxObjectData(371);
    // SetObjEffDraw(PsfxWipeKlo.pObj);
    // PsfxWipeKlo.pObj->Condition = 1;
    // SetAct(PsfxWipeKlo.pObj, PsfxWipeKlo.ActNum);
    // SetBaseMatrix(PsfxWipeKlo.pObj, PsfxWipeKlo.Rot, PsfxWipeKlo.Trans, 0);
}

void nkWipeSetMatrix() {
    sceVu0FMATRIX unit;
    sceVu0FVECTOR tv;
    sceVu0FMATRIX wvMtx;
    sceVu0FMATRIX vsMtx;
    sceVu0FMATRIX wsMtx;

    sceVu0UnitMatrix(unit);
    tv[0] = 0.0f;
    tv[1] = 0.0f;
    tv[2] = 200.0f;
    tv[3] = 1.0f;
    sceVu0TransMatrix(wvMtx, unit, tv);
    sceVu0ViewScreenMatrix(vsMtx, 2560.0f, 1.0f, 0.47f, 2048.0f, 2048.0f, 1.0f, 16777215.0f, 0.1f, 65535.0f);
    sceVu0MulMatrix(wsMtx, vsMtx, wvMtx);
    sceVu0CopyMatrix(Scr.WvMtx, wvMtx);
    sceVu0CopyMatrix(Scr.VsMtx, vsMtx);
    sceVu0CopyMatrix(Scr.WsMtx, wsMtx);
}

void nkWipeEffBClear() {
    struct {
        s32 size;
        sceGifTag giftag;
        sceGsClear c;
    } clear;
    
    clear.size = sceGsSetDefClear2(&clear.c, SCE_GS_ZGREATER, 2048 - 320, 2048 - 112, 640, 224, 0, 0, 0, 0, 0);
    clear.giftag.NLOOP = clear.size;
    clear.giftag.EOP = 1;
    clear.giftag.PRE = 0;
    clear.giftag.PRIM = 0;
    clear.giftag.FLG = 0;
    clear.giftag.NREG = 1;
    clear.giftag.REGS0 = 0xE; // A+D
    
    FlushCache(WRITEBACK_DCACHE);
    sceDmaSendN(DmaChGIF, &clear.giftag, clear.size + 1);
    sceDmaSync(DmaChGIF, 0, 0);
}
