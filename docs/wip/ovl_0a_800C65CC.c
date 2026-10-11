/*
 * ovl_0a func_800C65CC (0x638): walkmesh triangle transfer: express the actor position (a->unk24->unk14)
 * relative to triangle n0 by barycentric-like cross products (nv) and re-project it onto triangle n1;
 * the height then comes from n1's plane (func_800C1DC4 fixed-point, pl = D_800C9DF0.unk14[n1->unk6]) or,
 * without a plane, from n1's first vertex.
 * WIP (pass 21): 38 aligned diff rows, 398 of 398 instructions, shape exact (cmp_shape.py --shape: 0 rows), no fake
 * temps. (Pass 15: 58 rows with the permuter's new_var, 93 without.)
 * Found:
 *  - the else arm is written like func_800C3608's (`t3 = D_800C9DF0.unk1C + n1->unk4; vt = D_800C9DF0.unkC
 *    + n1->unkC[0];`), which fixes its load order (104 -> 93);
 *  - the original saves s6 without ever using it, and keeps the first cross product's mflo in s3: it has one
 *    more callee-saved pseudo than ours. The permuter's `new_var = v[1].vx - p.vx;` placed between the
 *    v[0].vz and v[1].vz loads (i.e. computed before p.vz, as in the original's `subu s0,t2,a1` at 0x1E8)
 *    reproduces the whole prologue and s-register layout (93 -> 58), but anywhere else it does nothing,
 *    so the real shape is probably a different statement order of the v/p loads or difference temps.
 * Left: `t` (n0's triangle) is in s0 here, t7 in the original; area's final subu and the r.vx store come
 * after the second triangle's v[*].vx loads in the original; one extra insn.
 * Pass 20: without the permuter's new_var (a fake temp) the draft is 93 rows, shape 23 (397 vs 398 insns): the
 * original's extra callee-saved register (s6 saved, never used) and the first cross product's mflo in s3 are the
 * shape difference. nv (0x38) and r (0x48) live on the stack. Not changed this pass.
 * Pass 21: the cross products are written with six named differences, dx0..dx2 = v[i].vx - p.vx computed right
 * after the v[*].vz loads and before p.vz (they only need p.vx), dz0..dz2 after p.vz, then `nv.vx = dx1 * dz2 -
 * dx2 * dz1;` etc. That is what the permuter's new_var stood for (dx1 lives in s0, the first mflo in s3 and s6 is
 * saved): 93 -> 58 without any fake temp. `r.vx = ... / area;` moved after the second triangle's v[*].vz loads
 * (right before r.vz) gives the original's late `sw a0,0x48(sp)` and the instruction count: 58 -> 49, shape exact.
 * Left: register naming only, in the big straight-line block (local-alloc): t is s0 here, t7 in the original
 * (splitting t into t/t2 per triangle: 51); o.vz / v[1].vx swap t2/t4, and the dz/dx temps and mflo results are
 * a permutation of a2/t2/t3/t4/t7. Single-line moves (700 compiles, build/w/mvrun.py) find nothing better.
 * Then 49 -> 38 by reusing one variable `d` for dz0 (v[0].vz - p.vz) and for the area (the permuter's hint was
 * `area = v[0].vz - p.vz`; the reuse makes it a multi-set pseudo, which also puts t back in t7). Left: o.vz /
 * v[1].vx swap t2/t4, and the dz/dx temps, d and the mflo results are a permutation of a2/t2/t3/t4/t8. dx/dz
 * statement orders (36), reusing k/c for a second difference (30) and 1700 single-line moves: no better.
 * Pass 22 (local-alloc simulation, build/search/lsim.py: reproduces all 100 local qtys of this block exactly from
 * the .lreg dump): the original cannot have one variable for dz0 and the area, since they are in different hard
 * registers there (dz0 v0: `subu v0,t1,v0` / `mult a2,v0` / `mult s0,v0`; area t2: `subu t2,t2,t1`, the div
 * divisor). With separate `dz0`/`area` locals (49 rows) the products and nv.vy already match; the rest is a
 * local-alloc order problem. Target (c65b numbering): o.vz {110,112} t4, v[1].vx chain {131,211,209,258} t2,
 * p.vz {222,224} t3, dx2 89 a2, dz1 90 t4, nv.vx 245 a2, nv.vz 251 t4, area 93 t2. The simulator shows that the
 * priority order dx2 > v1vx > p.vz > dz1 and nv.vx > area gives all of them except area (which then gets a3):
 * today p.vz 1923 > dx2 1875 > dz1 1500 > v1vx 1428 and area 909 > nv.vx 444, i.e. p.vz must lose ~20%
 * (live length 52 -> 54..66 half-insns with 5 refs) and v1vx must beat dz1 and p.vz, which no statement order
 * gives (1500 random topological orders of the dx/dz/p.vz/nv/area lines: all 49). Area probably is not a local
 * qty in the original. Also tried: dropping `t` (154 rows), a second pointer t2 for n1 (51), area reused as k (73).
 * Pass 23: area in c (70), dx0 (75), dz2 (47), dx2 (92), dz1 (115) or dx1 (74). The original never stores the area
 * to nv/r.pad. The param lever does not apply: the params are already used directly, and the caller's u8 * for b
 * changes no RTL.
 */
#include "ovl_0a.h"

s32 func_800C65CC(Unk800C9DF8 *b, Unk800C9DF4 *a, Unk800C9E00 *n0, Unk800C9E00 *n1) {
    SVECTOR o;
    SVECTOR v[3];
    SVECTOR p;
    VECTOR nv;
    VECTOR r;
    Unk800C9DF8Tri *t;
    Unk800C9DFC *vtx;
    Unk800C9E04 *pl;
    s32 dx0, dx1, dx2, dz1, dz2;
    s32 d;
    s32 k;
    s32 c;

    t = &D_800C9DF0.unk1C[n0->unk4];
    o.vx = b->unk8[0] + t->unkA[0];
    o.vy = b->unk8[1] + t->unkA[1];
    o.vz = b->unk8[2] + t->unkA[2];
    vtx = (Unk800C9DFC *)((u8 *)b + b->unk3A);
    v[0].vx = vtx[n0->unkC[0]].unk0;
    v[1].vx = vtx[n0->unkC[1]].unk0;
    v[2].vx = vtx[n0->unkC[2]].unk0;
    p.vx = a->unk24->unk14.vx - o.vx;
    v[0].vy = vtx[n0->unkC[0]].unk2;
    v[1].vy = vtx[n0->unkC[1]].unk2;
    v[2].vy = vtx[n0->unkC[2]].unk2;
    p.vy = a->unk24->unk14.vy - o.vy;
    v[0].vz = vtx[n0->unkC[0]].unk4;
    v[1].vz = vtx[n0->unkC[1]].unk4;
    v[2].vz = vtx[n0->unkC[2]].unk4;
    dx0 = v[0].vx - p.vx;
    dx1 = v[1].vx - p.vx;
    dx2 = v[2].vx - p.vx;
    p.vz = a->unk24->unk14.vz - o.vz;
    d = v[0].vz - p.vz;
    dz1 = v[1].vz - p.vz;
    dz2 = v[2].vz - p.vz;
    nv.vx = dx1 * dz2 - dx2 * dz1;
    nv.vy = dx2 * d - dx0 * dz2;
    nv.vz = dx0 * dz1 - dx1 * d;
    d = (v[1].vx - v[0].vx) * (v[2].vz - v[0].vz) - (v[2].vx - v[0].vx) * (v[1].vz - v[0].vz);
    t = &D_800C9DF0.unk1C[n1->unk4];
    o.vx = b->unk8[0] + t->unkA[0];
    o.vy = b->unk8[1] + t->unkA[1];
    o.vz = b->unk8[2] + t->unkA[2];
    v[0].vx = vtx[n1->unkC[0]].unk0;
    v[1].vx = vtx[n1->unkC[1]].unk0;
    v[2].vx = vtx[n1->unkC[2]].unk0;
    v[0].vy = vtx[n1->unkC[0]].unk2;
    v[1].vy = vtx[n1->unkC[1]].unk2;
    v[2].vy = vtx[n1->unkC[2]].unk2;
    v[0].vz = vtx[n1->unkC[0]].unk4;
    v[1].vz = vtx[n1->unkC[1]].unk4;
    v[2].vz = vtx[n1->unkC[2]].unk4;
    r.vx = (nv.vx * v[0].vx + nv.vy * v[1].vx + nv.vz * v[2].vx) / d;
    r.vz = (nv.vx * v[0].vz + nv.vy * v[1].vz + nv.vz * v[2].vz) / d;
    a->unk24->unk14.vx = r.vx + o.vx;
    a->unk24->unk14.vz = r.vz + o.vz;
    k = n1->unk6;
    if (k >= 0) {
        pl = &D_800C9DF0.unk14[k];
        c = func_800C1DC4(pl->unk0, a->unk24->unk14.vx << 16);
        a->unk24->unk14.vy = func_800C1DC4(n1->unk24 - c - func_800C1DC4(pl->unk8, a->unk24->unk14.vz << 16), pl->unkC) >> 16;
        a->unk24->unk14.vy += (s16)D_800C9DF0.unk1C[n1->unk4].unkA[1] - D_800C9DF0.unk1C[n1->unk4].unk4[1];
    } else {
        Unk800C9DF8Tri *t3;
        Unk800C9DFC *vt;
        t3 = D_800C9DF0.unk1C + n1->unk4;
        vt = D_800C9DF0.unkC + n1->unkC[0];
        a->unk24->unk14.vy = (s16)(vt->unk2 + (D_800C9DF0.unk8->unk8[1] + t3->unkA[1]));
    }
    return 1;
}
