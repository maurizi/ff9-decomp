/* WIP func_800ED02C: 4 differing rows (build/cmp.py-style aligned diff). New in pass 7 (window title bar: left/right SPRT, DR_TWIN, middle SPRT, DR_MODE, then func_8002709C). Left: the first SPRT's uvclut temp is a0 in the original (v0 here); d1/d2 placement, operand order and clut type don't change it; 7 min of permuter found nothing. */
#include "ovl_0b.h"

extern Unk800689B0 D_80068E78[3];
extern RECT D_800F8E10;
s32 func_80027480(s32);
void SetTexWindow(void *, RECT *);

void func_800ED02C(RECT *r, s32 otz) {
    u32 *ot;
    u32 clut;
    Unk800689B0 *d2;
    Unk800689B0 *d1;
    RECT tw;
    RECT r2;

    ot = &D_8007716C[otz];
    clut = func_80027480(D_80068E78[0].unk2) << 16;
    d2 = &D_80068E78[2];
    setlen(D_80077170, 4);
    d1 = &D_80068E78[1];
    ((SprtWords *)D_80077170)->rgbc = 0x64808080;
    ((SprtWords *)D_80077170)->xy = r->x | (r->y << 16);
    ((SprtWords *)D_80077170)->uvclut = D_80068E78[0].unk0 | (D_80068E78[0].unk1 << 8) | clut;
    ((SprtWords *)D_80077170)->wh = D_80068E78[0].unk4 | (D_80068E78[0].unk5 << 16);
    addPrim(ot, D_80077170);
    D_80077170 += sizeof(SprtWords);

    setlen(D_80077170, 4);
    ((SprtWords *)D_80077170)->rgbc = 0x64808080;
    ((SprtWords *)D_80077170)->xy = (r->x + r->w - d2->unk4) | (r->y << 16);
    ((SprtWords *)D_80077170)->uvclut = D_80068E78[2].unk0 | (d2->unk1 << 8) | clut;
    ((SprtWords *)D_80077170)->wh = d2->unk4 | (d2->unk5 << 16);
    addPrim(ot, D_80077170);
    D_80077170 += sizeof(SprtWords);

    tw.y = 0;
    tw.h = 0xFF;
    tw.x = D_80068E78[1].unk0;
    tw.w = d1->unk4;
    SetTexWindow(D_80077170, &D_800F8E10);
    addPrim(ot, D_80077170);
    D_80077170 += 0xC;

    setlen(D_80077170, 4);
    ((SprtWords *)D_80077170)->rgbc = 0x64808080;
    ((SprtWords *)D_80077170)->xy = (r->x + D_80068E78[0].unk4) | (r->y << 16);
    ((SprtWords *)D_80077170)->uvclut = (d1->unk1 << 8) | clut;
    ((SprtWords *)D_80077170)->wh = (r->w - (D_80068E78[0].unk4 + d2->unk4)) | (d1->unk5 << 16);
    addPrim(ot, D_80077170);
    D_80077170 += sizeof(SprtWords);

    setlen(D_80077170, 2);
    ((DrMode2Words *)D_80077170)->code[0] = get_mode(1, 1, d1->unk6);
    ((DrMode2Words *)D_80077170)->code[1] = get_tw(&tw);
    addPrim(ot, D_80077170);
    D_80077170 += sizeof(DrMode2Words);

    r2.x = r->x + 1;
    r2.y = r->y + 1;
    r2.w = r->w - 3;
    r2.h = r->h - 3;
    func_8002709C(&r2, otz, 0, 0, (D_8006794C->unk24 >> 18) & 1);
}
