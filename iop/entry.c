#include "iop.h"

ModuleInfo Module = { "KL2 Driver", 0x0101 };

int start() {
	struct ThreadParam param;
	int th;

    CpuEnableIntr();
    if (sceSifCheckInit() == 0) {
        sceSifInit();
    }
    sceSifInitRpc(0);
    
    param.attr = TH_C;
    param.entry = &MainThread;
    param.initPriority = 30;
    param.stackSize = 0x800;
    param.option = 0;

    th = CreateThread(&param);
    if (th > 0) {
        StartThread(th, 0);
        return 0;
    } else {
        return 1;
    }
}
