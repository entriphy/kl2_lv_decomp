#include <stdarg.h>
#include "okanoyo/okprint.h"
#include "okanoyo/okmain.h"
#include "hoshino/h_file.h"

u8 testfont[8192];

void OkSetIDprofile() {
    fontDispID = profile.fontID;
}

void OkSetIDprofileRoot() {
    fontDispID = 0;
}

void OkPrintDataLoad() {
    s32 *adr = (s32 *)hGetSysDataAddr(1);
    fontTexID = OkSendTex(NULL, (char *)adr + adr[1]);
}

void OkReSendTex(char *buff) {
    TEXINFO *texinf = texinfo;
    s32 *adr = (s32 *)hGetSysDataAddr(1);
    OkReLoad_tim((u32 *)((char *)adr + adr[1]), texinf);
}

void OkPrint(s32 personal_id, s16 x, s16 y, u8 *scr, char *format, ...) {
    /* s2 18 */ va_list ap;
    /* a2 6 */ s32 val;
    /* t1 9 */ s32 raw;
    /* s0 16 */ s32 scr_x;
    /* s6 22 */ s32 scr_y;
    /* s4 20 */ s32 ofs;
    /* a1 5 */ s32 i;
    /* a0 4 */ s32 hit;
    /* a0 4 */ s32 div;
    /* t3 11 */ s32 flag;
    /* -0x2f0(sp) */ char buf[256];
    /* -0x1f0(sp) */ char str[256];
    /* s3 19 */ s32 cnt;
    /* f0 38 */ f32 valf;
    /* s1 17 */ char *strp;
    /* a0 4 */ char tmp;
    /* s7 23 */ s16 org_x;

    // Empty function
}

void OkPrintSysInit(s32 personal_id, s32 screenw, s32 screenh, s32 fontw, s32 fonth) {
    PDbuf[personal_id].fsz_x = fontw;
    PDbuf[personal_id].fsz_y = fonth;
    PDbuf[personal_id].scr_x = screenw / fontw;
    PDbuf[personal_id].scr_y = screenh / fonth;
}

void OkDefaultSysInit(s32 screenw, s32 screenh, s32 fontw, s32 fonth) {
    s32 i;
    s32 j;

    for (i = 0; i < 9; i++) {
        PDbuf[i].fsz_x = fontw;
        PDbuf[i].fsz_y = fonth;
        PDbuf[i].scr_x = screenw / fontw;
        PDbuf[i].scr_y = screenh / fonth;
        for (j = 0; j < 280; j++) {
            // what
        }
    }
}

void OkFontScreenClr(s64 *scr) {
    s32 i;

    for (i = 0; i < 280; i++, scr++) {
        *scr = 0;
    }
}

void OkFontFlush(/* a0 4 */ s32 personal_id, /* -0xb0(sp) */ u8 *scr) {
	/* s0 16 */ TEXINFO *info = &texinfo[fontTexID];
	/* v0 2 */ s32 fsz_x = PDbuf[personal_id].fsz_x;
	/* v1 3 */ s32 fsz_y = PDbuf[personal_id].fsz_y;
	/* s7 23 */ s32 scr_x = PDbuf[personal_id].scr_x;
	/* -0xac(sp) */ s32 scr_y = PDbuf[personal_id].scr_y;
	/* -0xa8(sp) */ s32 w = fsz_x << 4;
	/* -0xa4(sp) */ s32 h = fsz_y << 4;
	/* a2 6 */ QWdata *buf;
	/* a1 5 */ s32 u;
	/* a0 4 */ s32 v;
	/* s1 17 */ s32 i;
	/* s0 16 */ s32 j;
	/* a3 7 */ s32 loop;
	/* s3 19 */ sceDmaChan *dmaVif;
	/* s8 30 */ QWdata *top;
	/* s6 22 */ QWdata *prim;
	/* a0 4 */ s32 envdata_c;
	/* -0x190(sp) */ s64 data[7][2] = {
        { SCE_GIF_SET_TAG(4, 0, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 0, 0), 0, 1), SCE_GIF_PACKED_AD },
        { SCE_GS_SET_TEX0(info->pbp, 2, SCE_GS_PSMT4, 7, 6, 1, 0, info->cbp, SCE_GS_PSMCT16, 0, 0, 1), SCE_GS_TEX0_1 },
        { SCE_GS_SET_TEX1(0, 0, 1, 1, 0, 0, 0), SCE_GS_TEX1_1 },
        { SCE_GS_SET_TEXA(32, 0, 0xFF), SCE_GS_TEXA },
        { SCE_GS_SET_ALPHA_1(0, 1, 0, 1, 0x80), SCE_GS_ALPHA_1 },
        { SCE_GIF_SET_TAG(1, 1, 0, 0, 1, 2), 0x10 },
        { 0x156, 0x180ffffff }
    };

    OkReSendTex((char *)testfont);
    dmaVif = sceDmaGetChan(2);
    dmaVif->chcr.TTE = 1;
    buf = SPR_MEM;
    buf[0].ul128 = 0;
    buf[0].ui32[0] = 0x70000000 | 7;
    buf[1].ul64[0] = SCE_GIF_SET_TAG(6, 1, 0, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 0, 0), 0, 1); // PRIM is set to 0...?
    buf[1].ul64[1] = SCE_GIF_PACKED_AD;
    buf[2].ul64[1] = SCE_GS_TEX0_1;
    buf[2].ul64[0] = SCE_GS_SET_TEX0(info->pbp, 2, SCE_GS_PSMT4, 7, 6, 1, 0, info->cbp, SCE_GS_PSMCT16, 0, 0, 1);
    buf[3].ul64[1] = SCE_GS_TEX1_1;
    buf[3].ul64[0] = SCE_GS_SET_TEX1(0, 0, 1, 1, 0, 0, 0);
    buf[4].ul64[1] = SCE_GS_CLAMP_1;
    buf[4].ul64[0] = SCE_GS_SET_CLAMP_1(1, 1, 0, 0, 0, 0);
    buf[5].ul64[1] = SCE_GS_TEXA;
    buf[5].ul64[0] = SCE_GS_SET_TEXA(32, 0, 0xFF);
    buf[6].ul64[1] = SCE_GS_TEST_1;
    buf[6].ul64[0] = SCE_GS_SET_TEST_1(1, 1, 0, 0, 0, 0, 0, 0);
    buf[7].ul64[1] = SCE_GS_TRXPOS;
    buf[7].ul64[0] = SCE_GS_SET_TRXPOS(0, 0, 0, 0, 0);
    FlushCache(WRITEBACK_DCACHE);
    sceDmaSend(dmaVif, SPR_SRC(buf));
    
    OkReSendTex((char *)testfont);
    dmaVif = sceDmaGetChan(2);
    dmaVif->chcr.TTE = 1;
    top = SPR_MEM;
    top->ul128 = 0;
    buf = SPR_MEM_IDX(1);
    for (envdata_c = 0, i = 0; i < 7; i++, buf++, envdata_c++) {
        buf->ul64[0] = data[i][0];
        buf->ul64[1] = data[i][1];
    }
    top->ui32[0] = 0x70000000 | envdata_c;
    FlushCache(WRITEBACK_DCACHE);
    sceDmaSend(dmaVif, SPR_SRC(top));
    sceGsSyncPath(0, 0);

    top = SPR_MEM;
    top->ul128 = 0;
    prim = top + 1;
    prim->ul64[1] = SCE_GS_UV | SCE_GS_XYZF2 << 4 | SCE_GS_UV << 8 | SCE_GS_XYZF2 << 12;
    buf = prim + 1;
    loop = 0;
    for (i = 0; i < scr_y; i++) {
        for (j = 0; j < scr_x; j++) {
            envdata_c = scr[i * scr_x + j];
            if (envdata_c) {
                u = envdata_c & 0xF;
                v = ((envdata_c - 0x20) >> 4) & 0xF;
                
                buf->ul64[0] = SCE_GS_SET_UV(u << 7, v << 7);
                buf->ul64[1] = SCE_GS_SET_XYZF2(w * j + 0x6C00, h * i + 0x7900, 0xFFFFFE, 0);
                loop++;
                buf++;
                buf->ul64[0] = SCE_GS_SET_UV((u + 1) << 7, (v + 1) << 7);
                // buf->ul64[1] = SCE_GS_SET_XYZF2(w * (j + 1) + 0x6C00, h * (i + 1) + 0x7900, 0xFFFFFE, 0);
                buf->ul64[1] = SCE_GS_SET_XYZF2(w + w * j + 0x6C00, h + h * i + 0x7900, 0xFFFFFE, 0);
                loop++;
                buf++;
                
                if (loop == 1022) {
                    prim->ul64[0] = SCE_GIF_SET_TAG(511, 1, 0, 0, 1, 4);
                    top->ui32[0] = 0x70000000 | 1023;
                    FlushCache(WRITEBACK_DCACHE);
                    sceDmaSend(dmaVif, SPR_SRC(top));
                    sceGsSyncPath(0, 0);
                    loop = 0;
                    buf = prim + 1;
                }
            }
        }
    }

    if (loop) {
        prim->ul64[0] = SCE_GIF_SET_TAG(loop / 2, 1, 0, 0, 1, 4);
        top->ui32[0] = (loop + 1) | 0x70000000;
        FlushCache(WRITEBACK_DCACHE);
        sceDmaSend(dmaVif, SPR_SRC(top));
        sceGsSyncPath(0, 0);
    }
}

void OkPFontFlush(u16 trg) {
    s32 i;

    OkWriteProfileChk();
    if (trg) {
        fontDispID++;
        fontDispID %= 9;
    }
    OkFontFlush(0, scr_p[0]);
    if (fontDispID != 0) {
        OkFontFlush(fontDispID, scr_p[fontDispID]);
    }
    for (i = 0; i < 9; i++) {
        OkFontScreenClr((s64 *)scr_p[i]);
    }
    for (i = 0; i < 16; i++) {
        // Empty loop
    }
    for (i = 0; i < 9; i++) {
        OkHtimeClr(i);
    }
}

void OkHtimeClrAll() {
    // Empty function
}

void OkHtimeClr(s32 id) {
    // Empty function
}

void OkSetHtimeStart(s32 personal_id, s64 col) {
    // Empty function
}

void OkSetHtimeEnd(s32 personal_id) {
    // Empty function
}

void OkDebugGame2pCon() {
    if (OkDebugGame2pTbl[fontDispID] != NULL) {
        OkDebugGame2pTbl[fontDispID]();
    }
}

void OkDebugGame2pConSet(s32 personal_id, void *prgptr) {
    OkDebugGame2pTbl[personal_id] = prgptr;
}

void OkDebugGame2pConInit(void *mainprgptr) {
    s32 i;

    for (i = 0; i < 9; i++) {
        OkDebugGame2pTbl[i] = mainprgptr;
    }
}