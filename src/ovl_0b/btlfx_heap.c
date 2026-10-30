#include "ovl_0b.h"

void btlfx_initParticles(s32 arg0, Pool30Entry *arg1, Pool30 *arg2) {
    s32 i = 0;
    arg2->capacity = arg0 / (s32)sizeof(Pool30Entry);
    arg2->entries = arg1;
    arg2->live = 0;
    arg2->maxUsed = 0;
    for (; i < arg2->capacity;) {
        i++;
        arg1->unk1F = 0;
        arg1++;
    }
}

s32 func_800E0134(Pool30Entry *arg0, Pool6CEntry *arg1) {
    s32 p = arg1->unk14;
    s32 t;
    if (p == 0) {
        return 0;
    }
    t = arg0->unk1F;
    t = (t - 1) * arg1->unkC;
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
    EffMotion *m;
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
    D_801014B0 = g_btlfxEnv->otZ;
    pool->live = 0;
    for (; i <= pool->maxUsed; i++, e++) {
        if (e->unk1F == 0) {
            continue;
        }
        frame = e->frame;
        frames = e->frames;
        owner = e->owner;
        if (frame >= frames) {
            e->unk1F = 0;
            if (e->unk12 & 0x80) {
                owner->unk34.s.y--;
            } else {
                owner->unk34.s.x--;
            }
            continue;
        }
        last = i;
        pool->live++;
        t = (frame << 12) / frames;
        if (e->unk12 & 0x80) {
            m = (EffMotion *)owner->sub;
            sub = owner->sub - 1;
            mode = sub->unk24;
            rs = owner->unk56;
            rp = owner->unk50;
            if (mode != 0) {
                curve = sub->unk26;
                range = owner->unk58.s.x;
                if (mode < 7) {
                    amp = owner->unk58.s.y;
                } else {
                    amp = sub->unk28[1];
                }
            }
        } else {
            m = (EffMotion *)owner->file;
            mode = owner->tail->unk4.unk1C;
            rs = owner->unk4A;
            rp = owner->unk44;
            if (mode != 0) {
                curve = owner->tail->unk4.unk1E;
                range = owner->unk4C.s.x;
                if (mode < 7) {
                    amp = owner->unk4C.s.y;
                } else {
                    amp = owner->tail->unk4.unk20[1];
                }
            }
            if (owner->sub != NULL) {
                lvl = (owner->sub - 1)->unkC;
                if (lvl >= 4) {
                    rel.vx = e->pos.vx >> 12;
                    rel.vy = e->pos.vy >> 12;
                    rel.vz = e->pos.vz >> 12;
                }
            }
        }
        switch (m->unk0) {
        case 0:
            btlfx_stepParticle(e, rs, rp, &pos);
            if (m->unk18 != 0) {
                e->unkC += btlfx_randCentered(m->unk1A);
                e->unkE += btlfx_randCentered(m->unk1C);
                e->unk10 += btlfx_randCentered(m->unk1E);
            }
            if (m->unk12 != 0) {
                if (m->unk12 == 1) {
                    if (owner->unk5C.pad - m->unk14 < pos.vy && e->unkE > 0) {
                        e->unkE = -e->unkE;
                        e->unkE = (e->unkE * m->unk16) >> 8;
                    }
                } else if (owner->unk5C.pad - m->unk14 < pos.vy) {
                    if (e->unkE > 0) {
                        s32 x = e->unkE * (owner->unk5C.pad - pos.vy);
                        if (m->unk14 != 0) {
                            x /= m->unk14;
                        }
                        e->unkE = x;
                    }
                    if (owner->unk5C.pad - (m->unk14 >> 1) < pos.vy) {
                        e->unkE = (s16)(e->unkE - 0x40) - ((owner->unk4A * e->timeStep) >> 6);
                    }
                    if (pos.vy > owner->unk5C.pad) {
                        e->unkE = 0;
                    }
                }
            }
            break;
        case 1:
            btlfx_stepParticleAltDrag(e, rs, rp, &pos);
            e->unk24.s.x += e->unk20;
            e->unk24.s.y += e->unk22;
            v.vx = e->unk24.s.x;
            v.vy = e->unk24.s.y;
            switch (m->unk14) {
            case 0:
                rs = (math_sin(btlfx_lerp(m->unk1C, m->unk1E, t)) * e->unk28.s.x) >> 12;
                break;
            case 1:
                rs = (e->unk28.s.x * t) >> 12;
                break;
            case 2:
                rs = e->unk28.s.x - ((e->unk28.s.x * t) >> 12);
                break;
            }
            btlfx_pointAhead((s16 *)&pos, (s16 *)&v, rs, (s16 *)&pos);
            break;
        case 2:
            btlfx_stepParticleNoDrag(e, rs, rp, &pos);
            e->unkC += e->unkE;
            e->unk20 += e->unk22;
            e->unk24.s.x += e->unk24.s.y;
            func_800D044C(&pos, (s32)&pos, e->unk20 >> 2, &e->unk28, e->unk24.s.x >> 2, e->unkC);
            break;
        case 3:
            btlfx_stepParticleNoDragAlt(e, rs, rp, &pos);
            e->unkC += e->unk20;
            e->unkE += e->unk22;
            v.vx = e->unk24.s.y + math_sin(e->unkC);
            v.vy = e->unk28.s.x + math_sin(e->unkE);
            switch (m->unk14) {
            case 0:
                rs = (math_sin(btlfx_lerp(m->unk1C, m->unk1E, t)) * e->unk10) >> 12;
                break;
            case 1:
                rs = (e->unk10 * t) >> 12;
                break;
            case 2:
                rs = e->unk10 - ((e->unk10 * t) >> 12);
                break;
            }
            btlfx_pointAhead((s16 *)&pos, (s16 *)&v, rs, (s16 *)&pos);
            break;
        case 4:
            btlfx_stepParticleNoDrag(e, rs, rp, &pos);
            e->unk24.w += e->unk22;
            e->unkC += e->unkE;
            switch (m->unkE) {
            case 0:
                rs = (math_sin(btlfx_lerp(m->unk1C, m->unk1E, t)) * e->unk20) >> 12;
                break;
            case 1:
                rs = (e->unk20 * t) >> 12;
                break;
            case 2:
                rs = e->unk20 - ((e->unk20 * t) >> 12);
                break;
            }
            func_800D044C(&pos, (s32)&pos, e->unk24.w >> 6, &e->unk28, rs, e->unkC);
            break;
        case 5:
            btlfx_stepParticle(e, rs, rp, &pos);
            if (m->unk18 != 0) {
                e->unkC += btlfx_randCentered(m->unk1A);
                e->unkE += btlfx_randCentered(m->unk1C);
                e->unk10 += btlfx_randCentered(m->unk1E);
            }
            rs = btlfx_lerp(m->unk12, m->unk14, t);
            btlfx_aimAngles((s16 *)&pos, (s16 *)owner->unk2C, (s16 *)&v);
            v.vx += m->unk20;
            v.vy += m->unk22;
            btlfx_pointAhead(&e->unkC, (s16 *)&v, rs, &e->unkC);
            break;
        case 0xFF:
            if (m->unk8(e, owner, &pos, func_800E0134(e, owner), frame, frames) != 0) {
                e->unk1F = 0;
                if (e->unk12 & 0x80) {
                    owner->unk34.s.y--;
                } else {
                    owner->unk34.s.x--;
                }
            }
            break;
        }
        ofs = 0xFFFF;
        if (mode != 0) {
            switch (mode) {
            case 1:
                /* padded: a zero word follows the first jump table */
                __asm__(".section .rodata\n\t.word 0\n\t.section .text");
                cnt = btlfx_distance(&pos, owner->unk64);
                if (cnt <= range) {
                    d.vx = pos.vx - owner->unk64->vx;
                    d.vy = pos.vy - owner->unk64->vy;
                    d.vz = pos.vz - owner->unk64->vz;
                    VectorNormalS(&d, &v);
                }
                break;
            case 2:
                cnt = btlfx_distance(&pos, owner->unk64);
                if (cnt <= range) {
                    d.vx = owner->unk64->vx - pos.vx;
                    d.vy = owner->unk64->vy - pos.vy;
                    d.vz = owner->unk64->vz - pos.vz;
                    VectorNormalS(&d, &v);
                }
                break;
            case 3:
            case 4:
                *(s32 *)&v.vx = *(s32 *)&pos.vx;
                v.vy = owner->unk64->vy;
                *(s32 *)&v.vz = *(s32 *)&pos.vz;
                cnt = btlfx_distance(&v, owner->unk64);
                ((s32 *)&v)[0] = 0;
                v.vy = -0x1000;
                break;
            case 5:
                func_800D0674(&pos, owner->unk64, owner->unk68, (s32)&alpha, (s32)&cnt, &scale);
                if (cnt <= range) {
                    btlfx_rotateOffset(owner->unk68, 0x1000, (s32)&v);
                }
                break;
            case 6:
                func_800D0674(&pos, owner->unk64, owner->unk68, (s32)&alpha, (s32)&cnt, &scale);
                if (cnt <= range) {
                    ((s32 *)&v)[1] = 0;
                    ((s32 *)&v)[0] = 0;
                    func_800D044C(&v, (s32)&v, 0, (XY32 *)owner->unk68, 0x1000, scale);
                }
                break;
            case 7:
                cnt = btlfx_distance(&pos, owner->unk64);
                break;
            case 8:
                *(s32 *)&v.vx = *(s32 *)&pos.vx;
                v.vy = owner->unk64->vy;
                *(s32 *)&v.vz = *(s32 *)&pos.vz;
                cnt = btlfx_distance(&v, owner->unk64);
                break;
            case 9:
                func_800D0674(&pos, owner->unk64, owner->unk68, (s32)&alpha, (s32)&cnt, &scale);
                break;
            }
            if (cnt <= range) {
                scale = (cnt << 10) / range;
                d.vx = v.vx;
                d.vy = v.vy;
                d.vz = v.vz;
                switch (curve) {
                case 0:
                    break;
                case 1:
                    amp = (math_cos(scale) * amp) >> 12;
                    break;
                case 2:
                    amp = (math_sin(scale) * amp) >> 12;
                    break;
                case 3:
                    amp = ((0x1000 - math_sin(scale)) * amp) >> 12;
                    break;
                case 4:
                    amp = ((0x1000 - math_cos(scale)) * amp) >> 12;
                    break;
                case 5:
                    amp = (math_sin(scale * 2) * amp) >> 12;
                    break;
                }
                if (mode < 7) {
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
            } else if (mode >= 7) {
                ofs = 0;
            }
        }
        g_btlfxEnv->otZ = owner->unk22;
        if (e->elem != 0xFF) {
            el = &owner->elems[e->elem];
            fl = el->unk3;
            vp = NULL;
            if (!(fl & 1)) {
                vp = &v;
                *(s32 *)&vp->vx = e->unk14.w;
                vp->vz = e->unk18;
                vp->vx += el->unkA;
                vp->vy += el->unkC;
                vp->vz += el->unkE;
                v.pad = ((fl >> 2) ^ 1) & 1;
                rs = e->unk13 * 4;
                if (fl & 0x10) {
                    vp->vx += rs;
                }
                if (fl & 0x20) {
                    vp->vy += rs;
                }
                if (fl & 0x40) {
                    vp->vz += rs;
                }
                e->unk14.w = *(s32 *)&vp->vx;
                e->unk18 = vp->vz;
            }
            if (fl & 8) {
                alpha = owner->unk20;
            } else {
                rs = owner->unk20;
                switch (el->unk6) {
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
            if (fl & 2) {
                scale = owner->unk1C;
            } else {
                rs = owner->unk1C;
                scale = (btlfx_lerp(el->unk1A << 7, el->unk1B << 7, t) * rs) >> 12;
            }
            rs = el->unk2;
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
                case 9:
                    if (vp == NULL) {
                        vp = &v;
                        ((s32 *)&v)[0] = 0;
                        ((s32 *)&v)[1] = 0;
                    }
                    el->unk24(e, owner, &pos, vp, &scale, &alpha, &cnt, frame, frames);
                    break;
                case 0:
                    btlfx_drawParticles((u8 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha, 0, 0);
                    break;
                case 1:
                    btlfx_drawSprite((u8 *)mode, &pos, e->elemFrame, cnt, alpha);
                    break;
                case 2:
                    btlfx_drawAnimQuads((u8 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha);
                    break;
                case 3:
                    btlfx_drawSprites((u8 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha);
                    break;
                case 4:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, (s32 *)owner->origin, e->elemFrame, scale, alpha, cnt, (u8 *)func_800E0134(e, owner), 1);
                    break;
                case 5:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, NULL, e->elemFrame, scale, alpha, cnt, (u8 *)func_800E0134(e, owner), 2);
                    break;
                case 6:
                    btlfx_drawRings((u8 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha);
                    break;
                case 7:
                    btlfx_drawFan((Unk800D2938 *)mode, &pos, vp, scale, e->elemFrame, cnt, alpha);
                    break;
                case 8:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, NULL, e->elemFrame, scale, alpha, cnt, (u8 *)func_800E0134(e, owner), 0);
                    break;
                }
                if (el->kind2 == 0xFF) {
                    break;
                }
            }
            if (el->unk4 == 1) {
                e->elemFrame++;
            } else if (el->unk4 == 2) {
                rs = e->elemFrame;
                rs++;
                if (rs >= el->unk8) {
                    rs = 0;
                }
                e->elemFrame = rs;
            }
        }
        if (owner->sub != NULL && (e->unk12 & 0x7F)) {
            if (lvl >= 4) {
                btlfx_aimAngles((s16 *)&rel, (s16 *)&pos, (s16 *)&v);
            }
            func_800E2F84(pool, e, &pos, &v);
        }
        frame++;
        e->frame = frame;
    }
    pool->maxUsed = last;
    g_btlfxEnv->otZ = D_801014B0;
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
        e->state = 0;
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
        if (e->state == 0) {
            if (arg1->maxUsed < i) {
                arg1->maxUsed = i;
            }
            p = arg2;
            for (k = 0; k < 0x6C; k += 4) {
                *(s32 *)((u8 *)e + k) = 0;
            }
            e->state = 1;
            e->unkE = 0;
            e->unk10 = 0;
            e->unk32 = 0;
            e->unk34.s.x = 0;
            e->unk34.s.y = 0;
            e->unk14 = 0;
            e->unk3A = 0xFF;
            e->unk22 = 0;
            e->file = (EffFileHdr *)p;
            if (*p != 0xFF) {
                p += 0x28;
            } else {
                p += 0x10;
            }
            if (((u8 *)e->file)[1] != 0) {
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
                if (el->kind != 9) {
                    if (el->tex != -1 && !(el->tex & 0x80000000)) {
                        el->tex = (s32)arg2 + el->tex;
                    }
                }
                if (el->kind2 != 0xFF) {
                    if (el->tex2 != -1 && !(el->tex2 & 0x80000000)) {
                        el->tex2 = (s32)arg2 + el->tex2;
                    }
                }
            }
            /* unk24 (tail), unk28, unk2C written through one pointer */
            q = (s32 *)&e->tail;
            q[0] = (s32)p;
            v = *(s32 *)p;
            k = (u32)p;
            if (v == -1) {
                *(s32 *)&((EffTail *)k)->unk3C = 0;
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
        if (e->unk1F != 0 && e->owner == m) {
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
            if (p->unk1F != 0 && p->owner == arg1) {
                p->frame = p->frames;
            }
            i++;
            p++;
        } while (i < arg0->capacity);
    }
    arg1->state = -1;
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
    EffFileHdr *f;
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
        if (e->state == 0) {
            continue;
        }
        if (e->state == -1) {
            e->state = 0;
            continue;
        }
        p->live++;
        def = e->tail;
        frame = e->frame;
        an = &def->unk4;
        last = i;
        y = an->unk2;
        t = (frame << 12) / y;
        e->unk32 = 0;
        if (frame >= y) {
            if (e->unk34.w == 0) {
                e->state = -1;
            }
            continue;
        }
        switch (def->unk4.unk0) {
        case 0:
            v = btlfx_lerp(an->unk4, an->unk6, t);
            break;
        case 1:
            v = (math_sin(t >> 1) * an->unk4) >> 12;
            break;
        case 2:
            def = e->tail;
            if (def->unk0 != -1) {
                tbl = def->unk50;
            } else {
                tbl = def->unk40;
            }
            v = tbl[(frame << 4) / y];
            break;
        case 3:
            v = 0x100;
            break;
        }
        x = v >> 8;
        y = e->unk3A;
        y += v & 0xFF;
        if (y >= 0x100) {
            y -= 0x100;
            x++;
        }
        e->unk3A = y;
        e->unk32 = x;
        if (frame == 0) {
            ((s32 *)&e->unk5C)[0] = ((s32 *)&an->unk28)[0];
            ((s32 *)&e->unk5C)[1] = ((s32 *)&an->unk28)[1];
        } else {
            e->unk5C.vx += an->unk30.vx;
            e->unk5C.vy += an->unk30.vy;
            e->unk5C.vz += an->unk30.vz;
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
        a.h[0] = hdr->unk6;
        a.h[1] = hdr->unkA;
        a.h[2] = an->unk12;
        b[0] = hdr->unk8;
        b[1] = hdr->unkC;
        b[2] = an->unk1A;
        btlfx_lerpVec(a.h, b, t, a.h);
        gte_lddp(0x1000 - t);
        gte_ldsv(an->unkC);
        gte_gpf12();
        e->unk1C = a.h[0];
        e->unk20 = a.h[1];
        e->unk4A = a.h[2];
        gte_lddp(t);
        gte_ldsv(an->unk14);
        gte_gpl12();
        gte_stsv(e->unk44);
        if (an->unk1C != 0) {
            btlfx_lerpVec(an->unk20, an->unk24, t, a.h);
            e->unk4C.w = a.w;
        }
        if (e->sub != NULL) {
            sub = e->sub - 1;
            btlfx_lerpVec(sub->unk14, sub->unk1C, t, e->unk50);
            e->unk56 = btlfx_lerp(sub->unk1A, sub->unk22, t);
            if (sub->unk24 != 0) {
                btlfx_lerpVec(sub->unk28, sub->unk2C, t, a.h);
                e->unk58.w = a.w;
            }
        }
        d2 = e->tail;
        switch (d2->unk0) {
        case 0:
            a.h[0] = d2->unk3C;
            a.h[1] = d2->unk40[0];
            b[0] = d2->unk3E;
            b[1] = d2->unk40[1];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->unk3C = a.h[0];
            e->unk3E = a.h[1];
            break;
        case 1:
        case 2:
        case 3:
            a.h[0] = d2->unk3C;
            a.h[1] = d2->unk3E;
            b[0] = d2->unk40[0];
            b[1] = d2->unk40[1];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->unk3C = a.h[0];
            e->unk3E = a.h[1];
            a.h[0] = d2->unk40[2];
            a.h[1] = d2->unk40[3];
            b[0] = d2->unk40[4];
            b[1] = d2->unk40[5];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->unk40 = a.h[0];
            e->unk42 = a.h[1];
            break;
        case 4:
            a.h[0] = d2->unk3C;
            a.h[1] = d2->unk40[0];
            a.h[2] = d2->unk40[2];
            b[0] = d2->unk3E;
            b[1] = d2->unk40[1];
            b[2] = d2->unk40[3];
            btlfx_lerpVec(a.h, b, t, a.h);
            e->unk3C = a.h[0];
            e->unk3E = a.h[1];
            e->unk40 = a.h[2];
            break;
        }
        switch (e->file->unk0) {
        case 0:
        case 5:
            e->unk8 = btlfx_lerp(e->file->unk8, e->file->unkA, t);
            break;
        case 1:
            f = e->file;
            e->unk8 = btlfx_lerp(f->unk18, f->unk1A, t);
            e->unkA = btlfx_lerp(f->unk20, f->unk22, t);
            break;
        case 2:
        case 4:
            e->unk8 = btlfx_lerp(e->file->unk8, e->file->unkA, t);
            break;
        case 3:
            e->unk8 = btlfx_lerp(e->file->unk18, e->file->unk1A, t);
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
        if (p->unk1F == 0) {
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
        if (e->state == 0) {
            continue;
        }
        if (e->state == -1) {
            continue;
        }
        for (j = 0; j < e->unk32; j++) {
            if (e->unk34.s.x >= e->tail->unk4.unk8) {
                break;
            }
            p = btlfx_allocParticle(pool);
            if (p == NULL) {
                break;
            }
            frame = e->frame;
            anim = &e->tail->unk4;
            e->unk34.s.x++;
            dur = anim->unk2;
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
                acc = btlfx_lerp(((EffEmit *)e->file)->unk2, ((EffEmit *)e->file)->unk4, t);
                acc += btlfx_randCentered(((EffEmit *)e->file)->unk6);
                if (acc <= 0) {
                    acc = 1;
                }
            } else {
                el = &e->elems[k];
                if (el->unk4 == 1) {
                    acc = el->unk8;
                } else {
                    acc = btlfx_lerp(((EffEmit *)e->file)->unk2, ((EffEmit *)e->file)->unk4, t);
                    acc += btlfx_randCentered(((EffEmit *)e->file)->unk6);
                    if (acc <= 0) {
                        acc = 1;
                    }
                }
                off.vx = (el->unk14[0] * 8) & 0xFFF;
                off.vy = (el->unk14[1] * 8) & 0xFFF;
                off.vz = (el->unk14[2] * 8) & 0xFFF;
                gte_lddp(frame);
                gte_ldsv(&off);
                gte_gpf0();
                ang.vx = (el->unk11[0] * 16 + btlfx_randCentered(el->unk17[0] * 16)) & 0xFFF;
                ang.vy = (el->unk11[1] * 16 + btlfx_randCentered(el->unk17[1] * 16)) & 0xFFF;
                ang.vz = (el->unk11[2] * 16 + btlfx_randCentered(el->unk17[2] * 16)) & 0xFFF;
                gte_lddp(1);
                gte_ldsv(&ang);
                gte_gpl0();
                p->unk13 = btlfx_randCentered(el->unk10);
                gte_stsv(&p->unk14);
            }
            p->unk1F = e->unkE % 255 + 1;
            p->frames = acc;
            p->frame = 0;
            p->owner = e;
            p->timeStep = btlfx_randCentered(anim->unkA) + 0x40;
            if (((EffEmit *)e->file)->unk1 != 0) {
                p->unk12 = 0x7F;
            } else {
                p->unk12 = 0;
            }
            e->unkE++;
            if (e->unk14 != 0) {
                e->unkE %= e->unk12;
            }
            switch (e->tail->unk0) {
            case 0:
                btlfx_randRotation(&rot);
                acc = btlfx_randRange(e->unk3C, e->unk3E);
                if (((EffTailSpawn *)e->tail)->unk44.cd == 1) {
                    if (rot.vx > 0x800) {
                        rot.vx -= 0x800;
                    }
                } else if (((EffTailSpawn *)e->tail)->unk44.cd == 2) {
                    if (rot.vx < 0x800) {
                        rot.vx += 0x800;
                    }
                }
                btlfx_rotateOffset(&rot, acc, (s32)&off);
                LoadAverageCol((u8 *)&((EffTailSpawn *)e->tail)->unk44, (u8 *)&((EffTailSpawn *)e->tail)->unk48.c, 0x1000 - t, t, (u8 *)&col);
                off.vx = (off.vx * col.r) >> 7;
                off.vy = (off.vy * col.g) >> 7;
                off.vz = (off.vz * col.b) >> 7;
                base = e->origin;
                break;
            case 1:
                rot.vx = 0;
                rot.vy = btlfx_rand();
                rot.vz = 0;
                btlfx_rotateOffset(&rot, btlfx_randRange(e->unk3C, e->unk3E), (s32)&off);
                off.vy = btlfx_randRange(e->unk40, e->unk42);
                if (((EffTailSpawn *)e->tail)->unk4C == 1) {
                    rot.vx = 0x400;
                    rot.vy = 0;
                } else if (((EffTailSpawn *)e->tail)->unk4C == 2) {
                    rot.vx = -0x400;
                    rot.vy = 0;
                }
                base = e->origin;
                break;
            case 2:
                ((XY32 *)&ang)[0].w = ((XY32 *)&e->unk5C)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&e->unk5C)[1].w;
                d58 = btlfx_rand();
                acc = btlfx_randRange(e->unk40, e->unk42);
                r = btlfx_randRange(e->unk3C, e->unk3E);
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                func_800D044C(&off, (s32)&off, acc, (XY32 *)&ang, r, d58);
                if (((EffTailSpawn *)e->tail)->unk4C == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->tail)->unk4C == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    func_800D0580(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->origin;
                break;
            case 3:
                btlfx_aimAngles((s16 *)e->origin, (s16 *)e->unk2C, &ang.vx);
                d58 = btlfx_rand();
                acc = btlfx_randRange(e->unk40, e->unk42);
                r = btlfx_randRange(e->unk3C, e->unk3E);
                btlfx_lerpVec((s16 *)e->origin, (s16 *)e->unk2C, acc, &off.vx);
                func_800D044C(&off, (s32)&off, 0, (XY32 *)&ang, r, d58);
                if (((EffTailSpawn *)e->tail)->unk4C == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->tail)->unk4C == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    func_800D0580(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->origin;
                off.vx -= base->vx;
                off.vy -= base->vy;
                off.vz -= base->vz;
                break;
            case 4: {
                s32 y4;
                ((XY32 *)&ang)[0].w = ((XY32 *)&e->unk5C)[0].w;
                ((XY32 *)&ang)[1].w = ((XY32 *)&e->unk5C)[1].w;
                n = 1;
                if (e->unk40 != 0) {
                    n = e->unk40;
                }
                acc = btlfx_rand() % n;
                r = btlfx_rand() & 0xFFF;
                d58 = (e->unk3C * r) >> 12;
                y4 = (e->unk3E * r) >> 12;
                r = btlfx_lerpRatio(d58, y4, acc, n);
                n = btlfx_rand();
                *(s32 *)&off.vx = 0;
                *(s32 *)&off.vz = 0;
                func_800D044C(&off, (s32)&off, acc, (XY32 *)&ang, r, n);
                if (((EffTailSpawn *)e->tail)->unk48.h == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->tail)->unk48.h == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    *(s32 *)&tmp.vx = 0;
                    *(s32 *)&tmp.vz = 0;
                    func_800D044C(&tmp, (s32)&tmp, 0, (XY32 *)&ang, d58, n);
                    *(s32 *)&rot.vx = 0;
                    *(s32 *)&rot.vz = 0;
                    func_800D044C(&rot, (s32)&rot, e->unk40, (XY32 *)&ang, y4, n);
                    ((XY32 *)&ang)[0].w = ((XY32 *)&rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)&rot)[1].w;
                    btlfx_aimAngles(&tmp.vx, &ang.vx, &rot.vx);
                }
                base = e->origin;
                break;
            }
            case -1:
                off.pad = 0;
                if (((EffTailSpawn *)e->tail)->unk3C(&off, &rot, frame, dur) != 0) {
                    anim->unk2 = 0;
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
            switch (anim->unk28.pad) {
            case 1:
                rot.vx += btlfx_randCentered(anim->unk30.pad);
                rot.vy += btlfx_randCentered(anim->unk30.pad);
                rot.vx = (rot.vx + 0x800) & 0xFFF;
                break;
            case 2:
                btlfx_randRotation(&rot);
                break;
            case 3:
                ((XY32 *)&rot)[0].w = ((XY32 *)&e->unk5C)[0].w;
                ((XY32 *)&rot)[1].w = ((XY32 *)&e->unk5C)[1].w;
                rot.vx += btlfx_randCentered(anim->unk30.pad);
                rot.vy += btlfx_randCentered(anim->unk30.pad);
                break;
            case 0:
                rot.vx += btlfx_randCentered(anim->unk30.pad);
                rot.vy += btlfx_randCentered(anim->unk30.pad);
                break;
            }
            if (el != NULL && el->unk7 != 0) {
                p->unk14.s.x += rot.vx;
                p->unk14.s.y += rot.vy;
            }
            switch (((EffEmit *)e->file)->unk0) {
            case 1: {
                EffEmit *mot = (EffEmit *)e->file;
                acc = e->unkA;
                btlfx_rotateOffset(&rot, acc + btlfx_randCentered(mot->unk24), (s32)&p->unkC);
                acc = mot->unk10 + btlfx_randCentered(mot->unk12);
                if (acc < 0) {
                    acc = 0;
                }
                p->unk28.s.y = acc;
                p->unk24.w = *(s32 *)&rot.vx;
                p->unk20 = mot->unk8 + btlfx_randCentered(mot->unkC);
                p->unk22 = mot->unkA + btlfx_randCentered(mot->unkE);
                p->unk28.s.x = e->unk8 + btlfx_randCentered(mot->unk16);
                break;
            }
            case 2: {
                EffEmit *mot = (EffEmit *)e->file;
                p->unk28.w = *(s32 *)&rot.vx;
                btlfx_lerpVec(&mot->unk10, &mot->unk14, t, &tmp.vx);
                p->unkE = tmp.vy + btlfx_randCentered(mot->unkE);
                p->unk24.s.y = tmp.vx + btlfx_randCentered(mot->unk18);
                p->unk22 = e->unk8 + btlfx_randCentered(mot->unkC);
                *(s32 *)&tmp.vx = 0;
                *(s32 *)&tmp.vz = 0;
                func_800D0674(&off, &tmp, &rot, (s32)&k, (s32)&r, &d58);
                p->unkC = d58;
                p->unk20 = k;
                p->unk24.s.x = r;
                p->pos.vx = base->vx << 12;
                p->pos.vy = base->vy << 12;
                p->pos.vz = base->vz << 12;
                p->unk10 = 0;
                break;
            }
            case 3: {
                EffEmit *mot = (EffEmit *)e->file;
                *(s32 *)&p->unkC = *(s32 *)&rot.vx;
                p->unk20 = mot->unk8 + btlfx_randCentered(mot->unkC);
                p->unk22 = mot->unkA + btlfx_randCentered(mot->unkE);
                p->unk10 = e->unk8 + btlfx_randCentered(mot->unk16);
                p->unk24.s.y = rot.vx + btlfx_randCentered(mot->unk10);
                p->unk28.s.x = rot.vy + btlfx_randCentered(mot->unk12);
                p->unk24.s.x = 0;
                break;
            }
            case 4: {
                EffEmit *mot = (EffEmit *)e->file;
                p->unk28.w = *(s32 *)&rot.vx;
                btlfx_lerpVec(&mot->unk10, &mot->unk14, t, &tmp.vx);
                p->unkE = tmp.vy + btlfx_randCentered(mot->unk1A);
                p->unk20 = tmp.vx + btlfx_randCentered(mot->unk18);
                p->unk22 = e->unk8 + btlfx_randCentered(mot->unkC);
                p->unkC = btlfx_rand();
                p->unk24.w = 0;
                p->unk10 = 0;
                break;
            }
            case 0:
            case 5: {
                EffEmit *mot = (EffEmit *)e->file;
                acc = e->unk8;
                btlfx_rotateOffset(&rot, acc + btlfx_randCentered(mot->unkC), (s32)&p->unkC);
                acc = mot->unkE + btlfx_randCentered(mot->unk10);
                if (acc < 0) {
                    acc = 0;
                }
                p->unk20 = acc;
                break;
            }
            }
            if (el != NULL && ((u32)(el->kind - 4) < 2 || el->kind == 8 || el->kind2 == 4 || el->kind2 == 5 || el->kind2 == 8)) {
                tmp.vx = p->pos.vx >> 12;
                tmp.vy = p->pos.vy >> 12;
                tmp.vz = p->pos.vz >> 12;
                btlfx_drawTrail((u8 *)el->tex, (s32)&tmp, NULL, -1, 0, 0, 0, (u8 *)func_800E0134(p, e), 0);
            }
        }
    }
}

void func_800E2F84(Pool30 *pool, Pool30Entry *e, SVECTOR *pos, SVECTOR *rot) {
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
    if (owner->state == 0) {
        return;
    }
    if (owner->state != -1) {
        sub = owner->sub - 1;
        frame = e->frame;
        t = (frame << 12) / e->frames;
        if ((!(e->unk12 & 0x80) || owner->frame < owner->tail->unk4.unk2) && frame >= sub->unk11 - 1) {
            r = btlfx_lerp(sub->unk0, sub->unk2, t);
            cnt = r >> 8;
            k = (e->unk12 & 0x7F) + ((r >> 1) & 0x7F);
            if (k >= 0x80) {
                k -= 0x80;
                if (k == 0) {
                    k = 1;
                }
                cnt++;
            }
            e->unk12 = (u8)k | (e->unk12 & 0x80);
            n = cnt;
            for (i = 0; i < n; i++) {
                if (owner->unk34.s.y >= sub->unk10) {
                    return;
                }
                p = btlfx_allocParticle(pool);
                if (p == NULL) {
                    return;
                }
                owner->unk34.s.y++;
                cnt = btlfx_rand() & 0x7F;
                k = 0;
                r = 0;
                for (; k < sub->unkD; k++) {
                    r += sub->unk8[k];
                    if (cnt < r) {
                        break;
                    }
                }
                if (k >= sub->unkD) {
                    cnt = 0xFF;
                } else {
                    cnt = sub->unk4[k];
                }
                p->elem = cnt;
                p->elemFrame = 0;
                if (cnt == 0xFF) {
                    r = btlfx_lerp(((EffEmit *)owner->sub)->unk2, ((EffEmit *)owner->sub)->unk4, t);
                    r += btlfx_randCentered(((EffEmit *)owner->sub)->unk6);
                    if (r <= 0) {
                        r = 1;
                    }
                } else {
                    el = &owner->elems[cnt];
                    if (el->unk4 == 1) {
                        r = el->unk8;
                    } else {
                        r = btlfx_lerp(((EffEmit *)owner->sub)->unk2, ((EffEmit *)owner->sub)->unk4, t);
                    r += btlfx_randCentered(((EffEmit *)owner->sub)->unk6);
                        if (r <= 0) {
                            r = 1;
                        }
                    }
                    off.vx = (el->unk14[0] * 8) & 0xFFF;
                    off.vy = (el->unk14[1] * 8) & 0xFFF;
                    off.vz = (el->unk14[2] * 8) & 0xFFF;
                    gte_lddp(frame);
                    gte_ldsv(&off);
                    gte_gpf0();
                    ang.vx = (el->unk11[0] * 16 + btlfx_randCentered(el->unk17[0] * 16)) & 0xFFF;
                    ang.vy = (el->unk11[1] * 16 + btlfx_randCentered(el->unk17[1] * 16)) & 0xFFF;
                    ang.vz = (el->unk11[2] * 16 + btlfx_randCentered(el->unk17[2] * 16)) & 0xFFF;
                    gte_lddp(1);
                    gte_ldsv(&ang);
                    gte_gpl0();
                    p->unk13 = btlfx_randCentered(el->unk10);
                    gte_stsv(&p->unk14);
                }
                if (((EffEmit *)owner->sub)->unk1 != 0) {
                    p->unk12 = 0xFF;
                } else {
                    p->unk12 = 0x80;
                }
                p->unk1F = owner->unk10 % 255 + 1;
                p->frames = r;
                p->frame = 0;
                p->owner = owner;
                p->timeStep = btlfx_randCentered(sub->unk12) + 0x40;
                owner->unk10++;
                if (owner->unk14 != 0) {
                    owner->unk10 %= owner->unk12;
                }
                gte_lddp(0x1000 - t);
                gte_ldsv(sub->unk30);
                gte_gpf12();
                btlfx_randRotation(&ang);
                gte_lddp(t);
                gte_ldsv(sub->unk34);
                gte_gpl12();
                gte_stsv(&dir);
                btlfx_rotateOffset(&ang, btlfx_randRange(dir.vx, dir.vy), (s32)&off);
                p->pos.vx = (off.vx + pos->vx) << 12;
                p->pos.vy = (off.vy + pos->vy) << 12;
                p->pos.vz = (off.vz + pos->vz) << 12;
                switch (sub->unkC) {
                case 2:
                    btlfx_randRotation(&ang);
                    break;
                case 3:
                    ((XY32 *)&ang)[0].w = ((XY32 *)&owner->unk5C)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)&owner->unk5C)[1].w;
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    break;
                case 4:
                    ((XY32 *)&ang)[0].w = ((XY32 *)rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)rot)[1].w;
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    break;
                case 0:
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    break;
                case 5:
                    ((XY32 *)&ang)[0].w = ((XY32 *)rot)[0].w;
                    ((XY32 *)&ang)[1].w = ((XY32 *)rot)[1].w;
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    ang.vx = (ang.vx + 0x800) & 0xFFF;
                    break;
                case 1:
                    ang.vx += btlfx_randCentered(sub->unkE);
                    ang.vy += btlfx_randCentered(sub->unkE);
                    ang.vx = (ang.vx + 0x800) & 0xFFF;
                    break;
                }
                if (el != NULL && el->unk7 != 0) {
                    p->unk14.s.x += ang.vx;
                    p->unk14.s.y += ang.vy;
                }
                switch (((EffEmit *)owner->sub)->unk0) {
                case 1:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    r = btlfx_lerp(mot->unk20, mot->unk22, t);
                    btlfx_rotateOffset(&ang, r + btlfx_randCentered(mot->unk24), (s32)&p->unkC);
                    r = mot->unk10 + btlfx_randCentered(mot->unk12);
                    if (r < 0) {
                        r = 0;
                    }
                    p->unk28.s.y = r;
                    p->unk24.w = *(s32 *)&ang.vx;
                    p->unk20 = mot->unk8 + btlfx_randCentered(mot->unkC);
                    p->unk22 = mot->unkA + btlfx_randCentered(mot->unkE);
                    p->unk28.s.x = btlfx_lerp(mot->unk18, mot->unk1A, t) + btlfx_randCentered(mot->unk16);
                    break;
                    }
                case 2:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    p->unk28.w = *(s32 *)&ang.vx;
                    btlfx_lerpVec(&mot->unk10, &mot->unk14, t, (s16 *)&dir);
                    p->unkE = dir.vy + btlfx_randCentered(mot->unkE);
                    p->unk24.s.y = dir.vx + btlfx_randCentered(mot->unk18);
                    p->unk22 = btlfx_lerp(mot->unk8, mot->unkA, t) + btlfx_randCentered(mot->unkC);
                    *(s32 *)&dir.vx = 0;
                    *(s32 *)&dir.vz = 0;
                    func_800D0674(&off, &dir, &ang, (s32)&d40, (s32)&cnt, &k);
                    p->unkC = k;
                    p->unk20 = d40;
                    p->unk24.s.x = cnt;
                    p->pos.vx = e->pos.vx;
                p->pos.vy = e->pos.vy;
                p->pos.vz = e->pos.vz;
                    p->unk10 = 0;
                    break;
                    }
                case 3:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    *(s32 *)&p->unkC = *(s32 *)&ang.vx;
                    p->unk20 = mot->unk8 + btlfx_randCentered(mot->unkC);
                    p->unk22 = mot->unkA + btlfx_randCentered(mot->unkE);
                    p->unk10 = btlfx_lerp(mot->unk18, mot->unk1A, t) + btlfx_randCentered(mot->unk16);
                    p->unk24.s.y = ang.vx + btlfx_randCentered(mot->unk10);
                    p->unk28.s.x = ang.vy + btlfx_randCentered(mot->unk12);
                    p->unk24.s.x = 0;
                    break;
                    }
                case 4:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    p->unk28.w = *(s32 *)&ang.vx;
                    btlfx_lerpVec(&mot->unk10, &mot->unk14, t, (s16 *)&dir);
                    p->unkE = dir.vy + btlfx_randCentered(mot->unk1A);
                    p->unk20 = dir.vx + btlfx_randCentered(mot->unk18);
                    p->unk22 = btlfx_lerp(mot->unk8, mot->unkA, t) + btlfx_randCentered(mot->unkC);
                    p->unkC = btlfx_rand();
                    p->unk24.w = 0;
                    p->unk10 = 0;
                    break;
                    }
                case 0:
                case 5:
                    {
                    EffEmit *mot = (EffEmit *)owner->sub;
                    r = btlfx_lerp(mot->unk8, mot->unkA, t);
                    btlfx_rotateOffset(&ang, r + btlfx_randCentered(mot->unkC), (s32)&p->unkC);
                    r = mot->unkE + btlfx_randCentered(mot->unk10);
                    if (r < 0) {
                        r = 0;
                    }
                    p->unk20 = r;
                    break;
                    }
                }
                if (el != NULL && ((u32)(el->kind - 4) < 2 || el->kind == 8 || el->kind2 == 4 || el->kind2 == 5 || el->kind2 == 8)) {
                    dir.vx = p->pos.vx >> 12;
                    dir.vy = p->pos.vy >> 12;
                    dir.vz = p->pos.vz >> 12;
                    btlfx_drawTrail((u8 *)el->tex, (s32)&dir, NULL, -1, 0, 0, 0, (u8 *)func_800E0134(p, owner), 0);
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
    func_800DA4E8();
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
    if (D_800F78AC == 0) {
        D_800F78AC = 1;
        akao_pause(2);
    }
}
