/* STATUS: near miss, 62 rows (same function as ovl_39 func_801F7FD4: one solution gives both). ovl_34 func_801EFC20 (0x338, slider gauge: three LINE_F2 rows, two end-cap sprites, a DR_TWIN-repeated middle sprite and a DR_TPAGE). Logic and stores right; left: register pressure. The original keeps x/y/otz*4/w+8/&D_80068988/&D_80068990 and the two OT masks in s0-s7/fp and spills the CSEd &D_8007716C and &D_80068998 pseudos (rematerialised through t6 at each use); ours spills ic/mid or w+8 instead. Permuter (25 min) found nothing better. */
#include "ovl_34.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u16 unk2;
    u8 unk4;
    u8 unk5;
    u8 pad6[2];
} IconT;
extern IconT D_80068988[2];
extern IconT D_80068998;
extern u32 D_801F154C[3];
extern RECT D_801F1544;
void SetTexWindow(void *p, RECT *r);

typedef struct {
    u32 tag;
    u32 rgbc;
    u32 xy0;
    u32 xy1;
} LineWords;

void func_801EFC20(s32 x, s32 y, s32 otz, s32 pos, s32 w) {
    IconT *ic;
    IconT *mid;
    SprtWords *p;
    LineWords *l;
    RECT r;
    s32 i;

    ic = D_80068988;
    mid = &ic[1];
    w += 8;
    if (pos != 0) {
        for (i = 0; i < 3; i++) {
            s32 yy = (y + i + 2) << 16;
            setlen(D_80077170, 3);
            l = (LineWords *)D_80077170;
            l->rgbc = D_801F154C[i];
            l->xy0 = (x + 4) | yy;
            l->xy1 = (x + pos + 3) | yy;
            addPrim(&D_8007716C[otz], l);
            D_80077170 = (u8 *)(l + 1);
        }
    }
    func_80027710(x, y, otz, ic, 0);
    func_80027710(x + w - D_80068998.unk4, y, otz, &D_80068998, 0);
    SetTexWindow(D_80077170, &D_801F1544);
    addPrim(&D_8007716C[otz], D_80077170);
    D_80077170 += 0xC;
    setlen(D_80077170, 4);
    p = (SprtWords *)D_80077170;
    p->rgbc = 0x64808080;
    p->xy = ((u16)(x + ic->unk4)) | (y << 16);
    p->uvclut = mid->unk2 << 16;
    p->wh = (w - (ic->unk4 + D_80068998.unk4)) | (mid->unk5 << 16);
    addPrim(&D_8007716C[otz], p);
    D_80077170 = (u8 *)(p + 1);
    r.x = mid->unk0;
    r.y = mid->unk1;
    r.w = mid->unk4;
    r.h = mid->unk5;
    SetTexWindow(D_80077170, &r);
    addPrim(&D_8007716C[otz], D_80077170);
    D_80077170 += 0xC;
    SetDrawTPage(D_80077170, 1, 1, 0x1F);
    addPrim(&D_8007716C[otz], D_80077170);
    D_80077170 += 8;
}
