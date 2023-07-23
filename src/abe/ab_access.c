#include "ab_access.h"

OBJWORK* abGetKlonoa(s32 player) {
    return GameGbl.klonoa;
}

OBJWORK* abGetMochifuku(s32 player) {
    return ((HERO_WORK *)GameGbl.klonoa->work)->mochifuku;
}
