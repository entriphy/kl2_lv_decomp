#ifndef H_PACKET_H
#define H_PACKET_H

#include "hoshino.h"

typedef struct { // 0x88
    /* 0x00 */ sceDmaChan *dmaGif;
    /* 0x04 */ u64 *pTop;
    /* 0x08 */ u64 *p;
    /* 0x0c */ u64 *pGiftag;
    /* 0x10 */ u64 *pDmatag;
    /* 0x14 */ u64 *pDmatagSV;
    /* 0x18 */ s32 size;
    /* 0x1c */ s32 eopchk;
    /* 0x20 */ u64 prim_def;
    /* 0x28 */ u64 prim;
    /* 0x30 */ s32 bp;
    /* 0x34 */ s32 tbp;
    /* 0x38 */ s32 cbp;
    /* 0x40 */ u64 tex0tmp;
    /* 0x48 */ u64 ctxt;
    /* 0x50 */ u64 tpcl;
    /* 0x58 */ u64 tex0[2];
    /* 0x68 */ u64 tex1[2];
    /* 0x78 */ u64 tex2[2];
} hPACKET_DATA;

extern void hDmaSync(sceDmaChan *DmaChan);
extern void hPkInit(hPACKET_DATA *pk_data);
extern void hPkReset(u128 *pk_area);
extern s32  hPkSetBp(s32 bp);
extern s32  hPkGetSize();
extern void hPkSend();
extern void hSetPrimDefault();
extern void hSetFST(s32 i);
extern void hSetCTXT(s32 i);
extern void hSetFIX(s32 i);
extern void hSetFog(s32 i);
extern void hSetAlpha(s32 i);
extern void hSetAnti(s32 i);
extern void hSetPoint();
extern void hSetLineF2();
extern void hSetLineG2();
extern void hSetLineF2S();
extern void hSetLineG2S();
extern void hSetPolyF3();
extern void hSetPolyG3();
extern void hSetPolyF3S();
extern void hSetPolyG3S();
extern void hSetPolyF3F();
extern void hSetPolyG3F();
extern void hSetPolyFT3();
extern void hSetPolyGT3();
extern void hSetPolyFT3S();
extern void hSetPolyGT3S();
extern void hSetPolyFT3F();
extern void hSetPolyGT3F();
extern void hSetTile();
extern void hSetSprite();
extern void hSetTEXnDefault();
extern void hSetRGB(s32 r, s32 g, s32 b);
extern void hSetRGBA(s32 r, s32 g, s32 b, s32 a);
extern void hSetRGBv(u8 *c);
extern void hSetRGBAv(u8 *c);
extern void hSetAlphaMode(s32 mode);
extern void hSetXYZ(s32 x, s32 y, s32 z);
extern void hSetXYZv(s32 *v);
extern void hSetXYZs(s32 x, s32 y, s32 z);
extern void hSetTpCl(s64 tbp0, s64 psm, s64 tw, s64 th, s64 cbp, s64 cpsm);
extern void hSetTex0(u64 tex0);
extern void hSetUV(s32 u, s32 v);
extern void hSetUVs(s32 u, s32 v);
extern void hSetUVp(s64 uv);
extern void hAddPrim();
extern void hAddDmaTag();
extern void hAddGim(s128 *buff, s32 mode);
extern u64  hGetTex0Tmp();
extern s32  hGet2n(s32 i);

#endif
