#include "common.h"

void hRpcInfo() {
    int i;
    int *ret = hRpc(IOP_RpcInfo);
    int *ret2;
    int *ret3;
    int *ret4;
    
    int *iopNext;
    sD->VoiceStat[0] = ret[0];
    sD->VoiceStat[1] = ret[1];

    ret2 = ret + 2;
    bD->iopPoint = ret2[1];
    bD->iopNext = ret[2];

    i = 0;
    iopNext = pD->iopNext;
    ret3 = ret2 + 2;
    for (; i < 4; i++) {
        iopNext[i] = ret3[i];
    }

    ret4 = ret2 + 2;
    aD->field_0xAC = ret4[4];
}

int hRpcStat() {
#ifdef SCE
    return SifCheckStatRpc(&sndRpc.rpcd);
#else
    return SifCheckStatRpc(&sndRpc);
#endif
}

void hRpcBind() {
    do {
        if (SifBindRpc(&sndRpc, 0x12346, 0) < 0) {
            while (true);
        }
        for (int i = 10000; i > 0; i--) {
            // Do nothing
        }
    }
#ifdef SCE
    while (!sndRpc.serve);
#else
    while (!sndRpc.server);
#endif
}

int * hRpc(s32 cmd) {
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

    int rsize;
    int ssize;
    int mode;
    int *send;
    int *receive;
    
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
    receive = RpcRecvBuf[0];
    mode = 0;

    switch (cmd) {
        case IOP_StrInfo:
        case 0x10000002: // Is this even a valid command?
            mode = SIF_RPCM_NOWAIT;
            break;
        case IOP_SndMain:
            send = SndMainBuffer;
            ssize = hSndPkGetSize();
            receive = RpcRecvBuf[1];
            mode = SIF_RPCM_NOWAIT;
            break;
        default:
            break;
    }

    sceSifCallRpc(&sndRpc, cmd, mode, send, ssize, receive, rsize, NULL, NULL);
    if ((cmd & 0xc000000) == 0x4000000) {
        return (int *)*receive;
    } else {
        return receive;
    }
}

s32 hRpc_0016c9b8(u8 *dest, u8 *src, u32 size) {
    u32 id;

#ifdef SCE
    sifdma_004171c0.data = (int)src;
    sifdma_004171c0.addr = (int)dest;
    sifdma_004171c0.size = size;
    sifdma_004171c0.mode = 0;
#else
    sifdma_004171c0.src = src;
    sifdma_004171c0.dest = dest;
    sifdma_004171c0.size = size;
    sifdma_004171c0.attr = 0;
#endif

    FlushCache(0);
    id = SifSetDma(&sifdma_004171c0, 1);
    if (id != 0) {
        while (SifDmaStat(id) >= 0);
        return 0;
    } else {
        return -1;
    }
}

u32 JamGetHdSize(u8 *hdaddr) {
    return *((u32 *)hdaddr + 4);
}

u32 JamGetBdSize(u8 *hdaddr) {
    return *((u32 *)hdaddr + 8);
}
