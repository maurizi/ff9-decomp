/*
 * ovl_0a func_800C4EC8 (0x2D4 bytes): point-in-triangle test on the walkmesh (x/z plane), the
 * plane-less sibling of func_800C4B30.
 * WIP: 181 of 181 instructions, 1 diff row: the third zero at the top. The original has
 * `t7 = 0 (r); t2 = t7 (i); t6 = 0` (t6 = the x*2 offset, a loop-hoisted 0); ours has
 * `t6 = t7` as well. Both copies come from postreload CSE (reload_cse_simplify_operands
 * replacing a constant source by a register already known to hold it), so in the original no
 * SImode register was known to hold 0 when t6 was set. With `u8 r` the t6 copy goes away but
 * the final `return r` then needs an andi (also with a u8 return type). Tried: x/z/i types
 * (u8/s8/u16/s16/s32/u32), all orders of the x/z/r/i/vtx initialisations.
 * Pass 9: the t6 = 0 is loop.c's hoisted x * 2 offset (inserted before the loop, after r = 0),
 * so for reload_cse not to see t7 = 0 the t6 set must precede r's set at that point (sched2 then
 * swaps them back). Moving `r = 0;` / `i = 0;` anywhere before the loop does not change it.
 */
#include "ovl_0a.h"

s32 func_800C4EC8(Unk800C9DF8 *b, Unk800C9DF8Tri *t, Unk800C9E00 *n, SVECTOR *sv) {
    s16 *p = (s16 *)sv;
    s8 c[2];
    s32 d[2];
    Unk800C9DFCv *pv[2];
    s16 o[3];
    Unk800C9DFCv v[3];
    u8 i;
    s32 r;
    u8 x;
    u8 z;
    Unk800C9DFCv *vtx;

    x = 0;
    z = 2;
    r = 0;
    i = 0;
    vtx = (Unk800C9DFCv *)D_800C9DF0.unkC;
    o[x] = b->unk8[x] + t->unkA[x];
    o[z] = b->unk8[z] + t->unkA[z];
    c[0] = 0;
    c[1] = 0;
    for (; i < 3; i++) {
        v[i].unk0[x] = o[x] + vtx[n->unkC[i]].unk0[x];
        v[i].unk0[z] = o[z] + vtx[n->unkC[i]].unk0[z];
        if (v[i].unk0[x] == p[x]) {
            c[0]++;
            if (v[i].unk0[z] <= p[z]) {
                c[0] = -c[0];
            }
        }
        if (v[i].unk0[z] == p[z]) {
            c[1]++;
            if (v[i].unk0[x] <= p[x]) {
                c[1] = -c[1];
            }
        }
    }
    if (c[0] == -2) {
        return 1;
    }
    if (c[1] == -2) {
        return 1;
    }
    pv[0] = &v[2];
    c[0] = p[z] >= pv[0]->unk0[z];
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        c[1] = p[z] >= pv[1]->unk0[z];
        if (c[0] != c[1]) {
            d[0] = (pv[1]->unk0[z] - p[z]) * (pv[0]->unk0[x] - pv[1]->unk0[x]);
            d[1] = (pv[1]->unk0[x] - p[x]) * (pv[0]->unk0[z] - pv[1]->unk0[z]);
            if (d[0] == d[1]) {
                return 1;
            }
            if ((d[1] < d[0]) == c[1]) {
                r ^= 1;
            }
        }
        pv[0] = pv[1];
        c[0] = c[1];
    }
    return r;
}

