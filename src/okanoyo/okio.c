#include "common.h"

s32 testcbp = 0;
s32 memtest = 0;
s32 memblocksize[16] = {};

s32 OkCheck_file(char *name) {
    s32 fd;
    s32 size;

    if ((fd = sceOpen(name, SCE_RDONLY)) < 0 || (size = sceLseek(fd, 0, SCE_SEEK_END)) <= 0) {
        sceClose(fd);
        return -1;
    } else {
        sceClose(fd);
        return size;
    }
}

s32 OkRead_file(char *name, char *buff) {
    s32 fd;
    s32 size;

    if ((fd = sceOpen(name, SCE_RDONLY)) < 0 || (size = sceLseek(fd, 0, SCE_SEEK_END)) <= 0) {
        sceClose(fd);
        return -1;
    } else {
        size = ((size + 0xF) / 0x10) * 0x10;
        sceLseek(fd, 0, SCE_SEEK_SET);
        sceRead(fd, buff, size);
        sceClose(fd);
        return size;
    }
}

void memorySysFormat() {
    debmemorySysFormat();
    buffstartptrORG = (s32 *)0x00A00000;
    buffstartptr = (s32 *)0x00A00000;
    memtest = 0;
    memoryStageptrSet();
}

void memoryStageptrSet() {
    buffstagetop = buffstartptr;
}

void memoryStageFormat() {
    buffstartptr = buffstagetop;
    memtest = 0;
}

void memoryAreaptrSet() {
    buffareatop = buffstartptr;
}

void memoryAreaFormat() {
    s32 *ptr;
	s32 i;
	s32 j;
	s32 ofs[4] = { 0x40, 0x100, 0x400, 0x800 };
	s32 loop[4] = { 0x1000, 0x400, 0x80, 0x40 };

    buffstartptr = buffareatop;
    ptr = (s32 *)getBuff(1, 0x300000, NULL, &i);
    for (i = 0; i < 4; i++) {
        if (prgBufPtrIdxMax[i] != 0)
            prgBufPtrIdxMax[i] = 0;
        prgBufPtr[i] = prgBufTop[i] = (s32 *)ptr;

        for (j = 0; j < loop[i]; j++) {
            *ptr = (s32)(ptr + ofs[i]);
            ptr += ofs[i];
        }
        *ptr = 0;
    }
}

void OkMemDisp() {
    s32 i;
    s32 ofs[4] = { 0x40, 0x100, 0x400, 0x800 };
    s32 loop[4] = { 0x1000, 0x400, 0x80, 0x40 };

    for (i = 0; i < 4; i++) {
        // lol
    }
}

s32 * getmemblksize() {
    memblocksize[0] = 0x004;
    memblocksize[1] = 0x040;
    memblocksize[2] = 0x100;
    memblocksize[3] = 0x400;
    memblocksize[4] = 0x800;
    return memblocksize;
}

void * getmem(s32 byte) {
    s32 *ptr;
	s32 type;
	s32 i;
	s32 cnt;

    s32 ofs[4] = { 0x40, 0x100, 0x400, 0x800 };
    type = -1;
    for (i = 0; i < 4; i++) {
        if (byte / 4 <= ofs[i]) {
            type = i;
            break;
        }
    }

    if (type == -1 || type == 4) {
        return (void *)-1;
    } else {
        ptr = prgBufPtr[type];
        if (*ptr != 0) {
            prgBufPtr[type] = (s32 *)*ptr;
            {
                s32 ofs[4] = { 0x40, 0x100, 0x400, 0x800 };
                cnt = (((s32)prgBufPtr[type] - (s32)prgBufTop[type]) / 4) / ofs[type];
                if (cnt > prgBufPtrIdxMax[type])
                    prgBufPtrIdxMax[type] = cnt;
            }
            return ptr;
        }
        return (void *)-1;
    }
}

s32 freemem(void *ptr) {
	s32 i;

    for (i = 3; i >= 0; i--) {
        if ((s32)prgBufTop[i] <= (s32)ptr) {
            *(s32 *)ptr = (s32)prgBufPtr[i];
            prgBufPtr[i] = (s32 *)ptr;
            if (memtest == 0)
                memtest = (int)ptr;
            return 1;
        }
    }
    return -1;
}

void * getDebmem(s32 ID, s32 byte) {
	s32 *ptr;
	u8 *top;

    ptr = (s32 *)OkDebMem[ID].top;
    top = (u8 *)ptr + ((byte + 0xF) / 0x10) * 0x10;
    OkDebMem[ID].top = (s32)top;
    return ptr;
}

void DebmemFormat(s32 ID) {
    u32 tops[] = {
        0x0000000, 0x2000000, 0x2100000, 0x2300000,
        0x2400000, 0x2800000, 0x2B00000, 0x2F00000,
        0x5700000
    };

    u32 ends[] = {
        0x0000000, 0x20FFFFF, 0x22FFFFF, 0x23FFFFF,
        0x27FFFFF, 0x2AFFFFF, 0x2EFFFFF, 0x56FFFFF,
        0x57FFFFF
    };

    OkDebMem[ID].top = tops[ID];
    OkDebMem[ID].end = ends[ID];
}

void debmemorySysFormat() {
    s32 i;
    for (i = 0; i < 9; i++) {
        DebmemFormat(i);
    }
}

void * getBuff(s32 type, s32 byte, char *name, s32 *ret) {
    u8 *ptr = (u8 *)buffstartptr;
    u8 *org = ptr;
    
    if (type == 0) {
        byte = OkCheck_file(name);
        *ret = -1;
        if (byte == -1)
            return (u8 *)-1;
        *ret = OkRead_file(name, ptr);
    }
    
    byte = ((byte + 0x0F) / 0x10) * 0x10;
    ptr += byte;
    buffstartptr = (s32 *)ptr;
    return org;
}

void freeBuff(s32 type, s32 byte, char *name) {
    u8 *buff = (u8 *)buffstartptr;
    
    if (type == 0) {
        byte = OkCheck_file(name);
        if (byte == -1)
            return;
    }

    byte = ((byte + 0xF) / 0x10) * 0x10;
    buff -= byte;
    
    buffstartptr = (s32 *)buff;
}
