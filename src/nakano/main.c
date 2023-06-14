#include "common.h"

NKDEBGBL nkDG = {};
SYSGBL SysGbl = {};
sceDmaChan *DmaChVIF0 = NULL;
sceDmaChan *DmaChVIF1 = NULL;
sceDmaChan *DmaChGIF = NULL;
sceDmaChan *DmaChfromIPU = NULL;
sceDmaChan *DmaChtoIPU = NULL;
sceDmaChan *DmaChfromSPR = NULL;
sceDmaChan *DmaChtoSPR = NULL;
qword PsfxWorkBuff[131072];
vpmINFO VpmInfo = {};
qword *p1_packet_top = NULL;
qword *p1_packet = NULL;
qword *p1_packet_cnt = NULL;
qword p1_packet_buff[2][4096];
s32 draw_id = 0;
qword p1_ot_buff[2][5];
qword *p1_ot = NULL;
s32 p1_packet_max = 0;
qword vu0mem_image[256] = {};
qword vu1mem_image[1024] = {};
u32 *NakanoPackAdr = NULL;
u32 *nkGimPackAdr = NULL;
u32 *vtWaveAdr = NULL;
u32 *vtSprayAdr = NULL;
u32 *vtMovieAdr[8] = {};

s32 main(s32 argc, char *argv[]) {
    memorySysFormat();
    hInitBoot();
    SysGbl.irqc = 0;
    init_config_system();
    *T0_MODE = T_MODE_CLKS_M | T_MODE_CUE_M;
    nkInitPad();
    OkMainInit(argc, argv);
    nkInitSys();
    hr_cold_start();
    hrpt_deb = 1;
    SysGbl.fmode = 1;
    GameGbl.vision = 0x1e00;
    nkDG.opflag |= 6;
    while (!sceGsSyncV(0));
    
    while (1) {
        hLoopTop();
        MainFunc();
        SysGbl.irqc++;
    }

    return 0;
}
