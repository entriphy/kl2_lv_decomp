#include "take/outline.h"

u128* OutlineAllInit(u128 *packet, kitOutlineDrawEnv *pOutlineEnv, u32 fr, u32 odev) {
    *(u128 *)pOutlineEnv->bbox[0] = 0;
    *(u128 *)pOutlineEnv->bbox[1] = 0;
    *(u128 *)pOutlineEnv->bbox[2] = 0;
    *(u128 *)pOutlineEnv->bbox[3] = 0;

    pOutlineEnv->fr_page = fr & 1 ? 0x46 : 0x00;
    pOutlineEnv->fr_block = fr & 1 ? 0x8C0 : 0x00;
    pOutlineEnv->xyoffset = SCE_GS_SET_XYOFFSET(0x6C00, odev & 1 ? 0x7908 : 0x7900);

    // Use effect framebuffer w/ XYOFFSET(0x6C00, 0x7900), update color, do not update Z buffer
    packet = (u128 *)EffectBufEnableNXY((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0, 1);

    // Clear entire effect framebuffer
    packet = (u128 *)InitEffectBufAll((kitInitEffectBufAllPacket *)packet);

    // Use display buffer, update color, update Z buffer
    packet = (u128 *)EffectBufDisable((kitEffectBufDisablePacket *)packet, pOutlineEnv, 0, 0);
    return packet;
}

u128* OutlinePreRoutine(u128 *packet, kitOutlineDrawEnv *pOutlineEnv, kitOutlineObjEnv *objenv) {
    pOutlineEnv->status = objenv->status[pOutlineEnv->partsnum];
    if (pOutlineEnv->status == 2 || pOutlineEnv->status == 3 || pOutlineEnv->status == 6) {
        // Use effect framebuffer, update color except lower alpha bits, update Z buffer
        return (u128 *)EffectBufEnable((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0x7F000000, 0);
    } else {
        // Use effect framebuffer, update color, do not update Z buffer
        packet = (u128 *)EffectBufEnable((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0, 1);

        // Clear framebuffer within bounding box *of the previous part* without updating Z buffer
        packet = (u128 *)InitEffectBufB((kitInitEffectBufBPacket *)packet, pOutlineEnv);

        // Use effect framebuffer, update color except lower alpha bits, update Z buffer
        return (u128 *)EffectBufEnable((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0x7F000000, 0);
    }
}

u128* OutlineExecution(u128 *packet, kitOutlineDrawEnv *pOutlineEnv) {
    s32 i;

    // Do not render to display buffer if outline type is 1, 2, or 5
    if (pOutlineEnv->status == 1 || pOutlineEnv->status == 2 || pOutlineEnv->status == 5) {
        return packet;
    }

    // Increment xy-max by 1 pixel
    pOutlineEnv->bbox[0][0] += 16;
    pOutlineEnv->bbox[0][1] += 16;

    // Decrement xy-min by 1 pixel
    pOutlineEnv->bbox[1][0] -= 16;
    pOutlineEnv->bbox[1][1] -= 16;

    // Increment z-max by 1 pixel (back)
    // I don't think this value is actually used?
    pOutlineEnv->bbox[0][2] += 16;

    for (i = 0; i < 2; i++) {
        // pOutlineEnv->bbox[i][0] = clamp(pOutlineEnv->bbox[i][0], 0x6C00(0.0), 0x9400(1.0))
        // pOutlineEnv->bbox[i][1] = clamp(pOutlineEnv->bbox[i][1], 0x7900(0.0), 0x8700(1.0))
        pOutlineEnv->bbox[i][0] = pOutlineEnv->bbox[i][0] > 0x9400 ? 0x9400 : pOutlineEnv->bbox[i][0] < 0x6C00 ? 0x6C00 : pOutlineEnv->bbox[i][0];
        pOutlineEnv->bbox[i][1] = pOutlineEnv->bbox[i][1] > 0x8700 ? 0x8700 : pOutlineEnv->bbox[i][1] < 0x7900 ? 0x7900 : pOutlineEnv->bbox[i][1];

        // Remove XYOFFSET (these are UVs)
        pOutlineEnv->bbox[i + 2][0] = pOutlineEnv->bbox[i][0] - 0x6C00;
        pOutlineEnv->bbox[i + 2][1] = pOutlineEnv->bbox[i][1] - 0x7900;
    }

    if (pOutlineEnv->status == 4) {
        packet = (u128 *)EffectBufDisableNXY((kitEffectBufDisablePacket *)packet, pOutlineEnv, 0, 1);
        packet = (u128 *)DrawSpriteRegSet((kitDrawSpriteRegSetPacket *)packet, pOutlineEnv);

        pOutlineEnv->bbox[2][0] += 8;
        pOutlineEnv->bbox[2][1] += 8;
        pOutlineEnv->bbox[3][0] += 8;
        pOutlineEnv->bbox[3][1] += 8;
        return (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, SCE_GS_SET_TEST(SCE_GS_TRUE, SCE_GS_ALPHA_GEQUAL, 1, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_TRUE, SCE_GS_ZGEQUAL), 0, SCE_GS_DECAL);
    } else {
        // Set CLAMP_2, TEX1_2, FRAME_1, FBA_2, and ALPHA_2 registers
        packet = (u128 *)DrawSpriteRegSet((kitDrawSpriteRegSetPacket *)packet, pOutlineEnv);

        // Increment xy-max UVs by half pixel
        pOutlineEnv->bbox[2][0] += 8;
        pOutlineEnv->bbox[2][1] += 8;

        // Increment xy-min UVs by half pixel
        pOutlineEnv->bbox[3][0] += 8;
        pOutlineEnv->bbox[3][1] += 8;
        
        // Use display buffer, update color, do not update Z buffer
        packet = (u128 *)EffectBufDisableNXY((kitEffectBufDisablePacket *)packet, pOutlineEnv, 0xFF000000, 1);

        // Draw color to display buffer if alpha >= 1 and depth test passes
        packet = (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, SCE_GS_SET_TEST(SCE_GS_TRUE, SCE_GS_ALPHA_GEQUAL, 1, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_TRUE, SCE_GS_ZGEQUAL), 0, SCE_GS_DECAL);

        // Update alpha
        packet = (u128 *)SetGsRegister(packet, SCE_GS_FRAME_2, SCE_GS_SET_FRAME_2(pOutlineEnv->fr_page, 10, SCE_GS_PSMCT32, 0x00FFFFFF));

        // Draw alpha to display buffer
        packet = (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, SCE_GS_SET_TEST(SCE_GS_FALSE, SCE_GS_ALPHA_NEVER, 0, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_TRUE, SCE_GS_ZALWAYS), 0, SCE_GS_DECAL);

        // Update color and alpha
        packet = (u128 *)SetGsRegister(packet, SCE_GS_FRAME_2, SCE_GS_SET_FRAME_2(pOutlineEnv->fr_page, 10, SCE_GS_PSMCT32, 0x00000000));
        
        // Decrement xy-max UVs by half pixel
        pOutlineEnv->bbox[2][0] -= 8;
        pOutlineEnv->bbox[2][1] -= 8;

        // Decrement xy-min UVs by half pixel
        pOutlineEnv->bbox[3][0] -= 8;
        pOutlineEnv->bbox[3][1] -= 8;

        // Draw to display buffer with alpha blending if alpha is < 0x80, dst alpha is 0x80, and depth test passes
        packet = (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, SCE_GS_SET_TEST(SCE_GS_TRUE, SCE_GS_ALPHA_LESS, 0x80, SCE_GS_AFAIL_KEEP, SCE_GS_TRUE, 1, SCE_GS_TRUE, SCE_GS_ZGEQUAL), 1, SCE_GS_MODULATE);
        
        // Increment xy-max UVs by 1 pixel
        pOutlineEnv->bbox[2][0] += 16;
        pOutlineEnv->bbox[2][1] += 16;

        // Increment xy-min UVs by 1 pixel
        pOutlineEnv->bbox[3][0] += 16;
        pOutlineEnv->bbox[3][1] += 16;

        // Draw to display buffer with alpha blending if alpha is < 0x80, dst alpha is 0x80, and depth test passes
        return (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, SCE_GS_SET_TEST(SCE_GS_TRUE, SCE_GS_ALPHA_LESS, 0x80, SCE_GS_AFAIL_KEEP, SCE_GS_TRUE, 1, SCE_GS_TRUE, SCE_GS_ZGEQUAL), 1, SCE_GS_MODULATE);
    }
}

void OutlineColorDefine(kitOutlineDrawEnv *pOutlineEnv, kitOutlineObjEnv *objenv) {
    pOutlineEnv->spritez = pOutlineEnv->bbox[0][2];
    // wtf
    pOutlineEnv->alpha = pOutlineEnv->spritez < 0x1000   ? 0                                               :
                         pOutlineEnv->spritez < 0x10000  ? ((pOutlineEnv->spritez - 0x1000) * 56) / 0xF000 :
                         pOutlineEnv->spritez >= 0x20000 ? (pOutlineEnv->spritez < 0x100000 ? ((pOutlineEnv->spritez - 0x20000) * 24) / 0xE0000 + 56 : 80)
                         : 56;
}

kitInitEffectBufAllPacket* InitEffectBufAll(kitInitEffectBufAllPacket *packet) {
    kitADDR_DATA *draw_packet = (kitADDR_DATA *)packet;
    s32 i;
    s32 j;

    draw_packet->data = SCE_GIF_SET_TAG(3, SCE_GS_FALSE, SCE_GS_TRUE, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 1, 0), SCE_GIF_PACKED, 1);
    draw_packet->addr = SCE_GIF_PACKED_AD;
    draw_packet++;
    draw_packet->addr = SCE_GS_TEST_2;
    draw_packet->data = SCE_GS_SET_TEST_2(SCE_GS_FALSE, SCE_GS_ALPHA_NEVER, 0, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_TRUE, SCE_GS_ZALWAYS);
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
        for (j = 0; j < 10; draw_packet++, j++) {
            draw_packet->data = SCE_GS_SET_XYZ2(0x6C00 + j * 0x400, 0x7900 + i * 0x200, 0);
            draw_packet->addr = SCE_GS_SET_XYZ2(0x6C00 + (j + 1) * 0x400, 0x7900 + (i + 1) * 0x200, 0);
        }
    }

    return (kitInitEffectBufAllPacket *)draw_packet;
}

void PacketKickPath3(u32 ptr, u32 size) {
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

void PacketDump(u128 *buf, u32 size) {
    u32 *p = (u32 *)buf;
    s32 i;
    s32 j;

    for (i = 0; i < size; i++) {
        // printf("%08x: %08x %08x %08x %08x\n", (u32)p, *p++, *p++, *p++, *p++);
        for (j = 0; j < 256; j++) {
            FlushCache(WRITEBACK_DCACHE);
        }
    }
}

kitEffectBufEnablePacket* EffectBufEnable(kitEffectBufEnablePacket *buf, kitOutlineDrawEnv *env, u64 fmask, u8 zmask) {
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

kitEffectBufEnablePacket* EffectBufEnableNXY(kitEffectBufEnablePacket *buf, kitOutlineDrawEnv *env, u64 fmask, u8 zmask) {
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

kitEffectBufDisablePacket* EffectBufDisable(kitEffectBufDisablePacket *packet, kitOutlineDrawEnv *env, u64 fmask, u8 zmask) {
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

kitEffectBufDisablePacket* EffectBufDisableNXY(kitEffectBufDisablePacket *packet, kitOutlineDrawEnv *env, u64 fmask, u8 zmask) {
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

kitInitEffectBufBPacket* InitEffectBufB(kitInitEffectBufBPacket *packet, kitOutlineDrawEnv *env) {
    packet->giftag1.data = SCE_GIF_SET_TAG(7, SCE_GS_TRUE, SCE_GS_TRUE, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 1, 0), SCE_GIF_PACKED, 1);
    packet->giftag1.addr = SCE_GIF_PACKED_AD;
    packet->test1.addr = SCE_GS_TEST_2;
    packet->test1.data = SCE_GS_SET_TEST_2(SCE_GS_FALSE, SCE_GS_ALPHA_NEVER, 0, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_TRUE, SCE_GS_ZALWAYS);
    packet->zubf1.addr = SCE_GS_ZBUF_2;
    packet->zubf1.data = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMCT16S, 0x1);
    packet->alpha1.addr = SCE_GS_RGBAQ;
    packet->alpha1.data = SCE_GS_SET_RGBAQ(0, 0, 0, 0, 0);
    packet->sprite[0].addr = SCE_GS_XYZ3;
    packet->sprite[0].data = SCE_GS_SET_XYZ3(env->bbox[0][0] + 32, env->bbox[0][1] + 32, 0);
    packet->sprite[1].addr = SCE_GS_XYZ2;
    packet->sprite[1].data = SCE_GS_SET_XYZ2(env->bbox[1][0] - 32, env->bbox[1][1] - 32, 0);
    packet->zbuf2.addr = SCE_GS_ZBUF_2;
    packet->zbuf2.data = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMCT16S, 0);
    packet->test2.addr = SCE_GS_TEST_2;
    packet->test2.data = SCE_GS_SET_TEST_2(SCE_GS_FALSE, SCE_GS_ALPHA_NEVER, 0, SCE_GS_AFAIL_KEEP, SCE_GS_FALSE, 0, SCE_GS_TRUE, SCE_GS_ZGREATER);
    packet++;
    return packet;
}

kitDrawSpritePacket* DrawSprite(kitDrawSpritePacket *packet, kitOutlineDrawEnv *env, u64 test, u8 alpha_switch, u8 tex_func) {
    packet->giftag1.data = SCE_GIF_SET_TAG(4, SCE_GS_FALSE, SCE_GS_TRUE, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, alpha_switch, 0, 1, 1, 0), SCE_GIF_PACKED, 1);
    packet->giftag1.addr = SCE_GIF_PACKED_AD;
    packet->tex0.addr = SCE_GS_TEX0_2;
    packet->tex0.data = SCE_GS_SET_TEX0(0x1680, 10, SCE_GS_PSMCT32, 10, 8, 1, tex_func, 0x0000, 0, 0, 0, 0);
    packet->texflush.addr = SCE_GS_TEXFLUSH;
    packet->texflush.data = 0;
    packet->test1.addr = SCE_GS_TEST_2;
    packet->test1.data = test;
    packet->rgba.addr = SCE_GS_RGBAQ;
    packet->rgba.data = SCE_GS_SET_RGBAQ(0, 0, 0, 0x80, 0);

    packet->giftag2.data = SCE_GIF_SET_TAG(2, SCE_GS_TRUE, SCE_GS_TRUE, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, alpha_switch, 0, 1, 1, 0), SCE_GIF_PACKED, 2);
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

kitDrawSpriteRegSetPacket* DrawSpriteRegSet(kitDrawSpriteRegSetPacket *packet, kitOutlineDrawEnv *env) {
    packet->giftag1.data = SCE_GIF_SET_TAG(5, SCE_GS_TRUE, SCE_GS_FALSE, 0, SCE_GIF_PACKED, 1);
    packet->giftag1.addr = SCE_GIF_PACKED_AD;
    packet->clamp1.addr = SCE_GS_CLAMP_2;
    packet->clamp1.data = SCE_GS_SET_CLAMP_2(SCE_GS_CLAMP, SCE_GS_CLAMP, 0, 0, 0, 0);
    packet->tex01.addr = SCE_GS_TEX1_2;
    packet->tex01.data = SCE_GS_SET_TEX1_2(0, 0, SCE_GS_LINEAR, SCE_GS_LINEAR, 0, 0, 0);
    packet->frame.addr = SCE_GS_FRAME_1;
    packet->frame.data = SCE_GS_SET_FRAME_1(env->fr_page, 10, SCE_GS_PSMCT32, 0xFF000000);
    packet->fba.addr = SCE_GS_FBA_2;
    packet->fba.data = SCE_GS_SET_FBA(0);
    packet->alpha1.addr = SCE_GS_ALPHA_2;
    packet->alpha1.data = SCE_GS_SET_ALPHA_2(SCE_GS_BLEND_RGB_SRC, SCE_GS_BLEND_RGB_DST, SCE_GS_BLEND_ALPHA_FIX, SCE_GS_BLEND_RGB_DST, env->alpha);
    packet++;
    return packet;
}

u128* SetGsRegister(u128 *packet, u8 addr, u64 data) {
    ((u64 *)packet)[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    ((u64 *)packet)[1] = SCE_GIF_PACKED_AD;
    ((u64 *)packet)[3] = addr;
    ((u64 *)packet)[2] = data;
    packet += 2;
    return packet;
}
