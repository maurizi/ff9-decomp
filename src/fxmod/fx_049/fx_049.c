#include "fxmod/fx_049.h"

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 from1, s32 to1,
                   s32 width, s32 dwidth, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_049_env);
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
    if (kind < FX_049_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_049_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_049_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 width, s32 dwidth, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_049_env);
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
    if (kind < FX_049_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_049_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_049_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx049Strip *strip, Fx049Strip *out, s32 *tbl, s32 i, s32 fade, s32 lineWidth,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *p = strip->pts4;
    SVECTOR v;

    p += i;
    ex = BTLFX_EXPORTS(fx_049_env);
    ex->btlfx_lerpVec((s16 *)p, (s16 *)(p + 1), 0x4000, (s16 *)&v);
    fxm_drawJaggedStrip4(entry, out, tbl, p, &v, 0x80, ex->btlfx_rand(), 0x37, 0, fade, lineWidth, kind);
}

static void fx_049_op83ElemDraw(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *col, s32 blend, s32 t, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_049_env);
    Unk80100C78Entry *entry;
    SVECTOR r;
    s32 s;
    s32 w;

    BTLFX_COPY_SVEC(&r, rot);
    entry = fx_049_task;
    s = ex->math_sin((t << 11) / n);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), pos, &r, *scale, t, 1, s, 0, 0);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x19), pos, &r, *scale, t, 1, s, 2, 2);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 0x1F), pos, &r, *scale * 5 / 2, t, 1, *col, 0, 0);
    w = (t << 12) / n + 0x1800;
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), pos, &r, w, t, 1, *col);
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, 0x1C), pos, &r, w, t, 1, *col);
}

static s32 fx_049_op83PoolTask(s32 mode, Fx049Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx049Strip *strip;
    Fx049Strip *br;
    Pool6CEntry *emitter;
    s32 c;
    s32 t;
    s32 w;

    if (mode == 1) {
        it->state11 = 0;
        return 0;
    }
    switch (it->state11) {
    case 0:
        t = frame << 9;
        if (it->kind14 < FX_049_STRIP_FLAT) {
            c = ex->math_cos(t >> 2);
            strip = &fx_049_strips[it->strip10];
            br = (Fx049Strip *)&fx_049_branches[it->strip10];
            w = it->width15;
            if (w == FX_049_WIDTH_DEFAULT) {
                w = -0x20;
            }
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame * 0x97, 0x186, frame, c,
                          ((0x1000 - t) * (w + 0xFB)) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, c, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18 + 3, it->branchAt13, c, (0x1000 - t) >> 6, it->kind14);
            if (frame == 0) {
                it->emit1C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&it->pos0);
                if (it->emit1C != NULL) {
                    it->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                if ((it->strip10 & 1) && it->width15 != FX_049_WIDTH_DEFAULT) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0xC, 0x46);
                    emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&it->pos0);
                    if (emitter != NULL) {
                        emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                    }
                }
            }
            if (frame >= 8) {
                it->state11 = 1;
                ((s16 *)it)[-1] = -1;
            }
        } else {
            t = (frame << 12) / 6;
            c = ex->math_cos(t >> 2);
            strip = &fx_049_strips[it->strip10];
            br = (Fx049Strip *)&fx_049_branches[it->strip10];
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame << 6, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, c, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18 + 3, it->branchAt13, c, (0x1000 - t) >> 6, it->kind14);
            if (frame == 0) {
                it->emit1C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&it->pos0);
                if (it->emit1C != NULL) {
                    it->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 6) {
                it->state11 = 1;
                ((s16 *)it)[-1] = -1;
            }
        }
        break;
    case 1:
        if (frame >= 6) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_049_op83(s32 mode, Fx049Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR a;
    SVECTOR b;
    Fx049Bolt *it;
    Pool6CEntry *p;
    Pool6CEntry *p2;
    s32 *q;
    s32 frame;
    s32 kind;
    s32 i;
    s32 r;
    s32 ang;
    s32 c;

    if (mode == 0) {
        init->workSize = 0x10;
        init->elemSize = 0x24;
        init->elemFn = (s32)fx_049_op83PoolTask;
        init->poolCount = 0xC;
    } else if (mode == 1) {        work->pos0.vx = env->targets->centerX;
        work->pos0.vy = env->targets->centerY;
        work->pos0.vz = env->targets->centerZ;
        fx_049_env = env;
        work->pos0.vy = 0;
        q = work->jitter8 = fx_049_jitter;
        for (i = 0; i < 0x40; i++) {
            *q++ = ex->btlfx_randCentered(0x100);
            *q++ = ex->btlfx_randCentered(0x200);
            *q++ = ex->btlfx_randCentered(0xF5);
        }
        fx_049_task = entry;
        work->boltCountC = 0;
    } else {
        frame = entry->frame;
        kind = 0;
        if (frame < 0x10) {
            kind = fx_049_boltSchedule[frame];
        }
        if (kind > 0) {
            work->boltCountC++;
            BTLFX_COPY_SVEC(&a, &work->pos0);
            if (kind != FX_049_BOLT_STRIKE) {
                a.vx += ex->btlfx_randCentered(0x200);
                a.vy += ex->btlfx_randCentered(0x300);
                a.vz += ex->btlfx_randCentered(0x200);
            }
            if (kind != FX_049_BOLT_PAIR) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->pos0, &a);
                    BTLFX_COPY_SVEC(&b, &a);
                    b.vy -= 0xC00;
                    r = ex->btlfx_randCentered(0x100) + 0x640;
                    if (kind != FX_049_BOLT_STRIKE) {
                        r = r * 3 / 8;
                    }
                    ang = (work->boltCountC << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    b.vx += (ex->math_cos(ang) * r) >> 12;
                    b.vz += (ex->math_sin(ang) * r) >> 12;
                    if (kind != FX_049_BOLT_STRIKE) {
                        it->pos0.vx = b.vx;
                        it->pos0.vz = b.vz;
                    }
                    BTLFX_COPY_SVEC(&it->pos8, &b);
                    it->strip10 = work->boltCountC % 12;
                    it->jitter18 = work->jitter8 + (ex->btlfx_rand() & 0x1F);
                    it->branchAt12 = ex->btlfx_rand() % 14;
                    it->branchAt13 = ex->btlfx_rand() % 14;
                    it->width15 = 0;
                    if (kind == FX_049_BOLT_FLAT) {
                        it->kind14 = FX_049_STRIP_FLAT;
                    } else if (kind == FX_049_BOLT_STRIKE) {
                        it->kind14 = ex->btlfx_rand() & 1;
                    }
                }
            } else {
                for (kind = 0; kind < 2; kind++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        BTLFX_COPY_SVEC(&it->pos0, &a);
                        BTLFX_COPY_SVEC(&b, &a);
                        b.vy -= 0xC00;
                        r = ex->btlfx_randCentered(0x80) + 0x80;
                        ang = ((work->boltCountC + kind) << 11) - 0x200 + ex->btlfx_randCentered(0x80);
                        b.vx += (ex->math_cos(ang) * r) >> 12;
                        b.vz += (ex->math_sin(ang) * r) >> 12;
                        BTLFX_COPY_SVEC(&it->pos8, &b);
                        it->strip10 = work->boltCountC % 12;
                        it->jitter18 = work->jitter8 + (ex->btlfx_rand() & 0x1F);
                        it->branchAt12 = ex->btlfx_rand() % 14;
                        it->branchAt13 = ex->btlfx_rand() % 14;
                        it->kind14 = ex->btlfx_rand() & 1;
                        it->width15 = 0x80;
                    }
                }
                p = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos0);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos0);
            }
        }
        if (frame == 0xF) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x40, 0x40, 0x40);
        }
        if (frame >= 0x10 && frame < 0x13) {
            c = 200 - (frame - 0x10) * 200 / 3;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 1, c >> 1, c);
        }
        if (frame == 0x14) {
            p2 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos0);
            if (p2 != NULL) {
                p2->elems->draw = (void (*)())fx_049_op83ElemDraw;
            }
        }
        if (frame >= 0x22 && frame - 0x22 < env->targets->targetCount) {
            ex->btlfx_getUnitBodyPos(frame - 0x22, 0, (s16 *)&b);
            it = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (it != NULL) {
                BTLFX_COPY_SVEC(&it->pos0, &b);
                BTLFX_COPY_SVEC(&it->pos8, &work->pos0);
                it->strip10 = work->boltCountC % 12;
                it->jitter18 = work->jitter8 + (ex->btlfx_rand() & 0x1F);
                it->branchAt12 = ex->btlfx_rand() % 14;
                it->branchAt13 = ex->btlfx_rand() % 14;
                it->kind14 = ex->btlfx_rand() & 1;
                it->width15 = FX_049_WIDTH_DEFAULT;
            }
        }
        if (frame >= 0x30) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_049/nonmatchings/fx_049", fx_049_headerPtr);
