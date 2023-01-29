#ifndef DATA_H
#define DATA_H

#include <sifdma.h>
#include "gbl.h"
#include "okanoyo.h"
#include "harada.h"

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
/* DAT_00366450 */ extern GAME_WORK GameGbl;
/* DAT_0036ea60 */ extern NKDEBGBL nkDG;
/* DAT_00376798 */ extern OKDEBDATAEX testdb;
/* DAT_0038a108 */ extern PROFILE profile;
/* DAT_0038a138 */ extern void* DAT_0038a138;
/* DAT_0038a268 */ extern DEBMEM OkDebMem[10];
/* DAT_0038a308 */ extern OKDEBDATA okDebData;
/* DAT_0038a330 */ extern PERSONAL_DEB PDbuf[9];
/* DAT_00395028 */ extern OKPROFILE okProfile;
/* DAT_003bfed0 */ extern hSTRDATA StrData;
/* DAT_003d9718 */ extern SifRpcClientData_t rpc__003d9718;
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
/* DAT_003fbaec */ extern void* texdeb;
/* DAT_003fbb38 */ extern void* buffstartptrORG;
/* DAT_003fbb3c */ extern void* buffstartptr;
/* DAT_003fbb40 */ extern void* buffstagetop;
/* DAT_003fbba8 */ extern int OkZakoFuncOn;
/* DAT_003fbc90 */ extern long profileFlag;
/* DAT_003fbaf4 */ extern int fontDispID;
/* DAT_003fc2ec */ extern void* areaBuff;
/* DAT_003bfed0 */ extern hSTRDATA* strD;
/* DAT_0040efa0 */ extern int RpcArg[16];
/* DAT_004171c0 */ extern SifDmaTransfer_t sifdma_004171c0;
/* DAT_00417200 */ extern int RpcRecvBuf[2][16];
/* DAT_00417280 */ extern SifRpcClientData_t sndRpc;
/* DAT_00417300 */ extern int SndMainBuffer[16];
/* DAT_00417700 */ extern u8 pptEeAddrs[4][0x40000];
/* DAT_00661ad0 */ extern int boot_flag; // ?

#endif