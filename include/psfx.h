#ifndef P_H
#define P_H

#include "types.h"
#include "ps2.h"

// Size: 0xB0
typedef struct PCOORD {
    FVECTOR Rot; // 0x00
    FVECTOR Trans; // 0x10
    FMATRIX Mtx; // 0x20
    FMATRIX MtxSav; // 0x60
    f32 WipCnt; // 0xA0
    struct PCOORD* Super; // 0xA4
    s32 Flag; // 0xA8
    float YokoSpd; // 0xAC
} PCOORD;

// Size: 0x18
typedef struct {
    u64 IdFlag; // 0x00
    u8* pVmime; // 0x08
    s16 VmimeInd; // 0x0C
    float VmimeCnt; // 0x10
    float VmimeCntSpeed; // 0x14
} PMIME;

typedef struct {
    s16 mot;
    s16 stopflag;
    s16 sync;
    s16 incnt;
    s16 outcnt;
    s16 inmode;
    s16 outmode;
    u64 onflag;
} PACTTBL;

// Size: 0x38?
typedef struct {
    f32 MotionCnt;
    f32 MotionEndCnt;
    f32 CntSpeed;
    f32 Weight;
    f32 TargetWeight;
    f32 InCnt;
    f32 OutCnt;
    u64 OnFlag;
    s16 StopFlag;
    s16 Type;
    s16 SyncFlag;
    s16 InMode;
    s16 OutMode;
} PMBLOCK;

typedef struct {
    s16 ActNum;
    s16 ActNumMax;
    f32 SubScale; // 0x04
    f32 unk;
    PCOORD* pBaseCoord; // 0x0C
    PCOORD* pCoord; // 0x10
    u8* pInf; // 0x14
    u32 pItr; // 0x18, TODO: this should be a pointer
    u32 pItrW; // 0x1C, TODO: this should be a pointer
    s16 CoordNum; // 0x20
    s16 BaseIndex; // 0x22
    s16 BaseMixIndex; // 0x24
    s16 IpIndex; // 0x26?
    s16 IpMixIndex; // 0x28?
    s16 spad;
    s32 pad[10];
    // float IpWeight[64]; // TODO: ????
    PACTTBL* pActtbl; // 0x54
    PMBLOCK Mb[4];
    s16 EndFlag;
} PMOTION;

typedef struct {
    s16 joints[4];
    u16 vertex_count;
    u16 normal_count;
    u16 field_0xC;
    u16 field_0xE;
    u32 vertices_offset;
    u32 vertex_weights_offset;
    u32 normals_offset;
    u32 normals_weights_offset;
} PSPARTS;

typedef struct {
    long reg;
    long id;
} PGSREG;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
} PVERTEX;

typedef struct {
    s32 u;
    s32 v;
} PUV;

typedef struct {
    u16 vertex;
    u16 uv;
    u16 normal;
    s16 flag;
} PINDEX;

// Size: 0x160
typedef struct {
    PGSREG regs[0x10];
    sceGsTex0 tex0;
    PSPARTS* subparts;
    u8* sfx_file;
    PVERTEX* vertices;
    PVERTEX* normals;
    s32 field_0x118;
    s32 field_0x11C;
    s32 field_0x120;
    s32 field_0x124;
    PUV* uvs;
    PINDEX* indices;
    s32 field_0x130;
    s32 field_0x134;
    s32 field_0x138;
    s32 field_0x13C;
    u16 enable;
    u16 subpart_count;
    u16 vertex_count;
    u16 normal_count;
    u16 uv_count;
    u16 tripstrip_count;
    u16 field_0x14C;
    u16 field_0x14E;
    s16 packet_index;
    s16 field_0x152;
    s16 field_0x154;
    s16 field_0x156;
    s32 field_0x158;
    s32 field_0x15C;
} PPARTS;

// Size: 0xA0
typedef struct PSFXOBJ {
    FVECTOR ScaleVector; // 0x00
    PMOTION* pMot; // 0x10
    FMATRIX* pLightColor; // 0x14
    FMATRIX* pNormalLight; // 0x18
    s32 PartsNum; // 0x1C
    u8* GmsNum; // 0x20
    u32 field_0x24;
    u32 field_0x28;
    u8* pSfx; // 0x2C
    u8* pAct; // 0x30
    f32 fpad2;
    u16 something;
    f32 scale; // 0x3C
    PPARTS* pParts;  // 0x40
    struct PSFXOBJ* pObjSub; // 0x44
    struct PSFXOBJ* pObjTop; // 0x48
    f32 ClutWeight; // 0x4C
    u32 Flag; // 0x50
    u32 Condition; // 0x54
    s16 Pause; // 0x58
    s16 GmsTransType; // 0x5A
    s32 partsMax;
    s32 ipad;
    PMIME* pMime; // 0x64
    s16 OutLineFlag; // 0x68
    f32 ClipOffset; // 0x6C
    f32 ClipZone; // 0x70
    s16 ClipFlag; // 0x74
    s16 Id; // 0x76
    u16 ClutNum0; // 0x78
    u16 ClutNum1; // 0x7A
    u16 Cbp; // 0x7C
    u16 NonActFlag; // 0x7E
    u16 MotionSyncFlag; // 0x80
    s32 ShadowType; // 0x84
    f32 ShadowSize; // 0x88
    f32 ShadowOffset; // 0x8C
    f32 ShadowRange; // 0x90
    s16 ObjNum;
    s16 DrawWorkCnt;
    s32 OutFlag;
    s32 actNum;
} PSFXOBJ;

#endif
