#include "common.h"

u32* Fadr(u32 *pAddr, s32 nNum) {
    return pAddr + (pAddr[nNum + 1] >> 2);
}
