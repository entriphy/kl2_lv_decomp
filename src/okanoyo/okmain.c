#include "common.h"

u8 *scr_p[9] = { scram_0, scram_1, scram_2, scram_3, scram_4, scram_5, scram_6, scram_7, scram_8 };
TEXINFO texinfo[256] = {};
TEXINFODEB texdeb[256] = {};
s32 fontTexID = 0;
s32 fontDispID = 0;
s32 menuID = 0;
s32 scr_x = 0;
s32 scr_y = 0;
s32 fsz_x = 0;
s32 fsz_y = 0;
PROFILE profile = {};
u8 loginName[16] = {};
s32 OBJWORKSortFlag = 0;
s32 DebTestOn = 0;
s32 *prgBufPtr[4] = {};
s32 *prgBufTop[4] = {};
s32 prgBufPtrIdxMax[4];
s32 zakodataflag = 0;
u8 *teki_shutugen = NULL;
u8 *teki_Rshutugen = NULL;
u8 *zoutflgBufp[32] = {};
s32 *ioutflgBufp[32] = {};
s32 okzcom_mode = 0;
s32 OkDebugGameMode = 0;
s32 texinfocnt = 0;
s32 OkDebugCamCurMode = 0;
s32 alive_mode = 0;
DEBMEM OkDebMem[10] = {};
s32 *MMptr = NULL;
s32 *buffstartptrORG = NULL;
s32 *buffstartptr = NULL;
s32 *buffstagetop = NULL;
s32 *buffareatop = NULL;
u8 *ZAKODataBuffp = NULL;
u8 *ZAKODataBuffTop = NULL;
s32 fontOnOff = 0;
void *ItemSetBufp = NULL;
s32 ItemSetFlag = 0;
s32 *siworkp = NULL;
u8 *ITEMDataBuffp = NULL;
s32 itemdataflag = 0;
s32 OkDebDPcnt = 0;
s32 OkDebHTcnt = 0;
s32 OkDebKLcnt = 0;
s32 OkDebEXcnt = 0;
s32 OkDebDPTrg = 0;
s32 OkDebHTTrg = 0;
s32 OkDebKLTrg = 0;
s32 OkDebEXTrg = 0;
s32 OkDebM_Elf = 0;
s32 OkDebZKRTno = 0;
s32 *OkDebSIDTop = NULL;
s32 OkDebSIDflag = 0;
s32 *OkSIDBufp = NULL;
s32 *OkSTGBufp = NULL;
s32 OkZakoFuncOn = 0;
void *OkDPGVpo = NULL;
void *OkDPBVpo = NULL;
s32 OkM_ElfCnt = 0;
s32 LastBalloonPic = 0;
OKDEBDATA okDebData = {};
PERSONAL_DEB PDbuf[9] = {};
u8 scram_0[4480] = {};
u8 scram_1[4480] = {};
u8 scram_2[4480] = {};
u8 scram_3[4480] = {};
u8 scram_4[4480] = {};
u8 scram_5[4480] = {};
u8 scram_6[4480] = {};
u8 scram_7[4480] = {};
u8 scram_8[4480] = {};
s32 clr_f[9] = {};
s32 fontID = 0;
void * (*OkDebugGame2pTbl[9])() = {};
OkCWork okCwork = {};
s16 *shutu[68] = {};
// TEKI_DATA tekidata = {};