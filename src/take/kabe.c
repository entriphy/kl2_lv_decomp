#include "take/kabe.h"
#include "take/object.h"
#include "take/motip.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"

static void DrawKabe(OBJWORK *pObjw);
static sceVu0FVECTOR WallVec[24] = {};
static sceVu0IVECTOR Xyz[256];
static sceVu0IVECTOR Xyz2[256];
static s32 LineIndex[36][2];

OBJWORK* SetKabe(sceVu0FMATRIX Mtx) {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    KABE* kabe;
    InitObjWork(objw);
    objw->stat0 = 9;
    objw->dreff = DrawKabe;
    kabe = (KABE *)getmem(0x1000);
    objw->reg0 = (s32)kabe;
    kabe->Mode = 1;
    kabe->Angle = 0.0f;
    sceVu0CopyMatrix(kabe->Mtx, Mtx);
    return objw;
}

void EraseKabe(OBJWORK *pObjw) {
    pObjw->stat0 = 0;
    freemem((void *)pObjw->reg0);
}

void SetKabeMode(OBJWORK *pObjw, s32 mode) {
    ((KABE *)pObjw->reg0)->Mode = mode;
}

static void DrawKabe(OBJWORK *pObjw) {
    sceVu0FMATRIX m;
    sceVu0IVECTOR vi;
    qword_uni GsEnv[8];
    f32 var_f21;
    sceVu0FVECTOR pad[0x1F8];
    sceVu0FVECTOR vf;
    s32 i;
    s32 ii;
    u8 col;
    s32 PacketInd;
    qword_uni *spr;

    KABE *kabe = (KABE *)pObjw->reg0;
    if (kabe->Mode == 0) {
        return;
    }

    sceVu0MulMatrix(m, Scr.WvMtx, kabe->Mtx);
    sceVu0MulMatrix(m, Scr.VsMtx, m);
    var_f21 = 1.0f;
    for (i = 0; i < 24; i++) {
        sceVu0ScaleVectorXYZ(vf, WallVec[i], 16.08f * var_f21);
        sceVu0RotTransPers(Xyz[i], m, vf, 0);
        if (Xyz[i][2] >= 0x1000000U || Xyz[i][0] < 0x5800 || Xyz[i][0] > 0xA800 || Xyz[i][1] < 0x5800 || Xyz[i][1] > 0xA800) {
            return;
        }
    }
    for (i = 0; i < 24; i++) {
        sceVu0ScaleVectorXYZ(vf, WallVec[i], 14.472f);
        sceVu0RotTransPers(Xyz2[i], m, vf, 0);
        if (Xyz2[i][2] >= 0x1000000U || Xyz2[i][0] < 0x5800 || Xyz2[i][0] > 0xA800 || Xyz2[i][1] < 0x5800 || Xyz2[i][1] > 0xA800) {
            return;
        }
    }

    pDma.Gif->chcr.TTE = 0;
    spr = GsEnv;
    PacketInd = 2;
    spr[PacketInd].u_u64[0] = 0x5160D;
    spr[PacketInd++].u_u64[1] = SCE_GS_TEST_2;
    spr[PacketInd].u_u64[0] = 0x3000000068;
    spr[PacketInd++].u_u64[1] = SCE_GS_ALPHA_2;
    spr[PacketInd].u_u64[0] = 0x3A00008C;
    spr[PacketInd].u_u64[1] = SCE_GS_ZBUF_2;
    spr[0].u_u128 = 0;
    spr[0].u_u32[0] = 0x70000000 | PacketInd;
    spr[1].u_u64[0] = SCE_GIF_SET_TAG(PacketInd - 1, 0, 0, 0, 0, 1);
    spr[1].u_u64[1] = SCE_GIF_PACKED_AD;
    FlushCache(0);
    sceDmaSend(pDma.Gif, spr);
    sceDmaSync(pDma.Gif, 0, 0);

    col = 63.0f + __cos(kabe->Angle) * 63.0f;
    SETVEC(vi, col, col, col + col, 0x80);

    spr = (qword_uni *)0x70000000;
    for (ii = 0; ii < 36; ii++) {
        PacketInd = 0;
        spr[PacketInd].u_u128 = 0;
        spr[PacketInd++].u_u32[0] = 0x70000000 | 6;
        spr[PacketInd].u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 0, 0, 0, 1, 0, 0, 1, 0), 0, 5);
        spr[PacketInd++].u_u64[1] = SCE_GS_RGBAQ | SCE_GS_XYZF2 << 4 | SCE_GS_XYZF2 << 8 | SCE_GS_XYZF2 << 12 | SCE_GS_XYZF2 << 16;
        spr[PacketInd++].u_u128 = *(u128 *)vi;
        spr[PacketInd++].u_u128 = *(u128 *)Xyz[LineIndex[ii][0]];
        spr[PacketInd++].u_u128 = *(u128 *)Xyz[LineIndex[ii][1]];
        spr[PacketInd++].u_u128 = *(u128 *)Xyz2[LineIndex[ii][0]];
        spr[PacketInd++].u_u128 = *(u128 *)Xyz2[LineIndex[ii][1]];
        sceDmaSend(pDma.Gif, (void *)((u32)spr | 0x80000000));
        sceDmaSync(pDma.Gif, 0, 0);
    }

    if (GameGbl.pause_flag == 0) {
        kabe->Angle += M_PI / 30.0;
        if (kabe->Angle > (f64)M_PI) {
            kabe->Angle -= M_TWOPI;
        }
        if (kabe->Angle < (f64)-M_PI) {
            kabe->Angle += M_TWOPI;
        }
    }
}