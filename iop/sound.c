// *****************************************************************************
// FILE -- /home/hoshino/klonoa2/src/hoshino/iop/src/sound.c
// *****************************************************************************

#include "iop.h"

/* bss 800 */ SNDDATA SndData;
/* bss 10 */ SNDDATA *sD;

/* 000044a0 000044fc */ static int SndDmaInt(/* 0x0(sp) */ void *common) {

}

/* 000044fc 000056f0 */ void SndGetPacket(/* 0x0(sp) */ u_char *pk) {
	/* -0x38(sp) */ SNDVOPRM *vp;
	/* -0x34(sp) */ int i;
	/* -0x30(sp) */ int pnum;
	/* -0x2c(sp) */ int com;
	/* -0x28(sp) */ int mix;
	/* -0x24(sp) */ int core;
	/* -0x20(sp) */ int vnum;
	/* -0x1c(sp) */ int bank;
	/* -0x18(sp) */ int prog;
	/* -0x14(sp) */ int splt;
	/* -0x10(sp) */ int tmp;
}

/* 000056f0 000072ec */ void SndMain(/* 0x0(sp) */ int *data) {
	/* -0x18(sp) */ SNDVOPRM *vp;
	/* -0x14(sp) */ int core;
	/* -0x10(sp) */ int vnum;
}

/* 000072ec 00007434 */ void SndMask(/* 0x0(sp) */ int *data) {
	/* -0x18(sp) */ int core;
	/* -0x14(sp) */ int vnum;
	/* -0x10(sp) */ int mask;
}

/* 00007434 00007ce0 */ void SndInit() {
	/* -0x10(sp) */ int core;
	/* -0xc(sp) */ int vnum;
}
