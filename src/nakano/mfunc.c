#include "common.h"

static FUNCTBL NameFuncTbl[8] = {
    {nkFuncTbl, -1}
};

void MainFunc() {
    if (NameFuncTbl[SysGbl.nmode].func == NULL) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    } else {
        NameFuncTbl[SysGbl.nmode].func[SysGbl.nsmode]();
    }
}
