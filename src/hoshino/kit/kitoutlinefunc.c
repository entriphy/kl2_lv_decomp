#include "hoshino/kit/kitoutlinefunc.h"
#include "hoshino/kit/kitoutlineprog.h"

u128* kitOutlineAllInit(u128 *packet, kitOutlineDrawEnv *pOutlineEnv, u32 fr, u32 odev) {
    *(u128 *)pOutlineEnv->bbox[0] = 0;
    *(u128 *)pOutlineEnv->bbox[1] = 0;
    *(u128 *)pOutlineEnv->bbox[2] = 0;
    *(u128 *)pOutlineEnv->bbox[3] = 0;

    pOutlineEnv->fr_page = fr & 1 ? 0x46 : 0x00;
    pOutlineEnv->fr_block = fr & 1 ? 0x8C0 : 0x00;
    pOutlineEnv->xyoffset = SCE_GS_SET_XYOFFSET(0x6C00, odev & 1 ? 0x7908 : 0x7900);

    packet = (u128 *)kitEffectBufEnableNXY((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0, 1);
    packet = (u128 *)kitInitEffectBufAll((kitInitEffectBufAllPacket *)packet);
    packet = (u128 *)kitEffectBufDisable((kitEffectBufDisablePacket *)packet, pOutlineEnv, 0, 0);
    return packet;
}

u128* kitOutlineExecution(u128 *packet, kitOutlineDrawEnv *pOutlineEnv) {
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
        packet = (u128 *)kitEffectBufDisableNXY((kitEffectBufDisablePacket *)packet, pOutlineEnv, 0, 1);
        packet = (u128 *)kitDrawSpriteRegSet((kitDrawSpriteRegSetPacket *)packet, pOutlineEnv);
        pOutlineEnv->bbox[2][0] += 8;
        pOutlineEnv->bbox[2][1] += 8;
        pOutlineEnv->bbox[3][0] += 8;
        pOutlineEnv->bbox[3][1] += 8;

        return (u128 *)kitDrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x5001B, 0, 1);
    } else {
        packet = (u128 *)kitDrawSpriteRegSet((kitDrawSpriteRegSetPacket *)packet, pOutlineEnv);
        pOutlineEnv->bbox[2][0] += 8;
        pOutlineEnv->bbox[2][1] += 8;
        pOutlineEnv->bbox[3][0] += 8;
        pOutlineEnv->bbox[3][1] += 8;

        packet = (u128 *)kitEffectBufDisableNXY((kitEffectBufDisablePacket *)packet, pOutlineEnv, 0xFF000000, 1);
        packet = (u128 *)kitDrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x5001B, 0, 1);
        packet = (u128 *)kitSetGsRegister(packet, SCE_GS_FRAME_2, SCE_GS_SET_FRAME_2(pOutlineEnv->fr_page, 10, 0xFFFF00, 0xFFFF00));
        packet = (u128 *)kitDrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x30000, 0, 1);
        packet = (u128 *)kitSetGsRegister(packet, SCE_GS_FRAME_2, SCE_GS_SET_FRAME_2(pOutlineEnv->fr_page, 10, 0, 0));
        pOutlineEnv->bbox[2][0] -= 8;
        pOutlineEnv->bbox[2][1] -= 8;
        pOutlineEnv->bbox[3][0] -= 8;
        pOutlineEnv->bbox[3][1] -= 8;

        packet = (u128 *)kitDrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x5C805, 1, 0);
        pOutlineEnv->bbox[2][0] += 16;
        pOutlineEnv->bbox[2][1] += 16;
        pOutlineEnv->bbox[3][0] += 16;
        pOutlineEnv->bbox[3][1] += 16;

        return (u128 *)kitDrawSprite((kitDrawSpritePacket *)packet, pOutlineEnv, 0x5C805, 1, 0);
    }
}

u128* kitOutlinePreRoutine(u128 *packet, kitOutlineDrawEnv *pOutlineEnv, kitOutlineObjEnv *objenv) {
    pOutlineEnv->status = objenv->status[pOutlineEnv->partsnum];
    if (pOutlineEnv->status == 2 || pOutlineEnv->status == 3 || pOutlineEnv->status == 6) {
        return (u128 *)kitEffectBufEnable((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0x7F000000, 0);
    } else {
        packet = (u128 *)kitEffectBufEnable((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0, 1);
        packet = (u128 *)kitInitEffectBufB((kitInitEffectBufBPacket *)packet, pOutlineEnv);
        packet = (u128 *)kitEffectBufEnable((kitEffectBufEnablePacket *)packet, pOutlineEnv, 0x7F000000, 0);
        pOutlineEnv->bbox[0][0] = 0x6C00;
        pOutlineEnv->bbox[0][1] = 0x7900;
        pOutlineEnv->bbox[0][2] = 0;
        pOutlineEnv->bbox[1][0] = 0x9400;
        pOutlineEnv->bbox[1][1] = 0x8700;
        pOutlineEnv->bbox[1][2] = -1;
        return packet;
    }
}

void kitOutlineColorDefine(kitOutlineDrawEnv *pOutlineEnv, kitOutlineObjEnv *objenv) {
    pOutlineEnv->spritez = pOutlineEnv->bbox[0][2];
    // wtf
    pOutlineEnv->alpha = pOutlineEnv->spritez < 0x1000   ? 0                                               :
                         pOutlineEnv->spritez < 0x8000  ? ((pOutlineEnv->spritez - 0x1000) * 48) / 0x7000 :
                         pOutlineEnv->spritez >= 0xC000 ? (pOutlineEnv->spritez < 0x20000 ? ((pOutlineEnv->spritez - 0xC000) / 0x400) + 48 : 128)
                         : 48;
}

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

void kitPacketDump(u128 *buf, u32 size) {
    u32 *p = (u32 *)buf;
    s32 i;
    s32 j;

    for (i = 0; i < size; i++) {
        printf("%08x: %08x %08x %08x %08x\n", (u32)p, *p++, *p++, *p++, *p++);
        for (j = 0; j < 256; j++) {
            FlushCache(WRITEBACK_DCACHE);
        }
    }
}
