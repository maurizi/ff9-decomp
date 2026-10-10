/*
 * ovl_0a func_800C519C (0x8E0 bytes): walkmesh edge crossing. Projects the triangle n onto the
 * plane picked from its normal (x/z axis pair like func_800C4B30, y = the remaining axis),
 * intersects the segment arg4 -> arg3 with each edge except `edge` (arg7), stores the hits and
 * their edge numbers, then: 1 hit -> that one; 2 hits -> func_800C1E14 picks between them;
 * 3 hits -> the one closest to the origin; otherwise *arg6 = -1 and 0.
 * WIP (pass 9, first draft): 570 vs 568 instructions, 140 aligned diff rows (build/s/cmp.py).
 * Control flow and the stack layout match; left is register allocation (the original uses
 * s0 = hit base, s1 = arg5, s3 = z, s4 = x, s5 = arg4, s6 = arg6, s7 = arg3, fp = v base,
 * edge spilled to 0x68 and &q[x] spilled to 0x70; ours still differs in x/z/p/q and the
 * caller-saved temps) and 3 extra insns.
 * What the shape needed:
 *  - `ABS(d) < ABS(e)` is distributed by the original into two compare-and-branch arms
 *    (`bltz d` then `slt; beqz cont; j next` / `negu; slt; bnez next`); every expression form
 *    (ABS, a `>= 0` ABS, `?:` of compares, if/else) cross-jumps into one bnez here. Only the
 *    explicit `if (d >= 0) { if (d >= ae) goto c1; goto next; } if (-d < ae) goto next; c1:`
 *    gives it;
 *  - the sign tests: `if (e >= 0) { if (d >= 0) goto s1; if (e > 0) goto next; }
 *    if (d >= 0) goto next; s1:` (same for f);
 *  - the hit-count dispatch is `== 1`, `< 2`, then a switch on 2/3 (an if chain, not a
 *    3-case switch), and the 1-hit result block sits inside case 2 after the
 *    func_800C1E14 call (`goto one`);
 *  - `d = q[x]; d = ... * (p[x] - d);` (permuter) spills edge and &q[x] like the original
 *    (237 -> 147 rows); vtx computed before o[] and a `tri = n` copy for the vertex loop
 *    (permuter) took it to 140.
 * func_800C1E14 is defined earlier in ovl_0a.c (no prototype in the header).
 */
#include "ovl_0a.h"

s32 func_800C519C(Unk800C9DF8 *b, Unk800C9DF8Tri *t, Unk800C9E00 *n, SVECTOR *arg3, SVECTOR *arg4, SVECTOR *arg5, s16 *arg6, s32 arg7) {
    s16 o[3];
    s16 *pv[3];
    s16 v[3][3];
    s16 hit[3][3];
    s16 idx[3];
    s16 *p;
    s16 *q;
    Unk800C9DFCv *vtx;
    Unk800C9E04 *pl;
    Unk800C9E00 *tri;
    s16 i;
    s16 cnt;
    s16 edge;
    u8 x;
    u8 y;
    u8 z;
    u8 best;
    s32 a;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
    s32 ae;
    s32 ax;
    s32 m;
    s32 min;

    p = (s16 *)arg3;
    q = (s16 *)arg4;
    edge = arg7;
    x = 0;
    z = 2;
    y = 1;
    if (n->unk6 >= 0) {
        pl = &D_800C9DF0.unk14[n->unk6];
        if ((u32)(pl->unk4 + 0x50F4) <= 0xA1E8) {
            a = pl->unk0;
            c = pl->unk8;
            if (ABS(a) > ABS(c)) {
                x = 1;
                z = 2;
                y = 0;
            } else {
                x = 0;
                z = 1;
                y = 2;
            }
        }
    }
    vtx = (Unk800C9DFCv *)((u8 *)b + b->unk3A);
    o[0] = b->unk8[0] + t->unkA[0];
    o[1] = b->unk8[1] + t->unkA[1];
    tri = n;
    o[2] = b->unk8[2] + t->unkA[2];
    for (i = 0; i < 3; i++) {
        v[i][0] = o[0] + vtx[tri->unkC[i]].unk0[0];
        v[i][1] = o[1] + vtx[tri->unkC[i]].unk0[1];
        v[i][2] = o[2] + vtx[tri->unkC[i]].unk0[2];
    }
    cnt = 0;
    pv[0] = v[2];
    for (i = 0; i < 3; i++) {
        pv[1] = v[i];
        if (i != edge) {
            d = q[x];
            d = (pv[1][x] - pv[0][x]) * (p[z] - q[z]) - (pv[1][z] - pv[0][z]) * (p[x] - d);
            e = (pv[0][z] - q[z]) * (p[x] - q[x]) - (pv[0][x] - q[x]) * (p[z] - q[z]);
            ae = ABS(e);
            if (d >= 0) {
                if (d >= ae) {
                    goto c1;
                }
                goto next;
            }
            if (-d < ae) {
                goto next;
            }
        c1:
            f = (pv[0][z] - q[z]) * (pv[1][x] - pv[0][x]) - (pv[0][x] - q[x]) * (pv[1][z] - pv[0][z]);
            ae = ABS(f);
            if (d >= 0) {
                if (d >= ae) {
                    goto c2;
                }
                goto next;
            }
            if (-d < ae) {
                goto next;
            }
        c2:
            if (e >= 0) {
                if (d >= 0) {
                    goto s1;
                }
                if (e > 0) {
                    goto next;
                }
            }
            if (d >= 0) {
                goto next;
            }
        s1:
            if (f >= 0) {
                if (d >= 0) {
                    goto s2;
                }
                if (f > 0) {
                    goto next;
                }
            }
            if (d >= 0) {
                goto next;
            }
        s2:
            if (d != 0) {
                while (e > 0x80000) {
                    e >>= 1;
                    d >>= 1;
                }
                hit[cnt][x] = pv[0][x] + e * (pv[1][x] - pv[0][x]) / d;
                hit[cnt][z] = pv[0][z] + e * (pv[1][z] - pv[0][z]) / d;
                ax = pv[1][x] - pv[0][x];
                if (ax < 0) {
                    ax = pv[0][x] - pv[1][x];
                }
                if ((pv[1][z] - pv[0][z] >= 0 ? pv[1][z] - pv[0][z] : pv[0][z] - pv[1][z]) < ax) {
                    hit[cnt][y] = pv[1][y] - (pv[1][x] - hit[cnt][x]) * (pv[0][y] - pv[1][y]) / (pv[0][x] - pv[1][x]);
                } else {
                    hit[cnt][y] = pv[1][y] - (pv[1][z] - hit[cnt][z]) * (pv[0][y] - pv[1][y]) / (pv[0][z] - pv[1][z]);
                }
            }
            idx[cnt] = i;
            cnt++;
        }
    next:
        pv[0] = pv[1];
    }
    if (cnt == 1) {
        goto one;
    }
    if (cnt < 2) {
        *arg6 = -1;
        return 0;
    }
    switch (cnt) {
    case 2:
        switch (idx[0] + idx[1]) {
        case 1:
            pv[0] = v[1];
            pv[1] = v[2];
            pv[2] = v[0];
            break;
        case 2:
            pv[0] = v[1];
            pv[1] = v[0];
            pv[2] = v[2];
            break;
        case 3:
            pv[0] = v[2];
            pv[1] = v[0];
            pv[2] = v[1];
            break;
        }
        if (func_800C1E14(pv[0], p, pv[2], pv[1], x, z)) {
        one:
            *arg6 = idx[0];
            arg5->vx = hit[0][0];
            arg5->vy = hit[0][1];
            arg5->vz = hit[0][2];
            return 1;
        }
        *arg6 = idx[1];
        arg5->vx = hit[1][0];
        arg5->vy = hit[1][1];
        arg5->vz = hit[1][2];
        return 1;
    case 3:
        best = 0;
        min = 0x7FFFFFFF;
        for (i = 0; i < 3; i++) {
            m = hit[i][0] * hit[i][0] + hit[i][1] * hit[i][1] + hit[i][2] * hit[i][2];
            if (m < min) {
                best = i;
                min = m;
            }
        }
        *arg6 = best;
        arg5->vx = hit[best][0];
        arg5->vy = hit[best][1];
        arg5->vz = hit[best][2];
        return 1;
    default:
        *arg6 = -1;
        return 0;
    }
}
