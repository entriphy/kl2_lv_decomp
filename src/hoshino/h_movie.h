#ifndef H_MOVIE_H
#define H_MOVIE_H

#include "hoshino.h"
#include <libcdvd.h>

typedef struct {
    sceCdlFILE file;
} hMOVDATA; // ?

#ifdef __cplusplus
extern "C" {
#endif

extern void hMovInit();
extern s32 hMov_002da2b0();

#ifdef __cplusplus
}
#endif

#endif
