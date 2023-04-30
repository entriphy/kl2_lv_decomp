#include "types.h"

u8 *GetFHMAddress(u8 *pAddr, int nNum) {
	return (u8*)(pAddr + *(int*)(pAddr + 4 + nNum * 4));
}
