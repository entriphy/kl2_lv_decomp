#ifndef DATA_H
#define DATA_H

#include "gbl.h"
#include "okanoyo.h"
#include "harada.h"
#include "ps2.h"

/* DAT_00336100 */ extern int ptflag_buff[4];
/* DAT_00336110 */ extern int ptflag_st[4];
/* DAT_00336120 */ extern int ptflag_ar[4];
/* DAT_00336130 */ extern int ptflag_th[4];
/* DAT_00336bb0 */ extern PTDATA scene_table[90];
/* DAT_00338390 */ extern hCDDATA CdData;
/* DAT_00338670 */ extern u8 EeCb[0x2000];
/* DAT_00348970 */ extern const char* modules[8];
/* DAT_0034b140 */ extern hSNDDATA SndData;
/* DAT_0034bb58 */ extern hBGMDATA BgmData;
/* DAT_0034bc00 */ extern hPPTDATA PptData;
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
/* DAT_00376798 */ extern OKDEBDATAEX testdb;
/* DAT_0038a108 */ extern PROFILE profile;
/* DAT_0038a138 */ extern void* DAT_0038a138;
/* DAT_0038a268 */ extern DEBMEM OkDebMem[10];
/* DAT_0038a308 */ extern OKDEBDATA okDebData;
/* DAT_0038a330 */ extern PERSONAL_DEB PDbuf[9];
/* DAT_00395028 */ extern OKPROFILE okProfile;
/* DAT_003bfed0 */ extern hMOVDATA MovData;
/* DAT_003d9718 */ extern SifClientData rpc__003d9718;
/* DAT_003fb900 */ extern sceDmaChan* DmaChVIF0;
/* DAT_003fb904 */ extern sceDmaChan* DmaChVIF1;
/* DAT_003fb908 */ extern sceDmaChan* DmaChGIF;
/* DAT_003fb914 */ extern sceDmaChan* DmaChfromSPR;
/* DAT_003fb918 */ extern sceDmaChan* DmaChtoSPR;
/* DAT_003fa3c8 */ extern int hr_abeoff;
/* DAT_003fa6cc */ extern void* hrpt_addr;
/* DAT_003fa6d0 */ extern void* hrptm_addr;
/* DAT_003fa6d4 */ extern short hrpt_flag;
/* DAT_003fa6d6 */ extern short hrpt_scene;
/* DAT_003fa6d8 */ extern short hrpt_view;
/* DAT_003fa6da */ extern short hrpt_th;
/* DAT_003fa6dc */ extern short hrpt_vt;
/* DAT_003fa6de */ extern s16 hrpt_deb;
/* DAT_003fa6e0 */ extern PT hrpt_id;
/* DAT_003fa6e4 */ extern short hrptm_res;
/* DAT_003fa704 */ extern int hrpt_sbgm;
/* DAT_003fa804 */ extern s32 ht_g_rand;
/* DAT_003fa808 */ extern hCDDATA* cD;
/* DAT_003fa80c */ extern hCDCUE* cQ;
/* DAT_003fa830 */ extern KLTABLE* KlTable;
/* DAT_003fa834 */ extern PPTTABLE* PptTable;
/* DAT_003fa838 */ extern BGMTABLE* BgmTable;
/* DAT_003fa83c */ extern hSNDDATA* sD;
/* DAT_003fa840 */ extern hBGMDATA* bD;
/* DAT_003fa844 */ extern hPPTDATA* pD;
/* DAT_003fa848 */ extern hAC3DATA* aD;
/* DAT_003fad80 */ extern int klworks_flag;
/* DAT_003fad84 */ extern DL_STATUS dl_status;
/* DAT_003fad88 */ extern int ret_gameread;
/* DAT_003fadcc */ extern int DAT_003fadcc;
/* DAT_003fb8ec */ extern int nkLoadTimer;
/* DAT_003fb8f0 */ extern int nkLoadStat;
/* DAT_003fb8f4 */ extern int DAT_003fb8f4;
/* DAT_003fb934 */ extern void *NakanoPackAddr;
/* DAT_003fb93c */ extern void *DAT_003fb93c;
/* DAT_003fbaec */ extern void* texdeb;
/* DAT_003fbb38 */ extern void* buffstartptrORG;
/* DAT_003fbb3c */ extern void* buffstartptr;
/* DAT_003fbb40 */ extern void* buffstagetop;
/* DAT_003fbba8 */ extern int OkZakoFuncOn;
/* DAT_003fbc90 */ extern long profileFlag;
/* DAT_003fbaf4 */ extern int fontDispID;
/* DAT_003fc2e8 */ extern void *DAT_003fc2e8; // tblMax?
/* DAT_003fc2ec */ extern void *areaBuff;
/* DAT_003fc468 */ extern void *DAT_003fc468;
/* DAT_003fc46c */ extern int key_data_top_index;
/* DAT_003fc470 */ extern int key_frame_cnt;
/* DAT_003fc474 */ extern int key_frame_max;
/* DAT_003fc478 */ extern int kf_mode;
/* DAT_003bfed0 */ extern hMOVDATA* mD;
/* DAT_0040efa0 */ extern int RpcArg[16];
/* DAT_004171c0 */ extern SifDmaData sifdma_004171c0;
/* DAT_00417200 */ extern int RpcRecvBuf[2][16];
/* DAT_00417280 */ extern SifClientData sndRpc;
/* DAT_00417300 */ extern int SndMainBuffer[16];
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
