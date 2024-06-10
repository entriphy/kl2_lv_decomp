#ifndef OKSTRING_H
#define OKSTRING_H

#include "okanoyo.h"

extern char* getline(char *line, char *buff);
extern char* findword(char *word, char *buff);
extern void stripstr(char *obst, char *word);
extern void replacestr(char *obst, char *inst, char *word);
extern char* getlineword(char *word, char *line);
extern char* getlineword2(char *word, char *line);
extern s32 htoi(char *str);
extern s32 stoi(char *str);

#endif
