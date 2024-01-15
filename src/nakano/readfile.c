#include <sifdev.h>
#include "nakano/readfile.h"

s32 read_file(char *name, char *buff) {
    s32 fd;
    s32 size;

    fd = sceOpen(name, SCE_RDONLY);
    if (fd < 0) {
        return -1;
    }

    size = sceLseek(fd, 0, SCE_SEEK_END);
    sceLseek(fd, 0, SCE_SEEK_SET);
    size = (size + 15) / 16 * 16;
    sceRead(fd, buff, size);
    sceClose(fd);
    return size;
}

// TODO: not matching
qword* read_file128(char *name, qword *buff) {
    s32 fd;
    s32 size;

    fd = sceOpen(name, SCE_RDONLY);
    if (fd < 0) {
        return (qword *)-1;
    }

    size = sceLseek(fd, 0, SCE_SEEK_END);
    sceLseek(fd, 0, SCE_SEEK_SET);
    size = (size + 15) / 16 * 16;
    sceRead(fd, buff, size);
    sceClose(fd);
    return (qword *)((u32)buff + size);
}