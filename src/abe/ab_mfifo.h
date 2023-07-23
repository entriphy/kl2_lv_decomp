#ifndef AB_MFIFO_H
#define AB_MFIFO_H

#include "abe.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void abMfifoInit();
extern void abMfifoSwapDBuffDc(sceGsDBuffDc *db, s32 id);
extern void abMfifoBegin();
extern void abMfifoEnd(void *tagw);
extern s32 abMfifoSync(s32 mode);
extern void abMfifoSend(void *sadr, u32 qwc);

#ifdef __cplusplus
}
#endif

#endif
