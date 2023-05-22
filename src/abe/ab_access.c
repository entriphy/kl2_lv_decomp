#include "common.h"

OBJWORK * abGetKlonoa(int player) {
    return GameGbl.klonoa;
}

OBJWORK * abGetMochifuku(int player) {
    return ((HERO_WORK *)GameGbl.klonoa->work)->mochifuku;
}
