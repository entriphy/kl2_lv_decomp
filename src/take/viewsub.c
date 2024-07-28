#include "take/viewsub.h"
#include "take/motsys.h"
#include "take/object.h"

void BaseMove(SFXOBJ *pObj, sceVu0FVECTOR Rot, sceVu0FVECTOR Trans) {
    if (PAD_LVL_CROSS(GameGbl.kpd[0])) {
        Rot[0] += (2 * M_PI) / 45.0;
        if (Rot[0] > M_PI) {
            Rot[0] -= 2.0 * M_PI;
        }
    }
    if (PAD_LVL_TRIANG(GameGbl.kpd[0])) {
        Rot[0] -= (2 * M_PI) / 45.0;
        if (Rot[0] < -M_PI) {
            Rot[0] += 2.0 * M_PI;
        }
    }

    if (PAD_LVL_CIRCLE(GameGbl.kpd[0])) {
        Rot[1] += (2 * M_PI) / 45.0;
        if (Rot[1] > M_PI) {
            Rot[1] -= 2.0 * M_PI;
        }
    }
    if (PAD_LVL_SQUARE(GameGbl.kpd[0])) {
        Rot[1] -= (2 * M_PI) / 45.0;
        if (Rot[1] < -M_PI) {
            Rot[1] += 2.0 * M_PI;
        }
    }

    if (PAD_LVL_DOWN(GameGbl.kpd[0])) {
        Trans[1] += 2.5;
    }
    if (PAD_LVL_UP(GameGbl.kpd[0])) {
        Trans[1] -= 2.5;
    }
    if (PAD_LVL_RIGHT(GameGbl.kpd[0])) {
        Trans[0] += 2.5;
    }
    if (PAD_LVL_LEFT(GameGbl.kpd[0])) {
        Trans[0] += 2.5;
    }
    if (PAD_LVL_R1(GameGbl.kpd[0])) {
        Trans[2] += 2.5;
    }
    if (PAD_LVL_R2(GameGbl.kpd[0])) {
        Trans[2] -= 2.5;
    }

    SetBaseMatrix(pObj, Rot, Trans, 0);
}

void BgDraw() {
	sceVu0IVECTOR COL0;
	sceVu0IVECTOR COL1;
	sceVu0IVECTOR COL2;
	sceVu0IVECTOR COL3;
	sceVu0IVECTOR XYZ0;
	sceVu0IVECTOR XYZ1;
	sceVu0IVECTOR XYZ2;
	sceVu0IVECTOR XYZ3;
	qword_uni PacketBuff[64];
	qword_uni *pPacket = PacketBuff;
    s32 PacketIndex;
    s32 var_a3;
    s32 var_a0;

    PacketIndex = 2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_TEST_2(0, 0, 0, 0, 0, 0, 1, SCE_GS_ZGEQUAL);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_TEST_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_ALPHA_2(1, 1, 0, 1, 128);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_ALPHA_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_TEXA(64, 0, 64);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_TEXA;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_ZBUF(0x8C, SCE_GS_PSMZ16S, SCE_GS_ZALWAYS);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_ZBUF_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_FBA_2(0x00);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_FBA_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_PABE(0x00);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_PABE;

    pDma.Gif->chcr.TTE = 0;
    pPacket[0].u_u128 = 0;
    pPacket[0].u_u32[0] = DMAend | 7;
    pPacket[1].u_u64[0] = SCE_GIF_SET_TAG(6, 1, 0, 0, SCE_GIF_PACKED, 1);
    pPacket[1].u_u64[1] = SCE_GIF_PACKED_AD;
    FlushCache(WRITEBACK_DCACHE);

    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, pPacket);
    sceDmaSync(pDma.Gif, 0, 0);

    var_a3 = 0x80;
    var_a0 = 0xA0;

    SETVEC(XYZ0, 0x6C00, 0x7900, 1, 0);
    SETVEC(XYZ1, 0x6C00, 0x8700, 1, 0);
    SETVEC(XYZ2, 0x9400, 0x7900, 1, 0);
    SETVEC(XYZ3, 0x9400, 0x8700, 1, 0);

    SETVEC(COL0, var_a0, 0x50,   0x50,   0x00);
    SETVEC(COL1, var_a0, var_a0, var_a3, 0x00);
    SETVEC(COL2, var_a3, var_a0, 0x64,   0x00);
    SETVEC(COL3, 0x40,   var_a3, var_a0, 0x00);

    PacketIndex = 0;
    pPacket[PacketIndex].u_u128 = 0;
    pPacket[PacketIndex++].u_u32[0] = DMAend | 9;
    pPacket[PacketIndex].u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 0, 0, 0, 1, 0), 0, 8);
    pPacket[PacketIndex++].u_u64[1] =
          (u64)SCE_GS_RGBAQ << 0x00 | (u64)SCE_GS_XYZ2 << 0x04
        | (u64)SCE_GS_RGBAQ << 0x08 | (u64)SCE_GS_XYZ2 << 0x0C
        | (u64)SCE_GS_RGBAQ << 0x10 | (u64)SCE_GS_XYZ2 << 0x14
        | (u64)SCE_GS_RGBAQ << 0x18 | (u64)SCE_GS_XYZ2 << 0x1C;
    pPacket[PacketIndex++].u_u128 = *(u128 *)COL0; pPacket[PacketIndex++].u_u128 = *(u128 *)XYZ0;
    pPacket[PacketIndex++].u_u128 = *(u128 *)COL1; pPacket[PacketIndex++].u_u128 = *(u128 *)XYZ1;
    pPacket[PacketIndex++].u_u128 = *(u128 *)COL2; pPacket[PacketIndex++].u_u128 = *(u128 *)XYZ2;
    pPacket[PacketIndex++].u_u128 = *(u128 *)COL3; pPacket[PacketIndex++].u_u128 = *(u128 *)XYZ3;
    FlushCache(WRITEBACK_DCACHE);

    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, pPacket);
    sceDmaSync(pDma.Gif, 0, 0);
}

s32 GetChrNum(char *str) {
	s32 num;
    u8 tmp;
    
    num = (str[0] - 'a') * 36;
    tmp = str[1];
    if (tmp - 'a' < 26U) {
        num -= 'a';
        num += (s8) tmp;
    } else {
        num -= 22;
        num += str[1];
    }
    
    return num;
}

void DrawClipZone(SFXOBJ *pObj) {
	sceVu0FMATRIX TmpMatrix;
	sceVu0FVECTOR TmpVector[8];
	static sceVu0IVECTOR TmpIVector[8];
	sceVu0IVECTOR COL0;
	sceVu0IVECTOR XYZ0;
	sceVu0IVECTOR XYZ1;
	sceVu0IVECTOR XYZ2;
	sceVu0IVECTOR XYZ3;
	qword_uni PacketBuff[64];
    qword_uni *pPacket;
    s32 PacketIndex;
	s32 TmpCol;
	s32 i;
	f32 Radius;
	f32 Scale;

    Radius = pObj->ClipZone;
    Scale = 0.0f;

    for (i = 0; i < 3; i++) {
        if (pObj->ScaleVector[i] > Scale) {
            Scale = pObj->ScaleVector[i];
        }
    }

    sceVu0UnitMatrix(TmpMatrix);
    SETVEC(TmpMatrix[3], 0.0f, pObj->ClipOffset, 0.0f, 1.0f);
    sceVu0MulMatrix(TmpMatrix, pObj->pMot->pBaseCoord->Mtx, TmpMatrix);
    sceVu0MulMatrix(TmpMatrix, Scr.WvMtx, TmpMatrix);
    SETVEC(TmpMatrix[0], Scale, 0.0f, 0.0f, 0.0f);
    SETVEC(TmpMatrix[1], 0.0f, Scale, 0.0f, 0.0f);
    SETVEC(TmpMatrix[2], 0.0f, 0.0f, Scale, 0.0f);
    sceVu0MulMatrix(TmpMatrix, Scr.VsMtx, TmpMatrix);
    SETVEC(TmpVector[0], Radius + 0.0f, Radius + 0.0f, Radius + 0.0f, 1.0f);
    SETVEC(TmpVector[1], Radius + 0.0f, Radius + 0.0f, 0.0f - Radius, 1.0f);
    SETVEC(TmpVector[2], Radius + 0.0f, 0.0f - Radius, Radius + 0.0f, 1.0f);
    SETVEC(TmpVector[3], Radius + 0.0f, 0.0f - Radius, 0.0f - Radius, 1.0f);
    SETVEC(TmpVector[4], 0.0f - Radius, Radius + 0.0f, Radius + 0.0f, 1.0f);
    SETVEC(TmpVector[5], 0.0f - Radius, Radius + 0.0f, 0.0f - Radius, 1.0f);
    SETVEC(TmpVector[6], 0.0f - Radius, 0.0f - Radius, Radius + 0.0f, 1.0f);
    SETVEC(TmpVector[7], 0.0f - Radius, 0.0f - Radius, 0.0f - Radius, 1.0f);
    sceVu0RotTransPers(TmpIVector[0], TmpMatrix, TmpVector[0], 1);
    sceVu0RotTransPers(TmpIVector[1], TmpMatrix, TmpVector[1], 1);
    sceVu0RotTransPers(TmpIVector[2], TmpMatrix, TmpVector[2], 1);
    sceVu0RotTransPers(TmpIVector[3], TmpMatrix, TmpVector[3], 1);
    sceVu0RotTransPers(TmpIVector[4], TmpMatrix, TmpVector[4], 1);
    sceVu0RotTransPers(TmpIVector[5], TmpMatrix, TmpVector[5], 1);
    sceVu0RotTransPers(TmpIVector[6], TmpMatrix, TmpVector[6], 1);
    sceVu0RotTransPers(TmpIVector[7], TmpMatrix, TmpVector[7], 1);

    TmpCol = (GameGbl.fr * 8) & 0xFF;
    if (TmpCol > 0x80) {
        TmpCol = 0x100 - TmpCol;
    }

    pPacket = PacketBuff;
    PacketIndex = 2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_TEST(1, 1, 128, 0, 0, 0, 1, SCE_GS_ZGEQUAL);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_TEST_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_ALPHA_2(1, 1, 0, 1, 128);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_ALPHA_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_TEXA(64, 0, 64);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_TEXA;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_ZBUF(0x8C, SCE_GS_PSMZ16S, SCE_GS_ZNOUSE);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_ZBUF_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_FBA_2(0x00);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_FBA_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_PABE(0x00);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_PABE;

    pDma.Gif->chcr.TTE = 0;
    pPacket[0].u_u128 = 0;
    pPacket[0].u_u32[0] = DMAend | 7;
    pPacket[1].u_u64[0] = SCE_GIF_SET_TAG(6, 0, 0, 0, SCE_GIF_PACKED, 1);
    pPacket[1].u_u64[1] = SCE_GIF_PACKED_AD;
    FlushCache(WRITEBACK_DCACHE);

    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, pPacket);
    sceDmaSync(pDma.Gif, 0, 0);

    SETVEC(COL0, TmpCol, TmpCol, TmpCol, 0x80);
    PacketIndex = 0;
    pPacket[PacketIndex].u_u128 = 0;
    pPacket[PacketIndex++].u_u32[0] = DMAend | 25;
    pPacket[PacketIndex].u_u64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_LINESTRIP, 1, 0, 0, 0, 0, 0, 1, 0), 0, 6);
    pPacket[PacketIndex++].u_u64[1] = (u64)SCE_GS_RGBAQ << 0x00 |
        (u64)SCE_GS_XYZ2 << 0x04 | (u64)SCE_GS_XYZ2 << 0x08 | (u64)SCE_GS_XYZ2 << 0x0C | (u64)SCE_GS_XYZ2 << 0x10 | (u64)SCE_GS_XYZ2 << 0x14;
    
    pPacket[PacketIndex++].u_u128 = *(u128 *)COL0;
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[4];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[5];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[7];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[6];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[4];

    pPacket[PacketIndex++].u_u128 = *(u128 *)COL0;
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[0];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[1];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[3];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[2];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[0];

    pPacket[PacketIndex++].u_u128 = *(u128 *)COL0;
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[1];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[3];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[7];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[5];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[1];

    pPacket[PacketIndex++].u_u128 = *(u128 *)COL0;
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[0];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[2];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[6];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[4];
    pPacket[PacketIndex++].u_u128 = *(u128 *)TmpIVector[0];

    FlushCache(WRITEBACK_DCACHE);
    pDma.Gif->chcr.TTE = 1;
    sceDmaSend(pDma.Gif, pPacket);
    sceDmaSync(pDma.Gif, 0, 0);
}
