/* 11 rows differ: the original keeps pv = &v in s0 across the SetRotMatrix/SetTransMatrix calls (and even addresses mat.m[1][1] as 16(s0)); we keep &mat in s0 instead. Moving pv around and do{}while(0) did not help. Pass 14: the original addresses both mat.m[1][1] (0x10(s0)) and v.vz (4(s0)) through s0 = &v, which is set right after the first call (after ry's last use), while v.vx/v.vy and the other mat stores use sp. pv placements (before/after the first call, after the 0x1000 store), pv for all v stores, and no pv: 11-23 rows. A local struct { SVECTOR v; MATRIX m; } s (v at sp+0x20, m at sp+0x28, adjacent as in the original) with `ps = &s` after the first call and `ps->m.m[1][1] = 0x1000; ... ps->v.vz = -500; gte_ldv0(&ps->v)` reproduces every s0-relative access (16(s0), 4(s0), lwc2 0/4(s0)), but &s.m for SetRotMatrix/SetTransMatrix is then CSE'd into its own s-reg (50 rows); the original rematerialises addiu a0,sp,0x28 for both calls. Pass 15: struct { SVECTOR v; MATRIX m; } with SetRotMatrix/SetTransMatrix taking &s.m, &ps->m or a mix, ps set before or after the 0x1000 store, a do-while or a pm temp between the calls: all 50 rows (the matrix address is always one pseudo kept in an s-reg; the original rematerialises it). An __asm__ volatile("") between the calls (flushes the CSE table) makes it worse (50-51). Pass 16: in our RTL, CSE replaces the second (plus fp 40) argument with the first call's argument pseudo (95), which then lives across SetRotMatrix in an s-reg; in the original both calls rematerialise sp+0x28, so the first argument was not in CSE's table at the second call (a different extended block, or a non-pseudo argument). The s0-relative m[1][1] store needs CSE to see sp+0x30 as pv+16, which find_best_addr only does when the address is written relative to pv. */
#include "ff9.h"

s32 func_8004BB7C(s32);
s32 func_8004BB80(s32);
s32 func_80052EC0(s32, s32, s32);

void func_80047B90(Unk8004AB64 *o, Unk80047B90 *m) {
    VECTOR out;
    SVECTOR v;
    MATRIX mat;
    s32 d;
    s16 ry;
    SVECTOR *pv;

    ry = o->unk14.unk22;
    pv = &v;
    mat.m[0][2] = func_8004BB80(ry);
    mat.m[1][1] = 0x1000;
    mat.m[2][2] = func_8004BB7C(ry);
    mat.m[0][0] = mat.m[0][1] = mat.m[1][0] = mat.m[1][2] = mat.m[2][0] = mat.m[2][1] = 0;
    mat.t[0] = o->unk14.vx;
    mat.t[1] = o->unk14.vy;
    mat.t[2] = o->unk14.vz;
    SetRotMatrix(&mat);
    SetTransMatrix(&mat);
    v.vx = 0;
    v.vy = (o->unk32 * (s8)o->unk2F) >> 6;
    pv->vz = -500;
    gte_ldv0(pv);
    gte_rtv0tr();
    gte_stlvnl(&out);
    d = func_80052EC0(out.vx - o->unk84, out.vy - o->unk86, out.vz - o->unk88);
    if (d < 100) {
        d = 100;
        o->unk50 &= 0xF8FF;
    }
    o->unk84 += (out.vx - o->unk84) * 100 / d;
    o->unk86 += (out.vy - o->unk86) * 100 / d;
    o->unk88 += (out.vz - o->unk88) * 100 / d;
    m->unk0 |= 1;
    m->unk48 = o->unk84;
    m->unk4A = o->unk86;
    m->unk4C = o->unk88;
}
