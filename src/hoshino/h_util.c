#include "common.h"

void dump8(u8 *p) {
    s32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 16; j++) {
            if (j == 7)
                printf("%02x-", *p++);
            else
                printf("%02x ", *p++);
        }
    }
}

void dump64(u64 *p, char *str) {
    s32 i;
    s32 j;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 2; j++) {

        }
    }
}

u32 GetFHMNum(u32 *pAddr) {
    return *pAddr;
}

u32* GetFHMAddress(u32 *pAddr, u32 nNum) {
    return pAddr + (pAddr[nNum + 1] >> 2);
}

s32 hFileSize(char *filename) {
    s32 fd;
    s32 size;

    fd = sceOpen(filename, SCE_RDONLY);
    if (fd < 0) {
        sceClose(fd);
        return -1;
    }

    size = sceLseek(fd, 0, SCE_SEEK_END);
    sceLseek(fd, 0, SCE_SEEK_SET);
    sceClose(fd);
    return size;
}

s32 hFileLoad(char *filename,  s32 buff) {
    s32 fd;
    s32 size;
    s32 p;

    fd = sceOpen(filename, SCE_RDONLY);
    if (fd < 0) {
        sceClose(fd);
        return -1;
    }

    size = sceLseek(fd, 0, SCE_SEEK_END);
    sceLseek(fd, 0, SCE_SEEK_SET);
    if (buff == 0) {
        p = (s32)malloc(size);
        if (p == 0) {
            sceClose(fd);
            return -2;
        }
    } else {
        p = buff;
    }

    size = sceRead(fd, (void *)p, size);
    if (size < 0) {
        sceClose(fd);
        if (buff == 0)
            free((void *)p);
        return -3;
    }
    sceClose(fd);
    return p;
}

s32 hFileRead(char *filename, s32 buff, s32 *size) {
    s32 fd;
    s32 p;

    fd = sceOpen(filename, SCE_RDONLY);
    if (fd < 0) {
        sceClose(fd);
        return -1;
    }

    *size = sceLseek(fd, 0, SCE_SEEK_END);
    sceLseek(fd, 0, SCE_SEEK_SET);
    if (buff == 0) {
        p = (s32)malloc(*size);
        if (p == 0) {
            sceClose(fd);
            return -2;
        }
    } else {
        p = buff;
    }

    *size = sceRead(fd, (void *)p, *size);
    if (*size < 0) {
        sceClose(fd);
        if (buff == 0)
            free((void *)p);
        return -3;
    }
    sceClose(fd);
    return p;
}

