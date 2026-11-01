#include "fxmod/fx_0d7.h"

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx0d7Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d7_env);
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
        p->pad = lineWidth;
        width += dwidth + *tbl++;
        p++;
    }
    if (kind < FX_0D7_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d7_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d7_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx0d7Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d7_env);
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
        p->pad = lineWidth;
        width += dwidth + *tbl++ / 2;
        p++;
    }
    if (kind < FX_0D7_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d7_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d7_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx0d7Strip *strip, Fx0d7Strip *out, s32 *tbl, s32 i, s32 fade, s32 lineWidth,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->pts4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(fx_0d7_env);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    fxm_drawJaggedStrip4(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x37, 0, fade, lineWidth, kind);
}

static s32 fx_0d7_op83PoolTask(s32 mode, Fx0d7Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0d7Strip *strip;
    Fx0d7Strip *br;
    s32 fade;
    s32 t;

    if (mode == 1) {
        it->state11 = 0;
        return 0;
    }
    switch (it->state11) {
    case 0:
        if (it->kind14 < FX_0D7_STRIP_FLAT) {
            t = (frame << 12) / 6;
            fade = ex->math_cos(t >> 2);
            strip = &fx_0d7_strips[it->strip10];
            br = (Fx0d7Strip *)&fx_0d7_branches[it->strip10];
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame * 0x9B, 0x186, frame, fade,
                          ((0x1000 - t) * 0xF1) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, fade, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18 + 3, it->branchAt13, fade, (0x1000 - t) >> 6, it->kind14);
            if (frame == 0) {
                it->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&it->pos0);
                if (it->emit1C != NULL) {
                    it->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                env->otZ = ex->btlfx_addRadiusSixth(0, 0x46);
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 8), &it->pos0, NULL, 0x1400, 4, 1, 0x1000);
                env->otZ = 0;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 7, 0x20);
            }
            if (frame == 1) {
                it->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&it->pos0);
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
            fade = ex->math_cos(t >> 2);
            strip = &fx_0d7_strips[it->strip10];
            br = (Fx0d7Strip *)&fx_0d7_branches[it->strip10];
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame << 7, 0x186, frame, fade,
                          ((0x1000 - t) * 0xAA) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, fade, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18 + 3, it->branchAt13, fade, (0x1000 - t) >> 6, it->kind14);
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

s32 fx_0d7_op83(s32 mode, Fx0d7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 radius;
    s32 ang;
    Fx0d7Bolt *it;
    s32 *jit;
    SVECTOR root;
    SVECTOR tip;

    if (mode == 0) {
        init->workSize = 0x10;
        init->elemSize = 0x24;
        init->elemFn = (s32)fx_0d7_op83PoolTask;
        init->poolCount = 0xC;
    } else if (mode == 1) {
        fx_0d7_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        work->jitter8 = fx_0d7_jitter;
        jit = fx_0d7_jitter;
        for (i = 0; i < 0x40; i++) {
            *jit++ = ex->btlfx_randCentered(0x100);
            *jit++ = ex->btlfx_randCentered(0x180);
            *jit++ = ex->btlfx_randCentered(0xC0);
        }
        work->boltCountC = 0;
    } else {
        frame = entry->frame;
        if (frame == 0 || frame == 3 || frame == 5 || frame == 6 || frame == 7) {
            if (work->boltCountC < 5) {
                work->boltCountC++;
                BTLFX_COPY_SVEC(&root, &work->pos0);
                if (work->boltCountC == 3) {
                    root.vx += ex->btlfx_randCentered(0x190);
                    root.vy += ex->btlfx_randCentered(0x190);
                    root.vz += ex->btlfx_randCentered(0x190);
                }
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->pos0, &root);
                    BTLFX_COPY_SVEC(&tip, &root);
                    tip.vy -= 0xC00;
                    radius = ex->btlfx_randCentered(0x100) + 0x640;
                    ang = (work->boltCountC << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    tip.vx += (ex->math_cos(ang) * radius) >> 12;
                    tip.vz += (ex->math_sin(ang) * radius) >> 12;
                    BTLFX_COPY_SVEC(&it->pos8, &tip);
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

s32 fx_0d7_op84(s32 mode, Fx0d7Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 alpha;
    s32 scale0;
    s32 scale1;
    s32 ang;
    s32 t;
    s32 flicker;
    s32 grow;
    u16 ry;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_0d7_env = env;
        work->state0 = 0;
        work->frame4 = 0;
    } else {
        alpha = 0;
        scale0 = 0;
        scale1 = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
        ry = 0;
        switch (work->state0) {
        case 0:
            ry = 0x200;
            alpha = (frame << 12) / 3;
            t = ex->math_sin(alpha >> 2);
            scale0 = t >> 1;
            scale1 = ((0x1000 - alpha) >> 1) + 0x800;
            if (frame >= 3) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 9;
            flicker = (frame & 1) << 8;
            scale1 = flicker + 0x1000;
            grow = (t >> 1) + 0x800;
            alpha = flicker + grow;
            ry = (t >> 3) + 0x200;
            scale0 = scale1;
            if (frame >= 9) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 9;
            ang = t >> 2;
            ry = 0x400 - ang;
            alpha = ex->math_cos(ang);
            scale0 = ex->math_sin(ang) + 0x1000;
            scale1 = ex->math_sin(ang) * 3 / 2 + 0x1000;
            if (frame >= 9) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xF), &work->pos8, NULL, scale0 / 3, 0, 1, alpha / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x10), &work->pos8, &rot, scale1 / 2, 0, 1, alpha * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d7/nonmatchings/fx_0d7", fx_0d7_headerPtr);
