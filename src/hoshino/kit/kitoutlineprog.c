#include "hoshino/kit/kitoutlineprog.h"

kitInitEffectBufAllPacket* kitInitEffectBufAll(kitInitEffectBufAllPacket *packet) {
    kitADDR_DATA *draw_packet = (kitADDR_DATA *)packet;
    s32 i;
    s32 j;

    draw_packet->data = SCE_GIF_SET_TAG(3, 0, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 1, 0), 0, 1);
    draw_packet->addr = SCE_GIF_PACKED_AD;
    draw_packet++;
    draw_packet->addr = SCE_GS_TEST_2;
    draw_packet->data = SCE_GS_SET_TEST_2(0, 0, 0, 0, 0, 0, 1, 1);
    draw_packet++;
    draw_packet->addr = SCE_GS_RGBAQ;
    draw_packet->data = SCE_GS_SET_RGBAQ(0, 0, 0, 0, 0);
    draw_packet++;
    draw_packet->addr = SCE_GS_FBA_2;
    draw_packet->data = SCE_GS_SET_FBA_2(0);
    draw_packet++;
    draw_packet->data = SCE_GIF_SET_TAG(140, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 1, 0), 1, 1);
    draw_packet->addr = SCE_GS_XYZ2;
    draw_packet++;

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 10; draw_packet++) {
            draw_packet->data = SCE_GS_SET_XYZ2(0x6C00 + j * 0x400, 0x7900 + i * 0x200, 0);
            j++;
            draw_packet->addr = SCE_GS_SET_XYZ2(0x6C00 + j * 0x400, 0x7900 + (i + 1) * 0x200, 0);
        }
    }

    return (kitInitEffectBufAllPacket *)draw_packet;
}

kitInitAlphaPlanePacket* kitInitAlphaPlane(kitInitAlphaPlanePacket *packet, kitOutlineDrawEnv *env) {
    packet->giftag1.data = SCE_GIF_SET_TAG(8, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 1, 0), SCE_GIF_PACKED, 1);
    packet->giftag1.addr = SCE_GIF_PACKED_AD;
    packet->test1.addr = SCE_GS_TEST_2;
    packet->test1.data = SCE_GS_SET_TEST_2(0, 0, 0, 0, 0, 0, 1, SCE_GS_ZALWAYS);
    packet->frame1.addr = SCE_GS_FRAME_2;
    packet->frame1.data = SCE_GS_SET_FRAME_1(env->fr_page, 10, SCE_GS_PSMCT32, 0xFFFFFF);
    packet->zbuf1.addr = SCE_GS_ZBUF_2;
    packet->zbuf1.data = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMCT16S, SCE_GS_ZALWAYS);
    packet->alpha1.addr = SCE_GS_RGBAQ;
    packet->alpha1.data = SCE_GS_SET_RGBAQ(0, 0, 0, 128, 0);
    packet->sprite[0].addr = SCE_GS_XYZ3;
    packet->sprite[0].data = SCE_GS_SET_XYZ3((s32)env->fr_page + 32, (s32)env->fr_block + 32, 0);
    packet->sprite[1].addr = SCE_GS_XYZ2;
    packet->sprite[1].data = SCE_GS_SET_XYZ2(((s32 *)&env->spritez)[0] - 32, ((s32 *)&env->spritez)[1] - 32, 0);
    packet->frame2.addr = SCE_GS_FRAME_2;
    packet->frame2.data = SCE_GS_SET_FRAME_2(env->fr_page, 10, SCE_GS_PSMCT32, 0);
    packet->zbuf2.addr = SCE_GS_ZBUF_2;
    packet->zbuf2.data = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMCT16S, SCE_GS_ZNOUSE);
    packet->test2.addr = SCE_GS_TEST_2;
    packet->test2.data = SCE_GS_SET_TEST_2(0, 0, 0, 0, 0, 0, 1, SCE_GS_ZGREATER);
    packet++;
    return packet;
}

kitEffectBufEnablePacket* kitEffectBufEnable(kitEffectBufEnablePacket *buf, kitOutlineDrawEnv *env, u64 fmask, u8 zmask) {
    buf->giftag.data = SCE_GIF_SET_TAG(3, 1, 0, 0, 0, 1);
    buf->giftag.addr = SCE_GIF_PACKED_AD;
    buf->frame.addr = SCE_GS_FRAME_2;
    buf->frame.data = SCE_GS_SET_FRAME_2(0xB4, 10, SCE_GS_PSMCT32, fmask);
    buf->zbuf.addr = SCE_GS_ZBUF_2;
    buf->zbuf.data = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMCT16S, zmask);
    buf->xyoffset.addr = SCE_GS_XYOFFSET_2;
    buf->xyoffset.data = env->xyoffset;
    buf++;
    return buf;
}

kitEffectBufEnablePacket* kitEffectBufEnableNXY(kitEffectBufEnablePacket *buf, kitOutlineDrawEnv *env, u64 fmask, u8 zmask) {
    buf->giftag.data = SCE_GIF_SET_TAG(3, 1, 0, 0, 0, 1);
    buf->giftag.addr = SCE_GIF_PACKED_AD;
    buf->frame.addr = SCE_GS_FRAME_2;
    buf->frame.data = SCE_GS_SET_FRAME_2(0xB4, 10, SCE_GS_PSMCT32, fmask);
    buf->zbuf.addr = SCE_GS_ZBUF_2;
    buf->zbuf.data = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMCT16S, zmask);
    buf->xyoffset.addr = SCE_GS_XYOFFSET_2;
    buf->xyoffset.data = SCE_GS_SET_XYOFFSET_2(0x6C00, 0x7900);
    buf++;
    return buf;
}

kitEffectBufDisablePacket* kitEffectBufDisable(kitEffectBufDisablePacket *packet, kitOutlineDrawEnv *env, u64 fmask, u8 zmask) {
    packet->giftag1.data = SCE_GIF_SET_TAG(3, 1, 0, 0, 0, 1);
    packet->giftag1.addr = SCE_GIF_PACKED_AD;
    packet->frame.addr = SCE_GS_FRAME_2;
    packet->frame.data = SCE_GS_SET_FRAME_2(env->fr_page, 10, SCE_GS_PSMCT32, fmask);
    packet->zbuf.addr = SCE_GS_ZBUF_2;
    packet->zbuf.data = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMCT16S, zmask);
    packet->xyoffset.addr = SCE_GS_XYOFFSET_2;
    packet->xyoffset.data = env->xyoffset;
    packet++;
    return packet;
}

kitEffectBufDisablePacket* kitEffectBufDisableNXY(kitEffectBufDisablePacket *packet, kitOutlineDrawEnv *env, u64 fmask, u8 zmask) {
    packet->giftag1.data = SCE_GIF_SET_TAG(3, 1, 0, 0, 0, 1);
    packet->giftag1.addr = SCE_GIF_PACKED_AD;
    packet->frame.addr = SCE_GS_FRAME_2;
    packet->frame.data = SCE_GS_SET_FRAME_2(env->fr_page, 10, SCE_GS_PSMCT32, fmask);
    packet->zbuf.addr = SCE_GS_ZBUF_2;
    packet->zbuf.data = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMCT16S, zmask);
    packet->xyoffset.addr = SCE_GS_XYOFFSET_2;
    packet->xyoffset.data = SCE_GS_SET_XYOFFSET_2(0x6C00, 0x7900);
    packet++;
    return packet;
}

kitInitEffectBufBPacket* kitInitEffectBufB(kitInitEffectBufBPacket *packet, kitOutlineDrawEnv *env) {
    packet->giftag1.data = SCE_GIF_SET_TAG(7, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 1, 0), 0, 1);
    packet->giftag1.addr = SCE_GIF_PACKED_AD;
    packet->test1.addr = SCE_GS_TEST_2;
    packet->test1.data = SCE_GS_SET_TEST_2(0, 0, 0, 0, 0, 0, 1, 1);
    packet->zubf1.addr = SCE_GS_ZBUF_2;
    packet->zubf1.data = SCE_GS_SET_ZBUF_1(0x8C, SCE_GS_PSMCT16S, 0x1);
    packet->alpha1.addr = SCE_GS_RGBAQ;
    packet->alpha1.data = SCE_GS_SET_RGBAQ(0, 0, 0, 0, 0);
    packet->sprite[0].addr = SCE_GS_XYZ3;
    packet->sprite[0].data = SCE_GS_SET_XYZ3(env->bbox[0][0] + 32, env->bbox[0][1] + 32, 0);
    packet->sprite[1].addr = SCE_GS_XYZ2;
    packet->sprite[1].data = SCE_GS_SET_XYZ2(env->bbox[1][0] - 32, env->bbox[1][1] - 32, 0);
    packet->zbuf2.addr = SCE_GS_ZBUF_2;
    packet->zbuf2.data = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMCT16S, 0);
    packet->test2.addr = SCE_GS_TEST_2;
    packet->test2.data = SCE_GS_SET_TEST_2(0, 0, 0, 0, 0, 0, 1, 3);
    packet++;
    return packet;
}

kitDrawSpritePacket* kitDrawSprite(kitDrawSpritePacket *packet, kitOutlineDrawEnv *env, u64 test, u8 alpha_switch, u8 tex_func) {
    packet->giftag1.data = SCE_GIF_SET_TAG(4, 0, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, alpha_switch, 0, 1, 1, 0), 0, 1);
    packet->giftag1.addr = SCE_GIF_PACKED_AD;
    packet->tex0.addr = SCE_GS_TEX0_2;
    packet->tex0.data = SCE_GS_SET_TEX0(0x1680, 10, 0, 10, 8, 1, tex_func, 0x0000, 0, 0, 0, 0);
    packet->texflush.addr = SCE_GS_TEXFLUSH;
    packet->texflush.data = 0;
    packet->test1.addr = SCE_GS_TEST_2;
    packet->test1.data = test;
    packet->rgba.addr = SCE_GS_RGBAQ;
    packet->rgba.data = SCE_GS_SET_RGBAQ(0, 0, 0, 0x80, 0);

    packet->giftag2.data = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, alpha_switch, 0, 1, 1, 0), 0, 2);
    packet->giftag2.addr = SCE_GS_UV | SCE_GS_XYZF2 << 4;
    packet->tri[0].data = *(u64 *)env->bbox[2];
    packet->tri[0].addr = 0;
    packet->tri[1].data = *(u64 *)env->bbox[0];
    packet->tri[1].addr = env->spritez;
    packet->tri[2].data = *(u64 *)env->bbox[3];
    packet->tri[2].addr = 0;
    packet->tri[3].data = *(u64 *)env->bbox[1];
    packet->tri[3].addr = env->spritez;

    packet++;
    return packet;
}

kitDrawSpriteRegSetPacket* kitDrawSpriteRegSet(kitDrawSpriteRegSetPacket *packet, kitOutlineDrawEnv *env) {
    packet->giftag1.data = SCE_GIF_SET_TAG(5, 1, 0, 0, 0, 1);
    packet->giftag1.addr = SCE_GIF_PACKED_AD;
    packet->clamp1.addr = SCE_GS_CLAMP_2;
    packet->clamp1.data = SCE_GS_SET_CLAMP_1(1, 1, 0, 0, 0, 0);
    packet->tex01.addr = SCE_GS_TEX1_2;
    packet->tex01.data = SCE_GS_SET_TEX1_2(0, 0, 1, 1, 0, 0, 0);
    packet->frame.addr = SCE_GS_FRAME_1;
    packet->frame.data = SCE_GS_SET_FRAME_1(env->fr_page, 10, SCE_GS_PSMCT32, 0xFF000000);
    packet->fba.addr = SCE_GS_FBA_2;
    packet->fba.data = SCE_GS_SET_FBA(0);
    packet->alpha1.addr = SCE_GS_ALPHA_2;
    packet->alpha1.data = SCE_GS_SET_ALPHA_2(0, 1, 2, 1, env->alpha);
    packet++;
    return packet;
}

u128* kitSetGsRegister(u128 *packet, u8 addr, u64 data) {
    ((u64 *)packet)[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    ((u64 *)packet)[1] = SCE_GIF_PACKED_AD;
    ((u64 *)packet)[3] = addr;
    ((u64 *)packet)[2] = data;
    packet += 2;
    return packet;
}
