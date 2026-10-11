/*
 * ovl_0d func_800B16A4 (0x330 bytes): draws the hand cursor (SPRT + shadow) or, in mode 2, the
 * zooming FT4 cursor pair.
 * WIP (pass 16): 205 vs 204 instructions, 28 aligned diff rows (was 37, 136).
 * Fixed: `s32 u` (with u8 u the FT4 branch's u got CSE'd into a separate local; the original keeps
 * u in s7 in both branches).
 * Left: ot should be s1 (it's s3 here) and the FT4 branch's ot + 3 local s5; the FT4 stores are
 * scheduled differently around the stack-arg reloads of y.
 * Pass 12: in the original the FT4 block starts with o2 = ot + 3 (s5) before any constant is
 * born, so ot (s1) does not conflict with the block's constant pseudos (0x80 s0, 0x90 s1,
 * 0x9C s3, 0x3AC0 s4, u + 0x17 s2); ours births the SetPolyFT4 constants first and ot ends in s3.
 * Pass 14: `o = ot + 3;` as the first statement of the FT4 branch (shared o) gives 147 rows.
 * Pass 16: the first FT4's y2 store right after x3 (permuter, statement order): 37 -> 28. A block-local
 * `o2 = ot + 3` in the FT4 branch gives 38.
 */
#include "ovl_0d.h"
void *func_800B16A4(u32 *ot, void *p, TmState *s, s32 x, s32 y, s32 mode) {
    s32 z;
    u8 c;
    s32 u;
    u8 u2;
    u32 *o;

    if (mode != 2) {
        z = 3;
        if (mode != 0) {
            z = 6;
        }
        c = s->unk2EC;
        setlen((SPRT *)p, 4);
        ((SPRT *)p)->code = 0x64;
        if (mode != 0) {
            ((SPRT *)p)->code = 0x66;
        }
        ((SPRT *)p)->x0 = x;
        ((SPRT *)p)->r0 = c;
        ((SPRT *)p)->g0 = c;
        ((SPRT *)p)->b0 = c;
        ((SPRT *)p)->y0 = y;
        u = ((s->unk480 / 6) & 7) * 24;
        if (mode != 0) {
            u = ((s->unk488 / 6) & 7) * 24;
        }
        o = ot + z;
        ((SPRT *)p)->u0 = u;
        ((SPRT *)p)->v0 = 0x80;
        ((SPRT *)p)->clut = 0x3AC0;
        ((SPRT *)p)->w = 0x18;
        ((SPRT *)p)->h = 0x10;
        AddPrim(o, p);
        p = (SPRT *)p + 1;
        ((SPRT *)p)->code = 0x66;
        setlen((SPRT *)p, 4);
        ((SPRT *)p)->x0 = x + 4;
        ((SPRT *)p)->y0 = y + 4;
        ((SPRT *)p)->r0 = 0;
        ((SPRT *)p)->g0 = 0;
        ((SPRT *)p)->b0 = 0;
        ((SPRT *)p)->u0 = u;
        ((SPRT *)p)->v0 = 0x80;
        ((SPRT *)p)->clut = 0x3AC0;
        ((SPRT *)p)->w = 0x18;
        ((SPRT *)p)->h = 0x10;
        AddPrim(o, p);
        p = (SPRT *)p + 1;
        setlen((DR_TPAGE *)p, 1);
        ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 0, 0x300, 0x100) & 0x9FF) | 0xE1000000;
        AddPrim(o, p);
        p = (DR_TPAGE *)p + 1;
    } else {
        u = ((s->unk480 / 6) & 7) * 24;
        SetPolyFT4((POLY_FT4 *)p);
        ((POLY_FT4 *)p)->x0 = x;
        ((POLY_FT4 *)p)->x1 = x + 0x18;
        ((POLY_FT4 *)p)->y0 = y;
        ((POLY_FT4 *)p)->x2 = x;
        ((POLY_FT4 *)p)->y1 = y;
        ((POLY_FT4 *)p)->x3 = x + 0x18;
        ((POLY_FT4 *)p)->y2 = y + 0x10;
        u2 = u + 0x17;
        ((POLY_FT4 *)p)->u0 = u2;
        ((POLY_FT4 *)p)->v0 = 0x80;
        ((POLY_FT4 *)p)->u1 = u;
        ((POLY_FT4 *)p)->v1 = 0x80;
        ((POLY_FT4 *)p)->u2 = u2;
        ((POLY_FT4 *)p)->v2 = 0x90;
        ((POLY_FT4 *)p)->u3 = u;
        ((POLY_FT4 *)p)->v3 = 0x90;
        ((POLY_FT4 *)p)->clut = 0x3AC0;
        ((POLY_FT4 *)p)->r0 = 0x80;
        ((POLY_FT4 *)p)->g0 = 0x80;
        ((POLY_FT4 *)p)->b0 = 0x80;
        ((POLY_FT4 *)p)->tpage = 0x9C;
        ((POLY_FT4 *)p)->y3 = y + 0x10;
        AddPrim(ot + 3, p);
        p = (POLY_FT4 *)p + 1;
        SetPolyFT4((POLY_FT4 *)p);
        ((POLY_FT4 *)p)->x0 = x + 4;
        ((POLY_FT4 *)p)->x2 = x + 4;
        ((POLY_FT4 *)p)->x1 = x + 0x1C;
        ((POLY_FT4 *)p)->y0 = y + 4;
        ((POLY_FT4 *)p)->y1 = y + 4;
        ((POLY_FT4 *)p)->y2 = y + 0x14;
        ((POLY_FT4 *)p)->x3 = x + 0x1C;
        ((POLY_FT4 *)p)->y3 = y + 0x14;
        ((POLY_FT4 *)p)->u0 = u2;
        ((POLY_FT4 *)p)->v0 = 0x80;
        ((POLY_FT4 *)p)->u1 = u;
        ((POLY_FT4 *)p)->v1 = 0x80;
        ((POLY_FT4 *)p)->u2 = u2;
        ((POLY_FT4 *)p)->v2 = 0x90;
        ((POLY_FT4 *)p)->u3 = u;
        ((POLY_FT4 *)p)->v3 = 0x90;
        ((POLY_FT4 *)p)->clut = 0x3AC0;
        ((POLY_FT4 *)p)->r0 = 0;
        ((POLY_FT4 *)p)->g0 = 0;
        ((POLY_FT4 *)p)->b0 = 0;
        ((POLY_FT4 *)p)->tpage = 0x9C;
        setSemiTrans((POLY_FT4 *)p, 1);
        AddPrim(ot + 3, p);
        p = (POLY_FT4 *)p + 1;
    }
    return p;
}
