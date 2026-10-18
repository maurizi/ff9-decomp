#include "fxmod/fx_0ba.h"

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", func_801E7704);

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_op80);

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", func_801E8778);

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_op81);

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_op82);

void func_801EA850(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *buf, s32 size, s32 *tab, SVECTOR *from,
                   SVECTOR *to, s32 r0, s32 r1, s32 s, s32 step, s32 a12, s32 a13, s32 pad, s32 wave) {
    Unk800F78A4 *env = D_801EC5F0;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR ang;
    s32 i;
    s32 n;
    s32 t;
    s32 h;
    SVECTOR *v;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    n = (size - 4) / 8;
    v = buf->unk4;
    buf->unk0 = buf->unk2 = n - 1;
    buf->unk1 = 0;
    for (i = 0; i < n; i++, v++) {
        t = 0x1000 - (i << 12) / (n - 1);
        t += *tab++ * 2 / 3;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *tab++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(r0, r1, t);
        ex->func_800D044C(&pos, (s32)v, 0, (XY32 *)&ang, h, s);
        if (wave != 0) {
            v->vy -= (ex->math_sin(t >> 1) * wave) >> 12;
        }
        v->pad = pad;
        s += step + *tab++;
    }
    env->unk28 = 0x80;
    ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, file), 0, NULL, a12, 0x1000, a13, 1, (u8 *)buf, a2);
    env->unk28 = 0;
}

void func_801EAAF0(Unk80100C78Entry *entry, s32 file, s32 a2, Fx0baStrip *path, Fx0baStrip *buf, s32 size, s32 *tab,
                   s32 i, s32 a12, s32 a13, s32 pad) {
    SVECTOR *v;
    BtlfxExports *ex;
    SVECTOR to;

    v = path->unk4;
    v += i;
    ex = BTLFX_EXPORTS(D_801EC5F0);
    ex->btlfx_lerpVec((s16 *)(v + 1), (s16 *)v, 0x4000, (s16 *)&to);
    func_801EA850(entry, file, a2, buf, size, tab, v, &to, 0, 0x20, ex->btlfx_rand(), 0x33, a12, a13, pad, 0);
}

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", func_801EAC08);

s32 func_801EB1CC(SVECTOR *v) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC5F0);
    SVECTOR *p = D_801ED424[ex->btlfx_rand() % 32].unk0;

    p += ex->btlfx_rand() % 17;
    v->vx = p->vx;
    v->vy = p->vy;
    v->vz = p->vz;
    v->pad = 1;
    return 0;
}

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_op83);

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", func_801EBFB8);

void func_801EC124(void) {
    D_801EEFDC = 0;
}

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", func_801EC130);

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_op85);

INCLUDE_RODATA("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_headerPtr);
