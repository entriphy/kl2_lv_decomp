// *****************************************************************************
// FILE -- /home/hoshino/klonoa2/src/hoshino/iop/src/main.c
// *****************************************************************************

#include "iop.h"

/* bss 1f60 */ IOP_MEM Mem;
/* bss 1f20 */ int RpcRet[16];
/* bss 1ee0 */ int RpcInfo[16];

/* 00007ce0 00007da8 */ void IopInit() {

}

/* 00007da8 00007fc8 */ static void* dispatch(/* 0x0(sp) */ u_int cmd, /* 0x4(sp) */ void *data, /* 0x8(sp) */ int size) {
	/* -0x10(sp) */ int *arg;
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
