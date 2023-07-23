#ifndef MIMESYS_H
#define MIMESYS_H

#include "take.h"

extern void MimeWork(SFXOBJ *pObj);
extern void MimeSetWork(SFXOBJ *pObj);
extern void MimeTblSet(SFXOBJ *pObj, s16 *TblAdrs);
extern u64  MimeIdCheck(SFXOBJ *pObj, s32 StartNum, s32 EndNum);
extern void MimeOff(SFXOBJ *pObj);
extern void SetVoiceMime(SFXOBJ *pObj, s16 *pVmime);
extern void EraseVoiceMime(SFXOBJ *pObj);
extern void VoiceMime(SFXOBJ *pObj);
extern s32  SetSyncMime(SFXOBJ *pObj);
extern void MimeSet(SFXOBJ *pObj, s32 StartNum, s32 EndNum, f32 Weight);

#endif
