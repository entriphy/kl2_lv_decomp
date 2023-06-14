#ifndef ABE_H
#define ABE_H

#include "types.h"
#include "objwork.h"

// Structs

typedef union { // 0x10
	/* 0x0 */ u128 ul128;
	/* 0x0 */ u64  ul64[2];
	/* 0x0 */ u32  ul32[4];
	/* 0x0 */ s32  vect[4];
} abQWdata;

typedef struct { // 0xc
	/* 0x0 */ abQWdata *buf;
	/* 0x4 */ u32 size;
	/* 0x8 */ u32 *pBase;
} abGifPacket;

typedef struct {
    s16 bun0;
    s16 m_clip;
    s16 m_prev_scale;
    f32 m_count;
    OBJWORK *m_particle;
} JIGEN_WORK;

// Classes

#ifdef __cplusplus

class CTexManager {
    protected:
        s32 m_texID[672];
    
    public:
        static void Initialize();
        static CTexManager* Instance();
};

#endif // __cplusplus

// C Functions

#ifdef __cplusplus
extern "C" {
#endif

// ab_access.c
extern OBJWORK * abGetKlonoa(s32 player);
extern OBJWORK * abGetMochifuku(s32 player);

// ab_dummy.c
extern void abObjDummy(OBJWORK *objw);
extern void abSetDummy(sceVu0FVECTOR vf);
extern void abInitDummy(u8 *buffer);

// ab_func.c
extern s32 abInit();
extern s32 abMain();

// ab_init.cc
extern void abGimInit();
extern void abStageInitA();

// ab_jigen_a.cc
extern void abJigenAnime(OBJWORK *objw, JIGEN_WORK *jgnw);
extern void abJigenDraw(OBJWORK *objw, JIGEN_WORK *jgnw);

// ab_menu.c
extern s32 abMenuInit();
extern s32 abMenuMain();

// ab_objtest.c
extern void abObjTest(OBJWORK *objw);
extern void abSetObjTest(sceVu0FVECTOR vf);

#ifdef __cplusplus
}
#endif

// Data

// ab_func.c
extern s32 (*abFuncTbl[2])();

// ab_menu.c
extern s32 (*abMenuFuncTbl[2])();

// ab_util.cc
extern s32 MfifoOn;

// firetest.c
extern s32 (*abFiretestFuncTbl[2])();

// sample.c
extern s32 (*abSampleFuncTbl[2])();

#endif

// Defines
#define abSPR (abQWdata *)0x70000000
