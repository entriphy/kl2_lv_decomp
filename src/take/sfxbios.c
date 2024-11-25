#include <sifdev.h>
#include "take/sfxbios.h"
#include "take/camera.h"
#include "take/klsfxsub.h"
#include "take/object.h"

extern u32 Vu0Prg __attribute__((section(".vutext")));

u32* Fadr(u32 *pAddr, s32 nNum) {
    return pAddr + (pAddr[nNum + 1] >> 2);
}

s32 DataRead(char *name, char *buff) {
    s32 fd;
    s32 size;

    fd = sceOpen(name, SCE_RDONLY);
    if (fd < 0) {
        return -1;
    }

    size = sceLseek(fd, 0, SEEK_END);
    sceLseek(fd, 0, SEEK_SET);
    sceRead(fd, buff, size);
    sceClose(fd);
    // no return :(
}

void GmsLoad(qword *gms) {
    FlushCache(0);
    sceDmaSync(pDma.Gif, 0, 0);
    sceDmaSend(pDma.Gif, gms);
    sceDmaSync(pDma.Gif, 0, 0);
}

void AlphaEnvClear() {
    s32 ind = 1;
    qword_uni Packet[16];
    qword_uni *p;

    Packet[ind].u_u64[0] = SCE_GS_SET_TEST_2(1, 1, 0, 0, 0, 0, 1, SCE_GS_ZGEQUAL);
    Packet[ind].u_u64[1] = SCE_GS_TEST_2;
    ind++;
    Packet[ind].u_u64[0] = SCE_GS_SET_ALPHA(0, 1, 0, 1, 127);
    Packet[ind].u_u64[1] = SCE_GS_ALPHA_2;
    ind++;
    Packet[ind].u_u64[0] = SCE_GS_SET_FBA_1(0);
    Packet[ind].u_u64[1] = SCE_GS_FBA_1;
    ind++;
    Packet[ind].u_u64[0] = SCE_GS_SET_TEXA(0x7F, 0, 0x7F);
    Packet[ind].u_u64[1] = SCE_GS_TEXA;
    ind++;
    Packet[ind].u_u64[0] = SCE_GS_SET_FBA_2(0);
    Packet[ind].u_u64[1] = SCE_GS_FBA_2;
    ind++;
    
    Packet[0].u_u64[0] = SCE_GIF_SET_TAG(5, 1, 0, 0, SCE_GIF_PACKED, 1);
    Packet[0].u_u64[1] = SCE_GIF_PACKED_AD;
    p = SPR_MEM_IDX(0x220);
    p->u_u128 = 0;
    p->u_u32[0] = ind;
    p->u_u32[1] = (u32)&Packet;
    sceDmaSync(pDma.Gif, 0, 0);
    sceDmaSend(pDma.Gif, SPR_SRC(p));
}

s32 ReadFile(char *fn, qword *addr) {
    s32 fd;
    s32 size;
    static char sim_fname[256];

    if (strncmp(fn, "sim", 3) == 0) {
        strcpy(sim_fname, "host0:");
        strcat(sim_fname, fn + 4);
    } else {
        strcpy(sim_fname, fn);
    }

    fd = sceOpen(sim_fname, SCE_RDONLY);
    if (fd < 0) {
        return -1;
    }

    size = sceLseek(fd, 0, SEEK_END);
    sceLseek(fd, 0, SEEK_SET);
    sceRead(fd, addr, size);
    sceClose(fd);
    FlushCache(WRITEBACK_DCACHE);
    return size;
}

void Vu0ProgTrans() {
    sceDmaSync(pDma.Vif0, 0, 0);
    DPUT_D0_QWC(0x00);
    DPUT_D0_TADR((u32)&Vu0Prg & 0x0FFFFFFF);
    DPUT_D_STAT(0x01);
    FlushCache(WRITEBACK_DCACHE);
    DPUT_D0_CHCR(0x145);
    sceDmaSync(pDma.Vif0, 0, 0);
}

void WorkMemInit() {
    s32 i;

    for (i = 0; i < 512; i++) {
        MemFlag[i] = 0;
    }
}

u32 UnitAlign(u32 Fig) {
    u32 Tmp1;

    if (Fig & 0xFFF) {
        Tmp1 = Fig + 0x1000;
        Fig = Tmp1 - (Fig & 0xFFF);
    }
    return Fig;
}

s32 CheckMem() {
    s32 i;
    s32 TotalUseUnit = 0;
    u32 MaxUnit = 0;

    for (i = 0; i < 512; i++) {
        if (MemFlag[i] != 0) {
            TotalUseUnit++;
            if (MaxUnit < i) {
                MaxUnit = i;
            }
        }
    }

    printf("UseMem= %x/%x  MaxUnit=%x\n", TotalUseUnit, 512, MaxUnit);
    return TotalUseUnit;
}

void func_00213088() {
    s32 i;
    u32 MaxUnit = 0;

    for (i = 0; i < 512; i++) {
        if (MemFlag[i] != 0) {
            if (MaxUnit < i) {
                MaxUnit = i;
            }
        }
    }

    if (MaxUnit >= 256) {
        printf("SFXのワークが１ﾒｶﾞをこえています！　By 岳。\n");
    }
}

// Not matching: https://decomp.me/scratch/jeE6o
u32 GetMem(u32 Fig) {
	s32 i;
	s32 ii;
	s32 iii;
	u32 Tmp1;
	u32 BlockNum;

    if (Fig & 0xFFF) {
        Tmp1 = Fig + 0x1000;
        Tmp1 = Tmp1 - (Fig & 0xFFF);
    } else {
        Tmp1 = Fig;
    }

    BlockNum = Tmp1 >> 12;
    for (i = 0; i < 512; i++) {
        if (MemFlag[i] == 0) {
            for (ii = 0; MemFlag[i + ii] == 0; ii++) {
                if (ii == BlockNum - 1) {
                    for (iii = 0; iii < BlockNum; iii++) {
                        MemFlag[i + iii] = BlockNum - iii;
                    }
                    return (u32)(SfxWorkBuff + i * 0x1000);
                }
            }
            i += ii;
        }
    }
    return Fig;
}

void OpenMem(u32 *Adrs) {
	s32 i;
	u32 Num;
	s32 Loop;

    Num = ((size_t)Adrs - (size_t)SfxWorkBuff) >> 12;
    Loop = MemFlag[Num];
    for (i = 0; i < Loop; i++) {
        MemFlag[Num + i] = 0;
    }
}

void SfxSystemInit(u32 *Buff) {
    pDma.Vif0 = sceDmaGetChan(SCE_DMA_VIF0);
    pDma.Vif1 = sceDmaGetChan(SCE_DMA_VIF1);
    pDma.Gif = sceDmaGetChan(SCE_DMA_GIF);
    pDma.ToSpr = sceDmaGetChan(SCE_DMA_toSPR);
    pDma.FromSpr = sceDmaGetChan(SCE_DMA_fromSPR);
    ScreenInit();
    LightInit();
    SfxWorkBuff = (char *)Buff;
    SfxWorkBuff = (char *)UnitAlign((u32)SfxWorkBuff);
    WorkMemInit();
    SfxTexInfoIndex = 0;
    SpecEnvInit();
    SetObjAllPause(0);
}

// Not matching: https://decomp.me/scratch/E8APs
void GimLoad(u32 *addr, GIMINFO *info) {
	sceGsLoadImage lp;
	char divid[5] = { 0x20, 0x18, 0x10, 0x08, 0x04 };
	GIMHEADER *g_tag;
	s32 ofs;
	s32 wx = 0;
	s32 hx = 0;
	s32 i;
	s16 psm;
	s16 pw;
	s16 ph;
	s16 tbw;
	s16 px;
	s16 py;
	s16 cpsm;
	s16 cw;
	s16 ch;
	s16 cbw;
	s16 cx;
	s16 cy;
	s32 check;
	s32 dw;
	s32 coy;
	s32 cox;

    info->flag = 1;
    // addr += 4;
    g_tag = (GIMHEADER *)(addr + 4);
    if (g_tag->bp != 0) {
        check = 1;
    } else {
        check = 0;
    }
    if (check == 1) {
        psm = g_tag->psm;
        pw = g_tag->w;
        ph = g_tag->h;
        px = g_tag->x;
        py = g_tag->y;
        tbw = g_tag->bw;

        info->pbp = g_tag->bp;
        info->px = px;
        info->py = py;
        info->h = g_tag->h;
        info->w = g_tag->w;
    } else {
        psm = g_tag->psm;
        pw = g_tag->w;
        ph = g_tag->h;
        px = 0;
        py = 0;
        tbw = g_tag->bw;
        
        info->px = px;
        info->py = py;
    }

    g_tag++;
    sceGsSetDefLoadImage(&lp, info->pbp, tbw, psm, px, py, pw, ph);
    FlushCache(0);
    sceGsExecLoadImage(&lp, (u128 *)g_tag);
    sceGsSyncPath(0, 0);

    if (tbw == 4) {
        dw = ((pw * divid[psm & 7] + 31) / 32) * 32;
    } else {
        dw = ((pw * divid[psm & 7] + 63) / 64) * 64;
    }
    ofs = (ph * dw) / 128;
    g_tag += ofs;
    
    cpsm = g_tag->psm;
    cw = g_tag->w;
    ch = g_tag->h;
    if (check == 1) {
        info->cbp = g_tag->bp;
        cx = g_tag->x;
        cy = g_tag->y;
        cbw = g_tag->bw;
    } else {
        info->cbp = info->pbp + (ofs / 4);
        cx = 0;
        cy = 0;
        cbw = (g_tag->w + 63) / 64;
    }
    info->cx = cx;
    info->cy = cy;
    g_tag++;

    for (i = 10; i > 0; i--) {
        if (((px + pw) >> i) & 1) {
            if (wx != 0) {
                wx++;
                break;
            } else {
                wx = i;
            }
        }
    }

    for (i = 10; i > 0; i--) {
        if (((py + ph) >> i) & 1) {
            if (hx != 0) {
                hx++;
                break;
            } else {
                hx = i;
            }
        }
    }

    coy = (cy & 0x8) / 4 + (cy & 0x10) / 2;
    cox = (cx & 0x8) / 8 + (cx & 0x10) / 4 + (cx & 0x20) / 2;
    info->tex0 = SCE_GS_SET_TEX0(info->pbp, tbw, psm, wx, hx, 0, 1, info->cbp + cox + coy, cpsm, 0, 0, 1);
    if (psm & 0x10) {
        sceGsSetDefLoadImage(&lp, info->cbp, cbw, cpsm, cx, cy, cw, ch);
        FlushCache(0);
        sceGsSyncPath(0, 0);
        sceGsExecLoadImage(&lp, (u128 *)g_tag);
    }
    sceGsSyncPath(0, 0);
}
