/* STATUS: 1 row off. ovl_65 func_801F2560 (0x13C, add exp and level up). The original has the lui of 0x98967E in the delay slot of the first 'beqz a1'; we get a nop. NOT a maspsx bug: 17 matched functions have the same reorder-mode branch + lui-only li and their originals have the nop, so ASPSX never fills these. gcc's own delay-slot filler (dbr) must have placed it in the original, i.e. the C differs: try the 9999999/9999998 compare and clamp written differently (constant in a local, '>' vs '>=', clamp as a separate if/else) so the branch is emitted in noreorder with the lui as filler. Level-up loop must be if + do-while with 'D_8006DF5C[c->unkB] <= c->unkC' (table loaded first). */
#include "ovl_65.h"

void func_801F2560(s32 i, u32 exp) {
    Character *c = D_8006794C->unk1C->unk54C[i];

    if (exp != 0) {
        if (c->unkC > 9999998) {
            c->unkC = 9999999;
            return;
        }
        D_801F7A78.unk24.unk1C[i].unk8 += exp;
        c->unkC += exp;
        if (c->unkC > 9999998) {
            c->unkC = 9999999;
        }
        if (c->unkB < 99 && D_8006DF5C[c->unkB] <= c->unkC) {
            do {
                func_801F29BC(i);
            } while (c->unkB < 99 && D_8006DF5C[c->unkB] <= c->unkC);
        }
    }
}
