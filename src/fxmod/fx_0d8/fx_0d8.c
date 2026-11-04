#include "fxmod/fx_0d8.h"

static void fxm_drawJaggedStrip16(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp0, s32 amp1,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d8_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = strip->pts4;
    strip->count2 = 0xF;
    strip->lastSlot0 = 0xF;
    strip->head1 = 0;
    for (i = 0; i < 16; i++) {
        t = 0x1000 - (i << 12) / 15;
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
    if (kind < FX_0D8_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d8_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d8_strip16Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedStrip4(Unk80100C78Entry *entry, Fx0d8Strip *strip, s32 *tbl, SVECTOR *from, SVECTOR *to, s32 amp,
                   s32 ang, s32 dang, s32 frame, s32 fade, s32 lineWidth, s32 kind) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d8_env);
    SVECTOR pos;
    SVECTOR rot;
    SVECTOR *pt;
    s32 i;
    s32 t;
    s32 offset;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&rot);
    pt = strip->pts4;
    strip->count2 = 3;
    strip->lastSlot0 = 3;
    strip->head1 = 0;
    for (i = 0; i < 4; i++) {
        t = 0x1000 - (i << 12) / 3;
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
    if (kind < FX_0D8_STRIP_FLAT) {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d8_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 0);
    } else {
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, fx_0d8_strip4Blocks[kind]), 0, NULL, frame, 0x1000, fade, 1,
                          (u8 *)strip, 2);
    }
}

static void fxm_drawJaggedBranch4(Unk80100C78Entry *entry, Fx0d8Strip *strip, Fx0d8Strip *out, s32 *tbl, s32 branchAt, s32 fade, s32 lineWidth,
                   s32 kind) {
    BtlfxExports *ex;
    SVECTOR *base = strip->pts4;
    SVECTOR mid;

    base += branchAt;
    ex = BTLFX_EXPORTS(fx_0d8_env);
    ex->btlfx_lerpVec((s16 *)base, (s16 *)(base + 1), 0x4000, (s16 *)&mid);
    fxm_drawJaggedStrip4(entry, out, tbl, base, &mid, 0x80, ex->btlfx_rand(), 0x37, 0, fade, lineWidth, kind);
}

static void fx_0d8_op83ElemDraw(Pool30Entry *pt, Pool6CEntry *p, SVECTOR *pos, SVECTOR *rot, s32 *scale, s32 *alpha, s32 blend,
                   s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0d8_env);
    Unk80100C78Entry *entry;
    SVECTOR elemRot;
    s32 fade;
    s32 spriteScale;

    BTLFX_COPY_SVEC(&elemRot, rot);
    entry = fx_0d8_task;
    fade = ex->math_sin((frame << 11) / n);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_ORB), pos, &elemRot, *scale * 3 / 2, frame, 1, fade, 0, 0);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_ORB_FADE), pos, &elemRot, *scale * 3 / 2, frame, 1, fade, 2, 2);
    ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_LENS_FLARES), pos, &elemRot, *scale * 5 / 2, frame, 1, fade, 0, 0);
    spriteScale = (frame << 12) / n + 0x1800;
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_ORB_SPARKS), pos, &elemRot, spriteScale, frame, 1, *alpha);
    ex->btlfx_drawSprites((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_ORB_DOTS), pos, &elemRot, spriteScale, frame, 1, *alpha);
}

static s32 fx_0d8_op83PoolTask(s32 mode, Fx0d8Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0d8Strip *strip;
    Fx0d8Strip *br;
    Pool6CEntry *emitter;
    s32 fade;
    s32 t;

    if (mode == 1) {
        part->state11 = 0;
        return 0;
    }
    switch (part->state11) {
    case 0:
        if (part->kind14 < FX_0D8_STRIP_FLAT) {
            t = frame << 9;
            fade = ex->math_cos(t >> 2);
            strip = &fx_0d8_strips[part->strip10];
            br = (Fx0d8Strip *)&fx_0d8_branches[part->strip10];
            fxm_drawJaggedStrip16(entry, strip, part->jitter18, &part->pos0, &part->pos8, 0, 0x200, frame * 0x97, 0x186, frame, fade,
                          ((0x1000 - t) * (part->width15 + 0xFB)) >> 12, part->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, part->jitter18, part->branchAt12, fade, (0x1000 - t) >> 6, part->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, part->jitter18 + 3, part->branchAt13, fade, (0x1000 - t) >> 6, part->kind14);
            if (frame == 0) {
                part->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_SPARK_DOTS), (s32)&part->pos0);
                if (part->emit1C != NULL) {
                    part->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                if (part->strip10 & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0xC, 0x46);
                    emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_IMPACT_FLASH), (s32)&part->pos0);
                    if (emitter != NULL) {
                        emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                    }
                }
            }
            if (frame >= 8) {
                part->state11 = 1;
                ((s16 *)part)[-1] = -1;
            }
        } else {
            t = (frame << 12) / 6;
            fade = ex->math_cos(t >> 2);
            strip = &fx_0d8_strips[part->strip10];
            br = (Fx0d8Strip *)&fx_0d8_branches[part->strip10];
            fxm_drawJaggedStrip16(entry, strip, part->jitter18, &part->pos0, &part->pos8, 0, 0x200, frame << 6, 0x186, frame, fade,
                          ((0x1000 - t) * 0xAA) >> 12, part->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, part->jitter18, part->branchAt12, fade, (0x1000 - t) >> 6, part->kind14);
            fxm_drawJaggedBranch4(entry, strip, br, part->jitter18 + 3, part->branchAt13, fade, (0x1000 - t) >> 6, part->kind14);
            if (frame == 0) {
                part->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_SPARK_DOTS), (s32)&part->pos0);
                if (part->emit1C != NULL) {
                    part->emit1C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame >= 6) {
                part->state11 = 1;
                ((s16 *)part)[-1] = -1;
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

s32 fx_0d8_op83(s32 mode, Fx0d8Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR root;
    SVECTOR tip;
    Fx0d8Part *it;
    Pool6CEntry *emitter;
    s32 *jit;
    s32 frame;
    s32 kind;
    s32 i;
    s32 radius;
    s32 ang;
    s32 tint;
    s32 n;

    if (mode == 0) {
        init->workSize = sizeof(Fx0d8Work83);
        init->elemSize = 0x24;
        init->elemFn = (s32)fx_0d8_op83PoolTask;
        init->poolCount = 0xC;
    } else if (mode == 1) {
        fx_0d8_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->jitter10 = fx_0d8_jitter;
        for (n = 0, jit = fx_0d8_jitter; n < 0x40; n++) {
            *jit++ = ex->btlfx_randCentered(0x100);
            *jit++ = ex->btlfx_randCentered(0x200);
            *jit++ = ex->btlfx_randCentered(0xF5);
        }
        fx_0d8_task = entry;
        work->boltCount14 = 0;
    } else {
        frame = entry->frame;
        kind = 0;
        if (frame < 16) {
            kind = fx_0d8_spawnKinds[frame];
        }
        if (kind > 0) {
            work->boltCount14++;
            BTLFX_COPY_SVEC(&root, &work->pos0);
            if (kind != 2) {
                root.vx += ex->btlfx_randCentered(0x200);
                root.vy += ex->btlfx_randCentered(0x300);
                root.vz += ex->btlfx_randCentered(0x200);
            }
            if (kind != 3) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    BTLFX_COPY_SVEC(&it->pos0, &root);
                    BTLFX_COPY_SVEC(&tip, &root);
                    tip.vy -= 0xC00;
                    radius = ex->btlfx_randCentered(0x100) + 0x640;
                    if (kind != 2) {
                        radius = radius * 3 / 8;
                    }
                    ang = (work->boltCount14 << 10) - 0x200 + ex->btlfx_randCentered(0x100);
                    tip.vx += (ex->math_cos(ang) * radius) >> 12;
                    tip.vz += (ex->math_sin(ang) * radius) >> 12;
                    if (kind != 2) {
                        it->pos0.vx = tip.vx;
                        it->pos0.vz = tip.vz;
                    }
                    BTLFX_COPY_SVEC(&it->pos8, &tip);
                    it->strip10 = work->boltCount14 % 12;
                    it->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                    it->branchAt12 = ex->btlfx_rand() % 14;
                    it->branchAt13 = ex->btlfx_rand() % 14;
                    it->width15 = 0;
                    if (kind == 1) {
                        it->kind14 = 2;
                    } else if (kind == 2) {
                        it->kind14 = ex->btlfx_rand() & 1;
                    }
                }
            } else {
                for (i = 0; i < 2; i++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        BTLFX_COPY_SVEC(&it->pos0, &root);
                        BTLFX_COPY_SVEC(&tip, &root);
                        tip.vy -= 0xC00;
                        radius = ex->btlfx_randCentered(0x80) + 0x80;
                        ang = ((work->boltCount14 + i) << 11) - 0x200 + ex->btlfx_randCentered(0x80);
                        tip.vx += (ex->math_cos(ang) * radius) >> 12;
                        tip.vz += (ex->math_sin(ang) * radius) >> 12;
                        BTLFX_COPY_SVEC(&it->pos8, &tip);
                        it->strip10 = work->boltCount14 % 12;
                        it->jitter18 = work->jitter10 + (ex->btlfx_rand() & 0x1F);
                        it->branchAt12 = ex->btlfx_rand() % 14;
                        it->branchAt13 = ex->btlfx_rand() % 14;
                        it->kind14 = ex->btlfx_rand() & 1;
                        it->width15 = 0x80;
                    }
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_IMPACT_FLASH), (s32)&work->pos0);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_IMPACT_RING), (s32)&work->pos8);
            }
        }
        if (frame == 15) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x40, 0x40, 0x40);
        }
        if ((u32)(frame - 16) < 3) {
            tint = 0xC8 - ((frame - 16) * 0xC8) / 3;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint >> 1, tint >> 1, tint);
        }
        if (frame == 20) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_IMPACT_RING), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_ORB_EMITTER), (s32)&work->pos0);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                emitter->elems->draw = (void (*)())fx_0d8_op83ElemDraw;
            }
        }
        if (frame >= 0x30) {
            return 1;
        }
    }
    return 0;
}

s32 fx_0d8_op84(s32 mode, Fx0d8Work84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
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
        fx_0d8_env = env;
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
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_SWORD_GLINT), &work->pos8, NULL, scale0 / 3, 0, 1, alpha / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_THUNDARA_SWORD_GLINT_2), &work->pos8, &rot, scale1 / 2, 0, 1, alpha * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d8/nonmatchings/fx_0d8", fx_0d8_headerPtr);
