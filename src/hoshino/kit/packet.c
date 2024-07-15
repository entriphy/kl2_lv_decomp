#include "hoshino/kit/packet.h"

void kitPacketKickPath3(u32 ptr, u32 size) {
    if (size == 0) {
        return;
    }

    if (ptr & 0x40000000 && ptr & 0x10000000 && ptr & 0x20000000) {
        ptr = ptr & 0xFFFFFFF | 0x80000000;
    } else if (ptr & 0x20000000) {
        ptr &= 0xFFFFFFF;
    }

    while (DGET_D2_CHCR() & 0x100);
    DPUT_D2_MADR(ptr);
    DPUT_D2_QWC(size);
    DPUT_D2_CHCR(0x100);
}
