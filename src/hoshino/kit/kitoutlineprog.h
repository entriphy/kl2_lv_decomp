#ifndef KITOUTLINEPROG_H
#define KITOUTLINEPROG_H

#include "vtusr.h"

extern kitInitEffectBufAllPacket* kitInitEffectBufAll(kitInitEffectBufAllPacket *packet);
extern kitInitAlphaPlanePacket* kitInitAlphaPlane(kitInitAlphaPlanePacket *packet, kitOutlineDrawEnv *env);
extern kitEffectBufEnablePacket* kitEffectBufEnable(kitEffectBufEnablePacket *buf, kitOutlineDrawEnv *env, u64 fmask, u8 zmask);
extern kitEffectBufEnablePacket* kitEffectBufEnableNXY(kitEffectBufEnablePacket *buf, kitOutlineDrawEnv *env, u64 fmask, u8 zmask);
extern kitEffectBufDisablePacket* kitEffectBufDisable(kitEffectBufDisablePacket *packet, kitOutlineDrawEnv *env, u64 fmask, u8 zmask);
extern kitEffectBufDisablePacket* kitEffectBufDisableNXY(kitEffectBufDisablePacket *packet, kitOutlineDrawEnv *env, u64 fmask, u8 zmask);
extern kitInitEffectBufBPacket* kitInitEffectBufB(kitInitEffectBufBPacket *packet, kitOutlineDrawEnv *env);
extern kitDrawSpritePacket* kitDrawSprite(kitDrawSpritePacket *packet, kitOutlineDrawEnv *env, u64 test, u8 alpha_switch, u8 tex_func);
extern kitDrawSpriteRegSetPacket* kitDrawSpriteRegSet(kitDrawSpriteRegSetPacket *packet, kitOutlineDrawEnv *env);
extern u128* kitSetGsRegister(u128 *packet, u8 addr, u64 data);

#endif
