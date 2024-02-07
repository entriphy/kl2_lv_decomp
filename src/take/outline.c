#include "take/outline.h"

u128* OutlineAllInit(u128 *packet, kitOutlineDrawEnv *pOutlineEnv, u32 fr, u32 odev) {
    *(u128 *)pOutlineEnv->bbox[0] = 0;
    *(u128 *)pOutlineEnv->bbox[1] = 0;
    *(u128 *)pOutlineEnv->bbox[2] = 0;
    *(u128 *)pOutlineEnv->bbox[3] = 0;

    pOutlineEnv->fr_page = fr & 1 ? 0x46 : 0x00;
    pOutlineEnv->fr_block = fr & 1 ? 0x8C0 : 0x00;
    pOutlineEnv->xyoffset = SCE_GS_SET_XYOFFSET(0x6C00, odev & 1 ? 0x7908 : 0x7900);

    return EffectBufDisable(InitEffectBufAll(EffectBufEnableNXY(packet, pOutlineEnv, 0, 1)), pOutlineEnv, 0, 0);
}

u128* OutlinePreRoutine(u128 *packet, kitOutlineDrawEnv *pOutlineEnv, kitOutlineObjEnv *objenv) {
    pOutlineEnv->status = objenv->status[pOutlineEnv->partsnum];
    if (pOutlineEnv->status == 2 || pOutlineEnv->status == 3 || pOutlineEnv->status == 6) {
        return (u128 *)EffectBufEnable((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0x7F000000, 0);
    } else {
        packet = (u128 *)EffectBufEnable((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0, 1);
        packet = (u128 *)InitEffectBufB((kitInitEffectBufBPacket *)packet, pOutlineEnv);
        return (u128 *)EffectBufEnable((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0x7F000000, 0);
    }
}

u128* OutlineExecution(u128 *packet, kitOutlineDrawEnv *pOutlineEnv) {
    s32 i;

    if (pOutlineEnv->status == 1 || pOutlineEnv->status == 2 || pOutlineEnv->status == 5) {
        return packet;
    }

    pOutlineEnv->bbox[0][0] += 16;
    pOutlineEnv->bbox[0][1] += 16;
    pOutlineEnv->bbox[1][0] -= 16;
    pOutlineEnv->bbox[1][1] -= 16;
    pOutlineEnv->bbox[0][2] += 16;

    for (i = 0; i < 2; i++) {
        pOutlineEnv->bbox[i][0] = pOutlineEnv->bbox[i][0] > 0x9400 ? 0x9400 : pOutlineEnv->bbox[i][0] < 0x6C00 ? 0x6C00 : pOutlineEnv->bbox[i][0];
        pOutlineEnv->bbox[i][1] = pOutlineEnv->bbox[i][1] > 0x8700 ? 0x8700 : pOutlineEnv->bbox[i][1] < 0x7900 ? 0x7900 : pOutlineEnv->bbox[i][1];
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

        return (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x5001B, 0, 1);
    } else {
        packet = (u128 *)DrawSpriteRegSet((kitDrawSpriteRegSetPacket *)packet, pOutlineEnv);
        pOutlineEnv->bbox[2][0] += 8;
        pOutlineEnv->bbox[2][1] += 8;
        pOutlineEnv->bbox[3][0] += 8;
        pOutlineEnv->bbox[3][1] += 8;

        packet = (u128 *)EffectBufDisableNXY((kitEffectBufDisablePacket *)packet, pOutlineEnv, 0xFF000000, 1);
        packet = (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x5001B, 0, 1);
        packet = (u128 *)SetGsRegister(packet, SCE_GS_FRAME_2, SCE_GS_SET_FRAME_2(pOutlineEnv->fr_page, 10, 0xFFFF00, 0xFFFF00));
        packet = (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x30000, 0, 1);
        packet = (u128 *)SetGsRegister(packet, SCE_GS_FRAME_2, SCE_GS_SET_FRAME_2(pOutlineEnv->fr_page, 10, 0, 0));
        pOutlineEnv->bbox[2][0] -= 8;
        pOutlineEnv->bbox[2][1] -= 8;
        pOutlineEnv->bbox[3][0] -= 8;
        pOutlineEnv->bbox[3][1] -= 8;

        packet = (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x5C805, 1, 0);
        pOutlineEnv->bbox[2][0] += 16;
        pOutlineEnv->bbox[2][1] += 16;
        pOutlineEnv->bbox[3][0] += 16;
        pOutlineEnv->bbox[3][1] += 16;

        return (u128 *)DrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x5C805, 1, 0);
    }
}

void OutlineColorDefine(kitOutlineDrawEnv *pOutlineEnv, kitOutlineObjEnv *objenv) {
    pOutlineEnv->spritez = pOutlineEnv->bbox[0][2];
    // wtf
    pOutlineEnv->alpha = pOutlineEnv->spritez < 0x1000 ? 0 :
        pOutlineEnv->spritez < 0x10000 ? ((pOutlineEnv->spritez - 0x1000) * 56) / 0xF000 :
        pOutlineEnv->spritez >= 0x20000 ? (pOutlineEnv->spritez < 0x100000 ? ((pOutlineEnv->spritez - 0x20000) * 24) / 0xE0000 + 56 : 80) : 56;
}

// TODO: Technically matches
kitInitEffectBufAllPacket* InitEffectBufAll(kitInitEffectBufAllPacket *packet) {
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
            draw_packet->data = SCE_GS_SET_XYZ2(j * 0x400 + 0x6C00, i * 0x200 + 0x7900, 0);
            j++;
            draw_packet->addr = SCE_GS_SET_XYZ2(j * 0x400 + 0x6C00, i * 0x200 + 0x7B00, 0);
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
    packet->zbuf2.data = SCE_GS_SET_ZBUF_1(0x8C, SCE_GS_PSMCT16S, 0);
    packet->test2.addr = SCE_GS_TEST_2;
    packet->test2.data = SCE_GS_SET_TEST_2(0, 0, 0, 0, 0, 0, 1, 3);
    packet++;
    return packet;
}

kitDrawSpritePacket* DrawSprite(kitDrawSpritePacket *packet, kitOutlineDrawEnv *env, u64 test, u8 alpha_switch, u8 tex_func) {
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

kitDrawSpriteRegSetPacket* DrawSpriteRegSet(kitDrawSpriteRegSetPacket *packet, kitOutlineDrawEnv *env) {
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

u128* SetGsRegister(u128 *packet, u_char addr, u_long data) {
    ((u64 *)packet)[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    ((u64 *)packet)[1] = SCE_GIF_PACKED_AD;
    ((u64 *)packet)[3] = addr;
    ((u64 *)packet)[2] = data;
    packet += 2;
    return packet;
}
