#ifndef SFXCALC_H
#define SFXCALC_H

#include "take.h"

#define VIF_UNPACK_V3_16 9
#define VIF_UNPACK_V4_8 14
#define DMArefe (0<<28)
#define DMAcnt  (1<<28)
#define DMAnext (2<<28)
#define DMAref  (3<<28)
#define DMArefs (4<<28)
#define DMAcall (5<<28)
#define DMAret  (6<<28)
#define DMAend  (7<<28)

extern void SfxFixCalc(SFXOBJ *pObj, PARTS *pParts, u128 *pVertexI, u128 *pColorI, u128 *pSpecUVI);
extern void SfxSkinCalc(SFXOBJ *pObj, PARTS *pParts, u128 *pVertexI, u128 *pColorI, u128 *pSpecUVI);
extern void SfxMimeCalc(SFXOBJ *pObj, PARTS *pParts, u128 *pVertexI, u128 *pColorI, u128 *pSpecUVI);

#endif
