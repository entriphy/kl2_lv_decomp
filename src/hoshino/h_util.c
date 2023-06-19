#include "common.h"

u32 GetFHMNum(u32 *pAddr) {
    return *pAddr;
}

u32* GetFHMAddress(u32 *pAddr, u32 nNum) {
    return pAddr + (pAddr[nNum + 1] >> 2);
}
