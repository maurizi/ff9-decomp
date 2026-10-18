#include "fxmod/fx_1bf_1.h"

s32 func_801EC704(SVECTOR *v, SVECTOR *rot) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EF2D8);
    s32 a = ex->btlfx_rand();

    v->pad = 0;
    v->vy = 0;
    v->vx = (ex->math_cos(a) * D_801EFDD8) >> 12;
    v->vz = (ex->math_sin(a) * D_801EFDD8) >> 12;
    rot->vy = a;
    return 0;
}

INCLUDE_ASM("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", func_801EC7C8);

INCLUDE_ASM("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", fx_1bf_1_op80);

INCLUDE_ASM("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", func_801EE6BC);

void func_801EE968(Unk80100C78Entry *entry, s32 file, s32 ot, Fx1bf1Trail *path, Fx1bf1Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EF2D8);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801EE6BC(entry, file, ot, trail, n, noise, &mid, pt, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

s32 func_801EEA80(s32 mode, Fx1bf1Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 c;
    s32 k;
    s32 i;
    s32 u;
    s32 kind;
    s32 a;
    Fx1bf1Trail *tr;
    Fx1bf1Trail *tr2;

    if (mode != 1) {
        kind = b->unk11;
        if (kind < 2) {
            if (kind < 0) {
                return 0;
            }
            t = frame << 8;
            c = ex->math_cos(t >> 2);
            i = b->unk10;
            u = b->unk15;
            tr = (Fx1bf1Trail *)&D_801EFDF8[i];
            tr2 = (Fx1bf1Trail *)&D_801F00B8[i];
            if (frame == 0) {
                u >>= 1;
            }
            b->unk14 += 3;
            k = frame < 3;
            if (b->unk11 == 0) {
                a = 0x1000 - t;
                func_801EE6BC(entry, 0xB, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk8, &b->unk0, 0, 0x3E8,
                              b->unk14 * 16, 0xF0, frame, c, a * u / 1500, 0);
                func_801EE968(entry, 0x12, 0, tr, tr2, 0x2C, b->unk18, b->unk12, frame, c, (a * u) >> 12);
                if (frame >= 0x10) {
                    return 1;
                }
            } else {
                t += frame << 9;
                c = 0x1000 - t;
                func_801EE6BC(entry, 0x11, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk8, &b->unk0, 0, 0x12C,
                              b->unk14 << 6, 0x14A, frame, c, (0x1000 - t / 2) * u / 1500, 0);
                func_801EE968(entry, 0x12, 0, tr, tr2, 0x2C, b->unk18, b->unk12, frame, c, (c * u) >> 12);
                if (frame >= 5) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", fx_1bf_1_op83);

INCLUDE_RODATA("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", fx_1bf_1_headerPtr);
