#ifndef HR_MAPDR_H
#define HR_MAPDR_H

#include "harada.h"
#include "harada/hr_main.h"
#include "harada/h_vpm2.h"

// TODO: pretty sure this belongs in a Kazuya header
typedef struct { // 0x50
    /* 0x00 */ qword DMAtag;
    /* 0x10 */ qword VIFtag0;
    /* 0x20 */ sceVu0FVECTOR foga;
    /* 0x30 */ qword VIFtag1;
    /* 0x40 */ sceVu0FVECTOR fogb;
} KVM_FOGDIST;

typedef struct { // 0x40
    /* 0x00 */ qword DMAtag;
    /* 0x10 */ qword VIFtag;
    /* 0x20 */ qword GIFtag;
    /* 0x30 */ u64 fogcol;
    /* 0x38 */ u64 GSreg;
} KGS_FOGCOL;

typedef struct { // 0x8
    /* 0x0 */ f32 len;
    /* 0x4 */ u32 no;
} HRSORT_LEN;

typedef struct { // 0x20
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword viftag;
} HR_VIFWAIT;

typedef struct { // 0x250
    /* 0x000 */ qword dmatag;
    /* 0x010 */ qword t00;
    /* 0x020 */ sceVu0FMATRIX lc;
    /* 0x060 */ qword t01;
    /* 0x070 */ sceVu0FMATRIX ll;
    /* 0x0b0 */ qword t02;
    /* 0x0c0 */ sceVu0FMATRIX lv;
    /* 0x100 */ qword t03;
    /* 0x110 */ sceVu0FMATRIX ls;
    /* 0x150 */ qword t04;
    /* 0x160 */ sceVu0FVECTOR foga;
    /* 0x170 */ qword t05;
    /* 0x180 */ sceVu0FVECTOR fogb;
    /* 0x190 */ qword t06;
    /* 0x1a0 */ sceVu0FVECTOR defcol;
    /* 0x1b0 */ qword t07;
    /* 0x1c0 */ qword gtag_tex0;
    /* 0x1d0 */ qword t08;
    /* 0x1e0 */ qword tex0_n;
    /* 0x1f0 */ qword t09;
    /* 0x200 */ qword tex0_r;
    /* 0x210 */ qword t10;
    /* 0x220 */ qword alpha;
    /* 0x230 */ qword t11;
    /* 0x240 */ qword block_offset;
} ATR_MAIN;

typedef struct { // 0x60
    /* 0x00 */ qword dma;
    /* 0x10 */ qword vif;
    /* 0x20 */ qword gif;
    /* 0x30 */ qword test;
    /* 0x40 */ qword alpha;
    /* 0x50 */ qword tex1;
} ATR_HRMPREG;

typedef struct { // 0xd0
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword t00;
    /* 0x20 */ sceVu0FVECTOR fixednum;
    /* 0x30 */ qword t01;
    /* 0x40 */ sceVu0FVECTOR clipnum;
    /* 0x50 */ qword t02;
    /* 0x60 */ qword giftag_tmp;
    /* 0x70 */ qword t10;
    /* 0x80 */ qword alpha;
    /* 0x90 */ qword t07;
    /* 0xa0 */ qword gtag_tex0;
    /* 0xb0 */ qword dblbuf;
    /* 0xc0 */ qword mscal;
} ATR_VU1_MAPS;

typedef struct { // 0xb0
    /* 0x00 */ qword dma;
    /* 0x10 */ qword vif;
    /* 0x20 */ qword gif;
    /* 0x30 */ qword rgb0;
    /* 0x40 */ qword xyz0;
    /* 0x50 */ qword rgb1;
    /* 0x60 */ qword xyz1;
    /* 0x70 */ qword rgb2;
    /* 0x80 */ qword xyz2;
    /* 0x90 */ qword rgb3;
    /* 0xa0 */ qword xyz3;
} ATR_VU1BACK;

typedef struct { // 0x30
    /* 0x00 */ qword vif;
    /* 0x10 */ qword gif;
    /* 0x20 */ qword frame1;
} ATR_HRFXZ;

typedef struct { // 0x80
    /* 0x00 */ qword vif;
    /* 0x10 */ qword gif;
    /* 0x20 */ sceGsClear clear;
} ATR_HRCLR;

typedef struct { // 0xc0
    /* 0x00 */ qword dmatag;
    /* 0x10 */ ATR_HRFXZ fxz;
    /* 0x40 */ ATR_HRCLR clr;
} ATR_MIRR_0;

typedef struct { // 0x50
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword t02;
    /* 0x20 */ sceVu0FVECTOR smax;
    /* 0x30 */ qword t03;
    /* 0x40 */ sceVu0FVECTOR smin;
} ATR_MIRR_1;

typedef struct { // 0x50
    /* 0x00 */ qword dmatag;
    /* 0x10 */ ATR_HRFXZ fxz;
    /* 0x40 */ qword vifwait;
} ATR_MIRR_RET;

typedef struct { // 0xf0
    /* 0x00 */ qword DMAtag;
    /* 0x10 */ qword gifc;
    /* 0x20 */ sceGsClear clear;
    /* 0x80 */ qword gif0;
    /* 0x90 */ qword tex0;
    /* 0xa0 */ qword clamp1;
    /* 0xb0 */ qword test;
    /* 0xc0 */ qword alpha;
    /* 0xd0 */ qword texa;
    /* 0xe0 */ qword gif1;
} ATR_MIRR_FAN3;

extern void DecodeVpm(u32 *vpm_top);
extern void DecodeVpm2(u32 *vpm_top, vpmINFO *vpmi);
extern void DecodeVpmMini(u32 *vpm_top, mINFO *vpmi);
extern void DecodeVpmTag(u32 *vpm_top);
extern void DecodeVpmS(u32 *vpm_top, vpmINFO *vpmi);
extern void DecodeVpmMiniS(u32 *vpm_top, mINFO *vpmi);
extern void hr_dmavif(u32 fsh2, u32 fsh3);
extern void hrSetObjAttrUnpack(qword tag, s32 vu_addr, s32 qwc);
extern void hrSetObjAttrMscal(qword tag, s32 vu_addr);
extern void hrSetObjAttrDBlbuf(qword tag, u32 base, u32 offset);
extern void hrInitAtrMain();
extern void hrSetMapREG();
extern void hrInitAtrVu1mIv(u64 tag0, u64 tag2, s32 mpgno, HRSCRST *scrst);
extern void hrSetBlockList(vpmINFO *vpmi, VPCLIP *vpc, sceVu0FMATRIX wvm);
extern u32 hrSetBlockListSL(vpmINFO *vpmi, VPCLIP *vpc, sceVu0FMATRIX wvm, f32 len, sceVu0FVECTOR pos);
extern u32 hrSetBlockListSL2(vpmINFO *vpmi, VPCLIP *vpc, sceVu0FMATRIX wvm, f32 len, sceVu0FVECTOR pos, sceVu0FVECTOR pos2);
extern void hrSetClipRoadParams();
extern void hr_set_vpmblock(vpmINFO *vpmi);
extern void hr_set_vpmblockM(mINFO *vpmi);
extern void hrDrawMap();
extern void hrDrawMTex();
extern void hrDrawVpo_st();
extern void hrDrawVpo(sceVu0FMATRIX m, u32 *top, u32 objno);
extern void hrDrawVpoi(sceVu0FMATRIX m, VPOINFO *info, u32 objno);
extern void hrDrawVpoLt(sceVu0FMATRIX nlm, sceVu0FMATRIX lcm);
extern void hrDrawVpo_end();
extern VPOINFO* hrget_vpoinfo(s32 id);
extern void hrDrawBG();
extern void hrDrawBack();
extern void hrDrawMir(s32 no);
extern void hrPathFlushOld();
extern void hrPathClear();
extern void hrDrawMirT();
extern void hrDrawAnmMap();

#endif
