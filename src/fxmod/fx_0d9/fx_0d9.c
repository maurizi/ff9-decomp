#include "fxmod/fx_0d9.h"

s32 fx_0d9_op80(s32 mode, Fx0d9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR rot2;
    VECTOR pos;
    VECTOR scale;
    SVECTOR ang;
    SVECTOR sparkPos;
    Pool6CEntry *emitter;
    s32 frame;
    s32 pulseAng;
    s32 waveT;
    s32 ringT;
    s32 fade;
    s32 sparkAlpha;
    s32 ringScale;
    s32 i;
    s32 sparkFrame;
    s32 sparkAng;
    s32 phaseT;

    if (mode == 0) {
        *args = sizeof(Fx0d9Work80);
    } else if (mode == 1) {
        fx_0d9_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        ex->btlfx_initEffModels(fx_0d9_effModels, 2);
        work->model14 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_GLOW_DOME), 0xB8, 0x3E40, 0,
                                                 0x80);
        work->model18 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_DISC), 0x119, 0xFFFF,
                                                 0, 0x80);
        work->state10 = 1;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_FAN_EMITTER), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        switch (work->state10) {
        case 1:
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x20, 0x40, 0xFF);
            }
            if (frame == 1) {
                env->otZ = 0x200;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_THIN_BOLT), &work->pos8, NULL, 0x2800, 0, 1,
                                        0x1000, 0, 0);
                env->otZ = 0;
            }
            env->otZ = 0x200;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_VIOLET_RING), &work->pos8, NULL, 0x800, 0, 1, -1);
            env->otZ = 0;
            if (frame == 2) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0xFF);
            }
            if (frame >= 2) {
                work->state10 = 2;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_FAN_EMITTER_2), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                work->emit1C = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_CONE_EMITTER), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            break;
        case 2:
            phaseT = (frame << 12) / 50;
            if (frame < 6) {
                s32 tint;

                tint = ex->math_cos((frame << 10) / 6) * 0xFF;
                fade = tint >> 12;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint >> 14, tint >> 13, fade);
            }
            if (frame < 20) {
                fade = ex->math_cos((frame << 10) / 20);
                env->otZ = 0x200;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_LIGHTNING_BOLT), &work->pos8, NULL, 0x2800, 0, 1,
                                        fade, 0, 0);
                if (frame < 10) {
                    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_THIN_BOLT), &work->pos8, NULL, 0x2800,
                                            0, 1, 0x1000 - (frame << 12) / 10, 0, 0);
                }
                env->otZ = 0;
            }
            if (frame < 24) {
                env->otZ = 0x200;
                ringT = (frame << 12) / 24;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_VIOLET_RING), &work->pos8, NULL,
                                  ex->math_cos(ringT >> 2) * 3 / 2 + 0x1000, 0, 1, ex->math_cos(ringT >> 2));
                env->otZ = 0;
            }
            rot.vx = 0x400;
            rot.vz = 0;
            rot.vy = 0;
            if (work->emit1C != NULL) {
                if (work->emit1C->state == BTLFX_EMIT_DONE) {
                    work->emit1C = NULL;
                }
                if (work->emit1C != NULL) {
                    work->emit1C->rot.vx = rot.vx;
                    work->emit1C->rot.vy = rot.vy;
                    work->emit1C->rot.vz = rot.vz;
                }
            }
            if (frame < 16) {
                rot.vx = 0x400;
                rot.vz = 0;
                rot.vy = 0;
                rot.pad = 1;
                waveT = frame << 8;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_WHITE_RING), &work->pos8, &rot,
                                  ex->math_sin(waveT >> 2) * 4, 0, 1, 0x1000 - waveT);
            }
            rot2.vx = 0x400;
            rot2.vz = 0;
            rot2.vy = 0;
            rot2.pad = 1;
            ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_BLUE_RING), &work->pos8, &rot2,
                              ex->math_sin(phaseT >> 2) * 4, 0, 1, ex->math_cos(phaseT >> 2));
            if (frame == 1) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_SPARKS), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame < 24) {
                pos.vx = work->pos8.vx;
                pos.vy = work->pos8.vy;
                pos.vz = work->pos8.vz;
                ang.vx = 0;
                ang.vy = frame * 0x18;
                ang.vz = 0;
                pulseAng = (frame << 12) / 24;
                pulseAng >>= 2;
                ringScale = ((ex->math_sin(pulseAng) * 0x5DC) >> 12) + 0x140;
                scale.vx = scale.vy = scale.vz = ringScale;
                fx_0d9_ringRadius = (ringScale * 0x708) >> 12;
                ex->Hi_DrawEffModel(&ang, &pos, (s32)&scale, work->model14);
                ex->Hi_ModifyEffModelAbr(work->model14, 1);
                ex->Hi_ModifyEffModelRGB(work->model14, (ex->math_cos(pulseAng) * 2 / 3 >> 5) - 0x80,
                                         (ex->math_cos(pulseAng) * 2 / 3 >> 5) - 0x80, (ex->math_cos(pulseAng) * 2 / 3 >> 5) - 0x80);
                env->otZ = 0x20;
                sparkAlpha = ex->math_cos(pulseAng);
                for (sparkFrame = frame, sparkAng = frame * 0x18, i = 0; i < 16; i++, sparkFrame += 3) {
                    BTLFX_COPY_SVEC(&sparkPos, &work->pos8);
                    sparkPos.vx += (ex->math_cos(sparkAng) * fx_0d9_ringRadius) >> 12;
                    sparkPos.vz += (ex->math_sin(sparkAng) * fx_0d9_ringRadius) >> 12;
                    sparkAng += 0x100;
                    {
                        void *fd_ = ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_SPARK_BALL);
                        s32 r0_ = ex->btlfx_randCentered(5); /* the PS1 draws them in this order */
                        s32 r1_ = ex->btlfx_randCentered(5);
                        ex->btlfx_drawParticles((u8 *)fd_, &sparkPos, NULL,
                                            0x1800 - ((i & 1) << 12), sparkFrame & 7, 1, sparkAlpha, r0_, r1_);
                    }
                }
                env->otZ = 0;
            }
            if (frame >= 50) {
                work->state10 = 2;
                return 1;
            }
            break;
        }
    }
    return 0;
}

static void fxm_drawJaggedStrip15(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d9_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = strip->pts4;
    strip->count2 = 0xE;
    strip->lastSlot0 = 0xE;
    strip->head1 = 0;
    for (i = 0; i < 15; i++) {
        t = 0x1000 - (i << 12) / 14;
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
    if (kind < FX_0D9_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d9_strip15Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d9_strip15Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedStrip3(Unk80100C78Entry *entry, Fx0d9Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d9_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = strip->pts4;
    strip->count2 = 2;
    strip->lastSlot0 = 2;
    strip->head1 = 0;
    for (i = 0; i < 3; i++) {
        t = 0x1000 - (i << 11);
        t += *tbl++ / 3;
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
    if (kind < FX_0D9_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d9_strip3Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d9_strip3Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedBranch3(Unk80100C78Entry *entry, Fx0d9Strip *strip, Fx0d9Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *base = strip->pts4;
    SVECTOR mid;

    base += branchAt;
    ex = BTLFX_EXPORTS(fx_0d9_env);
    ex->btlfx_lerpVec((s16 *)base, (s16 *)(base + 1), 0x4000, (s16 *)&mid);
    fxm_drawJaggedStrip3(entry, out, tbl, base, &mid, 0x80, ex->btlfx_rand(), 0x33, 0, fade, lineWidth, kind);
}

static s32 fxm_jaggedStripTask(s32 mode, Fx0d9Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0d9Strip *strip;
    Fx0d9Strip *branch;
    s32 n;
    s32 fade;
    s32 unused;
    s32 t;

    if (mode == 1) {
        it->state11 = 0;
    } else if (it->state11 == 0) {
        if (it->kind14 < FX_0D9_STRIP_FLAT) {
            n = 8;
            if (fx_0d9_fastBolts != 0) {
                n = 6;
            }
            t = (frame << 12) / n;
            fade = ex->math_cos(t >> 2);
            strip = &fx_0d9_strips[it->strip10];
            branch = (Fx0d9Strip *)&fx_0d9_branches[it->strip10];
            fxm_drawJaggedStrip15(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame * 0x97, 0x186, frame, fade,
                          ((0x1000 - t) * (it->width15 * 2 + 0x97)) >> 12, it->kind14);
            fxm_drawJaggedBranch3(entry, strip, branch, it->jitter18, it->branchAt12, fade, ((0x1000 - t) * (it->width15 * 2 + 0x97)) >> 13,
                          it->kind14);
            fxm_drawJaggedBranch3(entry, strip, branch, it->jitter18 + 3, it->branchAt13, fade,
                          ((0x1000 - t) * (it->width15 * 2 + 0x97)) >> 13, it->kind14);
            if (frame >= n) {
                it->state11 = 1;
                return 1;
            }
        } else {
            n = 6;
            if (fx_0d9_fastBolts != 0) {
                n = 4;
            }
            t = (frame << 12) / n;
            fade = ex->math_cos(t >> 2);
            strip = &fx_0d9_strips[it->strip10];
            branch = (Fx0d9Strip *)&fx_0d9_branches[it->strip10];
            fxm_drawJaggedStrip15(entry, strip, it->jitter18, &it->pos0, &it->pos8, 0, 0x200, frame << 6, 0x186, frame, fade,
                          ((0x1000 - t) * 0xAA) >> 12, it->kind14);
            fxm_drawJaggedBranch3(entry, strip, branch, it->jitter18, it->branchAt12, fade, (0x1000 - t) >> 6, it->kind14);
            fxm_drawJaggedBranch3(entry, strip, branch, it->jitter18 + 3, it->branchAt13, fade, (0x1000 - t) >> 6, it->kind14);
            if (frame >= n) {
                it->state11 = 1;
                return 1;
            }
        }
    }
    return 0;
}

static s32 fxm_randStripPoint(SVECTOR *out, SVECTOR *dir, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d9_env);
    SVECTOR *src = FX_0D9_STRIPS[ex->btlfx_rand() % 16].pts4;

    src += ex->btlfx_rand() % 15;

    out->vx = src->vx;
    out->vy = src->vy;
    out->vz = src->vz;
    out->pad = 1;
    return 0;
}

s32 fx_0d9_op83(s32 mode, Fx0d9Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR root;
    SVECTOR tip;
    SVECTOR vec;
    SVECTOR tip2;
    Fx0d9Bolt *it0;
    s32 radius0;
    s32 ang0;
    Fx0d9Bolt *it1;
    s32 radius1;
    s32 ang1;
    Fx0d9Bolt *it2;
    s32 radius2;
    s32 ang2;
    Fx0d9Bolt *it3;
    Fx0d9Bolt *it4;
    s32 radius4;
    Fx0d9Bolt *it5;
    Pool6CEntry *emitter;
    s32 *jit;
    s32 frame;
    s32 kind;
    s32 rnd2;
    s32 rnd3;
    s32 rnd5;
    s32 base2;
    s32 base3;
    s32 i;
    s32 j;
    s32 reach;

    if (mode == 0) {
        init->workSize = 0x1C;
        init->elemSize = 0x24;
        init->elemFn = (s32)fxm_jaggedStripTask;
        init->poolCount = 0x10;
    } else if (mode == 1) {
        fx_0d9_env = env;
        kind = init->workSize;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        i = 0;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        jit = work->jitter10 = fx_0d9_jitter;
        for (; i < 0x40; i++) {
            *jit++ = ex->btlfx_randCentered(0x100);
            *jit++ = ex->btlfx_randCentered(0x200);
            *jit++ = ex->btlfx_randCentered(0xF5);
        }
        work->kind18 = kind;
        fx_0d9_task = entry;
        work->boltCount14 = 0;
    } else {
        frame = entry->frame;
        switch (work->kind18) {
        case 0:
            fx_0d9_fastBolts = 0;
            if (frame < 29) {
                if (frame & 1) {
                    work->boltCount14++;
                    BTLFX_COPY_SVEC(&root, &work->pos0);
                    root.vy = 0;
                    it0 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it0 != NULL) {
                        BTLFX_COPY_SVEC(&it0->pos0, &root);
                        BTLFX_COPY_SVEC(&tip, &root);
                        tip.vy -= 0xC00;
                        radius0 = ex->btlfx_randCentered(0x100) + 0x384;
                        ang0 = (work->boltCount14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                        tip.vx += (ex->math_cos(ang0) * radius0) >> 12;
                        tip.vz += (ex->math_sin(ang0) * radius0) >> 12;
                        BTLFX_COPY_SVEC(&it0->pos8, &tip);
                        it0->strip10 = work->boltCount14 % 16;
                        it0->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                        it0->branchAt12 = ex->btlfx_rand() % 13;
                        it0->branchAt13 = ex->btlfx_rand() % 13;
                        it0->width15 = 0;
                        it0->kind14 = 2;
                    }
                }
            }
            if (frame >= 29) {
                return 1;
            }
            break;
        case 1:
            fx_0d9_fastBolts = 0;
            if (frame == 0) {
                for (j = 0; j < 6; j++) {
                    work->boltCount14++;
                    BTLFX_COPY_SVEC(&root, &work->pos0);
                    root.vy = 0;
                    it1 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it1 != NULL) {
                        BTLFX_COPY_SVEC(&it1->pos0, &root);
                        BTLFX_COPY_SVEC(&tip, &root);
                        tip.vy -= 0xC00;
                        radius1 = ex->btlfx_randCentered(0x80) + 0x3E8;
                        ang1 = (work->boltCount14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                        tip.vx += (ex->math_cos(ang1) * radius1) >> 12;
                        tip.vz += (ex->math_sin(ang1) * radius1) >> 12;
                        BTLFX_COPY_SVEC(&it1->pos8, &tip);
                        it1->strip10 = work->boltCount14 % 16;
                        it1->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                        it1->branchAt12 = ex->btlfx_rand() % 13;
                        it1->branchAt13 = ex->btlfx_rand() % 13;
                        it1->width15 = -12;
                        it1->kind14 = ex->btlfx_rand() & 1;
                    }
                }
            }
            if (frame >= 6 && frame < 16) {
                work->boltCount14++;
                BTLFX_COPY_SVEC(&vec, &work->pos0);
                vec.vy = 0;
                it2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it2 != NULL) {
                    BTLFX_COPY_SVEC(&it2->pos0, &vec);
                    BTLFX_COPY_SVEC(&tip2, &vec);
                    tip2.vy -= 0xC00;
                    rnd2 = ex->btlfx_randCentered(0x100);
                    base2 = (frame - 6) << 8;
                    rnd2 += 0x5DC;
                    radius2 = base2 + rnd2;
                    ang2 = ((work->boltCount14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    tip2.vx += (ex->math_cos(ang2) * radius2) >> 12;
                    tip2.vz += (ex->math_sin(ang2) * radius2) >> 12;
                    BTLFX_COPY_SVEC(&it2->pos8, &tip2);
                    it2->strip10 = work->boltCount14 % 16;
                    it2->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                    it2->branchAt12 = ex->btlfx_rand() % 13;
                    it2->branchAt13 = ex->btlfx_rand() % 13;
                    it2->width15 = 7;
                    it2->kind14 = ex->btlfx_rand() & 1;
                }
                work->boltCount14++;
                BTLFX_COPY_SVEC(&vec, &work->pos0);
                vec.vy = 0;
                it3 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it3 != NULL) {
                    BTLFX_COPY_SVEC(&it3->pos0, &vec);
                    BTLFX_COPY_SVEC(&tip2, &vec);
                    tip2.vy -= 0xC00;
                    rnd3 = ex->btlfx_randCentered(0x100);
                    base3 = (frame - 6) << 8;
                    rnd3 += 0x7D0;
                    radius2 = base3 + rnd3;
                    ang2 = ((work->boltCount14 / 2) << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    tip2.vx += (ex->math_cos(ang2) * radius2) >> 12;
                    tip2.vz += (ex->math_sin(ang2) * radius2) >> 12;
                    BTLFX_COPY_SVEC(&it3->pos8, &tip2);
                    it3->strip10 = work->boltCount14 % 16;
                    it3->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                    it3->branchAt12 = ex->btlfx_rand() % 13;
                    it3->branchAt13 = ex->btlfx_rand() % 13;
                    it3->width15 = 0;
                    it3->kind14 = 2;
                }
            }
            if (frame == 8 || frame == 10 || frame == 13) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x10, 0x40, 0x80);
            }
            if (frame == 9) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0xFF, 0xFF, 0);
            }
            if (frame >= 20) {
                return 1;
            }
            break;
        case 2:
            fx_0d9_fastBolts = 1;
            if (frame == 0) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_IMPACT_FLASH), (s32)&work->pos0);
                if (emitter != NULL) {
                    emitter->tail->shape = -1;
                    emitter->tail->s.module.spawnFn = fxm_randStripPoint;
                    emitter->otZ = 0x20;
                }
            }
            if (frame < 31) {
                if (frame == 0) {
                    BTLFX_COPY_SVEC(&fx_0d9_prevBoltEnd, &work->pos0);
                }
                reach = frame * 250 / 31 + 0x333;
                work->boltCount14++;
                BTLFX_COPY_SVEC(&root, &work->pos0);
                root.vy = 0;
                vec.vz = 0;
                BTLFX_COPY_SVEC(&tip, &root);
                radius4 = reach + ex->btlfx_randCentered(0x46);
                vec.vy = ex->btlfx_rand();
                vec.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                ex->btlfx_pointAhead((s16 *)&tip, (s16 *)&vec, radius4, (s16 *)&tip);
                work->boltCount14++;
                BTLFX_COPY_SVEC(&root, &work->pos0);
                root.vy = 0;
                it4 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it4 != NULL) {
                    BTLFX_COPY_SVEC(&it4->pos0, &tip);
                    BTLFX_COPY_SVEC(&it4->pos8, &fx_0d9_prevBoltEnd);
                    it4->strip10 = work->boltCount14 % 16;
                    it4->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                    it4->branchAt12 = ex->btlfx_rand() % 13;
                    it4->branchAt13 = ex->btlfx_rand() % 13;
                    it4->width15 = -(frame * 50 / 31);
                    it4->kind14 = ex->btlfx_rand() & 1;
                }
                BTLFX_COPY_SVEC(&fx_0d9_prevBoltEnd, &tip);
                if (frame < 19) {
                    work->boltCount14++;
                    BTLFX_COPY_SVEC(&root, &work->pos0);
                    root.vy = 0;
                    vec.vz = 0;
                    it5 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it5 != NULL) {
                        BTLFX_COPY_SVEC(&it5->pos8, &root);
                        BTLFX_COPY_SVEC(&tip, &root);
                        rnd5 = ex->btlfx_randCentered(0x46) + 0x400;
                        radius4 = reach + rnd5;
                        vec.vy = ex->btlfx_rand();
                        vec.vx = (ex->btlfx_rand() & 0xFF) + 0x80;
                        ex->btlfx_pointAhead((s16 *)&tip, (s16 *)&vec, radius4, (s16 *)&tip);
                        BTLFX_COPY_SVEC(&it5->pos0, &tip);
                        it5->strip10 = work->boltCount14 % 16;
                        it5->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                        it5->branchAt12 = ex->btlfx_rand() % 13;
                        it5->branchAt13 = ex->btlfx_rand() % 13;
                        it5->width15 = 0;
                        it5->kind14 = 2;
                    }
                }
            }
            if (frame & 1) {
                if (ex->btlfx_rand() & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x10, 0x20, 0x46);
                }
            }
            if (frame >= 31) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_0d9_op84(s32 mode, Fx0d9Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
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
        fx_0d9_env = env;
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
            scale1 = (ex->math_sin(ang) << 1) + 0x1000;
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
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_SWORD_GLINT), &work->pos8, NULL, scale0 / 3, 0, 1, alpha / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDAGA_SWORD_FLASH), &work->pos8, &rot, scale1, 0, 1, alpha * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d9/nonmatchings/fx_0d9", fx_0d9_headerPtr);
