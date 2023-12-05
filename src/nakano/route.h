#ifndef ROUTE_H
#define ROUTE_H

#include "nakano.h"

extern s32 InitMichi(u32 *adr);
extern ROUTE* GetMichiAdrs(s32 rtn);
extern void SetRouteNo(RT_WRK *rtw, s32 rtn);
extern s32 GetMichiNo(ROUTE *rtp);
extern void MichiPosi(RT_WRK *rtw, sceVu0FVECTOR vf);
extern void MichiPosiXZ(RT_WRK *rtw, sceVu0FVECTOR vf);
extern f32 MichiPosiY(RT_WRK *rtw);
extern void GetMichiRotXY(sceVu0FVECTOR vec, sceVu0FVECTOR rot);
extern void MichiMove(RT_WRK *rtw, s32 Spd);
extern s32 GetRouteLength(ROUTE *rtp);
extern u16 GetRootEndCode(RT_WRK *rtw, s16 *plcp);
extern s32 GetMichinori(RT_WRK *rtw);
extern void SetMichinori(RT_WRK *rtw, s32 mcn);
extern void GetMichiVec(RT_WRK *rtw, sceVu0FVECTOR spd);
extern f32 GetVectorLenXZ(sceVu0FVECTOR vf);
extern f32 GetMichiSaka(RT_WRK *rtw, s32 michimuki);
extern f32 GetDistanceXZ(sceVu0FVECTOR pos1, sceVu0FVECTOR pos2);
extern f32 GetDistanceXYZ(sceVu0FVECTOR pos1, sceVu0FVECTOR pos2);
extern f32 GetTaniVector(sceVu0FVECTOR pos1, sceVu0FVECTOR pos2, sceVu0FVECTOR vec);
extern f32 GetTaniVector2(sceVu0FVECTOR pos1, sceVu0FVECTOR pos2, sceVu0FVECTOR vec);
extern void SetTaniVector(sceVu0FVECTOR vfo, sceVu0FVECTOR vfi);
extern s32 noploop();
extern s32 MichiBunki(RT_WRK *rtw, const sceVu0FVECTOR pos);
extern s32 MichiBunki1(RT_WRK *rtw, sceVu0FVECTOR pos, ROUTE *rtp);
extern s32 MichiBunki2(RT_WRK *rtw, sceVu0FVECTOR pos);
extern s32 MichiBunki2Cir(RT_WRK *rtw, sceVu0FVECTOR pos);
extern void DrawDebugRoute();
extern void testmove(OBJWORK *objw);

#endif
