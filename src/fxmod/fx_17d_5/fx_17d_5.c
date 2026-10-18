#include "fxmod/fx_17d_5.h"

s32 func_801EC704(s32 mode, Fx17d5Spark *w, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx17d5Work80 *parent) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    VECTOR pos;
    VECTOR scale;
    SVECTOR *p;
    Pool6CEntry *e;
    s32 k;
    s32 a;
    s32 t; /* one variable for cases 2 and 3: register allocation */
    s32 s;
    s32 c;
    s32 g; /* separate from c: each stays block-local (allocation) */
    s32 m;
    s32 b;

    if (mode == 1) {
        w->unk4 = 0x18;
        return 0;
    }
    switch (w->unk0) {
    case 0:
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 5), (s32)&w->unk8, NULL, -1, 0, 0, 0, w->unk18, 0);
        }
        if (frame < 20) {
            w->unk2 += ex->math_cos((frame << 10) / 20) >> 7;
            w->unk4 += 0x20;
        } else if (frame < 24) {
            w->unk2 -= 0x20;
            w->unk4 += 0x80;
        } else {
            w->unk2 += 0x20;
            if (w->unk2 > 2000) {
                w->unk2 = 2000;
            }
            w->unk4 += 0x18;
            if (w->unk4 > 0x991) {
                w->unk4 = 0x992;
            }
        }
        p = &w->unk8;
        if (w->unk14 == 0) {
            ex->btlfx_aimAngles((s16 *)p, (s16 *)&parent->unk14, (s16 *)&ang);
            ex->btlfx_turnToward((s16 *)&ang, &w->unk10, w->unk4);
            k = frame * 12;
            if (k > 0xF5) {
                k = 0xF5;
            }
            a = w->unk6 * 0x9B + (frame << 7);
            w->unk10 += (ex->math_sin(a + w->unk12) * k) >> 12;
            w->unk12 -= (ex->math_cos(a - w->unk10) * k) >> 12;
            ex->func_800CFD78((s16 *)p, &w->unk10, w->unk2, (s16 *)p);
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 5), (s32)p, NULL, 0, k * 16 + 0x140, 0x1000, 1,
                                w->unk18, 0);
            if (parent->unk0 > 0) {
                w->unk14 = 1;
                ((s16 *)w)[-1] = -1;
            }
        } else {
            ex->btlfx_aimAngles((s16 *)p, (s16 *)&parent->unk14, (s16 *)&ang);
            ex->btlfx_turnToward((s16 *)&ang, &w->unk10, w->unk4);
            k = frame * 4;
            if (k > 0x80) {
                k = 0x80;
            }
            a = w->unk6 * 0x9B + frame * 24;
            w->unk10 += (ex->math_sin(a + w->unk12) * k) >> 12;
            w->unk12 -= (ex->math_cos(a - w->unk10) * k) >> 12;
            ex->func_800CFD78((s16 *)p, &w->unk10, w->unk2, (s16 *)p);
            env->unk28 = 0xFF;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 5), (s32)p, NULL, 0, w->unk22 - frame * 0x23A,
                                0x1000, 1, w->unk18, 0);
            env->unk28 = 0;
            if (w->unk6 == 1) {
                env->unk2C->unk0[1].vx = w->unk8.vx;
                env->unk2C->unk0[1].vy = w->unk8.vy;
                env->unk2C->unk0[1].vz = w->unk8.vz;
                env->unk2C->unk0[1].vy -= frame << 5;
                env->unk2C->unk0[1].vz -= frame << 5;
            }
        }
        break;
    case 1:
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8, NULL, -1, 0, 0, 0, w->unk18, 0);
        }
        m = 0x1800;
        w->unk8.vz += 0x300;
        w->unk8.vy += 0x300;
        if (w->unk10 == 2) {
            m = 0x5000;
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8, NULL, 0, m, 0x1000, 1, w->unk18, 0);
        if (frame >= 7) {
            w->unk0 = 2;
            ((s16 *)w)[-1] = -1;
            if (w->unk10 == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 2, 0x40, 0x40, 0x40);
                w->unk0 = 3;
            } else if (w->unk10 != 0) {
                c = (ex->btlfx_rand() & 0x1F) + 0x28;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, c, c, c);
            } else {
                e = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&w->unk8);
                if (e != NULL) {
                    e->unk22 = 0x80;
                }
            }
            w->unk8.vy = 0;
        }
        break;
    case 2:
        t = (frame << 12) / 24;
        if (frame < 8) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8, NULL, 0, 0x1000, 0x1000, 1,
                                w->unk18, 0);
        }
        env->unk28 = 0x140;
        if (w->unk10 != 0) {
            ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &w->unk8, NULL, 0x1000, frame, 1, 0xAAA);
            env->unk28 = 0;
        } else {
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->unk8, 8), &w->unk8, NULL, 0x1000, frame, 1, 0xC00,
                                    0, 0);
            env->unk28 = 0;
        }
        if (w->unk10 != 0) {
            ang.vz = 0;
            ang.vx = 0;
            ang.vy = frame * 8;
            if (frame == 0) {
                s = 0x555;
            } else {
                s = ((ex->math_sin(t >> 2) * 9) >> 3) + 0x1000;
            }
            scale.vx = scale.vy = scale.vz = (s * 0x578) >> 12;
            pos.vx = w->unk8.vx;
            pos.vy = w->unk8.vy;
            pos.vz = w->unk8.vz;
            ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->unk88);
            b = t >> 2;
            ex->Hi_ModifyEffModelRGB(parent->unk88, (ex->math_cos(b) >> 5) - 0x80, (ex->math_cos(b) >> 5) - 0x80,
                                     (ex->math_cos(b) >> 5) - 0x80);
        }
        if (frame >= 24) {
            w->unk0 = 2;
            return 1;
        }
        break;
    case 3:
        t = frame << 7;
        if (frame < 8) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->unk8, 9), (s32)&w->unk8, NULL, 0, 0x5000, 0x1000, 1,
                                w->unk18, 0);
            if (frame < 6) {
                g = 0x80 - t / 6;
                ex->btlfx_drawScreenTint((u32 *)(env->unk20 + 2), 1, g, g, g);
            }
        }
        env->unk28 = 0x400;
        ex->func_800D23D8((u8 *)ex->btlfx_getFileData(entry->unk8, 4), &w->unk8, NULL, 0x1800, frame, 1, 0x1000);
        env->unk28 = 0;
        ang.vz = 0;
        ang.vx = 0;
        ang.vy = frame * 12;
        if (frame == 0) {
            s = 0x800;
        } else if (t < 0xC00) {
            s = ex->math_sin(t / 3) * 2 / 3 + 0xC00;
        } else {
            s = 0x16AA;
        }
        scale.vx = scale.vy = scale.vz = s >> 2;
        pos.vx = w->unk8.vx;
        pos.vy = w->unk8.vy;
        pos.vz = w->unk8.vz;
        ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->unk90);
        ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->unk94);
        ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, parent->unk8C);
        if (frame >= 32) {
            w->unk0 = 2;
            return 1;
        }
        break;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_17d_5/nonmatchings/fx_17d_5", fx_17d_5_op80);

INCLUDE_RODATA("asm/us/fx_17d_5/nonmatchings/fx_17d_5", fx_17d_5_headerPtr);
