#ifndef READFILE_H
#define READFILE_H

#include "nakano.h"

extern s32 read_file(char *name, char *buff);
extern qword* read_file128(char *name, qword *buff);

#endif
