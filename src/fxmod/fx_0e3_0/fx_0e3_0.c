#include "fxmod/fx_0e3_0.h"

INCLUDE_ASM("asm/us/fx_0e3_0/nonmatchings/fx_0e3_0", func_801E7730);

void func_801E7B40(Unk80100C78Entry *entry, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1, s32 ang,
                   s32 dang, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA868);
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
        t += ex->btlfx_randCentered(D_801EB370);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(D_801EB374);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += ex->btlfx_lerp(amp0, amp1, t);
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = 0x19;
        pt++;
        ang += dang + ex->btlfx_randCentered(D_801EB378);
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 8), 0, 0, arg8, 0x1000, arg9, 1, (u8 *)bolt, 0);
}

void func_801E7D40(Unk80100C78Entry *entry, s32 arg1, Fx0e30Bolt *bolt, SVECTOR *from, SVECTOR *to, s32 amp, s32 ang,
                   s32 dang, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EA868);
    SVECTOR p;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = bolt->unk4;
    bolt->unk2 = 0x30;
    bolt->unk0 = 0x30;
    bolt->unk1 = 0;
    for (i = 0; i < 0x31; i++) {
        t = 0x1000 - (i << 12) / 48;
        t += ex->btlfx_randCentered(0x97);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&p);
        a = ex->btlfx_randCentered(0x73);
        a = (ex->math_sin(t >> 1) * a) >> 12;
        a += (ex->math_sin(t >> 1) * amp) >> 12;
        ex->func_800D044C(&p, (s32)pt, 0, (XY32 *)&rot, a, ang);
        pt->pad = 0x33;
        ang += dang + ex->btlfx_randCentered(0x100);
        pt++;
    }
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 2), 0, (s32 *)arg1, arg8, 0x1000, arg9, 1, (u8 *)bolt, 1);
}

s32 func_801E7F48(s32 mode, Fx0e30Puff *p, Unk800F78A4 *env, Unk80100C78Entry *entry) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR pos;
    s32 n;
    s32 t;
    s32 a;
    s32 size;

    if (mode == 1) {
        p->unk8 = 0;
        p->unkE = (ex->btlfx_rand() & 0x1F) + 0x100;
        p->unkC = (ex->btlfx_rand() & 0x1FF) + 0x1000;
    } else {
        n = p->unk8 + p->unkE;
        t = n >> 2;
        p->unk8 = n;
        a = 0x1000 - ex->math_cos(t);
        size = (p->unkC * ((n >> 1) + 0x800)) >> 12;
        rot.pad = 0;
        rot.vz = 0;
        rot.vy = 0;
        rot.vx = 0;
        pos.vx = p->unk0.vx;
        pos.vy = p->unk0.vy - (0x1000 - ex->math_cos(t)) * 3;
        pos.vz = p->unk0.vz;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &pos, &rot, size * 2, 0, 1, a, 0, 0);
        if (p->unk8 > 0x1000) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0e3_0/nonmatchings/fx_0e3_0", fx_0e3_0_headerPtr);

INCLUDE_ASM("asm/us/fx_0e3_0/nonmatchings/fx_0e3_0", fx_0e3_0_op80);
