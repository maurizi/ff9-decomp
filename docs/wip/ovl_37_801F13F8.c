/* STATUS: near miss, 25 rows (pass 16; was 40, 44). ovl_37 func_801F13F8 (0x380, items menu cursor). Left: blink lives in s2 and &D_801FAB78 in s3 (original: the reverse); the state>=2 block loads unk1 through a fresh &D_801FAB78 pointer and compares against 2 in t0; xor/cursor blocks differ only in v0/v1 naming. Store order inside each D_80077DE8 block found with build/perm.py; (s16) cast on the unk3C.y offset keeps the add unfolded. The permuter (~8k iterations) found nothing.  Pass 11 (new build/search/cmp.py, which also compares symbol names; its rows for %hi/%lo of func_ symbols are false positives): both xor blocks go through k (`k = l->unk7 ^ ...; unk0 = k * 0x96;`), 44 -> 40. The original computes &D_801FAB78 into a register (hi part in a1, lui in the bne delay slot) at the top of the state>=2 block and compares against 2 kept in t0; a permuter run (25 min, 505 -> 365) got there only with an extra D_801FAB78 field test around duplicated arms, i.e. the block reads D_801FAB78 more than once in the original. dup.py `if (blink)` around the last inner call: 27 rows (fake). Nested ifs, a block-local pointer, a k temp for unk1: no help.
 Pass 14: a pointer local for the state>=2 test, a duplicated (typo-style) unk1 test in the condition, and
 nested ifs give 40-44 rows.
 Pass 16: 40 -> 27 with the cursor body written twice (`if (unk1 >= 2 && unk1 != 4) { if (unk1 == 2) BODY
 else if (blink) BODY }`): jump2 cross-jumps the two copies after allocation, which is what the permuter's
 fake `if (&D_801FAB78) A else A` stood in for. Left: the test block still loads unk1 via %lo(D_801FAB78+1)
 where the original materialises &D_801FAB78 in v0 (hi in a1) and keeps the constant 2 in t0, i.e. both
 live longer there (probably another D_801FAB78 read or == 2 test in one of the copies); the xor result
 (k) is in a1 in both xor blocks; 223 vs 224 insns. `(s16)(off + 0x32)` (like block 3's cast) gives the
 original's off + 0x32 first (27 -> 25). */
#include "ovl_37.h"

extern s32 D_80077E54;

void func_801F13F8(void) {
    MenuList *l;
    s32 off;
    u8 blink;
    s32 a;
    s32 k;
    u8 u;

    off = 0;
    l = D_801F3040[D_801FAB78.unk0].unk8;
    blink = (D_80077E54 & 3) != 0;
    func_80035674(D_801FAB78.unk1 != 1);
    if (func_8002F810() || D_801FAB78.unk1 == 0) {
        return;
    }
    if (D_801FAB78.unk1 == 1 || blink) {
        D_80077DE8.unk0 = D_801F30AC[D_801FAB78.unk0] - 0x10;
        D_80077DE8.unk2 = 0x10;
        D_80077DE8.unk4 = 0x19;
        func_800355E4();
    }
    if (D_801FAB78.unk1 == 4 || (D_801FAB78.unk1 == 5 && blink)) {
        D_80077DE8.unk0 = 0x7E;
        D_80077DE8.unk4 = 0x12;
        D_80077DE8.unk2 = D_801FAB78.unk5 * 15 + 0xE;
        func_800355E4();
    }
    if (D_801FAB78.unk1 >= 2 && D_801FAB78.unk1 != 4) {
        if (D_801FAB78.unk1 == 2) {
            k = l->unk7 ^ ((l->unk8 >> 3) & 1);
            D_80077DE8.unk0 = k * 0x96;
            D_80077DE8.unk4 = 0x19;
            D_80077DE8.unk2 = l->unk6 * 15 + 0x34;
            func_800355E4();
        } else if (blink) {
            k = l->unk7 ^ ((l->unk8 >> 3) & 1);
            D_80077DE8.unk0 = k * 0x96;
            D_80077DE8.unk4 = 0x19;
            D_80077DE8.unk2 = l->unk6 * 15 + 0x34;
            func_800355E4();
        }
    }
    if (D_801FAB78.unk1 == 3) {
        D_80077DE8.unk4 = 0x12;
        D_80077DE8.unk0 = D_801FAB78.unk3C.unk4.x - 0xA;
        D_80077DE8.unk2 = D_801FAB78.unk3C.unk4.y + (s16)(D_801FAB78.unk2 * 43 + 0x12);
        func_800355E4();
    }
    if (D_801FAB78.unk1 == 5) {
        if (D_801FAB78.unk7 == 1 && blink && !(l->unk8 & 4)) {
            a = l->unk5;
            if (D_801FAB78.unk6 >= a * 2 && D_801FAB78.unk6 < (a + 0xB) * 2) {
                if (l->unk8 != 0) {
                    off = l->unk9;
                    if (!(l->unk8 & 2)) {
                        off = -off;
                    }
                }
                u = D_801FAB78.unk6;
                D_80077DE8.unk0 = (u & 1) * 0x96 + 2;
                D_80077DE8.unk2 = ((u >> 1) - l->unk5) * 15 + (s16)(off + 0x32);
                D_80077DE8.unk4 = 0x12;
                func_800355E4();
            }
        }
        k = l->unk7 ^ ((l->unk8 >> 3) & 1);
        D_80077DE8.unk0 = k * 0x96;
        D_80077DE8.unk2 = l->unk6 * 15 + 0x34;
        D_80077DE8.unk4 = 0x12;
        func_800355E4();
    }
}
