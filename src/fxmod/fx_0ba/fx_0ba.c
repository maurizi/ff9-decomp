#include "fxmod/fx_0ba.h"

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", func_801E7704);

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_op80);

void func_801E8778(Unk80100C78Entry *entry, Fx0baStrip *buf, SVECTOR *from, SVECTOR *to, s32 amp, s32 s, s32 step,
                   s32 a7, s32 a8, s32 pad) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC5F0);
    SVECTOR pos;
    SVECTOR ang;
    SVECTOR *v;
    s32 i;
    s32 t;
    s32 h;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    v = buf->unk4;
    buf->unk0 = buf->unk2 = 24;
    buf->unk1 = 0;
    for (i = 0; i < 25; i++, v++) {
        t = 0x1000 - (i << 12) / 24;
        t += ex->btlfx_randCentered(0x140);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        h = ex->btlfx_randCentered(0x80);
        h = (ex->math_sin(t >> 1) * h) >> 12;
        h += 0x20 + ((ex->math_sin(t >> 1) * amp) >> 12);
        ex->func_800D044C(&pos, (s32)v, 0, (XY32 *)&ang, h, s);
        v->pad = pad;
        s += step + ex->btlfx_randCentered(0x100);
    }
    ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 0x1E), 0, NULL, a7, 0x1000, a8, 1, (u8 *)buf, 0);
}

INCLUDE_ASM("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_op81);

s32 fx_0ba_op82(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s16 *keys;
    SVECTOR v;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        D_801EC5F0 = env;
    } else {
        frame = entry->unk0;
        keys = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x2E);
        i = frame;
        if (i >= keys[1]) {
            i = keys[1] - 1;
        }
        keys = (s16 *)((u8 *)keys + (i * 6 + 4));
        v.vx = keys[0];
        v.vy = keys[1] - 100;
        v.vz = keys[2];
        env->unk2C->unk0[0].vx = v.vx;
        env->unk2C->unk0[0].vy = v.vy;
        env->unk2C->unk0[0].vz = v.vz;
        keys = (s16 *)ex->btlfx_getFileData(entry->unk8, 0x2F);
        i = frame;
        if (i >= keys[1]) {
            i = keys[1] - 1;
        }
        keys = (s16 *)((u8 *)keys + (i * 6 + 4));
        v.vx = keys[0];
        v.vy = keys[1] - 100;
        v.vz = keys[2];
        env->unk2C->unk0[1].vx = v.vx;
        env->unk2C->unk0[1].vy = v.vy;
        env->unk2C->unk0[1].vz = v.vz;
        if (frame >= 0x5A) {
            return 1;
        }
    }
    return 0;
}

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

s32 func_801EBFB8(s32 mode, Fx0baPart *p, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 bias[2];
    s32 c;
    u8 *q;
    s32 (*fn)(Pool6CEntry *, s32, SVECTOR *, SVECTOR *);

    if (mode == 1) {
        p->unk12 = ex->btlfx_randCentered(0x20) + 0x1080;
    } else {
        c = 0x1000 - (frame << 12) / 5;
        q = p->unk18;
        if (frame != 0) {
            c >>= 1;
        }
        ((s16 *)q)[3] = (0x16 - frame * 4) >> 1;
        fn = ex->func_800E0014;
        bias[0] = 0;
        bias[1] = 0;
        fn((Pool6CEntry *)p, 0, (SVECTOR *)bias, (SVECTOR *)q);
        if (p->unk16 == 0) {
            p->unk18[-0x10] = p->unk18[-0xE] = 0x10;
            p->unk18[-0xF] = 0;
            ex->func_800D1494((u8 *)ex->btlfx_getFileData(entry->unk8, 0x32), 0, NULL, frame, 0x1000, c, 1,
                              p->unk18 - 4, 0);
        }
        if (frame >= 5) {
            return 1;
        }
    }
    return 0;
}

void func_801EC124(void) {
    D_801EEFDC = 0;
}

void func_801EC130(Unk80100C78Entry *entry, SVECTOR *center, s32 r, SVECTOR *from, SVECTOR *to) {
    Fx0baTrail *trail = &D_801EED20[D_801EEFDC];
    SVECTOR *p = &trail->unk4[16];
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EC5F0);
    SVECTOR v;
    Fx0baPart *part;
    s32 i;
    s32 t;

    trail->unk0 = trail->unk2 = 16;
    trail->unk1 = 0;
    for (i = 0; i < 17; i++, p--) {
        t = 0x1000 - (i << 8);
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        v.vx += 0x20 + ex->btlfx_randCentered(0x10);
        v.vy += 0x46 + ex->btlfx_randCentered(0x20);
        ex->func_800CFD78((s16 *)center, (s16 *)&v, r + ex->btlfx_randCentered(0x10) + (ex->math_sin(t >> 1) >> 6),
                          (s16 *)p);
        part = ex->btlfx_poolSpawn((s16 *)entry->unk10);
        if (part != NULL) {
            part->unk14 = D_801EEFDC;
            part->unk16 = 0x10 - i;
            part->unk18 = (u8 *)p;
            part->unk0 = p->vx << 12;
            part->unk4 = p->vy << 12;
            part->unk8 = p->vz << 12;
            part->unkC = 0;
            part->unkE = 0;
            part->unk10 = 0;
            part->unkC = ((p->vx - center->vx) >> 5) + ex->btlfx_randCentered(0xE);
            part->unkE = ((p->vy - center->vy) >> 5) + ex->btlfx_randCentered(0x18);
            part->unk10 = ((p->vz - center->vz) >> 5) + ex->btlfx_randCentered(0xE);
        }
    }
    D_801EEFDC = (D_801EEFDC + 1) % 5;
}

s32 fx_0ba_op85(s32 mode, Fx0baWork85 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;
    SVECTOR c;

    if (mode == 0) {
        ((EffInit *)arg4)->unk0 = sizeof(Fx0baWork85);
        ((EffInit *)arg4)->unk8 = sizeof(Fx0baPart);
        ((EffInit *)arg4)->unk14 = (s32)func_801EBFB8;
        ((EffInit *)arg4)->unk4 = 0x55;
    } else if (mode == 1) {
        D_801EC5F0 = env;
        func_801EC124();
        work->unk8.vx = 0;
        work->unk8.vy = 0;
        work->unk8.vz = 0;
    } else {
        if (D_801EC54C == 0) {
            BTLFX_COPY_SVEC(&from, &work->unk8);
            work->unk8.vx += 0x300 + (ex->btlfx_rand() & 0xFF);
            work->unk8.vy += 0x800 + (ex->btlfx_rand() & 0x1FF);
            BTLFX_COPY_SVEC(&c, &D_801ECAB0);
            c.vy -= 0x140;
            func_801EC130(entry, &c, 0x100, &from, &work->unk8);
        }
        work->unk8.vx &= 0xFFF;
        work->unk8.vy &= 0xFFF;
        if (D_801EC54C != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ba/nonmatchings/fx_0ba", fx_0ba_headerPtr);
