#ifndef TARO_MOVIE_H
#define TARO_MOVIE_H

#include "taro.h"

extern void vtIPU_pr_vtIDEC_MOVIE(vtIDEC_MOVIE *movie, char *msg);
extern s32 vtIPU_Sync(s32 mode, u16 timeout);
extern void vtCpMovieStruct(vtIDEC_MOVIE *m1, vtIDEC_MOVIE *m0, u32 vram_addr);
extern void vtIPU_setTH(vtIDEC_MOVIE *movie, s32 maxVal, s32 minVal);
extern void vtIPU_setMabiki(vtIDEC_MOVIE *movie, u32 skipVal);
extern void vtIPU_setIncrementNum(vtIDEC_MOVIE *movie, s32 num);
extern void vtIPU_initVQ(vtIDEC_MOVIE *movie, u16 *ipuClut, u8 *gsClut, u32 cbp, u32 cpsm, u32 csm, u32 csa, u32 cld);
extern void vtIPU_init(vtIDEC_MOVIE *movie, s64 texaddr);
extern s32 vtIPU_getFilesize(char *file);
extern s32 vtIPU_readFile(char *file, u8 *buff, s32 filesize);
extern s32 vtIPU_setBsdata(vtIDEC_MOVIE *movie, u8 *data);
extern s32 vtIPU_getBsTags_buflen(vtIDEC_MOVIE *movie);
extern void vtIPU_setWorkBuff(vtIDEC_MOVIE *movie, u128 *bstags, u128 *dmaTags0, u128 *dmaTags1, sceIpuRGB32 *cscBuff0, sceIpuRGB32 *cscBuff1);
extern void vtIPU_mkDmaTagToIPU(vtIDEC_MOVIE *movie);
extern void vtIPU_sendBsdataToSPR(vtIDEC_MOVIE *movie, u32 ptrOnSPR);
extern void vtIPU_startDecode(vtIDEC_MOVIE *movie);
extern s32 vtIPU_decode(vtIDEC_MOVIE *movie);
extern s32 vtIPU_wait(vtIDEC_MOVIE *movie, s32 id);
extern s32 vtIPU_syncDecode(vtIDEC_MOVIE *movie);
extern s32 vtIPU_VQ(vtIDEC_MOVIE *movie);
extern s32 vtIPU_SetVQ(vtIDEC_MOVIE *movie, u16 *clut);
extern s32 vtIPU_VectorQuantization(vtIDEC_MOVIE *movie);
extern s32 vtIPU_IDCT(vtIDEC_MOVIE *movie);
extern void vtIPU_killDecode(vtIDEC_MOVIE *movie);
extern void vtIPU_finish(vtIDEC_MOVIE *movie);
extern u128* vtIPU_mkDmaTagOfSendTex(u128 *dmaTag, sceIpuRGB32 *image, vtIDEC_MOVIE *movie);
extern u128* vtIPU_mkDmaTagOfSendTexVIF(u128 *dmaTag, sceIpuRGB32 *image, vtIDEC_MOVIE *movie);
extern u128* vtIPU_mkDmaTagOfSendINDX4VIF(u128 *dmaTag, sceIpuINDX4 *image, vtIDEC_MOVIE *movie, u32 vram);
extern u128* vtIPU_mkTex0(u128 *dmaTag, vtIDEC_MOVIE *movie, s32 context);
extern u128* vtIPU_mkPacketForVQ(u128 *packet, vtIDEC_MOVIE *movie, s32 context);
extern u128* vtIPU_mkPacketForVQ2(u128 *packet, vtIDEC_MOVIE *movie, s32 context);
extern u128* vtIPU_mkTex0ForVQ(u128 *dmaTag, vtIDEC_MOVIE *movie, s32 context);
extern u128* vtIPU_sendClut(u128 *packet, u8 *clut, u32 cbp, u32 cpsm, u32 csm, u32 csa, u32 cld, s32 context);
extern s32 vtIPU_readDataSCE(char *file, vtIDEC_MOVIE *movie);

#define IPU_BCLR(bp) (SCE_IPU_BCLR | bp)
#define IPU_IDEC(fb, qsc, dtd, sgn, dte, ofm) (SCE_IPU_IDEC | fb | qsc << 16 | dtd << 24 | sgn << 25 | dte << 26 | ofm << 27)
#define IPU_BDEC(fb, qsc, dt, dcr, mbi) (SCE_IPU_BDEC | mbi << 27 | dcr << 26 | dt << 25 | qsc << 16 | fb);
#define IPU_FDEC(fb) (SCE_IPU_FDEC | fb)
#define IPU_SETVQ()  (SCE_IPU_SETVQ)
#define IPU_PACK(mbc, dte, ofm) (SCE_IPU_PACK | mbc | dte << 26 | ofm << 27)
#define IPU_SETTH(th0, th1) (SCE_IPU_SETTH | th1 << 16 | th0)
#define IPU_RESET 0x40000000
#define IPU_ECD (*IPU_CTRL & (1 << 14))

#endif