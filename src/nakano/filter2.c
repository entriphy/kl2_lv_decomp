#include "nakano/filter2.h"
#include "nakano/gssub.h"
#include "nakano/main.h"

void nkDrawBlackIn(f32 pow) {
    nkGifPacket packet;
    s32 rate;

    packet.size = 0;
    packet.buf = (nkQWdata *)0x70000000;

    rate = (1.0f - pow) * 128.0f;
    if (rate > 255) {
        rate = 255;
    }
    rate &= 0xFF;

    nkGsSetNormalFZ2();
    packet.buf[packet.size].ul128 = 0;
    packet.size++;

    packet.buf[packet.size].ul64[0] = 0x1000000000000004;
    packet.buf[packet.size++].ul64[1] = SCE_GIF_PACKED_AD;
    packet.buf[packet.size].ul64[1] = SCE_GS_TEST_2;
    packet.buf[packet.size++].ul64[0] = 0x3001C;
    packet.buf[packet.size].ul64[1] = SCE_GS_CLAMP_2;
    packet.buf[packet.size++].ul64[0] = 5;
    packet.buf[packet.size].ul64[1] = SCE_GS_ALPHA_2;
    packet.buf[packet.size++].ul64[0] = 0x81;
    packet.buf[packet.size].ul64[1] = SCE_GS_ZBUF_2;
    packet.buf[packet.size++].ul64[0] = 0x13A00008C;
    packet.buf[packet.size].ul64[0] = 0x21A3400000008002;
    packet.buf[packet.size++].ul64[1] = SCE_GS_SCISSOR_2;

    packet.buf[packet.size].ul32[0] = 0;
    packet.buf[packet.size].ul32[1] = 0;
    packet.buf[packet.size].ul32[2] = 0;
    packet.buf[packet.size].ul32[3] = rate;
    packet.size++;
    packet.buf[packet.size].ul32[0] = 0x6C00;
    packet.buf[packet.size].ul32[1] = 0x7900;
    packet.buf[packet.size].ul32[2] = 0xFFFFF;
    packet.buf[packet.size].ul32[3] = 0x0000;
    packet.size++;

    packet.buf[packet.size].ul32[0] = 0;
    packet.buf[packet.size].ul32[1] = 0;
    packet.buf[packet.size].ul32[2] = 0;
    packet.buf[packet.size].ul32[3] = rate;
    packet.size++;
    packet.buf[packet.size].ul32[0] = 0x9400;
    packet.buf[packet.size].ul32[1] = 0x8700;
    packet.buf[packet.size].ul32[2] = 0xFFFFF;
    packet.buf[packet.size].ul32[3] = 0x0000;
    packet.size++;

    packet.buf[packet.size].ul64[0] = 0x1000000000008001;
    packet.buf[packet.size++].ul64[1] = SCE_GIF_PACKED_AD;
    packet.buf[packet.size].ul64[1] = 0x4F;
    packet.buf[packet.size++].ul64[0] = 0x3A00008C;

    packet.buf[0].ul32[0] = 0x70000000 | (packet.size - 1);

    sceDmaSend(DmaChGIF, (void *)((u32)packet.buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);
}