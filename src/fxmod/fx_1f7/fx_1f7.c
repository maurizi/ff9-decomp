#include "fxmod/fx_1f7.h"

s32 fx_1f7_op80(s32 mode, Fx1f7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR from;
    SVECTOR to;
    MATRIX m;
    Pool6CEntry *emitter;
    Fx1f7Trail *tr;
    Fx1f7Spark *sp;
    s32 frame;
    s32 i;
    s32 link;
    s32 j;
    s32 alpha;
    s32 n;
    SVECTOR *dst;
    s32 jj;
    SVECTOR *bp;
    s32 sparkLink;
    s32 pc;
    Fx1f7Trail *tc;

    if (mode == 0) {
        *args = sizeof(Fx1f7Work);
    } else if (mode == 1) {
        work->active0 = 0;
        work->active4 = 0;
        work->timer8 = 0;
        fx_1f7_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        for (i = 0; i < FX_BLUE_SHOCKWAVE_TRAILS; i++) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_BLUE_SHOCKWAVE_GLOW_TRAIL), 0, NULL, -1, 0x1000, 0x1000, BTLFX_ABR_ADD,
                                (u8 *)&fx_1f7_trails[i], 0);
            fx_1f7_trails[i].count10C = FX_BLUE_SHOCKWAVE_TRAIL_PASSES;
            fx_1f7_trails[i].index110 = FX_BLUE_SHOCKWAVE_TRAIL_START;
            fx_1f7_trails[i].delay114 = i * 2;
        }
        for (i = 0; i < FX_BLUE_SHOCKWAVE_SPARKS; i++) {
            fx_1f7_sparks[i].index0 = ex->btlfx_randRange(0, FX_BLUE_SHOCKWAVE_SPARK_CHAINS * FX_BLUE_SHOCKWAVE_SPARK_LINKS * FX_BLUE_SHOCKWAVE_LINK_STEPS - 1);
            fx_1f7_sparks[i].offsetX4 = ex->btlfx_randRange(-0x10, 0x10);
            fx_1f7_sparks[i].delay10 = i * 3;
            fx_1f7_sparks[i].count8 = FX_BLUE_SHOCKWAVE_SPARK_REPEATS;
            fx_1f7_sparks[i].frameC = 0;
        }
        work->active0 = 1;
        work->active4 = 1;
    } else {
        frame = entry->frame;
        if (frame == FX_BLUE_SHOCKWAVE_FLASH_FRAME) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLUE_SHOCKWAVE_FLASH), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x200);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BLUE_SHOCKWAVE_LIGHTNING), (s32)&work->posC);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x200);
            }
        }
        if (frame == FX_BLUE_SHOCKWAVE_PULSE_FRAME) {
            work->timer8 = FX_BLUE_SHOCKWAVE_PULSE_FRAMES;
        }
        if (work->active0 != 0) {
            for (i = 0; i < FX_BLUE_SHOCKWAVE_TRAILS; i++) {
                tr = &fx_1f7_trails[i];
                if (tr->count10C != 0) {
                    if (--tr->delay114 < 0) {
                        for (link = 0; link < FX_BLUE_SHOCKWAVE_TRAIL_LINKS; link++) {
                            if (link != 0) {
                                ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, fx_1f7_trailBones[i][link], (u16 *)&from);
                            } else {
                                ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, fx_1f7_trailBones[i][0], (BtlBone *)&m);
                                ex->btlfx_matrixPointAhead(&m, 0x400, (s32)&from);
                            }
                            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, fx_1f7_trailBones[i][link + 1], (u16 *)&to);
                            for (j = 0; j < FX_BLUE_SHOCKWAVE_LINK_STEPS; j++) {
                                ex->btlfx_lerpVec((s16 *)&from, (s16 *)&to, j << 10, (s16 *)&fx_1f7_trailPoints[link * FX_BLUE_SHOCKWAVE_LINK_STEPS + j]);
                            }
                        }
                        dst = tr->points4;
                        alpha = 0x1000;
                        n = FX_BLUE_SHOCKWAVE_TRAIL_POINTS;
                        tr->lastSlot0 = tr->count2 = n - 1;
                        tr->head1 = 0;
                        if (tr->index110 < FX_BLUE_SHOCKWAVE_TRAIL_FADE) {
                            alpha = tr->index110 << 8;
                        }
                        /* Own counter, base pointer and trail copy (the original's a0, t0 and a2); pad after vz so
                         * loop.c combines the vy/vz/pad address givs into &dst->pad. */
                        jj = 0;
                        bp = fx_1f7_trailPoints;
                        tc = tr;
                        for (; jj < n; jj++, dst++) {
                            dst->vx = bp[tc->index110 + jj].vx;
                            dst->vy = bp[tc->index110 + jj].vy;
                            dst->vz = bp[tc->index110 + jj].vz;
                            pc = 0x80;
                            dst->pad = pc;
                            /* Dead store, deleted by flow after loop.c: pc is set twice in the loop, so the 0x80
                             * is not hoisted out of it. */
                            pc = 0;
                        }
                        env->otZ = 0x200;
                        /* &fx_1f7_trails[i] rather than tr: cse shares the address, so its base is worth hoisting and
                         * tr becomes a giv that loop.c reduces (the original's s1). */
                        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_BLUE_SHOCKWAVE_GLOW_TRAIL), 0, NULL, frame, 0x1000, alpha, BTLFX_ABR_ADD,
                                            (u8 *)&fx_1f7_trails[i], 0);
                        env->otZ = 0;
                        tr->index110 -= 4;
                        if (tr->index110 < 0) {
                            tr->index110 = FX_BLUE_SHOCKWAVE_TRAIL_START;
                            tr->count10C--;
                        }
                    }
                }
            }
        }
        if (work->active4 != 0) {
            for (i = 0; i < FX_BLUE_SHOCKWAVE_SPARK_CHAINS; i++) {
                for (sparkLink = 0; sparkLink < FX_BLUE_SHOCKWAVE_SPARK_LINKS; sparkLink++) {
                    ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, fx_1f7_sparkBones[i][sparkLink], (u16 *)&m);
                    ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, fx_1f7_sparkBones[i][sparkLink + 1], (u16 *)&((SVECTOR *)&m)[1]);
                    for (j = 0; j < FX_BLUE_SHOCKWAVE_LINK_STEPS; j++) {
                        ex->btlfx_lerpVec((s16 *)&m, (s16 *)&((SVECTOR *)&m)[1], j << 10,
                                          (s16 *)&fx_1f7_sparkPoints[i * (FX_BLUE_SHOCKWAVE_SPARK_LINKS * FX_BLUE_SHOCKWAVE_LINK_STEPS) + sparkLink * FX_BLUE_SHOCKWAVE_LINK_STEPS + j]);
                    }
                }
            }
            for (i = 0; i < FX_BLUE_SHOCKWAVE_SPARKS; i++) {
                sp = &fx_1f7_sparks[i];
                if (sp->count8 != 0) {
                    if (--sp->delay10 < 0) {
                        env->otZ = 0x200;
                        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_BLUE_SHOCKWAVE_GLOW), &fx_1f7_sparkPoints[sp->index0], NULL, 0x4000,
                                                sp->frameC, BTLFX_ABR_ADD, 0x1000, sp->offsetX4, 0);
                        env->otZ = 0;
                        if (++sp->frameC >= FX_BLUE_SHOCKWAVE_SPARK_FRAMES) {
                            sp->index0 = ex->btlfx_randRange(0, FX_BLUE_SHOCKWAVE_SPARK_CHAINS * FX_BLUE_SHOCKWAVE_SPARK_LINKS * FX_BLUE_SHOCKWAVE_LINK_STEPS - 1);
                            sp->offsetX4 = ex->btlfx_randRange(-0x10, 0x10);
                            sp->frameC = 0;
                            sp->count8--;
                        }
                    }
                }
            }
        }
        if (work->timer8 != 0) {
            work->timer8--;
            ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_ADD, 0, 0, work->timer8 << 6);
        }
        return frame >= FX_BLUE_SHOCKWAVE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f7/nonmatchings/fx_1f7", fx_1f7_headerPtr);
