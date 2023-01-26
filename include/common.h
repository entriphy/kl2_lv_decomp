#ifndef COMMON_H
#define COMMON_H

#include "gbl.h"
#include "gim.h"
#include "hoshino.h"
#include "harada.h"
#include "objwork.h"
#include "ps2.h"
#include "tobj.h"
#include "types.h"

#include "structs.h"
#include "data_symbols.h"
#include "function_symbols.h"

void sce_print(const char* fmt, ...);
void* GetFHMAddress(void* pAddr, int nNum);

inline int roundSizeToSector(int size) {
	return size + 0x7FF & 0xFFFFF800;
}

#endif