#ifndef AB_JIGEN_A_H
#define AB_JIGEN_A_H

#include "abe.h"

typedef struct {
    s16 bun0;
    s16 m_clip;
    s16 m_prev_scale;
    f32 m_count;
    OBJWORK *m_particle;
} JIGEN_WORK;

#ifdef __cplusplus
extern "C" {
#endif

extern void abJigenAnime(OBJWORK *objw, JIGEN_WORK *jgnw);
extern void abJigenDraw(OBJWORK *objw, JIGEN_WORK *jgnw);

#ifdef __cplusplus
}
#endif

#endif
