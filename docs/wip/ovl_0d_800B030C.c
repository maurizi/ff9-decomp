/*
 * ovl_0d func_800B030C (0x30C bytes): draws the six hand cards (SPRT, or a zooming POLY_FT4 for
 * the selected one).
 * WIP: 195 of 195 instructions, 20 aligned diff rows (30 with the plain 0x40 literals).
 * The original keeps 0x40 (the texture v) in fp, set in the loop preheader next to the
 * hoisted 0x55555556 and ot + 100, and uses it for the FT4 v0/v1 and the SPRT v0, while v2/v3
 * are a literal 0x73. With literals loop.c never moves the 0x40 sets here (moving the v0 store
 * anywhere in the FT4 block doesn't help); a local `v = 0x40` before the loop gets fp-like
 * treatment only when it is also used as `v + 0x33` (else it's turned back into constants).
 * Left: 0x73 is computed from v, tpage/clut store order, the SPRT x/y loads and the u
 * register (a2 vs v0).
 */
#include "ovl_0d.h"
void *func_800B030C(u32 *ot, void *p, TmState *s) {
    TmUnk2F0 *e;
    s32 i;
    s32 n;
    s32 dx;
    s32 dy;
    s32 dw;
    s32 dh;
    u8 c;
    u8 u;
    s32 v;

    c = s->unk2EC;
    if (s->unk0 == 0x15) {
        c = s->unk2E8 * 8;
    }
    e = s->unk2F0;
    v = 0x40;
    for (i = 0; i < 6; i++) {
        if (e[i].unk0 != 0) {
            continue;
        }
        if (s->unk0 == 0x21 && s->unk5AC == 0 && i == s->unk56C) {
            n = 10 - s->unk5A8;
            if (n < 0) {
                n = 0;
            }
            dx = n * 4 / 3;
            dy = n * 5 / 3;
            dw = dx + 0x2A;
            dh = dy + 0x33;
            SetPolyFT4((POLY_FT4 *)p);
            ((POLY_FT4 *)p)->x0 = e[i].unk4 - dx;
            ((POLY_FT4 *)p)->y0 = e[i].unk8 - dy;
            ((POLY_FT4 *)p)->x1 = e[i].unk4 + dw;
            ((POLY_FT4 *)p)->y1 = e[i].unk8 - dy;
            ((POLY_FT4 *)p)->x2 = e[i].unk4 - dx;
            ((POLY_FT4 *)p)->y2 = e[i].unk8 + dh;
            ((POLY_FT4 *)p)->x3 = e[i].unk4 + dw;
            ((POLY_FT4 *)p)->y3 = e[i].unk8 + dh;
            u = 0x98;
            if (e[i].unkC == 0) {
                u = 0x68;
            }
            ((POLY_FT4 *)p)->u0 = u;
            ((POLY_FT4 *)p)->v0 = v;
            ((POLY_FT4 *)p)->u1 = u + 0x2A;
            ((POLY_FT4 *)p)->v1 = v;
            ((POLY_FT4 *)p)->u2 = u;
            ((POLY_FT4 *)p)->v2 = v + 0x33;
            ((POLY_FT4 *)p)->u3 = u + 0x2A;
            ((POLY_FT4 *)p)->v3 = v + 0x33;
            ((POLY_FT4 *)p)->r0 = c;
            ((POLY_FT4 *)p)->g0 = c;
            ((POLY_FT4 *)p)->b0 = c;
            ((POLY_FT4 *)p)->clut = 0x3A80;
            ((POLY_FT4 *)p)->tpage = 0x9C;
            AddPrim(ot + 99, p);
            p = (POLY_FT4 *)p + 1;
        } else {
            setlen((SPRT *)p, 4);
            ((SPRT *)p)->code = 0x64;
            if (s->unk0 == 0x15) {
                ((SPRT *)p)->code = 0x66;
            }
            ((SPRT *)p)->x0 = e[i].unk4;
            ((SPRT *)p)->r0 = c;
            ((SPRT *)p)->g0 = c;
            ((SPRT *)p)->b0 = c;
            ((SPRT *)p)->y0 = e[i].unk8;
            u = 0x98;
            if (e[i].unkC == 0) {
                u = 0x68;
            }
            ((SPRT *)p)->u0 = u;
            ((SPRT *)p)->v0 = v;
            ((SPRT *)p)->clut = 0x3A80;
            ((SPRT *)p)->w = 0x2A;
            ((SPRT *)p)->h = 0x33;
            AddPrim(ot + 100, p);
            p = (SPRT *)p + 1;
            setlen((DR_TPAGE *)p, 1);
            ((DR_TPAGE *)p)->code[0] = (GetTPage(1, 1, 0x300, 0x100) & 0x9FF) | 0xE1000000;
            AddPrim(ot + 100, p);
            p = (DR_TPAGE *)p + 1;
        }
    }
    return p;
}
