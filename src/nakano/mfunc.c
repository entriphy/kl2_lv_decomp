#include "common.h"

static FUNCTBL NameFuncTbl[8] = {
    {nkFuncTbl, -1},
    {}, // OkFuncTbl
    {}, // HFuncTbl
    {}, // hrFuncTbl
    {abFuncTbl, -1},
    {}, // htFuncTbl
    {}, // tkFuncTbl
    {}  // kzFuncTbl
};

void MainFunc() {
    if (NameFuncTbl[SysGbl.nmode].func == NULL) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    } else {
        NameFuncTbl[SysGbl.nmode].func[SysGbl.nsmode]();
    }
}
