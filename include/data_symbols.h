#ifndef DATA_H
#define DATA_H

#include "gbl.h"
#include "okanoyo.h"
#include "harada.h"
#include "ps2.h"

/* DAT_00341460 */ extern hGAMEDATA GameData;
/* DAT_0034bd50 */ extern hAC3DATA Ac3Data;
/* DAT_0036ead8 */ extern SYSGBL SysGbl;
/* DAT_00366440 */ extern int (*GameFuncTbl[3])();
/* DAT_00366450 */ extern GAME_WORK GameGbl;
/* DAT_00366bb0 */ extern OBJWORK ObjWorkBuff[128];
/* DAT_0036ea60 */ extern NKDEBGBL nkDG;
/* DAT_00375b90 */ extern int (*nkInitAtrTbl[2])();
/* DAT_00375b90 */ extern FUNCTBL functbl[8];
/* DAT_00375bd0 */ extern FUNCTBL nkFuncs[5];
/* DAT_00375bf8 */ extern int (*nkFuncTbl[2])();
/* DAT_00398c00 */ extern SCRENV Scr;
/* DAT_003bfed0 */ extern hMOVDATA MovData;
/* DAT_003fb900 */ extern sceDmaChan* DmaChVIF0;
/* DAT_003fb904 */ extern sceDmaChan* DmaChVIF1;
/* DAT_003fb908 */ extern sceDmaChan* DmaChGIF;
/* DAT_003fb914 */ extern sceDmaChan* DmaChfromSPR;
/* DAT_003fb918 */ extern sceDmaChan* DmaChtoSPR;
/* DAT_003fa804 */ extern s32 ht_g_rand;
/* DAT_003fa810 */ extern hGAMEDATA *gD;
/* DAT_003fa834 */ extern PPTTABLE* PptTable;
/* DAT_003fa838 */ extern BGMTABLE* BgmTable;
/* DAT_003fa848 */ extern hAC3DATA* aD;
/* DAT_003fad80 */ extern int klworks_flag;
/* DAT_003fad84 */ extern DL_STATUS dl_status;
/* DAT_003fad88 */ extern int ret_gameread;
/* DAT_003fadcc */ extern int DAT_003fadcc;
/* DAT_003faf2c */ extern int disp_mask_flag;
/* DAT_003fb8ec */ extern int nkLoadTimer;
/* DAT_003fb8f0 */ extern int nkLoadStat;
/* DAT_003fb8f4 */ extern int nkLoadBun;
/* DAT_003fb93c */ extern u8 *DAT_003fb93c;
/* DAT_003fc46c */ extern int key_data_top_index;
/* DAT_003fc470 */ extern int key_frame_cnt;
/* DAT_003fc474 */ extern int key_frame_max;
/* DAT_003fc478 */ extern int kf_mode;
/* DAT_003bfed0 */ extern hMOVDATA* mD;
/* DAT_00417700 */ extern u8 pptEeAddrs[4][0x40000];

/* DAT_00639770 */ extern int DAT_00639770;
/* DAT_00639774 */ extern int DAT_00639774;
/* DAT_00639778 */ extern int DAT_00639778;
/* DAT_0063977c */ extern int DAT_0063977c;
/* DAT_00639780 */ extern int DAT_00639780;
/* DAT_00639784 */ extern int DAT_00639784;
/* DAT_006397c8 */ extern int DAT_006397c8;
/* DAT_006397cc */ extern int DAT_006397cc;
/* DAT_006397d0 */ extern int DAT_006397d0;
/* DAT_006397d8 */ extern int DAT_006397d8;
/* DAT_006397dc */ extern int DAT_006397dc;
/* DAT_006397e0 */ extern float DAT_006397e0;
/* DAT_006397e8 */ extern float DAT_006397e8;
/* DAT_006397ec */ extern float DAT_006397ec;
/* DAT_006397f0 */ extern float DAT_006397f0;
/* DAT_00661ad0 */ extern int boot_flag; // ?

#endif
