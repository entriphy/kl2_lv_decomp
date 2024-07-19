#ifndef SFXDRAW_H
#define SFXDRAW_H

#include "take.h"

extern void SfxDrawOutLine(SFXOBJ *pObj);
extern void DmaSend_Mac(sceDmaChan *DmaChan, u128 *Addr);
extern void DmaSync_Mac(sceDmaChan *DmaChan);

#endif