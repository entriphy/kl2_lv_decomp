#ifndef NK_DMA_H
#define NK_DMA_H

#include "nakano.h"

extern void jDmaPauseOn();
extern void jDmaPauseOff();
extern void nkInitDma();
extern void nk_PrintOT(qword *tag);
extern void* nkAddRef(void *p1);
extern qword* nkMakeUnpackVif1(qword **pbp, void *data, s32 vu_addr, s32 qwc);
extern void nkDmaSendGif(qword *addr);
extern void nkLoadGms(qword *gms);
extern void nkClearOT(qword *tagp, s32 otn, s32 pce);
extern void nkPathReset();
extern void nkP1Flush(qword *otp);
extern void nkDebFlush();
extern void nkDmaSendVif1(qword *addr);
extern void nkDmaSendVif0(qword *addr);
extern void nkPathFlush();
extern void nkPathBlock();

#endif
