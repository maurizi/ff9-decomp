/* STATUS: logic right, 54 rows (pass 6; was 89). ovl_64 func_8019C434 (0x3B0, CdReadyCallback of the FMV stream:
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
 reused for `unk1 = 1` (the original rematerialises it). */
#include "ovl_64.h"
void func_80022F64(s32);
s32 DecDCToutSync(s32);
extern s32 D_801A07F0[];
extern s32 D_801A06F0[];

void func_8019C434(u8 status, u8 *result) {
    u8 hdr[12];
    s32 limit;
    s32 r;
    s32 prev;
    s32 sect;
    s32 last;
    u8 *addr;
    Ovl64RingHdr *h;

    if (D_8019DEA4 != 0) {
        return;
    }
    if (status == 5 || status == 0) {
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
                                addr = D_8019DE10.unk78 + (D_8019DE10.unk68 * D_8019DE10.unk5C - (D_8019DE10.unk64 - (last = D_8019DE10.unk5C - 1))) * 0x8F4 - 0x20;
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
                addr = D_8019DE10.unk78 + (D_8019DE10.unk68 * D_8019DE10.unk5C - (D_8019DE10.unk64 - (last = D_8019DE10.unk5C - 1))) * 0x8F4 - 0x20;
        func_800213C0(addr, 0x245);
        D_800761D0.unkC += 0x800;
        func_8019BA70();
    }
    D_8019DE10.unk14 = VSync(-1);
end:
    D_8019DEA4 = 0;
}
