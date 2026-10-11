/*
 * ovl_0a func_800C65CC (0x638): walkmesh triangle transfer: express the actor position (a->unk24->unk14)
 * relative to triangle n0 by barycentric-like cross products (nv) and re-project it onto triangle n1;
 * the height then comes from n1's plane (func_800C1DC4 fixed-point, pl = D_800C9DF0.unk14[n1->unk6]) or,
 * without a plane, from n1's first vertex.
 * WIP (pass 15, draft moved here from build/drafts/ovl_0a/nm_0a_C65CC.c): 58 aligned diff rows, 399 vs 398.
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
    s32 new_var;
    s32 area;
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
    new_var = v[1].vx - p.vx;
    v[1].vz = vtx[n0->unkC[1]].unk4;
    v[2].vz = vtx[n0->unkC[2]].unk4;
    p.vz = a->unk24->unk14.vz - o.vz;
    nv.vx = new_var * (v[2].vz - p.vz) - (v[2].vx - p.vx) * (v[1].vz - p.vz);
    nv.vy = (v[2].vx - p.vx) * (v[0].vz - p.vz) - (v[0].vx - p.vx) * (v[2].vz - p.vz);
    nv.vz = (v[0].vx - p.vx) * (v[1].vz - p.vz) - (v[1].vx - p.vx) * (v[0].vz - p.vz);
    area = (v[1].vx - v[0].vx) * (v[2].vz - v[0].vz) - (v[2].vx - v[0].vx) * (v[1].vz - v[0].vz);
    t = &D_800C9DF0.unk1C[n1->unk4];
    o.vx = b->unk8[0] + t->unkA[0];
    o.vy = b->unk8[1] + t->unkA[1];
    o.vz = b->unk8[2] + t->unkA[2];
    v[0].vx = vtx[n1->unkC[0]].unk0;
    v[1].vx = vtx[n1->unkC[1]].unk0;
    v[2].vx = vtx[n1->unkC[2]].unk0;
    r.vx = (nv.vx * v[0].vx + nv.vy * v[1].vx + nv.vz * v[2].vx) / area;
    v[0].vy = vtx[n1->unkC[0]].unk2;
    v[1].vy = vtx[n1->unkC[1]].unk2;
    v[2].vy = vtx[n1->unkC[2]].unk2;
    v[0].vz = vtx[n1->unkC[0]].unk4;
    v[1].vz = vtx[n1->unkC[1]].unk4;
    v[2].vz = vtx[n1->unkC[2]].unk4;
    r.vz = (nv.vx * v[0].vz + nv.vy * v[1].vz + nv.vz * v[2].vz) / area;
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
