#include "take/klsfxsub.h"
#include "take/object.h"

void SpecEnvInit() {
	u64 tex0;
	s32 index;

    index = abGetTexIndex(256);
    if (index > 0) {
        tex0 = OkGetTex0(index);
    } else {
        tex0 = SCE_GS_SET_TEX0(0x3D00, 2, SCE_GS_PSMT8, 6, 6, 0, SCE_GS_DECAL, 0x3D20, SCE_GS_PSMCT32, 0, 0, 1);
    }

    SpecGsEnvInd = 1;
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = tex0;
    SpecGsEnv[SpecGsEnvInd++].u_u64[1] = SCE_GS_TEX0_2;
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = SCE_GS_SET_TEX1(0, 0, 1, 1, 0, 0, 0);
    SpecGsEnv[SpecGsEnvInd++].u_u64[1] = SCE_GS_TEX1_2;
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = SCE_GS_SET_TEST_2(1, 1, 0, 0, 0, 0, 1, SCE_GS_ZGEQUAL);
    SpecGsEnv[SpecGsEnvInd++].u_u64[1] = SCE_GS_TEST_2;
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 2, 2, 1, 0x7F);
    SpecGsEnv[SpecGsEnvInd].u_u64[1] = SCE_GS_ALPHA_2;
    SpecGsEnvAlpha = &SpecGsEnv[SpecGsEnvInd++];
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = SCE_GS_SET_ZBUF(0x8C, SCE_GS_PSMZ16S, 0);
    SpecGsEnv[SpecGsEnvInd++].u_u64[1] = SCE_GS_ZBUF_2;
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = SCE_GS_SET_FBA_2(1);
    SpecGsEnv[SpecGsEnvInd++].u_u64[1] = SCE_GS_FBA_2;
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = SCE_GS_SET_FBA_1(0);
    SpecGsEnv[SpecGsEnvInd++].u_u64[1] = SCE_GS_FBA_1;
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = SCE_GS_SET_TEXA(0x40, 0x00, 0x40);
    SpecGsEnv[SpecGsEnvInd++].u_u64[1] = SCE_GS_TEXA;
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = SCE_GS_SET_CLAMP_2(0, 0, 0, 0, 0, 0);
    SpecGsEnv[SpecGsEnvInd++].u_u64[1] = SCE_GS_CLAMP_2;
    SpecGsEnv[SpecGsEnvInd].u_u64[0] = SCE_GS_SET_RGBAQ(0x80, 0x80, 0x80, 0x80, 0);
    SpecGsEnv[SpecGsEnvInd++].u_u64[1] = SCE_GS_RGBAQ;

    SpecGsEnv[0].u_u64[0] = SCE_GIF_SET_TAG(SpecGsEnvInd - 1, 0, 0, 0, 0, 1);
    SpecGsEnv[0].u_u64[1] = SCE_GIF_PACKED_AD;
}

void SetDefaultDrawEnv() {
    u128 PacketBuff[16];
    s32 PacketIndex;
    sceGsDrawEnv2 *drawEnv = (GameGbl.fr - 1) & 1 ? &GameGbl.db.draw12 : &GameGbl.db.draw02;
    qword_uni *pPacket = (qword_uni *)PacketBuff;

    PacketIndex = 2;
    pPacket[PacketIndex].u_u64[0] = *(u64 *)&drawEnv->frame2;
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_FRAME_2;
    pPacket[PacketIndex].u_u64[0] = *(u64 *)&drawEnv->xyoffset2;
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_XYOFFSET_2;
    pPacket[PacketIndex].u_u64[0] = *(u64 *)&drawEnv->zbuf2;
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_ZBUF_2;
    pPacket[PacketIndex].u_u64[0] = *(u64 *)&drawEnv->scissor2;
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_SCISSOR_2;

    pDma.Gif->chcr.TTE = 0;
    pPacket[0].u_u128 = 0;
    pPacket[0].u_u32[0] = 0x70000000 | PacketIndex - 1;
    pPacket[1].u_u64[0] = SCE_GIF_SET_TAG(4, 1, 0, 0, 0, 1);
    pPacket[1].u_u64[1] = SCE_GIF_PACKED_AD;
    FlushCache(WRITEBACK_DCACHE);
    pDma.Gif->chcr.TTE = 0;

    sceDmaSend(pDma.Gif, pPacket);
    sceDmaSync(pDma.Gif, 0, 0);
}

void EffBuffClear() {
	sceVu0IVECTOR COL;
	sceVu0IVECTOR XYZ0;
	sceVu0IVECTOR XYZ1;
	sceVu0IVECTOR XYZ2;
	sceVu0IVECTOR XYZ3;
	qword PacketBuff[64];
	qword_uni *pPacket = (qword_uni *)PacketBuff;
    s32 PacketIndex;

    PacketIndex = 2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_TEST_2(1, 1, 0, 0, 0, 0, 0, SCE_GS_ZNOUSE);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_TEST_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 0, 2, 0, 0);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_ALPHA_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_TEXA(0, 0, 0);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_TEXA;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMZ16S, 1);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_ZBUF_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_FBA_2(0);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_FBA_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_PABE(0);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_PABE;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_FRAME_2(0xB4, 10, SCE_GS_PSMCT32, 0);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_FRAME_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_XYOFFSET_2(0x6C00, 0x7900);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_XYOFFSET_2;
    pDma.Gif->chcr.TTE = 0;
    pPacket[0].u_u128 = 0;
    pPacket[0].u_u32[0] = 0x70000000 | PacketIndex - 1;
    pPacket[1].u_u64[0] = SCE_GIF_SET_TAG(8, 1, 0, 0, 0, 1);
    pPacket[1].u_u64[1] = SCE_GIF_PACKED_AD;

    FlushCache(WRITEBACK_DCACHE);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, pPacket);
    sceDmaSync(pDma.Gif, 0, 0);

    SETVEC(XYZ0, 0x6C00, 0x7900, 1, 0);
    SETVEC(XYZ1, 0x6C00, 0x8700, 1, 0);
    SETVEC(XYZ2, 0x9400, 0x7900, 1, 0);
    SETVEC(XYZ3, 0x9400, 0x8700, 1, 0);
    SETVEC(COL, 0, 0, 0, 0);

    PacketIndex = 0;
    pPacket[PacketIndex].u_u128 = 0;
    pPacket[PacketIndex++].u_u32[0] = 0x70000000 | 6;
    pPacket[PacketIndex].u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 0, 0, 0, 1, 0), 0, 5);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 4 | SCE_GS_XYZ2 << 8 | SCE_GS_XYZ2 << 12 | SCE_GS_XYZ2 << 16;
    pPacket[PacketIndex++].u_u128 = *(u128 *)COL;
    pPacket[PacketIndex++].u_u128 = *(u128 *)XYZ0;
    pPacket[PacketIndex++].u_u128 = *(u128 *)XYZ1;
    pPacket[PacketIndex++].u_u128 = *(u128 *)XYZ2;
    pPacket[PacketIndex++].u_u128 = *(u128 *)XYZ3;

    FlushCache(WRITEBACK_DCACHE);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, pPacket);
    sceDmaSync(pDma.Gif, 0, 0);
}

void SetEffBuff() {
    sceVu0IVECTOR COL;
    sceVu0IVECTOR XYZ0;
    sceVu0IVECTOR XYZ1;
    sceVu0IVECTOR XYZ2;
    sceVu0IVECTOR XYZ3;
    qword PacketBuff[64];
    qword_uni *pPacket = (qword_uni *)PacketBuff;
    s32 PacketIndex;

    PacketIndex = 2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMZ16S, 1);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_ZBUF_2;
    pPacket[PacketIndex].u_u64[0] = SCE_GS_SET_FRAME_2(0xB4, 10, SCE_GS_PSMCT32, 0);
    pPacket[PacketIndex++].u_u64[1] = SCE_GS_FRAME_2;
    pDma.Gif->chcr.TTE = 0;
    pPacket[0].u_u128 = 0;
    pPacket[0].u_u32[0] = 0x70000000 | PacketIndex - 1;
    pPacket[1].u_u64[0] = SCE_GIF_SET_TAG(2, 1, 0, 0, 0, 1);
    pPacket[1].u_u64[1] = SCE_GIF_PACKED_AD;
    FlushCache(WRITEBACK_DCACHE);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, pPacket);
    sceDmaSync(pDma.Gif, 0, 0);
}

void func_00211C28(u64 Tex0, s32 u, s32 v, s32 x, s32 y) {
    sceVu0IVECTOR XYZ[4];
    sceVu0IVECTOR UV[4];
    sceVu0IVECTOR RGBA;
    qword_uni *pPacket = SPR_MEM;
    s32 PacketInd = 2;
    
    pPacket[PacketInd].u_u64[0] = Tex0;
    pPacket[PacketInd++].u_u64[1] = SCE_GS_TEX0_2;
    pPacket[PacketInd].u_u64[0] = SCE_GS_SET_TEST_2(1, 1, 0, 0, 0, 0, 1, SCE_GS_ZGEQUAL);
    pPacket[PacketInd++].u_u64[1] = SCE_GS_TEST_2;
    pPacket[PacketInd].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 1, 0, 1, 128);
    pPacket[PacketInd++].u_u64[1] = SCE_GS_ALPHA_2;
    pPacket[PacketInd].u_u64[0] = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMZ16S, 0);
    pPacket[PacketInd].u_u64[1] = SCE_GS_ZBUF_2;

    pPacket[0].u_u128 = 0;
    pPacket[0].u_u32[0] = 0x70000000 | PacketInd;
    pPacket[1].u_u64[0] = SCE_GIF_SET_TAG(PacketInd - 1, 0, 0, 0, 0, 1);
    pPacket[1].u_u64[1] = SCE_GIF_PACKED_AD;
    sceDmaSend(pDma.Gif, SPR_SRC(pPacket));
    sceDmaSync(pDma.Gif, 0, 0);

    SETVEC(RGBA, 0x80, 0x80, 0x80, 0x80);
    SETVEC(XYZ[0], 0x8000 - x * 16, 0x8000 - (y / 2) * 16, 0, 0);
    SETVEC(XYZ[1], 0x8000 - x * 16, 0x8000 + (y / 2) * 16, 0, 0);
    SETVEC(XYZ[2], 0x8000 + x * 16, 0x8000 - (y / 2) * 16, 0, 0);
    SETVEC(XYZ[3], 0x8000 + x * 16, 0x8000 + (y / 2) * 16, 0, 0);
    SETVEC(UV[0], 0x8 + u * 16, 0x8 + v * 16, 0, 0);
    SETVEC(UV[1], 0x8 + u * 16, 0x8 + (v + y) * 16, 0, 0);
    SETVEC(UV[2], 0x8 + (u + x) * 16, 0x8 + v * 16, 0, 0);
    SETVEC(UV[3], 0x8 + (u + x) * 16, 0x8 + (v + y) * 16, 0, 0);

    PacketInd = 0;
    pPacket[PacketInd].u_u128 = 0;
    pPacket[PacketInd++].u_u32[0] = 0x70000000 | 13;
    pPacket[PacketInd].u_u64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    pPacket[PacketInd++].u_u64[1] = SCE_GS_UV | SCE_GS_RGBAQ << 4 | SCE_GS_XYZ2 << 8;
    pPacket[PacketInd++].u_u128 = *(u128 *)UV[0];
    pPacket[PacketInd++].u_u128 = *(u128 *)RGBA;
    pPacket[PacketInd++].u_u128 = *(u128 *)XYZ[0];
    pPacket[PacketInd++].u_u128 = *(u128 *)UV[1];
    pPacket[PacketInd++].u_u128 = *(u128 *)RGBA;
    pPacket[PacketInd++].u_u128 = *(u128 *)XYZ[1];
    pPacket[PacketInd++].u_u128 = *(u128 *)UV[2];
    pPacket[PacketInd++].u_u128 = *(u128 *)RGBA;
    pPacket[PacketInd++].u_u128 = *(u128 *)XYZ[2];
    pPacket[PacketInd++].u_u128 = *(u128 *)UV[3];
    pPacket[PacketInd++].u_u128 = *(u128 *)RGBA;
    pPacket[PacketInd++].u_u128 = *(u128 *)XYZ[3];

    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, SPR_SRC(pPacket));
    sceDmaSync(pDma.Gif, 0, 0);
}

void func_00211F50(sceVu0IVECTOR *Xyz, s32 col, u64 Tex0, s32 u0, s32 v0, s32 u1, s32 v1) {
    sceVu0IVECTOR UV[4];
    sceVu0IVECTOR RGBA;
    qword_uni *pPacket;
    s32 PacketInd;

    RGBA[0] = col;
    pPacket = SPR_MEM;
    SETVEC(UV[0], 0x8 + u0 * 16, 0x8 + v0 * 16, 0, 0);
    SETVEC(UV[1], 0x8 + u0 * 16, 0x8 + (v0 + v1) * 16, 0, 0);
    SETVEC(UV[2], 0x8 + (u0 + u1) * 16, 0x8 + v0 * 16, 0, 0);
    SETVEC(UV[3], 0x8 + (u0 + u1) * 16, 0x8 + (v0 + v1) * 16, 0, 0);
    
    PacketInd = 0;
    pPacket[PacketInd].u_u128 = 0;
    pPacket[PacketInd++].u_u32[0] = 0x70000000 | 11;
    pPacket[PacketInd].u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 1, 0, 1, 1, 0), 0, 10);
    pPacket[PacketInd++].u_u64[1] = (u64)SCE_GIF_PACKED_AD | (u64)SCE_GS_RGBAQ << 4
        | (u64)SCE_GS_UV << 8 | (u64)SCE_GS_XYZF2 << 12
        | (u64)SCE_GS_UV << 16 | (u64)SCE_GS_XYZF2 << 20
        | (u64)SCE_GS_UV << 24 | (u64)SCE_GS_XYZF2 << 28
        | (u64)SCE_GS_UV << 32 | (u64)SCE_GS_XYZF2 << 36;
    pPacket[PacketInd].u_u64[0] = Tex0;
    pPacket[PacketInd++].u_u64[1] = SCE_GS_TEX0_2;
    pPacket[PacketInd++].u_u128 = *(u128 *)RGBA;
    pPacket[PacketInd++].u_u128 = *(u128 *)UV[0];
    pPacket[PacketInd++].u_u128 = *(u128 *)Xyz[0];
    pPacket[PacketInd++].u_u128 = *(u128 *)UV[1];
    pPacket[PacketInd++].u_u128 = *(u128 *)Xyz[1];
    pPacket[PacketInd++].u_u128 = *(u128 *)UV[2];
    pPacket[PacketInd++].u_u128 = *(u128 *)Xyz[2];
    pPacket[PacketInd++].u_u128 = *(u128 *)UV[3];
    pPacket[PacketInd++].u_u128 = *(u128 *)Xyz[3];
    
    pDma.Gif->chcr.TTE = 0;
    sceDmaSync(pDma.Gif, 0, 0);
    sceDmaSend(pDma.Gif, SPR_SRC(pPacket));
    sceDmaSync(pDma.Gif, 0, 0);
}