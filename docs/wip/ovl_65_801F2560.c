/* STATUS: C is believed right, 1 row (assembler). ovl_65 func_801F2560 (0x13C, add exp and level up). gcc leaves the first 'beq a1,zero' unfilled in .set reorder mode and splits li 0x98967E into lui+ori; the original has the lui in that delay slot (ASPSX filled it), maspsx emits a nop. Needs maspsx to fill an empty reorder-mode branch delay slot with a following lui. Level-up loop must be if + do-while with 'D_8006DF5C[c->unkB] <= c->unkC' (table loaded first). */
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
