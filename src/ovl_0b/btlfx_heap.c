#include "ovl_0b.h"

void btlfx_initParticles(s32 bytes, Pool30Entry *buf, Pool30 *pool) {
    s32 i = 0;
    pool->capacity = bytes / (s32)sizeof(Pool30Entry);
    pool->entries = buf;
    pool->live = 0;
    pool->maxUsed = 0;
    for (; i < pool->capacity;) {
        i++;
        buf->slot = 0;
        buf++;
    }
}

s32 btlfx_getParticleTrail(Pool30Entry *particle, Pool6CEntry *emitter) {
    s32 trails = emitter->trails;
    s32 ofs;
    if (trails == 0) {
        return 0;
    }
    ofs = particle->slot;
    ofs = (ofs - 1) * emitter->trailStride;
    return trails + ofs;
}

s32 btlfx_updateParticles(Pool30 *pool) {
    SVECTOR rel;
    SVECTOR pos;
    SVECTOR vec; /* scratch: orbit, wobble, homing aim, field direction and element rotation in turn (one frame slot) */
    VECTOR delta;
    s32 alpha;
    s32 cnt;
    s32 scale;
    Pool30Entry *part;
    Pool6CEntry *owner;
    EffEmit *emit;
    EffElem *el;
    EffSub *sub;
    SVECTOR *vp;
    s16 *rp;
    s32 i;
    s32 frame;
    s32 frames;
    s32 last;
    s32 lvl;
    s32 t;
    s32 range;
    s32 ofs;
    s32 curve;
    s32 mode;
    s32 rs;
    s32 amp;
    s32 k;
    u32 fl;

    lvl = 0;
    range = 0;
    curve = 0;
    last = 0;
    part = pool->entries;
    amp = 0;
    i = 0;
    g_btlfxSavedOtZ = g_btlfxEnv->otZ;
    pool->live = 0;
    for (; i <= pool->maxUsed; i++, part++) {
        if (part->slot == 0) {
            continue;
        }
        frame = part->frame;
        frames = part->frames;
        owner = part->owner;
        if (frame >= frames) {
            part->slot = 0;
            if (part->spawnFlags & BTLFX_PART_SUB) {
                owner->live.s.y--;
            } else {
                owner->live.s.x--;
            }
            continue;
        }
        last = i;
        pool->live++;
        t = (frame << 12) / frames;
        if (part->spawnFlags & BTLFX_PART_SUB) {
            emit = (EffEmit *)owner->sub;
            sub = owner->sub - 1;
            mode = sub->fieldMode;
            rs = owner->subGravity;
            rp = owner->subAccel;
            if (mode != BTLFX_FIELD_NONE) {
                curve = sub->fieldCurve;
                range = owner->subField.s.x;
                if (mode < BTLFX_FIELD_FADE) {
                    amp = owner->subField.s.y;
                } else {
                    amp = sub->fieldStart[1];
                }
            }
        } else {
            emit = owner->file;
            mode = owner->tail->anim.fieldMode;
            rs = owner->gravity;
            rp = owner->accel;
            if (mode != BTLFX_FIELD_NONE) {
                curve = owner->tail->anim.fieldCurve;
                range = owner->field.s.x;
                if (mode < BTLFX_FIELD_FADE) {
                    amp = owner->field.s.y;
                } else {
                    amp = owner->tail->anim.fieldStart[1];
                }
            }
            if (owner->sub != NULL) {
                lvl = (owner->sub - 1)->rotMode;
                if (lvl >= BTLFX_LAUNCH_PARENT) {
                    rel.vx = part->pos.vx >> 12;
                    rel.vy = part->pos.vy >> 12;
                    rel.vz = part->pos.vz >> 12;
                }
            }
        }
        switch (emit->kind) {
        case BTLFX_MOTION_BALLISTIC:
            btlfx_stepParticle(part, rs, rp, &pos);
            if (emit->m.ballistic.jitter != 0) {
                part->motA.vel.x += btlfx_randCentered(emit->m.ballistic.jitterX);
                part->motA.vel.y += btlfx_randCentered(emit->m.ballistic.jitterY);
                part->motA.vel.z += btlfx_randCentered(emit->m.ballistic.jitterZ);
            }
            if (emit->m.ballistic.floorMode != 0) {
                if (emit->m.ballistic.floorMode == 1) {
                    if (owner->rot.pad - emit->m.ballistic.floorMargin < pos.vy && part->motA.vel.y > 0) {
                        part->motA.vel.y = -part->motA.vel.y;
                        part->motA.vel.y = (part->motA.vel.y * emit->m.ballistic.bounce) >> 8;
                    }
                } else if (owner->rot.pad - emit->m.ballistic.floorMargin < pos.vy) {
                    if (part->motA.vel.y > 0) {
                        s32 x = part->motA.vel.y * (owner->rot.pad - pos.vy);
                        if (emit->m.ballistic.floorMargin != 0) {
                            x /= emit->m.ballistic.floorMargin;
                        }
                        part->motA.vel.y = x;
                    }
                    if (owner->rot.pad - (emit->m.ballistic.floorMargin >> 1) < pos.vy) {
                        part->motA.vel.y = (s16)(part->motA.vel.y - 0x40) - ((owner->gravity * part->timeStep) >> 6);
                    }
                    if (pos.vy > owner->rot.pad) {
                        part->motA.vel.y = 0;
                    }
                }
            }
            break;
        case BTLFX_MOTION_ORBIT:
            btlfx_stepParticleAltDrag(part, rs, rp, &pos);
            part->motB.orbit.dir.s.x += part->motB.orbit.turnX;
            part->motB.orbit.dir.s.y += part->motB.orbit.turnY;
            vec.vx = part->motB.orbit.dir.s.x;
            vec.vy = part->motB.orbit.dir.s.y;
            switch (emit->m.orbit.radiusCurve) {
            case BTLFX_RADIUS_SIN:
                rs = (math_sin(btlfx_lerp(emit->m.orbit.sinStart, emit->m.orbit.sinEnd, t)) * part->motB.orbit.radius) >> 12;
                break;
            case BTLFX_RADIUS_GROW:
                rs = (part->motB.orbit.radius * t) >> 12;
                break;
            case BTLFX_RADIUS_SHRINK:
                rs = part->motB.orbit.radius - ((part->motB.orbit.radius * t) >> 12);
                break;
            }
            btlfx_pointAhead((s16 *)&pos, (s16 *)&vec, rs, (s16 *)&pos);
            break;
        case BTLFX_MOTION_SPIRAL:
            btlfx_stepParticleNoDrag(part, rs, rp, &pos);
            part->motA.spin.angle += part->motA.spin.speed;
            part->motB.spiral.axial += part->motB.spiral.axialSpeed;
            part->motB.spiral.radius += part->motB.spiral.radiusSpeed;
            btlfx_offsetPoint(&pos, (s32)&pos, part->motB.spiral.axial >> 2, &part->motB.spiral.axis, part->motB.spiral.radius >> 2, part->motA.spin.angle);
            break;
        case BTLFX_MOTION_WOBBLE:
            btlfx_stepParticleNoDragAlt(part, rs, rp, &pos);
            part->motA.wobble.phaseX += part->motB.wobble.swingXSpeed;
            part->motA.wobble.phaseY += part->motB.wobble.swingYSpeed;
            vec.vx = part->motB.wobble.dirX + math_sin(part->motA.wobble.phaseX);
            vec.vy = part->motB.wobble.dirY + math_sin(part->motA.wobble.phaseY);
            switch (emit->m.wobble.radiusCurve) {
            case BTLFX_RADIUS_SIN:
                rs = (math_sin(btlfx_lerp(emit->m.wobble.sinStart, emit->m.wobble.sinEnd, t)) * part->motA.wobble.radius) >> 12;
                break;
            case BTLFX_RADIUS_GROW:
                rs = (part->motA.wobble.radius * t) >> 12;
                break;
            case BTLFX_RADIUS_SHRINK:
                rs = part->motA.wobble.radius - ((part->motA.wobble.radius * t) >> 12);
                break;
            }
            btlfx_pointAhead((s16 *)&pos, (s16 *)&vec, rs, (s16 *)&pos);
            break;
        case BTLFX_MOTION_HELIX:
            btlfx_stepParticleNoDrag(part, rs, rp, &pos);
            part->motB.helix.axial += part->motB.helix.axialSpeed;
            part->motA.spin.angle += part->motA.spin.speed;
            switch (emit->m.helix.radiusCurve) {
            case BTLFX_RADIUS_SIN:
                rs = (math_sin(btlfx_lerp(emit->m.helix.sinStart, emit->m.helix.sinEnd, t)) * part->motB.helix.radius) >> 12;
                break;
            case BTLFX_RADIUS_GROW:
                rs = (part->motB.helix.radius * t) >> 12;
                break;
            case BTLFX_RADIUS_SHRINK:
                rs = part->motB.helix.radius - ((part->motB.helix.radius * t) >> 12);
                break;
            }
            btlfx_offsetPoint(&pos, (s32)&pos, part->motB.helix.axial >> 6, &part->motB.helix.axis, rs, part->motA.spin.angle);
            break;
        case BTLFX_MOTION_HOMING:
            btlfx_stepParticle(part, rs, rp, &pos);
            if (emit->m.homing.jitter != 0) {
                part->motA.vel.x += btlfx_randCentered(emit->m.homing.jitterX);
                part->motA.vel.y += btlfx_randCentered(emit->m.homing.jitterY);
                part->motA.vel.z += btlfx_randCentered(emit->m.homing.jitterZ);
            }
            rs = btlfx_lerp(emit->m.homing.steerStart, emit->m.homing.steerEnd, t);
            btlfx_aimAngles((s16 *)&pos, (s16 *)owner->target, (s16 *)&vec);
            vec.vx += emit->m.homing.aimX;
            vec.vy += emit->m.homing.aimY;
            btlfx_pointAhead(&part->motA.vel.x, (s16 *)&vec, rs, &part->motA.vel.x);
            break;
        case BTLFX_MOTION_MODULE:
            if (emit->m.module.update(part, owner, &pos, btlfx_getParticleTrail(part, owner), frame, frames) != 0) {
                part->slot = 0;
                if (part->spawnFlags & BTLFX_PART_SUB) {
                    owner->live.s.y--;
                } else {
                    owner->live.s.x--;
                }
            }
            break;
        }
        ofs = 0xFFFF;
        if (mode != BTLFX_FIELD_NONE) {
            switch (mode) {
            case BTLFX_FIELD_REPEL:
                /* padded: a zero word follows the first jump table */
                __asm__(".section .rodata\n\t.word 0\n\t.section .text");
                cnt = btlfx_distance(&pos, PSX_NULLABLE(owner->fieldPos));
                if (cnt <= range) {
                    delta.vx = pos.vx - PSX_NULLABLE(owner->fieldPos)->vx;
                    delta.vy = pos.vy - PSX_NULLABLE(owner->fieldPos)->vy;
                    delta.vz = pos.vz - PSX_NULLABLE(owner->fieldPos)->vz;
                    VectorNormalS(&delta, &vec);
                }
                break;
            case BTLFX_FIELD_ATTRACT:
                cnt = btlfx_distance(&pos, PSX_NULLABLE(owner->fieldPos));
                if (cnt <= range) {
                    delta.vx = PSX_NULLABLE(owner->fieldPos)->vx - pos.vx;
                    delta.vy = PSX_NULLABLE(owner->fieldPos)->vy - pos.vy;
                    delta.vz = PSX_NULLABLE(owner->fieldPos)->vz - pos.vz;
                    VectorNormalS(&delta, &vec);
                }
                break;
            case BTLFX_FIELD_UP:
            case BTLFX_FIELD_UP_2:
                *(s32 *)&vec.vx = *(s32 *)&pos.vx;
                vec.vy = PSX_NULLABLE(owner->fieldPos)->vy;
                *(s32 *)&vec.vz = *(s32 *)&pos.vz;
                cnt = btlfx_distance(&vec, PSX_NULLABLE(owner->fieldPos));
                ((s32 *)&vec)[0] = 0;
                vec.vy = -0x1000;
                break;
            case BTLFX_FIELD_ALONG_AXIS:
                btlfx_toLocalPolar(&pos, PSX_NULLABLE(owner->fieldPos), PSX_NULLABLE(owner->fieldRot), (s32)&alpha, (s32)&cnt, &scale);
                if (cnt <= range) {
                    btlfx_rotateOffset(PSX_NULLABLE(owner->fieldRot), 0x1000, (s32)&vec);
                }
                break;
            case BTLFX_FIELD_AROUND_AXIS:
                btlfx_toLocalPolar(&pos, PSX_NULLABLE(owner->fieldPos), PSX_NULLABLE(owner->fieldRot), (s32)&alpha, (s32)&cnt, &scale);
                if (cnt <= range) {
                    ((s32 *)&vec)[1] = 0;
                    ((s32 *)&vec)[0] = 0;
                    btlfx_offsetPoint(&vec, (s32)&vec, 0, (XY32 *)PSX_NULLABLE(owner->fieldRot), 0x1000, scale);
                }
                break;
            case BTLFX_FIELD_FADE:
                cnt = btlfx_distance(&pos, PSX_NULLABLE(owner->fieldPos));
                break;
            case BTLFX_FIELD_FADE_FLAT:
                *(s32 *)&vec.vx = *(s32 *)&pos.vx;
                vec.vy = PSX_NULLABLE(owner->fieldPos)->vy;
                *(s32 *)&vec.vz = *(s32 *)&pos.vz;
                cnt = btlfx_distance(&vec, PSX_NULLABLE(owner->fieldPos));
                break;
            case BTLFX_FIELD_FADE_POLAR:
                btlfx_toLocalPolar(&pos, PSX_NULLABLE(owner->fieldPos), PSX_NULLABLE(owner->fieldRot), (s32)&alpha, (s32)&cnt, &scale);
                break;
            }
            if (cnt <= range) {
                scale = (cnt << 10) / range;
                delta.vx = vec.vx;
                delta.vy = vec.vy;
                delta.vz = vec.vz;
                switch (curve) {
                case BTLFX_FALLOFF_CONST:
                    break;
                case BTLFX_FALLOFF_COS:
                    amp = (math_cos(scale) * amp) >> 12;
                    break;
                case BTLFX_FALLOFF_SIN:
                    amp = (math_sin(scale) * amp) >> 12;
                    break;
                case BTLFX_FALLOFF_INV_SIN:
                    amp = ((0x1000 - math_sin(scale)) * amp) >> 12;
                    break;
                case BTLFX_FALLOFF_INV_COS:
                    amp = ((0x1000 - math_cos(scale)) * amp) >> 12;
                    break;
                case BTLFX_FALLOFF_SIN2:
                    amp = (math_sin(scale * 2) * amp) >> 12;
                    break;
                }
                if (mode < BTLFX_FIELD_FADE) {
                    gte_lddp(amp);
                    gte_ldir(&delta);
                    gte_gpf12();
                    gte_stir(&delta);
                    part->pos.vx += delta.vx << 6;
                    part->pos.vy += delta.vy << 6;
                    part->pos.vz += delta.vz << 6;
                } else {
                    ofs = amp;
                }
            } else if (mode >= BTLFX_FIELD_FADE) {
                ofs = 0;
            }
        }
        g_btlfxEnv->otZ = owner->otZ;
        if (part->elem != 0xFF) {
            el = &owner->elems[part->elem];
            fl = el->flags;
            vp = NULL;
            if (!(fl & BTLFX_ELEM_NO_ROT)) {
                vp = &vec;
                *(s32 *)&vp->vx = part->elemRot.w;
                vp->vz = part->elemRotZ;
                vp->vx += el->rotStepX;
                vp->vy += el->rotStepY;
                vp->vz += el->rotStepZ;
                vec.pad = ((fl >> 2) ^ 1) & 1;
                rs = part->spin * 4;
                if (fl & BTLFX_ELEM_SPIN_X) {
                    vp->vx += rs;
                }
                if (fl & BTLFX_ELEM_SPIN_Y) {
                    vp->vy += rs;
                }
                if (fl & BTLFX_ELEM_SPIN_Z) {
                    vp->vz += rs;
                }
                part->elemRot.w = *(s32 *)&vp->vx;
                part->elemRotZ = vp->vz;
            }
            if (fl & BTLFX_ELEM_FIXED_ALPHA) {
                alpha = owner->alpha;
            } else {
                rs = owner->alpha;
                switch (el->fadeCurve) {
                case 1:
                    alpha = (rs * math_cos(t >> 2)) >> 12;
                    break;
                case 2:
                    alpha = (rs * math_sin(t >> 2)) >> 12;
                    break;
                case 3:
                    alpha = (rs * math_sin(t >> 1)) >> 12;
                    break;
                }
            }
            if (ofs != 0xFFFF) {
                alpha += ofs;
                if (alpha < 0) {
                    alpha = 0;
                }
                if (alpha > 0x1000) {
                    alpha = 0x1000;
                }
            }
            if (alpha == 0x1000) {
                alpha = -1;
            }
            if (fl & BTLFX_ELEM_FIXED_SCALE) {
                scale = owner->scale;
            } else {
                rs = owner->scale;
                scale = (btlfx_lerp(el->scaleStart << 7, el->scaleEnd << 7, t) * rs) >> 12;
            }
            rs = el->blend;
            if ((rs == 0xFF || rs == 0) && (u32)alpha < 0x801) {
                rs = 1;
            }
            cnt = rs;
            for (k = 0; k < 2; k++) {
                if (k == 0) {
                    rs = el->kind;
                    mode = el->tex;
                } else {
                    rs = el->kind2;
                    mode = el->tex2;
                }
                switch (rs) {
                case BTLFX_ELEMK_CALLBACK:
                    if (vp == NULL) {
                        vp = &vec;
                        ((s32 *)&vec)[0] = 0;
                        ((s32 *)&vec)[1] = 0;
                    }
                    el->draw(part, owner, &pos, vp, &scale, &alpha, &cnt, frame, frames);
                    break;
                case BTLFX_ELEMK_PARTICLES:
                    btlfx_drawParticles((u8 *)mode, &pos, vp, scale, part->elemFrame, cnt, alpha, 0, 0);
                    break;
                case BTLFX_ELEMK_SPRITE:
                    btlfx_drawSprite((u8 *)mode, &pos, part->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_ANIM_QUADS:
                    btlfx_drawAnimQuads((u8 *)mode, &pos, vp, scale, part->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_SPRITES:
                    btlfx_drawSprites((u8 *)mode, &pos, vp, scale, part->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_TRAIL_3D:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, (s32 *)owner->origin, part->elemFrame, scale, alpha, cnt, (u8 *)btlfx_getParticleTrail(part, owner), 1);
                    break;
                case BTLFX_ELEMK_TRAIL_FLAT:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, NULL, part->elemFrame, scale, alpha, cnt, (u8 *)btlfx_getParticleTrail(part, owner), 2);
                    break;
                case BTLFX_ELEMK_RINGS:
                    btlfx_drawRings((u8 *)mode, &pos, vp, scale, part->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_FAN:
                    btlfx_drawFan((Unk800D2938 *)mode, &pos, vp, scale, part->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_TRAIL_SCREEN:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, NULL, part->elemFrame, scale, alpha, cnt, (u8 *)btlfx_getParticleTrail(part, owner), 0);
                    break;
                }
                if (el->kind2 == BTLFX_ELEMK_NONE) {
                    break;
                }
            }
            if (el->frameMode == 1) {
                part->elemFrame++;
            } else if (el->frameMode == 2) {
                rs = part->elemFrame;
                rs++;
                if (rs >= el->frameCount) {
                    rs = 0;
                }
                part->elemFrame = rs;
            }
        }
        if (owner->sub != NULL && (part->spawnFlags & BTLFX_PART_SUB_FRAC)) {
            if (lvl >= BTLFX_LAUNCH_PARENT) {
                btlfx_aimAngles((s16 *)&rel, (s16 *)&pos, (s16 *)&vec);
            }
            btlfx_runSubEmitter(pool, part, &pos, &vec);
        }
        frame++;
        part->frame = frame;
    }
    pool->maxUsed = last;
    g_btlfxEnv->otZ = g_btlfxSavedOtZ;
    return pool->live;
}

void btlfx_initEmitters(u32 bytes, Pool6C *list) {
    s32 i;
    Pool6CEntry *emitter = list->buf;
    list->capacity = (bytes - 0x10) / sizeof(Pool6CEntry);
    list->entries = emitter;
    list->live = 0;
    list->maxUsed = 0;
    for (i = 0; i < list->capacity;) {
        i++;
        emitter->state = BTLFX_EMIT_FREE;
        emitter++;
    }
}

Pool6CEntry *btlfx_spawnEmitter(Pool30 *pool, Pool6C *list, u8 *data, s32 pos) {
    Pool6CEntry *emitter;
    s32 i;
    u32 k;
    u8 *cursor;
    EffElemHdr *h;
    EffElem *el;
    s32 *ptrs;
    s32 shape;

    emitter = list->entries;
    for (i = 0; i < list->capacity; i++, emitter++) {
        if (emitter->state == BTLFX_EMIT_FREE) {
            if (list->maxUsed < i) {
                list->maxUsed = i;
            }
            cursor = data;
            for (k = 0; k < 0x6C; k += 4) {
                *(s32 *)((u8 *)emitter + k) = 0;
            }
            emitter->state = BTLFX_EMIT_LIVE;
            emitter->spawnCount = 0;
            emitter->subSpawnCount = 0;
            emitter->spawnNow = 0;
            emitter->live.s.x = 0;
            emitter->live.s.y = 0;
            emitter->trails = 0;
            emitter->spawnFrac = 0xFF;
            emitter->otZ = 0;
            emitter->file = (EffEmit *)cursor;
            if (*cursor != 0xFF) {
                cursor += 0x28;
            } else {
                cursor += 0x10;
            }
            if (emitter->file->emitsSub != 0) {
                cursor += 0x38;
                emitter->sub = (EffSub *)cursor;
                cursor += 0x28;
            } else {
                emitter->sub = NULL;
            }
            h = (EffElemHdr *)cursor;
            cursor += 0x14;
            el = (EffElem *)cursor;
            emitter->elems = el;
            i = 0;
            cursor += h->elemCount * 0x28;
            for (; i < h->elemCount; i++, el++) {
                if (el->kind != BTLFX_ELEMK_CALLBACK) {
                    if (el->tex != -1 && !(el->tex & BTLFX_TEX_ADDR)) {
                        el->tex = (s32)data + el->tex;
                    }
                }
                if (el->kind2 != BTLFX_ELEMK_NONE) {
                    if (el->tex2 != -1 && !(el->tex2 & BTLFX_TEX_ADDR)) {
                        el->tex2 = (s32)data + el->tex2;
                    }
                }
            }
            /* tail, origin, target written through one pointer */
            ptrs = (s32 *)&emitter->tail;
            ptrs[0] = (s32)cursor;
            shape = *(s32 *)cursor;
            k = (u32)cursor;
            if (shape == BTLFX_SHAPE_MODULE) {
                ((EffTail *)k)->s.module.spawnFn = NULL;
            }
            ptrs[1] = pos;
            ptrs[2] = pos;
            do {
            } while (0);
            return emitter;
        }
    }
    return NULL;
}

void btlfx_moveEmitter(Pool30 *pool, s32 handle, s32 newPos) {
    Pool6CEntry *emitter = (Pool6CEntry *)handle;
    SVECTOR *pos = (SVECTOR *)newPos;
    VECTOR delta;
    Pool30Entry *part;
    s32 i;
    delta.vx = (pos->vx - emitter->origin->vx) << 12;
    delta.vy = (pos->vy - emitter->origin->vy) << 12;
    delta.vz = (pos->vz - emitter->origin->vz) << 12;
    part = pool->entries;
    for (i = 0; i < pool->capacity; i++, part++) {
        if (part->slot != 0 && part->owner == emitter) {
            part->pos.vx += delta.vx;
            part->pos.vy += delta.vy;
            part->pos.vz += delta.vz;
        }
    }
    *(s32 *)&emitter->origin->vx = *(s32 *)&pos->vx;
    emitter->origin->vz = pos->vz;
}

void btlfx_killEmitter(Pool30 *pool, Pool6CEntry *emitter) {
    s32 i = 0;
    Pool30Entry *part = pool->entries;
    if (i < pool->capacity) {
        do {
            if (part->slot != 0 && part->owner == emitter) {
                part->frame = part->frames;
            }
            i++;
            part++;
        } while (i < pool->capacity);
    }
    emitter->state = BTLFX_EMIT_DONE;
}

s32 btlfx_updateEmitters(Pool30 *pool, Pool6C *list) {
    union {
        s16 h[4];
        s32 w;
    } cur;
    s16 end[4];
    s32 i;
    s32 last;
    Pool6CEntry *emitter;
    EffTail *def;
    EffTail *d2;
    EffAnim *an;
    EffElemHdr *hdr;
    EffSub *sub;
    EffEmit *emit;
    s16 *tbl;
    s32 frame;
    s32 t;
    s32 val; /* spawn rate, then the lower blend weight index (separate locals don't match) */
    s32 y;
    s32 x;
    Pool6C *emitters = list;

    last = 0;
    emitter = emitters->entries;
    val = 0;
    i = 0;
    emitters->live = 0;
    for (; i <= emitters->maxUsed; i++, emitter++) {
        if (emitter->state == BTLFX_EMIT_FREE) {
            continue;
        }
        if (emitter->state == BTLFX_EMIT_DONE) {
            emitter->state = BTLFX_EMIT_FREE;
            continue;
        }
        emitters->live++;
        def = emitter->tail;
        frame = emitter->frame;
        an = &def->anim;
        last = i;
        y = an->duration;
        t = (frame << 12) / y;
        emitter->spawnNow = 0;
        if (frame >= y) {
            if (emitter->live.w == 0) {
                emitter->state = BTLFX_EMIT_DONE;
            }
            continue;
        }
        switch (def->anim.rateCurve) {
        case 0:
            val = btlfx_lerp(an->rateStart, an->rateEnd, t);
            break;
        case 1:
            val = (math_sin(t >> 1) * an->rateStart) >> 12;
            break;
        case 2:
            def = emitter->tail;
            if (def->shape != BTLFX_SHAPE_MODULE) {
                tbl = def->rateTable;
            } else {
                tbl = def->s.module.rateTable;
            }
            val = tbl[(frame << 4) / y];
            break;
        case 3:
            val = 0x100;
            break;
        }
        x = val >> 8;
        y = emitter->spawnFrac;
        y += val & 0xFF;
        if (y >= 0x100) {
            y -= 0x100;
            x++;
        }
        emitter->spawnFrac = y;
        emitter->spawnNow = x;
        if (frame == 0) {
            ((s32 *)&emitter->rot)[0] = ((s32 *)&an->rot)[0];
            ((s32 *)&emitter->rot)[1] = ((s32 *)&an->rot)[1];
        } else {
            emitter->rot.vx += an->rotStep.vx;
            emitter->rot.vy += an->rotStep.vy;
            emitter->rot.vz += an->rotStep.vz;
        }
        hdr = (EffElemHdr *)emitter->elems - 1;
        if (hdr->weightAnim == 1) {
            hdr->weights.w = 0;
            x = btlfx_lerp(0, hdr->weightCount * 4, t);
            y = (x & 3) << 5;
            val = x >> 2;
            if (y != 0) {
                x = val + 1;
                if (x >= hdr->weightCount) {
                    x = val;
                }
                hdr->weights.b[val] = -0x80 - y;
                hdr->weights.b[x] += y;
            } else {
                hdr->weights.b[val] = 0x80;
            }
        }
        cur.h[0] = hdr->scaleStart;
        cur.h[1] = hdr->alphaStart;
        cur.h[2] = an->gravityStart;
        end[0] = hdr->scaleEnd;
        end[1] = hdr->alphaEnd;
        end[2] = an->gravityEnd;
        btlfx_lerpVec(cur.h, end, t, cur.h);
        gte_lddp(0x1000 - t);
        gte_ldsv(an->accelStart);
        gte_gpf12();
        emitter->scale = cur.h[0];
        emitter->alpha = cur.h[1];
        emitter->gravity = cur.h[2];
        gte_lddp(t);
        gte_ldsv(an->accelEnd);
        gte_gpl12();
        gte_stsv(emitter->accel);
        if (an->fieldMode != 0) {
            btlfx_lerpVec(an->fieldStart, an->fieldEnd, t, cur.h);
            emitter->field.w = cur.w;
        }
        if (emitter->sub != NULL) {
            sub = emitter->sub - 1;
            btlfx_lerpVec(sub->accelStart, sub->accelEnd, t, emitter->subAccel);
            emitter->subGravity = btlfx_lerp(sub->gravityStart, sub->gravityEnd, t);
            if (sub->fieldMode != 0) {
                btlfx_lerpVec(sub->fieldStart, sub->fieldEnd, t, cur.h);
                emitter->subField.w = cur.w;
            }
        }
        d2 = emitter->tail;
        switch (d2->shape) {
        case BTLFX_SHAPE_SPHERE:
            cur.h[0] = d2->s.sphere.radiusMinStart;
            cur.h[1] = d2->s.sphere.radiusMaxStart;
            end[0] = d2->s.sphere.radiusMinEnd;
            end[1] = d2->s.sphere.radiusMaxEnd;
            btlfx_lerpVec(cur.h, end, t, cur.h);
            emitter->radiusMin = cur.h[0];
            emitter->radiusMax = cur.h[1];
            break;
        case BTLFX_SHAPE_CYLINDER:
        case BTLFX_SHAPE_AXIS:
        case BTLFX_SHAPE_TUBE:
            cur.h[0] = d2->s.cylinder.radiusStart[0];
            cur.h[1] = d2->s.cylinder.radiusStart[1];
            end[0] = d2->s.cylinder.radiusEnd[0];
            end[1] = d2->s.cylinder.radiusEnd[1];
            btlfx_lerpVec(cur.h, end, t, cur.h);
            emitter->radiusMin = cur.h[0];
            emitter->radiusMax = cur.h[1];
            cur.h[0] = d2->s.cylinder.spanStart[0];
            cur.h[1] = d2->s.cylinder.spanStart[1];
            end[0] = d2->s.cylinder.spanEnd[0];
            end[1] = d2->s.cylinder.spanEnd[1];
            btlfx_lerpVec(cur.h, end, t, cur.h);
            emitter->shape.span.min = cur.h[0];
            emitter->shape.span.max = cur.h[1];
            break;
        case BTLFX_SHAPE_CONE:
            cur.h[0] = d2->s.cone.radiusMinStart;
            cur.h[1] = d2->s.cone.radiusMaxStart;
            cur.h[2] = d2->s.cone.lengthStart;
            end[0] = d2->s.cone.radiusMinEnd;
            end[1] = d2->s.cone.radiusMaxEnd;
            end[2] = d2->s.cone.lengthEnd;
            btlfx_lerpVec(cur.h, end, t, cur.h);
            emitter->radiusMin = cur.h[0];
            emitter->radiusMax = cur.h[1];
            emitter->shape.cone.length = cur.h[2];
            break;
        }
        switch (emitter->file->kind) {
        case BTLFX_MOTION_BALLISTIC:
        case BTLFX_MOTION_HOMING:
            emitter->cur.ballistic.speed = btlfx_lerp(emitter->file->m.ballistic.speedStart, emitter->file->m.ballistic.speedEnd, t);
            break;
        case BTLFX_MOTION_ORBIT:
            emit = emitter->file;
            emitter->cur.orbit.radius = btlfx_lerp(emit->m.orbit.radiusStart, emit->m.orbit.radiusEnd, t);
            emitter->cur.orbit.speed = btlfx_lerp(emit->m.orbit.speedStart, emit->m.orbit.speedEnd, t);
            break;
        case BTLFX_MOTION_SPIRAL:
        case BTLFX_MOTION_HELIX:
            emitter->cur.spiral.axialSpeed = btlfx_lerp(emitter->file->m.spiral.axialSpeedStart, emitter->file->m.spiral.axialSpeedEnd, t);
            break;
        case BTLFX_MOTION_WOBBLE:
            emitter->cur.wobble.radius = btlfx_lerp(emitter->file->m.wobble.radiusStart, emitter->file->m.wobble.radiusEnd, t);
            break;
        }
        frame++;
        emitter->frame = frame;
    }
    emitters->maxUsed = last;
    return emitters->live;
}

Pool30Entry *btlfx_allocParticle(Pool30 *pool) {
    s32 i;
    Pool30Entry *part = pool->entries;
    for (i = 0; i < pool->capacity; i++, part++) {
        if (part->slot == 0) {
            if (pool->maxUsed < i) {
                pool->maxUsed = i;
            }
            break;
        }
    }
    if (i >= pool->capacity) {
        part = 0;
    }
    return part;
}

void btlfx_runEmitters(Pool30 *pool, Pool6C *list) {
    SVECTOR off;
    SVECTOR rot;
    SVECTOR scratch;
    SVECTOR ang;
    CVECTOR col;
    s32 k;
    s32 val; /* random rolls, shape radii and the polar radius in turn (separate locals don't match) */
    s32 d58;
    Pool6C *pl;
    s32 i;
    s32 j;
    s32 dur;
    s32 t;
    Pool6CEntry *emitter;
    Pool30Entry *part;
    EffElemHdr *hdr;
    EffElem *el;
    EffAnim *anim;
    SVECTOR *base;
    s32 frame;
    s32 acc;
    s32 aux; /* cone length step count, then a random angle (separate locals don't match) */

    el = NULL;
    pl = list;
    emitter = pl->entries;
    base = NULL;
    for (i = 0; i <= pl->maxUsed; i++, emitter++) {
        if (emitter->state == BTLFX_EMIT_FREE) {
            continue;
        }
        if (emitter->state == BTLFX_EMIT_DONE) {
            continue;
        }
        for (j = 0; j < emitter->spawnNow; j++) {
            if (emitter->live.s.x >= emitter->tail->anim.maxParticles) {
                break;
            }
            part = btlfx_allocParticle(pool);
            if (part == NULL) {
                break;
            }
            frame = emitter->frame;
            anim = &emitter->tail->anim;
            emitter->live.s.x++;
            dur = anim->duration;
            t = (frame << 12) / dur;
            hdr = (EffElemHdr *)emitter->elems - 1;
            acc = 0;
            val = btlfx_rand() & 0x7F;
            for (k = 0; k < hdr->weightCount; k++) {
                acc += hdr->weights.b[k];
                if (val < acc) {
                    break;
                }
            }
            if (k >= hdr->weightCount) {
                k = 0xFF;
            }
            part->elem = k;
            part->elemFrame = 0;
            if (k == 0xFF) {
                acc = btlfx_lerp(emitter->file->lifeStart, emitter->file->lifeEnd, t);
                acc += btlfx_randCentered(emitter->file->lifeSpread);
                if (acc <= 0) {
                    acc = 1;
                }
            } else {
                el = &emitter->elems[k];
                if (el->frameMode == 1) {
                    acc = el->frameCount;
                } else {
                    acc = btlfx_lerp(emitter->file->lifeStart, emitter->file->lifeEnd, t);
                    acc += btlfx_randCentered(emitter->file->lifeSpread);
                    if (acc <= 0) {
                        acc = 1;
                    }
                }
                off.vx = (el->spawnRotStep[0] * 8) & 0xFFF;
                off.vy = (el->spawnRotStep[1] * 8) & 0xFFF;
                off.vz = (el->spawnRotStep[2] * 8) & 0xFFF;
                gte_lddp(frame);
                gte_ldsv(&off);
                gte_gpf0();
                ang.vx = (el->baseRot[0] * 16 + btlfx_randCentered(el->rotSpread[0] * 16)) & 0xFFF;
                ang.vy = (el->baseRot[1] * 16 + btlfx_randCentered(el->rotSpread[1] * 16)) & 0xFFF;
                ang.vz = (el->baseRot[2] * 16 + btlfx_randCentered(el->rotSpread[2] * 16)) & 0xFFF;
                gte_lddp(1);
                gte_ldsv(&ang);
                gte_gpl0();
                part->spin = btlfx_randCentered(el->spinRange);
                gte_stsv(&part->elemRot);
            }
            part->slot = emitter->spawnCount % 255 + 1;
            part->frames = acc;
            part->frame = 0;
            part->owner = emitter;
            part->timeStep = btlfx_randCentered(anim->stepSpread) + 0x40;
            if (emitter->file->emitsSub != 0) {
                part->spawnFlags = BTLFX_PART_SUB_FRAC;
            } else {
                part->spawnFlags = 0;
            }
            emitter->spawnCount++;
            if (emitter->trails != 0) {
                emitter->spawnCount %= emitter->trailSlots;
            }
            switch (emitter->tail->shape) {
            case BTLFX_SHAPE_SPHERE:
                btlfx_randRotation(&rot);
                acc = btlfx_randRange(emitter->radiusMin, emitter->radiusMax);
                if (emitter->tail->s.sphere.axisScaleStart.cd == 1) {
                    if (rot.vx > 0x800) {
                        rot.vx -= 0x800;
                    }
                } else if (emitter->tail->s.sphere.axisScaleStart.cd == 2) {
                    if (rot.vx < 0x800) {
                        rot.vx += 0x800;
                    }
                }
                btlfx_rotateOffset(&rot, acc, (s32)&off);
                LoadAverageCol((u8 *)&emitter->tail->s.sphere.axisScaleStart, (u8 *)&emitter->tail->s.sphere.axisScaleEnd, 0x1000 - t, t, (u8 *)&col);
                off.vx = (off.vx * col.r) >> 7;
                off.vy = (off.vy * col.g) >> 7;
                off.vz = (off.vz * col.b) >> 7;
                base = emitter->origin;
                break;
            case BTLFX_SHAPE_CYLINDER:
                rot.vx = 0;
                rot.vy = btlfx_rand();
                rot.vz = 0;
                btlfx_rotateOffset(&rot, btlfx_randRange(emitter->radiusMin, emitter->radiusMax), (s32)&off);
                off.vy = btlfx_randRange(emitter->shape.span.min, emitter->shape.span.max);
                if (emitter->tail->s.cylinder.dirMode == 1) {
                    rot.vx = 0x400;
                    rot.vy = 0;
                } else if (emitter->tail->s.cylinder.dirMode == 2) {
                    rot.vx = -0x400;
                    rot.vy = 0;
                }
                base = emitter->origin;
                break;
            case BTLFX_SHAPE_AXIS:
                ((XY32 *)&ang)[0].w = ((XY32 *)&emitter->rot)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&emitter->rot)[1].w;
                d58 = btlfx_rand();
                acc = btlfx_randRange(emitter->shape.span.min, emitter->shape.span.max);
                val = btlfx_randRange(emitter->radiusMin, emitter->radiusMax);
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                btlfx_offsetPoint(&off, (s32)&off, acc, (XY32 *)&ang, val, d58);
                if (emitter->tail->s.cylinder.dirMode == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (emitter->tail->s.cylinder.dirMode == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    btlfx_forwardAngles(&rot.vx, (XY32 *)&ang, d58);
                }
                base = emitter->origin;
                break;
            case BTLFX_SHAPE_TUBE:
                btlfx_aimAngles((s16 *)emitter->origin, (s16 *)emitter->target, &ang.vx);
                d58 = btlfx_rand();
                acc = btlfx_randRange(emitter->shape.span.min, emitter->shape.span.max);
                val = btlfx_randRange(emitter->radiusMin, emitter->radiusMax);
                btlfx_lerpVec((s16 *)emitter->origin, (s16 *)emitter->target, acc, &off.vx);
                btlfx_offsetPoint(&off, (s32)&off, 0, (XY32 *)&ang, val, d58);
                if (emitter->tail->s.cylinder.dirMode == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (emitter->tail->s.cylinder.dirMode == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    btlfx_forwardAngles(&rot.vx, (XY32 *)&ang, d58);
                }
                base = emitter->origin;
                off.vx -= base->vx;
                off.vy -= base->vy;
                off.vz -= base->vz;
                break;
            case BTLFX_SHAPE_CONE: {
                s32 y4;
                ((XY32 *)&ang)[0].w = ((XY32 *)&emitter->rot)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&emitter->rot)[1].w;
                aux = 1;
                if (emitter->shape.cone.length != 0) {
                    aux = emitter->shape.cone.length;
                }
                acc = btlfx_rand() % aux;
                val = btlfx_rand() & 0xFFF;
                d58 = (emitter->radiusMin * val) >> 12;
                y4 = (emitter->radiusMax * val) >> 12;
                val = btlfx_lerpRatio(d58, y4, acc, aux);
                aux = btlfx_rand();
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                btlfx_offsetPoint(&off, (s32)&off, acc, (XY32 *)&ang, val, aux);
                if (emitter->tail->s.cone.dirMode == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (emitter->tail->s.cone.dirMode == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    *(s32 *)&scratch.vx = 0;
                    *(s32 *)&scratch.vz = 0;
                    btlfx_offsetPoint(&scratch, (s32)&scratch, 0, (XY32 *)&ang, d58, aux);
                    *(s32 *)&rot.vx = 0;
                    *(s32 *)&rot.vz = 0;
                    btlfx_offsetPoint(&rot, (s32)&rot, emitter->shape.cone.length, (XY32 *)&ang, y4, aux);
                    ((XY32 *)&ang)[0].w = ((XY32 *)&rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)&rot)[1].w;
                    btlfx_aimAngles(&scratch.vx, &ang.vx, &rot.vx);
                }
                base = emitter->origin;
                break;
            }
            case BTLFX_SHAPE_MODULE:
                off.pad = 0;
                if (emitter->tail->s.module.spawnFn(&off, &rot, frame, dur) != 0) {
                    anim->duration = 0;
                }
                if (off.pad != 0) {
                    base = &scratch;
                    *(s32 *)&scratch.vx = 0;
                    *(s32 *)&scratch.vz = 0;
                } else {
                    base = emitter->origin;
                }
                break;
            }
            part->pos.vx = (off.vx + base->vx) << 12;
            part->pos.vy = (off.vy + base->vy) << 12;
            part->pos.vz = (off.vz + base->vz) << 12;
            switch (anim->rot.pad) {
            case BTLFX_LAUNCH_SHAPE_FLIPPED:
                rot.vx += btlfx_randCentered(anim->rotStep.pad);
                rot.vy += btlfx_randCentered(anim->rotStep.pad);
                rot.vx = (rot.vx + 0x800) & 0xFFF;
                break;
            case BTLFX_LAUNCH_RANDOM:
                btlfx_randRotation(&rot);
                break;
            case BTLFX_LAUNCH_EMITTER:
                ((XY32 *)&rot)[0].w = ((XY32 *)&emitter->rot)[0].w;
                ((XY32 *)&rot)[1].w = ((XY32 *)&emitter->rot)[1].w;
                rot.vx += btlfx_randCentered(anim->rotStep.pad);
                rot.vy += btlfx_randCentered(anim->rotStep.pad);
                break;
            case BTLFX_LAUNCH_SHAPE:
                rot.vx += btlfx_randCentered(anim->rotStep.pad);
                rot.vy += btlfx_randCentered(anim->rotStep.pad);
                break;
            }
            if (el != NULL && el->alignRot != 0) {
                part->elemRot.s.x += rot.vx;
                part->elemRot.s.y += rot.vy;
            }
            switch (emitter->file->kind) {
            case BTLFX_MOTION_ORBIT: {
                EffEmit *mot = emitter->file;
                acc = emitter->cur.orbit.speed;
                btlfx_rotateOffset(&rot, acc + btlfx_randCentered(mot->m.orbit.speedSpread), (s32)&part->motA.vel);
                acc = mot->m.orbit.drag + btlfx_randCentered(mot->m.orbit.dragSpread);
                if (acc < 0) {
                    acc = 0;
                }
                part->motB.orbit.drag = acc;
                part->motB.orbit.dir.w = *(s32 *)&rot.vx;
                part->motB.orbit.turnX = mot->m.orbit.turnX + btlfx_randCentered(mot->m.orbit.turnXSpread);
                part->motB.orbit.turnY = mot->m.orbit.turnY + btlfx_randCentered(mot->m.orbit.turnYSpread);
                part->motB.orbit.radius = emitter->cur.orbit.radius + btlfx_randCentered(mot->m.orbit.radiusSpread);
                break;
            }
            case BTLFX_MOTION_SPIRAL: {
                EffEmit *mot = emitter->file;
                part->motB.spiral.axis.w = *(s32 *)&rot.vx;
                btlfx_lerpVec(&mot->m.spiral.radiusSpeedStart, &mot->m.spiral.radiusSpeedEnd, t, &scratch.vx);
                part->motA.spin.speed = scratch.vy + btlfx_randCentered(mot->m.spiral.spinSpeedSpread);
                part->motB.spiral.radiusSpeed = scratch.vx + btlfx_randCentered(mot->m.spiral.radiusSpeedSpread);
                part->motB.spiral.axialSpeed = emitter->cur.spiral.axialSpeed + btlfx_randCentered(mot->m.spiral.axialSpeedSpread);
                *(s32 *)&scratch.vx = 0;
                *(s32 *)&scratch.vz = 0;
                btlfx_toLocalPolar(&off, &scratch, &rot, (s32)&k, (s32)&val, &d58);
                part->motA.spin.angle = d58;
                part->motB.spiral.axial = k;
                part->motB.spiral.radius = val;
                part->pos.vx = base->vx << 12;
                part->pos.vy = base->vy << 12;
                part->pos.vz = base->vz << 12;
                part->motA.spin.fall = 0;
                break;
            }
            case BTLFX_MOTION_WOBBLE: {
                EffEmit *mot = emitter->file;
                *(s32 *)&part->motA.wobble.phaseX = *(s32 *)&rot.vx;
                part->motB.wobble.swingXSpeed = mot->m.wobble.swingXSpeed + btlfx_randCentered(mot->m.wobble.swingXSpread);
                part->motB.wobble.swingYSpeed = mot->m.wobble.swingYSpeed + btlfx_randCentered(mot->m.wobble.swingYSpread);
                part->motA.wobble.radius = emitter->cur.wobble.radius + btlfx_randCentered(mot->m.wobble.radiusSpread);
                part->motB.wobble.dirX = rot.vx + btlfx_randCentered(mot->m.wobble.dirXSpread);
                part->motB.wobble.dirY = rot.vy + btlfx_randCentered(mot->m.wobble.dirYSpread);
                part->motB.wobble.fall = 0;
                break;
            }
            case BTLFX_MOTION_HELIX: {
                EffEmit *mot = emitter->file;
                part->motB.helix.axis.w = *(s32 *)&rot.vx;
                btlfx_lerpVec(&mot->m.helix.radiusStart, &mot->m.helix.radiusEnd, t, &scratch.vx);
                part->motA.spin.speed = scratch.vy + btlfx_randCentered(mot->m.helix.spinSpeedSpread);
                part->motB.helix.radius = scratch.vx + btlfx_randCentered(mot->m.helix.radiusSpread);
                part->motB.helix.axialSpeed = emitter->cur.spiral.axialSpeed + btlfx_randCentered(mot->m.helix.axialSpeedSpread);
                part->motA.spin.angle = btlfx_rand();
                part->motB.helix.axial = 0;
                part->motA.spin.fall = 0;
                break;
            }
            case BTLFX_MOTION_BALLISTIC:
            case BTLFX_MOTION_HOMING: {
                EffEmit *mot = emitter->file;
                acc = emitter->cur.ballistic.speed;
                btlfx_rotateOffset(&rot, acc + btlfx_randCentered(mot->m.ballistic.speedSpread), (s32)&part->motA.vel);
                acc = mot->m.ballistic.drag + btlfx_randCentered(mot->m.ballistic.dragSpread);
                if (acc < 0) {
                    acc = 0;
                }
                part->motB.ballistic.drag = acc;
                break;
            }
            }
            if (el != NULL && ((u32)(el->kind - BTLFX_ELEMK_TRAIL_3D) < 2 || el->kind == BTLFX_ELEMK_TRAIL_SCREEN || el->kind2 == BTLFX_ELEMK_TRAIL_3D || el->kind2 == BTLFX_ELEMK_TRAIL_FLAT || el->kind2 == BTLFX_ELEMK_TRAIL_SCREEN)) {
                scratch.vx = part->pos.vx >> 12;
                scratch.vy = part->pos.vy >> 12;
                scratch.vz = part->pos.vz >> 12;
                btlfx_drawTrail((u8 *)el->tex, (s32)&scratch, NULL, -1, 0, 0, 0, (u8 *)btlfx_getParticleTrail(part, emitter), 0);
            }
        }
    }
}

void btlfx_runSubEmitter(Pool30 *pool, Pool30Entry *parent, SVECTOR *pos, SVECTOR *rot) {
    SVECTOR off;
    SVECTOR ang;
    SVECTOR dir;
    s32 d40;
    s32 cnt;
    s32 k;
    Pool6CEntry *owner;
    EffSub *sub;
    EffElem *el;
    Pool30Entry *part;
    s32 count;
    s32 frame;
    s32 t;
    s32 val; /* spawn rate, weight sum, life, launch speed and drag in turn (separate locals don't match) */
    s32 i;

    owner = parent->owner;
    el = NULL;
    if (owner->state == BTLFX_EMIT_FREE) {
        return;
    }
    if (owner->state != BTLFX_EMIT_DONE) {
        sub = owner->sub - 1;
        frame = parent->frame;
        t = (frame << 12) / parent->frames;
        if ((!(parent->spawnFlags & BTLFX_PART_SUB) || owner->frame < owner->tail->anim.duration) && frame >= sub->startFrame - 1) {
            val = btlfx_lerp(sub->rateStart, sub->rateEnd, t);
            cnt = val >> 8;
            k = (parent->spawnFlags & BTLFX_PART_SUB_FRAC) + ((val >> 1) & 0x7F);
            if (k >= 0x80) {
                k -= 0x80;
                if (k == 0) {
                    k = 1;
                }
                cnt++;
            }
            parent->spawnFlags = (u8)k | (parent->spawnFlags & BTLFX_PART_SUB);
            count = cnt;
            for (i = 0; i < count; i++) {
                if (owner->live.s.y >= sub->maxParticles) {
                    return;
                }
                part = btlfx_allocParticle(pool);
                if (part == NULL) {
                    return;
                }
                owner->live.s.y++;
                cnt = btlfx_rand() & 0x7F;
                k = 0;
                val = 0;
                for (; k < sub->elemCount; k++) {
                    val += sub->weights[k];
                    if (cnt < val) {
                        break;
                    }
                }
                if (k >= sub->elemCount) {
                    cnt = 0xFF;
                } else {
                    cnt = sub->elems[k];
                }
                part->elem = cnt;
                part->elemFrame = 0;
                if (cnt == 0xFF) {
                    val = btlfx_lerp(((EffEmit *)owner->sub)->lifeStart, ((EffEmit *)owner->sub)->lifeEnd, t);
                    val += btlfx_randCentered(((EffEmit *)owner->sub)->lifeSpread);
                    if (val <= 0) {
                        val = 1;
                    }
                } else {
                    el = &owner->elems[cnt];
                    if (el->frameMode == 1) {
                        val = el->frameCount;
                    } else {
                        val = btlfx_lerp(((EffEmit *)owner->sub)->lifeStart, ((EffEmit *)owner->sub)->lifeEnd, t);
                    val += btlfx_randCentered(((EffEmit *)owner->sub)->lifeSpread);
                        if (val <= 0) {
                            val = 1;
                        }
                    }
                    off.vx = (el->spawnRotStep[0] * 8) & 0xFFF;
                    off.vy = (el->spawnRotStep[1] * 8) & 0xFFF;
                    off.vz = (el->spawnRotStep[2] * 8) & 0xFFF;
                    gte_lddp(frame);
                    gte_ldsv(&off);
                    gte_gpf0();
                    ang.vx = (el->baseRot[0] * 16 + btlfx_randCentered(el->rotSpread[0] * 16)) & 0xFFF;
                    ang.vy = (el->baseRot[1] * 16 + btlfx_randCentered(el->rotSpread[1] * 16)) & 0xFFF;
                    ang.vz = (el->baseRot[2] * 16 + btlfx_randCentered(el->rotSpread[2] * 16)) & 0xFFF;
                    gte_lddp(1);
                    gte_ldsv(&ang);
                    gte_gpl0();
                    part->spin = btlfx_randCentered(el->spinRange);
                    gte_stsv(&part->elemRot);
                }
                if (((EffEmit *)owner->sub)->emitsSub != 0) {
                    part->spawnFlags = BTLFX_PART_SUB | BTLFX_PART_SUB_FRAC;
                } else {
                    part->spawnFlags = BTLFX_PART_SUB;
                }
                part->slot = owner->subSpawnCount % 255 + 1;
                part->frames = val;
                part->frame = 0;
                part->owner = owner;
                part->timeStep = btlfx_randCentered(sub->stepSpread) + 0x40;
                owner->subSpawnCount++;
                if (owner->trails != 0) {
                    owner->subSpawnCount %= owner->trailSlots;
                }
                gte_lddp(0x1000 - t);
                gte_ldsv(sub->radiusStart);
                gte_gpf12();
                btlfx_randRotation(&ang);
                gte_lddp(t);
                gte_ldsv(sub->radiusEnd);
                gte_gpl12();
                gte_stsv(&dir);
                btlfx_rotateOffset(&ang, btlfx_randRange(dir.vx, dir.vy), (s32)&off);
                part->pos.vx = (off.vx + pos->vx) << 12;
                part->pos.vy = (off.vy + pos->vy) << 12;
                part->pos.vz = (off.vz + pos->vz) << 12;
                switch (sub->rotMode) {
                case BTLFX_LAUNCH_RANDOM:
                    btlfx_randRotation(&ang);
                    break;
                case BTLFX_LAUNCH_EMITTER:
                    ((XY32 *)&ang)[0].w = ((XY32 *)&owner->rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)&owner->rot)[1].w;
                    ang.vx += btlfx_randCentered(sub->rotJitter);
                    ang.vy += btlfx_randCentered(sub->rotJitter);
                    break;
                case BTLFX_LAUNCH_PARENT:
                    ((XY32 *)&ang)[0].w = ((XY32 *)rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)rot)[1].w;
                    ang.vx += btlfx_randCentered(sub->rotJitter);
                    ang.vy += btlfx_randCentered(sub->rotJitter);
                    break;
                case BTLFX_LAUNCH_SHAPE:
                    ang.vx += btlfx_randCentered(sub->rotJitter);
                    ang.vy += btlfx_randCentered(sub->rotJitter);
                    break;
                case BTLFX_LAUNCH_PARENT_REVERSED:
                    ((XY32 *)&ang)[0].w = ((XY32 *)rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)rot)[1].w;
                    ang.vx += btlfx_randCentered(sub->rotJitter);
                    ang.vy += btlfx_randCentered(sub->rotJitter);
                    ang.vx = (ang.vx + 0x800) & 0xFFF;
                    break;
                case BTLFX_LAUNCH_SHAPE_FLIPPED:
                    ang.vx += btlfx_randCentered(sub->rotJitter);
                    ang.vy += btlfx_randCentered(sub->rotJitter);
                    ang.vx = (ang.vx + 0x800) & 0xFFF;
                    break;
                }
                if (el != NULL && el->alignRot != 0) {
                    part->elemRot.s.x += ang.vx;
                    part->elemRot.s.y += ang.vy;
                }
                switch (((EffEmit *)owner->sub)->kind) {
                case BTLFX_MOTION_ORBIT:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    val = btlfx_lerp(mot->m.orbit.speedStart, mot->m.orbit.speedEnd, t);
                    btlfx_rotateOffset(&ang, val + btlfx_randCentered(mot->m.orbit.speedSpread), (s32)&part->motA.vel);
                    val = mot->m.orbit.drag + btlfx_randCentered(mot->m.orbit.dragSpread);
                    if (val < 0) {
                        val = 0;
                    }
                    part->motB.orbit.drag = val;
                    part->motB.orbit.dir.w = *(s32 *)&ang.vx;
                    part->motB.orbit.turnX = mot->m.orbit.turnX + btlfx_randCentered(mot->m.orbit.turnXSpread);
                    part->motB.orbit.turnY = mot->m.orbit.turnY + btlfx_randCentered(mot->m.orbit.turnYSpread);
                    part->motB.orbit.radius = btlfx_lerp(mot->m.orbit.radiusStart, mot->m.orbit.radiusEnd, t) + btlfx_randCentered(mot->m.orbit.radiusSpread);
                    break;
                    }
                case BTLFX_MOTION_SPIRAL:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    part->motB.spiral.axis.w = *(s32 *)&ang.vx;
                    btlfx_lerpVec(&mot->m.spiral.radiusSpeedStart, &mot->m.spiral.radiusSpeedEnd, t, (s16 *)&dir);
                    part->motA.spin.speed = dir.vy + btlfx_randCentered(mot->m.spiral.spinSpeedSpread);
                    part->motB.spiral.radiusSpeed = dir.vx + btlfx_randCentered(mot->m.spiral.radiusSpeedSpread);
                    part->motB.spiral.axialSpeed = btlfx_lerp(mot->m.spiral.axialSpeedStart, mot->m.spiral.axialSpeedEnd, t) + btlfx_randCentered(mot->m.spiral.axialSpeedSpread);
                    *(s32 *)&dir.vx = 0;
                    *(s32 *)&dir.vz = 0;
                    btlfx_toLocalPolar(&off, &dir, &ang, (s32)&d40, (s32)&cnt, &k);
                    part->motA.spin.angle = k;
                    part->motB.spiral.axial = d40;
                    part->motB.spiral.radius = cnt;
                    part->pos.vx = parent->pos.vx;
                part->pos.vy = parent->pos.vy;
                part->pos.vz = parent->pos.vz;
                    part->motA.spin.fall = 0;
                    break;
                    }
                case BTLFX_MOTION_WOBBLE:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    *(s32 *)&part->motA.wobble.phaseX = *(s32 *)&ang.vx;
                    part->motB.wobble.swingXSpeed = mot->m.wobble.swingXSpeed + btlfx_randCentered(mot->m.wobble.swingXSpread);
                    part->motB.wobble.swingYSpeed = mot->m.wobble.swingYSpeed + btlfx_randCentered(mot->m.wobble.swingYSpread);
                    part->motA.wobble.radius = btlfx_lerp(mot->m.wobble.radiusStart, mot->m.wobble.radiusEnd, t) + btlfx_randCentered(mot->m.wobble.radiusSpread);
                    part->motB.wobble.dirX = ang.vx + btlfx_randCentered(mot->m.wobble.dirXSpread);
                    part->motB.wobble.dirY = ang.vy + btlfx_randCentered(mot->m.wobble.dirYSpread);
                    part->motB.wobble.fall = 0;
                    break;
                    }
                case BTLFX_MOTION_HELIX:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    part->motB.helix.axis.w = *(s32 *)&ang.vx;
                    btlfx_lerpVec(&mot->m.helix.radiusStart, &mot->m.helix.radiusEnd, t, (s16 *)&dir);
                    part->motA.spin.speed = dir.vy + btlfx_randCentered(mot->m.helix.spinSpeedSpread);
                    part->motB.helix.radius = dir.vx + btlfx_randCentered(mot->m.helix.radiusSpread);
                    part->motB.helix.axialSpeed = btlfx_lerp(mot->m.helix.axialSpeedStart, mot->m.helix.axialSpeedEnd, t) + btlfx_randCentered(mot->m.helix.axialSpeedSpread);
                    part->motA.spin.angle = btlfx_rand();
                    part->motB.helix.axial = 0;
                    part->motA.spin.fall = 0;
                    break;
                    }
                case BTLFX_MOTION_BALLISTIC:
                case BTLFX_MOTION_HOMING:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    val = btlfx_lerp(mot->m.ballistic.speedStart, mot->m.ballistic.speedEnd, t);
                    btlfx_rotateOffset(&ang, val + btlfx_randCentered(mot->m.ballistic.speedSpread), (s32)&part->motA.vel);
                    val = mot->m.ballistic.drag + btlfx_randCentered(mot->m.ballistic.dragSpread);
                    if (val < 0) {
                        val = 0;
                    }
                    part->motB.ballistic.drag = val;
                    break;
                    }
                }
                if (el != NULL && ((u32)(el->kind - BTLFX_ELEMK_TRAIL_3D) < 2 || el->kind == BTLFX_ELEMK_TRAIL_SCREEN || el->kind2 == BTLFX_ELEMK_TRAIL_3D || el->kind2 == BTLFX_ELEMK_TRAIL_FLAT || el->kind2 == BTLFX_ELEMK_TRAIL_SCREEN)) {
                    dir.vx = part->pos.vx >> 12;
                    dir.vy = part->pos.vy >> 12;
                    dir.vz = part->pos.vz >> 12;
                    btlfx_drawTrail((u8 *)el->tex, (s32)&dir, NULL, -1, 0, 0, 0, (u8 *)btlfx_getParticleTrail(part, owner), 0);
                }
            }
        }
    }
}

void btlfx_heapInit(Heap *heap, s32 bytes) {
    s32 size = btlfx_align4(bytes) - 0xC;
    heap->cursor = heap->data;
    heap->free = size;
    heap->size = size;
}

s32 btlfx_heapAlloc(Heap *h, s32 size) {
    s32 sz;
    u8 *mem;
    s32 *words;

    sz = btlfx_align4(size);
    if (h->free < sz) {
        words = (s32 *)h->data;
        h->cursor = (u8 *)words;
        h->free = h->size;
    }
    mem = h->cursor;
    h->free -= sz;
    h->cursor = mem + sz;
    words = (s32 *)mem;
    for (size = 0; size < sz >> 2; size++) {
        *words++ = 0;
    }
    return (s32)mem;
}

s32 btlfx_heapWrapIfFull(Heap *heap, s32 bytes) {
    s32 size = btlfx_align4(bytes);
    s32 wrapped = 1;
    if (heap->free < size) {
        heap->cursor = heap->data;
        heap->free = heap->size;
    } else {
        wrapped = 0;
    }
    return wrapped;
}

void btlfx_heapFree(void *mem) {
}

void btlfx_initParticleSystem(void) {
    btlfx_initParticles(0x3000, g_btlfxParticleBuf, &g_btlfxParticles);
    btlfx_initEmitters(0x920, &g_btlfxEmitters);
}

void btlfx_updateParticleSystem(void) {
    btlfx_runEmitters(&g_btlfxParticles, &g_btlfxEmitters);
    btlfx_updateParticles(&g_btlfxParticles);
    btlfx_updateEmitters(&g_btlfxParticles, &g_btlfxEmitters);
}

Pool6CEntry *btlfx_startEmitter(s32 data, s32 pos) {
    return btlfx_spawnEmitter(&g_btlfxParticles, &g_btlfxEmitters, (u8 *)data, pos);
}

void btlfx_moveEmitterTo(s32 emitter, s32 pos) {
    if (emitter != 0) {
        btlfx_moveEmitter(&g_btlfxParticles, emitter, pos);
    }
}

void btlfx_stopEmitter(Pool6CEntry *emitter) {
    if (emitter != NULL) {
        btlfx_killEmitter(&g_btlfxParticles, emitter);
    }
}

void btlfx_initAll(void) {
    btlfx_initCommonData();
    btlfx_resetAll();
}

void btlfx_resetAll(void) {
    btlfx_clearTargets();
    btlfx_reset();
    btlfx_initTasks();
    btlfx_resetLoader();
    btlfx_initParticleSystem();
    btlfx_initSounds();
    btlfx_clearEffModels();
    btlfx_clearSummonModels();
    vib_clear();
}

void btlfx_copyDisplayToDraw(void) {
    RECT rect;
    DR_MOVE *prim;
    s32 i;
    rect.x = g_btlfxEnv->dispEnv->disp.x;
    rect.y = g_btlfxEnv->dispEnv->disp.y;
    rect.w = 0x20;
    rect.h = 0xDA;
    i = 0;
    prim = (DR_MOVE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(prim + 10);
    do {
        SetDrawMove(prim, &rect, g_btlfxEnv->drawEnv->clip.x + i, g_btlfxEnv->drawEnv->clip.y);
        AddPrim(g_btlfxEnv->ot, prim++);
        i += 0x20;
        rect.x += 0x20;
    } while (i < 0x140);
    if (g_btlfxSoundPaused == 0) {
        g_btlfxSoundPaused = 1;
        akao_pause(2);
    }
}
