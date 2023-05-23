#include "common.h"

static s32 sbuff[16] __attribute__((aligned(16)));
static s32 sndBuff[16] __attribute__((aligned(16)));
static sceSifClientData gCd;

void hSndRpcRet() {
    hRPCINFO *ret = (hRPCINFO *)hRpc(IOP_RpcInfo);
    hSTRINFO2 *str;
    int i;
    
    sD->VoiceStat[0] = ret->vStatKeyon[0];
    sD->VoiceStat[1] = ret->vStatKeyon[1];

    str = &ret->STRinfo;
    bD->iopPoint = str->BGMpoint;
    bD->iopNext = str->BGMnext;
    
    for (i = 0; i < 4; i++) {
        pD->iopNext[i] = str->PPTnext[i];
    }
    
    aD->AC3stat = str->AC3stat;
}

s32 hRpcSync() {
    return sceSifCheckStatRpc(&gCd.rpcd);
}

void hRpcInit() {
    int i;

    do {
        if (sceSifBindRpc(&gCd, 0x12346, 0) < 0) {
            while (true);
        }
        for (i = 10000; i > 0; i--) {
            // Do nothing
        }
    } while (!gCd.serve);
}

s32 hRpc(s32 cmd) {
    // 0x08000000: IopInit(),        r = 64, s = 0
    // 0x08000001: return RpcInfo,   r = 64, s = 0
    // 0x10000003: StrKick(),        r = 0,  s = 0
    // 0x14000000: StrInit(),        r = 16, s = 0
    // 0x1b000001: StrInfo(data),    r = 64, s = 128
    // 0x20000004: JamBdTrans(),     r = 0,  s = 0
    // 0x21000003: JamBankSet(data), r = 0,  s = 16
    // 0x22000002: SndMask(data),    r = 0,  s = 64
    // 0x24000000: SndInit(),        r = 16, s = 0
    // 0x2a000001: SndMain(data),    r = 64, s = 64

    s32 rsize;
    s32 ssize;
    s32 mode;
    s32 *send;
    s32 *receive;
    
    switch (cmd & 0xc000000) {
        case 0x4000000:
            rsize = 16;
            break;
        case 0x8000000:
            rsize = 64;
            break;
        case 0xc000000:
            rsize = 128;
            break;
        default:
            rsize = 0;
            break;
    }
    
    switch (cmd & 0x3000000) {
        case 0x1000000:
            ssize = 16;
            break;
        case 0x2000000:
            ssize = 64;
            break;
        case 0x3000000:
            ssize = 128;
            break;
        default:
            ssize = 0;
            break;
    }
    
    send = RpcArg;
    receive = sbuff;
    mode = 0;

    switch (cmd) {
        case IOP_StrInfo:
        case 0x10000002: // Is this even a valid command?
            mode = SIF_RPCM_NOWAIT;
            break;
        case IOP_SndMain:
            send = (s32 *)SndPacket;
            ssize = hSndPkGetSize();
            receive = sndBuff;
            mode = SIF_RPCM_NOWAIT;
            break;
        default:
            break;
    }

    sceSifCallRpc(&gCd, cmd, mode, send, ssize, receive, rsize, NULL, NULL);
    if ((cmd & 0xc000000) == 0x4000000) {
        return *receive;
    } else {
        return (s32)receive;
    }
}

s32 hTrans2IOP(s32 iopAddr, s32 eeAddr, s32 size) {
    static sceSifDmaData transData;
    u32 did;

    transData.data = eeAddr;
    transData.addr = iopAddr;
    transData.size = size;
    transData.mode = 0;

    FlushCache(WRITEBACK_DCACHE);
    did = sceSifSetDma(&transData, 1);
    if (did != 0) {
        while (sceSifDmaStat(did) >= 0);
        return 0;
    } else {
        return -1;
    }
}

