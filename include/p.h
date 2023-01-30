#ifndef P_H
#define P_H

#include "types.h"

typedef struct {} PPARTS;

// Size: 0xB0
typedef struct PCOORD {
    FVECTOR Rot; // 0x00
    FVECTOR Trans; // 0x10
    FMATRIX Mtx; // 0x20
    FMATRIX MtxSav; // 0x60
    float WipCnt; // 0xA0
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
    float MotionCnt;
    float MotionEndCnt;
    float CntSpeed;
    float Weight;
    float TargetWeight;
    float InCnt;
    float OutCnt;
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
    float SubScale; // 0x04
    float unk;
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

typedef struct PSFXOBJ {
    FVECTOR ScaleVector; // 0x00
    PMOTION* pMot; // 0x10
    FMATRIX* pLightColor;
    FMATRIX* pNormalLight;
    s32 PartsNum;
    s32 GmsNum; // 0x38, TODO: short or int?
    float scale;
    PPARTS* pParts;
    float pad[6];
    struct PSFXOBJ* pObjSub; // 0x44
    struct PSFXOBJ* pObjTop; // 0x48
    float ClutWeight; // 0x4C
    u32 Flag;
    u32 Condition;
    s16 Pause; // 0x58
    s16 GmsTransType;
    PMIME pMime; // 0x64
    s16 OutLineFlag;
    float ClipOffset;
    float ClipZone;
    s16 ClipFlag;
    s16 Id;
    u16 ClutNum0; // 0x78
    u16 ClutNum1; // 0x7A
    u16 Cbp; // 0x7C
    u16 NonActFlag; // 0x7E
    u16 MotionSyncFlag; // 0x80
    s32 ShadowType;
    float ShadowSize;
    float ShadowOffset;
    float ShadowRange;
    u16 ObjNum;
    u16 DrawWorkCnt;
    s32 OutFlag;
    s32 actNum;
} PSFXOBJ;

#endif
