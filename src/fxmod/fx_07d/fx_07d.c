#include "fxmod/fx_07d.h"

INCLUDE_ASM("asm/us/fx_07d/nonmatchings/fx_07d", func_801E7718);

void func_801E7ECC(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, s32 a3, s32 *a4, s32 *a5, s32 a6, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E9FB0);

    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(D_801EA534->unk8, 3), pos, (SVECTOR *)a3, *a4 / 2, frame, 1, *a5, 0,
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

s32 func_801E8108(s32 mode, Fx07dOrb *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, s32 a5, Fx07dWork80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    s32 c;
    s32 s;
    s32 m;
    s32 a;
    s32 a2;

    if (mode == 1) {
        p->unk0 = 0;
        p->unk14 = ex->btlfx_rand();
        p->unk16 = (ex->btlfx_rand() & 7) + 8;
    } else {
        a2 = (frame << 12) / 12;
        p->unk14 += p->unk16;
        func_801E80C4((s16 *)&p->unk4, (s16 *)&rot);
        rot.vz = p->unk14;
        pos.vx = p->unk4.vx;
        pos.vy = p->unk4.vy;
        pos.vz = p->unk4.vz;
        m = ex->math_sin(a2 >> 2);
        m *= 3;
        s = p->unk2;
        scale.vx = scale.vy = scale.vz = ((s * m) >> 13) + (s >> 1);
        if (frame < 3) {
            c = ((frame + 1) << 12) / 3;
        } else {
            c = ex->math_cos(((frame - 3) << 10) / 9);
        }
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk28);
        s = c >> 5;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk28, s, s, s);
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->unk2C);
        s = c * 2;
        s -= 0x1000;
        s >>= 5;
        s -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->unk2C, s, s, s);
        if (frame < 8) {
            p->unkC.vz += 0xFF;
            scale.vx = scale.vy = scale.vx / 5;
            scale.vz = scale.vx / 7;
            ex->Hi_DrawEffModel(&p->unkC, &pos, (s32)&scale, work->unk20);
            a = frame << 10;
            a >>= 3;
            ex->Hi_ModifyEffModelRGB(work->unk20, (ex->math_cos(a) >> 5) - 0x80, (ex->math_cos(a) >> 5) - 0x80,
                                     (ex->math_cos(a) >> 5) - 0x80);
        }
        env->unk28 = 0x200;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 3), &p->unk4, NULL, (p->unk2 * 3) >> 1, 0, 1, c >> 1,
                          0, 0);
        env->unk28 = 0;
        if (frame >= 12) {
            return 1;
        }
    }
    return 0;
}

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
    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 8), 0, NULL, a9, 0x1000, a10 * 2 / 3, 2, (u8 *)buf, 0);
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
