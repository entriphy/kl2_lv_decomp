#include "init.h"
#include "harada/hr_pall.h"
#include "main.h"
#include "gssub.h"

sceDmaChan *DmaChVIF0;
sceDmaChan *DmaChVIF1;
sceDmaChan *DmaChGIF;
sceDmaChan *DmaChfromSPR;
sceDmaChan *DmaChtoSPR;
OBJWORK ObjWorkBuff[128];

void init_config_system() {
    sceDevVif0Reset();
    sceDevVu0Reset();
    sceGsResetPath();
    sceDmaReset(1);
    SysGbl.nmode = 0;
    SysGbl.nsmode = 0;
    SysGbl.fmode = 0;
    SysGbl.smode = 0;
    GameGbl.kpd[0].cnt = 0;
    GameGbl.kloZanki[0] = 3;
    GameGbl.vs_clear_cnt = 160;
    GameGbl.time_atack_best = 80;
    nkGsInitFrm();
    SysGbl.Language = 1;
    SysGbl.TVSystem = 2;
}

void nkInitSys() {
    // TODO
    nkDG.capture_num = 0;
    nkDG.map_draw_flag = 0;
    nkDG.opflag = 0;
    hrpt_deb = 0;
}
