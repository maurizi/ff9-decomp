#include "fxmod/fx_049.h"

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_049_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = strip->pts4;
    strip->count2 = FX_THUNDARA_STRIP_POINTS - 1;
    strip->lastSlot0 = FX_THUNDARA_STRIP_POINTS - 1;
    strip->head1 = 0;
    for (i = 0; i < FX_THUNDARA_STRIP_POINTS; i++) {
        t = 0x1000 - (i << 12) / (FX_THUNDARA_STRIP_POINTS - 1);
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
    if (kind < FX_049_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_049_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, BTLFX_ABR_ADD,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_049_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, BTLFX_ABR_ADD,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx049Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_049_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = strip->pts4;
    strip->count2 = FX_THUNDARA_BRANCH_POINTS - 1;
    strip->lastSlot0 = FX_THUNDARA_BRANCH_POINTS - 1;
    strip->head1 = 0;
    for (i = 0; i < FX_THUNDARA_BRANCH_POINTS; i++) {
        t = 0x1000 - (i << 12) / (FX_THUNDARA_BRANCH_POINTS - 1);
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
    if (kind < FX_049_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_049_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, BTLFX_ABR_ADD,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_049_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, BTLFX_ABR_ADD,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx049Strip *strip, Fx049Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *base = strip->pts4;
    SVECTOR mid;

    base += branchAt;
    ex = BTLFX_EXPORTS(fx_049_env);
    ex->btlfx_lerpVec((s16 *)base, (s16 *)(base + 1), 0x4000, (s16 *)&mid);
    fxm_drawJaggedStrip4(entry, out, tbl, base, &mid, 0x80, ex->btlfx_rand(), 0x37, 0, fade, lineWidth, kind);
}

static void fx_049_op83ElemDraw(s32 pt, s32 owner, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *alpha, s32 blend, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_049_env);
    Unk80100C78Entry *entry;
    SVECTOR elemRot;
    s32 fade;
    s32 spriteScale;

    BTLFX_COPY_SVEC(&elemRot, rot);
    entry = fx_049_task;
    fade = ex->math_sin((frame << 11) / n);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_THUNDARA_ORB), pos, &elemRot, *scale, frame, BTLFX_ABR_ADD, fade, 0, 0);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_THUNDARA_ORB_FADE), pos, &elemRot, *scale, frame, BTLFX_ABR_ADD, fade, 2, 2);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_THUNDARA_LENS_FLARES), pos, &elemRot, *scale * 5 / 2, frame, BTLFX_ABR_ADD, *alpha, 0, 0);
    spriteScale = (frame << 12) / n + 0x1800;
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, FX_THUNDARA_STAR_GLINTS), pos, &elemRot, spriteScale, frame, BTLFX_ABR_ADD, *alpha);
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, FX_THUNDARA_ORB_DOTS), pos, &elemRot, spriteScale, frame, BTLFX_ABR_ADD, *alpha);
}

static s32 fx_049_op83PoolTask(s32 mode, Fx049Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx049Strip *strip;
    Fx049Strip *br;
    Pool6CEntry *emitter;
    s32 bright;
    s32 t;
    s32 width;

    if (mode == 1) {
        it->state11 = 0;
        return 0;
    }
    switch (it->state11) {
    case 0:
        t = frame << 9;
        if (it->kind14 < FX_049_STRIP_FLAT) {
            bright = ex->math_cos(t >> 2);
            strip = &fx_049_strips[it->strip10];
            br = (Fx049Strip *)&fx_049_branches[it->strip10];
            width = it->width15;
            if (width == FX_049_WIDTH_DEFAULT) {
                width = -0x20;
            }
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame * 0x97, 0x186, frame, bright,
                          ((0x1000 - t) * (width + FX_THUNDARA_BOLT_WIDTH)) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, bright, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18 + 3, it->branchAt13, bright, (0x1000 - t) >> 6, it->kind14);
            if (frame == 0) {
                it->emit1C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THUNDARA_SPARK_DOTS), (s32)&it->pos0);
                if (it->emit1C != NULL) {
                    it->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                if ((it->strip10 & 1) && it->width15 != FX_049_WIDTH_DEFAULT) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, FX_THUNDARA_BOLT_TINT_RGB);
                    emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THUNDARA_IMPACT_FLASH), (s32)&it->pos0);
                    if (emitter != NULL) {
                        emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                    }
                }
            }
            if (frame >= FX_THUNDARA_BOLT_FRAMES) {
                it->state11 = 1;
                ((s16 *)it)[-1] = -1;
            }
        } else {
            t = (frame << 12) / FX_THUNDARA_FLAT_BOLT_FRAMES;
            bright = ex->math_cos(t >> 2);
            strip = &fx_049_strips[it->strip10];
            br = (Fx049Strip *)&fx_049_branches[it->strip10];
            fxm_drawJaggedStrip16(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame << 6, 0x186, frame, bright,
                          ((0x1000 - t) * FX_THUNDARA_FLAT_BOLT_WIDTH) >> 12, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18, it->branchAt12, bright, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, it->jitter18 + 3, it->branchAt13, bright, (0x1000 - t) >> 6, it->kind14);
            if (frame == 0) {
                it->emit1C = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THUNDARA_SPARK_DOTS), (s32)&it->pos0);
                if (it->emit1C != NULL) {
                    it->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= FX_THUNDARA_FLAT_BOLT_FRAMES) {
                it->state11 = 1;
                ((s16 *)it)[-1] = -1;
            }
        }
        break;
    case 1:
        if (frame >= FX_THUNDARA_BOLT_TAIL_FRAMES) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_049_op83(s32 mode, Fx049Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR start;
    SVECTOR end;
    Fx049Bolt *it;
    Pool6CEntry *emitter;
    Pool6CEntry *drawEmitter;
    s32 *dst;
    s32 frame;
    s32 kind;
    s32 i;
    s32 radius;
    s32 ang;
    s32 tint;

    if (mode == 0) {
        init->workSize = sizeof(Fx049Work);
        init->elemSize = sizeof(Fx049Bolt);
        init->elemFn = (s32)fx_049_op83PoolTask;
        init->poolCount = FX_THUNDARA_BOLT_SLOTS;
    } else if (mode == 1) {        work->pos0.vx = env->targets->centerX;
        work->pos0.vy = env->targets->centerY;
        work->pos0.vz = env->targets->centerZ;
        fx_049_env = env;
        work->pos0.vy = 0;
        dst = work->jitter8 = fx_049_jitter;
        for (i = 0; i < FX_THUNDARA_JITTER_ROWS; i++) {
            *dst++ = ex->btlfx_randCentered(0x100);
            *dst++ = ex->btlfx_randCentered(0x200);
            *dst++ = ex->btlfx_randCentered(0xF5);
        }
        fx_049_task = entry;
        work->boltCountC = 0;
    } else {
        frame = entry->frame;
        kind = 0;
        if (frame < FX_THUNDARA_SCHEDULE_FRAMES) {
            kind = fx_049_boltSchedule[frame];
        }
        if (kind > 0) {
            work->boltCountC++;
            BTLFX_COPY_SVEC(&start, &work->pos0);
            if (kind != FX_049_BOLT_STRIKE) {
                start.vx += ex->btlfx_randCentered(0x200);
                start.vy += ex->btlfx_randCentered(0x300);
                start.vz += ex->btlfx_randCentered(0x200);
            }
            if (kind != FX_049_BOLT_PAIR) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->pos0, &start);
                    BTLFX_COPY_SVEC(&end, &start);
                    end.vy -= FX_THUNDARA_BOLT_HEIGHT;
                    radius = ex->btlfx_randCentered(0x100) + FX_THUNDARA_BOLT_REACH;
                    if (kind != FX_049_BOLT_STRIKE) {
                        radius = radius * 3 / 8;
                    }
                    ang = (work->boltCountC << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    end.vx += (ex->math_cos(ang) * radius) >> 12;
                    end.vz += (ex->math_sin(ang) * radius) >> 12;
                    if (kind != FX_049_BOLT_STRIKE) {
                        it->pos0.vx = end.vx;
                        it->pos0.vz = end.vz;
                    }
                    BTLFX_COPY_SVEC(&it->pos8, &end);
                    it->strip10 = work->boltCountC % FX_THUNDARA_BOLT_SLOTS;
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
                        BTLFX_COPY_SVEC(&it->pos0, &start);
                        BTLFX_COPY_SVEC(&end, &start);
                        end.vy -= FX_THUNDARA_BOLT_HEIGHT;
                        radius = ex->btlfx_randCentered(0x80) + 0x80;
                        ang = ((work->boltCountC + kind) << 11) - 0x200 + ex->btlfx_randCentered(0x80);
                        end.vx += (ex->math_cos(ang) * radius) >> 12;
                        end.vz += (ex->math_sin(ang) * radius) >> 12;
                        BTLFX_COPY_SVEC(&it->pos8, &end);
                        it->strip10 = work->boltCountC % FX_THUNDARA_BOLT_SLOTS;
                        it->jitter18 = work->jitter8 + (ex->btlfx_rand() & 0x1F);
                        it->branchAt12 = ex->btlfx_rand() % 14;
                        it->branchAt13 = ex->btlfx_rand() % 14;
                        it->kind14 = ex->btlfx_rand() & 1;
                        it->width15 = 0x80;
                    }
                }
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THUNDARA_IMPACT_FLASH), (s32)&work->pos0);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THUNDARA_IMPACT_RING), (s32)&work->pos0);
            }
        }
        if (frame == FX_THUNDARA_DARKEN_FRAME) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_THUNDARA_DARKEN_RGB);
        }
        if (frame >= FX_THUNDARA_FLASH_FRAME && frame < FX_THUNDARA_FLASH_FRAME + FX_THUNDARA_FLASH_FRAMES) {
            tint = 200 - (frame - FX_THUNDARA_FLASH_FRAME) * 200 / FX_THUNDARA_FLASH_FRAMES;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint >> 1, tint >> 1, tint);
        }
        if (frame == FX_THUNDARA_ORB_FRAME) {
            drawEmitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THUNDARA_ORB_EMITTER), (s32)&work->pos0);
            if (drawEmitter != NULL) {
                drawEmitter->elems->draw = (void (*)())fx_049_op83ElemDraw;
            }
        }
        if (frame >= FX_THUNDARA_TARGET_BOLTS_FRAME && frame - FX_THUNDARA_TARGET_BOLTS_FRAME < env->targets->targetCount) {
            ex->btlfx_getUnitBodyPos(frame - FX_THUNDARA_TARGET_BOLTS_FRAME, 0, (s16 *)&end);
            it = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (it != NULL) {
                BTLFX_COPY_SVEC(&it->pos0, &end);
                BTLFX_COPY_SVEC(&it->pos8, &work->pos0);
                it->strip10 = work->boltCountC % FX_THUNDARA_BOLT_SLOTS;
                it->jitter18 = work->jitter8 + (ex->btlfx_rand() & 0x1F);
                it->branchAt12 = ex->btlfx_rand() % 14;
                it->branchAt13 = ex->btlfx_rand() % 14;
                it->kind14 = ex->btlfx_rand() & 1;
                it->width15 = FX_049_WIDTH_DEFAULT;
            }
        }
        if (frame >= FX_THUNDARA_FRAMES) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_049/nonmatchings/fx_049", fx_049_headerPtr);
