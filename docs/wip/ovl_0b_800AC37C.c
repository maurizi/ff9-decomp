/* WIP func_800AC37C: 175 aligned diff rows, 41 with registers masked, 1103 of 1100 insns. Pass 11 notes: the original keeps tv (0xE0) in t7 and tu (0) in s1 from the top; quads 0/1 load 0x1F/0xFF into t5/t3 at first use, quad 2 re-loads them into a1/a0, and quad 3 reuses t5/t3 but uses s1 (not $zero) for tu, so CSE paths differ per quad (quad 2 sits on a path without quads 0/1). Ours puts the full address of D_8006794C in t7 (lo_sum CSEd into a register) where the original only keeps %hi in t0, which pushes tv to s1 and tu to s2. tu/tv as u8/u32/s16 change nothing. */
#include "ovl_0b.h"

#define SHADOW_PT(x, z) \
    v.vx = (x); \
    v.vy = 0; \
    v.vz = (z); \
    gte_ldv0(&v); \
    gte_rtps()

#define SHADOW_QUAD(q) e->unk8[D_8006794C->unk1C->unk8][q]

u8 *func_800AC37C(Battle11C028 *arg0, s32 arg1, s32 arg2, s32 arg3, u8 *prim) {
    MATRIX m;
    s32 pad;
    SVECTOR v;
    s32 z[9];
    Battle11C028 *e;
    BtlObj *o;
    s32 i;
    s32 k;
    s32 tu;
    s32 tv;

    e = &arg0[arg1];
    o = (BtlObj *)func_8001E350(arg1);
    if (o->unk0 & 0x10) {
        return prim;
    }
    func_800AD4E4((Unk800AD4E4 *)e, o, &m, arg2, arg3 & 0xFF, (arg3 >> 8) & 0xFF);
    for (i = 0; i < 4; i++) {
        SHADOW_QUAD(i) = (POLY_FT4 *)prim;
        setPolyFT4(SHADOW_QUAD(i));
        setRGB0(SHADOW_QUAD(i), e->unk0_0, e->unk0_0, e->unk0_0);
        setSemiTrans(SHADOW_QUAD(i), 1);
        SHADOW_QUAD(i)->tpage = 0xC8;
        SHADOW_QUAD(i)->clut = 0x3FD0;
        prim += sizeof(POLY_FT4);
    }
    tu = 0;
    tv = 0xE0;
    gte_MulMatrix0(&D_8006794C->unk1C->unk18, &m, &m);
    gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
    gte_ldlv0(m.t);
    gte_rtv0tr();
    gte_stir(m.t);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    SHADOW_PT(-0x1000, 0x1000);
    gte_stsxy(&SHADOW_QUAD(0)->x0);
    gte_stsz(&z[0]);
    SHADOW_PT(0, 0x1000);
    gte_stsxy(&SHADOW_QUAD(0)->x1);
    gte_stsxy(&SHADOW_QUAD(1)->x0);
    gte_stsz(&z[1]);
    SHADOW_PT(0x1000, 0x1000);
    gte_stsxy(&SHADOW_QUAD(1)->x1);
    gte_stsz(&z[2]);
    SHADOW_PT(-0x1000, 0);
    gte_stsxy(&SHADOW_QUAD(0)->x2);
    gte_stsxy(&SHADOW_QUAD(2)->x0);
    gte_stsz(&z[3]);
    SHADOW_PT(0, 0);
    gte_stsxy(&SHADOW_QUAD(0)->x3);
    gte_stsxy(&SHADOW_QUAD(1)->x2);
    gte_stsxy(&SHADOW_QUAD(2)->x1);
    gte_stsxy(&SHADOW_QUAD(3)->x0);
    gte_stsz(&z[4]);
    if (z[4] == 0) {
        return prim;
    }
    SHADOW_PT(0x1000, 0);
    gte_stsxy(&SHADOW_QUAD(1)->x3);
    gte_stsxy(&SHADOW_QUAD(3)->x1);
    gte_stsz(&z[5]);
    SHADOW_PT(-0x1000, -0x1000);
    gte_stsxy(&SHADOW_QUAD(2)->x2);
    gte_stsz(&z[6]);
    SHADOW_PT(0, -0x1000);
    gte_stsxy(&SHADOW_QUAD(2)->x3);
    gte_stsxy(&SHADOW_QUAD(3)->x2);
    gte_stsz(&z[7]);
    SHADOW_PT(0x1000, -0x1000);
    gte_stsxy(&SHADOW_QUAD(3)->x3);
    gte_stsz(&z[8]);

    k = z[0];
    if (k < z[1]) {
        k = z[1];
    }
    if (k < z[3]) {
        k = z[3];
    }
    if (k < z[4]) {
        k = z[4];
    }
    setUV4(SHADOW_QUAD(0), tu, tv, tu + 0x1F, tv, tu, tv + 0x1F, tu + 0x1F, tv + 0x1F);
    addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][k >> 3], SHADOW_QUAD(0));
    k = z[1];
    if (k < z[2]) {
        k = z[2];
    }
    if (k < z[4]) {
        k = z[4];
    }
    if (k < z[5]) {
        k = z[5];
    }
    setUV4(SHADOW_QUAD(1), tu + 0x1F, tv, tu, tv, tu + 0x1F, tv + 0x1F, tu, tv + 0x1F);
    addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][k >> 3], SHADOW_QUAD(1));
    k = z[3];
    if (k < z[6]) {
        k = z[6];
    }
    if (k < z[7]) {
        k = z[7];
    }
    setUV4(SHADOW_QUAD(2), tu, tv + 0x1F, tu + 0x1F, tv + 0x1F, tu, tv, tu + 0x1F, tv);
    addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][k >> 3], SHADOW_QUAD(2));
    k = z[4];
    if (k < z[5]) {
        k = z[5];
    }
    if (k < z[7]) {
        k = z[7];
    }
    if (k < z[8]) {
        k = z[8];
    }
    setUV4(SHADOW_QUAD(3), tu + 0x1F, tv + 0x1F, tu, tv + 0x1F, tu + 0x1F, tv, tu, tv);
    addPrim(&D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8][k >> 3], SHADOW_QUAD(3));
    return prim;
}
