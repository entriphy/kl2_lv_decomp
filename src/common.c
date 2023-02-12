#include <stdio.h>

#ifndef SCE
#include <kernel.h>
#include <sio.h>
#endif

#ifndef SCE
void sce_print(const char* fmt, ...) {
	char buf[128];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, 128, fmt, ap);
	va_end(ap);
	sio_puts(buf);
}
#endif

void* GetFHMAddress(void* pAddr, int nNum) {
	return (void*)((char*)pAddr + *(int*)((char*)pAddr + 4 + nNum * 4));
}

inline int roundSizeToSector(int size) {
	return size + 0x7FF & 0xFFFFF800;
}