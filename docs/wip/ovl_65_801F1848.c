/* STATUS: near miss, 35 rows. ovl_65 func_801F1848 (0x1BC, level-up popup windows). Pointer-increment loop with the unk8 countdown in the else branch gets the shape; left: the original hoists %hi(D_801F8200) into s6 (ours hoists %hi(D_801F81F8)), keeps &w->unk8 as a second giv (s0 = w + 8), and has one shared i++ (ours duplicates it in one branch). */
#include "ovl_65.h"

void func_801F1848(void) {
    WinSlot *w;
    RECT *r;
    s32 i;

    for (i = 0, w = D_801F7A78.unk1D8, r = &D_801F7A78.unkAC[2].unk4; i < 4; i++, w++, r = (RECT *)((u8 *)r + sizeof(WinDef))) {
        if (D_801F7A78.unk18[i] != 0) {
            if (w->unk8 == 0) {
                if (w->unk0 == NULL) {
                    if (w->unk4 != 0) {
                        D_801F8200 = r;
                        D_801F81F8.x = r->x + 0x26;
                        D_801F81F8.w = 0x70;
                        D_801F81F8.h = 0x19;
                        D_801F81F8.y = r->y + 0x1B;
                        w->unk0 = func_8002DC54(&D_801F81F8, 6, 2);
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
