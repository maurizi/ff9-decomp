/* STATUS: near miss, 25 rows, register allocation only. ovl_3c func_801F6D34 (0x1CC, synth list row). The ingredient loop is not optimised by loop.c in the original (no strength reduction of &r->unk2[i], 0xFF not hoisted), which a goto loop reproduces; a real for loop is 40 rows. Left: s1/s2/s3 hold ok/r/row in the original but r/row/ok here. */
#include "ovl_3c.h"

void func_801F6D34(s32 unused, s32 row, s32 idx, MenuView *v) {
    u8 buf[16];
    SynthRecipe *r;
    Unk80069018 *it;
    s32 i;
    s32 t;
    s32 x;
    s32 y;
    u8 ok;
    u8 id;

    r = &D_801FC338.unk3C[idx];
    ok = 0;
    id = r->unk4;
    it = &D_80069018[id];
    if (func_80027F34(id) < 99) {
        ok = D_8006794C->unk1C->unk560 >= r->unk0;
    }
    x = 0x18;
    y = 0x26;
    y = row * 15 + (v->unk2 + y);
    i = 0;
loop:
    if (r->unk2[i] != 0xFF) {
        if (r->unk2[0] == r->unk2[1]) {
            t = i < func_80027F34(r->unk2[i]);
        } else {
            t = func_80027F34(r->unk2[i]);
        }
        if (!t) {
            ok = 0;
        }
    }
    i++;
    if (i < 2) {
        goto loop;
    }
    func_8002BAE4(r->unk0, buf, 5, 1);
    func_80026C34(x, y, 0x1E, r->unk4, ok);
    func_8002B5FC(x + 0x10, y, 0x1E, D_8006A218 + it->unk0, ok ? 0 : 2);
    func_8002B56C(x + 0x72, y + 4, 0x1E, buf, ok ? 0 : 2);
}
