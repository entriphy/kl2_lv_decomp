#include "common.h"

int FUN_0016c778() {
#ifdef SCE
    return SifCheckStatRpc(&sndRpc.rpcd);
#else
    return SifCheckStatRpc(&sndRpc);
#endif
}

void FUN_0016c798() {
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

// TODO: Fix this
int *hRpc(u32 param) {
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
    switch (param & 0xc000000) {
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

    int ssize;
    switch (param & 0x3000000) {
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

    int mode = 0;
    int* send = RpcArg;
    int* receive = RpcRecvBuf[0];
    if (param == 0x2a000001) {
        send = SndMainBuffer;
        ssize = hSndPkGetSize();
        receive = RpcRecvBuf[1];
    } else {
        mode = SIF_RPC_M_NOWAIT;
    }

    SifCallRpc(&sndRpc, param, mode, send, ssize, receive, rsize, 0, 0);
    if ((param & 0xc000000) == 0x4000000) {
        // TODO
    }
    return receive;
}

s32 FUN_0016c9b8(u8 *dest, u8 *src, u32 size) {
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
