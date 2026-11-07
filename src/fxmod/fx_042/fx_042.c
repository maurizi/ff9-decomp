#include "fxmod/fx_042.h"

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx042Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_042_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = strip->pts4;
    strip->count2 = FX_THUNDER_STRIP_POINTS - 1;
    strip->lastSlot0 = FX_THUNDER_STRIP_POINTS - 1;
    strip->head1 = 0;
    for (i = 0; i < FX_THUNDER_STRIP_POINTS; i++) {
        t = 0x1000 - (i << 12) / (FX_THUNDER_STRIP_POINTS - 1);
        t += *tbl++;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = *tbl++;
        offset = (ex->math_sin(t >> 1) * offset) >> 12;
        offset += ex->btlfx_lerp(amp0, amp1, t);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = lineWidth;
        ang += dang + *tbl++;
        pt++;
    }
    if (kind < FX_042_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_042_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_042_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx042Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_042_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = strip->pts4;
    strip->count2 = FX_THUNDER_BRANCH_POINTS - 1;
    strip->lastSlot0 = FX_THUNDER_BRANCH_POINTS - 1;
    strip->head1 = 0;
    for (i = 0; i < FX_THUNDER_BRANCH_POINTS; i++) {
        t = 0x1000 - (i << 12) / (FX_THUNDER_BRANCH_POINTS - 1);
        t += *tbl++ / 2;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        offset = *tbl++ / 2;
        t >>= 1;
        offset = (ex->math_sin(t) * offset) >> 12;
        offset += (ex->math_sin(t) * amp) >> 12;
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&rot, offset, ang);
        pt->pad = lineWidth;
        ang += dang + *tbl++ / 2;
        pt++;
    }
    if (kind < FX_042_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_042_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_042_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx042Strip *strip, Fx042Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *base = strip->pts4;
    SVECTOR mid;

    base += branchAt;
    ex = BTLFX_EXPORTS(fx_042_env);
    ex->btlfx_lerpVec((s16 *)base, (s16 *)(base + 1), 0x4000, (s16 *)&mid);
    fxm_drawJaggedStrip4(entry, out, tbl, base, &mid, 0x80, ex->btlfx_rand(), 0x37, 0, fade, lineWidth, kind);
}

static s32 fx_042_op83PoolTask(s32 mode, Fx042Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx042Strip *strip;
    Fx042Strip *br;
    s32 bright;
    s32 t;

    if (mode == 1) {
        it->state11 = 0;
        return 0;
    }
    switch (it->state11) {
    case 0:
        t = frame << 10;
        if (it->kind14 < FX_042_STRIP_FLAT) {
            bright = ex->math_cos(t >> 2);
            strip = &fx_042_strips[it->strip10];
            br = (Fx042Strip *)&fx_042_branches[it->strip10];
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame << 7, 0x186, frame, bright,
                          ((0x1000 - t) * FX_THUNDER_BOLT_WIDTH) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, bright, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18 + 3, it->branchAt13, bright, (0x1000 - t) >> 6, it->kind14);
            if (frame == 0) {
                it->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THUNDER_IMPACT_FLASH), (s32)&it->pos0);
                if (it->emit1C != NULL) {
                    it->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, FX_THUNDER_BOLT_TINT_RGB);
            }
            if (frame >= FX_THUNDER_BOLT_FRAMES) {
                it->state11 = 1;
                ((s16 *)it)[-1] = -1;
            }
        } else {
            t = (frame << 12) / FX_THUNDER_FLAT_BOLT_FRAMES;
            bright = ex->math_cos(t >> 2);
            strip = &fx_042_strips[it->strip10];
            br = (Fx042Strip *)&fx_042_branches[it->strip10];
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame << 7, 0x186, frame, bright,
                          ((0x1000 - t) * FX_THUNDER_FLAT_BOLT_WIDTH) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, bright, (0x1000 - t) >> 6, it->kind14);
            if (frame >= FX_THUNDER_FLAT_BOLT_FRAMES) {
                return 1;
            }
        }
        break;
    case 1:
        if (frame >= FX_THUNDER_BOLT_TAIL_FRAMES) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_042_op83(s32 mode, Fx042Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR start;
    SVECTOR end;
    Fx042Bolt *it;
    s32 *dst;
    s32 i;
    s32 radius;
    s32 ang;
    s32 frame;

    if (mode == 0) {
        init->workSize = sizeof(Fx042Work);
        init->elemSize = sizeof(Fx042Bolt);
        init->elemFn = (s32)fx_042_op83PoolTask;
        init->poolCount = FX_THUNDER_BOLT_SLOTS;
    } else {
        if (mode == 1) {
            work->pos0.vx = env->targets->centerX;
            work->pos0.vy = env->targets->centerY;
            work->pos0.vz = env->targets->centerZ;
            fx_042_env = env;
            work->pos0.vy = 0;
            dst = work->jitter8 = fx_042_jitter;
            for (i = 0; i < FX_THUNDER_JITTER_ROWS; i++) {
                *dst++ = ex->btlfx_randCentered(0x100);
                *dst++ = ex->btlfx_randCentered(0x180);
                *dst++ = ex->btlfx_randCentered(0xC0);
            }
            work->boltCountC = 0;
        } else {
            frame = entry->frame;
            if (work->boltCountC < FX_THUNDER_BOLT_COUNT) {
                work->boltCountC++;
                BTLFX_COPY_SVEC(&start, &work->pos0);
                if (work->boltCountC != 1) {
                    if ((work->boltCountC / 2) & 1) {
                        start.vx += 0x46 + (ex->btlfx_rand() % 14) * 0x6E;
                    } else {
                        start.vx -= 0x46 + (ex->btlfx_rand() % 14) * 0x6E;
                    }
                    start.vz += (ex->btlfx_rand() % 10) * 0xDC - 0x44C;
                }
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->pos0, &start);
                    BTLFX_COPY_SVEC(&end, &start);
                    end.vy -= FX_THUNDER_BOLT_HEIGHT;
                    radius = ex->btlfx_randCentered(0x100) + FX_THUNDER_BOLT_REACH;
                    ang = (work->boltCountC << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    end.vx += (ex->math_cos(ang) * radius) >> 12;
                    end.vz += (ex->math_sin(ang) * radius) >> 12;
                    BTLFX_COPY_SVEC(&it->pos8, &end);
                    it->strip10 = work->boltCountC % FX_THUNDER_BOLT_SLOTS;
                    it->jitter18 = work->jitter8 + (ex->btlfx_rand() & 0x1F);
                    it->branchAt12 = ex->btlfx_rand() % 14;
                    it->branchAt13 = ex->btlfx_rand() % 14;
                    if (work->boltCountC & 1) {
                        it->kind14 = FX_042_STRIP_FLAT;
                    } else {
                        it->kind14 = ex->btlfx_rand() & 1;
                    }
                }
            }
            if (frame >= FX_THUNDER_FRAMES) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_042/nonmatchings/fx_042", fx_042_headerPtr);
