#ifndef DATA_H
#define DATA_H

#include "okanoyo.h"
#include "harada.h"
#include "ps2.h"

/* DAT_00341460 */ extern hGAMEDATA GameData;
/* DAT_0034bd50 */ extern hAC3DATA Ac3Data;
/* DAT_003bfed0 */ extern hMOVDATA MovData;
/* DAT_003fa810 */ extern hGAMEDATA *gD;
/* DAT_003fa834 */ extern PPTTABLE* PptTable;
/* DAT_003fa838 */ extern BGMTABLE* BgmTable;
/* DAT_003fa848 */ extern hAC3DATA* aD;
/* DAT_003fad80 */ extern int klworks_flag;
/* DAT_003fad84 */ extern DL_STATUS dl_status;
/* DAT_003fad88 */ extern int ret_gameread;
/* DAT_003fadcc */ extern int klwcnt;
/* DAT_003bfed0 */ extern hMOVDATA* mD;
/* DAT_00417700 */ extern u8 pptEeAddrs[4][0x40000];

#endif
