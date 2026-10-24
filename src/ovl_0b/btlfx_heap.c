#include "ovl_0b.h"

void func_800E00DC(s32 arg0, Pool30Entry *arg1, Pool30 *arg2) {
    s32 i = 0;
    arg2->unk0 = arg0 / (s32)sizeof(Pool30Entry);
    arg2->unkC = arg1;
    arg2->unk4 = 0;
    arg2->unk8 = 0;
    for (; i < arg2->unk0;) {
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

s32 func_800E0168(Pool30 *pool) {
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
    e = pool->unkC;
    amp = 0;
    i = 0;
    D_801014B0 = g_btlfxEnv->unk28;
    pool->unk4 = 0;
    for (; i <= pool->unk8; i++, e++) {
        if (e->unk1F == 0) {
            continue;
        }
        frame = e->unk1A;
        frames = e->unk1B;
        owner = e->unk2C;
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
        pool->unk4++;
        t = (frame << 12) / frames;
        if (e->unk12 & 0x80) {
            m = (EffMotion *)owner->unk4;
            sub = owner->unk4 - 1;
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
            m = (EffMotion *)owner->unk0;
            mode = owner->unk24->unk4.unk1C;
            rs = owner->unk4A;
            rp = owner->unk44;
            if (mode != 0) {
                curve = owner->unk24->unk4.unk1E;
                range = owner->unk4C.s.x;
                if (mode < 7) {
                    amp = owner->unk4C.s.y;
                } else {
                    amp = owner->unk24->unk4.unk20[1];
                }
            }
            if (owner->unk4 != NULL) {
                lvl = (owner->unk4 - 1)->unkC;
                if (lvl >= 4) {
                    rel.vx = e->unk0.vx >> 12;
                    rel.vy = e->unk0.vy >> 12;
                    rel.vz = e->unk0.vz >> 12;
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
                        e->unkE = (s16)(e->unkE - 0x40) - ((owner->unk4A * e->unk1E) >> 6);
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
            func_800CFD78((s16 *)&pos, (s16 *)&v, rs, (s16 *)&pos);
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
            func_800CFD78((s16 *)&pos, (s16 *)&v, rs, (s16 *)&pos);
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
            func_800CFD78(&e->unkC, (s16 *)&v, rs, &e->unkC);
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
                    func_800CFCA8(owner->unk68, 0x1000, (s32)&v);
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
                    e->unk0.vx += d.vx << 6;
                    e->unk0.vy += d.vy << 6;
                    e->unk0.vz += d.vz << 6;
                } else {
                    ofs = amp;
                }
            } else if (mode >= 7) {
                ofs = 0;
            }
        }
        g_btlfxEnv->unk28 = owner->unk22;
        if (e->unk1C != 0xFF) {
            el = &owner->unk18[e->unk1C];
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
                    rs = el->unk0;
                    mode = el->unk1C;
                } else {
                    rs = el->unk1;
                    mode = el->unk20;
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
                    btlfx_drawParticles((u8 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha, 0, 0);
                    break;
                case 1:
                    func_800D198C((u8 *)mode, &pos, e->unk1D, cnt, alpha);
                    break;
                case 2:
                    btlfx_drawAnimQuads((u8 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha);
                    break;
                case 3:
                    btlfx_drawSprites((u8 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha);
                    break;
                case 4:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, (s32 *)owner->unk28, e->unk1D, scale, alpha, cnt, (u8 *)func_800E0134(e, owner), 1);
                    break;
                case 5:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, NULL, e->unk1D, scale, alpha, cnt, (u8 *)func_800E0134(e, owner), 2);
                    break;
                case 6:
                    func_800D23D8((u8 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha);
                    break;
                case 7:
                    func_800D2938((Unk800D2938 *)mode, &pos, vp, scale, e->unk1D, cnt, alpha);
                    break;
                case 8:
                    btlfx_drawTrail((u8 *)mode, (s32)&pos, NULL, e->unk1D, scale, alpha, cnt, (u8 *)func_800E0134(e, owner), 0);
                    break;
                }
                if (el->unk1 == 0xFF) {
                    break;
                }
            }
            if (el->unk4 == 1) {
                e->unk1D++;
            } else if (el->unk4 == 2) {
                rs = e->unk1D;
                rs++;
                if (rs >= el->unk8) {
                    rs = 0;
                }
                e->unk1D = rs;
            }
        }
        if (owner->unk4 != NULL && (e->unk12 & 0x7F)) {
            if (lvl >= 4) {
                btlfx_aimAngles((s16 *)&rel, (s16 *)&pos, (s16 *)&v);
            }
            func_800E2F84(pool, e, &pos, &v);
        }
        frame++;
        e->unk1A = frame;
    }
    pool->unk8 = last;
    g_btlfxEnv->unk28 = D_801014B0;
    return pool->unk4;
}

void func_800E14D8(u32 arg0, Pool6C *arg1) {
    s32 i;
    Pool6CEntry *e = arg1->unk10;
    arg1->unk0 = (arg0 - 0x10) / sizeof(Pool6CEntry);
    arg1->unkC = e;
    arg1->unk4 = 0;
    arg1->unk8 = 0;
    for (i = 0; i < arg1->unk0;) {
        i++;
        e->unk30 = 0;
        e++;
    }
}

Pool6CEntry *func_800E1534(Pool30 *arg0, Pool6C *arg1, u8 *arg2, s32 arg3) {
    Pool6CEntry *e;
    s32 i;
    u32 k;
    u8 *p;
    EffElemHdr *h;
    EffElem *el;
    s32 *q;
    s32 v;

    e = arg1->unkC;
    for (i = 0; i < arg1->unk0; i++, e++) {
        if (e->unk30 == 0) {
            if (arg1->unk8 < i) {
                arg1->unk8 = i;
            }
            p = arg2;
            for (k = 0; k < 0x6C; k += 4) {
                *(s32 *)((u8 *)e + k) = 0;
            }
            e->unk30 = 1;
            e->unkE = 0;
            e->unk10 = 0;
            e->unk32 = 0;
            e->unk34.s.x = 0;
            e->unk34.s.y = 0;
            e->unk14 = 0;
            e->unk3A = 0xFF;
            e->unk22 = 0;
            e->unk0 = (EffFileHdr *)p;
            if (*p != 0xFF) {
                p += 0x28;
            } else {
                p += 0x10;
            }
            if (((u8 *)e->unk0)[1] != 0) {
                p += 0x38;
                e->unk4 = (EffSub *)p;
                p += 0x28;
            } else {
                e->unk4 = NULL;
            }
            h = (EffElemHdr *)p;
            p += 0x14;
            el = (EffElem *)p;
            e->unk18 = el;
            i = 0;
            p += h->unk4 * 0x28;
            for (; i < h->unk4; i++, el++) {
                if (el->unk0 != 9) {
                    if (el->unk1C != -1 && !(el->unk1C & 0x80000000)) {
                        el->unk1C = (s32)arg2 + el->unk1C;
                    }
                }
                if (el->unk1 != 0xFF) {
                    if (el->unk20 != -1 && !(el->unk20 & 0x80000000)) {
                        el->unk20 = (s32)arg2 + el->unk20;
                    }
                }
            }
            /* unk24 (tail), unk28, unk2C written through one pointer */
            q = (s32 *)&e->unk24;
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

void func_800E1708(Pool30 *arg0, s32 arg1, s32 arg2) {
    Pool6CEntry *m = (Pool6CEntry *)arg1;
    SVECTOR *v = (SVECTOR *)arg2;
    VECTOR d;
    Pool30Entry *e;
    s32 i;
    d.vx = (v->vx - m->unk28->vx) << 12;
    d.vy = (v->vy - m->unk28->vy) << 12;
    d.vz = (v->vz - m->unk28->vz) << 12;
    e = arg0->unkC;
    for (i = 0; i < arg0->unk0; i++, e++) {
        if (e->unk1F != 0 && e->unk2C == m) {
            e->unk0.vx += d.vx;
            e->unk0.vy += d.vy;
            e->unk0.vz += d.vz;
        }
    }
    *(s32 *)&m->unk28->vx = *(s32 *)&v->vx;
    m->unk28->vz = v->vz;
}

void func_800E1810(Pool30 *arg0, Pool6CEntry *arg1) {
    s32 i = 0;
    Pool30Entry *p = arg0->unkC;
    if (i < arg0->unk0) {
        do {
            if (p->unk1F != 0 && p->unk2C == arg1) {
                p->unk1A = p->unk1B;
            }
            i++;
            p++;
        } while (i < arg0->unk0);
    }
    arg1->unk30 = -1;
}

s32 func_800E1874(Pool30 *arg0, Pool6C *arg1) {
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
    e = p->unkC;
    v = 0;
    i = 0;
    p->unk4 = 0;
    for (; i <= p->unk8; i++, e++) {
        if (e->unk30 == 0) {
            continue;
        }
        if (e->unk30 == -1) {
            e->unk30 = 0;
            continue;
        }
        p->unk4++;
        def = e->unk24;
        frame = e->unk38;
        an = &def->unk4;
        last = i;
        y = an->unk2;
        t = (frame << 12) / y;
        e->unk32 = 0;
        if (frame >= y) {
            if (e->unk34.w == 0) {
                e->unk30 = -1;
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
            def = e->unk24;
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
        hdr = (EffElemHdr *)e->unk18 - 1;
        if (hdr->unkE == 1) {
            hdr->unk10.w = 0;
            x = btlfx_lerp(0, hdr->unk0 * 4, t);
            y = (x & 3) << 5;
            v = x >> 2;
            if (y != 0) {
                x = v + 1;
                if (x >= hdr->unk0) {
                    x = v;
                }
                hdr->unk10.b[v] = -0x80 - y;
                hdr->unk10.b[x] += y;
            } else {
                hdr->unk10.b[v] = 0x80;
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
        if (e->unk4 != NULL) {
            sub = e->unk4 - 1;
            btlfx_lerpVec(sub->unk14, sub->unk1C, t, e->unk50);
            e->unk56 = btlfx_lerp(sub->unk1A, sub->unk22, t);
            if (sub->unk24 != 0) {
                btlfx_lerpVec(sub->unk28, sub->unk2C, t, a.h);
                e->unk58.w = a.w;
            }
        }
        d2 = e->unk24;
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
        switch (e->unk0->unk0) {
        case 0:
        case 5:
            e->unk8 = btlfx_lerp(e->unk0->unk8, e->unk0->unkA, t);
            break;
        case 1:
            f = e->unk0;
            e->unk8 = btlfx_lerp(f->unk18, f->unk1A, t);
            e->unkA = btlfx_lerp(f->unk20, f->unk22, t);
            break;
        case 2:
        case 4:
            e->unk8 = btlfx_lerp(e->unk0->unk8, e->unk0->unkA, t);
            break;
        case 3:
            e->unk8 = btlfx_lerp(e->unk0->unk18, e->unk0->unk1A, t);
            break;
        }
        frame++;
        e->unk38 = frame;
    }
    p->unk8 = last;
    return p->unk4;
}

Pool30Entry *func_800E1F90(Pool30 *arg0) {
    s32 i;
    Pool30Entry *p = arg0->unkC;
    for (i = 0; i < arg0->unk0; i++, p++) {
        if (p->unk1F == 0) {
            if (arg0->unk8 < i) {
                arg0->unk8 = i;
            }
            break;
        }
    }
    if (i >= arg0->unk0) {
        p = 0;
    }
    return p;
}

void func_800E2000(Pool30 *pool, Pool6C *list) {
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
    e = pl->unkC;
    base = NULL;
    for (i = 0; i <= pl->unk8; i++, e++) {
        if (e->unk30 == 0) {
            continue;
        }
        if (e->unk30 == -1) {
            continue;
        }
        for (j = 0; j < e->unk32; j++) {
            if (e->unk34.s.x >= e->unk24->unk4.unk8) {
                break;
            }
            p = func_800E1F90(pool);
            if (p == NULL) {
                break;
            }
            frame = e->unk38;
            anim = &e->unk24->unk4;
            e->unk34.s.x++;
            dur = anim->unk2;
            t = (frame << 12) / dur;
            hdr = (EffElemHdr *)e->unk18 - 1;
            acc = 0;
            r = btlfx_rand() & 0x7F;
            for (k = 0; k < hdr->unk0; k++) {
                acc += hdr->unk10.b[k];
                if (r < acc) {
                    break;
                }
            }
            if (k >= hdr->unk0) {
                k = 0xFF;
            }
            p->unk1C = k;
            p->unk1D = 0;
            if (k == 0xFF) {
                acc = btlfx_lerp(((EffEmit *)e->unk0)->unk2, ((EffEmit *)e->unk0)->unk4, t);
                acc += btlfx_randCentered(((EffEmit *)e->unk0)->unk6);
                if (acc <= 0) {
                    acc = 1;
                }
            } else {
                el = &e->unk18[k];
                if (el->unk4 == 1) {
                    acc = el->unk8;
                } else {
                    acc = btlfx_lerp(((EffEmit *)e->unk0)->unk2, ((EffEmit *)e->unk0)->unk4, t);
                    acc += btlfx_randCentered(((EffEmit *)e->unk0)->unk6);
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
            p->unk1B = acc;
            p->unk1A = 0;
            p->unk2C = e;
            p->unk1E = btlfx_randCentered(anim->unkA) + 0x40;
            if (((EffEmit *)e->unk0)->unk1 != 0) {
                p->unk12 = 0x7F;
            } else {
                p->unk12 = 0;
            }
            e->unkE++;
            if (e->unk14 != 0) {
                e->unkE %= e->unk12;
            }
            switch (e->unk24->unk0) {
            case 0:
                btlfx_randRotation(&rot);
                acc = btlfx_randRange(e->unk3C, e->unk3E);
                if (((EffTailSpawn *)e->unk24)->unk44.cd == 1) {
                    if (rot.vx > 0x800) {
                        rot.vx -= 0x800;
                    }
                } else if (((EffTailSpawn *)e->unk24)->unk44.cd == 2) {
                    if (rot.vx < 0x800) {
                        rot.vx += 0x800;
                    }
                }
                func_800CFCA8(&rot, acc, (s32)&off);
                LoadAverageCol((u8 *)&((EffTailSpawn *)e->unk24)->unk44, (u8 *)&((EffTailSpawn *)e->unk24)->unk48.c, 0x1000 - t, t, (u8 *)&col);
                off.vx = (off.vx * col.r) >> 7;
                off.vy = (off.vy * col.g) >> 7;
                off.vz = (off.vz * col.b) >> 7;
                base = e->unk28;
                break;
            case 1:
                rot.vx = 0;
                rot.vy = btlfx_rand();
                rot.vz = 0;
                func_800CFCA8(&rot, btlfx_randRange(e->unk3C, e->unk3E), (s32)&off);
                off.vy = btlfx_randRange(e->unk40, e->unk42);
                if (((EffTailSpawn *)e->unk24)->unk4C == 1) {
                    rot.vx = 0x400;
                    rot.vy = 0;
                } else if (((EffTailSpawn *)e->unk24)->unk4C == 2) {
                    rot.vx = -0x400;
                    rot.vy = 0;
                }
                base = e->unk28;
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
                if (((EffTailSpawn *)e->unk24)->unk4C == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->unk24)->unk4C == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    func_800D0580(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->unk28;
                break;
            case 3:
                btlfx_aimAngles((s16 *)e->unk28, (s16 *)e->unk2C, &ang.vx);
                d58 = btlfx_rand();
                acc = btlfx_randRange(e->unk40, e->unk42);
                r = btlfx_randRange(e->unk3C, e->unk3E);
                btlfx_lerpVec((s16 *)e->unk28, (s16 *)e->unk2C, acc, &off.vx);
                func_800D044C(&off, (s32)&off, 0, (XY32 *)&ang, r, d58);
                if (((EffTailSpawn *)e->unk24)->unk4C == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->unk24)->unk4C == 2) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                    rot.vx += 0x800;
                } else {
                    func_800D0580(&rot.vx, (XY32 *)&ang, d58);
                }
                base = e->unk28;
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
                if (((EffTailSpawn *)e->unk24)->unk48.h == 1) {
                    ((XY32 *)&rot)[0].w = ((XY32 *)&ang)[0].w;
                } else if (((EffTailSpawn *)e->unk24)->unk48.h == 2) {
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
                base = e->unk28;
                break;
            }
            case -1:
                off.pad = 0;
                if (((EffTailSpawn *)e->unk24)->unk3C(&off, &rot, frame, dur) != 0) {
                    anim->unk2 = 0;
                }
                if (off.pad != 0) {
                    base = &tmp;
                    *(s32 *)&tmp.vx = 0;
                    *(s32 *)&tmp.vz = 0;
                } else {
                    base = e->unk28;
                }
                break;
            }
            p->unk0.vx = (off.vx + base->vx) << 12;
            p->unk0.vy = (off.vy + base->vy) << 12;
            p->unk0.vz = (off.vz + base->vz) << 12;
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
            switch (((EffEmit *)e->unk0)->unk0) {
            case 1: {
                EffEmit *mot = (EffEmit *)e->unk0;
                acc = e->unkA;
                func_800CFCA8(&rot, acc + btlfx_randCentered(mot->unk24), (s32)&p->unkC);
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
                EffEmit *mot = (EffEmit *)e->unk0;
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
                p->unk0.vx = base->vx << 12;
                p->unk0.vy = base->vy << 12;
                p->unk0.vz = base->vz << 12;
                p->unk10 = 0;
                break;
            }
            case 3: {
                EffEmit *mot = (EffEmit *)e->unk0;
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
                EffEmit *mot = (EffEmit *)e->unk0;
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
                EffEmit *mot = (EffEmit *)e->unk0;
                acc = e->unk8;
                func_800CFCA8(&rot, acc + btlfx_randCentered(mot->unkC), (s32)&p->unkC);
                acc = mot->unkE + btlfx_randCentered(mot->unk10);
                if (acc < 0) {
                    acc = 0;
                }
                p->unk20 = acc;
                break;
            }
            }
            if (el != NULL && ((u32)(el->unk0 - 4) < 2 || el->unk0 == 8 || el->unk1 == 4 || el->unk1 == 5 || el->unk1 == 8)) {
                tmp.vx = p->unk0.vx >> 12;
                tmp.vy = p->unk0.vy >> 12;
                tmp.vz = p->unk0.vz >> 12;
                btlfx_drawTrail((u8 *)el->unk1C, (s32)&tmp, NULL, -1, 0, 0, 0, (u8 *)func_800E0134(p, e), 0);
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

    owner = e->unk2C;
    el = NULL;
    if (owner->unk30 == 0) {
        return;
    }
    if (owner->unk30 != -1) {
        sub = owner->unk4 - 1;
        frame = e->unk1A;
        t = (frame << 12) / e->unk1B;
        if ((!(e->unk12 & 0x80) || owner->unk38 < owner->unk24->unk4.unk2) && frame >= sub->unk11 - 1) {
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
                p = func_800E1F90(pool);
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
                p->unk1C = cnt;
                p->unk1D = 0;
                if (cnt == 0xFF) {
                    r = btlfx_lerp(((EffEmit *)owner->unk4)->unk2, ((EffEmit *)owner->unk4)->unk4, t);
                    r += btlfx_randCentered(((EffEmit *)owner->unk4)->unk6);
                    if (r <= 0) {
                        r = 1;
                    }
                } else {
                    el = &owner->unk18[cnt];
                    if (el->unk4 == 1) {
                        r = el->unk8;
                    } else {
                        r = btlfx_lerp(((EffEmit *)owner->unk4)->unk2, ((EffEmit *)owner->unk4)->unk4, t);
                    r += btlfx_randCentered(((EffEmit *)owner->unk4)->unk6);
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
                if (((EffEmit *)owner->unk4)->unk1 != 0) {
                    p->unk12 = 0xFF;
                } else {
                    p->unk12 = 0x80;
                }
                p->unk1F = owner->unk10 % 255 + 1;
                p->unk1B = r;
                p->unk1A = 0;
                p->unk2C = owner;
                p->unk1E = btlfx_randCentered(sub->unk12) + 0x40;
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
                func_800CFCA8(&ang, btlfx_randRange(dir.vx, dir.vy), (s32)&off);
                p->unk0.vx = (off.vx + pos->vx) << 12;
                p->unk0.vy = (off.vy + pos->vy) << 12;
                p->unk0.vz = (off.vz + pos->vz) << 12;
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
                switch (((EffEmit *)owner->unk4)->unk0) {
                case 1:
                    {
                    EffEmit *mot = (EffEmit *)owner->unk4;
                    r = btlfx_lerp(mot->unk20, mot->unk22, t);
                    func_800CFCA8(&ang, r + btlfx_randCentered(mot->unk24), (s32)&p->unkC);
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
                    EffEmit *mot = (EffEmit *)owner->unk4;
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
                    p->unk0.vx = e->unk0.vx;
                p->unk0.vy = e->unk0.vy;
                p->unk0.vz = e->unk0.vz;
                    p->unk10 = 0;
                    break;
                    }
                case 3:
                    {
                    EffEmit *mot = (EffEmit *)owner->unk4;
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
                    EffEmit *mot = (EffEmit *)owner->unk4;
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
                    EffEmit *mot = (EffEmit *)owner->unk4;
                    r = btlfx_lerp(mot->unk8, mot->unkA, t);
                    func_800CFCA8(&ang, r + btlfx_randCentered(mot->unkC), (s32)&p->unkC);
                    r = mot->unkE + btlfx_randCentered(mot->unk10);
                    if (r < 0) {
                        r = 0;
                    }
                    p->unk20 = r;
                    break;
                    }
                }
                if (el != NULL && ((u32)(el->unk0 - 4) < 2 || el->unk0 == 8 || el->unk1 == 4 || el->unk1 == 5 || el->unk1 == 8)) {
                    dir.vx = p->unk0.vx >> 12;
                    dir.vy = p->unk0.vy >> 12;
                    dir.vz = p->unk0.vz >> 12;
                    btlfx_drawTrail((u8 *)el->unk1C, (s32)&dir, NULL, -1, 0, 0, 0, (u8 *)func_800E0134(p, owner), 0);
                }
            }
        }
    }
}

void btlfx_heapInit(Heap *arg0, s32 arg1) {
    s32 size = btlfx_align4(arg1) - 0xC;
    arg0->unk0 = arg0->unkC;
    arg0->unk4 = size;
    arg0->unk8 = size;
}

s32 btlfx_heapAlloc(Heap *h, s32 size) {
    s32 sz;
    u8 *p;
    s32 *q;

    sz = btlfx_align4(size);
    if (h->unk4 < sz) {
        q = (s32 *)h->unkC;
        h->unk0 = (u8 *)q;
        h->unk4 = h->unk8;
    }
    p = h->unk0;
    h->unk4 -= sz;
    h->unk0 = p + sz;
    q = (s32 *)p;
    for (size = 0; size < sz >> 2; size++) {
        *q++ = 0;
    }
    return (s32)p;
}

s32 btlfx_heapWrapIfFull(Heap *arg0, s32 arg1) {
    s32 size = btlfx_align4(arg1);
    s32 ret = 1;
    if (arg0->unk4 < size) {
        arg0->unk0 = arg0->unkC;
        arg0->unk4 = arg0->unk8;
    } else {
        ret = 0;
    }
    return ret;
}

void btlfx_heapFree(void *arg0) {
}

void func_800E3C58(void) {
    func_800E00DC(0x3000, D_801014B8, &D_801044B8);
    func_800E14D8(0x920, &D_801044C8);
}

void func_800E3C98(void) {
    func_800E2000(&D_801044B8, &D_801044C8);
    func_800E0168(&D_801044B8);
    func_800E1874(&D_801044B8, &D_801044C8);
}

Pool6CEntry *func_800E3CEC(s32 arg0, s32 arg1) {
    return func_800E1534(&D_801044B8, &D_801044C8, (u8 *)arg0, arg1);
}

void func_800E3D20(s32 arg0, s32 arg1) {
    if (arg0 != 0) {
        func_800E1708(&D_801044B8, arg0, arg1);
    }
}

void func_800E3D54(Pool6CEntry *arg0) {
    if (arg0 != NULL) {
        func_800E1810(&D_801044B8, arg0);
    }
}

void func_800E3D80(void) {
    btlfx_initCommonData();
    btlfx_resetAll();
}

void btlfx_resetAll(void) {
    btlfx_clearTargets();
    btlfx_reset();
    btlfx_initTasks();
    func_800DA4E8();
    func_800E3C58();
    btlfx_initSounds();
    btlfx_clearEffModels();
    btlfx_clearSummonModels();
    func_80054F80();
}

void func_800E3E08(void) {
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
        func_8005562C(2);
    }
}
