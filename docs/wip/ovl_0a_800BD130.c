/*
 * ovl_0a func_800BD130 (0x270 bytes): builds the two (double-buffered) sprite lists of a background
 * layer: per tile a SPRT (with the semi-trans bit from the tile) plus a DR_TPAGE merged with
 * MargePrim, tracking the highest CLUT row in D_800CA108.
 * WIP (pass 10): 156 of 156 instructions, 12 aligned diff rows (was 48).
 * Fixed: the tpage call is made in both arms of `if (tile[i].tp == 0) ... GetTPage(0, ...) else
 * GetTPage(1, ...)` with a u16 result (jump2 cross-jumps the identical argument setup, leaving the
 * original's branch for the 0/1 instead of a store-flag); the outer loop is a for loop; prologue
 * statement order w, pos, tile, unk18, unk1A, n, unk25 (brute force).
 * Left: prologue scheduling (the original loads D_800C9DB4 and D_800CA108 first, D_800CA108 into
 * a1, and n = unk26 straight into s4) and the order of the pos/arg1 giv increments at the loop end
 * (original: s2 (pos) before s0 (arg1)). Permuter hint: `n = arg0->unk2C; tile = ... + n;` puts
 * arg0/n in s5/s4 like the original but breaks the prologue order (24 rows).
 * Pass 11 (new build/search/cmp.py): a per-iteration `BgSprt *sp = &arg1[i];` fixes the giv
 * increment order at the loop end (pos before arg1), 14 -> 12 rows. Left: only the prologue (the
 * original loads D_800C9DB4 before D_800CA108 and computes pos and tile back to back); all 5040 orders
 * of the seven prologue statements, variable reuse for the unk28/unk2C/D_800CA108 loads and declaration
 * shuffles keep 12.
 * Pass 13: pos/tile are reload spills (the stack stores follow their defs); the original's chain has
 * both computations adjacent and before the unk18 loads (so the spill stores precede the lhu loads,
 * which after reload depend on any sp-relative store), i.e. sched1 did not float tile's computation
 * down as a free filler. Making pos/tile memory variables (struct, array, address-taken, volatile)
 * adds loop reloads (83-104 rows). Same filler-LUID class as B4A5C.
 * Pass 19: 12 -> 9 rows with `Unk800C9DB4 *bg = D_800C9DB4;` as a local (D_800C9DB4 now loads before
 * D_800CA108, into a0, like the original), then the 7! orders of the prologue lines (build/pp.py) with tile
 * before pos. Left: the original loads D_800CA108 (lbu a1) at the top but adds 0xDF only after the pos/tile
 * addu pair, and both addu's (and their spill stores) precede the unkC/unk30 loads. The lbu is QImode, so
 * it cannot pass the `arg0->unk30 = arg1` store (true_dependence's in-struct exception excludes QImode):
 * the w statement must precede that store in sched1, but every such order (and `w = D_800CA108; ... w += 0xDF`,
 * a u8 temp) puts tile's addu after the unk18 block again (9-21 rows). Permuter (15 min on this base) best:
 * the unk18/unk1A stores before w, n right after (permuter score 515, 15 cmp rows), not kept.
 */
#include "ovl_0a.h"

s32 func_800BD130(Unk800C9DB4Obj *arg0, BgSprt *arg1, BgSprt **arg2) {
    BgPos *pos;
    BgTile *tile;
    u32 i;
    u32 buf;
    s32 n;
    s32 w;
    s32 x;
    u16 t;

    Unk800C9DB4 *bg = D_800C9DB4;
    arg0->unk30 = arg1;
    w = D_800CA108 + 0xDF;
    tile = (BgTile *)((u8 *)bg + arg0->unk2C);
    pos = (BgPos *)((u8 *)bg + arg0->unk28);
    arg0->unk18 = arg0->unkC + bg->unk30;
    arg0->unk1A = arg0->unkE + bg->unk32;
    n = arg0->unk26;
    arg0->unk25 = 0;
    arg0->unk25_1 = 0;
    for (buf = 0; buf < 2; buf++) {
        for (i = 0; i < n; i++) {
            BgSprt *sp = &arg1[i];
            setlen(&sp->unk8, 3);
            sp->unk8.code = 0x7C;
            sp->unk8.r0 = 0x80;
            sp->unk8.g0 = 0x80;
            sp->unk8.b0 = 0x80;
            sp->unk8.x0 = arg0->unk18 + pos[i].x;
            sp->unk8.y0 = arg0->unk1A + pos[i].y;
            sp->unk8.u0 = tile[i].u;
            sp->unk8.v0 = tile[i].v;
            if (tile[i].trans) {
                sp->unk8.code |= 2;
            } else {
                sp->unk8.code &= ~2;
            }
            sp->unk8.clut = GetClut(tile[i].clutX << 4, tile[i].clutY);
            if (tile[i].tp == 0) {
                t = GetTPage(0, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            } else {
                t = GetTPage(1, tile[i].abr, tile[i].tpX << 6, tile[i].tpY << 8);
            }
            setDrawTPage(&sp->unk0, 1, 1, t);
            MargePrim(&sp->unk0, &sp->unk8);
            x = tile[i].clutY;
            if (w < x) {
                w = x;
            }
        }
        arg1 += n;
    }
    *arg2 = arg1;
    D_800CA108 = w + 0x21;
    return 1;
}
