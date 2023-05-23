#ifndef ABE_H
#define ABE_H

#include "types.h"
#include "objwork.h"

typedef struct {
    s16 bun0;
    s16 m_clip;
    s16 m_prev_scale;
    f32 m_count;
    OBJWORK *m_particle;
} JIGEN_WORK;

#ifdef __cplusplus

#include "types.h"

class CTexManager {
    protected:
        s32 m_texID[672];
    
    public:
        static void Initialize();
        static CTexManager* Instance();
};

#endif

#endif
