/* 9 rows differ: scheduling only. The first POLY_G4's stores and the lui of D_8006794C are ordered differently. 5040 permutations of the 7 stores bottom out at 9. */
#include "ff9.h"

void func_80024BD8(void) {
    s32 w;
    RECT tw;
    PolyG4Words *p;

    w = D_8006794C->unkC->disp.w;
    setlen(D_80077170, 8);
    p = (PolyG4Words *)D_80077170;
    p->rgb2 = 0x505050;
    p->rgb3 = 0x505050;
    p->xy1 = w;
    p->rgb1 = 0;
    p->xy0 = 0;
    p->xy2 = 0x800000;
    p->rgbc0 = 0x3A000000;
    w |= 0x800000;
    p->xy3 = w;
    addPrim(&D_8007716C[0xFFF], p);
    D_80077170 += sizeof(PolyG4Words);

    setlen(D_80077170, 1);
    ((DrTpageWords *)D_80077170)->code = 0xE1000640;
    addPrim(&D_8007716C[0xFFF], D_80077170);
    D_80077170 += sizeof(DrTpageWords);

    tw.x = D_80068DC0.unk0;
    tw.y = D_80068DC0.unk1;
    tw.w = D_80068DC0.unk4;
    tw.h = D_80068DC0.unk5;

    setlen(D_80077170, 2);
    ((DrMode2Words *)D_80077170)->code[0] = get_tw(&D_80068F40);
    ((DrMode2Words *)D_80077170)->code[1] = 0;
    addPrim(&D_8007716C[0xFFF], D_80077170);
    D_80077170 += sizeof(DrMode2Words);

    setlen(D_80077170, 4);
    ((SprtWords *)D_80077170)->rgbc = 0x64808080;
    ((SprtWords *)D_80077170)->xy = 0;
    ((SprtWords *)D_80077170)->uvclut = D_80068DC0.unk2 << 16;
    ((SprtWords *)D_80077170)->wh = w;
    addPrim(&D_8007716C[0xFFF], D_80077170);
    D_80077170 += sizeof(SprtWords);

    setlen(D_80077170, 2);
    ((DrMode2Words *)D_80077170)->code[0] = get_mode(1, 1, D_80068DC0.unk6);
    ((DrMode2Words *)D_80077170)->code[1] = get_tw(&tw);
    addPrim(&D_8007716C[0xFFF], D_80077170);
    D_80077170 += sizeof(DrMode2Words);
}
