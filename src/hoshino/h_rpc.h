#ifndef H_RPC_H
#define H_RPC_H

#include "hoshino.h"

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

typedef struct {
    /* 0x00 */ s32 BGMnext;
    /* 0x04 */ s32 BGMpoint;
    /* 0x08 */ s32 PPTnext[4];
    /* 0x18 */ s32 AC3stat;
} hSTRINFO2;

typedef struct {
    u32 vStatKeyon[2];
    hSTRINFO2 STRinfo;
} hRPCINFO;

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

typedef struct { // 0x50
    /* 0x00 */ CHK_VER Version;
    /* 0x10 */ CHK_HEAD Header;
} JAMHD;

extern void hSndRpcRet();
extern s32  hRpcSync();
extern s32  hRpcInit();
extern s32  hRpc(s32 cmd);
extern s32  hTrans2IOP(s32 iopAddr, s32 eeAddr, s32 size);

#endif
