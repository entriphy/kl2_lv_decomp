#include "gflow.h"
#include "kazuya/disp.h"
#include "dma.h"
#include "game.h"
#include "gssub.h"

void GameFlow() {
    if (GameGbl.wipe.intime > 0)
        GameGbl.wipe.intime--;
    
    if (GameGbl.init_vision != 0) {
        kzSetDispMaskOn();
        nkPathReset();
        nkPathBlock();
        nkInitPS2();
        nkGsSetNormalFZ2_0();
        nkWipeEffBClear();
        nkGsSetNormalFZ2_1();
        nkWipeEffBClear();
        TkRemoveAllEffect();

        if ((GameGbl.vision & 0xFF00) != (GameGbl.init_vision & 0xFF00)) {
            GameGbl.vision = GameGbl.init_vision;
            SysGbl.smode = 0;
        } else {
            GameGbl.vision = GameGbl.init_vision;
            InitAllObjWork2(SysGbl.objwork, SysGbl.n_objw);
            nkAreaInit();
        }
    } else if (GameGbl.wipe.outtime > 0) {
        if (GameGbl.wipe.flag & 2 && GameGbl.wipe.fdata > 0) {
            if (--GameGbl.wipe.fdata == 0)
                hGameAreaEnd(GameGbl.wipe.next_vision);
        } else {
            if (--GameGbl.wipe.outtime < 1) {
                GameGbl.wipe.flag &= 0xFFFD;
                if (GameGbl.wipe.next_vision != 0) {
                    kzSetDispMaskOn();
                    nkPathReset();
                    nkPathBlock();
                    GameGbl.wipe.intime = 64;
                    GameGbl.vision = GameGbl.wipe.next_vision;
                    GameGbl.wipe.next_vision = 0;
                    GameGbl.st_rtn = GameGbl.wipe.rtn;
                    GameGbl.st_mcn = GameGbl.wipe.mcn;
                    GameGbl.Re_st_rtn = -1;
                    nkInitPS2();
                    InitAllObjWork2(SysGbl.objwork, SysGbl.n_objw);
                    TkRemoveAllEffect();
                    nkAreaInit();
                }
            }
        }
    } else {
        if (GameGbl.vision >> 8 < 0x1E)
            nkRtConnMng(GameGbl.klonoa);
    }
}
