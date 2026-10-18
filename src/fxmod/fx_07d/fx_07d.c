#include "fxmod/fx_07d.h"

INCLUDE_ASM("asm/us/fx_07d/nonmatchings/fx_07d", func_801E7718);

void func_801E7ECC(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 a3, s32 *a4, s32 *a5, s32 a6, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9FB0);

    ex->func_800D1680((u8 *)ex->btlfx_getFileData(D_801EA534->unk8, 3), pos, (SVECTOR *)a3, *a4 / 2, frame, 1, *a5, 0,
                      0);
    BTLFX_COPY_SVEC(&D_801EA470[D_801EA530], pos);
    D_801EA530 = (D_801EA530 + 1) % 24;
}

void func_801E7FF4(s32 r, s32 a1, s32 a, s32 a3, s16 *out) {
    BtlfxExports *ex;
    s32 t;
    s32 x;
    s32 y;

    t = ((0x6C - r) << 12) / 108;
    t = (t * D_801EA538) >> 12;
    r = (r * 9) >> 3;
    ex = BTLFX_EXPORTS(D_801E9FB0);
    a += 0x9B;
    a += t;
    x = (ex->math_cos(a) * r) >> 12;
    y = (ex->math_sin(a) * r) >> 12;
    out[0] = x;
    out[1] = y;
}

void func_801E80C4(s16 *a, s16 *b) {
    BTLFX_EXPORTS(D_801E9FB0)->btlfx_aimAngles((s16 *)&D_801E9FB0->unk2C->unk54, a, b);
}

INCLUDE_ASM("asm/us/fx_07d/nonmatchings/fx_07d", func_801E8108);

INCLUDE_RODATA("asm/us/fx_07d/nonmatchings/fx_07d", fx_07d_headerPtr);

INCLUDE_ASM("asm/us/fx_07d/nonmatchings/fx_07d", fx_07d_op80);

void func_801E9834(Unk80100C78Entry *entry, Fx07dStrip *buf, s32 *tab, SVECTOR *from, SVECTOR *to, s32 r0, s32 r1, s32 s,
                   s32 step, s32 a9, s32 a10, s32 pad) {
    Unk800F78A4 *env = D_801E9FB0;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR ang;
    s32 i;
    s32 t;
    s32 h;
    SVECTOR *v;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    v = buf->unk4;
    buf->unk0 = buf->unk2 = 14;
    buf->unk1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
        t += *tab++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *tab++;
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += ex->btlfx_lerp(r0, r1, t);
        ex->func_800D044C(&pos, (s32)v, 0, (XY32 *)&ang, h, s);
        v->pad = pad;
        s += step + *tab++;
        v++;
    }
    env->unk28 = 0x200;
    ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 8), 0, NULL, a9, 0x1000, a10 * 2 / 3, 2, (u8 *)buf, 0);
    env->unk28 = 0;
}

s32 func_801E9A40(s32 mode, Fx07dArc *arc, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    if (mode != 1) {
        func_801E9834(entry, &D_801EA540[arc->unk10], arc->unk14, &arc->unk0, &arc->unk8, 0, 0x200, frame * 0x9B, 0x186,
                      2, 0x1000, ((0x1000 - (frame << 12) / 5) << 6) >> 12);
        if (frame >= 5) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_07d/nonmatchings/fx_07d", fx_07d_op83);
