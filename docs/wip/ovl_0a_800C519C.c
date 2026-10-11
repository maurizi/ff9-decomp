/*
 * ovl_0a func_800C519C (0x8E0 bytes): walkmesh edge crossing. Projects the triangle n onto the
 * plane picked from its normal (x/z axis pair like func_800C4B30, y = the remaining axis),
 * intersects the segment arg4 -> arg3 with each edge except `edge` (arg7), stores the hits and
 * their edge numbers, then: 1 hit -> that one; 2 hits -> func_800C1E14 picks between them;
 * 3 hits -> the one closest to the origin; otherwise *arg6 = -1 and 0.
 * WIP (pass 21): 568 of 568 instructions, 39 aligned diff rows (pass 20: 74) (pass 9: 570 instructions, 140 rows).
 * Control flow and the stack layout match; left is register allocation (the original uses
 * s0 = hit base, s1 = arg5, s3 = z, s4 = x, s5 = arg4, s6 = arg6, s7 = arg3, fp = v base,
 * edge spilled to 0x68 and &q[x] spilled to 0x70; ours still differs in x/z/p/q and the
 * caller-saved temps).
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
 * Pass 20 (shape first): 140 -> 108 rows, 568 of 568 instructions; the shape diff is down to the prologue's
 * save/arg-load order (register-driven), one `addiu v0,sp,0x3C` slot after the vertex loop, and two
 * symbol-name false positives (%hi(D_800C9E04) = D_800C9DF0.unk14). What fixed the shape:
 *  - the edge test is func_800C1E14's code pasted in (that function is matched and compiles to exactly the
 *    original's bgez/negu, bltz, slt/beqz/j and negu/slt/bnez sequence): `d = (pv[1][x] - pv[0][x]) *
 *    (ez = p[z] - q[z]); ex = p[x] - q[x]; e = (pv[0][z] - q[z]) * ex; d -= (pv[1][z] - pv[0][z]) * ex;
 *    e -= (pv[0][x] - q[x]) * ez;` (the original's mult order is d1, e1, d2, e2), then nested
 *    `if (ABS(e) <= ABS(d)) { f = ...; f -= ...; if (ABS(f) <= ABS(d)) { if ((e >= 0 && d >= 0) || (e <= 0 &&
 *    d < 0)) { if ((f >= 0 && d >= 0) || (f <= 0 && d < 0)) {` -- no gotos (the old goto form compiled
 *    identically, so it was an equivalent spelling);
 *  - the result block: `if (cnt == 1) { one } else if (cnt < 2) { *arg6 = -1; return 0; } else switch (cnt)
 *    { case 2: ... break; case 3: ... break; default: *arg6 = -1; return 0; } return 1;` (every success
 *    path jumps to one shared `return 1` before the epilogue, as in the original).
 * Left: register allocation. The original's s-registers are s0 = hit, s1 = arg5, s2 = edge, s3 = z, s4 = x,
 * s5 = arg4 (q), s6 = arg6, s7 = arg3 (p), fp = v; ours s1 = edge, s2 = q, s3 = arg5, s4 = p, s5 = z, s6 = x,
 * s7 = arg6. Also every `pv[k][x]` address is `addu base, x2` in the original and `addu x2, base` here
 * (p[z]/q[z] are index-first in both); index spellings (*(pv + x), x[pv]) and x/y/z/edge/cnt/i types do not
 * change it (cnt or i as s32 breaks the shape).
 *  - vertices as func_800C4B30 has them: `Unk800C9DFCv v[3]; Unk800C9DFCv *pv[3];`, `pv[1] = &v[i];`,
 *    `pv[k]->unk0[x]` (108 -> 74: the pv address adds become base-first like the original). hit[] as
 *    Unk800C9DFCv or s16[3][3] makes no difference.
 * Now left (74 rows): only the s-register permutation (and a0/a3 for pl in the plane test). galloc.py: arg5 (85)
 * ends in s2 and the arg7/edge copy goes through s1, the original has arg5 in s1 and the edge/&q[x] reloads in
 * s2; both edge (95) and &q[x] (275) are spilled (MEM) in both, so it is reload's spill-register choice
 * (reload1.c order_regs_for_reload / hard_reg_n_uses) that differs: ours spills s1, the original s2. The rest
 * follows: z/x/q/arg6/p are s3/s4/s5/s6/s7 in the original, ours p s3?/q s4/z s5/x s6/arg6 s7.
 * Root cause (galloc.py): &q[x] (loop-invariant, pseudo 275, refs 5, length 222, priority 450) is allocated just
 * before arg5 (pseudo 85, refs 13, length 868, priority 449.3), takes s1 and is then spilled by reload, which
 * makes s1 the spill register. The original must have arg5 ahead (it loads the stack args in the order arg4,
 * arg5, arg6, ours arg5, arg6, arg4, so arg5's live range starts earlier here). Declaration order (80 random
 * scalar orders) and `goto one` for cnt == 1 (79) do not help.
 * Pass 21: 74 -> 39 rows (shape: only the prologue's save/arg-load order and one `addiu v0,sp,0x3C` slot, both
 * register-driven). `tri = n;` as the first statement (moved from before o[2]) settles the arg5 / &q[x] near-tie:
 * arg5 now gets s1 and reload spills through s2, as in the original. `pv[0] = &v[2];` before `cnt = 0;` (-2).
 * Left: the s3..s7 permutation. Ours q s3, p s4, z s5, x s6, arg6 s7 (global order q 380 > p 330 > z 285 >
 * x 284 > arg6 171); the original needs z, x > q > arg6 > p. Refs match the original's insns (p: copy, two
 * in-loop adds, call arg; q: load, three adds), so the difference is live length or loop weighting. x/y/z types
 * (25 combos), p/q assignment placement (35 variants) and single-line moves (build/w/mvrun.py, 178 moves) give
 * nothing better. A 20-min permuter run (score 885 -> 445) only moved `p = arg3` into the vertex loop or a switch
 * case (fake) or reused p as a hit-row pointer in case 3 (`p = hit[i]`), which is 88 cmp rows.
 */
#include "ovl_0a.h"

s32 func_800C519C(Unk800C9DF8 *b, Unk800C9DF8Tri *t, Unk800C9E00 *n, SVECTOR *arg3, SVECTOR *arg4, SVECTOR *arg5, s16 *arg6, s32 arg7) {
    s16 o[3];
    Unk800C9DFCv *pv[3];
    Unk800C9DFCv v[3];
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
    s32 ex;
    s32 ez;

    tri = n;
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
    o[2] = b->unk8[2] + t->unkA[2];
    for (i = 0; i < 3; i++) {
        v[i].unk0[0] = o[0] + vtx[tri->unkC[i]].unk0[0];
        v[i].unk0[1] = o[1] + vtx[tri->unkC[i]].unk0[1];
        v[i].unk0[2] = o[2] + vtx[tri->unkC[i]].unk0[2];
    }
    pv[0] = &v[2];
    cnt = 0;
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        if (i != edge) {
            d = (pv[1]->unk0[x] - pv[0]->unk0[x]) * (ez = p[z] - q[z]);
            ex = p[x] - q[x];
            e = (pv[0]->unk0[z] - q[z]) * ex;
            d -= (pv[1]->unk0[z] - pv[0]->unk0[z]) * ex;
            e -= (pv[0]->unk0[x] - q[x]) * ez;
            if (ABS(e) <= ABS(d)) {
                f = (pv[0]->unk0[z] - q[z]) * (pv[1]->unk0[x] - pv[0]->unk0[x]);
                f -= (pv[0]->unk0[x] - q[x]) * (pv[1]->unk0[z] - pv[0]->unk0[z]);
                if (ABS(f) <= ABS(d)) {
                    if ((e >= 0 && d >= 0) || (e <= 0 && d < 0)) {
                        if ((f >= 0 && d >= 0) || (f <= 0 && d < 0)) {
            if (d != 0) {
                while (e > 0x80000) {
                    e >>= 1;
                    d >>= 1;
                }
                hit[cnt][x] = pv[0]->unk0[x] + e * (pv[1]->unk0[x] - pv[0]->unk0[x]) / d;
                hit[cnt][z] = pv[0]->unk0[z] + e * (pv[1]->unk0[z] - pv[0]->unk0[z]) / d;
                ax = pv[1]->unk0[x] - pv[0]->unk0[x];
                if (ax < 0) {
                    ax = pv[0]->unk0[x] - pv[1]->unk0[x];
                }
                if ((pv[1]->unk0[z] - pv[0]->unk0[z] >= 0 ? pv[1]->unk0[z] - pv[0]->unk0[z] : pv[0]->unk0[z] - pv[1]->unk0[z]) < ax) {
                    hit[cnt][y] = pv[1]->unk0[y] - (pv[1]->unk0[x] - hit[cnt][x]) * (pv[0]->unk0[y] - pv[1]->unk0[y]) / (pv[0]->unk0[x] - pv[1]->unk0[x]);
                } else {
                    hit[cnt][y] = pv[1]->unk0[y] - (pv[1]->unk0[z] - hit[cnt][z]) * (pv[0]->unk0[y] - pv[1]->unk0[y]) / (pv[0]->unk0[z] - pv[1]->unk0[z]);
                }
            }
                            idx[cnt] = i;
                            cnt++;
                        }
                    }
                }
            }
        }
        pv[0] = pv[1];
    }
    if (cnt == 1) {
        *arg6 = idx[0];
        arg5->vx = hit[0][0];
        arg5->vy = hit[0][1];
        arg5->vz = hit[0][2];
    } else if (cnt < 2) {
        *arg6 = -1;
        return 0;
    } else switch (cnt) {
    case 2:
        switch (idx[0] + idx[1]) {
        case 1:
            pv[0] = &v[1];
            pv[1] = &v[2];
            pv[2] = &v[0];
            break;
        case 2:
            pv[0] = &v[1];
            pv[1] = &v[0];
            pv[2] = &v[2];
            break;
        case 3:
            pv[0] = &v[2];
            pv[1] = &v[0];
            pv[2] = &v[1];
            break;
        }
        if (func_800C1E14(pv[0]->unk0, p, pv[2]->unk0, pv[1]->unk0, x, z)) {
            *arg6 = idx[0];
            arg5->vx = hit[0][0];
            arg5->vy = hit[0][1];
            arg5->vz = hit[0][2];
        } else {
            *arg6 = idx[1];
            arg5->vx = hit[1][0];
            arg5->vy = hit[1][1];
            arg5->vz = hit[1][2];
        }
        break;
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
        break;
    default:
        *arg6 = -1;
        return 0;
    }
    return 1;
}
