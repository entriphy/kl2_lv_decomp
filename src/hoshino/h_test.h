#ifndef H_TEST_H
#define H_TEST_H

#include "hoshino.h"

extern s32 hTestBGMskip;
extern s32 TestMode;
extern s32 (*hTestFuncTbl[2])();
extern s32 TestWork;

extern void hTestInit();
extern void hTestBGM();
extern void hTestPPT();
extern void hTestAC3();
extern void hTestConfig();
extern void hTestMov();
extern void hTestMain();

#endif
