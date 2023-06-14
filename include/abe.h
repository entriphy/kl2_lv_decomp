#ifndef ABE_H
#define ABE_H

#include "types.h"
#include "objwork.h"

// Structs

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

// ab_jigen_a.cc
extern void abJigenAnime(OBJWORK *objw, JIGEN_WORK *jgnw);
extern void abJigenDraw(OBJWORK *objw, JIGEN_WORK *jgnw);

// ab_menu.c
extern s32 abMenuInit();
extern s32 abMenuMain();

#ifdef __cplusplus
}
#endif

// Data

// ab_menu.c
extern s32 (*abMenuFuncTbl[2])();

#endif
