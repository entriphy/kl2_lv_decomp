#include "common.h"

static hPACKET_DATA *pk;

void hDmaSync(sceDmaChan *DmaChan) {
    while (*(vs32 *)(&DmaChan->chcr) & D_CHCR_STR_M);
}

void hPkInit(hPACKET_DATA *pk_data) {
    pk = pk_data;
    memset(pk, 0, sizeof(hPACKET_DATA));
    pk->dmaGif = sceDmaGetChan(DMAC_GIF);
}

void hPkReset(u128 *pk_area) {
    pk->pTop = (u64 *)pk_area;
    pk->pGiftag = NULL;
    pk->pDmatag = (u64 *)pk_area;
    pk->p = (u64 *)(pk_area + 1);
    pk->pDmatagSV = NULL;
    *(u64 *)pk_area = 0;
    pk->pDmatag[1] = 0;
    pk->prim = 0;
    pk->eopchk = 0;
    pk->size = 0;
    pk->ctxt = 0;

    hSetPrimDefault();
    hSetTEXnDefault();
}

s32 hPkSetBp(s32 bp) {
    pk->bp = bp;
    return (s32)pk; // ?
}

s32 hPkGetSize() {
    return ((s32)pk->p - (s32)pk->pDmatag) / 16 - 1;
}

void hPkSend() {
    s32 size;

    if (pk->size != 0) {
        size = hPkGetSize();
        if (size > 0) {
            *pk->pDmatag = (u64)(size | 0x70000000);
            if (pk->eopchk != 0)
                *(u64 *)((u32)pk->pGiftag) |= 0x8000;

        } else {
            if (pk->pDmatagSV != NULL)
                *(u64 *)((u32)pk->pDmatagSV) |= 0x70000000;
        }

        FlushCache(0);
        sceDmaSend(pk->dmaGif, pk->pTop);
        hDmaSync(pk->dmaGif);
    }
}

void hSetPrimDefault() {
    pk->prim_def = 0x100;
}

void hSetFST(s32 i) {
    pk->prim_def &= ~GS_PRIM_FST_M;
    pk->prim_def |= i << GS_PRIM_FST_O;
}

void hSetCTXT(s32 i) {
    pk->prim_def &= ~GS_PRIM_CTXT_M;
    pk->prim_def |= i << GS_PRIM_CTXT_O;
    pk->ctxt = i;
}

void hSetFIX(s32 i) {
    pk->prim_def &= ~GS_PRIM_FIX_M;
    pk->prim_def |= i << GS_PRIM_FIX_O;
}

void hSetFog(s32 i) {
    pk->prim |= GS_PRIM_FGE_M;
}

void hSetAlpha(s32 i) {
    pk->prim |= GS_PRIM_ABE_M;
}

void hSetAnti(s32 i) {
    pk->prim |= GS_PRIM_AA1_M;
}

void hSetPoint() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_POINT;
    pk->eopchk = 1;
}

void hSetLineF2() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_LINE;
    pk->eopchk = 1;
}

void hSetLineG2() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_LINE | SCE_GS_PRIM_IIP;
    pk->eopchk = 1;
}

void hSetLineF2S() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_LINESTRIP;
    pk->eopchk = 1;
}

void hSetLineG2S() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_LINESTRIP | SCE_GS_PRIM_IIP;
    pk->eopchk = 1;
}

void hSetPolyF3() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRI;
    pk->eopchk = 1;
}

void hSetPolyG3() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRI | SCE_GS_PRIM_IIP;
    pk->eopchk = 1;
}

void hSetPolyF3S() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRISTRIP;
    pk->eopchk = 1;
}

void hSetPolyG3S() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_IIP;
    pk->eopchk = 1;
}

void hSetPolyF3F() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_LINE;
    pk->eopchk = 1;
}

void hSetPolyG3F() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_LINE | SCE_GS_PRIM_IIP;
    pk->eopchk = 1;
}

void hSetPolyFT3() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRI | SCE_GS_PRIM_TME;
    pk->eopchk = 1;
}

void hSetPolyGT3() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRI | SCE_GS_PRIM_TME | SCE_GS_PRIM_IIP;
    pk->eopchk = 1;
}

void hSetPolyFT3S() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_TME;
    pk->eopchk = 1;
}

void hSetPolyGT3S() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_TME | SCE_GS_PRIM_IIP;
    pk->eopchk = 1;
}

void hSetPolyFT3F() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_LINE | SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_TME;
    pk->eopchk = 1;
}

void hSetPolyGT3F() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_LINE | SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_TME | SCE_GS_PRIM_IIP;
    pk->eopchk = 1;
}

void hSetTile() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_SPRITE; // ?
    pk->eopchk = 1;
}

void hSetSprite() {
    pk->pGiftag = pk->p;
    pk->p += 2;
    pk->prim = SCE_GS_PRIM_SPRITE | SCE_GS_PRIM_TME;
    pk->eopchk = 1;
}

void hSetTEXnDefault() {
    s32 c;

    c = pk->ctxt;
    pk->eopchk = 1;
    pk->pGiftag = pk->p;
    *pk->p++ = SCE_GIF_SET_TAG(5, 0, 0, 0, 0, 1);
    *pk->p++ = SCE_GIF_PACKED_AD;
    pk->tex0[c] = SCE_GS_SET_TEX0(0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1);
    *pk->p++ = pk->tex1[c] = 0;
    *pk->p++ = c + SCE_GS_TEX1_1;
    *pk->p++ = SCE_GS_SET_CLAMP(1, 1, 0, 0, 0, 0);
    *pk->p++ = c + SCE_GS_CLAMP_1;
    *pk->p++ = SCE_GS_SET_TEXA(0, 1, 1);
    *pk->p++ = SCE_GS_TEXA;
    *pk->p++ = SCE_GS_SET_ALPHA(0, 1, 0, 1, 1);
    *pk->p++ = c + SCE_GS_ALPHA_1;
    *pk->p++ = SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2);
    *pk->p++ = c + SCE_GS_TEST_1;
    pk->size += 6;
}

void hSetRGB(s32 r, s32 g, s32 b) {
    *pk->p++ = SCE_GS_SET_RGBAQ(r, g, b, 0x80, 1);
    *pk->p++ = SCE_GS_RGBAQ;
    pk->size++;
}

void hSetRGBA(s32 r, s32 g, s32 b, s32 a) {
    *pk->p++ = SCE_GS_SET_RGBAQ(r, g, b, a, 1);
    *pk->p++ = SCE_GS_RGBAQ;
    pk->size++;
}

void hSetRGBv(u8 *c) {
    *pk->p++ = SCE_GS_SET_RGBAQ(c[0], c[1], c[2], 0x80, 1);
    *pk->p++ = SCE_GS_RGBAQ;
    pk->size++;
}

void hSetRGBAv(u8 *c) {
    *pk->p++ = SCE_GS_SET_RGBAQ(c[0], c[1], c[2], c[3], 1);
    *pk->p++ = SCE_GS_RGBAQ;
    pk->size++;
}

void hSetAlphaMode(s32 mode) {
    pk->prim |= SCE_GS_PRIM_ABE;

    switch (mode) {
        case 0:
            *pk->p++ = SCE_GS_SET_ALPHA(0, 1, 0, 1, 128);
            break;
        case 1:
            *pk->p++ = SCE_GS_SET_ALPHA(0, 2, 0, 1, 128);
            break;
        case 2:
            *pk->p++ = SCE_GS_SET_ALPHA(2, 0, 0, 1, 128);
            break;
    }

    *pk->p++ = pk->ctxt + SCE_GS_ALPHA_1;
    pk->size++;
}

void hSetXYZ(s32 x, s32 y, s32 z) {
    *pk->p++ = SCE_GS_SET_XYZ(x, y, z);
    *pk->p++ = SCE_GS_XYZ2;
    pk->size++;
}

void hSetXYZv(s32 *v) {
    *pk->p++ = SCE_GS_SET_XYZ(v[0], v[1], v[2]);
    *pk->p++ = SCE_GS_XYZ2;
    pk->size++;
}

void hSetXYZs(s32 x, s32 y, s32 z) {
    *pk->p++ = SCE_GS_SET_XYZ(x * 16 + 0x6c00, y * 16 + 0x7900, z);
    *pk->p++ = SCE_GS_XYZ2;
    pk->size++;
}

void hSetTpCl(s64 tbp0, s64 psm, s64 tw, s64 th, s64 cbp, s64 cpsm) {
    pk->tpcl = SCE_GS_SET_TEX0(tbp0, (1 << tw) / 64, psm, tw, th, 0, 0, cbp, cpsm, 0, 0, 0);
    *pk->p++ = pk->tpcl | pk->tex0[pk->ctxt];
    *pk->p++ = pk->ctxt + SCE_GS_TEX0_1;
    pk->size++;
}

void hSetTex0(u64 tex0) {
    if (tex0 == 0)
        *pk->p++ = pk->tex0tmp;
    else
        *pk->p++ = tex0;
    *pk->p++ = pk->ctxt + SCE_GS_TEX0_1;
    pk->size++;
}

void hSetUV(s32 u, s32 v) {
    *pk->p++ = SCE_GS_SET_UV(u, v);
    *pk->p++ = SCE_GS_UV;
    pk->size++;
}

void hSetUVs(s32 u, s32 v) {
    *pk->p++ = SCE_GS_SET_UV(u * 16 + 8, v * 16 + 8);
    *pk->p++ = SCE_GS_UV;
    pk->size++;
}

void hSetUVp(s64 uv) {
    *pk->p++ = uv;
    *pk->p++ = SCE_GS_UV;
    pk->size++;
}

void hAddPrim() {
    s32 size;

    size = ((s32)pk->p - (s32)pk->pGiftag) / 16;
    pk->pGiftag[0] = SCE_GIF_SET_TAG(size - 1, 0, 1, pk->prim | pk->prim_def, 4, 1);
    pk->pGiftag[1] = SCE_GIF_PACKED_AD;
    pk->size++;
}

void hAddDmaTag() {
    pk->pDmatagSV = pk->pDmatag;
    pk->pDmatag = pk->p;
    pk->p += 2;
    pk->pDmatag[0] = 0;
    pk->pDmatag[1] = 0;
    pk->eopchk = 0;
}

void hAddGim(s128 *buff, s32 mode) {
    hGIM_PHEAD *ph;
    hGIM_CHEAD *ch;
    s32 psize;
    s32 csize;
    s64 bw;

    ph = (hGIM_PHEAD *)(buff + 1);
    if (ph->PSM == SCE_GS_PSMT8)
        psize = (ph->PW * ph->PH) / 16;
    else
        psize = (ph->PW * ph->PH) / 32;

    ch = (hGIM_CHEAD *)ph + psize;
    ch++;

    if (mode == 0)
        pk->tbp = pk->bp;
    else
        pk->tbp = ph->TBP;

    *pk->p++ = SCE_GIF_SET_TAG(4, 1, 0, 0, 0, 1);
    *pk->p++ = SCE_GIF_PACKED_AD;

    bw = ph->PW / 64;
    // *pk->p++ = SCE_GS_SET_BITBLTBUF(0, bw, ph->PSM, pk->tbp, bw, ph->PSM);
    *pk->p++ = (u64)ph->PSM << 0x18 | (u64)ph->PSM << 0x38 | (u64)pk->tbp << 0x20 | (u64)bw << 0x10 | (u64)bw << 0x30;
    *pk->p++ = SCE_GS_BITBLTBUF;
    *pk->p++ = SCE_GS_SET_TRXPOS(0, 0, 0, 0, 0);
    *pk->p++ = SCE_GS_TRXPOS;
    *pk->p++ = SCE_GS_SET_TRXREG(ph->PW, ph->PH);
    *pk->p++ = SCE_GS_TRXREG;
    *pk->p++ = SCE_GS_SET_TRXDIR(0);
    *pk->p++ = SCE_GS_TRXDIR;
    *pk->p++ = SCE_GIF_SET_TAG(psize, 1, 0, 0, 2, 0);
    pk->p++;
    pk->size += 6;

    *pk->pDmatag = hPkGetSize() | 0x10000000;
    pk->pDmatag = pk->p;
    pk->p += 2;
    pk->pDmatag[0] = (u64)psize | 0x30000000 | (u64)(u32)(ph + 1) << 32;
    hAddDmaTag();

    if (mode == 0)
        pk->bp = ((psize + 15) / 16) + pk->tbp;

    if (ch->CPSM == SCE_GS_PSMCT16)
        csize = (ch->CW * ch->CH) / 8;
    else
        csize = (ch->CW * ch->CH) / 4;

    if (csize > 0) {
        if (mode == 0)
            pk->cbp = pk->bp;
        else
            pk->cbp = ch->CBP;

        *pk->p++ = SCE_GIF_SET_TAG(4, 1, 0, 0, 0, 1);
        *pk->p++ = SCE_GIF_PACKED_AD;

        bw = ch->CW / 64;
        *pk->p++ = (u64)pk->cbp << 0x20 | (u64)ch->CPSM << 0x18 | (u64)ch->CPSM << 0x38  | (u64)bw << 0x10 | (u64)bw << 0x30;
        *pk->p++ = SCE_GS_BITBLTBUF;
        *pk->p++ = SCE_GS_SET_TRXPOS(0, 0, 0, 0, 0);
        *pk->p++ = SCE_GS_TRXPOS;
        *pk->p++ = SCE_GS_SET_TRXREG(ch->CW, ch->CH);
        *pk->p++ = SCE_GS_TRXREG;
        *pk->p++ = SCE_GS_SET_TRXDIR(0);
        *pk->p++ = SCE_GS_TRXDIR;
        *pk->p++ = SCE_GIF_SET_TAG(csize, 1, 0, 0, 2, 0);
        pk->p++;
        pk->size += 6;

        *pk->pDmatag = hPkGetSize() | 0x10000000;
        pk->pDmatag = pk->p;
        pk->p += 2;
        pk->pDmatag[0] = (u64)csize | 0x30000000 | (u64)(u32)(ch + 1) << 32;
        hAddDmaTag();

        if (mode == 0)
            pk->bp = ((csize + 15) / 16) + pk->cbp;
    }

    *pk->p++ = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    *pk->p++ = SCE_GIF_PACKED_AD;
    *pk->p++ = 0;
    *pk->p++ = SCE_GS_TEXFLUSH;
    pk->size += 2;

    pk->tex0tmp = SCE_GS_SET_TEX0(pk->tbp, ph->PW / 64, ph->PSM, hGet2n(ph->PW), hGet2n(ph->PH), 1, 0, pk->cbp, ch->CPSM, 0, 0, 1);
}

u64 hGetTex0Tmp() {
    return pk->tex0tmp;
}

s32 hGet2n(s32 i) {
    s32 a;
    s32 n;

    a = 1;
    if (i == 0)
        return 0;

    for (n = 0; a < i; n++, a <<= 1) {

    }

    return n;
}
