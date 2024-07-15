#include <sifdev.h>
#include "vtusr/vtutil.h"
#include "okanoyo/okio.h"

u128* vtFileRead(char *name) {
    s32 fd;
    s32 size;
    u128 *buf;
    char tmp;
    s32 status;

    fd = sceOpen(name, SCE_RDONLY);
    if (fd < 0) {
        printf("can't open [%s]\n", name);
        return NULL;
    }

    size = sceLseek(fd, 0, SEEK_END);
    sceLseek(fd, 0, SEEK_SET);
    buf = getBuff(1, size, &tmp, &status);
    if (sceRead(fd, buf, size) < 0) {
        printf("load failed [%s]\n", name);
        return NULL;
    } else {
        printf("load sucees %d byte\n", size); // sucees
        sceClose(fd);
        return buf;
    }
}

void* vtGetBuf64(u32 size) {
    char tmp;
    s32 status;
    void *buf;

    buf = getBuff(1, size + 0x3F, &tmp, &status);
    buf = (void *)(((u32)buf - 1) & ~0x3F);
    buf = (void *)((u8 *)buf + 0x40);
    return buf;
}