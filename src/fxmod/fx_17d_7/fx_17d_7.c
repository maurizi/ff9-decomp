#include "fxmod/fx_17d_7.h"

s32 func_801EC704(SVECTOR *v, SVECTOR *rot) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EF2DC);
    s32 a = ex->btlfx_rand();

    v->pad = 0;
    v->vy = 0;
    v->vx = (ex->math_cos(a) * D_801EFDDC) >> 12;
    v->vz = (ex->math_sin(a) * D_801EFDDC) >> 12;
    rot->vy = a;
    return 0;
}

s32 func_801EC7C8(s32 mode, Fx17d7Rock *o, Unk800F78A4 *env, s32 arg3, s32 frame, s32 arg5, Fx17d7RockInst *inst) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    s32 t;
    s32 s;

    if (mode == 1) {
        o->unk2 = 0;
        o->unk1C = ex->btlfx_rand() % 64;
        o->unk1E = ex->btlfx_rand() % 32;
    } else {
        switch (o->unk0) {
        case 0:
            switch (o->unk2) {
            case 0:
                pos.vx = o->unkC.vx;
                pos.vy = o->unkC.vy;
                pos.vz = o->unkC.vz;
                pos.vx += ex->btlfx_randCentered(frame + 0x4D);
                pos.vy += ex->btlfx_randCentered(frame * 2 + 0x4D);
                s = o->unkA;
                scale.vx = scale.vz = s;
                scale.vy = s * 3;
                ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk84[o->unk20]);
                ex->Hi_ModifyEffModelRGB(inst->unk84[o->unk20], -0x80, -0x80, -0x80);
                if (D_801EF260 < o->unk22) {
                    o->unk2 = 1;
                    ((s16 *)o)[-1] = -1;
                }
                break;
            case 1:
                t = (frame << 12) / 24;
                if (o->unkC.vx > 0) {
                    o->unk1C -= (0xE10 - o->unkC.vz) / 300;
                } else {
                    o->unk1C += (o->unkC.vz + 0xE10) / 300;
                }
                o->unk14.vx += o->unk1C;
                o->unk14.vy += o->unk1E;
                o->unkC.vx += o->unk4;
                o->unkC.vy += o->unk6;
                o->unkC.vz += o->unk8;
                pos.vx = o->unkC.vx;
                pos.vy = o->unkC.vy;
                pos.vz = o->unkC.vz;
                s = ((0x1000 - ex->math_sin(t >> 2)) * o->unkA) >> 12;
                scale.vx = scale.vz = s;
                scale.vy = s * 3;
                ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk84[o->unk20]);
                ex->Hi_ModifyEffModelRGB(inst->unk84[o->unk20], -0x80, -0x80, -0x80);
                if (frame >= 0x18) {
                    return 1;
                }
                break;
            }
            break;
        case 1:
            t = frame << 8;
            o->unk14.vx += o->unk1C;
            o->unk14.vy += o->unk1E;
            o->unkC.vx += o->unk4;
            o->unkC.vy += o->unk6;
            o->unkC.vz += o->unk8;
            pos.vx = o->unkC.vx;
            pos.vy = o->unkC.vy;
            pos.vz = o->unkC.vz;
            s = ((0x1000 - t) * o->unkA) >> 12;
            scale.vx = scale.vz = s;
            scale.vy = s * 3;
            ex->Hi_DrawEffModel(&o->unk14, &pos, (s32)&scale, inst->unk84[o->unk20]);
            ex->Hi_ModifyEffModelRGB(inst->unk84[o->unk20], -0x80, -0x80, -0x80);
            if (frame >= 0x10) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_17d_7/nonmatchings/fx_17d_7", fx_17d_7_op80);

void func_801EE6E4(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d7Trail *trail, s32 n, s32 *noise, SVECTOR *from,
                   SVECTOR *to, s32 h0, s32 h1, s32 ang, s32 dang, s32 type, s32 alpha, s32 pad, s32 dip) {
    Unk800F78A4 *env = D_801EF2DC;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    SVECTOR rot;
    s32 i;
    s32 cnt;
    s32 t;
    s32 h;
    SVECTOR *pt;
    s32 a;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    cnt = (n - 4) / 8;
    a = cnt;
    trail->unk0 = trail->unk2 = a - 1;
    trail->unk1 = 0;
    pt = trail->unk4;
    for (i = 0; i < a; i++, pt++) {
        t = 0x1000 - (i << 12) / (a - 1);
        t += *noise++ / 2;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = *noise++ / 3;
        h = (ex->math_sin((t >> 1) >> 1) * h) >> 12;
        t >>= 1;
        h += ex->btlfx_lerp(h0, h1, ex->math_sin(t));
        ex->func_800D044C(&pos, (s32)pt, 0, (XY32 *)&rot, h, ang);
        if (dip != 0) {
            pt->vy -= (ex->math_sin(t) * dip) >> 12;
        }
        pt->pad = pad;
        ang += dang + *noise++;
    }
    env->unk28 = 0x80;
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, file), 0, NULL, type, 0x1000, alpha, 1, (u8 *)trail, ot);
    env->unk28 = 0;
}

void func_801EE990(Unk80100C78Entry *entry, s32 file, s32 ot, Fx17d7Trail *path, Fx17d7Trail *trail, s32 n, s32 *noise,
                   s32 i, s32 type, s32 alpha, s32 pad) {
    SVECTOR *pt = path->unk4;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EF2DC);
    SVECTOR mid;

    pt += i;
    ex->btlfx_lerpVec((s16 *)(pt + 1), (s16 *)pt, 0x4000, (s16 *)&mid);
    func_801EE6E4(entry, file, ot, trail, n, noise, pt, &mid, 0, 0x20, ex->btlfx_rand(), 0x33, type, alpha, pad, 0);
}

s32 func_801EEAA8(s32 mode, Fx17d7Bolt *b, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 t;
    s32 c;
    s32 k;
    s32 i;
    s32 u;
    s32 kind;
    s32 a;
    Fx17d7Trail *tr;
    Fx17d7Trail *tr2;

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
            tr = (Fx17d7Trail *)&D_801EFDFC[i];
            tr2 = (Fx17d7Trail *)&D_801F00BC[i];
            if (frame == 0) {
                u >>= 1;
            }
            b->unk14 += 3;
            k = frame < 3;
            if (b->unk11 == 0) {
                a = 0x1000 - t;
                func_801EE6E4(entry, 0xF, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk8, &b->unk0, 0, 0x3E8,
                              b->unk14 * 16, 0xF0, frame, c, a * u / 1500, 0);
                func_801EE990(entry, 0xE, 0, tr, tr2, 0x2C, b->unk18, b->unk12, frame, c, (a * u) >> 12);
                if (frame >= 0x10) {
                    return 1;
                }
            } else {
                t = frame << 10;
                c = 0x1000 - t;
                func_801EE6E4(entry, 0x13, 0, tr, 0x8C, b->unk18 + frame * k, &b->unk8, &b->unk0, 0, 0x12C,
                              b->unk14 << 7, 0x14A, frame, c, c * u / 1500, 0);
                func_801EE990(entry, 0xE, 0, tr, tr2, 0x2C, b->unk18, b->unk12, frame, c, (c * u) >> 12);
                if (frame >= 4) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

s32 fx_17d_7_op83(s32 mode, Fx17d7Work83 *work, Unk800F78A4 *env, Fx17d7Inst *inst, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 *p;
    s32 i;
    s32 n;
    s32 k;
    s32 frame;
    s32 r;
    s32 a;
    Fx17d7Bolt *b;

    if (mode == 0) {
        init->unk0 = 0xC;
        init->unk8 = 0x1C;
        init->unk14 = (s32)func_801EEAA8;
        init->unk4 = 5;
    } else if (mode == 1) {
        n = init->unk0;
        D_801EF2DC = env;
        work->unk0 = D_801F019C[0];
        p = D_801F019C[0];
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
                ((s32 *)&b->unk0)[0] = ((s32 *)&D_801EFDE4)[0];
                ((s32 *)&b->unk0)[1] = ((s32 *)&D_801EFDE4)[1];
                ((s32 *)&b->unk8)[0] = ((s32 *)&D_801EFDEC)[0];
                ((s32 *)&b->unk8)[1] = ((s32 *)&D_801EFDEC)[1];
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
                    ((s32 *)&b->unk0)[0] = ((s32 *)&D_801EFDEC)[0];
                    ((s32 *)&b->unk0)[1] = ((s32 *)&D_801EFDEC)[1];
                    ((s32 *)&b->unk8)[0] = ((s32 *)&D_801EFDE4)[0];
                    ((s32 *)&b->unk8)[1] = ((s32 *)&D_801EFDE4)[1];
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

INCLUDE_RODATA("asm/us/fx_17d_7/nonmatchings/fx_17d_7", fx_17d_7_headerPtr);
