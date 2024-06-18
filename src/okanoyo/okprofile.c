#include "okanoyo/okprofile.h"
#include "okanoyo/okinit.h"
#include "okanoyo/okio.h"
#include "okanoyo/okmain.h"
#include "okanoyo/okstring.h"

OKPROFILE okProfile = {};
s64 profileFlag = 0;

void okProfileInit() {
	s32 i;
	s32 *ptr = (s32 *)&okProfile;

    for (i = 0; i < sizeof(okProfile) / sizeof(s32); i++) {
        ptr[i] = 0;
    }
    profileFlag = 0;
}

s32 OkGetStrMember(char *ptr, char *member, char *type) {
	char line[256];
	char tmp[256];
	char *word;
	s32 cnt = 0;

    while ((s32)(ptr = getline(line, ptr)) != -1) {
        if ((s32)findword("float", line) != -1 || (s32)findword("int", line) != -1) {
            word = findword("float", line);
            if ((s32)word != -1) {
                strcpy(type, "f ");
            } else {
                word = findword("int", line);
                strcpy(type, "i ");
            }
            type += 2;

            getlineword(tmp, getlineword(tmp, getlineword(tmp, word)));
            replacestr(";", " ", tmp);
            strcpy(member, tmp);
            member += strlen(tmp);

            cnt++;
        }
    }

    return cnt;
}

void OkSetStrMemberBIN(char *ptr, int *sptr, char *member, char *type, s32 cnt) {
	char line[256];
	char wbuf[256];
	s32 i;
	char *mem;
	char *typ;
	char mstr[256];
	char tstr[256];

    while ((s32)(ptr = getline(line, ptr)) != -1) {
        getlineword(wbuf, line);
        if ((s32)findword("#", wbuf) == -1) {
            mem = member;
            typ = type;
            for (i = 0; i < cnt; i++) {
                mem = getlineword(mstr, mem);
                typ = getlineword(tstr, typ);
                if (strcmp(wbuf, mstr) == 0) {
                    getlineword(wbuf, getlineword(wbuf, line));
                    if (strcmp(tstr, "f") == 0) {
                        ((f32 *)sptr)[i] = atof(wbuf);
                    } else {
                        sptr[i] = stoi(wbuf);
                    }
                    profileFlag |= 1 << i;
                }
            }
        }
    }
}

s32 OkSetStrMemberTXT(char *ptr, s32 *sptr, char *member, char *type, s32 cnt) {
	s32 i;
	char *mem;
	char *typ;
	char mstr[256];
	char tstr[256];
	s32 size;
	s32 allsize;
	char *bp;

    bp = ptr;
    i = 0;
    mem = member;
    typ = type;
    allsize = 0;

    for (; i < cnt; i++) {
        mem = getlineword(mstr, mem);
        typ = getlineword(tstr, typ);
        if (strcmp(mstr, "vsonI") >= 1) {
            size = sprintf(bp, "%s\t0x%02d%02x\r\n", mstr, GameGbl.vision / 256, GameGbl.vision & 0xFF);
        } else if (strcmp(tstr, "f") == 0) {
            if (pfdatap[i] == NULL) {
                size = sprintf(bp, "%s\t0.0\r\n", mstr);
            } else {
                size = sprintf(bp, "%s\t%f\r\n", mstr, (f32)*pfdatap[i]);
            }
        } else {
            if (pfdatap[i] == NULL) {
                size = sprintf(bp, "%s\t0\r\n", mstr);
            } else {
                size = sprintf(bp, "%s\t%d\r\n", mstr, *pfdatap[i]);
            }
        }

        bp += size;
        allsize += size;
    }

    return allsize;
}

s32 OkReadProfile(char *filename) {
	char buff[4096];
	char member[256];
	char type[128];
	s32 i;
	s32 cnt;

    for (i = 0; i < 4096; i++) {
        buff[i] = 0;
    }

    fontDispID = 0;
    buff[OkRead_file("host0:../okanoyo/profilestr.h", buff)] = 0;
    cnt = OkGetStrMember(buff, member, type);
    for (i = 0; i < 4096; i++) {
        buff[i] = 0;
    }
    buff[OkRead_file(filename,buff)] = 0;
    OkSetStrMemberBIN(buff, &okProfile.fontID, member, type, cnt);

    menuID = okProfile.vsonID;
    profile.menuID = okProfile.vsonID;
    profile.fontID = okProfile.fontID;

    return 1;
}

void OkWriteProfile() {
	char buff[4096];
	char member[256];
	char type[128];
	char filename[256];
	s32 size;
	s32 cnt;
	s32 i;

    buff[OkRead_file("host0:../okanoyo/profilestr.h", buff)] = 0;
    cnt = OkGetStrMember(buff,member,type);
    for (i = 0; i < 4096; i++) {
        buff[i] = 0;
    }
    size = OkSetStrMemberTXT(buff, &okProfile.fontID, member, type, cnt);
    sprintf(filename, "host0:/home/%s/profile.klo", loginName);

    if (OkWrite_file(filename, buff, size) == 0) {
        printf("cannot write profile.\n");
    } else {
        printf("%s profile updated.\n", loginName);
    }
}

s32 OkGetProfile(s32 argc, char *argv[]) {
	char filename[256];

    okProfileInit();
    if (argc >= 2) {
        sprintf(filename, "host0:/home/%s/profile.klo", argv[1]);
        if (OkCheck_file(filename) != -1) {
            printf("%s, hello!\n", argv[1]);
            sprintf(loginName, "%s", argv[1]);
        } else {
            sprintf(filename, "host0:./profile/%s.pro", argv[1]);
            if (OkCheck_file(filename) != -1) {
                printf("%s, hello!\n", argv[1]);
                sprintf(loginName, "%s", argv[1]);
            } else {
                printf("Welcome!\n");
                sprintf(filename, "host0:/home/main/profile.klo");
                sprintf(loginName, "root");
            }
        }
    } else {
        printf("Welcome!\n");
        sprintf(filename, "host0:/home/main/profile.klo");
        sprintf(loginName, "root");
    }

    return OkReadProfile(filename);
}

void OkWriteProfileChk() {
    if (GameGbl.pause_flag && PAD_LVL_SQUARE(GameGbl.kpd[0]) && PAD_TRG_CROSS(GameGbl.kpd[0])) {
        OkWriteProfile();
    }
}