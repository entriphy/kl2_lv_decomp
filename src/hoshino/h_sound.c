#include "h_sound.h"
#include "h_rpc.h"
#include "h_gamesnd.h"
#include "h_menu.h"
#include "h_str.h"
#include "h_util.h"
#include "h_cdvd.h"
#include "okanoyo/okio.h"
#include "h_file.h"

EFXSE efdm[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0100[1] = {
    {4, 0.17f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0101[1] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0102[1] = {
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0103[1] = {
    {4, 0.6f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0104[1] = {
    {4, 0.05f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0105[1] = {
    {5, 0.6f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0200[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0201[1] = {
    {5, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0202[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0203[1] = {
    {5, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0204[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0205[1] = {
    {5, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0206[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0207[1] = {
    {4, 0.3f, 0, 0, 1, 0.09f, 0.0f, 0, 0}
};

EFXSE ef0400[1] = {
    {4, 0.11f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0401[1] = {
    {4, 0.2f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0402[1] = {
    {4, 0.25f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0403[1] = {
    {4, 0.4f, 0, 0, 1, 0.13f, 0.0f, 0, 0}
};

EFXSE ef0404[1] = {
    {4, 0.6f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0405[1] = {
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0406[1] = {
    {4, 0.35f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0407[1] = {
    {5, 0.44f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0408[1] = {
    {4, 0.35f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0409[1] = {
    {5, 0.33f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef040A[1] = {
    {4, 0.45f, 0, 0, 1, 0.13f, 0.0f, 0, 0}
};

EFXSE ef040B[2] = {
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0500[1] = {
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0501[3] = {
    {3, 0.6f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {3, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {3, 0.5f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0504[3] = {
    {3, 0.7f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {3, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {3, 0.8f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0503[2] = {
    {4, 0.7f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0505[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0600[2] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {5, 0.8f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0602[1] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0603[1] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0604[3] = {
    {5, 0.6f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {5, 0.9f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {5, 0.7f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0605[1] = {
    {4, 0.4f, 0, 0, 1, 0.12f, 0.0f, 0, 0}
};

EFXSE ef0606[1] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0607[1] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0608[1] = {
    {5, 0.5f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0609[1] = {
    {5, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0700[1] = {
    {5, 0.5f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0701[2] = {
    {4, 0.45f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0702[1] = {
    {5, 0.45f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0703[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0704[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0705[2] = {
    {4, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0706[1] = {
    {4, 0.1f, 0, 0, 1, 0.075f, 0.0f, 0, 0}
};

EFXSE ef0900[2] = {
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0901[1] = {
    {5, 0.25f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0902[1] = {
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0903[1] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0904[1] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0905[1] = {
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0906[1] = {
    {4, 0.35f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0907[1] = {
    {4, 0.35f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef0908[1] = {
    {4, 0.1f, 0, 0, 1, 0.04f, 0.0f, 0, 0}
};

EFXSE ef1000[1] = {
    {4, 0.15f, 0, 0, 1, 0.02f, 0.0f, 0, 0}
};

EFXSE ef1001[1] = {
    {5, 0.7f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1002[1] = {
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1003[2] = {
    {4, 0.7f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1004[2] = {
    {4, 0.7f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1100[1] = {
    {3, 0.3f, 0, 0, 1, 0.09f, 0.0f, 0, 0}
};

EFXSE ef1101[1] = {
    {3, 0.18f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1102[1] = {
    {3, 0.18f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1103[1] = {
    {4, 0.13f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1104[1] = {
    {3, 0.25f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1105[1] = {
    {3, 0.2f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1106[1] = {
    {3, 0.2f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1107[1] = {
    {4, 0.12f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1108[1] = {
    {4, 0.13f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1109[1] = {
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef110A[1] = {
    {3, 0.25f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef110C[1] = {
    {3, 0.2f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef110D[1] = {
    {3, 0.22f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef110E[1] = {
    {3, 0.22f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef110F[1] = {
    {3, 0.2f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1110[1] = {
    {3, 0.25f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1111[1] = {
    {4, 0.9f, 0, 0, 1, 0.04f, 0.0f, 0, 0}
};

EFXSE ef1200[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1201[1] = {
    {5, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1202[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1203[1] = {
    {5, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1204[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1205[1] = {
    {5, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1206[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1207[1] = {
    {4, 0.3f, 0, 0, 1, 0.09f, 0.0f, 0, 0}
};

EFXSE ef1208[1] = {
    {5, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1300[1] = {
    {4, 0.22f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1301[1] = {
    {5, 0.6f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1302[1] = {
    {4, 0.25f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1303[1] = {
    {4, 0.6f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1304[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1306[1] = {
    {5, 0.1f, 0, 0, 1, 0.04f, 0.0f, 0, 0}
};

EFXSE ef1400[1] = {
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1401[1] = {
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1402[2] = {
    {4, 0.7f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1403[1] = {
    {5, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1404[1] = {
    {5, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1500[2] = {
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0},
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1501[1] = {
    {5, 0.25f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1502[1] = {
    {4, 0.15f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1503[1] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1504[1] = {
    {5, 0.4f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1505[1] = {
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1506[1] = {
    {4, 0.35f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1507[1] = {
    {4, 0.35f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef16dm[1] = {
    {3, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1700[1] = {
    {4, 0.9f, 0, 0, 1, 0.09f, 0.0f, 0, 0}
};

EFXSE ef1701[1] = {
    {5, 0.9f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef1702[1] = {
    {4, 0.9f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1800[1] = {
    {4, 0.1f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1801[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1802[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1803[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1804[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1805[1] = {
    {4, 0.1f, 0, 0, 1, 0.09f, 0.0f, 0, 0}
};

EFXSE ef1900[1] = {
    {4, 0.1f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1901[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1902[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1903[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1904[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1905[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1906[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1907[1] = {
    {5, 0.3f, 0, 0, 1, 0.07f, 0.0f, 0, 0}
};

EFXSE ef1908[1] = {
    {4, 0.07f, 0, 0, 1, 0.09f, 0.0f, 0, 0}
};

EFXSE ef2100[1] = {
    {5, 0.1f, 0, 0, 1, 0.02f, 0.0f, 0, 0}
};

EFXSE ef2200[1] = {
    {4, 0.2f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef2201[1] = {
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef2300[1] = {
    {5, 0.3f, 0, 0, 1, 0.09f, 0.0f, 0, 0}
};

EFXSE ef2301[1] = {
    {5, 0.4f, 0, 0, 1, 0.09f, 0.0f, 0, 0}
};

EFXSE ef2400[1] = {
    {4, 0.3f, 0, 0, 1, 0.02f, 0.0f, 0, 0}
};

EFXSE ef2600[1] = {
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef2601[1] = {
    {5, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef2700[1] = {
    {5, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef2701[1] = {
    {5, 0.6f, 0, 0, 1, 0.6f, 0.0f, 0, 0}
};

EFXSE ef2702[1] = {
    {5, 0.6f, 0, 0, 1, 0.6f, 0.0f, 0, 0}
};

EFXSE ef3901[1] = {
    {4, 0.02f, 0, 0, 1, 0.02f, 0.0f, 0, 0}
};

EFXSE ef4001[1] = {
    {4, 0.1f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE ef4002[1] = {
    {4, 0.3f, 0, 0, 1, 0.05f, 0.0f, 0, 0}
};

EFXSE *EfxSE[50][20] = {
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef0100, ef0101, ef0102, ef0103, ef0104, ef0105, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef0200, ef0201, ef0202, ef0203, ef0204, ef0205, ef0206, ef0207, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef0400, ef0401, ef0402, ef0403, ef0404, ef0405, ef0406, ef0407, ef0408, ef0409, ef040A, ef040B, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef0500, ef0501, efdm, ef0503, ef0504, ef0505, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef0600, efdm, ef0602, ef0603, ef0604, ef0605, ef0606, ef0607, ef0608, ef0609, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef0700, ef0701, ef0702, ef0703, ef0704, ef0705, ef0706, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef0700, ef0701, ef0702, ef0703, ef0704, ef0705, ef0706, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef0900, ef0901, ef0902, ef0903, ef0904, ef0905, ef0906, ef0907, ef0908, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef1000, ef1001, ef1002, ef1003, ef1004, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef1100, ef1101, ef1102, ef1103, ef1104, ef1105, ef1106, ef1107, ef1108, ef1109, ef110A, efdm, ef110C, ef110D, ef110E, ef110F, ef1110, ef1111, efdm, efdm },
    { ef1200, ef1201, ef1202, ef1203, ef1204, ef1205, ef1206, ef1207, ef1208, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef1300, ef1301, ef1302, ef1303, ef1304, efdm, ef1306, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef1400, ef1401, ef1402, ef1403, ef1404, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef1500, ef1501, ef1502, ef1503, ef1504, ef1505, ef1506, ef1507, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef16dm, ef16dm, ef16dm, ef16dm, ef16dm, ef16dm, ef16dm, ef16dm, ef16dm, ef16dm, ef16dm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef1700, ef1701, ef1702, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef1800, ef1801, ef1802, ef1803, ef1804, ef1805, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef1900, ef1901, ef1902, ef1903, ef1904, ef1905, ef1906, ef1907, ef1908, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef2100, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef2200, ef2201, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef2300, ef2301, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef2400, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef2600, ef2601, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { ef2700, ef2701, ef2702, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, ef3901, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, ef4001, ef4002, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm },
    { efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm, efdm }
};

u8 SndPacket[1024] __attribute__((aligned(16)));
u8 SndTempBuff[1048576] __attribute__((aligned(16)));
hSNDDATA SndData = {};
hSNDDATA *sD = NULL;

s32 JamGetHdSize(s32 hdaddr) {
    return ((s32 *)hdaddr)[7];
}

s32 JamGetBdSize(s32 hdaddr) {
    return ((s32 *)hdaddr)[8];
}

void hSndPkEffect() {
    s32 core;
    
    sD->effChange = 1;
    for (core = 0; core < 2; core++) {
        sD->PkNum++;
        *sD->Pk++ = 8;
        *sD->Pk++ = core;
        *sD->Pk++ = sD->effMode;
        *sD->Pk++ = sD->effDepth;
        *sD->Pk++ = sD->effDepth >> 8;
        *sD->Pk++ = sD->effDepth;
        *sD->Pk++ = sD->effDepth >> 8;
        *sD->Pk++ = sD->effDelay;
        *sD->Pk++ = sD->effFeed;
    }
}

void hSndPkSetMVol(s32 voll, s32 volr) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_MVOLALL;
    *sD->Pk++ = voll;
    *sD->Pk++ = (voll >> 8) & 0x7F;
    *sD->Pk++ = volr;
    *sD->Pk++ = (volr >> 8) & 0x7F;
}

void hSndPkSetEVol(s32 vol) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_EVOL;
    *sD->Pk++ = vol;
    *sD->Pk++ = vol >> 8;
}

void hSndPkSetVol(s32 voice, f32 vollf, f32 volrf) {
    s32 voll;
    s32 volr;

    voll = vollf * 16384.0f;
    volr = volrf * 16384.0f;
    if (voll > 0x4000)
        voll = 0x4000;
    if (voll < -0x4000)
        voll = -0x4000;
    if (volr > 0x4000)
        volr = 0x4000;
    if (volr < -0x4000)
        volr = -0x4000;
    
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_VOL;
    *sD->Pk++ = voice;
    *sD->Pk++ = voll;
    *sD->Pk++ = voll >> 8;
    *sD->Pk++ = volr;
    *sD->Pk++ = volr >> 8;
}

void hSndPkSetPitch(s32 voice, s32 pitch) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_PITCH;
    *sD->Pk++ = voice;
    *sD->Pk++ = pitch;
    *sD->Pk++ = pitch >> 8;
}

void hSndPkSetPalPitch(s32 voice) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_PALPITCH;
    *sD->Pk++ = voice;
}

void hSndPkKeyOn(s32 voice, s32 flag, s32 bank, s32 prog, s32 splt, f32 vollf, f32 volrf) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_KEYON;
    *sD->Pk++ = voice;
    *sD->Pk++ = flag;
    *sD->Pk++ = bank;
    *sD->Pk++ = prog;
    *sD->Pk++ = splt;
    sD->KeyonV[voice & 1] |= 1 << (voice >> 1);
    hSndPkSetVol(voice, vollf, volrf);
}

void hSndPkKeyOff(s32 voice) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_KEYOFF;
    *sD->Pk++ = voice;
}

void hSndPkKeyOffAll() {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_KEYOFFALL;
}

int hSndPkGetSize() {
    s32 size;

    size = (s32)sD->Pk - (s32)SndPacket;
    return (size + 0xF) / 0x10 * 0x10;
}

void hSndReset() {
    hBgmReset();
    hPptReset();
    hSndPkKeyOffAll();
    hSeInitGrp(0);
    hSndSetMVol(1.0f);
    sD->fadeFlag = 0;
    sD->Mute = 0;
}

void hSndFadeOutAll(s32 frame) {
    hSeKeyOffAll();
    sD->fadeFlag = 1;
    sD->fadeCnt = 0.0f;
    sD->fadeMax = (f32)frame;
}

void hSndFadeInAll(s32 frame) {
    sD->fadeFlag = 3;
    sD->fadeCnt = 0.0f;
    sD->fadeMax = (f32)frame;
}

void hSndSetMVol(f32 vol) {
    sD->MVol = hSndFader(vol);
}

s32 hSndFader(f32 vol) {
    s32 retvol;

    if (vol == 0.0f)
        return 0;
    if (vol > 1.0f)
        vol = 1.0f;
    vol = sD->dBfader * (1.0f - vol);
    vol = powf(10.0f, (vol + sD->log10Volume) / 20.0f);
    retvol = (s32)vol;
    if (retvol > 0x3FFF)
        retvol = 0x3FFF;
    if (retvol < -0x4000)
        retvol = -0x4000;

    return retvol;
}

f32 hSndFader2(f32 vol) {
    if (vol == 0.0f)
        return 0.0f;
    if (vol > 1.0f)
        vol = 1.0f;
    vol = sD->dBfader * (1.0f - vol);
    vol = powf(10.0f, vol / 20.0f);
    return vol;
}

void hSndMain() {
    if (sD->TitleDelayCnt != 0 && --sD->TitleDelayCnt == 0) {
        hSeKeyOn(0xc80c80ca0e900, NULL, 0);
        hSeKeyOn(0xc80c80ca0e901, NULL, 0);
    }

    switch (sD->fadeFlag) {
        case 1:
            hSndSetMVol((sD->fadeMax - sD->fadeCnt) / sD->fadeMax);
            sD->fadeCnt += 1.0f;
            if (sD->fadeCnt >= sD->fadeMax) {
                sD->fadeFlag = 2;
                sD->fadeCnt = 0.0f;
                hBgmReset();
                hPptReset();
                hSndPkKeyOffAll();
                hSeInitGrp(0);
                hSndSetMVol(0.0f);
            }
            break;
        case 2:
            sD->fadeCnt += 1.0f;
            if (sD->fadeCnt >= 2.0f) {
                sD->fadeFlag = 0;
                hSndSetMVol(1.0f);
            }
            break;
        case 3:
            hSndSetMVol(sD->fadeCnt / sD->fadeMax);
            sD->fadeCnt += 1.0f;
            if (sD->fadeCnt >= sD->fadeMax) {
                hSndSetMVol(1.0f);
                sD->fadeFlag = 0;
                sD->fadeCnt = 0.0f;
            }
            break;
        case 4:
            sD->seMVol = sD->fadeCnt * 1.43f / sD->fadeMax;
            sD->fadeCnt += 1.0f;
            if (sD->fadeCnt >= sD->fadeMax) {
                sD->seMVol = 1.43f;
                sD->fadeFlag = 0;
                sD->fadeCnt = 0.0f;
            }
            break;
    }

    if (sD->effVol != sD->effVolBak && sD->effChange == 0) {
        if (sD->effVol - sD->effVolBak > 0.01f)
            sD->effVolBak += 0.01f;
        else if (sD->effVol - sD->effVolBak < -0.01f)
            sD->effVolBak -= 0.01f;
        else
            sD->effVolBak = sD->effVol;
        hSndPkSetEVol(sD->effVolBak * 32767.0f);
    }

    if (sD->Mute != 0) {
        hSndPkSetMVol(0,0);
        sD->Mute = 0;
    } else {
        hSndPkSetMVol(sD->MVol, sD->MVol);
    }

    hSndPkGetSize(); // why does this get called lol
    ((s16 *)SndPacket)[0] = sD->PkNum;
    hRpc(IOP_SndMain);
    sD->Pk = SndPacket + 2;
    sD->PkNum = 0;
    sD->VoiceStat[0] |= sD->KeyonV[0];
    sD->VoiceStat[1] |= sD->KeyonV[1];
    sD->KeyonV[0] = 0;
    sD->KeyonV[1] = 0;
    hSeObjMain();
    sD->effChange = 0;
}

void hSndInit() {
    sceSdRemoteInit();
    
    sD = &SndData;
    sD->Pk = &SndPacket[2];
    sD->PkNum = 0;
    sD->iopBankAddr = hRpc(IOP_SndInit);

    RpcArg[0] = 0xFFFFFF;
    RpcArg[1] = 0xFFFF;
    hRpc(IOP_SndMask);
    
    sD->PkMax = 0;
    sD->Stereo = SND_MODE_STEREO;
    sD->Mute = 0;
    sD->stageBank = 0;
    sD->dBfader = -30.0f;
    sD->log10Volume = (f32)log10(16367.0) * 20.0f;
    hSndSetMVol(0.0f);
    hSndPkSetMVol(0, 0);
    sD->effMode = 0;
    sD->effDepth = 0;
    sD->effDelay = 0;
    sD->effFeed = 0;
    sD->effMix = 3;
    hSndPkEffect();
    sD->effVolBak = 0.0f;
    sD->effVol = 0.0f;
    hSndPkSetEVol(0);
    sD->envNum = 0;
    hSeLock(0);
    hSeInitGrp(0);
    sD->seMVol = 1.43f;
    sD->seObjID = 0;
    sD->bgmMVol = 0.708661437f;
    sD->pptMVol = 0.314960629f;
    sD->TitleDelayCnt = 0;

    hStrInit();
}

void hSndBankSet(s32 packaddr, s32 id) {
    s32 hdaddr;
    s32 bdaddr;
    s32 hdsize;
    s32 bdsize;
    s32 transsize;
    s32 tbladdr;
    s32 tblsize;
    char stackShiz[0x10];
    
    if (id != 0) {
        tbladdr = (s32)GetFHMAddress((u32 *)packaddr, 2);
        tblsize = *(s16 *)tbladdr;
        sD->stageTblNum = tblsize;
        memcpy(sD->stageTbl, (void *)(tbladdr + 2), tblsize * 2);
    }

    hdaddr = (s32)GetFHMAddress((u32 *)packaddr, 0);
    bdaddr = (s32)GetFHMAddress((u32 *)packaddr, 1);
    hdsize = JamGetHdSize(hdaddr);
    bdsize = JamGetBdSize(hdaddr);
    hTrans2IOP(sD->iopBankAddr + id * 0x8000, hdaddr, hdsize);
    
    RpcArg[0] = id;
    hRpc(IOP_JamBankSet);
    
    while (bdsize > 0) {
        if (bdsize < 0x10000)
            transsize = bdsize;
        else
            transsize = 0x10000;

        hTrans2IOP(sD->iopBankAddr + 0x10000, bdaddr, transsize);
        bdsize -= 0x10000;
        bdaddr += 0x10000;
        hRpc(IOP_JamBdTrans);
    }
}

void hSndBankSetMain() {
    s32 buff;
    s32 i;

    buff = (s32)getBuff(1, 0x200000, NULL, &i);
    hCdReadDataBlock(198, buff);
    buff = (s32)GetFHMAddress((u32 *)buff, 2);
    hSndBankSet(buff, 0);
    freeBuff(1, 0x200000, NULL);
}

void hSndBankSetStage() {
    s32 addr;

    addr = hGetDataAddr(2);
    if (addr != 0) {
        sD->stageBank = 1;
        hSndBankSet(addr, 1);
    } else {
        sD->stageBank = 0;
    }
}

void hSndEffSetArea() {
    EFXSE *eff;

    eff = &EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF][0];
    sD->effIdx = 0;
    sD->effVol = eff->vol;
    sD->effVolBak = 0.0f;
    sD->effMode = eff->efx;
    sD->effDepth = 0x7fff;
    sD->effDelay = eff->delay;
    sD->effFeed = eff->feed;
    sD->effMix = eff->dry | 2;
    hSndPkEffect();
    hSndPkSetEVol(0);
}

void hSndEffSetVolIdx(s32 idx) {
    EFXSE *eff;

    eff = EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF];
    sD->effIdx = idx;
    eff = &eff[idx];
    sD->effVol = eff->vol;
}

void hSndEffSetVol_PPTstart() {
    EFXSE *eff;

    eff = EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF];
    eff = &eff[sD->effIdx];
    sD->effVol = eff->vol_ppt;
}

void hSndEffSetVol_PPTend() {
    EFXSE *eff;

    eff = EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF];
    eff = &eff[sD->effIdx];
    sD->effVol = eff->vol;
}

void hSndEffSetVol(f32 vol) {
    sD->effVol = vol;
}

void hSndSetStereo(SND_MODE i) {
    sD->Stereo = i;
}
