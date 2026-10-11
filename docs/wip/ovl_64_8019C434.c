/* STATUS: byte-identical but needs a dead block-local `last` assignment, not integrated (pass 20; was 36, 54, 89). ovl_64 func_8019C434 (0x3B0, CdReadyCallback of the FMV stream:
 header check, ring bookkeeping, next sector read). The two timeout blocks are cross-jumped in the original (keep both
 copies in C).
 Pass 6 fixes: the signature is (u8 status, u8 *result) as in ovl_64.h (callers pass two args); `prev` is an
 if/else (`if (unk68 != 0) prev = unk68 - 1; else prev = unk6C - 1;`), which ends cse's extended block at the join
 so &D_8019DE10 is rematerialised there like the original (the plain `prev = ...; if (== 0) prev = ...` form lets
 cse skip the block and keep the old base in s0); the slot address is computed into `addr` before the
 D_801A07F0/D_801A06F0/D_8019DE0C stores (the original loads unk68/unk5C/unk64/unk78 above them); and
 `unk64 - (unk5C - 1)` needs the `- 1` hidden from fold (fold-const.c associate: split_tree rewrites
 `a - (b - 1)` as `(a + 1) - b`), here via `last = unk5C - 1` inside the expression.
 Left: (1) s0/s1 swapped for the &D_8019DE10 base and the timeout limit in the first blocks; (2) `last` gets its own
 register (t2/t2) where the original decrements the unk5C register in place after the mult (a3 = a3 - 1), so the
 operand order/scheduling of the address block differs; (3) the constant 1 for `D_8019DEA4 = 1` is kept in s1 and
 reused for `unk1 = 1` (the original rematerialises it).
 Pass 15: the two cross-jumped timeout blocks are one static inline (`retry()`, called twice); that fixes (1) and
 (3) (54 -> 36). Left: only the two address blocks' scheduling: the original loads unk68, D_8019DE0C, unk5C, unk64,
 unk78, decrements the unk5C register in place after the mult, and stores D_8019DE0C++ before the D_801A07F0 store.
 An in-place `last = unk5C; ... (last -= 1)` / `sect = unk68 * last; last -= 1;` (8 spellings) gets the in-place
 addiu but costs scheduling (44-63 rows).
 Pass 16: the second address block's in-place `unk5C - 1` (addiu a3,a3,-1 after the mult) comes from a
 block-local variable: `(last2 = D_8019DE10.unk5C - 1)` with its own last2 instead of the shared `last`
 (a variable set in both blocks is global, so local-alloc cannot tie it to the dying unk5C register).
 That block then differs only in unk64/unk78 (unk78 loads early into a0 here, late into v1 there), but
 the first block gets worse, 39 rows overall (build/w/c4a.c in the pass-16 worktree). Permuter on that
 base: 34-35 rows only with `(last2 = D_8019DE10.unk68) * unk5C` (unk68 read into the shared variable),
 not used. Per-block `addr` variables, a `cur = unk68` local and `D_801A06F0[D_8019DE0C++]` do not help.
 Pass 19: the original stores D_8019DE0C + 1 before the D_801A07F0 store (both non-struct, so their order is the
 source order) and indexes both arrays with the old value: `r = D_8019DE0C++; D_801A07F0[r] = ...; D_801A06F0[r]
 = prev;` (before or after the addr statement, or with the address in the call) is 51-52 rows, because sched1 then
 issues the store before the unk68/unk5C loads. Block-local last/addr in either block (16 combinations) and
 per-assignment variable splits: 36-39.

 Pass 20: BYTE-IDENTICAL (make verify 20/20 with it integrated), but NOT integrated because it needs a dead
 assignment: each address block computes `unk5C - 1` into its own block-local `last` inside the expression,
 `sect = unk68 * unk5C - (unk64 - (last = unk5C - 1));`, and `last` is never read. That assignment does two
 things: fold cannot reassociate `unk64 - (unk5C - 1)` into `(unk64 + 1) - unk5C`, and the block-local pseudo is
 tied in place to the dying unk5C register (addiu a3,a3,-1). Without it: casts ((s32), (u32), (u16)) do not stop
 fold (8 shape rows), a separate `last = unk5C - 1;` statement before or after the mult is 10-26 rows, an inline
 `last_sect()` is 10 (preexpand_calls moves the call first), and inlining the whole address is 24+.
 The other two pieces are clean: a separate `sect` statement (shape 22 -> 14) and `addr = unk78 + sect * 0x8F4;
 addr -= 0x20;` (unk78 then loads late, right before the add, as in the original). The 4 rows cmp_shape still
 prints are symbol-name false positives (%hi(D_8019DE68), %hi(D_8019DE24) = D_8019DE10 + 0x58 / + 0x14).
 To integrate (if the dead `last` is acceptable): move func_8019C434_retry, D_801A07F0/D_801A06F0 and the
 func_80022F64 / DecDCToutSync prototypes below into include/ovl_64.h, then tools/integrate.py this file. */
#include "ovl_64.h"
void func_80022F64(s32);
s32 DecDCToutSync(s32);
extern s32 D_801A07F0[];
extern s32 D_801A06F0[];

static inline void retry(void) {
    s32 limit;

    limit = 0xB4;
    if (D_8019DE10.unk58 == 1) {
        limit = 0x438;
    }
    if (VSync(-1) - D_8019DE10.unk14 < limit) {
        func_80022F64(D_8019DE10.unk44);
    } else {
        D_8019DE10.unk2 = 2;
        func_80023018();
    }
}

void func_8019C434(u8 status, u8 *result) {
    u8 hdr[12];
    s32 limit;
    s32 r;
    s32 prev;
    s32 sect;
    u8 *addr;
    Ovl64RingHdr *h;

    if (D_8019DEA4 != 0) {
        return;
    }
    if (status == 5 || status == 0) {
        retry();
        return;
    }
    if (D_8019DEA8 == 0 && D_8019DE10.unk58 >= 2) {
        r = DecDCToutSync(1);
        if (r == 1) {
            D_8019DEA8 = r;
            return;
        }
    }
    func_800213C0(hdr, 3);
    if (CdPosToInt((CdlLOC *)hdr) != D_8019DE10.unk44) {
        retry();
        return;
    }
    if (D_8019DE10.unk18 < 0) {
        if (hdr[5] == hdr[9]) {
            D_8019DE10.unk18 = hdr[5];
        }
    } else if (hdr[5] == hdr[9] && D_8019DE10.unk18 != hdr[5]) {
        D_8019DE10.unk2 = 2;
        func_80023018();
    }
    D_8019DEA4 = 1;
    D_8019DE0C = 0;
    if (D_8019DE10.unk4C < 2) {
        func_800213C0(D_8019DE10.unk90 + (D_8019DE10.unk80 << 11), 0x200);
        D_800761D0.unkC += 0x800;
        func_8019BCE8();
    } else {
        if (D_8019DE10.unk64 == 0) {
            if (D_8019DE10.unk68 != 0) {
                prev = D_8019DE10.unk68 - 1;
            } else {
                prev = D_8019DE10.unk6C - 1;
            }
            h = &D_8019DE10.unk74[prev];
            if (!(h->unk0 & 0x1000)) {
                {
                    u32 last;
                    sect = D_8019DE10.unk68 * D_8019DE10.unk5C - (D_8019DE10.unk64 - (last = D_8019DE10.unk5C - 1));
                }
                addr = D_8019DE10.unk78 + sect * 0x8F4;
                addr -= 0x20;
                D_801A07F0[D_8019DE0C] = D_8019DE10.unk58;
                D_801A06F0[D_8019DE0C] = prev;
                D_8019DE0C++;
                func_800213C0(addr, 0x245);
                D_800761D0.unkC += 0x800;
                func_80023018();
                func_8019BA70();
                D_8019DE10.unk1 = 1;
                goto end;
            }
            h->unk0 = (h->unk0 & 0xFFF) | 0x6000;
        }
        {
            u32 last;
            sect = D_8019DE10.unk68 * D_8019DE10.unk5C - (D_8019DE10.unk64 - (last = D_8019DE10.unk5C - 1));
        }
        addr = D_8019DE10.unk78 + sect * 0x8F4;
        addr -= 0x20;
        func_800213C0(addr, 0x245);
        D_800761D0.unkC += 0x800;
        func_8019BA70();
    }
    D_8019DE10.unk14 = VSync(-1);
end:
    D_8019DEA4 = 0;
}
