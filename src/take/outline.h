#ifndef OUTLINE_H
#define OUTLINE_H

#include "take.h"

extern u128* OutlineAllInit(u128 *packet, kitOutlineDrawEnv *pOutlineEnv, u32 fr, u32 odev);
extern u128* OutlinePreRoutine(u128 *packet, kitOutlineDrawEnv *pOutlineEnv, kitOutlineObjEnv *objenv);
extern u128* OutlineExecution(u128 *packet, kitOutlineDrawEnv *pOutlineEnv);
extern void OutlineColorDefine(kitOutlineDrawEnv *pOutlineEnv, kitOutlineObjEnv *objenv);
extern kitInitEffectBufAllPacket* InitEffectBufAll(kitInitEffectBufAllPacket *packet);
extern void PacketKickPath3(u32 ptr, u32 size);
extern void PacketDump(u128 *buf, u32 size);
extern kitEffectBufEnablePacket* EffectBufEnable(kitEffectBufEnablePacket *buf, kitOutlineDrawEnv *env, u64 fmask, u8 zmask);
extern kitEffectBufEnablePacket* EffectBufEnableNXY(kitEffectBufEnablePacket *buf, kitOutlineDrawEnv *env, u64 fmask, u8 zmask);
extern kitEffectBufDisablePacket* EffectBufDisable(kitEffectBufDisablePacket *packet, kitOutlineDrawEnv *env, u64 fmask, u8 zmask);
extern kitEffectBufDisablePacket* EffectBufDisableNXY(kitEffectBufDisablePacket *packet, kitOutlineDrawEnv *env, u64 fmask, u8 zmask);
extern kitInitEffectBufBPacket* InitEffectBufB(kitInitEffectBufBPacket *packet, kitOutlineDrawEnv *env);
extern kitDrawSpritePacket* DrawSprite(kitDrawSpritePacket *packet, kitOutlineDrawEnv *env, u64 test, u8 alpha_switch, u8 tex_func);
extern kitDrawSpriteRegSetPacket* DrawSpriteRegSet(kitDrawSpriteRegSetPacket *packet, kitOutlineDrawEnv *env);
extern u128* SetGsRegister(u128 *packet, u8 addr, u64 data);

#endif