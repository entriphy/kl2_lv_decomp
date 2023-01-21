#include <kernel.h>
#include <stdio.h>
#include <sio.h>

void sce_print(const char* fmt, ...) {
	char buf[128];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, 128, fmt, ap);
	va_end(ap);
	sio_puts(buf);
}

void* GetFHMAddress(void* pAddr, int nNum) {
	return (void*)((char*)pAddr + *(int*)((char*)pAddr + 4 + nNum * 4));
}
