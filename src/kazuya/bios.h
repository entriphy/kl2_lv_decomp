#ifndef KZ_BIOS_H
#define KZ_BIOS_H

#include "kazuya.h"

typedef struct { // 0x10
    /* 0x0 */ u8 r0;
    /* 0x1 */ u8 g0;
    /* 0x2 */ u8 b0;
    /* 0x3 */ u8 a0;
    /* 0x4 */ f32 q0;
    /* 0x8 */ u16 x0;
    /* 0xa */ u16 y0;
    /* 0xc */ u32 z0;
} GS_GPOS;

typedef struct { // 0x60
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 prim;
    /* 0x28 */ u64 pad;
    /* 0x30 */ u64 tag1;
    /* 0x38 */ u64 tag1_regs;
    /* 0x40 */ GS_GPOS p[2];
} GS_LINE;

typedef struct { // 0x60
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 prim;
    /* 0x28 */ u64 tex0;
    /* 0x30 */ u8 r0;
    /* 0x31 */ u8 g0;
    /* 0x32 */ u8 b0;
    /* 0x33 */ u8 a0;
    /* 0x34 */ f32 q0;
    /* 0x38 */ u16 u0;
    /* 0x3a */ u16 v0;
    /* 0x3c */ u32 pad0;
    /* 0x40 */ u16 x0;
    /* 0x42 */ u16 y0;
    /* 0x44 */ u32 z0;
    /* 0x48 */ u16 u1;
    /* 0x4a */ u16 v1;
    /* 0x4c */ u32 pad1;
    /* 0x50 */ u16 x1;
    /* 0x52 */ u16 y1;
    /* 0x54 */ u32 z1;
    /* 0x58 */ u64 pad;
} GS_SPRT_UV;

typedef struct { // 0x80
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 prim;
    /* 0x28 */ u64 tex0;
    /* 0x30 */ u8 r0;
    /* 0x31 */ u8 g0;
    /* 0x32 */ u8 b0;
    /* 0x33 */ u8 a0;
    /* 0x34 */ f32 q0;
    /* 0x38 */ u16 u0;
    /* 0x3a */ u16 v0;
    /* 0x3c */ u32 pad0;
    /* 0x40 */ u16 x0;
    /* 0x42 */ u16 y0;
    /* 0x44 */ u32 z0;
    /* 0x48 */ u16 u1;
    /* 0x4a */ u16 v1;
    /* 0x4c */ u32 pad1;
    /* 0x50 */ u16 x1;
    /* 0x52 */ u16 y1;
    /* 0x54 */ u32 z1;
    /* 0x58 */ u16 u2;
    /* 0x5a */ u16 v2;
    /* 0x5c */ u32 pad2;
    /* 0x60 */ u16 x2;
    /* 0x62 */ u16 y2;
    /* 0x64 */ u32 z2;
    /* 0x68 */ u16 u3;
    /* 0x6a */ u16 v3;
    /* 0x6c */ u32 pad3;
    /* 0x70 */ u16 x3;
    /* 0x72 */ u16 y3;
    /* 0x74 */ u32 z3;
    /* 0x78 */ u64 pad4;
} GS_POLY_FT4_UV;

typedef struct { // 0x50
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 prim;
    /* 0x28 */ u8 r0;
    /* 0x29 */ u8 g0;
    /* 0x2a */ u8 b0;
    /* 0x2b */ u8 a0;
    /* 0x2c */ f32 q0;
    /* 0x30 */ u16 x0;
    /* 0x32 */ u16 y0;
    /* 0x34 */ u32 z0;
    /* 0x38 */ u16 x1;
    /* 0x3a */ u16 y1;
    /* 0x3c */ u32 z1;
    /* 0x40 */ u16 x2;
    /* 0x42 */ u16 y2;
    /* 0x44 */ u32 z2;
    /* 0x48 */ u16 x3;
    /* 0x4a */ u16 y3;
    /* 0x4c */ u32 z3;
} GS_POLY_F4;

typedef struct { // 0x70
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 prim;
    /* 0x28 */ u8 r0;
    /* 0x29 */ u8 g0;
    /* 0x2a */ u8 b0;
    /* 0x2b */ u8 a0;
    /* 0x2c */ f32 q0;
    /* 0x30 */ u16 x0;
    /* 0x32 */ u16 y0;
    /* 0x34 */ u32 z0;
    /* 0x38 */ u8 r1;
    /* 0x39 */ u8 g1;
    /* 0x3a */ u8 b1;
    /* 0x3b */ u8 a1;
    /* 0x3c */ f32 q1;
    /* 0x40 */ u16 x1;
    /* 0x42 */ u16 y1;
    /* 0x44 */ u32 z1;
    /* 0x48 */ u8 r2;
    /* 0x49 */ u8 g2;
    /* 0x4a */ u8 b2;
    /* 0x4b */ u8 a2;
    /* 0x4c */ f32 q2;
    /* 0x50 */ u16 x2;
    /* 0x52 */ u16 y2;
    /* 0x54 */ u32 z2;
    /* 0x58 */ u8 r3;
    /* 0x59 */ u8 g3;
    /* 0x5a */ u8 b3;
    /* 0x5b */ u8 a3;
    /* 0x5c */ f32 q3;
    /* 0x60 */ u16 x3;
    /* 0x62 */ u16 y3;
    /* 0x64 */ u32 z3;
    /* 0x68 */ u64 pad;
} GS_POLY_G4;

typedef struct { // 0x90
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 prim;
    /* 0x28 */ u64 tex0;
    /* 0x30 */ u16 u0;
    /* 0x32 */ u16 v0;
    /* 0x34 */ u32 pad0;
    /* 0x38 */ u8 r0;
    /* 0x39 */ u8 g0;
    /* 0x3a */ u8 b0;
    /* 0x3b */ u8 a0;
    /* 0x3c */ f32 q0;
    /* 0x40 */ u16 x0;
    /* 0x42 */ u16 y0;
    /* 0x44 */ u32 z0;
    /* 0x48 */ u16 u1;
    /* 0x4a */ u16 v1;
    /* 0x4c */ u32 pad1;
    /* 0x50 */ u8 r1;
    /* 0x51 */ u8 g1;
    /* 0x52 */ u8 b1;
    /* 0x53 */ u8 a1;
    /* 0x54 */ f32 q1;
    /* 0x58 */ u16 x1;
    /* 0x5a */ u16 y1;
    /* 0x5c */ u32 z1;
    /* 0x60 */ u16 u2;
    /* 0x62 */ u16 v2;
    /* 0x64 */ u32 pad2;
    /* 0x68 */ u8 r2;
    /* 0x69 */ u8 g2;
    /* 0x6a */ u8 b2;
    /* 0x6b */ u8 a2;
    /* 0x6c */ f32 q2;
    /* 0x70 */ u16 x2;
    /* 0x72 */ u16 y2;
    /* 0x74 */ u32 z2;
    /* 0x78 */ u16 u3;
    /* 0x7a */ u16 v3;
    /* 0x7c */ u32 pad3;
    /* 0x80 */ u8 r3;
    /* 0x81 */ u8 g3;
    /* 0x82 */ u8 b3;
    /* 0x83 */ u8 a3;
    /* 0x84 */ f32 q3;
    /* 0x88 */ u16 x3;
    /* 0x8a */ u16 y3;
    /* 0x8c */ u32 z3;
} GS_POLY_GT4_UV;

typedef struct { // 0x50
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 prim;
    /* 0x28 */ u8 r0;
    /* 0x29 */ u8 g0;
    /* 0x2a */ u8 b0;
    /* 0x2b */ u8 a0;
    /* 0x2c */ f32 q0;
    /* 0x30 */ u16 x0;
    /* 0x32 */ u16 y0;
    /* 0x34 */ u32 z0;
    /* 0x38 */ u16 x1;
    /* 0x3a */ u16 y1;
    /* 0x3c */ u32 z1;
    /* 0x40 */ u16 x2;
    /* 0x42 */ u16 y2;
    /* 0x44 */ u32 z2;
    /* 0x48 */ u64 pad;
} GS_POLY_F3;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 alpha;
    /* 0x28 */ u64 addr;
} GS_ALPHA;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 clamp;
    /* 0x28 */ u64 addr;
} GS_CLAMP;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 fba;
    /* 0x28 */ u64 addr;
} GS_FBA;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 frame;
    /* 0x28 */ u64 addr;
} GS_FRAME;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 pabe;
    /* 0x28 */ u64 addr;
} GS_PABE;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 test;
    /* 0x28 */ u64 addr;
} GS_TEST;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 tex1;
    /* 0x28 */ u64 addr;
} GS_TEX1;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 texa;
    /* 0x28 */ u64 addr;
} GS_TEXA;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 texflush;
    /* 0x28 */ u64 addr;
} GS_TEXFLUSH;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 xyoffset;
    /* 0x28 */ u64 addr;
} GS_XYOFFSET;

typedef struct { // 0x30
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 zbuf;
    /* 0x28 */ u64 addr;
} GS_ZBUF;

// idk
typedef struct { // 0x80
    /* 0x00 */ u64 DMAtag;
    /* 0x08 */ u64 DMApad;
    /* 0x10 */ u64 GIFtag;
    /* 0x18 */ u64 GIFtag_regs;
    /* 0x20 */ u64 bitbltbuf;
    /* 0x28 */ u64 bitbltbuf_addr;
    /* 0x30 */ u64 trxpos;
    /* 0x38 */ u64 trxpos_addr;
    /* 0x40 */ u64 trxreg;
    /* 0x48 */ u64 trxreg_addr;
    /* 0x50 */ u64 trxdir;
    /* 0x58 */ u64 trxdir_addr;
    /* 0x60 */ u64 hwreg;
    /* 0x68 */ u64 hwreg_addr;
    /* 0x70 */ u64 texflush;
    /* 0x78 */ u64 texflush_addr;
} GS_DRAWPIXEL;

extern GAMEDATA gamdat;

extern void kz_Set_Line(DVECTOR *sxy0, DVECTOR *sxy1, u32 sz, KZCOLOR *col);
extern void kz_Set_SprtUV(DVECTOR *sxy, u32 sz, DVECTOR *swh, DVECTOR *uv, KZCOLOR *col);
extern void kz_Set_SprtUV_Eff(DVECTOR *sxy, u32 sz, DVECTOR *swh, DVECTOR *uv, KZCOLOR *col);
extern void kz_Set_TileF4(DVECTOR *sxy, u32 sz, DVECTOR *swh, KZCOLOR *col);
extern void kz_Set_TileG4(DVECTOR *sxy, u32 sz, DVECTOR *swh, KZCOLOR *col0, KZCOLOR *col1, KZCOLOR *col2, KZCOLOR *col3);
extern void kz_Set_TileFT4UV(DVECTOR *sxy, u32 sz, DVECTOR *swh, DVECTOR *uv, DVECTOR *twh, KZCOLOR *col);
extern void kz_Set_TileGT4UV(DVECTOR *sxy, u32 sz, DVECTOR *swh, DVECTOR *uv, DVECTOR *twh, KZCOLOR *col0, KZCOLOR *col1, KZCOLOR *col2, KZCOLOR *col3);
extern void kz_Set_PolyF3(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, KZCOLOR *col);
extern void kz_Set_PolyF4(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, KZCOLOR *col);
extern void kz_Set_PolyG4(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, KZCOLOR *col0, KZCOLOR *col1, KZCOLOR *col2, KZCOLOR *col3);
extern void kz_Set_PolyFT4UV(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, DVECTOR *uv0, DVECTOR *uv1, DVECTOR *uv2, DVECTOR *uv3, KZCOLOR *col);
extern void kz_Set_PolyFT4UV_NC(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, DVECTOR *uv0, DVECTOR *uv1, DVECTOR *uv2, DVECTOR *uv3, KZCOLOR *col);
extern void kz_Set_PolyGT4UV(sceVu0IVECTOR *crd0, sceVu0IVECTOR *crd1, sceVu0IVECTOR *crd2, sceVu0IVECTOR *crd3, DVECTOR *uv0, DVECTOR *uv1, DVECTOR *uv2, DVECTOR *uv3, KZCOLOR *col0, KZCOLOR *col1, KZCOLOR *col2, KZCOLOR *col3);
extern void kz_Set_Alpha(u32 ctxt, u64 A, u64 B, u64 C, u64 D, u64 FIX);
extern void kz_Set_Clamp(u32 ctxt, u64 WMS, u64 WMT, u64 MINU, u64 MAXU, u64 MINV, u64 MAXV);
extern void kz_Set_Fba(u32 ctxt, u64 FBA);
extern void kz_Set_Frame(u32 ctxt, u64 FBP, u64 FBW, u64 PSM, u64 FBMSK);
extern void kz_Set_Pabe(u64 PABE);
extern void kz_Set_Test(u32 ctxt, u64 ATE, u64 ATST, u64 AREF, u64 AFAIL, u64 DATE, u64 DATM, u64 ZTE, u64 ZTST);
extern void kz_Set_Tex0(u64 TBP0, u64 TBW, u64 PSM, u64 TW, u64 TH, u64 TCC, u64 TFX, u64 CBP, u64 CPSM, u64 CSM, u64 CSA, u64 CLD);
extern void kz_Set_Tex0_Imd(u64 tex0);
extern void kz_Set_Tex1(u32 ctxt, u64 LCM, u64 MXL, u64 MMAG, u64 MMIN, u64 MTBA, u64 L, u64 K);
extern void kz_Set_Texa(u64 TA0, u64 AEM, u64 TA1);
extern void kz_Set_Texflush();
extern void kz_Set_Xyoffset(u32 ctxt, u64 OFX, u64 OFY);
extern void kz_Set_Zbuf(u32 ctxt, u64 ZBP, u64 PSM, u64 ZMSK);
extern void kz_Set_DrawPixel(u64 DBP, u64 DSAX, u64 DSAY, KZCOLOR *col0, KZCOLOR *col1);
extern void kz_Set_FogDist(s32 near, s32 far);
extern void kz_Set_FogCol(sceVu0IVECTOR col);
extern void kz_Set_Box(DVECTOR *sxy, u32 sz, DVECTOR *swh, s32 thick, KZCOLOR *col);
extern void kz_Set_Box_VGrad(DVECTOR *sxy, u32 sz, DVECTOR *swh, s32 thick, KZCOLOR *col0,KZCOLOR *col1);
extern void kz_Set_Box_HGrad(DVECTOR *sxy, u32 sz, DVECTOR *swh, s32 thick, KZCOLOR *col0, KZCOLOR *col1);
extern s32 kz_Get_KazuyaStageNo();
extern s32 kz_Get_Cont_NormalStageNo();
extern s32 kz_Get_Cont_BossStageNo();
extern s32 kz_Get_AreaNo();
extern s32 kz_Get_StageMode();
extern s32 kz_Get_RetryVision();
extern s32 kz_Check_Clear_CurrentStage();
extern s32 kz_Get_PlaceStrNo();
extern s32 kz_Check_GenjuTotalTime();
extern s32 kz_Get_FinishedDolls();
extern s32 kz_Get_YumeCompleteStages();
extern void kz_Set_KL2System_Light(s32 light_no);
extern BGM_LIST_INFO* kz_Get_Ptr_BgmListInfo();
extern BGM_INFO* kz_Get_Ptr_BgmInfo(s32 index);
extern s32 kz_Get_BgmIndex_Prev();
extern s32 kz_Get_BgmIndex_Next();
extern s32 kz_Get_BgmNameStr(s32 index);
extern void kz_Init_SoundEffect(s32 se_no);
extern void kz_KeyOn_SoundEffect(s32 se_no);
extern void kz_Load_CompGms(s32 gms_no);
extern f32 kz_Get_Coef_Intpl(s32 iNow, s32 iStart, s32 iEnd);
extern void kz_Intpl_Color(f32 coef, KZCOLOR *dst, KZCOLOR *src0, KZCOLOR *src1);
extern void func_00173BA0(void *out, s32 size, char *filename);


#endif
