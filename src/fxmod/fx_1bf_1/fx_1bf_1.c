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

s32 fx_1bf_1_op83(s32 mode, Fx1bf1Work83 *work, Unk800F78A4 *env, Fx1bf1Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 *p;
    s32 i;
    s32 n;
    s32 k;
    s32 frame;
    s32 r;
    s32 a;
    Fx1bf1Bolt *b;

    if (mode == 0) {
        init->unk0 = 0xC;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801EEA80;
        init->unk4 = 5;
    } else if (mode == 1) {
        n = init->unk0;
        D_801EF2D8 = env;
        work->unk0 = D_801F0198[0];
        p = D_801F0198[0];
        for (i = 0; i < 64; i++) {
            *p++ = ex->btlfx_randCentered(0xD2);
            *p++ = ex->btlfx_randCentered(0x1C3);
            *p++ = ex->btlfx_randCentered(0xDC);
        }
        work->unk8 = n;
        work->unk4 = 0;
    } else {
        k = work->unk8;
        frame = inst->unk0;
        switch (k) {
        case 0:
            if (frame >= 5) {
                return 1;
            }
            work->unk4++;
            b = ex->btlfx_poolSpawn(inst->unk10);
            if (b != NULL) {
                ((s32 *)&b->unk0)[0] = ((s32 *)&D_801EFDE0)[0];
                ((s32 *)&b->unk0)[1] = ((s32 *)&D_801EFDE0)[1];
                ((s32 *)&b->unk8)[0] = ((s32 *)&D_801EFDE8)[0];
                ((s32 *)&b->unk8)[1] = ((s32 *)&D_801EFDE8)[1];
                b->unk11 = 0;
                b->unk10 = work->unk4 % 5;
                b->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                b->unk12 = ex->btlfx_rand() % 15;
                b->unk13 = ex->btlfx_rand() % 15;
                b->unk15 = 0x4E;
                b->unk14 = work->unk4 * 0x33 + ex->btlfx_randCentered(7);
            }
            break;
        case 1:
            if (frame < 0x1E) {
                work->unk4++;
                r = 0xD48 - frame * 0xD48 / 28;
                b = ex->btlfx_poolSpawn(inst->unk10);
                if (b != NULL) {
                    a = (frame << 11) / 3 + 0x400;
                    ((s32 *)&b->unk0)[0] = ((s32 *)&D_801EFDE8)[0];
                    ((s32 *)&b->unk0)[1] = ((s32 *)&D_801EFDE8)[1];
                    ((s32 *)&b->unk8)[0] = ((s32 *)&D_801EFDE0)[0];
                    ((s32 *)&b->unk8)[1] = ((s32 *)&D_801EFDE0)[1];
                    b->unk0.vx += (ex->math_sin(a) * r) >> 12;
                    b->unk0.vz += (ex->math_cos(a) * r) >> 12;
                    b->unk11 = k;
                    b->unk8.vy += 0x400;
                    b->unk10 = work->unk4 % 5;
                    b->unk18 = work->unk0 + (ex->btlfx_rand() & 0x1F);
                    b->unk12 = ex->btlfx_rand() % 15;
                    b->unk13 = ex->btlfx_rand() % 15;
                    b->unk15 = 0x3E - frame;
                    b->unk14 = work->unk4 * 0x33 + ex->btlfx_randCentered(7);
                }
            }
            if (frame >= 0x1E) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1bf_1/nonmatchings/fx_1bf_1", fx_1bf_1_headerPtr);
