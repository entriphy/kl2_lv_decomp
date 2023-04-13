// *****************************************************************************
// FILE -- /home/hoshino/klonoa2/src/hoshino/iop/src/entry.c
// *****************************************************************************

#include "iop.h"

ModuleInfo Module = { "KL2 Driver", 0x0101 };;

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
