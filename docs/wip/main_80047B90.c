/* 11 rows differ: the original keeps pv = &v in s0 across the SetRotMatrix/SetTransMatrix calls (and even addresses mat.m[1][1] as 16(s0)); we keep &mat in s0 instead. Moving pv around and do{}while(0) did not help. */
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
