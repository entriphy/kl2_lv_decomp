#include "ab_mfifo.h"

/*
 * In order to get this file to match, D_CTRL and Dn_CHCR must be changed to a non-volatile s32 pointer.
 * Why the devs didn't just use whatever is in the SDK headers, I have no idea.
 */

#define	MFIFO_SIZE 0x8000
static char mfifo_base[MFIFO_SIZE];

extern "C" {

void abMfifoInit() {
    DPUT_D_RBOR((u32)mfifo_base);
    DPUT_D_RBSR(MFIFO_SIZE - 0x10);
    DPUT_D8_MADR((u32)mfifo_base);
    DPUT_D2_TADR((u32)mfifo_base);
}

void abMfifoSwapDBuffDc(sceGsDBuffDc *db, s32 id) {
    sceGsSyncPath(0, 0);
    DPUT_D_CTRL(DGET_D_CTRL() & ~D_CTRL_MFD_M);
    sceGsSwapDBuffDc(db, id);
    while (DGET_D2_CHCR() & D_CHCR_STR_M);
    DPUT_D_CTRL(DGET_D_CTRL() | D_CTRL_MFD_M);
    DPUT_D8_MADR((u32)mfifo_base);
    DPUT_D2_TADR((u32)mfifo_base);
    DPUT_D2_CHCR(0x104);
}

void abMfifoBegin() {
    while (DGET_D2_CHCR() & D_CHCR_STR_M);
    DPUT_D_CTRL(DGET_D_CTRL() | D_CTRL_MFD_M);
    DPUT_D8_MADR((u32)mfifo_base);
    DPUT_D2_TADR((u32)mfifo_base);
    DPUT_D2_CHCR(0x104);
}

void abMfifoEnd(/* v0 2 */ void *tagw) {
    *(u128 *)tagw = 0;
    *(u32 *)tagw = 0x70000000;
    abMfifoSend(tagw, 1);
    while (DGET_D2_CHCR() & D_CHCR_STR_M);
    DPUT_D_CTRL(DGET_D_CTRL() & ~D_CTRL_MFD_M);
}

s32 abMfifoSync(s32 mode) {
    if (mode != 0) {
        return DGET_D8_CHCR() & D_CHCR_STR_M;
    } else {
        while (DGET_D8_CHCR() & D_CHCR_STR_M);
        return 0;
    }
}

void abMfifoSend(void *sadr, u32 qwc) {
    static u32 tadr;
    static u32 madr;
    static u32 remain;
    static s32 debug = 0;

    debug = 0;
    DPUT_D8_SADR((u32)sadr & 0x3FF0);
    DPUT_D8_QWC(qwc);
    qwc <<= 4;
    do {
        tadr = DGET_D2_TADR();
        madr = DGET_D8_MADR();
        remain = tadr != madr ? (tadr - madr & 0x7FF0) : 0x8000;
    } while (remain <= qwc);
    DPUT_D8_CHCR(DGET_D8_CHCR() | 0x100);
}

}
