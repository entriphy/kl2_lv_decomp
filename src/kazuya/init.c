#include "common.h"

s32 ENGLISH_30[] = {0, 5, 10, 11, 12, 13, -1};
s32 FRENCH_30[]  = {1, 6, 10, 11, 12, 13, -1};
s32 SPANISH_30[] = {2, 7, 10, 11, 12, 13, -1};
s32 GERMAN_30[]  = {3, 8, 10, 11, 12, 13, -1};
s32 ITALIAN_30[] = {4, 9, 10, 11, 12, 13, -1};

s32 MUTLI_31[] = {0, 1, -1};

s32 ENGLISH_32[] = {0, -1};
s32 FRENCH_32[]  = {1, -1};
s32 SPANISH_32[] = {2, -1};
s32 GERMAN_32[]  = {3, -1};
s32 ITALIAN_32[] = {4, -1};

s32 ENGLISH_34[] = {0, 5, 6, -1};
s32 FRENCH_34[]  = {1, 5, 6, -1};
s32 SPANISH_34[] = {2, 5, 6, -1};
s32 GERMAN_34[]  = {3, 5, 6, -1};
s32 ITALIAN_34[] = {4, 5, 6, -1};

s32 MULTI_35[] = {0, -1};

void kzStageInit0() {
    s32 stage_no;
    s32 *gms_language;
    u8 *gms;
    s32 i;
    u8 stackShiz[0x100];
    
    kzLoadCommonGms();
    stage_no = kz_Get_KazuyaStageNo();
    if (stage_no > -1) {
        switch (stage_no) {
            case 30:
                switch (SysGbl.Language) {
                    case 1:
                        gms_language = ENGLISH_30;
                        break;
                    case 2:
                        gms_language = FRENCH_30;
                        break;
                    case 3:
                        gms_language = SPANISH_30;
                        break;
                    case 4:
                        gms_language = GERMAN_30;
                        break;
                    case 5:
                        gms_language = ITALIAN_30;
                        break;
                }
                break;
            case 31:
                gms_language = MUTLI_31;
                break;
            case 32:
                switch (SysGbl.Language) {
                    case 1:
                        gms_language = ENGLISH_32;
                        break;
                    case 2:
                        gms_language = FRENCH_32;
                        break;
                    case 3:
                        gms_language = SPANISH_32;
                        break;
                    case 4:
                        gms_language = GERMAN_32;
                        break;
                    case 5:
                        gms_language = ITALIAN_32;
                        break;
                }
                break;
            case 33:
                return;
            case 34:
                switch (SysGbl.Language) {
                    case 1:
                        gms_language = ENGLISH_34;
                        break;
                    case 2:
                        gms_language = FRENCH_34;
                        break;
                    case 3:
                        gms_language = SPANISH_34;
                        break;
                    case 4:
                        gms_language = GERMAN_34;
                        break;
                    case 5:
                        gms_language = ITALIAN_34;
                        break;
                }
                break;
            case 35:
                gms_language = MULTI_35;
                break;
        }

        i = 0;
        gms = (u8 *)hGetDataAddr(7);
        if (gms != NULL) {
            while (gms_language[i] >= 0) {
                FUN_0018dcb0(gms + ((s32 *)gms)[i + 1]);
                sceGsSyncPath(0, 0);
                i++;
            }
        }
        FlushCache(WRITEBACK_DCACHE);
    }
}
