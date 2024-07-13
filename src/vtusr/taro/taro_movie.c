#include <sifdev.h>
#include "vtusr/taro/taro_movie.h"
#include "vtusr/vtutil.h"

u16 dmaStat = 0;
u16 _ipuTimeout = 0;

static u128* mkDmaTagToIPU_SCE(u128 *tags, u8 *data, s32 datasize);

void vtIPU_pr_vtIDEC_MOVIE(vtIDEC_MOVIE *movie, char *msg) {
    // Empty function
}

s32 vtIPU_Sync(s32 mode, u16 timeout) {
	return sceIpuSync(0, 0);
}

void vtCpMovieStruct(vtIDEC_MOVIE *m1, vtIDEC_MOVIE *m0, u32 vram_addr) {
    m1->type = m0->type;
    m1->bsDataSize = m0->bsDataSize;
    m1->width = m0->width;
    m1->height = m0->height;
    m1->nframes = m0->nframes;
    m1->frame_cnt = m0->frame_cnt;
    m1->thVal0 = m0->thVal0;
    m1->thVal1 = m0->thVal1;
    m1->incnum = m0->incnum;
    m1->field_0x18 = m0->field_0x18;
    m1->field_0x1a = m0->field_0x1a;
    m1->mbx = m0->mbx;
    m1->mby = m0->mby;
    m1->texbp = vram_addr;
    m1->texbw = m0->texbw;
    m1->tw = m0->tw;
    m1->th = m0->th;
    m1->cbp = m0->cbp;
    m1->cpsm = m0->cpsm;
    m1->csm = m0->csm;
    m1->csa = m0->csa;
    m1->cld = m0->cld;
    m1->killFg = m0->killFg;
    m1->top = m0->top;
    m1->bsData = m0->bsData;
    m1->bsData1Addr = m0->bsData1Addr;
    m1->bsData1Size = m0->bsData1Size;
    m1->bsTags = m0->bsTags;
    m1->currentBufNo = m0->currentBufNo;
    m1->dmaTags[0] = vtGetBuf64(0x6400);
    m1->dmaTags[1] = vtGetBuf64(0x6400);
    m1->cscBuff[0] = m0->cscBuff[0];
    m1->cscBuff[1] = m0->cscBuff[1];
    vtIPU_mkDmaTagOfSendTex(m1->dmaTags[0], m1->cscBuff[0], m1);
    vtIPU_mkDmaTagOfSendTex(m1->dmaTags[1], m1->cscBuff[1], m1);
    m1->vqBuff[0] = m0->vqBuff[0];
    m1->vqBuff[1] = m0->vqBuff[1];
    m1->idctBuff[0] = m0->idctBuff[0];
    m1->idctBuff[1] = m0->idctBuff[1];
}

void vtIPU_setTH(vtIDEC_MOVIE *movie, s32 maxVal, s32 minVal) {
    movie->thVal1 = maxVal > 255 ? 255 : maxVal < 0 ? 0 : maxVal;
    movie->thVal0 = minVal < 0 ? 0 : minVal > 255 ? 255 : minVal;
}

void vtIPU_setMabiki(vtIDEC_MOVIE *movie, u32 skipVal) {
    skipVal++;
    if (movie->type != 1) return;
    if (movie->nframes < skipVal) skipVal = movie->nframes;
    if (movie->type != 1) return; // what
    if (movie->incnum < 0) movie->incnum = -skipVal;
    else movie->incnum = skipVal;
}

void vtIPU_setIncrementNum(vtIDEC_MOVIE *movie, s32 num) {
    if (movie->type != 1) {
        movie->incnum = 1;
        return;
    }

    if (num > 0) {
        if (num > movie->nframes) {
            num = movie->nframes;
        }
    } else {
        if (movie->nframes > -num) {
            num = -movie->nframes;
        }
    }

    if (movie->type == 1) {
        movie->incnum = num;
    }
}

void vtIPU_initVQ(vtIDEC_MOVIE *movie, u16 *ipuClut, u8 *gsClut, u32 cbp, u32 cpsm, u32 csm, u32 csa, u32 cld) {
    movie->ipuClut = ipuClut;
    movie->gsClut = gsClut;
    if (cbp == 0) {
        cbp = movie->texbp + (s32)powf(2.0, (movie->tw + movie->th) - 6);
    }
    movie->cbp = cbp;
    movie->cpsm = cpsm;
    movie->csm = csm;
    movie->csa = csa;
    movie->cld = cld;
}

void vtIPU_init(vtIDEC_MOVIE *movie, s64 texaddr) {
    if (texaddr >= 0) {
        movie->texbp = texaddr;
    }
    movie->frame_cnt = 0;
    movie->currentBufNo = 0;
    movie->killFg = 0;
    *D_PCR |= D_PCR_CPC3_M | D_PCR_CPC4_M | D_PCR_CPC8_M | D_PCR_CPC9_M;
    *D_STAT = 0x8;
    *D_STAT = 0x10;
    *D_STAT = 0x100;
    *D_STAT = 0x200;
    *D_CTRL = *D_CTRL & 0x70f | 0x70;
    dmaStat = 0;
    sceIpuInit();
}

s32 vtIPU_getFilesize(char *file) {
	s32 fd;
	s32 filesize;

    fd = sceOpen(file, SCE_RDONLY);
    if (fd < 0) {
        return 0;
    } else {
        filesize = sceLseek(fd, 0, SEEK_END);
        sceClose(fd);
    }

    return filesize;
}

s32 vtIPU_readFile(char *file, u8 *buff, s32 filesize) {
	s32 fd;

    fd = sceOpen(file, SCE_RDONLY);
    if (fd < 0) {
        return 0;
    } else {
        if (sceRead(fd, buff, filesize) != filesize) {
            sceClose(fd);
            return 0;
        } else {
            sceClose(fd);
            return 1;
        }
    }
}

s32 vtIPU_setBsdata(vtIDEC_MOVIE *movie, u8 *data) {
    movie->field_0x18 = 0;
    movie->field_0x1a = 0;
    movie->bsDataSize = *(u32 *)(data + 0x4);
    movie->width = *(u16 *)(data + 0x8);
    movie->height = *(u16 *)(data + 0xA);
    movie->nframes = *(u32 *)(data + 0xC);;

    if (data[0] == 'v' && data[1] == 't' && data[2] == 'i' && data[3] == 'p') {
        movie->type = 1;
        movie->top = (u32 *)(data + 0x20);
        movie->bsData = data + ((int)(movie->nframes * 4 + 0x37) >> 4) * 0x10;
        movie->thVal0 = data[0x10];
        movie->thVal1 = data[0x11];
        movie->texbp = *(u32 *)(data + 0x14);
        movie->incnum = *(s16 *)(data + 0x12);
        movie->field_0x18 = *(u16 *)(data + 0x18);
        movie->field_0x1a = 0;
    } else if (data[0] == 'i' && data[1] == 'p' && data[2] == 'u' && data[3] == 'm') {
        movie->type = 0;
        movie->bsDataSize -= 8;
        movie->bsData = data + 0x10;
        movie->thVal0 = 0;
        movie->thVal1 = 0;
        movie->incnum = 1;
    } else if (data[0] == 'v' && data[1] == 't' && data[2] == 'i' && data[3] == 'c') {
        movie->type = 1;
        movie->thVal0 = data[0x10];
        movie->thVal1 = data[0x11];
        movie->incnum = *(s16 *)(data + 0x12);
        movie->field_0x18 = *(u16 *)(data + 0x18);
        movie->field_0x1a = 0;
        movie->texbp = *(u32 *)(data + 0x14);
        return 0;
    } else {
        return -1;
    }

    movie->mbx = movie->width >> 4;
    movie->mby = movie->height >> 4;
    movie->texbw = movie->width >> 6;
    movie->tw = 0;
    while (movie->width >> movie->tw > 1) movie->tw++;
    movie->th = 0;
    while (movie->height >> movie->th > 1) movie->th++;
    
    return 1;
}

s32 vtIPU_getBsTags_buflen(vtIDEC_MOVIE *movie) {
    if (movie->type == 0) {
        return 11;
    } else {
        return movie->nframes + 1;
    }
}

void vtIPU_setWorkBuff(vtIDEC_MOVIE *movie, u128 *bstags, u128 *dmaTags0, u128 *dmaTags1, sceIpuRGB32 *cscBuff0,sceIpuRGB32 *cscBuff1) {
    movie->bsTags = bstags;
    movie->dmaTags[0] = dmaTags0;
    movie->dmaTags[1] = dmaTags1;
    movie->cscBuff[0] = cscBuff0;
    movie->cscBuff[1] = cscBuff1;
}

void vtIPU_mkDmaTagToIPU(vtIDEC_MOVIE *movie) {
    if (movie->type == 0) {
        movie->incnum = 0;
        mkDmaTagToIPU_SCE(movie->bsTags, movie->bsData, movie->bsDataSize);
    }
}

static u128* _mk1DmaTagToIPU(vtIDEC_MOVIE *movie, u64 *ptr, u32 *size) {
	u64 *p;
	s32 i;
	s32 j;

    *size = 0;
    p = !((u32)ptr & 0x70000000) ? (u64 *)((u32)ptr | 0x20000000) : ptr;

    i = movie->frame_cnt;
    for (j = 0; j <= 0 && i < movie->nframes; i += movie->incnum, j++, p++) {
        *p++ = SET_DMAREF((movie->top[i + 1] >> 4) - (movie->top[i] >> 4), (u32)&movie->bsData[movie->top[i] & ~0xF]);
        *size = *size + ((movie->top[i + 1] >> 4) - (movie->top[i] >> 4));
    }

    i = movie->nframes;
    *p = SET_DMAREF(1, (u32)&movie->bsData[movie->top[i] & ~0xF]);
    *size += 1;
    *p++ &= ~0x30000000;
     p++;
    __asm__("sync");

    return (u128 *)(((u32)ptr & 0x70000000) ? p : (u64 *)((u32)p & 0x0FFFFFFF));
}

void vtIPU_sendBsdataToSPR(vtIDEC_MOVIE *movie, u32 ptrOnSPR) {
    s32 n;

    if (movie->type != 1) {
        return;
    }

    movie->bsData1Addr = ((ptrOnSPR + 0x3F) >> 6) << 6;
    n = movie->top[movie->frame_cnt];
    vtIPU_wait(movie, 9);
    vtIPU_wait(movie, 4);
    *D9_MADR = (u32)&movie->bsData[n] & ~0xF;
    *D9_QWC = (movie->top[movie->frame_cnt + 1] >> 4) - (n >> 4);
    *D9_SADR = movie->bsData1Addr;
    *D9_CHCR = 0x100;
    dmaStat |= 0x200;
    movie->bsData1Size = (((movie->top[movie->frame_cnt + 1] - n) + 0xF) >> 4) << 4;
}

static s32 _decodeFlag(u32 *flag) {
    *IPU_CMD = IPU_FDEC(0x00);
    sceIpuSync(0, 0);
    *flag = *IPU_CMD >> 24;
    if (vtIPU_Sync(0, _ipuTimeout) != 0) {
        return -1;
    }

    *IPU_CMD = IPU_FDEC(0x08);
    if (vtIPU_Sync(0, _ipuTimeout) != 0) {
        return -1;
    }

    *IPU_CTRL = SCE_IPU_CTRL_PCT_IPIC | (*flag & ~0x4) << 16;
    return 0;
}

void vtIPU_startDecode(vtIDEC_MOVIE *movie) {
    vtIPU_decode(movie);
}

static s32 vtIPU_decodeSCE(vtIDEC_MOVIE *movie) {
    sceIpuDmaEnv env;

    if (movie->frame_cnt != 0) {
        return 0;
    }

    *IPU_CTRL = IPU_RESET;
    if (vtIPU_Sync(0, _ipuTimeout) != 0) {
        return -1;
    }

    sceIpuStopDMA(&env);
    *IPU_CMD = IPU_BCLR(0);
    sceIpuRestartDMA(&env);
    if (vtIPU_Sync(0, _ipuTimeout) != 0) {
        return -1;
    }

    vtIPU_wait(movie, 4);
    *D4_TADR = (u32)movie->bsTags;
    *D4_MADR = 0;
    *D4_QWC = 0;
    *D4_CHCR = 0x104;
    dmaStat |= 0x10;
    return 0;
}

s32 vtIPU_decode(vtIDEC_MOVIE *movie) {
    u32 flag;
	sceIpuDmaEnv env;
	u8 *p;
	s32 i;
	s32 j;
    
    if (movie->type == 1) {
        i = (s32)(movie->frame_cnt + movie->incnum) % (s32)movie->nframes; // ...?
        movie->frame_cnt = i >= 0 ? i : i + movie->nframes;
    } else {
        movie->frame_cnt = (movie->frame_cnt + 1) % movie->nframes;
    }
    
    movie->currentBufNo ^= 1;
    if (movie->type == 0) {
        if (vtIPU_decodeSCE(movie) != 0) {
            return -1;
        }
    } else {
        *IPU_CTRL = IPU_RESET;
        if (vtIPU_Sync(0, _ipuTimeout) != 0) {
            return -1;
        }

        sceIpuStopDMA(&env);
        *IPU_CMD = IPU_BCLR(0);
        sceIpuRestartDMA(&env);
        if (vtIPU_Sync(0, _ipuTimeout) != 0) {
            return -1;
        }

        if (movie->killFg) {
            return 0;
        }

        vtIPU_sendBsdataToSPR(movie, 0x70000000);
        p = (u8 *)((0x70000000 | movie->bsData1Addr) + movie->bsData1Size);
        movie->bsData1Size += 19;
        movie->bsData1Size >>= 4;
        movie->bsData1Size <<= 4;
        vtIPU_wait(movie, 9);

        for (i = 0; i < 16; i++) {
            if (*(p - i - 1) == 0xB0 && *(p - i - 2) == 1) {
                (p - i)[0] = 0;
                (p - i)[1] = 0;
                (p - i)[2] = 1;
                (p - i)[3] = 0xB1;
                for (j = i - 4; j >= 0; j--) {
                    *(p - j) = 0;
                }
            }
        }

        vtIPU_wait(movie, 4);
        *D4_TADR = 0;
        *D4_MADR = (movie->bsData1Addr & 0x0FFFFFFF) | 0x80000000;
        *D4_QWC = movie->bsData1Size >> 4;
        *D4_CHCR = 0x100;
        dmaStat |= 0x10;
    }

    vtIPU_wait(movie, 3);
    *D3_MADR = (u32)movie->cscBuff[movie->currentBufNo];
    *D3_QWC = movie->mbx * movie->mby * sizeof(sceIpuRGB32) >> 4;
    *D3_CHCR = 0x100;
    dmaStat |= 0x8;
    *IPU_CMD = IPU_SETTH(movie->thVal0, movie->thVal1);
    if (_decodeFlag(&flag) == -1) {
        return -1;
    }
    *IPU_CMD = IPU_IDEC(0x00, 1, (flag >> 2 & 1), SCE_IPU_IDEC_NOOFFSET, SCE_IPU_IDEC_NODITHER, SCE_IPU_IDEC_RGB32);
    return 0;
}

static s32 _skipFrameDelimitaCode() {
    *IPU_CMD = IPU_FDEC(0x20);
    if (vtIPU_Sync(0, _ipuTimeout) != 0) {
        return -1;
    }
    return 0;
}

s32 vtIPU_wait(vtIDEC_MOVIE *movie, s32 id) {
    if (id == 3 || id == 4) {
        if (vtIPU_Sync(0, _ipuTimeout) != 0) {
            return -1;
        }
    }

    if (((dmaStat >> id) & 1) && !(*D_STAT & (1 << id))) {
        asm __volatile__(
            "    addi $6,$0,10000\n"
            "    sync.p\n"
            "VTIPU_WAIT:\n"
            "    bltz $6,VTIPU_BREAK\n"
            "    addi $6,$6,-1\n"
            "    bc0f VTIPU_WAIT\n"
            "    nop\n"
            "VTIPU_BREAK:\n"
            "    nop\n"
        );
    }

    dmaStat &= ~(1 << id);
    *D_STAT = 1 << id;
    return 0;
}

s32 vtIPU_syncDecode(vtIDEC_MOVIE *movie) {
    s32 i;
	sceIpuDmaEnv env;

    if (vtIPU_Sync(0, _ipuTimeout) != 0) {
        return -1;
    }

    vtIPU_wait(movie, 3);
    if (movie->killFg) {
        movie->killFg = 0;
        return 0;
    }

    *IPU_CMD = IPU_FDEC(0x20);
    if (vtIPU_Sync(0, _ipuTimeout) != 0) {
        return -1;
    }
    
    if (movie->type == 1) {
        for (i = movie->top[movie->frame_cnt] & 0xF; i >= 4; i -= 4) {
            *IPU_CMD = IPU_FDEC(0x20);
            if (vtIPU_Sync(0, _ipuTimeout) != 0) {
                return -1;
            }
        }
    
        if (i > 0) {
            *IPU_CMD = IPU_FDEC(i * 8);
            if (vtIPU_Sync(0, _ipuTimeout) != 0) {
                return -1;
            }
        }
    }

    return 0;
}

s32 vtIPU_VQ(vtIDEC_MOVIE *movie) {
    if (vtIPU_SetVQ(movie, movie->ipuClut) != 0) {
        return -1;
    }

    sceIpuSync(0, 0);
    return vtIPU_VectorQuantization(movie);
}

s32 vtIPU_SetVQ(vtIDEC_MOVIE *movie, u16 *clut) {
    sceIpuDmaEnv env;

    *IPU_CTRL = IPU_RESET;
    sceIpuSync(0, 0);
    sceIpuStopDMA(&env);

    *IPU_CMD = IPU_BCLR(0);
    sceIpuSync(0, 0);
    sceIpuRestartDMA(&env);
    vtIPU_wait(movie, 4);

    *D4_TADR = 0;
    *D4_MADR = (u32)clut;
    *D4_QWC = 2;
    *D4_CHCR = 0x100;
    dmaStat |= 0x10;
    *IPU_CMD = IPU_SETVQ();

    return 0;
}

s32 vtIPU_VectorQuantization(vtIDEC_MOVIE *movie) {
	sceIpuDmaEnv env;

    *IPU_CTRL = IPU_RESET;
    sceIpuSync(0, 0);
    sceIpuStopDMA(&env);

    *IPU_CMD = IPU_BCLR(0);
    sceIpuRestartDMA(&env);
    sceIpuSync(0, 0);
    vtIPU_wait(movie, 4);

    *D4_TADR = 0;
    *D4_MADR = (u32)movie->cscBuff[movie->currentBufNo];
    *D4_QWC = movie->mbx * movie->mby * sizeof(sceIpuRGB32) >> 4;
    *D4_CHCR = 0x100;
    dmaStat |= 0x10;
    vtIPU_wait(movie, 3);
    
    *D3_MADR = (u32)movie->vqBuff[movie->currentBufNo];
    *D3_QWC = movie->mbx * movie->mby * 0x80 >> 4;
    *D3_CHCR = 0x100;
    dmaStat = dmaStat | 8;
    *IPU_CMD = IPU_PACK(movie->mbx * movie->mby, SCE_IPU_PACK_NODITHER, SCE_IPU_PACK_INDX4);

    return 0;
}

s32 vtIPU_IDCT(vtIDEC_MOVIE *movie) {
	sceIpuDmaEnv env;
	u32 flag;
	sceIpuRGB32 *prgb;
	sceIpuRAW16 *praw;
	u32 *ppix;
	s16 *py;
	s32 y;
	s32 i;
	s32 j;
	s32 len;
	u32 skipBit;
	u32 qsc;
	struct { // 0x4
		u32 first : 1;
		u32 dcreset : 1;
		u32 dt : 1;
		u32 qsc : 1;
		u32 tmp : 28;
	} isDecodeFg;
	s32 cb1;
	s32 cb2;
	s32 cr1;
	s32 cr2;
	s32 r;
	s32 g;
	s32 b;

    prgb = movie->cscBuff[movie->currentBufNo];
    praw = movie->idctBuff[movie->currentBufNo];
    if (movie->type == 1) {
        movie->frame_cnt = (movie->frame_cnt + movie->incnum) % movie->nframes;
    } else {
        movie->frame_cnt = (movie->frame_cnt + 1) % movie->nframes;
    }
    movie->currentBufNo ^= 1;
    if (movie->type == 0) {
        return -1;
    }
    
    *IPU_CTRL = IPU_RESET;
    if (vtIPU_Sync(0, _ipuTimeout) != 0) {
        return -1;
    }
    
    sceIpuStopDMA(&env);
    *IPU_CMD = IPU_BCLR(0);
    sceIpuRestartDMA(&env);
    if (vtIPU_Sync(0, _ipuTimeout) != 0) {
        return -1;
    }
    
    vtIPU_sendBsdataToSPR(movie, 0x70000000);
    movie->bsData1Size += 19;
    movie->bsData1Size >>= 4;
    movie->bsData1Size <<= 4;
    vtIPU_wait(movie, 9);
    vtIPU_wait(movie, 4);
    *D4_TADR = 0;
    *D4_MADR = (movie->bsData1Addr & 0x0FFFFFFF) | 0x80000000;
    *D4_QWC = movie->bsData1Size >> 4;
    *D4_CHCR = 0x100;
    dmaStat |= 0x10;

    isDecodeFg.first = 1;
    len = movie->mbx * movie->mby;
    for (i = 0; i < len; i++) {
        if (IPU_ECD) {
            printf("(vtIPU_IDCT) IPU ERROR\n");
        }
        vtIPU_wait(movie, 3);
        FlushCache(WRITEBACK_DCACHE);
        *D3_MADR = (u32)praw;
        *D3_QWC = sizeof(sceIpuRAW16) >> 4;
        *D3_CHCR = 0x100;
        dmaStat |= 0x8;

        if (isDecodeFg.first) {
            *IPU_CMD = IPU_FDEC(0x00);
            if (vtIPU_Sync(0, _ipuTimeout) != 0) {
                return -1;
            }

            flag = *IPU_CMD;
            flag = (flag >> 24) & 0xFF;
            *IPU_CTRL = SCE_IPU_CTRL_PCT_IPIC | (flag & ~0x4) << 16;
            if (vtIPU_Sync(0, _ipuTimeout) != 0) {
                return -1;
            }
            *IPU_CMD = IPU_FDEC(0x08);
            isDecodeFg.dt = (flag & 0x4) != 0;
            isDecodeFg.first = 0;
            isDecodeFg.dcreset = 1;
            skipBit = 0;
        } else {
            *IPU_CMD = IPU_FDEC(0x00);
            isDecodeFg.dcreset = 0;
            skipBit = 1;
        }

        if (vtIPU_Sync(0, _ipuTimeout) != 0) {
            return -1;
        }

        flag = *IPU_CMD;
        printf("MB flag = 0x%x\n", flag);
        if ((flag >> skipBit) & 1) {
            skipBit++;
            isDecodeFg.qsc = 0;
        } else {
            skipBit += 2;
            isDecodeFg.qsc = 1;
        }
        skipBit += isDecodeFg.dt;
        if (isDecodeFg.qsc) {
            qsc = flag >> skipBit;
            qsc &= 0x1F;
            skipBit += 5;
        } else {
            qsc = 0;
        }
        printf("isDecodeFg.dt = %d isDecodeFg.qsc = %d qsc = 0x%x\n", isDecodeFg.dt, isDecodeFg.qsc, qsc);
        *IPU_CMD = IPU_BDEC(skipBit, qsc, isDecodeFg.dt, isDecodeFg.dcreset, SCE_IPU_BDEC_INTRA);
        if (vtIPU_Sync(0, _ipuTimeout) != 0) {
            return -1;
        }

        printf("raw[%d].y[%d] = 0x%x\n", i, 0, (u32)(movie->idctBuff[movie->currentBufNo] + i));
        if (i >= 0) {
            vtIPU_wait(movie, 3);
            ppix = (u32 *)((u32)prgb | 0x20000000);
            py = (s16 *)((u32)praw | 0x20000000);
            for (j = 0; j < 256; j++, ppix++, py++) {
                cr1 = praw->cr[j] - 128;
                cb1 = praw->cb[j] - 128;
                
                cr2 = ((cr1 * 0x68) & ~0x3F) >> 6;
                cr1 = ((cr1 * 0xCC) & ~0x3F) >> 6;
                cb2 = ((cb1 * 0x102) & ~0x3F) >> 6;
                cb1 = ((cb1 * 0x32) & ~0x3F) >> 6;
                
                y = *py;
                y = (((y - 16) * 0x95) & ~0x3F) >> 6;
                
                r = y + cr1;
                g = y - cb1 - cr2;
                b = y + cb2;
                r = (r >> 1) + (r & 1);
                g = (g >> 1) + (g & 1);
                b = (b >> 1) + (b & 1);
                
                r = r > 0xFF ? 0xFF : r < 0 ? 0 : r;
                g = g > 0xFF ? 0xFF : g < 0 ? 0 : g;
                b = b > 0xFF ? 0xFF : b < 0 ? 0 : b;
                
                *ppix = r | g << 8 | b << 16 | 128 << 24;
            }

            prgb++;
            praw++;
        }
    }

    asm("sync");
    FlushCache(WRITEBACK_DCACHE);
    return 0;
}

void vtIPU_killDecode(vtIDEC_MOVIE *movie) {
    movie->killFg = 1;
}

void vtIPU_finish(vtIDEC_MOVIE *movie) {
    // Empty function
}

u128* vtIPU_mkPacketVIF(u128 *packet, vtIDEC_MOVIE *movie) {
    return vtIPU_mkDmaTagOfSendTexVIF(packet, movie->cscBuff[movie->currentBufNo], movie);
}

u128* vtIPU_mkDmaTagOfSendTex(u128 *dmaTag, sceIpuRGB32 *image, vtIDEC_MOVIE *movie) {
    u64 *p = (u64 *)dmaTag;
    s32 mbx;
    s32 mby;
    s32 i;
    s32 j;
    u64 size;
    u8 *pimage = (u8 *)image;
    
    *p++ = 0x10000000 | 3;
     p++;
    *p++ = SCE_GIF_SET_TAG(2, 1, 0, 0, 0, 1);
    *p++ = SCE_GIF_PACKED_AD;
    *p++ = SCE_GS_SET_BITBLTBUF(0x00, 0, 0, movie->texbp, movie->texbw, 0);
    *p++ = SCE_GS_BITBLTBUF;
    size = sizeof(sceIpuRGB32);
    *p++ = SCE_GS_SET_TRXREG(16, 16);
    *p++ = SCE_GS_TRXREG;

    mbx = movie->mbx;
    mby = movie->mby;
    for (j = 0; j < mby; j++) {
        for (i = 0; i < mbx; i++) {
            *p++ = 0x10000000 | 4;
             p++;
            *p++ = SCE_GIF_SET_TAG(2, 1, 0, 0, 0, 1);
            *p++ = SCE_GIF_PACKED_AD;
            *p++ = SCE_GS_SET_TRXPOS(0, 0, i * 16, j * 16, 0);
            *p++ = SCE_GS_TRXPOS;
            *p++ = SCE_GS_SET_TRXDIR(0);
            *p++ = SCE_GS_TRXDIR;
            *p++ = SCE_GIF_SET_TAG(size >> 4, 1, 0, 0, SCE_GIF_IMAGE, size >> 4); // this should not match...
            *p++ = SCE_GIF_PACKED;
            *p++ = SET_DMAREF(size >> 4, (u32)pimage & 0x0FFFFFFF);
             p++;
            pimage += size;
        }
    }

    *p++ = 0x70000000;
     p++;
    return (u128 *)p;
}

u128* vtIPU_mkDmaTagOfSendTexVIF(u128 *dmaTag, sceIpuRGB32 *image, vtIDEC_MOVIE *movie) {
    u64 *p = (u64 *)dmaTag;
    s32 mbx;
    s32 mby;
    s32 i;
    s32 j;
    u64 size;
    u8 *pimage = (u8 *)image;
    
    p[0] = 0x10000000 | 3;
    ((u32 *)p)[2] = 0;
    ((u32 *)p)[3] = 0x50000000 | 3;
    p += 2;
    p[0] = SCE_GIF_SET_TAG(2, 1, 0, 0, 0, 1);
    p[1] = SCE_GIF_PACKED_AD;
    p += 2;
    p[1] = SCE_GS_BITBLTBUF;
    p[0] = SCE_GS_SET_BITBLTBUF(0x00, 0, 0, movie->texbp, movie->texbw, 0);
    p += 2;
    size = sizeof(sceIpuRGB32);
    p[1] = SCE_GS_TRXREG;
    p[0] = SCE_GS_SET_TRXREG(16, 16);
    p += 2;

    mbx = movie->mbx;
    mby = movie->mby;
    for (j = 0; j < mby; j++) {
        for (i = 0; i < mbx; i++) {
            p[0] = 0x10000000 | 4;
            ((u32 *)p)[2] = 0;
            ((u32 *)p)[3] = 0x50000000 | 4;
            p += 2;
            
            p[0] = SCE_GIF_SET_TAG(2, 1, 0, 0, 0, 1);
            p[1] = SCE_GIF_PACKED_AD;
            p += 2;
            p[1] = SCE_GS_TRXPOS;
            p[0] = SCE_GS_SET_TRXPOS(0, 0, i * 16, j * 16, 0);
            p += 2;
            p[1] = SCE_GS_TRXDIR;
            p[0] = SCE_GS_SET_TRXDIR(0);
            p += 2;
            
            p[0] = SCE_GIF_SET_TAG(size >> 4, 1, 0, 0, SCE_GIF_IMAGE, 0);
            p[1] = 0;
            p += 2;
            p[0] = SET_DMAREF2(size >> 4, (u32)pimage);
            ((u32 *)p)[2] = 0;
            ((u32 *)p)[3] = 0x50000000 | (s32)(size >> 4);
            p += 2;
            pimage += size;
        }
    }

    p[0] = 0x10000000;
    ((u32 *)p)[2] = 0;
    ((u32 *)p)[3] = 0x11000000;
    p += 2;
    return (u128 *)p;
}

// Not matching: https://decomp.me/scratch/uTIIC
u128* vtIPU_mkDmaTagOfSendINDX4VIF(u128 *dmaTag, sceIpuINDX4 *image, vtIDEC_MOVIE *movie, u32 vram) {
    kitADDR_DATA *packet = (kitADDR_DATA *)dmaTag;
	s32 mbx;
	s32 mby;
	s32 i;
	s32 j;
	u64 size;
	u8 *pimage = (u8 *)image;
	kitADDR_DATA *pgiftag;
	kitDMAPACKET *pdmatag;

    pdmatag = (kitDMAPACKET *)packet++;
    pgiftag = packet++;

    packet->data = SCE_GS_SET_BITBLTBUF(0x00, 0, 0, vram, movie->texbw, SCE_GS_PSMT4);
    packet->addr = SCE_GS_BITBLTBUF;
    packet++;
    size = sizeof(sceIpuINDX4);
    packet->data = SCE_GS_SET_TRXREG(16, 16);
    packet->addr = SCE_GS_TRXREG;
    packet++;
    
    pgiftag->data = SCE_GIF_SET_TAG(2, 1, 0, 0, 0, 1);
    pgiftag->addr = SCE_GIF_PACKED_AD;

    pdmatag->ul[0] = 0x10000000 | 3;
    pdmatag->ui[2] = 0;
    pdmatag->ui[3] = SCE_VIF1_SET_DIRECT(3, 0);

    mbx = movie->mbx;
    mby = movie->mby;
    for (j = 0; j < mby; j++) {
        for (i = 0; i < mbx; i++) {
            pdmatag = (kitDMAPACKET *)packet++;
            pgiftag = packet++;
            
            packet->addr = SCE_GS_TRXPOS;
            packet->data = SCE_GS_SET_TRXPOS(0, 0, i * 16, j * 16, 0);
            packet++;
            packet->addr = SCE_GS_TRXDIR;
            packet->data = SCE_GS_SET_TRXDIR(0);
            packet++;

            pgiftag->data = SCE_GIF_SET_TAG(2, 1, 0, 0, 0, 1);
            pgiftag->addr = SCE_GIF_PACKED_AD;
            pgiftag = packet++;
            pgiftag->data = SCE_GIF_SET_TAG(size >> 4, 1, 0, 0, SCE_GIF_IMAGE, 0);
            pgiftag->addr = 0;

            pdmatag->ul[0] = 0x10000000 | 4;
            pdmatag->ui[2] = 0;
            pdmatag->ui[3] = SCE_VIF1_SET_DIRECT(4, 0);
            pdmatag = (kitDMAPACKET *)packet++;
            pdmatag->ul[0] = SET_DMAREF2(size >> 4, (u32)pimage);
            pdmatag->ui[2] = 0;
            pdmatag->ui[3] = SCE_VIF1_SET_DIRECT(size >> 4, 0);
            
            pimage += size;
        }
    }

    return (u128 *)packet;
}

u128* vtIPU_mkTex0(u128 *dmaTag, vtIDEC_MOVIE *movie, s32 context) {
    kitADDR_DATA *p = (kitADDR_DATA *)dmaTag;
    p->data = 0;
    p->addr = SCE_GS_TEXFLUSH;
    p++;
    p->data = SCE_GS_SET_TEX0(movie->texbp, movie->texbw, SCE_GS_PSMCT32, movie->tw, movie->th, 1, 0, 0, 0, 0, 0, 0);
    p->addr = context == 0 ? SCE_GS_TEX0_1 : SCE_GS_TEX0_2;
    p++;
    return (u128 *)p;
}

u128* vtIPU_mkPacketForVQ(u128 *packet, vtIDEC_MOVIE *movie, s32 context) {
    packet = vtIPU_sendClut(packet, movie->gsClut, movie->cbp, movie->cpsm, movie->csm, movie->csa, movie->cld, context);
    packet = vtIPU_mkDmaTagOfSendINDX4VIF(packet, movie->vqBuff[movie->currentBufNo], movie, movie->texbp);
    return packet;
}

u128* vtIPU_mkPacketForVQ2(u128 *packet, vtIDEC_MOVIE *movie, s32 context) {
    packet = vtIPU_mkDmaTagOfSendINDX4VIF(packet, movie->vqBuff[movie->currentBufNo], movie, movie->texbp);
    return packet;
}

u128* vtIPU_mkTex0ForVQ(u128 *dmaTag, vtIDEC_MOVIE *movie, s32 context) {
    kitADDR_DATA *p = (kitADDR_DATA *)dmaTag;
    p->data = 0;
    p->addr = SCE_GS_TEXFLUSH;
    p++;
    p->data = SCE_GS_SET_TEX0(movie->texbp, movie->texbw, SCE_GS_PSMT4, movie->tw, movie->th, 1, 0, movie->cbp, movie->cpsm, movie->csm, movie->csa, movie->cld);
    p->addr = context == 0 ? SCE_GS_TEX0_1 : SCE_GS_TEX0_2;
    p++;
    return (u128 *)p;
}

u128* vtIPU_sendClut(u128 *packet, u8 *clut, u32 cbp, u32 cpsm, u32 csm, u32 csa, u32 cld, s32 context) {
    kitADDR_DATA *p = (kitADDR_DATA *)packet;
    kitDMAPACKET *pdmatag = (kitDMAPACKET *)p++;
    kitADDR_DATA *pgiftag = p++;

    p->data = SCE_GS_SET_BITBLTBUF(0, 1, 0, cbp, 1, 0);
    p->addr = SCE_GS_BITBLTBUF;
    p++;
    p->data = SCE_GS_SET_TRXREG(8, 2);
    p->addr = SCE_GS_TRXREG;
    p++;
    pgiftag->data = SCE_GIF_SET_TAG(2, 0, 0, 0, SCE_GIF_PACKED, 1);
    pgiftag->addr = SCE_GIF_PACKED_AD;
    pgiftag = p++;

    p->data = SCE_GS_SET_TRXPOS(0, 0, 0, 0, 0);
    p->addr = SCE_GS_TRXPOS;
    p++;
    p->data = SCE_GS_SET_TRXDIR(0);
    p->addr = SCE_GS_TRXDIR;
    p++;
    pgiftag->data = SCE_GIF_SET_TAG(2, 0, 0, 0, SCE_GIF_PACKED, 1);
    pgiftag->addr = SCE_GIF_PACKED_AD;
    pgiftag = p++;

    pgiftag->data = SCE_GIF_SET_TAG(sizeof(sceIpuINDX4) >> 4, 1, 0, 0, SCE_GIF_IMAGE, 0);
    pgiftag->addr = 0;

    pdmatag->ul[0] = SET_DMACNT(7);
    pdmatag->ui[2] = (u32)VU0_MICRO;
    pdmatag->ui[3] = SCE_VIF1_SET_DIRECT(7, 0);
    pdmatag = (kitDMAPACKET *)p++;
    pdmatag->ul[0] = SET_DMAREF2(8, clut);
    pdmatag->ui[2] = 0;
    pdmatag->ui[3] = SCE_VIF1_SET_DIRECT(8, 0);
    pdmatag = (kitDMAPACKET *)p++;
    pgiftag = p++;

    p->data = 0;
    p->addr = SCE_GS_TEXFLUSH;
    p++;
    p->data = SCE_GS_SET_TEX2(SCE_GS_PSMT4, cbp, cpsm, csm, csa, cld);
    p->addr = context == 0 ? SCE_GS_TEX2_1 : SCE_GS_TEX2_2;
    p++;
    pgiftag->data = SCE_GIF_SET_TAG(2, 1, 0, 0, SCE_GIF_PACKED, 1);
    pgiftag->addr = SCE_GIF_PACKED_AD;
    
    pdmatag->ul[0] = SET_DMACNT(3);
    pdmatag->ui[2] = 0;
    pdmatag->ui[3] = SCE_VIF1_SET_DIRECT(3, 0);

    return (u128 *)p;
}

static u128* mkDmaTagToIPU_SCE(u128 *tags, u8 *data, s32 datasize) {
	s32 chunkSize;
	u64 *p = (u64 *)tags;

    while (datasize > 0) {
        chunkSize = datasize >= 0xFFFF0 ? 0xFFFF0 : datasize;
        datasize -= chunkSize;
        *p++ = SET_DMAREF((chunkSize + 0xF) >> 4, (u32)data);
         p++;
        data += chunkSize;
    }

    *(p -2) &= ~0x30000000L;
    return (u128 *)p;
}

s32 vtIPU_readDataSCE(char *file, vtIDEC_MOVIE *movie) {
    s32 fd;

    fd = sceOpen(file, SCE_RDONLY);
    if (fd < 0) {
        return 0;
    }

    if (sceLseek(fd, 0x10, SEEK_SET) < 0) {
        printf("ERROR: seek failed\n");
        sceClose(fd);
        return 0;
    }

    if (sceRead(fd, movie->bsData, movie->bsDataSize) != movie->bsDataSize) {
        sceClose(fd);
        return 0;
    } else {
        sceClose(fd);
        return 1;
    }
}