#include "h_wkmng.h"
#include "h_menu.h"

static hMNG Mng[4];

void hMngInit(hMNGLIST *pMng, s32 max, s32 id) {
    s32 i;

    Mng[id].Idx = 0;
    Mng[id].Max = max;
    Mng[id].pList = pMng;
    for (i = 0; i < max; i++) {
        pMng[i].Idx = i;
        pMng[i].Busy = 0;
        pMng[i].Next = i + 1;
    }

    hG.light_col[0][0] = hG.light_col[0][1] = hG.light_col[0][2] = hG.light_col[0][3] = 0x80;
    hG.light_col[3][0] = hG.light_col[3][1] = hG.light_col[3][2] = hG.light_col[3][3] = 0x80;
}

s32 hWorkGet(s32 id) {
    hMNGLIST *p;
    s32 idx;

    p = &Mng[id].pList[Mng[id].Idx];
    idx = Mng[id].Idx;
    p->Busy = 1;
    Mng[id].Idx = p->Next;
    return idx;
}

void hWorkDel(s32 id, s32 idx) {
    hMNGLIST *p;

    p = &Mng[id].pList[idx];
    p->Busy = 0;
    p->Next = Mng[id].Idx;
    Mng[id].Idx = idx;
}
