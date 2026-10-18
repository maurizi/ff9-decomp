#include "fxmod/fx_0e3_1.h"

void func_801EC71C(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 0xC;
    bolt->unk0 = 0xC;
    bolt->unk1 = 0;
    for (i = 0; i < 13; i++) {
        t = 0x1000 - (i << 12) / 12;
        t += ex->btlfx_randCentered(0x180);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0x80);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = 0x1C;
        ang += dang + ex->btlfx_randCentered(0x100);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x15), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

void func_801EC910(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang, s32 dang,
                   s32 arg7, s32 arg8) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 4;
    bolt->unk0 = 4;
    bolt->unk1 = 0;
    for (i = 0; i < 5; i++) {
        t = 0x1000 - (i << 10);
        t += ex->btlfx_randCentered(0x80);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0x20);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += (ex->math_sin(t >> 1) * amp) >> 12;
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = 0x1A;
        ang += dang + ex->btlfx_randCentered(0xC0);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x11), 0, 0, arg7, 0x1000, arg8, 1, (u8 *)bolt, 0);
}

void func_801ECAF8(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 col) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 56;
    bolt->unk0 = 56;
    bolt->unk1 = 0;
    for (i = 0; i < 56 + 1; i++) {
        t = 0x1000 - (i << 12) / 56;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0xA0);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = col;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0xA), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

void func_801ECCF4(Unk80100C78Entry *entry, Fx0e31Bolt *src, Fx0e31Bolt *dst, s32 arg3) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR *a = src->unk4;

    a += ex->btlfx_rand() % 56;

    ex->btlfx_lerpVec((s16 *)a, (s16 *)(a + 1), 0x4000, (s16 *)&p);
    func_801EC910(entry, dst, a, &p, 0x80, ex->btlfx_rand(), 0x37, 0, arg3);
}

void func_801ECDEC(Unk80100C78Entry *entry, Fx0e31Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9, s32 col) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801F0A04);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 12;
    bolt->unk0 = 12;
    bolt->unk1 = 0;
    for (i = 0; i < 12 + 1; i++) {
        t = 0x1000 - (i << 12) / 12;
        t += ex->btlfx_randCentered(0x100);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0xA0);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = col;
        ang += dang + ex->btlfx_randCentered(0x80);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x23), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", func_801ECFE4);

void func_801ED708(VECTOR *pos, SVECTOR *rot) {
    Unk800F78A4 *env = D_801F0A04;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 tmp;
    s32 a;

    v.vx = pos->vx;
    v.vy = pos->vy;
    v.vz = pos->vz;
    ex->func_800D0674(&env->unk2C->unk54, &v, rot, (s32)&tmp, (s32)&tmp, &a);
    rot->vz = -a + 0x400;
}

INCLUDE_RODATA("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", fx_0e3_1_headerPtr);

INCLUDE_ASM("asm/us/fx_0e3_1/nonmatchings/fx_0e3_1", fx_0e3_1_op80);
