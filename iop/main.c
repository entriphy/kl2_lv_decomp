#include "iop.h"

IOP_MEM Mem;
int RpcRet[16];
int RpcInfo[16];

void IopInit() {
    Mem.iTop = AllocSysMemory(0, 0x150180, NULL);
    Mem.iStream = Mem.iTop;
    Mem.iBankhd = Mem.iTop + 0x130180;
    Mem.iBankbd = Mem.iTop + 0x140180;
    Mem.sBankbd = 0x5c10;
    RpcRet[0] = Mem.iBankhd;
    RpcRet[1] = Mem.iStream;
}

static void * dispatch(u_int cmd, void *data, int size) {
    int *arg;

    arg = data;
    
    switch (cmd) {
        case IOP_IopInit:
            IopInit();
            break;
        case IOP_RpcInfo:
            return RpcInfo;
            break;
        case IOP_StrInit:
            StrInit();
            break;
        case IOP_StrInfo:
            StrInfo(data);
            break;
        case IOP_StrKick:
            StrKick();
            break;
        case IOP_SndInit:
            SndInit();
            break;
        case IOP_SndMain:
            SndMain(data);
            break;
        case IOP_SndMask:
            SndMask(data);
            break;
        case IOP_JamBankSet:
            JamBankSet(*arg);
            break;
        case IOP_JamBdTrans:
            JamBdTrans();
            break;
        case 2: // ?
            break;
        default:
            break;
    }
	
    return RpcRet;
}

int MainThread() {
    static u32 arg[256];
    sceSifQueueData qd;
    sceSifServeData sd;

    CpuEnableIntr();
    EnableIntr(0x24);
    EnableIntr(0x28);
    EnableIntr(9);
    sceSifInitRpc(0);
    sceSifSetRpcQueue(&qd, GetThreadId());
    sceSifRegisterRpc(&sd, 0x12346, &dispatch, &arg, 0, 0, &qd);
    sceSifRpcLoop(&qd);
    return 0;
}
