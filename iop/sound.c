#include "iop.h"

SNDDATA SndData;
SNDDATA *sD;

#ifdef SCE_OBSOLETE
static int SndDmaInt(void *common)
#else
static int SndDmaInt(int core, void *common)
#endif
{
	sD->DmaWait--;
	return 1;
}

void SndGetPacket(u_char *pk) {
	SNDVOPRM *vp;
	int i;
	int pnum;
	int com;
	int mix;
	int core;
	int vnum;
	int bank;
	int prog;
	int splt;
	int tmp;

    pnum = *(u16 *)pk;
    pk += 2;
    for (i = 0; i < pnum; i++) {
        com = *pk;
        
        switch ((com = *pk++)) {
            case SNDCMD_KEYON:
                vnum = *pk++;
                core = vnum & 1;
                vnum >>= 1;
                vp = &sD->VoPrm[core][vnum];
                mix = *pk++;
                
                if (mix & 1)
                    sD->VMIXL[core] |= 1 << vnum;
                else
                    sD->VMIXL[core] &= ~(1 << vnum);

                if (mix & 2)
                    sD->VMIXEL[core] |= 1 << vnum;
                else
                    sD->VMIXEL[core] &= ~(1 << vnum);

                sD->VMIXL[core] &= 0xFFFFFF;
                sD->VMIXEL[core] &= 0xFFFFFF;
                sD->VMIXR[core] = sD->VMIXL[core];
                sD->VMIXER[core] = sD->VMIXEL[core];
                bank = *pk++;
                prog = *pk++;
                splt = *pk++;
                JamGetPrm(bank, prog, splt, &sD->KeyPrm);
                vp->flag |= 0x3F;
                vp->loop = sD->KeyPrm.Loop;
                vp->VOLL = sD->KeyPrm.VOLL;
                vp->VOLR = sD->KeyPrm.VOLR;
                vp->PITCH = sD->KeyPrm.PITCH;
                vp->ADSR1 = sD->KeyPrm.ADSR1;
                vp->ADSR2 = sD->KeyPrm.ADSR2;
                vp->SSA = sD->KeyPrm.SSA;
                vp->setVolL = 0x4000;
                vp->setVolR = 0x4000;
                vp->setPitch = 0x1000;
                sD->KeyOnV[core] |= 1 << vnum;
                sD->KeyOffV[core] &= ~(1 << vnum);
                break;
            case SNDCMD_KEYOFF:
                vnum = *pk++;
                core = vnum & 1;
                vnum >>= 1;
                sD->KeyOffV[core] |= 1 << vnum;
                sD->KeyOnV[core] &= ~(1 << vnum);
                break;
            case SNDCMD_KEYOFFALL:
                sD->KeyOffV[0] |= sD->KeyMask[0];
                sD->KeyOffV[1] |= sD->KeyMask[1];
                sD->KeyOnV[0] &= ~(1 << vnum);
                sD->KeyOnV[1] &= ~(1 << vnum);
                break;
            case SNDCMD_PITCH:
                vnum = *pk++;
                core = vnum & 1;
                vnum >>= 1;
                vp = &sD->VoPrm[core][vnum];
                vp->flag |= 1;
                vp->PITCH = (vp->PITCH * 5) / 6;
                break;
            case SNDCMD_PALPITCH:
                vnum = *pk++;
                core = vnum & 1;
                vnum >>= 1;
                vp = &sD->VoPrm[core][vnum];
                vp->flag |= 1;
                tmp = *pk++;
                vp->setPitch = *pk++ << 8;
                vp->setPitch |= tmp & 0xFF;
                break;
            case SNDCMD_VOL:
                vnum = *pk++;
                core = vnum & 1;
                vnum >>= 1;
                vp = &sD->VoPrm[core][vnum];
                vp->flag |= 6;
                tmp = *pk++;
                vp->setVolL = *pk++ << 8;
                vp->setVolL |= tmp & 0xFF;
                tmp = *pk++;
                vp->setVolR = *pk++ << 8;
                vp->setVolR |= tmp & 0xFF;
                break;
            case SNDCMD_MVOL:
                core = *pk++;
                tmp = *pk++;
                sD->MVOLL[core] = *pk++ << 8;
                sD->MVOLL[core] |= tmp & 0xFF;
                tmp = *pk++;
                sD->MVOLR[core] = *pk++ << 8;
                sD->MVOLR[core] |= tmp & 0xFF;
                break;
            case SNDCMD_MVOLALL:
                sD->MVOLL[0] = *pk++;
                sD->MVOLL[0] |= *pk++ << 8;
                sD->MVOLL[1] = sD->MVOLL[0];
                sD->MVOLR[0] = *pk++;
                sD->MVOLR[0] |= *pk++ << 8;
                sD->MVOLR[1] = sD->MVOLL[0];
                break;
            case SNDCMD_EFFECT:
                core = *pk++;
                sD->EffAttr[core].mode = *pk++;
                sD->EffAttr[core].depth_L = *pk++;
                sD->EffAttr[core].depth_L |= *pk++ << 8;
                sD->EffAttr[core].depth_R = *pk++;
                sD->EffAttr[core].depth_R |= *pk++ << 8;
                sD->EffAttr[core].delay = *pk++;
                sD->EffAttr[core].feedback = *pk++;
                sD->EffChange[core] = 1;
                sD->EffAttr[core].mode |= 0x100;
                break;
            case SNDCMD_EVOL:
                sD->EVOLL[0] = *pk++;
                sD->EVOLL[0] = sD->EVOLL[0] | *pk++ << 8;
                sD->EVOLL[1] = sD->EVOLL[0];
                sD->EVOLR[0] = sD->EVOLL[0];
                sD->EVOLR[1] = sD->EVOLL[0];
                break;
            default:
                break;
        }
    }
}

void SndMain(int *data) {
	SNDVOPRM *vp;
	int core;
	int vnum;

    SndGetPacket((u_char *)data);
    for (core = 0; core < 2; core++) {
        if (sD->EffChange[core] != 0) {
            sD->Batch[0].func = SD_BSET_PARAM;
            sD->Batch[0].entry = core | SD_P_EVOLL;
            sD->Batch[0].value = 1;
            sD->Batch[1].func = SD_BSET_PARAM;
            sD->Batch[1].entry = core | SD_P_EVOLR;
            sD->Batch[1].value = 1;
            sceSdProcBatch(sD->Batch, NULL, 2);
            sceSdSetEffectAttr(core, &sD->EffAttr[core]);
            sD->EffChange[core] = 0;
            sD->EVOLLbak[core] = 0x7FFF;
            sD->EVOLRbak[core] = 0x7FFF;
        }
    }

    for (core = 0; core < 2; core++) {
        for (vnum = 0; vnum < 0x18; vnum++) {
            vp = &sD->VoPrm[core][vnum];
            if (vp->disable != 0) {
                // yes this empty block is necessary lol
            } else {
                if (vp->flag & 1) {
                    sD->Batch[sD->BatNum].func = SD_BSET_PARAM;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | SD_VP_PITCH;
                    sD->Batch[sD->BatNum].value = (vp->PITCH * vp->setPitch) / 0x1000;
                    sD->BatNum++;
                }
                if (vp->flag & 2) {
                    sD->Batch[sD->BatNum].func = SD_BSET_PARAM;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | SD_VP_VOLL;
                    sD->Batch[sD->BatNum].value = ((vp->VOLL * vp->setVolL) / 0x4000) & 0x7FFF;
                    sD->BatNum++;
                }
                if (vp->flag & 4) {
                    sD->Batch[sD->BatNum].func = SD_BSET_PARAM;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | SD_VP_VOLR;
                    sD->Batch[sD->BatNum].value = ((vp->VOLR * vp->setVolR) / 0x4000) & 0x7FFF;
                    sD->BatNum++;
                }
                if (vp->flag & 8) {
                    sD->Batch[sD->BatNum].func = SD_BSET_PARAM;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | SD_VP_ADSR1;
                    sD->Batch[sD->BatNum].value = vp->ADSR1;
                    sD->BatNum++;
                }
                if (vp->flag & 0x10) {
                    sD->Batch[sD->BatNum].func = SD_BSET_PARAM;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | SD_VP_ADSR2;
                    sD->Batch[sD->BatNum].value = vp->ADSR2;
                    sD->BatNum++;
                }
                if (vp->flag & 0x20) {
                    sD->Batch[sD->BatNum].func = SD_BSET_ADDR;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | SD_VA_SSA;
                    sD->Batch[sD->BatNum].value = vp->SSA;
                    sD->BatNum++;
                }
                vp->flag = 0;
            }
        }

        if (sD->KeyOnV[core] != 0) {
            sD->Batch[sD->BatNum].func = SD_BSET_SWITCH;
            sD->Batch[sD->BatNum].entry = core | SD_S_KON;
            sD->Batch[sD->BatNum].value = sD->KeyOnV[core] & sD->KeyMask[core];
            sD->BatNum++;
        }
        if (sD->KeyOffV[core] != 0) {
            sD->Batch[sD->BatNum].func = SD_BSET_SWITCH;
            sD->Batch[sD->BatNum].entry = core | SD_S_KOFF;
            sD->Batch[sD->BatNum].value = sD->KeyOffV[core] & sD->KeyMask[core];
            sD->BatNum++;
            sD->vStatKeyon[core] &= ~sD->KeyOffV[core];
            sD->KeyOffV[core] = 0;
        }
        if (sD->MVOLL[core] != sD->MVOLLbak[core]) {
            sD->Batch[sD->BatNum].func = SD_BSET_PARAM;
            sD->Batch[sD->BatNum].entry = core | SD_P_MVOLL;
            sD->Batch[sD->BatNum].value = sD->MVOLL[core];
            sD->BatNum++;
        }
        if (sD->MVOLR[core] != sD->MVOLRbak[core]) {
            sD->Batch[sD->BatNum].func = SD_BSET_PARAM;
            sD->Batch[sD->BatNum].entry = core | SD_P_MVOLR;
            sD->Batch[sD->BatNum].value = sD->MVOLR[core];
            sD->BatNum++;
        }
        sD->MVOLLbak[core] = sD->MVOLL[core];
        sD->MVOLRbak[core] = sD->MVOLR[core];
        if (sD->EVOLL[core] != sD->EVOLLbak[core]) {
            sD->Batch[sD->BatNum].func = SD_BSET_PARAM;
            sD->Batch[sD->BatNum].entry = core | SD_P_EVOLL;
            sD->Batch[sD->BatNum].value = sD->EVOLL[core];
            sD->BatNum++;
        }
        if (sD->EVOLR[core] != sD->EVOLRbak[core]) {
            sD->Batch[sD->BatNum].func = SD_BSET_PARAM;
            sD->Batch[sD->BatNum].entry = core | SD_P_EVOLR;
            sD->Batch[sD->BatNum].value = sD->EVOLR[core];
            sD->BatNum++;
        }
        sD->EVOLLbak[core] = sD->EVOLL[core];
        sD->EVOLRbak[core] = sD->EVOLR[core];
        if (sD->VMIXL[core] != sD->VMIXLbak[core]) {
            sD->Batch[sD->BatNum].func = SD_BSET_SWITCH;
            sD->Batch[sD->BatNum].entry = core | SD_S_VMIXL;
            sD->Batch[sD->BatNum].value = sD->VMIXL[core];
            sD->BatNum++;
        }
        if (sD->VMIXR[core] != sD->VMIXRbak[core]) {
            sD->Batch[sD->BatNum].func = SD_BSET_SWITCH;
            sD->Batch[sD->BatNum].entry = core | SD_S_VMIXR;
            sD->Batch[sD->BatNum].value = sD->VMIXR[core];
            sD->BatNum++;
        }
        if (sD->VMIXEL[core] != sD->VMIXELbak[core]) {
            sD->Batch[sD->BatNum].func = SD_BSET_SWITCH;
            sD->Batch[sD->BatNum].entry = core | SD_S_VMIXEL;
            sD->Batch[sD->BatNum].value = sD->VMIXEL[core];
            sD->BatNum++;
        }
        if (sD->VMIXER[core] != sD->VMIXERbak[core]) {
            sD->Batch[sD->BatNum].func = SD_BSET_SWITCH;
            sD->Batch[sD->BatNum].entry = core | SD_S_VMIXER;
            sD->Batch[sD->BatNum].value = sD->VMIXER[core];
            sD->BatNum++;
        }
        sD->VMIXLbak[core] = sD->VMIXL[core];
        sD->VMIXRbak[core] = sD->VMIXR[core];
        sD->VMIXELbak[core] = sD->VMIXEL[core];
        sD->VMIXERbak[core] = sD->VMIXER[core];
    }

    sceSdProcBatch(sD->Batch, NULL, sD->BatNum);
    sD->BatNum = 0;
    for (core = 0; core < 2; core++) {
        for (vnum = 0; vnum < 24; vnum++) {
            sD->Batch[core * 24 + vnum].func = SD_BGET_PARAM;
            sD->Batch[core * 24 + vnum].entry = core | vnum << 1 | SD_VP_ENVX;
            sD->Batch[core * 24 + vnum].value = 0;
        }
    }
    sceSdProcBatch(sD->Batch, (u_int *)sD->ENVX, 48);
    for (core = 0; core < 2; core++) {
        sD->vStatEnv[core] = 0;
        for (vnum = 0; vnum < 24; vnum++) {
            if (sD->ENVX[core][vnum] != 0) {
                sD->vStatEnv[core] |= 1 << vnum;
            }
        }
    }

    sD->vStatKeyon[0] &= sD->vStatEnv[0];
    sD->vStatKeyon[1] &= sD->vStatEnv[1];
    sD->vStatKeyon[0] |= sD->KeyOnV[0];
    sD->vStatKeyon[1] |= sD->KeyOnV[1];
    sD->KeyOnV[0] = 0;
    sD->KeyOnV[1] = 0;
    RpcRet[0] = sD->vStatKeyon[0];
    RpcRet[1] = sD->vStatKeyon[1];
    RpcInfo[0] = sD->vStatKeyon[0];
    RpcInfo[1] = sD->vStatKeyon[1];
}

void SndMask(int *data) {
    int core;
	int vnum;
	int mask;

    for (core = 0; core < 2; core++) {
        mask = *data++;
        sD->KeyMask[core] = mask;
        for (vnum = 0; vnum < 24; vnum++) {
            sD->VoPrm[core][vnum].disable = ~mask & 1;
            mask >>= 1;
        }
    }
}

void SndInit() {
	int core;
	int vnum;

    sceSdInit(0);
    sceSdSetCoreAttr(SD_CORE_0 | SD_C_NOISE_CLK, 0x800);
    sD = &SndData;
    sD->BatNum = 0;

    for (core = 0; core < 2; core++) {
        for (vnum = 0; vnum < 0x18; vnum++) {
            sD->VoPrm[core][vnum].disable = 0;
            sD->VoPrm[core][vnum].flag = 0;
            sD->VoPrm[core][vnum].loop = 0;
        }
        sD->vStatKeyon[core] = 0;
        sD->KeyOnV[core] = 0;
        sD->KeyOffV[core] = 0;
        sD->KeyMask[core] = 0xFFFFFF;
    }

    for (core = 0; core < 2; core++) {
        sD->MVOLL[core] = sD->MVOLLbak[core] = 0;
        sD->MVOLR[core] = sD->MVOLRbak[core] = 0;
        sceSdSetParam(core | SD_P_MVOLL, 0);
        sceSdSetParam(core | SD_P_MVOLR, 0);
        for (vnum = 0; vnum < 24; vnum++) {
            sceSdSetParam(core | vnum << 1 | SD_VP_VOLL, 0);
            sceSdSetParam(core | vnum << 1 | SD_VP_VOLR, 0);
        }
        
        sD->VMIXL[core] = sD->VMIXLbak[core] = 0xFFFFFF;
        sD->VMIXR[core] = sD->VMIXRbak[core] = sD->VMIXL[core];
        if (core != 0)
            sD->VMIXEL[core] = sD->VMIXELbak[core] = 0xF0000;
        else
            sD->VMIXEL[core] = sD->VMIXELbak[core] = 0;
        sD->VMIXER[core] = sD->VMIXERbak[core]= sD->VMIXEL[core];
        
        sceSdSetSwitch(core | SD_S_VMIXL, sD->VMIXL[core]);
        sceSdSetSwitch(core | SD_S_VMIXR, sD->VMIXR[core]);
        sceSdSetSwitch(core | SD_S_VMIXEL, sD->VMIXEL[core]);
        sceSdSetSwitch(core | SD_S_VMIXER, sD->VMIXER[core]);
        sceSdSetAddr(core | SD_A_EEA, 0x1FFFFF - core * 0x20000);
        sD->EffAttr[core].mode = SD_REV_MODE_CLEAR_WA;
        sD->EffAttr[core].depth_L = 0;
        sD->EffAttr[core].depth_R = 0;
        sD->EffAttr[core].delay = 0;
        sD->EffAttr[core].feedback = 0;
        sceSdSetEffectAttr(core, &sD->EffAttr[core]);
        sceSdSetCoreAttr(core | SD_C_EFFECT_ENABLE, 1);
        sceSdSetParam(core | SD_P_EVOLL, 0);
        sceSdSetParam(core | SD_P_EVOLR, 0);
        sceSdSetParam(core | SD_P_BVOLL, 0);
        sceSdSetParam(core | SD_P_BVOLR, 0);
    }

    sceSdSetParam(SD_CORE_0 | SD_P_MMIX, 0xFC0);
    sceSdSetParam(SD_CORE_1 | SD_P_MMIX, 0xFFC);
#ifdef SCE_OBSOLETE
    sceSdSetTransCallback(0, &SndDmaInt);
#else
    sceSdSetTransIntrHandler(0, &SndDmaInt, NULL);
#endif
    JamInit();
    RpcRet[0] = Mem.iBankhd;
}
