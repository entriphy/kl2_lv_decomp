#include "wipe.h"
#include "main.h"
#include "take/object.h"

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
