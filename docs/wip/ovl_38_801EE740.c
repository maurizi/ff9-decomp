/* STATUS: near miss, 6 rows (was 17). ovl_38 func_801EE740 (0x2A4, status window: row label list). y as a giv (y = i * 15 + 6) fixes the loop registers. Left: the original keeps the table base sp+0x20 in s4 (lhu 0(v0) after addu s4) and rebuilds sp+0x260 for the pool each iteration; gcc 2.8.1 folds the table offset into the load and hoists the pool address. A tp = t.v pointer gets the table right but still hoists the pool (13 rows); a goto loop stops all hoisting (27 rows). */
#include "ovl_38.h"
typedef struct {
    StatusEntry v[48];
} StatusTable;
typedef struct {
    u8 s[0x11E];
} StatusPool;
extern StatusTable D_801EDF04;
extern StatusPool D_801EE144;

void func_801EE740(void) {
    RECT r;
    StatusTable t;
    StatusPool pool;
    RECT *w;
    Character *c;
    s32 i;
    s32 y;
    s32 dy;
    s32 k;

    w = &D_801F120C.unk8;
    c = D_8006794C->unk1C->unk54C[D_801F120C.unk2];
    t = D_801EDF04;
    pool = D_801EE144;
    r.w = 0x42;
    r.h = 0x48;
    r.x = w->x + 0xE4;
    r.y = w->y + 8;
    D_801EFBB4[1] = D_801EFBB8[c->unk34.b.row][0];
    D_801EFBB4[2] = D_801EFBB8[c->unk34.b.row][1];
    for (i = 0; i < 4; i++) {
        y = i * 15 + 6;
        D_801F120C.unk7C = func_8002920C(r.x + 0xC, r.y + y, D_8007716C + 25, pool.s + t.v[D_801EFBB4[i]].unk0, 0, D_801F120C.unk7C);
    }
    func_8002B524(r.x + 6, r.y - 3, 0x19, D_8007042C, 0);
    func_8002DEE4(&r, 0x19, 2);
    func_8002705C(&r, 0x19, 2);
}
