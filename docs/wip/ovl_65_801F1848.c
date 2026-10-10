/* STATUS: near miss, 20 rows under the new cmp.py (4 of them are its func_ %hi/%lo false positives; 23 before). ovl_65 func_801F1848 (0x1BC, level-up popup windows). w = D_801F7A78.unk1D8 first, then i = 0, m = &D_801F7A78 (the original derives m as w - 0x1D8), increments ordered w++, i++, r++ (fixes the duplicated i++), and the popup RECT through rr = &D_801F81F8 with .x stored directly. Left: the original hoists %hi(D_801F8200) into s6 and stores .x through a fresh lui (ours hoists %hi(D_801F81F8)), r is built as m + 0xD0 (ours w - 0x108), and m->unk18[i] adds i + m. Statement permutations of the store block and the permuter (15 min) did not fix it. Pass 5: storing D_801F8200 before .x, rr->x, all-direct D_801F81F8 and a late rr all score worse; permuter 20 min (only semantics-breaking outputs).  Pass 11 (new build/search/cmp.py, which also compares symbol names; its rows for %hi/%lo of func_ symbols are false positives): r = &D_801F7A78.unkAC[2].unk4 and the loop test D_801F7A78.unk18[i] (no m variable): CSE now derives the base as w - 0x1D8 in s5 and the test adds i + s5 like the original, and .x is stored through a fresh lui. Left: the original hoists %hi(D_801F8200) into s6 (loop.c movable) and builds r as base + 0xD0; ours keeps the D_801F8200 lui in the loop and r = w - 0x108. */
#include "ovl_65.h"

void func_801F1848(void) {
    WinSlot *w;
    RECT *r;
    s32 i;
    RECT *rr;

    w = D_801F7A78.unk1D8;
    i = 0;
    r = &D_801F7A78.unkAC[2].unk4;
    rr = &D_801F81F8;
    for (; i < 4; w++, i++, r = (RECT *)((u8 *)r + sizeof(WinDef))) {
        if (D_801F7A78.unk18[i] != 0) {
            if (w->unk8 == 0) {
                if (w->unk0 == NULL) {
                    if (w->unk4 != 0) {
                        D_801F81F8.x = r->x + 0x26;
                        D_801F8200 = r;
                        rr->y = r->y + 0x1B;
                        rr->h = 0x19;
                        rr->w = 0x70;
                        w->unk0 = func_8002DC54(rr, 6, 2);
                        w->unk0->unk50 = i;
                        func_8002E304(w->unk0, (s32)D_801F5228);
                        func_8002E2EC(w->unk0, func_801F1D94);
                        func_8002E2FC(w->unk0, func_801F2248);
                        func_8002CF20(w->unk0, 0);
                        func_8002F488(0x413);
                        w->unk4--;
                    }
                } else if (++w->unk1C >= 0x40) {
                    func_8002D1AC(w->unk0, 0);
                    func_8002DD00(w->unk0);
                    w->unk8 = 4;
                    w->unk1C = 0;
                    w->unk6++;
                }
            } else {
                w->unk8--;
            }
        }
    }
}
