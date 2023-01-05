#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8;
typedef volatile u8 vu8;
typedef signed char s8;
typedef volatile s8 vs8;

typedef unsigned short u16;
typedef volatile u16 vu16;
typedef signed short s16;
typedef volatile s16 vs16;

typedef unsigned int u32;
typedef volatile u32 vu32;
typedef signed int s32;
typedef volatile s32 vs32;

typedef unsigned long long u64;
typedef volatile u64 vu64;
typedef signed long long s64;
typedef volatile s64 vs64;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} FVECTOR __attribute__((aligned(16)));

typedef struct {
	int	x;
	int	y;
	int	z;
	int	w;
} IVECTOR __attribute__((aligned(16)));

typedef struct {
	short	x;
	short	y;
	short	z;
	short	w;
} SVECTOR __attribute__((aligned(16)));

typedef struct {
	FVECTOR m[4];
} FMATRIX __attribute__((aligned(16)));

// Size: 0xF0
struct OBJWORK {
    u32 pers; // 0x00
    u32 draw; // 0x04
    u32 drmir; // 0x08
    u32 drmiref; // 0x0C
    u32 dreff; // 0x10
    u32 prim; // 0x14, klMODEL, 
    u32 work; // 0x18
    s16 stat0; // 0x1C
    s16 stat1; // 0x1E
    s16 prty; // 0x20
    s16 hitram; // 0x22
    s16 bun0; // 0x24
    s16 bun1; // 0x26
    s16 pad0; // 0x28
    s16 pad1; // 0x2A
    s16 live; // 0x2C
    s16 stat; // 0x2E
    FVECTOR posi; // 0x30
    FVECTOR spd; // 0x40
    FVECTOR muki; // 0x50
    FVECTOR ang; // 0x60
    FVECTOR rot; // 0x70
    FVECTOR rtw; // 0x80
    s32 reg0; // 0x90
    s32 reg1; // 0x94
    s32 reg2; // 0x98
    s32 reg3; // 0x9C
    s32 reg4; // 0xA0
    s32 reg5; // 0xA4
    s32 reg6; // 0xA8
    s32 reg7; // 0xAC
    float freg0; // 0xB0
    float freg1; // 0xB4
    float freg2; // 0xB8
    float freg3; // 0xBC
    float freg4; // 0xC0
    float freg5; // 0xC4
    float freg6; // 0xC8
    float freg7; // 0xCC
    s64 htblid; // 0xD0
    s64 dummy; // 0xD8
    s16 option; // 0xE0
    s16 sflag; // 0xE2
    u32 hpo; // 0xE4
    u32 gimmick; // 0xE8
    u32 _hOp; // 0xEC
};

typedef struct {
    FVECTOR Trans; // 0x00
    FVECTOR Rot; // 0x10
    FVECTOR Scale; // 0x20
    u32 pObj; // 0x30
    int ActNum; // 0x34
    int old_muki; // 0x38
    int wip_timer; // 0x3C
    int dummy[4]; // 0x40
} PsfxMODEL;

typedef struct {
    PsfxMODEL klm;
    PsfxMODEL ksm;
    PsfxMODEL kmm;
    PsfxMODEL shm;
    PsfxMODEL shm2;
} klMODEL;

typedef struct {
    FVECTOR trans;
    FVECTOR rot;
    FVECTOR light0;
    FVECTOR light1;
    FVECTOR light2;
    FVECTOR color0;
    FVECTOR color1;
    FVECTOR color2;
    FVECTOR ambient;
    FVECTOR fogcol;
    u8 draw_enable;
    u8 prim;
    u8 lod;
    u8 mmesh;
    u8 basetex;
    u8 fog;
    u8 multitex;
    u8 headtex;
    u8 headtexval;
    u8 heightajust;
    u8 basetexarea;
    u8 pad;
    float planesizex;
    float planesizez;
    float meshsizex;
    float meshsizez;
    float height;
    float intervalx;
    float intervalz;
    float radiusx;
    float radiusz;
    float texajust;
    float speedx;
    float speedz;
    float shear;
    float basetexspeedx;
    float basetexspeedz;
    float multitexspeedx;
    float multitexspeedz;
    float random;
    float ajust_h_far;
    float ajust_h_near;
    float ajust_h_val;
    float alphablend;
    u8 padding[12];
} kitWaveParam;

#endif