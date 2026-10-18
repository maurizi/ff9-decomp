#include "fxmod/fx_19f.h"

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", func_801E7704);

void func_801E7894(Unk80100C78Entry *entry, Fx19fTrail *trail, SVECTOR *from, SVECTOR *to, s32 amp, s32 arg5, s32 arg6,
                   s32 arg7, s32 arg8, s32 arg9) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 h;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    i = 0;
    p = trail->unk4;
    trail->unk0 = trail->unk2 = 0x18;
    trail->unk1 = 0;
    do {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x140);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0x80);
        a = t >> 1;
        h = (ex->math_sin(a) * h) >> 12;
        i++;
        h += 0x20 + ((ex->math_sin(a) * amp) >> 12);
        ex->func_800D044C(&pos, (s32)p, 0, (XY32 *)&rot, h, arg5);
        p->pad = arg9;
        arg5 += arg6 + ex->btlfx_randCentered(0x100);
        p++;
    } while (i < 25);
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 0x20), 0, NULL, arg7, 0x1000, arg8, 1, (u8 *)trail, 0);
}

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_op81);

s32 fx_19f_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx19fKeys *k;
    s32 frame;
    s32 f;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801EAA48 = env;
    } else {
        frame = entry->unk0;
        k = (Fx19fKeys *)ex->btlfx_getFileData(entry->unk8, 0x2B);
        f = frame;
        if (f >= k->unk2) {
            f = k->unk2 - 1;
        }
        k = (Fx19fKeys *)((u8 *)k + (f * 6 + 4));
        v.vx = k->unk0;
        v.vy = k->unk2 - 100;
        v.vz = ((s16 *)k)[2];
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        k = (Fx19fKeys *)ex->btlfx_getFileData(entry->unk8, 0x2C);
        f = frame;
        if (f >= k->unk2) {
            f = k->unk2 - 1;
        }
        k = (Fx19fKeys *)((u8 *)k + (f * 6 + 4));
        v.vx = k->unk0;
        v.vy = k->unk2 - 100;
        v.vz = ((s16 *)k)[2];
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        if (frame >= 0x5A) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", func_801E9968);

void func_801E9C08(Unk80100C78Entry *entry, s32 file, s32 ot, Fx19fTrail *path, Fx19fTrail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801E9968(entry, file, ot, trail, n, noise, pt, &mid, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

s32 func_801E9D20(s32 mode, Fx19fBolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 c;
    s32 k;
    s32 s;
    s32 kind;
    Fx19fTrail *tr;
    Fx19fTrail *tr2;

    if (mode != 1) {
        kind = b->unk11;
        if (kind < 2) {
            if (kind < 0) {
                return 0;
            }
            t = frame << 9;
            c = ex->math_cos(t >> 2);
            tr = (Fx19fTrail *)&D_801EB3B8[b->unk10];
            tr2 = (Fx19fTrail *)&D_801EC538[b->unk10];
            if (b->unk11 == 0) {
                b->unk8.vx = D_801EA9AC.vx;
                b->unk8.vy = D_801EA9AC.vy;
                b->unk8.vz = D_801EA9AC.vz;
                func_801E9968(entry, 0x1E, 0, tr, 0x8C, b->unk18, &b->unk0, &b->unk8, 0, 0x180, frame * 0x97, 0x140, frame,
                              c, ((0x1000 - t) * b->unk15) >> 12, ((t * 3 * 128) >> 12) + 0x80);
            } else {
                if (frame == 0) {
                    k = 2;
                } else if (frame < 4) {
                    k = 3;
                } else {
                    k = 0;
                }
                s = ((0x1000 - t) * b->unk15) >> 12;
                if (frame == 0) {
                    s = (s * 3) >> 1;
                }
                func_801E9968(entry, 2, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk0, &b->unk8, 0, 0x180, frame * 0xC0,
                              0x168, frame, c, s, 0);
                func_801E9C08(entry, 3, 0, tr, tr2, 0x24, b->unk18, b->unk12, frame, c, s >> 2);
            }
            if (frame >= 8) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801E9FC4(SVECTOR *out, SVECTOR *arg1, s32 arg2, s32 arg3) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAA48);
    SVECTOR *p = D_801EB3BC[ex->btlfx_rand() % 32].unk0;

    p += ex->btlfx_rand() % 17;

    out->vx = p->vx;
    out->vy = p->vy;
    out->vz = p->vz;
    out->pad = 1;
    return 0;
}

INCLUDE_ASM("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_op83);

INCLUDE_RODATA("asm/us/fx_19f/nonmatchings/fx_19f", fx_19f_headerPtr);
