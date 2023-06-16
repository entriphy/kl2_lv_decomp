// File written by stdump on 2023-04-10
// 
// Input file:
//   KL2.IRX
// Toolchain version(s):
//   unknown
// Built-in types:
//   char                     8-bit integer
//   complex double           64-bit floating point
//   complex float            32-bit floating point
//   complex long double      64-bit floating point
//   double                   64-bit floating point
//   float                    32-bit floating point
//   int                      32-bit signed integer
//   long double              64-bit floating point
//   long int                 32-bit signed integer
//   long long int            64-bit signed integer
//   long long unsigned int   64-bit unsigned integer
//   long unsigned int        error
//   short int                16-bit signed integer
//   short unsigned int       16-bit unsigned integer
//   signed char              8-bit signed integer
//   unsigned char            8-bit unsigned integer
//   unsigned int             error

#ifndef IOP_H
#define IOP_H

#include <libsd.h>
#include <kernel.h>
#include <sif.h>
#include <sifrpc.h>
#include <thread.h>
#include <string.h>

typedef char s8;
typedef short int s16;
typedef int s32;
typedef long int s64;
typedef u_char u8;
typedef u_short u16;
typedef u_int u32;
typedef u_long u64;

typedef struct { // 0x18
	/* 0x00 */ u_int iTop;
	/* 0x04 */ u_int iStream;
	/* 0x08 */ u_int iPuppet;
	/* 0x0c */ u_int iBankhd;
	/* 0x10 */ u_int iBankbd;
	/* 0x14 */ u_int sBankbd;
} IOP_MEM;

typedef struct { // 0x48
	/* 0x00 */ int iopID;
	/* 0x04 */ int iopIDnext;
	/* 0x08 */ int iopOffset;
	/* 0x0c */ int spuAddr[2][2];
	/* 0x1c */ int spuZero[2][2];
	/* 0x2c */ int Vnum[2];
	/* 0x34 */ int Ch;
	/* 0x38 */ int Vol;
	/* 0x3c */ int VolBak;
	/* 0x40 */ int iopFine[2];
} BGMDATA;

typedef struct { // 0x34
	/* 0x00 */ int iopID;
	/* 0x04 */ int iopNext;
	/* 0x08 */ int iopOffset;
	/* 0x0c */ int iopSize;
	/* 0x10 */ int iopAddr[2];
	/* 0x18 */ int spuAddr[2];
	/* 0x20 */ int spuZero[2];
	/* 0x28 */ int Vnum;
	/* 0x2c */ int Vol;
	/* 0x30 */ int VolBak;
} PPTDATA;

typedef struct { // 0x80
	/* 0x00 */ int DmaWait;
	/* 0x04 */ int ThID;
	/* 0x08 */ int Sem;
	/* 0x0c */ int intCnt;
	/* 0x10 */ int eeCnt;
	/* 0x14 */ int spuID;
	/* 0x18 */ int ZeroBuff;
	/* 0x1c */ int ZeroBuff2;
	/* 0x20 */ int BGMsp;
	/* 0x24 */ int BGMcnt;
	/* 0x28 */ int BGMpoint;
	/* 0x2c */ int BGMlength;
	/* 0x30 */ int BGMskipsize;
	/* 0x34 */ int BGMch2;
	/* 0x38 */ int BGMchmax2;
	/* 0x3c */ int BGMstat;
	/* 0x40 */ int BGMaddr[2];
	/* 0x48 */ int BGMok[2];
	/* 0x50 */ int BGMfinetop[2];
	/* 0x58 */ int BGMfine[2];
	/* 0x60 */ int BGMchmax;
	/* 0x64 */ int BGMstereo;
	/* 0x68 */ int BGMbasevol;
	/* 0x6c */ int BGMchkcnt;
	/* 0x70 */ int BGMerror;
	/* 0x74 */ int PPTstat;
	/* 0x78 */ int ac3Size;
	/* 0x7c */ int ac3Buff;
} STRDATA;

// warning: multiple differing types with the same name (#1,  not equal)
enum {
	BGM_PLAY = 0,
	BGM_PAUSE = 1,
	BGM_RESET = 2
};

// warning: multiple differing types with the same name (#1,  not equal)
enum {
	PPT_PLAY = 0,
	PPT_PAUSE = 1,
	PPT_RESET = 2
};

typedef struct { // 0x58
	/* 0x00 */ int eeCnt;
	/* 0x04 */ int Command;
	/* 0x08 */ int BGMstereo;
	/* 0x0c */ int BGMlength;
	/* 0x10 */ int BGMskipsize;
	/* 0x14 */ int BGMvol;
	/* 0x18 */ char BGMch;
	/* 0x19 */ char BGMchmax;
	/* 0x1a */ char BGMok[2];
	/* 0x1c */ int BGMfinetop[2];
	/* 0x24 */ int BGMfine[2];
	/* 0x2c */ int PPTvol[4];
	/* 0x3c */ int PPTsize[4];
	/* 0x4c */ char PPTstop[4];
	/* 0x50 */ int work0;
	/* 0x54 */ int AC3size;
} STRINFO;

typedef struct { // 0x1c
	/* 0x00 */ int BGMnext;
	/* 0x04 */ int BGMpoint;
	/* 0x08 */ int PPTnext[4];
	/* 0x18 */ int AC3stat;
} STRINFO2;

// warning: multiple differing types with the same name (#1,  not equal)
enum {
	SNDCMD_KEYON = 0,
	SNDCMD_KEYOFF = 1,
	SNDCMD_KEYOFFALL = 2,
	SNDCMD_PITCH = 3,
	SNDCMD_PALPITCH = 4,
	SNDCMD_VOL = 5,
	SNDCMD_MVOL = 6,
	SNDCMD_MVOLALL = 7,
	SNDCMD_EFFECT = 8,
	SNDCMD_EVOL = 9
};

typedef struct { // 0x1c
	/* 0x00 */ u32 SSA;
	/* 0x04 */ u16 ADSR1;
	/* 0x06 */ u16 ADSR2;
	/* 0x08 */ u16 VOLL;
	/* 0x0a */ u16 VOLR;
	/* 0x0c */ u16 PITCH;
	/* 0x0e */ u16 BendRangeL;
	/* 0x10 */ u16 BendRangeH;
	/* 0x12 */ u8 VmixL;
	/* 0x13 */ u8 VmixR;
	/* 0x14 */ u8 VmixEL;
	/* 0x15 */ u8 VmixER;
	/* 0x16 */ u8 Core;
	/* 0x17 */ u8 Loop;
	/* 0x18 */ u8 p1;
	/* 0x19 */ u8 p2;
} SNDKEYPRM;

typedef struct { // 0x1c
	/* 0x00 */ u16 flag;
	/* 0x02 */ u8 disable;
	/* 0x03 */ u8 loop;
	/* 0x04 */ u16 PITCH;
	/* 0x06 */ u16 VOLL;
	/* 0x08 */ u16 VOLR;
	/* 0x0a */ u16 ADSR1;
	/* 0x0c */ u16 ADSR2;
	/* 0x10 */ u32 SSA;
	/* 0x14 */ u16 setPitch;
	/* 0x16 */ s16 setVolL;
	/* 0x18 */ s16 setVolR;
} SNDVOPRM;

typedef struct { // 0x16dc
	/* 0x0000 */ sceSdBatch Batch[512];
	/* 0x1000 */ SNDKEYPRM KeyPrm;
	/* 0x101c */ SNDVOPRM VoPrm[2][24];
	/* 0x155c */ int DmaWait;
	/* 0x1560 */ int BatNum;
	/* 0x1564 */ sceSdEffectAttr EffAttr[2];
	/* 0x158c */ u32 EffChange[2];
	/* 0x1594 */ u32 vStatKeyon[2];
	/* 0x159c */ u32 vStatEnv[2];
	/* 0x15a4 */ u32 KeyOnV[2];
	/* 0x15ac */ u32 KeyOffV[2];
	/* 0x15b4 */ u32 KeyMask[2];
	/* 0x15bc */ u32 ENVX[2][24];
	/* 0x167c */ u32 VMIXL[2];
	/* 0x1684 */ u32 VMIXLbak[2];
	/* 0x168c */ u32 VMIXR[2];
	/* 0x1694 */ u32 VMIXRbak[2];
	/* 0x169c */ u32 VMIXEL[2];
	/* 0x16a4 */ u32 VMIXELbak[2];
	/* 0x16ac */ u32 VMIXER[2];
	/* 0x16b4 */ u32 VMIXERbak[2];
	/* 0x16bc */ u16 MVOLL[2];
	/* 0x16c0 */ u16 MVOLLbak[2];
	/* 0x16c4 */ u16 MVOLR[2];
	/* 0x16c8 */ u16 MVOLRbak[2];
	/* 0x16cc */ u16 EVOLL[2];
	/* 0x16d0 */ u16 EVOLLbak[2];
	/* 0x16d4 */ u16 EVOLR[2];
	/* 0x16d8 */ u16 EVOLRbak[2];
} SNDDATA;

typedef struct { // 0x1c
	/* 0x00 */ int BankNum;
	/* 0x04 */ int hdAddr[2];
	/* 0x0c */ int bdAddr[2];
	/* 0x14 */ int transAddr;
	/* 0x18 */ int transSize;
} JAMDATA;

typedef struct { // 0x10
	/* 0x0 */ u32 Creator;
	/* 0x4 */ u32 Type;
	/* 0x8 */ u32 chunkSize;
	/* 0xc */ u16 reserved;
	/* 0xe */ u8 versionMajor;
	/* 0xf */ u8 versionMinor;
} CHK_VER;

typedef struct { // 0x40
	/* 0x00 */ u32 Creator;
	/* 0x04 */ u32 Type;
	/* 0x08 */ u32 chunkSize;
	/* 0x0c */ u32 headerSize;
	/* 0x10 */ u32 bodySize;
	/* 0x14 */ u32 programChunkAddr;
	/* 0x18 */ u32 samplesetChunkAddr;
	/* 0x1c */ u32 sampleChunkAddr;
	/* 0x20 */ u32 vagInfoChunkAddr;
	/* 0x24 */ u8 reserved[28];
} CHK_HEAD;

typedef struct { // 0x14
	/* 0x00 */ u32 Creator;
	/* 0x04 */ u32 Type;
	/* 0x08 */ u32 chunkSize;
	/* 0x0c */ u32 maxProgramNumber;
	/* 0x10 */ u32 pointer;
} CHK_PROG;

typedef struct { // 0x14
	/* 0x00 */ u32 Creator;
	/* 0x04 */ u32 Type;
	/* 0x08 */ u32 chunkSize;
	/* 0x0c */ u32 maxSampleSetNumber;
	/* 0x10 */ u32 pointer;
} CHK_SSET;

typedef struct { // 0x14
	/* 0x00 */ u32 Creator;
	/* 0x04 */ u32 Type;
	/* 0x08 */ u32 chunkSize;
	/* 0x0c */ u32 maxSampleNumber;
	/* 0x10 */ u32 pointer;
} CHK_SMPL;

typedef struct { // 0x14
	/* 0x00 */ u32 Creator;
	/* 0x04 */ u32 Type;
	/* 0x08 */ u32 chunkSize;
	/* 0x0c */ u32 maxVagInfoNumber;
	/* 0x10 */ u32 pointer;
} CHK_VINFO;

typedef struct { // 0x28
	/* 0x00 */ u32 splitBlockAddr;
	/* 0x04 */ u8 nSplit;
	/* 0x05 */ u8 sizeSplitBlock;
	/* 0x06 */ u8 progVolume;
	/* 0x07 */ s8 progPanpot;
	/* 0x08 */ s8 progTranspose;
	/* 0x09 */ s8 progDetune;
	/* 0x0a */ s8 keyFollowPan;
	/* 0x0b */ u8 keyFollowPanCenter;
	/* 0x0c */ u8 progAttr;
	/* 0x0d */ u8 reserved;
	/* 0x0e */ u8 progLfoWave;
	/* 0x0f */ u8 progLfoWave2;
	/* 0x10 */ u8 progLfoStartPhase;
	/* 0x11 */ u8 progLfoStartPhase2;
	/* 0x12 */ u8 progLfoPhaseRandom;
	/* 0x13 */ u8 progLfoPhaseRandom2;
	/* 0x14 */ u16 progLfoCycle;
	/* 0x16 */ u16 progLfoCycle2;
	/* 0x18 */ s16 progLfoPitchDepth;
	/* 0x1a */ s16 progLfoPitchDepth2;
	/* 0x1c */ s16 progLfoMidiPitchDepth;
	/* 0x1e */ s16 progLfoMidiPitchDepth2;
	/* 0x20 */ s8 progLfoAmpDepth;
	/* 0x21 */ s8 progLfoAmpDepth2;
	/* 0x22 */ s8 progLfoMidiAmpDepth;
	/* 0x23 */ s8 progLfoMidiAmpDepth2;
	/* 0x24 */ u32 pointer;
} PROG_PRMBLK;

typedef struct { // 0x14
	/* 0x00 */ u16 sampleSetIndex;
	/* 0x02 */ u8 splitRangeLow;
	/* 0x03 */ u8 splitCrossFade;
	/* 0x04 */ u8 splitRangeHigh;
	/* 0x05 */ u8 splitNumber;
	/* 0x06 */ u16 splitBendRangeLow;
	/* 0x08 */ u16 splitBendRangeHigh;
	/* 0x0a */ s8 keyFollowPitch;
	/* 0x0b */ u8 keyFollowPitchCenter;
	/* 0x0c */ s8 keyFollowAmp;
	/* 0x0d */ u8 keyFollowAmpCenter;
	/* 0x0e */ s8 keyFollowPan;
	/* 0x0f */ u8 keyFollowPanCenter;
	/* 0x10 */ u8 splitVolume;
	/* 0x11 */ s8 splitPanpot;
	/* 0x12 */ s8 splitTranspose;
	/* 0x13 */ s8 splitDetune;
} SPLT_PRMBLK;

typedef struct { // 0x6
	/* 0x0 */ u8 velCurve;
	/* 0x1 */ u8 velLimitLow;
	/* 0x2 */ u8 velLimitHigh;
	/* 0x3 */ u8 nSample;
	/* 0x4 */ u16 pointer;
} SSET_PRMBLK;

typedef struct { // 0x2a
	/* 0x00 */ s16 VagIndex;
	/* 0x02 */ u8 velRangeLow;
	/* 0x03 */ u8 velCrossFade;
	/* 0x04 */ u8 velRangeHigh;
	/* 0x05 */ s8 velFollowPitch;
	/* 0x06 */ u8 velFollowPitchCenter;
	/* 0x07 */ u8 velFollowPitchVelCurve;
	/* 0x08 */ s8 velFollowAmp;
	/* 0x09 */ u8 velFollowAmpCenter;
	/* 0x0a */ u8 velFollowAmpVelCurve;
	/* 0x0b */ u8 sampleBaseNote;
	/* 0x0c */ s8 sampleDetune;
	/* 0x0d */ s8 samplePanpot;
	/* 0x0e */ u8 sampleGroup;
	/* 0x0f */ u8 samplePriority;
	/* 0x10 */ u8 sampleVolume;
	/* 0x11 */ s8 reserved;
	/* 0x12 */ u16 sampleAdsr1;
	/* 0x14 */ u16 sampleAdsr2;
	/* 0x16 */ s8 keyFollowAr;
	/* 0x17 */ u8 keyFollowArCenter;
	/* 0x18 */ s8 keyFollowDr;
	/* 0x19 */ u8 keyFollowDrCenter;
	/* 0x1a */ s8 keyFollowSr;
	/* 0x1b */ u8 keyFollowSrCenter;
	/* 0x1c */ s8 keyFollowRr;
	/* 0x1d */ u8 keyFollowRrCenter;
	/* 0x1e */ s8 keyFollowSl;
	/* 0x1f */ u8 keyFollowSlCenter;
	/* 0x20 */ u16 samplePitchLfoDelay;
	/* 0x22 */ u16 samplePitchLfoFade;
	/* 0x24 */ u16 sampleAmpLfoDelay;
	/* 0x26 */ u16 sampleAmpLfoFade;
	/* 0x28 */ u8 sampleLfoAttr;
	/* 0x29 */ u8 sampleSpuAttr;
} SMPL_PRMBLK;

typedef struct { // 0x8
	/* 0x0 */ u32 vagOffsetAddr;
	/* 0x4 */ u16 vagSampleRate;
	/* 0x6 */ u8 vagAttribute;
	/* 0x7 */ u8 reserved;
} VINFO_PRMBLK;

typedef struct { // 0x50
	/* 0x00 */ CHK_VER Version;
	/* 0x10 */ CHK_HEAD Header;
} JAMHD;

#define IOP_IopInit 0x08000000
#define IOP_RpcInfo 0x08000001
#define IOP_StrKick 0x10000003
#define IOP_StrInit 0x14000000
#define IOP_StrInfo 0x1b000001
#define IOP_JamBdTrans  0x20000004
#define IOP_JamBankSet  0x21000003
#define IOP_SndMask 0x22000002
#define IOP_SndInit 0x24000000
#define IOP_SndMain 0x2a000001

extern int RpcRet[16];
extern int RpcInfo[16];
extern IOP_MEM Mem;
extern SNDDATA *sD;
extern int MainThread();
extern void StrInit();
extern void StrInfo(int *data);
extern void StrKick();
extern void SndInit();
extern void SndMain(int *data);
extern void SndMask(int *data);
extern void JamBankSet(int id);
extern void JamBdTrans();
extern void JamGetPrm(int id, int prog, int splt, SNDKEYPRM *prm);
extern void JamInit();
extern void PptStop(int st);
extern void Ac3Play();
extern void Ac3Stop();
extern void Ac3ZeroPcmPlay();
extern void Ac3ZeroPcmStop();
extern void Ac3SetDigitalOut();
extern void Ac3Play2();
extern void Ac3Clear();

#ifndef SCE_OBSOLETE
extern int CpuDisableIntr();
extern int CpuEnableIntr();
extern int EnableIntr(int irq);
#endif

#endif // IOP_H