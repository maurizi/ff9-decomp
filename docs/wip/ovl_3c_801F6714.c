/* STATUS: 66 rows. ovl_3c func_801F6714 (0x368, synth menu twin of func_801F51B8; same fixes apply). Extra problem: &eq (sp+0x20) is hoisted out of the loop and spilled in ours; the original recomputes addiu s0,sp,0x20 inside the loop. */
#include "ovl_3c.h"
s32 func_80028014(s32);
s32 func_80028954(s32);
s32 func_80033A1C(Character *);
extern s32 D_801FC0D0[2];
extern u8 D_801FC0D8[5];
extern u16 D_801FC0E0[];
typedef struct {
    u8 v[5];
} Equip5;
extern s32 D_801F3FFC;

void func_801F6714(void) {
    u8 buf[8];
    Equip5 eq;
    RECT *r;
    MenuList *l;
    Character *c;
    SynthRecipe *s;
    s32 kind;
    u8 armor;
    u8 ok;
    s32 cur;
    s32 new;
    s32 i;
    s32 x;
    s32 y;

    r = &D_801FC338.unk18;
    l = &D_801FC338.unk28;
    s = &D_801FC338.unk3C[l->unk5 + l->unk6];
    if (D_801FC338.unk4 == 0) {
        return;
    }
    func_8002DEE4(r, 0x23, 1);
    kind = func_80028014(s->unk4);
    if (kind < 0) {
        return;
    }
    armor = kind > 0;
    func_8002B524(r->x + 6, r->y - 3, 0x1D, ((u8 **)&D_800703FC)[D_801FC0D0[armor]], 0);
    y = 0x9E;
    for (i = 0; i < D_801FC2D0.unk14; i++) {
        x = 0xF + i * 0x24;
        c = &D_8006794C->unk1C->unk3C[(u8)D_801FC2D0.unk18[i]];
        ok = (D_80069018[s->unk4].unk6 & D_801FC0E0[func_80033A1C(c)]) != 0;
        if (ok) {
            if (!armor) {
                new = D_8006E0E8[s->unk4].unk5;
                cur = D_8006E0E8[c->unk39[0]].unk5;
            } else {
                eq = *(Equip5 *)c->unk39;
                eq.v[kind] = s->unk4;
                cur = func_801F42A8(kind, c->unk39);
                new = func_801F42A8(kind, eq.v);
            }
            if (kind == 4) {
                *(s32 *)buf = D_801F3FFC;
            } else {
                func_8002BAE4(new, buf, 3, 1);
            }
            func_801F44F4(x - 1, y + 0x2E, 0x1E, D_801FC0D8[kind], 0);
            func_8002B56C(x + 0xC, y + 0x2F, 0x1E, buf, 0);
            if (cur < new) {
                func_801F44F4(x - 1, y + 0x27, 0x1E, 5, 0);
            } else if (new < cur) {
                func_801F44F4(x - 1, y + 0x28, 0x1E, 6, 0);
            }
        }
        func_80028204(x, y, 0x1E, func_80028954((u8)D_801FC2D0.unk18[i]), 0x2D, !ok);
    }
}
