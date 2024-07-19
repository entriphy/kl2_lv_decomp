#include "take/sfxdraw.h"
#include "take/object.h"
#include "take/outline.h"
#include "take/sfxcalc.h"

u128 PacketBuffer1[256] = {};
u128 PacketBuffer2[512] = {};
s32 iii[100] = {};
kitOutlineObjEnv LineObjEnv = {};

static void SfxPacketMakeLight(SFXOBJ *pObj, s32 PartsNum, u64 *pStrip, u64 *pUv, u128 *pColorI, u128 *pVertexI, u128 *pSpecUVI, s32 LastStripNum);
static void SfxPacketMake(SFXOBJ *pObj, s32 PartsNum, long_uni *pStrip, u64 *pUv, u128 *pColorI, u128 *pVertexI, u128 *pSpecUVI, s32 LastStripNum);

void SfxDrawOutLine(SFXOBJ *pObj) {
	s32 i;
	u128 *pVertexI;
	u128 *pColorI;
	u128 *pSpecUVI;
	u64 *pUv;
	PARTS *pParts;
	TYPE_STRIP *pStrip;
    qword_uni *pPacket;
    s32 PacketInd;

    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;
    pPacket = SPR_MEM_IDX(0x230);
    PacketInd = 0;
    pPacket[PacketInd].u_u128 = 0;
    pPacket[PacketInd++].u_u32[0] = 0x70000000 | 1;
    pPacket[PacketInd].u_u64[0] = SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1);
    pPacket[PacketInd++].u_u64[1] = 0xF;

    pVertexI = (u128 *)SfxVertexI;
    pColorI = (u128 *)SfxColorI;
    pSpecUVI = (u128 *)SfxSpecUVI;
    SpActive = 0;
    SfxPacketBuff[0] = SPR_MEM_IDX(0x000);
    SfxPacketBuff[1] = SPR_MEM_IDX(0x110);
    LineObjEnv.partsmax = pObj->LineEnv.partsmax;
    LineObjEnv.status = pObj->LineEnv.status;

    for (i = pObj->PartsNum - 1; i >= 0; i--) {
        OutlineEnv.partsnum = i;
        pParts = &pObj->pParts[i];
        pUv = (u64 *)pParts->uv_adrs;
        pStrip = (TYPE_STRIP *)pParts->prim_adrs;

        switch (pParts->type) {
            case 0:
                SfxFixCalc(pObj, &pObj->pParts[i], pVertexI, pColorI, pSpecUVI);
                break;
            case 1:
                SfxSkinCalc(pObj, &pObj->pParts[i], pVertexI, pColorI, pSpecUVI);
                break;
            case 3:
                SfxMimeCalc(pObj, &pObj->pParts[i], pVertexI, pColorI, pSpecUVI);
                break;
        }

        if (pObj->OutLineFlag != 0) {
            if (bboxTmp[1][2] - 2 < 0xFFFFDU) {
                u128 *packet;
                {
                    u128 *tmp_buf = UNCACHED(PacketBuffer1);
                    packet = OutlinePreRoutine(tmp_buf, &OutlineEnv, &LineObjEnv);
                    __asm__("sync");
                    PacketKickPath3((u32)tmp_buf, ((u32)packet - (u32)tmp_buf) / 16);
                    sceVu0CopyVector(OutlineEnv.bbox[0], bboxTmp[0]);
                    sceVu0CopyVector(OutlineEnv.bbox[1], bboxTmp[1]);
                    SfxPacketMake(pObj, i, (long_uni *)pStrip, pUv, pColorI, pVertexI, pSpecUVI, pParts->prim_num);
                    if (pObj->pParts[i].SpecType != 0) {
                        SfxPacketMakeLight(pObj, i, (u64 *)pStrip, pUv, pColorI, pVertexI, pSpecUVI, pParts->prim_num);
                    }
                    OutlineColorDefine(&OutlineEnv, &LineObjEnv);
                }

                {
                    u128 *tmp_buf = UNCACHED(PacketBuffer2);
                    packet = OutlineExecution(tmp_buf, &OutlineEnv);
                    __asm__("sync");
                    PacketKickPath3((u32)tmp_buf, ((u32)packet - (u32)tmp_buf) / 16);
                }
               
                
            }
        } else {
            SfxPacketMake(pObj, i, (long_uni *)pStrip, pUv, pColorI, pVertexI, pSpecUVI,pParts->prim_num);
            if (pObj->pParts[i].SpecType != 0) {
                SfxPacketMakeLight(pObj, i, (u64 *)pStrip, pUv, pColorI, pVertexI, pSpecUVI, pParts->prim_num);
            }
        }
    }
}

// Not matching: https://decomp.me/scratch/RTkE0
static void SfxPacketMakeLight(SFXOBJ *pObj, s32 PartsNum, u64 *pStrip, u64 *pUv, u128 *pColorI, u128 *pVertexI, u128 *pSpecUVI, s32 LastStripNum) {
	s32 i;
    s32 ind;
    u64 s_tmp;
    s32 adc_cnt = 0;
    qword_uni *pPacket;
    qword_uni *tmp = SPR_MEM_IDX(0x220);
    
    tmp->u_u64[1] = 0;
    tmp->u_u32[0] = SpecGsEnvInd;
    tmp->u_u32[1] = (u32)SpecGsEnv;
    DmaSync_Mac(pDma.Gif);
    DmaSend_Mac(pDma.Gif, SPR_SRC(tmp));
    
    for (; LastStripNum > 0; LastStripNum--, pStrip += i) {
        for (pPacket = (qword_uni *)SfxPacketBuff[SpActive], ind = 1; ; LastStripNum--, pStrip += i) {
            pPacket[ind].u_u64[0] = SCE_GIF_SET_TAG((s16)(*pStrip >> 48), 0, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
            pPacket[ind++].u_u64[1] = 0xF | SCE_GS_UV << 4 | SCE_GS_XYZF2 << 8;
            for (i = 0; i < (s16)(*pStrip >> 48); i++) {
                s_tmp = pStrip[i];
                ind++;
                pPacket[ind++].u_u64[0] = *(u64 *)&pSpecUVI[(s16)s_tmp];
                s_tmp >>= 32;
                pPacket[ind].u_u128 = pVertexI[(s16)s_tmp];
                if (pPacket[ind].u_u32[3] != 0) {
                    adc_cnt = 3;
                }

                if (adc_cnt != 0) {
                    adc_cnt--;
                    pPacket[ind].u_u32[3] = 0x8000;
                }
                ind++;
                s_tmp >>= 16;
            }
            
            if ((s16)s_tmp < 0) {
                break;
            }
        }
        pPacket->u_u128 = 0;
        pPacket->u_u32[0] = 0x70000000 | ind - 1;
        DmaSync_Mac(pDma.Gif);
        DmaSend_Mac(pDma.Gif, SPR_SRC(pPacket));
        SpActive = (SpActive + 1) % 2;
    }
    
    sceDmaSync(pDma.Gif, 0, 0);
    DmaSend_Mac(pDma.Gif, SPR_SRC(SPR_MEM_IDX(0x230)));
}

static void SfxPacketMake(SFXOBJ *pObj, s32 PartsNum, long_uni *pStrip, u64 *pUv, u128 *pColorI, u128 *pVertexI, u128 *pSpecUVI, s32 LastStripNum) {
	s32 adc_cnt = 0;
    qword_uni *pk = SPR_MEM_IDX(0x220);

    pk->u_u64[1] = 0;
    pk->u_u32[0] = pObj->pParts[PartsNum].GsEnvInd;
    pk->u_u32[1] = (u32)pObj->pParts[PartsNum].GsEnv;
    DmaSync_Mac(pDma.Gif);
    DmaSend_Mac(pDma.Gif, SPR_SRC(pk));

    while (LastStripNum > 0) {
        qword_uni *pPacket = (qword_uni *)SfxPacketBuff[SpActive];
        s32 ind = 1;
        s32 i;
        
        loop: {
            pPacket[ind].u_u64[0] = SCE_GIF_SET_TAG(pStrip->u_u16[3], 0, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
            pPacket[ind++].u_u64[1] = SCE_GS_RGBAQ | SCE_GS_UV << 4 | SCE_GS_XYZF2 << 8;
            for (i = pStrip->u_u16[3]; i > 0; i--) {
                pPacket[ind++].u_u128 = pColorI[pStrip->u_u16[0]];
                pPacket[ind++].u_u64[0] = pUv[pStrip->u_u16[1]];
                pPacket[ind].u_u128 = pVertexI[pStrip->u_u16[2]];
                if (((u32 *)&pVertexI[pStrip->u_u16[2]])[3] != 0) {
                    adc_cnt = 3;
                }
                if (adc_cnt != 0) {
                    adc_cnt--;
                    pPacket[ind].u_u32[3] = 0x8000;
                }
                ind++;
                pStrip++;
            }
            LastStripNum--;
    
            if (pStrip[-1].u_s16[3] >= 0) {
                goto loop; // :(
            }
        }
        
        pPacket->u_u128 = 0;
        pPacket->u_u32[0] = 0x70000000 | ind - 1;
        sceDmaSync(pDma.Gif, 0, 0);
        DmaSend_Mac(pDma.Gif, SPR_SRC(pPacket));
        SpActive = (SpActive + 1) % 2;
    }
    
    sceDmaSync(pDma.Gif, 0, 0);
    DmaSend_Mac(pDma.Gif, SPR_SRC(SPR_MEM_IDX(0x230)));
}

// These are technically inline but whatever duuuuude
void DmaSend_Mac(sceDmaChan *DmaChan, u128 *Addr) {
    __asm__("sync");
    DmaChan->tadr = (sceDmaTag *)Addr;
    DmaChan->qwc = 0;
    DmaChan->chcr.MOD = 1;
    DmaChan->chcr.STR = 1;
}

void DmaSync_Mac(sceDmaChan *DmaChan) {
    while (*(vs32 *)(&DmaChan->chcr) & D_CHCR_STR_M);
}
