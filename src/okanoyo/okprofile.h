#ifndef OKPROFILE_H
#define OKPROFILE_H

#include "okanoyo.h"

typedef struct {
    s32 fontID;
    s32 vsonID;
    s32 modeID;
    s32 kloDmg;
    s32 plnMod;
} OKPROFILE;

extern OKPROFILE okProfile;
extern s64 profileFlag;

extern void okProfileInit();
extern s32 OkGetStrMember(char *ptr, char *member, char *type);
extern void OkSetStrMemberBIN(char *ptr, int *sptr, char *member, char *type, s32 cnt);
extern s32 OkSetStrMemberTXT(char *ptr, s32 *sptr, char *member, char *type, s32 cnt);
extern s32 OkReadProfile(char *filename);
extern void OkWriteProfile();
extern s32 OkGetProfile(s32 argc, char *argv[]);
extern void OkWriteProfileChk();

#endif
