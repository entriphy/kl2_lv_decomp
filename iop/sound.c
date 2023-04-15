#include "iop.h"

SNDDATA SndData;
SNDDATA *sD;

static int SndDmaInt(void *common) {
	sD->DmaWait -= 1;
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
                
                if ((mix & 1) != 0) {
                    sD->VMIXL[core] |= 1 << vnum;
                } else {
                    sD->VMIXL[core] &= ~(1 << vnum);
                }

                if ((mix & 2) != 0) {
                    sD->VMIXEL[core] |= 1 << vnum;
                } else {
                    sD->VMIXEL[core] &= ~(1 << vnum);
                }

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

    SndGetPacket(data);
    for (core = 0; core < 2; core++) {
        if (sD->EffChange[core] != 0) {
            sD->Batch[0].func = 1;
            sD->Batch[0].entry = core | 0xb80;
            sD->Batch[0].value = 1;
            sD->Batch[1].func = 1;
            sD->Batch[1].entry = core | 0xc80;
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
                if ((vp->flag & 1) != 0) {
                    sD->Batch[sD->BatNum].func = 1;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | 0x200;
                    sD->Batch[sD->BatNum].value = (vp->PITCH * vp->setPitch) / 0x1000;
                    sD->BatNum++;
                }
                if ((vp->flag & 2) != 0) {
                    sD->Batch[sD->BatNum].func = 1;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core;
                    sD->Batch[sD->BatNum].value = ((vp->VOLL * vp->setVolL) / 0x4000) & 0x7FFF;
                    sD->BatNum++;
                }
                if ((vp->flag & 4) != 0) {
                    sD->Batch[sD->BatNum].func = 1;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | 0x100;
                    sD->Batch[sD->BatNum].value = ((vp->VOLR * vp->setVolR) / 0x4000) & 0x7FFF;
                    sD->BatNum++;
                }
                if ((vp->flag & 8) != 0) {
                    sD->Batch[sD->BatNum].func = 1;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | 0x300;
                    sD->Batch[sD->BatNum].value = vp->ADSR1;
                    sD->BatNum++;
                }
                if ((vp->flag & 0x10) != 0) {
                    sD->Batch[sD->BatNum].func = 1;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | 0x400;
                    sD->Batch[sD->BatNum].value = vp->ADSR2;
                    sD->BatNum++;
                }
                if ((vp->flag & 0x20) != 0) {
                    sD->Batch[sD->BatNum].func = 3;
                    sD->Batch[sD->BatNum].entry = vnum << 1 | core | 0x2040;
                    sD->Batch[sD->BatNum].value = vp->SSA;
                    sD->BatNum++;
                }
                vp->flag = 0;
            }
        }

        if (sD->KeyOnV[core] != 0) {
            sD->Batch[sD->BatNum].func = 2;
            sD->Batch[sD->BatNum].entry = core | 0x1500;
            sD->Batch[sD->BatNum].value = sD->KeyOnV[core] & sD->KeyMask[core];
            sD->BatNum++;
        }
        if (sD->KeyOffV[core] != 0) {
            sD->Batch[sD->BatNum].func = 2;
            sD->Batch[sD->BatNum].entry = core | 0x1600;
            sD->Batch[sD->BatNum].value = sD->KeyOffV[core] & sD->KeyMask[core];
            sD->BatNum++;
            sD->vStatKeyon[core] &= ~sD->KeyOffV[core];
            sD->KeyOffV[core] = 0;
        }
        if (sD->MVOLL[core] != sD->MVOLLbak[core]) {
            sD->Batch[sD->BatNum].func = 1;
            sD->Batch[sD->BatNum].entry = core | 0x980;
            sD->Batch[sD->BatNum].value = sD->MVOLL[core];
            sD->BatNum++;
        }
        if (sD->MVOLR[core] != sD->MVOLRbak[core]) {
            sD->Batch[sD->BatNum].func = 1;
            sD->Batch[sD->BatNum].entry = core | 0xA80;
            sD->Batch[sD->BatNum].value = sD->MVOLR[core];
            sD->BatNum++;
        }
        sD->MVOLLbak[core] = sD->MVOLL[core];
        sD->MVOLRbak[core] = sD->MVOLR[core];
        if (sD->EVOLL[core] != sD->EVOLLbak[core]) {
            sD->Batch[sD->BatNum].func = 1;
            sD->Batch[sD->BatNum].entry = core | 0xB80;
            sD->Batch[sD->BatNum].value = sD->EVOLL[core];
            sD->BatNum++;
        }
        if (sD->EVOLR[core] != sD->EVOLRbak[core]) {
            sD->Batch[sD->BatNum].func = 1;
            sD->Batch[sD->BatNum].entry = core | 0xC80;
            sD->Batch[sD->BatNum].value = sD->EVOLR[core];
            sD->BatNum++;
        }
        sD->EVOLLbak[core] = sD->EVOLL[core];
        sD->EVOLRbak[core] = sD->EVOLR[core];
        if (sD->VMIXL[core] != sD->VMIXLbak[core]) {
            sD->Batch[sD->BatNum].func = 2;
            sD->Batch[sD->BatNum].entry = core | 0x1800;
            sD->Batch[sD->BatNum].value = sD->VMIXL[core];
            sD->BatNum++;
        }
        if (sD->VMIXR[core] != sD->VMIXRbak[core]) {
            sD->Batch[sD->BatNum].func = 2;
            sD->Batch[sD->BatNum].entry = core | 0x1A00;
            sD->Batch[sD->BatNum].value = sD->VMIXR[core];
            sD->BatNum++;
        }
        if (sD->VMIXEL[core] != sD->VMIXELbak[core]) {
            sD->Batch[sD->BatNum].func = 2;
            sD->Batch[sD->BatNum].entry = core | 0x1900;
            sD->Batch[sD->BatNum].value = sD->VMIXEL[core];
            sD->BatNum++;
        }
        if (sD->VMIXER[core] != sD->VMIXERbak[core]) {
            sD->Batch[sD->BatNum].func = 2;
            sD->Batch[sD->BatNum].entry = core | 0x1B00;
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
        for (vnum = 0; vnum < 0x18; vnum++) {
            sD->Batch[core * 0x18 + vnum].func = 0x11;
            sD->Batch[core * 0x18 + vnum].entry = core | vnum << 1 | 0x500;
            sD->Batch[core * 0x18 + vnum].value = 0;
        }
    }
    sceSdProcBatch(sD->Batch, sD->ENVX, 0x30);
    for (core = 0; core < 2; core++) {
        sD->vStatEnv[core] = 0;
        for (vnum = 0; vnum < 0x18; vnum++) {
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
        for (vnum = 0; vnum < 0x18; vnum++) {
            sD->VoPrm[core][vnum].disable = ~mask & 1;
            mask >>= 1;
        }
    }
}

void SndInit() {
	int core;
	int vnum;

    sceSdInit(0);
    sceSdSetCoreAttr(10, 0x800);
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
        sceSdSetParam(core | 0x980, 0);
        sceSdSetParam(core | 0xA80, 0);
        for (vnum = 0; vnum < 0x18; vnum++) {
            sceSdSetParam(core | vnum << 1, 0);
            sceSdSetParam(core | vnum << 1 | 0x100, 0);
        }
        
        sD->VMIXL[core] = sD->VMIXLbak[core] = 0xFFFFFF;
        sD->VMIXR[core] = sD->VMIXRbak[core] = sD->VMIXL[core];
        if (core != 0) {
            sD->VMIXEL[core] = sD->VMIXELbak[core]= 0xF0000;
        } else {
            sD->VMIXEL[core] = sD->VMIXELbak[core]= 0;
        }
        sD->VMIXER[core] = sD->VMIXERbak[core]= sD->VMIXEL[core];
        
        sceSdSetSwitch(core | 0x1800, sD->VMIXL[core]);
        sceSdSetSwitch(core | 0x1A00, sD->VMIXR[core]);
        sceSdSetSwitch(core | 0x1900, sD->VMIXEL[core]);
        sceSdSetSwitch(core| 0x1B00, sD->VMIXER[core]);
        sceSdSetAddr(core | 0x1D00, 0x1FFFFF - core * 0x20000);
        sD->EffAttr[core].mode = 0x100;
        sD->EffAttr[core].depth_L = 0;
        sD->EffAttr[core].depth_R = 0;
        sD->EffAttr[core].delay = 0;
        sD->EffAttr[core].feedback = 0;
        sceSdSetEffectAttr(core, &sD->EffAttr[core]);
        sceSdSetCoreAttr(core | 2, 1);
        sceSdSetParam(core | 0xB80, 0);
        sceSdSetParam(core | 0xC80, 0);
        sceSdSetParam(core | 0xF80, 0);
        sceSdSetParam(core | 0x1080, 0);
    }

    sceSdSetParam(0x800, 0xFC0);
    sceSdSetParam(0x801, 0xFFC);
    sceSdSetTransCallback(0, &SndDmaInt);
    JamInit();
    RpcRet[0] = Mem.iBankhd;
}
