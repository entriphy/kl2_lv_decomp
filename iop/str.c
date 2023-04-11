// *****************************************************************************
// FILE -- /home/hoshino/klonoa2/src/hoshino/iop/src/str.c
// *****************************************************************************

#include "iop.h"

/* bss 600 */ STRDATA StrData;
/* bss 8 */ STRDATA *pSTR;
/* bss 750 */ BGMDATA BGMdata[2];
/* bss 0 */ BGMDATA *pBGM;
/* bss 4 */ BGMDATA *pBGM2;
/* bss 680 */ PPTDATA PPTdata[4];

/* 000000d0 0000014c */ void MarkTop(/* 0x0(sp) */ int a, /* 0x4(sp) */ int s) {

}

/* 0000014c 000001c8 */ void MarkBottom(/* 0x0(sp) */ int a, /* 0x4(sp) */ int s) {

}

/* 000001c8 00000214 */ static void* SpuInt() {

}

/* 00000214 0000027c */ void StrDebug() {
	/* bss 0 */ static u_char buff[512];
}

/* 0000027c 00000588 */ void StrKick() {
	/* -0x10(sp) */ int i;
	/* -0xc(sp) */ int st;
}

/* 00000588 00000fb4 */ void StrInfo(/* 0x0(sp) */ int *data) {
	/* -0x18(sp) */ STRINFO *info;
	/* -0x14(sp) */ STRINFO2 *info2;
	/* -0x10(sp) */ int i;
}

/* 00000fb4 00001218 */ void StrCross() {
	/* -0x18(sp) */ BGMDATA *p;
	/* -0x14(sp) */ u_int cnt;
}

/* 00001218 00002608 */ int StrMain(/* 0x0(sp) */ int status) {
	/* -0x20(sp) */ BGMDATA *bgm;
	/* -0x1c(sp) */ PPTDATA *ppt;
	/* -0x18(sp) */ int st;
	/* -0x14(sp) */ int ch;
	/* -0x10(sp) */ int addr;
}

/* 00002608 0000264c */ int StrThread(/* 0x0(sp) */ int status) {

}

/* 0000264c 00003820 */ void StrInit() {
	/* -0x40(sp) */ struct SemaParam sem;
	/* -0x30(sp) */ struct ThreadParam param;
	/* -0x18(sp) */ int i;
	/* -0x14(sp) */ int st;
	/* -0x10(sp) */ char *c;
}

/* 00003820 000038d0 */ void PptStop(/* 0x0(sp) */ int st) {

}

/* 000038d0 00003938 */ void Ac3Clear() {

}

/* 00003938 00003958 */ void Ac3Play() {

}

/* 00003958 000039c0 */ void Ac3ZeroPcmPlay() {

}

/* 000039c0 00003a00 */ void Ac3ZeroPcmStop() {

}

/* 00003a00 00003a48 */ void Ac3SetDigitalOut() {

}

/* 00003a48 00003a90 */ void Ac3Play2() {

}

/* 00003a90 00003ae8 */ void Ac3Stop() {

}
