// *****************************************************************************
// FILE -- /home/hoshino/klonoa2/src/hoshino/iop/src/jam.c
// *****************************************************************************

#include "iop.h"

/* data 10 */ static int transpitch[0];
/* bss 7e0 */ JAMDATA JamData;
/* bss c */ JAMDATA *jM;

/* 00003af0 00003b64 */ u_int* GetFHMAddress(/* 0x0(sp) */ u_int *pAddr, /* 0x4(sp) */ int nNum) {
    
}

/* 00003b64 00003bdc */ int JamGetBdSize(/* 0x0(sp) */ int id) {
	/* -0x10(sp) */ JAMHD *pHD;
	/* -0xc(sp) */ CHK_HEAD *pHead;
}

/* 00003bdc 000041c0 */ void JamGetPrm(/* 0x0(sp) */ int id, /* 0x4(sp) */ int prog, /* 0x8(sp) */ int splt, /* 0xc(sp) */ SNDKEYPRM *prm) {
	/* -0x50(sp) */ JAMHD *pHD;
	/* -0x4c(sp) */ CHK_HEAD *pHead;
	/* -0x48(sp) */ CHK_PROG *pProg;
	/* -0x44(sp) */ CHK_SSET *pSSet;
	/* -0x40(sp) */ CHK_SMPL *pSmpl;
	/* -0x3c(sp) */ CHK_VINFO *pVInfo;
	/* -0x38(sp) */ PROG_PRMBLK *pProgPrm;
	/* -0x34(sp) */ SPLT_PRMBLK *pSpltPrm;
	/* -0x30(sp) */ SSET_PRMBLK *pSSetPrm;
	/* -0x2c(sp) */ SMPL_PRMBLK *pSmplPrm;
	/* -0x28(sp) */ VINFO_PRMBLK *pVInfoPrm;
	/* -0x24(sp) */ u32 addr;
	/* -0x20(sp) */ u32 *ptr;
	/* -0x1c(sp) */ u16 *ptr2;
	/* -0x18(sp) */ s32 vol;
}

/* 000041c0 00004320 */ void JamBankSet(/* 0x0(sp) */ int id) {

}

/* 00004320 00004458 */ void JamBdTrans() {
	/* -0x10(sp) */ int size;
}

/* 00004458 00004498 */ void JamInit() {

}