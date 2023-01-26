#ifndef DATA_H
#define DATA_H

#include "gbl.h"

/* DAT_00338390 */ extern hCDDATA CdData;
/* DAT_00338670 */ extern u8 EeCb[0x2000];
/* DAT_00348970 */ extern const char* modules[8];
/* DAT_0034b140 */ extern hSNDDATA SndData;
/* DAT_0034bb58 */ extern hBGMDATA BgmData;
/* DAT_0034bc00 */ extern hPPTDATA PptData;
/* DAT_0034bd50 */ extern hAC3DATA Ac3Data;
/* DAT_0036ead8 */ extern SYSGBL SysGbl;
/* DAT_00366450 */ extern GAME_WORK GameGbl;
/* DAT_003bfed0 */ extern hSTRDATA StrData;
/* DAT_003d9718 */ extern SifRpcClientData_t rpc__003d9718;
/* DAT_003fa6de */ extern s16 hrpt_deb;
/* DAT_003fa808 */ extern hCDDATA* cD;
/* DAT_003fa80c */ extern hCDCUE* cQ;
/* DAT_003fa830 */ extern int* kldataHead;
/* DAT_003fa834 */ extern int* pptpackHead;
/* DAT_003fa838 */ extern int* bgmpackHead;
/* DAT_003fa83c */ extern hSNDDATA* sD;
/* DAT_003fa840 */ extern hBGMDATA* bD;
/* DAT_003fa844 */ extern hPPTDATA* pD;
/* DAT_003fa848 */ extern hAC3DATA* aD;
/* DAT_00417700 */ extern u8 pptEeAddrs[4][0x40000];
/* DAT_003fbb3c */ extern void* buffstartptr;
/* DAT_003bfed0 */ extern hSTRDATA* strD;
/* DAT_0040efa0 */ extern int RpcArg[16];
/* DAT_00417200 */ extern int RpcRecvBuf[2][16];
/* DAT_00417300 */ extern int SndMainBuffer[16];
/* DAT_00417280 */ extern SifRpcClientData_t sndRpc;
/* DAT_00661ad0 */ extern int boot_flag; // ?

#endif