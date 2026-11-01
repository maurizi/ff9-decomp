#include "fxmod/fx_003.h"

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx003Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 arg10, s32 arg11, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_003_env);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->pts4;
    strip->count2 = 0xF;
    strip->lastSlot0 = 0xF;
    strip->head1 = 0;
    for (i = 0; i < 16; i++) {
        t = 0x1000 - (i << 12) / 15;
        t += *tbl++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        s = *tbl++;
        s = (ex->math_sin(t >> 1) * s) >> 12;
        s += ex->btlfx_lerp(from1, to1, t);
        ex->btlfx_offsetPoint(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg11;
        width += dwidth + *tbl++;
        p++;
    }
    if (kind < 2) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_003_strip16Blocks[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_003_strip16Blocks[kind]), 0, NULL, frame, 0x1000, arg10, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx003Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 arg9, s32 arg10, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_003_env);
    SVECTOR v;
    SVECTOR ang;
    SVECTOR *p;
    s32 i;
    s32 t;
    s32 s;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    p = strip->pts4;
    strip->count2 = 3;
    strip->lastSlot0 = 3;
    strip->head1 = 0;
    for (i = 0; i < 4; i++) {
        t = 0x1000 - (i << 12) / 3;
        t += *tbl++ / 2;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&v);
        s = *tbl++ / 2;
        t >>= 1;
        s = (ex->math_sin(t) * s) >> 12;
        s += (ex->math_sin(t) * amp) >> 12;
        ex->btlfx_offsetPoint(&v, (s32)p, 0, (XY32 *)&ang, s, width);
        p->pad = arg10;
        width += dwidth + *tbl++ / 2;
        p++;
    }
    if (kind < 2) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_003_strip4Blocks[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_003_strip4Blocks[kind]), 0, NULL, frame, 0x1000, arg9, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx003Strip *strip, Fx003Strip *out, s32 *tbl, s32 i, s32 arg5, s32 arg6,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->pts4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(fx_003_env);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    fxm_drawJaggedStrip4(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x37, 0, arg5, arg6, kind);
}

static s32 fx_003_op83PoolTask(s32 mode, Fx003Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx003Strip *strip;
    Fx003Strip *br;
    s32 c;
    s32 t;

    if (mode == 1) {
        it->state11 = 0;
        return 0;
    }
    switch (it->state11) {
    case 0:
        if (it->kind14 < 2) {
            t = (frame << 12) / 6;
            c = ex->math_cos(t >> 2);
            strip = &fx_003_strips[it->strip10];
            br = (Fx003Strip *)&fx_003_branches[it->strip10];
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame * 0x9B, 0x186, frame, c,
                          ((0x1000 - t) * 0xF1) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, c, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18 + 3, it->branchAt13, c, (0x1000 - t) >> 6, it->kind14);
            if (frame == 0) {
                it->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&it->pos0);
                if (it->emit1C != NULL) {
                    it->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                env->otZ = ex->btlfx_addRadiusSixth(0, 0x46);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xB), &it->pos0, NULL, 0x1400, 4, 1, 0x1000);
                env->otZ = 0;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 7, 0x20);
            }
            if (frame == 1) {
                it->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&it->pos0);
                if (it->emit20 != NULL) {
                    it->emit20->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 6) {
                it->state11 = 1;
                ((s16 *)it)[-1] = -1;
            }
        } else {
            t = frame << 10;
            c = ex->math_cos(t >> 2);
            strip = &fx_003_strips[it->strip10];
            br = (Fx003Strip *)&fx_003_branches[it->strip10];
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame << 7, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, c, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18 + 3, it->branchAt13, c, (0x1000 - t) >> 6, it->kind14);
            if (frame >= 4) {
                return 1;
            }
        }
        break;
    case 1:
        if (frame >= 0x10) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_003_op83(s32 mode, Fx003Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 r;
    s32 ang;
    Fx003Bolt *it;
    s32 *q;
    SVECTOR a;
    SVECTOR b;

    if (mode == 0) {
        init->workSize = 0x10;
        init->elemSize = 0x24;
        init->elemFn = (s32)fx_003_op83PoolTask;
        init->poolCount = 0xC;
    } else if (mode == 1) {
        fx_003_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        work->jitter8 = fx_003_jitter;
        q = fx_003_jitter;
        for (i = 0; i < 0x40; i++) {
            *q++ = ex->btlfx_randCentered(0x100);
            *q++ = ex->btlfx_randCentered(0x180);
            *q++ = ex->btlfx_randCentered(0xC0);
        }
        work->boltCountC = 0;
    } else {
        frame = entry->frame;
        if (frame == 0 || frame == 3 || frame == 5 || frame == 6 || frame == 7) {
            if (work->boltCountC < 5) {
                work->boltCountC++;
                BTLFX_COPY_SVEC(&a, &work->pos0);
                if (work->boltCountC == 3) {
                    a.vx += ex->btlfx_randCentered(0x190);
                    a.vy += ex->btlfx_randCentered(0x190);
                    a.vz += ex->btlfx_randCentered(0x190);
                }
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->pos0, &a);
                    BTLFX_COPY_SVEC(&b, &a);
                    b.vy -= 0xC00;
                    r = ex->btlfx_randCentered(0x100) + 0x640;
                    ang = (work->boltCountC << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    b.vx += (ex->math_cos(ang) * r) >> 12;
                    b.vz += (ex->math_sin(ang) * r) >> 12;
                    BTLFX_COPY_SVEC(&it->pos8, &b);
                    it->strip10 = work->boltCountC;
                    it->jitter18 = work->jitter8 + (ex->btlfx_rand() & 0x1F);
                    it->branchAt12 = ex->btlfx_rand() % 14;
                    it->branchAt13 = ex->btlfx_rand() % 14;
                    if ((work->boltCountC - 1) & 1) {
                        it->kind14 = 2;
                    } else {
                        it->kind14 = ex->btlfx_rand() & 1;
                    }
                }
            }
        }
        if (frame >= 0xC) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_003/nonmatchings/fx_003", fx_003_headerPtr);
