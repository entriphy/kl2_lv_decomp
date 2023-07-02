#include "common.h"

DMAPTR pDma = {};
SCRENV Scr = {};
LIGHT3 Light3 = {};
sceVu0FMATRIX SfxLsMtx[64] = {};
sceVu0FMATRIX SfxLvMtx[64] = {};
sceVu0FMATRIX SfxLvSpecMtx[64] = {};
sceVu0FMATRIX SfxLcLightMtx[64] = {};
sceVu0FMATRIX SfxSkinMtx[64] = {};
sceVu0IVECTOR SfxVertexI[2048] = {};
sceVu0IVECTOR SfxColorI[2048] = {};
sceVu0IVECTOR SfxSpecUVI[2048] = {};
s32 SfxAllPause = 0;