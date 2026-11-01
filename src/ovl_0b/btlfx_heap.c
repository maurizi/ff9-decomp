#include "ovl_0b.h"

void btlfx_initParticles(s32 arg0, Pool30Entry *arg1, Pool30 *arg2) {
    s32 i = 0;
    arg2->capacity = arg0 / (s32)sizeof(Pool30Entry);
    arg2->entries = arg1;
    arg2->live = 0;
    arg2->maxUsed = 0;
    for (; i < arg2->capacity;) {
        i++;
        arg1->slot = 0;
        arg1++;
    }
}

s32 btlfx_getParticleTrail(Pool30Entry *arg0, Pool6CEntry *arg1) {
    s32 p = arg1->trails;
    s32 t;
    if (p == 0) {
        return 0;
    }
    t = arg0->slot;
    t = (t - 1) * arg1->trailStride;
    return p + t;
}

s32 btlfx_updateParticles(Pool30 *pool) {
    SVECTOR rel;
    SVECTOR pos;
    SVECTOR v;
    VECTOR d;
    s32 alpha;
    s32 cnt;
    s32 scale;
    Pool30Entry *e;
    Pool6CEntry *owner;
    EffEmit *m;
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
    e = pool->entries;
    amp = 0;
    i = 0;
    g_btlfxSavedOtZ = g_btlfxEnv->otZ;
    pool->live = 0;
    for (; i <= pool->maxUsed; i++, e++) {
        if (e->slot == 0) {
            continue;
        }
        frame = e->frame;
        frames = e->frames;
        owner = e->owner;
        if (frame >= frames) {
            e->slot = 0;
            if (e->spawnFlags & BTLFX_PART_SUB) {
                owner->live.s.y--;
            } else {
                owner->live.s.x--;
            }
            continue;
        }
        last = i;
        pool->live++;
        t = (frame << 12) / frames;
        if (e->spawnFlags & BTLFX_PART_SUB) {
            m = (EffEmit *)owner->sub;
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
            m = owner->file;
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
                    rel.vx = e->pos.vx >> 12;
                    rel.vy = e->pos.vy >> 12;
                    rel.vz = e->pos.vz >> 12;
                }
            }
        }
        switch (m->kind) {
        case BTLFX_MOTION_BALLISTIC:
            btlfx_stepParticle(e, rs, rp, &pos);
            if (m->m.ballistic.jitter != 0) {
                e->motA.vel.x += btlfx_randCentered(m->m.ballistic.jitterX);
                e->motA.vel.y += btlfx_randCentered(m->m.ballistic.jitterY);
                e->motA.vel.z += btlfx_randCentered(m->m.ballistic.jitterZ);
            }
            if (m->m.ballistic.floorMode != 0) {
                if (m->m.ballistic.floorMode == 1) {
                    if (owner->rot.pad - m->m.ballistic.floorMargin < pos.vy && e->motA.vel.y > 0) {
                        e->motA.vel.y = -e->motA.vel.y;
                        e->motA.vel.y = (e->motA.vel.y * m->m.ballistic.bounce) >> 8;
                    }
                } else if (owner->rot.pad - m->m.ballistic.floorMargin < pos.vy) {
                    if (e->motA.vel.y > 0) {
                        s32 x = e->motA.vel.y * (owner->rot.pad - pos.vy);
                        if (m->m.ballistic.floorMargin != 0) {
                            x /= m->m.ballistic.floorMargin;
                        }
                        e->motA.vel.y = x;
                    }
                    if (owner->rot.pad - (m->m.ballistic.floorMargin >> 1) < pos.vy) {
                        e->motA.vel.y = (s16)(e->motA.vel.y - 0x40) - ((owner->gravity * e->timeStep) >> 6);
                    }
                    if (pos.vy > owner->rot.pad) {
                        e->motA.vel.y = 0;
                    }
                }
            }
            break;
        case BTLFX_MOTION_ORBIT:
            btlfx_stepParticleAltDrag(e, rs, rp, &pos);
            e->motB.orbit.dir.s.x += e->motB.orbit.turnX;
            e->motB.orbit.dir.s.y += e->motB.orbit.turnY;
            v.vx = e->motB.orbit.dir.s.x;
            v.vy = e->motB.orbit.dir.s.y;
            switch (m->m.orbit.radiusCurve) {
            case BTLFX_RADIUS_SIN:
                rs = (math_sin(btlfx_lerp(m->m.orbit.sinStart, m->m.orbit.sinEnd, t)) * e->motB.orbit.radius) >> 12;
                break;
            case BTLFX_RADIUS_GROW:
                rs = (e->motB.orbit.radius * t) >> 12;
                break;
            case BTLFX_RADIUS_SHRINK:
                rs = e->motB.orbit.radius - ((e->motB.orbit.radius * t) >> 12);
                break;
            }
            btlfx_pointAhead((s16 *)&pos, (s16 *)&v, rs, (s16 *)&pos);
            break;
        case BTLFX_MOTION_SPIRAL:
            btlfx_stepParticleNoDrag(e, rs, rp, &pos);
            e->motA.spin.angle += e->motA.spin.speed;
            e->motB.spiral.axial += e->motB.spiral.axialSpeed;
            e->motB.spiral.radius += e->motB.spiral.radiusSpeed;
            btlfx_offsetPoint(&pos, (s32)&pos, e->motB.spiral.axial >> 2, &e->motB.spiral.axis, e->motB.spiral.radius >> 2, e->motA.spin.angle);
            break;
        case BTLFX_MOTION_WOBBLE:
            btlfx_stepParticleNoDragAlt(e, rs, rp, &pos);
            e->motA.wobble.phaseX += e->motB.wobble.swingXSpeed;
            e->motA.wobble.phaseY += e->motB.wobble.swingYSpeed;
            v.vx = e->motB.wobble.dirX + math_sin(e->motA.wobble.phaseX);
            v.vy = e->motB.wobble.dirY + math_sin(e->motA.wobble.phaseY);
            switch (m->m.wobble.radiusCurve) {
            case BTLFX_RADIUS_SIN:
                rs = (math_sin(btlfx_lerp(m->m.wobble.sinStart, m->m.wobble.sinEnd, t)) * e->motA.wobble.radius) >> 12;
                break;
            case BTLFX_RADIUS_GROW:
                rs = (e->motA.wobble.radius * t) >> 12;
                break;
            case BTLFX_RADIUS_SHRINK:
                rs = e->motA.wobble.radius - ((e->motA.wobble.radius * t) >> 12);
                break;
            }
            btlfx_pointAhead((s16 *)&pos, (s16 *)&v, rs, (s16 *)&pos);
            break;
        case BTLFX_MOTION_HELIX:
            btlfx_stepParticleNoDrag(e, rs, rp, &pos);
            e->motB.helix.axial += e->motB.helix.axialSpeed;
            e->motA.spin.angle += e->motA.spin.speed;
            switch (m->m.helix.radiusCurve) {
            case BTLFX_RADIUS_SIN:
                rs = (math_sin(btlfx_lerp(m->m.helix.sinStart, m->m.helix.sinEnd, t)) * e->motB.helix.radius) >> 12;
                break;
            case BTLFX_RADIUS_GROW:
                rs = (e->motB.helix.radius * t) >> 12;
                break;
            case BTLFX_RADIUS_SHRINK:
                rs = e->motB.helix.radius - ((e->motB.helix.radius * t) >> 12);
                break;
            }
            btlfx_offsetPoint(&pos, (s32)&pos, e->motB.helix.axial >> 6, &e->motB.helix.axis, rs, e->motA.spin.angle);
            break;
        case BTLFX_MOTION_HOMING:
            btlfx_stepParticle(e, rs, rp, &pos);
            if (m->m.homing.jitter != 0) {
                e->motA.vel.x += btlfx_randCentered(m->m.homing.jitterX);
                e->motA.vel.y += btlfx_randCentered(m->m.homing.jitterY);
                e->motA.vel.z += btlfx_randCentered(m->m.homing.jitterZ);
            }
            rs = btlfx_lerp(m->m.homing.steerStart, m->m.homing.steerEnd, t);
            btlfx_aimAngles((s16 *)&pos, (s16 *)owner->target, (s16 *)&v);
            v.vx += m->m.homing.aimX;
            v.vy += m->m.homing.aimY;
            btlfx_pointAhead(&e->motA.vel.x, (s16 *)&v, rs, &e->motA.vel.x);
            break;
        case BTLFX_MOTION_MODULE:
            if (m->m.module.update(e, owner, &pos, btlfx_getParticleTrail(e, owner), frame, frames) != 0) {
                e->slot = 0;
                if (e->spawnFlags & BTLFX_PART_SUB) {
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
                cnt = btlfx_distance(&pos, owner->fieldPos);
                if (cnt <= range) {
                    d.vx = pos.vx - owner->fieldPos->vx;
                    d.vy = pos.vy - owner->fieldPos->vy;
                    d.vz = pos.vz - owner->fieldPos->vz;
                    VectorNormalS(&d, &v);
                }
                break;
            case BTLFX_FIELD_ATTRACT:
                cnt = btlfx_distance(&pos, owner->fieldPos);
                if (cnt <= range) {
                    d.vx = owner->fieldPos->vx - pos.vx;
                    d.vy = owner->fieldPos->vy - pos.vy;
                    d.vz = owner->fieldPos->vz - pos.vz;
                    VectorNormalS(&d, &v);
                }
                break;
            case BTLFX_FIELD_UP:
            case BTLFX_FIELD_UP_2:
                *(s32 *)&v.vx = *(s32 *)&pos.vx;
                v.vy = owner->fieldPos->vy;
                *(s32 *)&v.vz = *(s32 *)&pos.vz;
                cnt = btlfx_distance(&v, owner->fieldPos);
                ((s32 *)&v)[0] = 0;
                v.vy = -0x1000;
                break;
            case BTLFX_FIELD_ALONG_AXIS:
                btlfx_toLocalPolar(&pos, owner->fieldPos, owner->fieldRot, (s32)&alpha, (s32)&cnt, &scale);
                if (cnt <= range) {
                    btlfx_rotateOffset(owner->fieldRot, 0x1000, (s32)&v);
                }
                break;
            case BTLFX_FIELD_AROUND_AXIS:
                btlfx_toLocalPolar(&pos, owner->fieldPos, owner->fieldRot, (s32)&alpha, (s32)&cnt, &scale);
                if (cnt <= range) {
                    ((s32 *)&v)[1] = 0;
                    ((s32 *)&v)[0] = 0;
                    btlfx_offsetPoint(&v, (s32)&v, 0, (XY32 *)owner->fieldRot, 0x1000, scale);
                }
                break;
            case BTLFX_FIELD_FADE:
                cnt = btlfx_distance(&pos, owner->fieldPos);
                break;
            case BTLFX_FIELD_FADE_FLAT:
                *(s32 *)&v.vx = *(s32 *)&pos.vx;
                v.vy = owner->fieldPos->vy;
                *(s32 *)&v.vz = *(s32 *)&pos.vz;
                cnt = btlfx_distance(&v, owner->fieldPos);
                break;
            case BTLFX_FIELD_FADE_POLAR:
                btlfx_toLocalPolar(&pos, owner->fieldPos, owner->fieldRot, (s32)&alpha, (s32)&cnt, &scale);
                break;
            }
            if (cnt <= range) {
                scale = (cnt << 10) / range;
                d.vx = v.vx;
                d.vy = v.vy;
                d.vz = v.vz;
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
                    gte_ldir(&d);
                    gte_gpf12();
                    gte_stir(&d);
                    e->pos.vx += d.vx << 6;
                    e->pos.vy += d.vy << 6;
                    e->pos.vz += d.vz << 6;
                } else {
                    ofs = amp;
                }
            } else if (mode >= BTLFX_FIELD_FADE) {
                ofs = 0;
            }
        }
        g_btlfxEnv->otZ = owner->otZ;
        if (e->elem != 0xFF) {
            el = &owner->elems[e->elem];
            fl = el->flags;
            vp = NULL;
            if (!(fl & BTLFX_ELEM_NO_ROT)) {
                vp = &v;
                *(s32 *)&vp->vx = e->elemRot.w;
                vp->vz = e->elemRotZ;
                vp->vx += el->rotStepX;
                vp->vy += el->rotStepY;
                vp->vz += el->rotStepZ;
                v.pad = ((fl >> 2) ^ 1) & 1;
                rs = e->spin * 4;
                if (fl & BTLFX_ELEM_SPIN_X) {
                    vp->vx += rs;
                }
                if (fl & BTLFX_ELEM_SPIN_Y) {
                    vp->vy += rs;
                }
                if (fl & BTLFX_ELEM_SPIN_Z) {
                    vp->vz += rs;
                }
                e->elemRot.w = *(s32 *)&vp->vx;
                e->elemRotZ = vp->vz;
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
                        vp = &v;
                        ((s32 *)&v)[0] = 0;
                        ((s32 *)&v)[1] = 0;
                    }
                    el->draw(e, owner, &pos, vp, &scale, &alpha, &cnt, frame, frames);
                    break;
                case BTLFX_ELEMK_PARTICLES:
                    btlfx_drawParticles((u8 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha, 0, 0);
                    break;
                case BTLFX_ELEMK_SPRITE:
                    btlfx_drawSprite((u8 *)mode, &pos, e->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_ANIM_QUADS:
                    btlfx_drawAnimQuads((u8 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_SPRITES:
                    btlfx_drawSprites((u8 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_TRAIL_3D:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, (s32 *)owner->origin, e->elemFrame, scale, alpha, cnt, (u8 *)btlfx_getParticleTrail(e, owner), 1);
                    break;
                case BTLFX_ELEMK_TRAIL_FLAT:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, NULL, e->elemFrame, scale, alpha, cnt, (u8 *)btlfx_getParticleTrail(e, owner), 2);
                    break;
                case BTLFX_ELEMK_RINGS:
                    btlfx_drawRings((u8 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_FAN:
                    btlfx_drawFan((Unk800D2938 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha);
                    break;
                case BTLFX_ELEMK_TRAIL_SCREEN:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, NULL, e->elemFrame, scale, alpha, cnt, (u8 *)btlfx_getParticleTrail(e, owner), 0);
                    break;
                }
                if (el->kind2 == BTLFX_ELEMK_NONE) {
                    break;
                }
            }
            if (el->frameMode == 1) {
                e->elemFrame++;
            } else if (el->frameMode == 2) {
                rs = e->elemFrame;
                rs++;
                if (rs >= el->frameCount) {
                    rs = 0;
                }
                e->elemFrame = rs;
            }
        }
        if (owner->sub != NULL && (e->spawnFlags & BTLFX_PART_SUB_FRAC)) {
            if (lvl >= BTLFX_LAUNCH_PARENT) {
                btlfx_aimAngles((s16 *)&rel, (s16 *)&pos, (s16 *)&v);
            }
            btlfx_runSubEmitter(pool, e, &pos, &v);
        }
        frame++;
        e->frame = frame;
    }
    pool->maxUsed = last;
    g_btlfxEnv->otZ = g_btlfxSavedOtZ;
    return pool->live;
}

void btlfx_initEmitters(u32 arg0, Pool6C *arg1) {
    s32 i;
    Pool6CEntry *e = arg1->buf;
    arg1->capacity = (arg0 - 0x10) / sizeof(Pool6CEntry);
    arg1->entries = e;
    arg1->live = 0;
    arg1->maxUsed = 0;
    for (i = 0; i < arg1->capacity;) {
        i++;
        e->state = BTLFX_EMIT_FREE;
        e++;
    }
}

Pool6CEntry *btlfx_spawnEmitter(Pool30 *arg0, Pool6C *arg1, u8 *arg2, s32 arg3) {
    Pool6CEntry *e;
    s32 i;
    u32 k;
    u8 *p;
    EffElemHdr *h;
    EffElem *el;
    s32 *q;
    s32 v;

    e = arg1->entries;
    for (i = 0; i < arg1->capacity; i++, e++) {
        if (e->state == BTLFX_EMIT_FREE) {
            if (arg1->maxUsed < i) {
                arg1->maxUsed = i;
            }
            p = arg2;
            for (k = 0; k < 0x6C; k += 4) {
                *(s32 *)((u8 *)e + k) = 0;
            }
            e->state = BTLFX_EMIT_LIVE;
            e->spawnCount = 0;
            e->subSpawnCount = 0;
            e->spawnNow = 0;
            e->live.s.x = 0;
            e->live.s.y = 0;
            e->trails = 0;
            e->spawnFrac = 0xFF;
            e->otZ = 0;
            e->file = (EffEmit *)p;
            if (*p != 0xFF) {
                p += 0x28;
            } else {
                p += 0x10;
            }
            if (e->file->emitsSub != 0) {
                p += 0x38;
                e->sub = (EffSub *)p;
                p += 0x28;
            } else {
                e->sub = NULL;
            }
            h = (EffElemHdr *)p;
            p += 0x14;
            el = (EffElem *)p;
            e->elems = el;
            i = 0;
            p += h->elemCount * 0x28;
            for (; i < h->elemCount; i++, el++) {
                if (el->kind != BTLFX_ELEMK_CALLBACK) {
                    if (el->tex != -1 && !(el->tex & 0x80000000)) {
                        el->tex = (s32)arg2 + el->tex;
                    }
                }
                if (el->kind2 != BTLFX_ELEMK_NONE) {
                    if (el->tex2 != -1 && !(el->tex2 & 0x80000000)) {
                        el->tex2 = (s32)arg2 + el->tex2;
                    }
                }
            }
            /* tail, origin, target written through one pointer */
            q = (s32 *)&e->tail;
            q[0] = (s32)p;
            v = *(s32 *)p;
            k = (u32)p;
            if (v == BTLFX_SHAPE_MODULE) {
                ((EffTail *)k)->s.module.spawnFn = NULL;
            }
            q[1] = arg3;
            q[2] = arg3;
            do {
            } while (0);
            return e;
        }
    }
    return NULL;
}

void btlfx_moveEmitter(Pool30 *arg0, s32 arg1, s32 arg2) {
    Pool6CEntry *m = (Pool6CEntry *)arg1;
    SVECTOR *v = (SVECTOR *)arg2;
    VECTOR d;
    Pool30Entry *e;
    s32 i;
    d.vx = (v->vx - m->origin->vx) << 12;
    d.vy = (v->vy - m->origin->vy) << 12;
    d.vz = (v->vz - m->origin->vz) << 12;
    e = arg0->entries;
    for (i = 0; i < arg0->capacity; i++, e++) {
        if (e->slot != 0 && e->owner == m) {
            e->pos.vx += d.vx;
            e->pos.vy += d.vy;
            e->pos.vz += d.vz;
        }
    }
    *(s32 *)&m->origin->vx = *(s32 *)&v->vx;
    m->origin->vz = v->vz;
}

void btlfx_killEmitter(Pool30 *arg0, Pool6CEntry *arg1) {
    s32 i = 0;
    Pool30Entry *p = arg0->entries;
    if (i < arg0->capacity) {
        do {
            if (p->slot != 0 && p->owner == arg1) {
                p->frame = p->frames;
            }
            i++;
            p++;
        } while (i < arg0->capacity);
    }
    arg1->state = BTLFX_EMIT_DONE;
}

s32 btlfx_updateEmitters(Pool30 *arg0, Pool6C *arg1) {
    union {
        s16 h[4];
        s32 w;
    } a;
    s16 b[4];
    s32 i;
    s32 last;
    Pool6CEntry *e;
    EffTail *def;
    EffTail *d2;
    EffAnim *an;
    EffElemHdr *hdr;
    EffSub *sub;
    EffEmit *f;
    s16 *tbl;
    s32 frame;
    s32 t;
    s32 v;
    s32 y;
    s32 x;
    Pool6C *p = arg1;

    last = 0;
    e = p->entries;
    v = 0;
    i = 0;
    p->live = 0;
    for (; i <= p->maxUsed; i++, e++) {
        if (e->state == BTLFX_EMIT_FREE) {
            continue;
        }
        if (e->state == BTLFX_EMIT_DONE) {
            e->state = BTLFX_EMIT_FREE;
            continue;
        }
        p->live++;
        def = e->tail;
        frame = e->frame;
        an = &def->anim;
        last = i;
        y = an->duration;
        t = (frame << 12) / y;
        e->spawnNow = 0;
        if (frame >= y) {
            if (e->live.w == 0) {
                e->state = BTLFX_EMIT_DONE;
            }
            continue;
        }
        switch (def->anim.rateCurve) {
        case 0:
            v = btlfx_lerp(an->rateStart, an->rateEnd, t);
            break;
        case 1:
            v = (math_sin(t >> 1) * an->rateStart) >> 12;
            break;
        case 2:
            def = e->tail;
            if (def->shape != BTLFX_SHAPE_MODULE) {
                tbl = def->rateTable;
            } else {
                tbl = def->s.module.rateTable;
            }
            v = tbl[(frame << 4) / y];
            break;
        case 3:
            v = 0x100;
            break;
        }
        x = v >> 8;
        y = e->spawnFrac;
        y += v & 0xFF;
        if (y >= 0x100) {
            y -= 0x100;
            x++;
        }
        e->spawnFrac = y;
        e->spawnNow = x;
        if (frame == 0) {
            ((s32 *)&e->rot)[0] = ((s32 *)&an->rot)[0];
            ((s32 *)&e->rot)[1] = ((s32 *)&an->rot)[1];
        } else {
            e->rot.vx += an->rotStep.vx;
            e->rot.vy += an->rotStep.vy;
            e->rot.vz += an->rotStep.vz;
        }
        hdr = (EffElemHdr *)e->elems - 1;
        if (hdr->weightAnim == 1) {
            hdr->weights.w = 0;
            x = btlfx_lerp(0, hdr->weightCount * 4, t);
            y = (x & 3) << 5;
            v = x >> 2;
            if (y != 0) {
                x = v + 1;
                if (x >= hdr->weightCount) {
                    x = v;
                }
                hdr->weights.b[v] = -0x80 - y;
                hdr->weights.b[x] += y;
            } else {
                hdr->weights.b[v] = 0x80;
            }
        }
        a.h[0] = hdr->scaleStart;
        a.h[1] = hdr->alphaStart;
        a.h[2] = an->gravityStart;
        b[0] = hdr->scaleEnd;
        b[1] = hdr->alphaEnd;
        b[2] = an->gravityEnd;
        btlfx_lerpVec(a.h, b, t, a.h);
        gte_lddp(0x1000 - t);
        gte_ldsv(an->accelStart);
        gte_gpf12();
        e->scale = a.h[0];
        e->alpha = a.h[1];
        e->gravity = a.h[2];
        gte_lddp(t);
        gte_ldsv(an->accelEnd);
        gte_gpl12();
        gte_stsv(e->accel);
        if (an->fieldMode != 0) {
            btlfx_lerpVec(an->fieldStart, an->fieldEnd, t, a.h);
            e->field.w = a.w;
        }
        if (e->sub != NULL) {
            sub = e->sub - 1;
            btlfx_lerpVec(sub->accelStart, sub->accelEnd, t, e->subAccel);
            e->subGravity = btlfx_lerp(sub->gravityStart, sub->gravityEnd, t);
            if (sub->fieldMode != 0) {
                btlfx_lerpVec(sub->fieldStart, sub->fieldEnd, t, a.h);
                e->subField.w = a.w;
            }
        }
        d2 = e->tail;
        switch (d2->shape) {
        case BTLFX_SHAPE_SPHERE:
            a.h[0] = d2->s.sphere.radiusMinStart;
            a.h[1] = d2->s.sphere.radiusMaxStart;
            b[0] = d2->s.sphere.radiusMinEnd;
            b[1] = d2->s.sphere.radiusMaxEnd;
            btlfx_lerpVec(a.h, b, t, a.h);
            e->radiusMin = a.h[0];
            e->radiusMax = a.h[1];
            break;
        case BTLFX_SHAPE_CYLINDER:
        case BTLFX_SHAPE_AXIS:
        case BTLFX_SHAPE_TUBE:
            a.h[0] = d2->s.cylinder.radiusStart[0];
            a.h[1] = d2->s.cylinder.radiusStart[1];
            b[0] = d2->s.cylinder.radiusEnd[0];
            b[1] = d2->s.cylinder.radiusEnd[1];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->radiusMin = a.h[0];
            e->radiusMax = a.h[1];
            a.h[0] = d2->s.cylinder.spanStart[0];
            a.h[1] = d2->s.cylinder.spanStart[1];
            b[0] = d2->s.cylinder.spanEnd[0];
            b[1] = d2->s.cylinder.spanEnd[1];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->shape.span.min = a.h[0];
            e->shape.span.max = a.h[1];
            break;
        case BTLFX_SHAPE_CONE:
            a.h[0] = d2->s.cone.radiusMinStart;
            a.h[1] = d2->s.cone.radiusMaxStart;
            a.h[2] = d2->s.cone.lengthStart;
            b[0] = d2->s.cone.radiusMinEnd;
            b[1] = d2->s.cone.radiusMaxEnd;
            b[2] = d2->s.cone.lengthEnd;
            btlfx_lerpVec(a.h, b, t, a.h);
            e->radiusMin = a.h[0];
            e->radiusMax = a.h[1];
            e->shape.cone.length = a.h[2];
            break;
        }
        switch (e->file->kind) {
        case BTLFX_MOTION_BALLISTIC:
        case BTLFX_MOTION_HOMING:
            e->cur.ballistic.speed = btlfx_lerp(e->file->m.ballistic.speedStart, e->file->m.ballistic.speedEnd, t);
            break;
        case BTLFX_MOTION_ORBIT:
            f = e->file;
            e->cur.orbit.radius = btlfx_lerp(f->m.orbit.radiusStart, f->m.orbit.radiusEnd, t);
            e->cur.orbit.speed = btlfx_lerp(f->m.orbit.speedStart, f->m.orbit.speedEnd, t);
            break;
        case BTLFX_MOTION_SPIRAL:
        case BTLFX_MOTION_HELIX:
            e->cur.spiral.axialSpeed = btlfx_lerp(e->file->m.spiral.axialSpeedStart, e->file->m.spiral.axialSpeedEnd, t);
            break;
        case BTLFX_MOTION_WOBBLE:
            e->cur.wobble.radius = btlfx_lerp(e->file->m.wobble.radiusStart, e->file->m.wobble.radiusEnd, t);
            break;
        }
        frame++;
        e->frame = frame;
    }
    p->maxUsed = last;
    return p->live;
}

Pool30Entry *btlfx_allocParticle(Pool30 *arg0) {
    s32 i;
    Pool30Entry *p = arg0->entries;
    for (i = 0; i < arg0->capacity; i++, p++) {
        if (p->slot == 0) {
            if (arg0->maxUsed < i) {
                arg0->maxUsed = i;
            }
            break;
        }
    }
    if (i >= arg0->capacity) {
        p = 0;
    }
    return p;
}

void btlfx_runEmitters(Pool30 *pool, Pool6C *list) {
    SVECTOR off;
    SVECTOR rot;
    SVECTOR tmp;
    SVECTOR ang;
    CVECTOR col;
    s32 k;
    s32 r;
    s32 d58;
    Pool6C *pl;
    s32 i;
    s32 j;
    s32 dur;
    s32 t;
    Pool6CEntry *e;
    Pool30Entry *p;
    EffElemHdr *hdr;
    EffElem *el;
    EffAnim *anim;
    SVECTOR *base;
    s32 frame;
    s32 acc;
    s32 n;

    el = NULL;
    pl = list;
    e = pl->entries;
    base = NULL;
    for (i = 0; i <= pl->maxUsed; i++, e++) {
        if (e->state == BTLFX_EMIT_FREE) {
            continue;
        }
        if (e->state == BTLFX_EMIT_DONE) {
            continue;
        }
        for (j = 0; j < e->spawnNow; j++) {
            if (e->live.s.x >= e->tail->anim.maxParticles) {
                break;
            }
            p = btlfx_allocParticle(pool);
            if (p == NULL) {
                break;
            }
            frame = e->frame;
            anim = &e->tail->anim;
            e->live.s.x++;
            dur = anim->duration;
            t = (frame << 12) / dur;
            hdr = (EffElemHdr *)e->elems - 1;
            acc = 0;
            r = btlfx_rand() & 0x7F;
            for (k = 0; k < hdr->weightCount; k++) {
                acc += hdr->weights.b[k];
                if (r < acc) {
                    break;
                }
            }
            if (k >= hdr->weightCount) {
                k = 0xFF;
            }
            p->elem = k;
            p->elemFrame = 0;
            if (k == 0xFF) {
                acc = btlfx_lerp(e->file->lifeStart, e->file->lifeEnd, t);
                acc += btlfx_randCentered(e->file->lifeSpread);
                if (acc <= 0) {
                    acc = 1;
                }
            } else {
                el = &e->elems[k];
                if (el->frameMode == 1) {
                    acc = el->frameCount;
                } else {
                    acc = btlfx_lerp(e->file->lifeStart, e->file->lifeEnd, t);
                    acc += btlfx_randCentered(e->file->lifeSpread);
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
                p->spin = btlfx_randCentered(el->spinRange);
                gte_stsv(&p->elemRot);
            }
            p->slot = e->spawnCount % 255 + 1;
            p->frames = acc;
            p->frame = 0;
            p->owner = e;
            p->timeStep = btlfx_randCentered(anim->stepSpread) + 0x40;
            if (e->file->emitsSub != 0) {
                p->spawnFlags = BTLFX_PART_SUB_FRAC;
            } else {
                p->spawnFlags = 0;
            }
            e->spawnCount++;
            if (e->trails != 0) {
                e->spawnCount %= e->trailSlots;
            }
            switch (e->tail->shape) {
            case BTLFX_SHAPE_SPHERE:
                btlfx_randRotation(&rot);
                acc = btlfx_randRange(e->radiusMin, e->radiusMax);
                if (e->tail->s.sphere.axisScaleStart.cd == 1) {
                    if (rot.vx > 0x800) {
                        rot.vx -= 0x800;
                    }
                } else if (e->tail->s.sphere.axisScaleStart.cd == 2) {
                    if (rot.vx < 0x800) {
                        rot.vx += 0x800;
                    }
                }
                btlfx_rotateOffset(&rot, acc, (s32)&off);
                LoadAverageCol((u8 *)&e->tail->s.sphere.axisScaleStart, (u8 *)&e->tail->s.sphere.axisScaleEnd, 0x1000 - t, t, (u8 *)&col);
                off.vx = (off.vx * col.r) >> 7;
                off.vy = (off.vy * col.g) >> 7;
                off.vz = (off.vz * col.b) >> 7;
                base = e->origin;
                break;
            case BTLFX_SHAPE_CYLINDER:
                rot.vx = 0;
                rot.vy = btlfx_rand();
                rot.vz = 0;
                btlfx_rotateOffset(&rot, btlfx_randRange(e->radiusMin, e->radiusMax), (s32)&off);
                off.vy = btlfx_randRange(e->shape.span.min, e->shape.span.max);
                if (e->tail->s.cylinder.dirMode == 1) {
                    rot.vx = 0x400;
                    rot.vy = 0;
                } else if (e->tail->s.cylinder.dirMode == 2) {
                    rot.vx = -0x400;
                    rot.vy = 0;
                }
                base = e->origin;
                break;
            case BTLFX_SHAPE_AXIS:
                ((XY32 *)&ang)[0].w = ((XY32 *)&e->rot)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&e->rot)[1].w;
                d58 = btlfx_rand();
                acc = btlfx_randRange(e->shape.span.min, e->shape.span.max);
                r = btlfx_randRange(e->radiusMin, e->radiusMax);
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                btlfx_offsetPoint(&off, (s32)&off, acc, (XY32 *)&ang, r, d58);
                if (e->tail->s.cylinder.dirMode == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (e->tail->s.cylinder.dirMode == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    btlfx_forwardAngles(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->origin;
                break;
            case BTLFX_SHAPE_TUBE:
                btlfx_aimAngles((s16 *)e->origin, (s16 *)e->target, &ang.vx);
                d58 = btlfx_rand();
                acc = btlfx_randRange(e->shape.span.min, e->shape.span.max);
                r = btlfx_randRange(e->radiusMin, e->radiusMax);
                btlfx_lerpVec((s16 *)e->origin, (s16 *)e->target, acc, &off.vx);
                btlfx_offsetPoint(&off, (s32)&off, 0, (XY32 *)&ang, r, d58);
                if (e->tail->s.cylinder.dirMode == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (e->tail->s.cylinder.dirMode == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    btlfx_forwardAngles(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->origin;
                off.vx -= base->vx;
                off.vy -= base->vy;
                off.vz -= base->vz;
                break;
            case BTLFX_SHAPE_CONE: {
                s32 y4;
                ((XY32 *)&ang)[0].w = ((XY32 *)&e->rot)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&e->rot)[1].w;
                n = 1;
                if (e->shape.cone.length != 0) {
                    n = e->shape.cone.length;
                }
                acc = btlfx_rand() % n;
                r = btlfx_rand() & 0xFFF;
                d58 = (e->radiusMin * r) >> 12;
                y4 = (e->radiusMax * r) >> 12;
                r = btlfx_lerpRatio(d58, y4, acc, n);
                n = btlfx_rand();
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                btlfx_offsetPoint(&off, (s32)&off, acc, (XY32 *)&ang, r, n);
                if (e->tail->s.cone.dirMode == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (e->tail->s.cone.dirMode == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    *(s32 *)&tmp.vx = 0;
                    *(s32 *)&tmp.vz = 0;
                    btlfx_offsetPoint(&tmp, (s32)&tmp, 0, (XY32 *)&ang, d58, n);
                    *(s32 *)&rot.vx = 0;
                    *(s32 *)&rot.vz = 0;
                    btlfx_offsetPoint(&rot, (s32)&rot, e->shape.cone.length, (XY32 *)&ang, y4, n);
                    ((XY32 *)&ang)[0].w = ((XY32 *)&rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)&rot)[1].w;
                    btlfx_aimAngles(&tmp.vx, &ang.vx, &rot.vx);
                }
                base = e->origin;
                break;
            }
            case BTLFX_SHAPE_MODULE:
                off.pad = 0;
                if (e->tail->s.module.spawnFn(&off, &rot, frame, dur) != 0) {
                    anim->duration = 0;
                }
                if (off.pad != 0) {
                    base = &tmp;
                    *(s32 *)&tmp.vx = 0;
                    *(s32 *)&tmp.vz = 0;
                } else {
                    base = e->origin;
                }
                break;
            }
            p->pos.vx = (off.vx + base->vx) << 12;
            p->pos.vy = (off.vy + base->vy) << 12;
            p->pos.vz = (off.vz + base->vz) << 12;
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
                ((XY32 *)&rot)[0].w = ((XY32 *)&e->rot)[0].w;
                ((XY32 *)&rot)[1].w = ((XY32 *)&e->rot)[1].w;
                rot.vx += btlfx_randCentered(anim->rotStep.pad);
                rot.vy += btlfx_randCentered(anim->rotStep.pad);
                break;
            case BTLFX_LAUNCH_SHAPE:
                rot.vx += btlfx_randCentered(anim->rotStep.pad);
                rot.vy += btlfx_randCentered(anim->rotStep.pad);
                break;
            }
            if (el != NULL && el->alignRot != 0) {
                p->elemRot.s.x += rot.vx;
                p->elemRot.s.y += rot.vy;
            }
            switch (e->file->kind) {
            case BTLFX_MOTION_ORBIT: {
                EffEmit *mot = e->file;
                acc = e->cur.orbit.speed;
                btlfx_rotateOffset(&rot, acc + btlfx_randCentered(mot->m.orbit.speedSpread), (s32)&p->motA.vel);
                acc = mot->m.orbit.drag + btlfx_randCentered(mot->m.orbit.dragSpread);
                if (acc < 0) {
                    acc = 0;
                }
                p->motB.orbit.drag = acc;
                p->motB.orbit.dir.w = *(s32 *)&rot.vx;
                p->motB.orbit.turnX = mot->m.orbit.turnX + btlfx_randCentered(mot->m.orbit.turnXSpread);
                p->motB.orbit.turnY = mot->m.orbit.turnY + btlfx_randCentered(mot->m.orbit.turnYSpread);
                p->motB.orbit.radius = e->cur.orbit.radius + btlfx_randCentered(mot->m.orbit.radiusSpread);
                break;
            }
            case BTLFX_MOTION_SPIRAL: {
                EffEmit *mot = e->file;
                p->motB.spiral.axis.w = *(s32 *)&rot.vx;
                btlfx_lerpVec(&mot->m.spiral.radiusSpeedStart, &mot->m.spiral.radiusSpeedEnd, t, &tmp.vx);
                p->motA.spin.speed = tmp.vy + btlfx_randCentered(mot->m.spiral.spinSpeedSpread);
                p->motB.spiral.radiusSpeed = tmp.vx + btlfx_randCentered(mot->m.spiral.radiusSpeedSpread);
                p->motB.spiral.axialSpeed = e->cur.spiral.axialSpeed + btlfx_randCentered(mot->m.spiral.axialSpeedSpread);
                *(s32 *)&tmp.vx = 0;
                *(s32 *)&tmp.vz = 0;
                btlfx_toLocalPolar(&off, &tmp, &rot, (s32)&k, (s32)&r, &d58);
                p->motA.spin.angle = d58;
                p->motB.spiral.axial = k;
                p->motB.spiral.radius = r;
                p->pos.vx = base->vx << 12;
                p->pos.vy = base->vy << 12;
                p->pos.vz = base->vz << 12;
                p->motA.spin.fall = 0;
                break;
            }
            case BTLFX_MOTION_WOBBLE: {
                EffEmit *mot = e->file;
                *(s32 *)&p->motA.wobble.phaseX = *(s32 *)&rot.vx;
                p->motB.wobble.swingXSpeed = mot->m.wobble.swingXSpeed + btlfx_randCentered(mot->m.wobble.swingXSpread);
                p->motB.wobble.swingYSpeed = mot->m.wobble.swingYSpeed + btlfx_randCentered(mot->m.wobble.swingYSpread);
                p->motA.wobble.radius = e->cur.wobble.radius + btlfx_randCentered(mot->m.wobble.radiusSpread);
                p->motB.wobble.dirX = rot.vx + btlfx_randCentered(mot->m.wobble.dirXSpread);
                p->motB.wobble.dirY = rot.vy + btlfx_randCentered(mot->m.wobble.dirYSpread);
                p->motB.wobble.fall = 0;
                break;
            }
            case BTLFX_MOTION_HELIX: {
                EffEmit *mot = e->file;
                p->motB.helix.axis.w = *(s32 *)&rot.vx;
                btlfx_lerpVec(&mot->m.helix.radiusStart, &mot->m.helix.radiusEnd, t, &tmp.vx);
                p->motA.spin.speed = tmp.vy + btlfx_randCentered(mot->m.helix.spinSpeedSpread);
                p->motB.helix.radius = tmp.vx + btlfx_randCentered(mot->m.helix.radiusSpread);
                p->motB.helix.axialSpeed = e->cur.spiral.axialSpeed + btlfx_randCentered(mot->m.helix.axialSpeedSpread);
                p->motA.spin.angle = btlfx_rand();
                p->motB.helix.axial = 0;
                p->motA.spin.fall = 0;
                break;
            }
            case BTLFX_MOTION_BALLISTIC:
            case BTLFX_MOTION_HOMING: {
                EffEmit *mot = e->file;
                acc = e->cur.ballistic.speed;
                btlfx_rotateOffset(&rot, acc + btlfx_randCentered(mot->m.ballistic.speedSpread), (s32)&p->motA.vel);
                acc = mot->m.ballistic.drag + btlfx_randCentered(mot->m.ballistic.dragSpread);
                if (acc < 0) {
                    acc = 0;
                }
                p->motB.ballistic.drag = acc;
                break;
            }
            }
            if (el != NULL && ((u32)(el->kind - BTLFX_ELEMK_TRAIL_3D) < 2 || el->kind == BTLFX_ELEMK_TRAIL_SCREEN || el->kind2 == BTLFX_ELEMK_TRAIL_3D || el->kind2 == BTLFX_ELEMK_TRAIL_FLAT || el->kind2 == BTLFX_ELEMK_TRAIL_SCREEN)) {
                tmp.vx = p->pos.vx >> 12;
                tmp.vy = p->pos.vy >> 12;
                tmp.vz = p->pos.vz >> 12;
                btlfx_drawTrail((u8 *)el->tex, (s32)&tmp, NULL, -1, 0, 0, 0, (u8 *)btlfx_getParticleTrail(p, e), 0);
            }
        }
    }
}

void btlfx_runSubEmitter(Pool30 *pool, Pool30Entry *e, SVECTOR *pos, SVECTOR *rot) {
    SVECTOR off;
    SVECTOR ang;
    SVECTOR dir;
    s32 d40;
    s32 cnt;
    s32 k;
    Pool6CEntry *owner;
    EffSub *sub;
    EffElem *el;
    Pool30Entry *p;
    s32 n;
    s32 frame;
    s32 t;
    s32 r;
    s32 i;

    owner = e->owner;
    el = NULL;
    if (owner->state == BTLFX_EMIT_FREE) {
        return;
    }
    if (owner->state != BTLFX_EMIT_DONE) {
        sub = owner->sub - 1;
        frame = e->frame;
        t = (frame << 12) / e->frames;
        if ((!(e->spawnFlags & BTLFX_PART_SUB) || owner->frame < owner->tail->anim.duration) && frame >= sub->startFrame - 1) {
            r = btlfx_lerp(sub->rateStart, sub->rateEnd, t);
            cnt = r >> 8;
            k = (e->spawnFlags & BTLFX_PART_SUB_FRAC) + ((r >> 1) & 0x7F);
            if (k >= 0x80) {
                k -= 0x80;
                if (k == 0) {
                    k = 1;
                }
                cnt++;
            }
            e->spawnFlags = (u8)k | (e->spawnFlags & BTLFX_PART_SUB);
            n = cnt;
            for (i = 0; i < n; i++) {
                if (owner->live.s.y >= sub->maxParticles) {
                    return;
                }
                p = btlfx_allocParticle(pool);
                if (p == NULL) {
                    return;
                }
                owner->live.s.y++;
                cnt = btlfx_rand() & 0x7F;
                k = 0;
                r = 0;
                for (; k < sub->elemCount; k++) {
                    r += sub->weights[k];
                    if (cnt < r) {
                        break;
                    }
                }
                if (k >= sub->elemCount) {
                    cnt = 0xFF;
                } else {
                    cnt = sub->elems[k];
                }
                p->elem = cnt;
                p->elemFrame = 0;
                if (cnt == 0xFF) {
                    r = btlfx_lerp(((EffEmit *)owner->sub)->lifeStart, ((EffEmit *)owner->sub)->lifeEnd, t);
                    r += btlfx_randCentered(((EffEmit *)owner->sub)->lifeSpread);
                    if (r <= 0) {
                        r = 1;
                    }
                } else {
                    el = &owner->elems[cnt];
                    if (el->frameMode == 1) {
                        r = el->frameCount;
                    } else {
                        r = btlfx_lerp(((EffEmit *)owner->sub)->lifeStart, ((EffEmit *)owner->sub)->lifeEnd, t);
                    r += btlfx_randCentered(((EffEmit *)owner->sub)->lifeSpread);
                        if (r <= 0) {
                            r = 1;
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
                    p->spin = btlfx_randCentered(el->spinRange);
                    gte_stsv(&p->elemRot);
                }
                if (((EffEmit *)owner->sub)->emitsSub != 0) {
                    p->spawnFlags = BTLFX_PART_SUB | BTLFX_PART_SUB_FRAC;
                } else {
                    p->spawnFlags = BTLFX_PART_SUB;
                }
                p->slot = owner->subSpawnCount % 255 + 1;
                p->frames = r;
                p->frame = 0;
                p->owner = owner;
                p->timeStep = btlfx_randCentered(sub->stepSpread) + 0x40;
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
                p->pos.vx = (off.vx + pos->vx) << 12;
                p->pos.vy = (off.vy + pos->vy) << 12;
                p->pos.vz = (off.vz + pos->vz) << 12;
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
                    p->elemRot.s.x += ang.vx;
                    p->elemRot.s.y += ang.vy;
                }
                switch (((EffEmit *)owner->sub)->kind) {
                case BTLFX_MOTION_ORBIT:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    r = btlfx_lerp(mot->m.orbit.speedStart, mot->m.orbit.speedEnd, t);
                    btlfx_rotateOffset(&ang, r + btlfx_randCentered(mot->m.orbit.speedSpread), (s32)&p->motA.vel);
                    r = mot->m.orbit.drag + btlfx_randCentered(mot->m.orbit.dragSpread);
                    if (r < 0) {
                        r = 0;
                    }
                    p->motB.orbit.drag = r;
                    p->motB.orbit.dir.w = *(s32 *)&ang.vx;
                    p->motB.orbit.turnX = mot->m.orbit.turnX + btlfx_randCentered(mot->m.orbit.turnXSpread);
                    p->motB.orbit.turnY = mot->m.orbit.turnY + btlfx_randCentered(mot->m.orbit.turnYSpread);
                    p->motB.orbit.radius = btlfx_lerp(mot->m.orbit.radiusStart, mot->m.orbit.radiusEnd, t) + btlfx_randCentered(mot->m.orbit.radiusSpread);
                    break;
                    }
                case BTLFX_MOTION_SPIRAL:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    p->motB.spiral.axis.w = *(s32 *)&ang.vx;
                    btlfx_lerpVec(&mot->m.spiral.radiusSpeedStart, &mot->m.spiral.radiusSpeedEnd, t, (s16 *)&dir);
                    p->motA.spin.speed = dir.vy + btlfx_randCentered(mot->m.spiral.spinSpeedSpread);
                    p->motB.spiral.radiusSpeed = dir.vx + btlfx_randCentered(mot->m.spiral.radiusSpeedSpread);
                    p->motB.spiral.axialSpeed = btlfx_lerp(mot->m.spiral.axialSpeedStart, mot->m.spiral.axialSpeedEnd, t) + btlfx_randCentered(mot->m.spiral.axialSpeedSpread);
                    *(s32 *)&dir.vx = 0;
                    *(s32 *)&dir.vz = 0;
                    btlfx_toLocalPolar(&off, &dir, &ang, (s32)&d40, (s32)&cnt, &k);
                    p->motA.spin.angle = k;
                    p->motB.spiral.axial = d40;
                    p->motB.spiral.radius = cnt;
                    p->pos.vx = e->pos.vx;
                p->pos.vy = e->pos.vy;
                p->pos.vz = e->pos.vz;
                    p->motA.spin.fall = 0;
                    break;
                    }
                case BTLFX_MOTION_WOBBLE:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    *(s32 *)&p->motA.wobble.phaseX = *(s32 *)&ang.vx;
                    p->motB.wobble.swingXSpeed = mot->m.wobble.swingXSpeed + btlfx_randCentered(mot->m.wobble.swingXSpread);
                    p->motB.wobble.swingYSpeed = mot->m.wobble.swingYSpeed + btlfx_randCentered(mot->m.wobble.swingYSpread);
                    p->motA.wobble.radius = btlfx_lerp(mot->m.wobble.radiusStart, mot->m.wobble.radiusEnd, t) + btlfx_randCentered(mot->m.wobble.radiusSpread);
                    p->motB.wobble.dirX = ang.vx + btlfx_randCentered(mot->m.wobble.dirXSpread);
                    p->motB.wobble.dirY = ang.vy + btlfx_randCentered(mot->m.wobble.dirYSpread);
                    p->motB.wobble.fall = 0;
                    break;
                    }
                case BTLFX_MOTION_HELIX:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    p->motB.helix.axis.w = *(s32 *)&ang.vx;
                    btlfx_lerpVec(&mot->m.helix.radiusStart, &mot->m.helix.radiusEnd, t, (s16 *)&dir);
                    p->motA.spin.speed = dir.vy + btlfx_randCentered(mot->m.helix.spinSpeedSpread);
                    p->motB.helix.radius = dir.vx + btlfx_randCentered(mot->m.helix.radiusSpread);
                    p->motB.helix.axialSpeed = btlfx_lerp(mot->m.helix.axialSpeedStart, mot->m.helix.axialSpeedEnd, t) + btlfx_randCentered(mot->m.helix.axialSpeedSpread);
                    p->motA.spin.angle = btlfx_rand();
                    p->motB.helix.axial = 0;
                    p->motA.spin.fall = 0;
                    break;
                    }
                case BTLFX_MOTION_BALLISTIC:
                case BTLFX_MOTION_HOMING:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    r = btlfx_lerp(mot->m.ballistic.speedStart, mot->m.ballistic.speedEnd, t);
                    btlfx_rotateOffset(&ang, r + btlfx_randCentered(mot->m.ballistic.speedSpread), (s32)&p->motA.vel);
                    r = mot->m.ballistic.drag + btlfx_randCentered(mot->m.ballistic.dragSpread);
                    if (r < 0) {
                        r = 0;
                    }
                    p->motB.ballistic.drag = r;
                    break;
                    }
                }
                if (el != NULL && ((u32)(el->kind - BTLFX_ELEMK_TRAIL_3D) < 2 || el->kind == BTLFX_ELEMK_TRAIL_SCREEN || el->kind2 == BTLFX_ELEMK_TRAIL_3D || el->kind2 == BTLFX_ELEMK_TRAIL_FLAT || el->kind2 == BTLFX_ELEMK_TRAIL_SCREEN)) {
                    dir.vx = p->pos.vx >> 12;
                    dir.vy = p->pos.vy >> 12;
                    dir.vz = p->pos.vz >> 12;
                    btlfx_drawTrail((u8 *)el->tex, (s32)&dir, NULL, -1, 0, 0, 0, (u8 *)btlfx_getParticleTrail(p, owner), 0);
                }
            }
        }
    }
}

void btlfx_heapInit(Heap *arg0, s32 arg1) {
    s32 size = btlfx_align4(arg1) - 0xC;
    arg0->cursor = arg0->data;
    arg0->free = size;
    arg0->size = size;
}

s32 btlfx_heapAlloc(Heap *h, s32 size) {
    s32 sz;
    u8 *p;
    s32 *q;

    sz = btlfx_align4(size);
    if (h->free < sz) {
        q = (s32 *)h->data;
        h->cursor = (u8 *)q;
        h->free = h->size;
    }
    p = h->cursor;
    h->free -= sz;
    h->cursor = p + sz;
    q = (s32 *)p;
    for (size = 0; size < sz >> 2; size++) {
        *q++ = 0;
    }
    return (s32)p;
}

s32 btlfx_heapWrapIfFull(Heap *arg0, s32 arg1) {
    s32 size = btlfx_align4(arg1);
    s32 ret = 1;
    if (arg0->free < size) {
        arg0->cursor = arg0->data;
        arg0->free = arg0->size;
    } else {
        ret = 0;
    }
    return ret;
}

void btlfx_heapFree(void *arg0) {
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

Pool6CEntry *btlfx_startEmitter(s32 arg0, s32 arg1) {
    return btlfx_spawnEmitter(&g_btlfxParticles, &g_btlfxEmitters, (u8 *)arg0, arg1);
}

void btlfx_moveEmitterTo(s32 arg0, s32 arg1) {
    if (arg0 != 0) {
        btlfx_moveEmitter(&g_btlfxParticles, arg0, arg1);
    }
}

void btlfx_stopEmitter(Pool6CEntry *arg0) {
    if (arg0 != NULL) {
        btlfx_killEmitter(&g_btlfxParticles, arg0);
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
    RECT r;
    DR_MOVE *p;
    s32 i;
    r.x = g_btlfxEnv->dispEnv->disp.x;
    r.y = g_btlfxEnv->dispEnv->disp.y;
    r.w = 0x20;
    r.h = 0xDA;
    i = 0;
    p = (DR_MOVE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(p + 10);
    do {
        SetDrawMove(p, &r, g_btlfxEnv->drawEnv->clip.x + i, g_btlfxEnv->drawEnv->clip.y);
        AddPrim(g_btlfxEnv->ot, p++);
        i += 0x20;
        r.x += 0x20;
    } while (i < 0x140);
    if (g_btlfxSoundPaused == 0) {
        g_btlfxSoundPaused = 1;
        akao_pause(2);
    }
}
