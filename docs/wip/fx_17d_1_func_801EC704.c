/* Draft of func_801EC704 (module fx_17d_1). Status: 66 aligned mismatches, 18 ignoring registers and spill offsets
 * (spnorm 0x10). Pass 3: the q+4 giv is a separate pointer biv (q = p at the top of each row, q++ at the bottom) with
 * the addPrim argument copied first (pp = p++ at the top), which puts the giv increment at the bottom and a2/p at the
 * top as in the original; prologue statement order (searched) fixes j/dh (t2/t1); row header y1, v1, uu, q and clut
 * before tpage (swap search). Open: the giv (65 refs) takes s0 and uu s1 where the original has them swapped (uu
 * cannot outrank the giv by global-alloc priority, so something makes s0 unavailable to the giv in the original);
 * x0/v1 (t4/t5) flip with the header order (y1, uu, v1, q gives the right t4/t5 at 62 rows/26 noreg); sched2
 * position of sra fp (dw) in the prologue and of the abrBits reload in the loop.
 * Earlier: dw initialized through m (m = w >> 2; dw = m;), xs = dw * i and xa = xs + dw per iteration, k = i < 3
 * then i++, the r0 word store after clut. */
#include "fxmod/fx_17d_1.h"

void func_801EC704(s32 ot, s32 x, s32 y, s32 w, s32 h, s32 c, s32 tpage, s32 clut, s32 u, s32 v, s32 abr) {
    POLY_FT4 *p;
    BtlfxExports *ex;
    u8 col[4];
    s32 x0;
    s32 y0;
    s32 y1;
    s32 dw;
    s32 dh;
    s32 v1;
    s32 abrBits;
    s32 uu;
    s32 xa;
    s32 xs;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 ta;
    s32 ts;
    POLY_FT4 *q;
    POLY_FT4 *pp;

    x0 = x - (w >> 1) * 3 / 2;
    abrBits = (abr & 3) << 5;
    m = w >> 2;
    ex = BTLFX_EXPORTS(D_801ED420);
    p = (POLY_FT4 *)D_801ED420->unk24;
    dh = h >> 2;
    D_801ED420->unk24 = (u8 *)(p + 16);
    y0 = y - (h >> 1);
    dw = m;
    for (j = 0; j < 4; j++) {
        y1 = y0 + dh;
        v1 = v + dh;
        uu = u;
        q = p;
        for (i = 0; i < 4;) {
            col[0] = col[1] = col[2] = c;
            if (abr != 0xFF) {
                col[3] = 0x2E;
            } else {
                col[3] = 0x2C;
            }
            pp = p++;
            xs = dw * i;
            xa = xs + dw;
            ta = xa * 3;
            ts = xs * 3;
            k = i < 3;
            i++;
            q->x0 = q->x2 = x0 + ts / 2;
            q->u0 = q->u2 = uu;
            uu += dw;
            q->u1 = q->u3 = uu - (k ^ 1);
            m = j < 3;
            q->v2 = q->v3 = v1 - (m ^ 1);
            q->x1 = q->x3 = x0 + ta / 2;
            q->v0 = q->v1 = v;
            q->y0 = q->y1 = y0;
            q->y2 = q->y3 = y1;
            tpage |= abrBits;
            q->clut = clut;
            q->tpage = tpage;
            *(u32 *)&q->r0 = *(u32 *)col;
            q++;
            ex->btlfx_addPrim(0x09000000, ot, pp);
        }
        v += dh;
        y0 += dh;
    }
}
