#include "ff9.h"

s32 func_80037D6C(s32 op) {
    s32 r;
    u8 buf[8];
    Unk8004AB64 *o;
    s32 id;

    switch ((u32)op) {
    case 0:
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        r = *g_randPtr++;
        break;
    case 1:
        func_800C1B5C(buf);
        r = buf[0];
        break;
    case 2:
        r = g_evtState->unk16EC;
        break;
    case 3:
        r = func_800B0124(0xC00, -1, 0, 0, 0);
        break;
    case 4:
        r = D_8007AE20;
        break;
    case 5:
        id = ((u8 *)&((u32 *)g_evtCurObj + g_evtCurObj->unk9)[g_evtCurObj->unkA])[-1];
        o = func_8004B040_inline(id);
        r = -1;
        if (o != NULL) {
            r = o->unk6 - (D_8007ADFC - 9);
        }
        break;
    case 6:
        r = g_gameState->unk1C->gil;
        break;
    case 7:
        r = D_8007AEC4;
        break;
    case 8:
        r = D_8007BBD8;
        break;
    case 9:
        r = func_8004ECC4();
        break;
    case 10:
        r = D_8007AE24;
        break;
    case 11:
        r = D_8007AE28;
        break;
    case 12:
        r = D_8007AE2C;
        break;
    case 13:
        r = D_8007AE30;
        break;
    case 14:
        r = fld_movieCommand(4, 0, 0);
        break;
    case 15:
        r = fld_movieCommand(3, 0, 0);
        break;
    case 16:
        r = g_gameState->unk1C->unk568;
        break;
    case 17:
        r = timer_getSeconds();
        break;
    case 18:
        r = func_8006275C();
        break;
    case 19:
        r = func_800625F8();
        break;
    case 20:
        r = game_getPlayTimeSeconds();
        if (r > 0x7FFFFF) {
            r = 0x7FFFFF;
        }
        break;
    case 21:
        r = timer_isEnabled();
        break;
    case 22:
        r = D_8007AE38;
        break;
    case 23:
        r = D_8007AE3C;
        break;
    case 24:
        r = (s16)g_gameState->unk1C->battleId;
        break;
    case 25:
        r = (u8)btlseq_isBusy();
        break;
    case 26:
        r = g_evtMode == 4;
        break;
    case 27:
        r = g_gameState->unk1C->unk56E;
        break;
    case 28:
        r = (D_8007AE34 >> 8) & 0xFF;
        break;
    case 29:
        r = (u8)D_8007AE34;
        break;
    case 30:
        r = g_gameState->unk1C->unk8E4.ovl65->unkD80;
        break;
    case 31:
        r = g_gameState->unk1C->unk8E4.ovl65->unk1340;
        break;
    default:
        if (op >= 0xC0) {
            r = w_frameGetParameter(op);
        } else {
            r = btl_getBattleVar(op);
        }
        break;
    }
    return r;
}

Unk8004AB64 *func_80038150(void) {
    return func_8004B4C4_inline(evt_getArg8_inline2());
}

/* Find the entry point for `key` in script `idx`. */
static inline u8 *func_800384D0_find(s32 idx, s32 key) {
    u8 *h = D_8007ADF8 + *(u16 *)(D_8007ADF8 + idx * 8);
    s32 n = *(u16 *)h >> 8;
    u16 *p = (u16 *)(h + 2);

    while (n != 0) {
        if (*p++ == key) {
            break;
        }
        n--;
        p++;
    }
    return n != 0 ? h + (*p + 2) : NULL;
}

void func_80038278(Unk8004AB64 *o) {
    s32 v;
    Unk16D8 *n;
    Unk16D8 *m;
    Unk8004AB64 *t;
    Unk8004AB64 *q;
    u32 *base;
    u32 *p;

    if (o->unkA >= 2) {
        p = (u32 *)o + o->unk9 + o->unkA;
        p--;
        v = p[0];
        if (!(*(u32 *)&o->unk4 & 0xFF0000FF)) {
            for (m = g_evtState->unk16D8; m != NULL; m = m->unk0) {
                t = m->unk4;
                if (t->unk7 != 0) {
                    t->unkE = t->unk12;
                }
            }
        }
        o->unk0 = g_evtState->unk16F0 + p[-1];
        o->unkD = v;
        o->unkA -= 2;
        o->unk4 = v >> 8;
        if (((v >> 16) & 0xFF) != 0xFF) {
            for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
                if (n->unk4->unk7 == (u32)v >> 24) {
                    break;
                }
            }
            q = NULL;
            if (n != NULL) {
                q = n->unk4;
            }
            if (q != NULL) {
                if (q->unkD == 0xFF) {
                    q->unkD = 0;
                    return;
                }
                base = (u32 *)q + q->unk9;
                p = base + (q->unkA - 1);
                while (base < p) {
                    if (*(u8 *)p == 0xFF) {
                        break;
                    }
                    p -= 2;
                }
                if (base < p) {
                    *p &= ~0xFF;
                }
            }
        }
    } else if (o->unkE == 2) {
        o->unkE = 1;
        o->unk0 = func_800384D0_find(o->unk6, 1);
        o->unk4 = 7;
    } else {
        o->unk0 = NULL;
    }
}

s32 func_800384D0(Unk8004AB64 *o, s32 lv, s32 key, s32 save) {
    u8 *e = NULL;
    u32 *slot;
    u16 w;
    u32 hi;
    u32 x;

    if (o != NULL && lv < o->unk4) {
        e = func_800384D0_find(o->unk6, key);
        if (e != NULL) {
            slot = &((u32 *)o + o->unk9)[o->unkA];
            slot[0] = o->unk0 - g_evtState->unk16F0;
            w = o->unkD | (o->unk4 << 8);
            hi = g_evtCurObj->unk7 << 24;
            if (save) {
                x = (g_evtCurObj->unk4 << 16);
                x = x | w;
                x |= hi;
            } else {
                x = hi | 0xFF0000;
                x = x | w;
            }
            slot[1] = x;
            if (save) {
                g_evtCurObj->unkD = 0xFF;
            }
            o->unk0 = e;
            o->unk4 = lv;
            o->unkD = 0;
            o->unkA += 2;
        }
    }
    return e != NULL;
}

void *func_80038618(Unk8004AB64 *o, s32 flags, s32 *dist) {
    Unk8004AB64 *best = NULL;
    s32 bestd = 0;
    s32 x, y, z;
    s32 back;
    s32 r;
    Unk16D8 *n;
    Unk8004AB64 *q;
    s32 a;
    s32 b;
    s32 dx, dz;
    s32 rr;
    s32 d;
    s32 s;
    u8 f;
    u8 g;
    s32 key;

    x = o->pos.vx;
    y = o->pos.vy;
    back = flags & 4;
    z = o->pos.vz;
    if (back) {
        r = o->unk3D * 4;
    } else {
        r = o->unk3C * 4;
    }
    for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
        q = n->unk4;
        a = 0;
        if (back) {
            a = 1;
        } else {
            g = o->unkF;
            if (q->unk7 == g_evtState->playerUid ? (g & 2) : (g & 4)) {
                a = 1;
            }
        }
        s = q->unkF;
        if (best) {
            f = s;
        } else {
            f = q->unkF;
        }
        if (back) {
            b = f & 8;
        } else if (o->unk7 == g_evtState->playerUid) {
            b = f & 2;
        } else {
            b = f & 4;
        }
        if (q == o || (a && b)) {
            continue;
        }
        if ((flags & 6) && func_800384D0_find(q->unk6, back ? 3 : 2) == NULL) {
            continue;
        }
        if (q->unk5 != 4) {
            continue;
        }
        {
            Unk8004AB64 *p = q;
        if ((u32)(p->pos.vy - y + 399) >= 799) {
            continue;
        }
        dx = p->pos.vx - x;
        dz = p->pos.vz - z;
        if ((dx < 0 ? -dx : dx) >= 0x800) {
            continue;
        }
        if ((dz < 0 ? -dz : dz) >= 0x800) {
            continue;
        }
        if (back) {
            rr = p->unk3D * 4;
        } else {
            if (a) {
                rr = p->unk3C * 4;
            } else {
                rr = p->unk3C * 4;
            }
        }
        if (p->unk2D != 0) {
            s = math_cos(func_800531D0(p, o, p->pos.rotY));
            rr = (rr * func_80052EEC(((s * s) >> 4) * p->unk2D + 0x1000000)) >> 12;
        }
        rr += r;
        if (flags & 6) {
            rr += 0x3C + p->unk5C;
        }
        d = func_80052DEC(dx, dz, rr);
        if (back) {
            s = func_800531D0(o, p, o->pos.rotY);
            if (s < 0) {
                s = -s;
            }
            s -= 640;
            d += s;
            if (s >= 0) {
                continue;
            }
        }
        if (d < bestd) {
            bestd = d;
            best = p;
        }
        }
    }
    if (dist != NULL) {
        *dist = bestd;
    }
    return best;
}

s32 func_80038990(s32 x, s32 y, s32 z, s32 flags) {
    Unk8004AB64 *p;
    s32 dz;
    s32 dx;
    s32 dy;
    s32 speed;
    s16 v[4];
    s32 dist;
    s32 ry;
    s32 rx;
    s32 cur;
    s32 d;
    s32 step;
    s32 hit;
    s32 r;

    p = D_8007B704;
    speed = p->unk5C;
    dx = x - p->pos.vx;
    dz = z - p->pos.vz;
    if (p->unk50 & 0x20) {
        ry = p->pos.rotY & 0xFFF;
    } else {
        ry = func_80052F24(dx, dz);
        if (!(flags & 1)) {
            cur = p->pos.rotY & 0xFFF;
            d = ry - cur;
            if (d < -0x800) {
                d += 0x1000;
            } else if (d >= 0x800) {
                d -= 0x1000;
            }
            if (d == 0) {
                p->unk50 |= 0x20;
            }
            step = p->unk5F * 8;
            if (d > 0) {
                if (step < d) {
                    ry = cur + step;
                }
            } else {
                if (d < -step) {
                    ry = cur - step;
                }
            }
            ry &= 0xFFF;
            p->pos.rotY = ry;
        }
    }
    if (flags & 2) {
        dy = y - p->pos.vy;
        rx = func_80052F24(-dy, -func_80052EC0(dx, 0, dz)) & 0xFFF;
        cur = p->unk30 & 0xFFF;
        d = rx - cur;
        if (d < -0x800) {
            d += 0x1000;
        } else if (d >= 0x800) {
            d -= 0x1000;
        }
        step = p->unk5F * 8;
        if (d > 0) {
            if (step < d) {
                rx = cur + step;
            }
        } else {
            if (d < -step) {
                rx = cur - step;
            }
        }
        rx &= 0xFFF;
        p->unk30 = rx;
    } else {
        rx = 0;
        dy = 0;
    }
    func_8005309C(v, rx, ry, speed);
    p->pos.vx += v[0];
    p->pos.vy += v[1];
    p->pos.vz += v[2];
    hit = func_80038618(p, 0, &dist);
    if (hit != 0 && dist <= 0) {
        p->pos.vx -= v[0];
        p->pos.vy -= v[1];
        p->pos.vz -= v[2];
    }
    if (p->unk63 != 0 && p->unk63 != 0xFF) {
        p->unk63--;
    }
    r = p->unk63 != 0;
    if (flags < 0) {
        r = 0;
        if (hit != flags || dist > 0) {
            r = 1;
        }
    }
    dist = func_80052E48(dx, dy, dz);
    if (dist < speed * speed || ((p->unk50 & 0x10) && p->unk78 < dist)) {
        r = 0;
    }
    if (p->unk78 < 0x7FFFFFFF && dist < p->unk78) {
        p->unk50 |= 0x10;
    }
    p->unk78 = dist;
    if (r == 0) {
        p->unk78 = 0x7FFFFFFF;
        p->unk30 = 0;
        p->unk50 &= ~0x30;
    }
    if (p->unk7 == g_evtState->playerUid) {
        D_8007B710++;
    }
    return r;
}

s32 evt_rollEncounter(Unk8004AB64 *p) {
    s32 id = 0;

    if (g_evtState->unk16EC != 0 && g_evtEncounterDist > 0x3C0) {
        g_evtEncounterDist = 0;
        g_evtEncounterChance += g_evtState->unk16EF;
        if (evt_rollEncounter_rand() < (g_evtEncounterChance >> 3)) {
            g_evtEncounterChance = 0;
            id = evt_rollEncounter_pick();
            if (id == g_evtLastEncounter) {
                id = evt_rollEncounter_pick();
            }
            g_evtLastEncounter = id;
            evt_setNextBattle(id);
            g_gameState->unk1C->unk848 = -1;
        }
    }
    return id != 0;
}

/* screen position of an actor's speech-bubble anchor */
static inline void func_80038FD8_pos(Unk8004AB64 *obj, s32 *x, s32 *y) {
    SVECTOR sv;

    sv.vx = obj->pos.vx;
    sv.vy = obj->pos.vy + ((obj->unk32 * (s8)obj->unk2F) >> 6) - 50;
    sv.vz = obj->pos.vz;
    if (obj->unk5 == 4) {
        sv.vx += obj->unk8A;
        sv.vy += obj->unk8C;
        sv.vz += obj->unk8E;
    }
    func_8004BA20(&sv, x, y);
}

static inline void func_80038FD8_x(WinLayout *w, s32 sx, s32 right, s32 *ptx) {
    s32 width = w->unk2;
    s32 tx, l, r;

    if (right) {
        tx = sx - 6;
        if (tx < 32) {
            tx = 32;
        }
        if (tx > 304) {
            tx = 304;
        }
        l = tx + 8 - width;
        r = tx - 24;
    } else {
        tx = sx + 6;
        if (tx > 288) {
            tx = 288;
        }
        if (tx < 16) {
            tx = 16;
        }
        l = tx + 24 - width;
        r = tx - 8;
    }
    *ptx = tx;
    if (l < 8) {
        l = 8;
    }
    if (r > 312 - width) {
        r = 312 - width;
    }
    w->unk0 = (l + r) >> 1;
}

static inline void func_80038FD8_y(WinLayout *w, s32 sy, s32 *below, s32 fixed) {
    s32 h = w->unk9;
    s32 ty;

    if (*below) {
        ty = sy - h - 16;
        if (!fixed && ty < 8) {
            *below ^= 1;
            ty = sy + 32;
        }
    } else {
        ty = sy + 32;
        if (!fixed && ty > 212 - h) {
            *below = 1;
            ty = sy - h - 16;
        }
    }
    if (ty < 8) {
        ty = 8;
    } else if (ty > 212 - h) {
        ty = 212 - h;
    }
    w->unk8 = ty;
}

void func_80038FD8(Unk8004AB64 *obj, WinLayout *w, s32 dir, s32 fixed) {
    s32 sx, sy;
    s32 ox, oy;
    s32 tx;
    s32 below;
    s32 right;
    s32 id;
    Unk8004AB64 *other;

    if (obj->unk5 == 4) {
        obj->unk8A = D_8007AE5C;
        obj->unk8C = D_8007AE60;
        obj->unk8E = D_8007AE64;
    }
    func_80038FD8_pos(obj, &sx, &sy);
    if (dir < 0) {
        if (obj->unk7 == g_evtState->playerUid) {
            id = obj->unk93;
        } else {
            id = g_evtState->playerUid;
        }
        other = func_8004B040_inline(id);
        if (other != NULL) {
            func_80038FD8_pos(other, &ox, &oy);
            below = sy < oy;
            right = sx < ox;
        } else {
            below = 1;
            right = 0;
        }
    } else {
        below = (dir >> 1) & 1;
        right = dir & 1;
    }
    if (fixed == 0) {
        right = right ? right ^ (sx < 40) : sx > 280;
    }
    func_80038FD8_x(w, sx, right, &tx);
    func_80038FD8_y(w, sy, &below, fixed);
    if (fixed == 0 && func_8004EBC0((Unk8004EBC0 *)w)) {
        below ^= 1;
        func_80038FD8_y(w, sy, &below, 0);
    }
    w->unk4 = tx - w->unk0;
    w->unk6 = (below << 1) | right;
}

/* bubble x/y targets for func_80039444 (func_80038FD8_x/_y without the flip logic) */
static inline s32 func_80039444_x(s32 width, s32 sx, s32 right, s32 *ptx) {
    s32 tx, l, r;

    if (right) {
        tx = sx - 6;
        if (tx < 32) {
            tx = 32;
        }
        if (tx > 304) {
            tx = 304;
        }
        l = tx + 8 - width;
        r = tx - 24;
    } else {
        tx = sx + 6;
        if (tx > 288) {
            tx = 288;
        }
        if (tx < 16) {
            tx = 16;
        }
        l = tx + 24 - width;
        r = tx - 8;
    }
    *ptx = tx;
    if (l < 8) {
        l = 8;
    }
    if (r > 312 - width) {
        r = 312 - width;
    }
    return (l + r) >> 1;
}

static inline s32 func_80039444_y(s32 h, s32 *sy, s32 below) {
    s32 ty;

    if (below) {
        ty = *sy - h - 16;
    } else {
        ty = *sy + 32;
    }
    if (ty < 8) {
        ty = 8;
    } else if (ty > 212 - h) {
        ty = 212 - h;
    }
    return ty;
}

void func_80039444(Unk8004AB64 *obj, Window *w) {
    s32 sx, sy;
    s32 tx;
    s32 ox, oy;
    s32 right, below;
    s32 x, y;

    ox = w->unk0;
    oy = w->unk2;
    func_80038FD8_pos(obj, &sx, &sy);
    below = (w->unk29 >> 1) & 1;
    right = w->unk29 & 1;
    x = func_80039444_x(w->unk4 - 3, sx, right, &tx);
    y = func_80039444_y(w->unk6 - 3, &sy, below);
    w->unk0 = (ox + x) >> 1;
    w->unk2 = (oy + y) >> 1;
    w->unk2A = tx - w->unk0 - right * 16;
    w->unk8.x += w->unk0 - ox;
    w->unk8.y += w->unk2 - oy;
}

WinLayout *func_80039660(Unk8004AB64 *obj, s32 id, s32 flags) {
    WinLayout *w = func_8004E248(id, &D_8007AE5C, &D_8007AE60, &D_8007AE64);
    s32 ww;
    s32 wh;

    w->unk4 = 0;
    w->unk6 = 0;
    if ((flags & 0x40) && w->unk2 < 0x72) {
        w->unk2 = 0x72;
    }
    if (flags & 0x80) {
        if (w->unk2 < 0x1D) {
            w->unk2 = 0x1D;
        }
        switch (w->unkA) {
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
            func_80038FD8(obj, w, w->unkA & 3, 0);
            break;
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
            func_80038FD8(obj, w, w->unkA & 3, 1);
            break;
        default:
            func_80038FD8(obj, w, -1, 0);
            break;
        }
    } else {
        ww = w->unk2;
        wh = w->unk9;
        switch (w->unkA) {
        case 0x53:
            w->unk0 = 12;
            w->unk8 = 12;
            break;
        case 0x55:
            w->unk0 = 160 - (ww >> 1);
            w->unk8 = 12;
            break;
        case 0x52:
            w->unk0 = 308 - ww;
            w->unk8 = 12;
            break;
        case 0x5C:
            w->unk0 = 160 - (ww >> 1);
            w->unk8 = 74 - (wh >> 1);
            break;
        case 0x51:
            w->unk0 = 12;
            w->unk8 = -48 - wh;
            break;
        case 0x54:
            w->unk0 = 160 - (ww >> 1);
            w->unk8 = -48 - wh;
            break;
        case 0x50:
            w->unk0 = 308 - ww;
            w->unk8 = -48 - wh;
            break;
        case 0:
            w->unk0 = 160 - (ww >> 1);
            w->unk8 = 110 - (wh >> 1);
            break;
        }
    }
    if ((flags & 4) && w->unk2 < 0x1D) {
        w->unk2 = 0x1D;
    }
    w->unk6 |= flags & 0x30;
    return w;
}

void func_800398D0(Unk8004AB64 *p, s32 d, s32 wrap, s32 speed) {
    s32 cur;
    s32 rate;
    s32 frames;
    s16 a;

    p->unk52 = d & 0xFFF;
    cur = p->pos.rotY & 0xFFF;
    d -= cur;
    if (wrap) {
        if (d > 0x800) {
            d -= 0x1000;
        } else if (d < -0x800) {
            d += 0x1000;
        }
    }
    if (d == 0) {
        return;
    }
    p->unk60 = 0;
    p->unk61 = 0xFF;
    p->unk80 = cur;
    p->unk5A = p->unk36;
    p->unkF |= 0x80;
    if (speed == 0) {
        speed = 0x10;
    }
    if (d < 0) {
        frames = evt_getAnimFrameCount(p->unk34, (u16)p->unk40.unk20);
        rate = frames * 16 / speed;
        if (-d < 0x400) {
            rate = rate * -d >> 10;
        }
        if (rate >= 6) {
            p->unk6C = (d + 0x400) / rate;
            p->unk62 = 0;
            evt_execInstr_anim(p, (u16)p->unk40.unk20);
            if (p->unk7 == g_evtState->playerUid) {
                D_8007B710++;
            }
            p->unk5D = frames * 16 / rate;
        } else {
            p->unk6C = 0x7FFE;
            a = cur;
            if (p->unk52 >= cur) {
                a = cur | 0x1000;
            }
            p->pos.rotY = a;
        }
    } else {
        frames = evt_getAnimFrameCount(p->unk34, (u16)p->unk40.rotY);
        rate = frames * 16 / speed;
        if (d < 0x400) {
            rate = rate * d >> 10;
        }
        if (rate >= 6) {
            p->unk6C = (d - 0x400) / rate;
            p->unk62 = 0;
            evt_execInstr_anim(p, (u16)p->unk40.rotY);
            if (p->unk7 == g_evtState->playerUid) {
                D_8007B710++;
            }
            p->unk5D = frames * 16 / rate;
        } else {
            p->unk6C = 0x7FFF;
            a = cur;
            if (cur >= p->unk52) {
                a = cur | 0xF000;
            }
            p->pos.rotY = a;
        }
    }
    if (rate <= 0) {
        rate = 1;
    }
    p->unk82 = d / rate;
}

s32 evt_execInstr(void) {
    Unk8004AB64 *po = D_8007B704;
    Unk8004AB64 *actor = po;
    Unk8004AB64 *obj;
    Unk8004AB64 *o;
    Unk16D8 *n;
    s32 op;
    s32 a, b, c, d, e, f, i;
    SVECTOR sv;
    Unk80039C38Pt *pt;
    Unk8004AFF0 *m, *m2;
    u8 *str;

    g_evtInstrStart = g_evtCurObj->unk0;
    op = *g_evtCurObj->unk0++;
    g_evtArgFlags = *g_evtCurObj->unk0++;
    D_8007B6F8 = 0;

    switch (op) {
    case 0x07:
        func_8004B3A8_inline(g_evtArgFlags, *g_evtCurObj->unk0++);
        D_8007B6F8 = 1;
        return 0;
    case 0x08:
        func_8004B3D0_inline(g_evtArgFlags, *g_evtCurObj->unk0++);
        D_8007B6F8 = 1;
        return 0;
    case 0x09:
        a = g_evtArgFlags;
        b = *g_evtCurObj->unk0++;
        if ((u32)(a - 0xFB) < 4) {
            a = g_evtState->unk16E4[a - 0xFB];
        }
        evt_execInstr_newactor(a, b);
        D_8007B6F8 = 1;
        return 0;
    case 0x46:
    case 0x79:
    case 0xD2:
    case 0xD3:
        return 0;
    case 0x10:
        a = evt_getArg8_inline();
        func_800384D0(func_80038150(), a, *g_evtCurObj->unk0++, 0);
        return 0;
    case 0x12:
        a = evt_getArg8_inline();
        obj = func_80038150();
        c = *g_evtCurObj->unk0++;
        if (obj != NULL && a < obj->unk4) {
            func_800384D0(obj, a, c, 0);
            return 0;
        }
        g_evtCurObj->unk0 = g_evtInstrStart;
        D_8007B6F8 = 1;
        return 1;
    case 0x14:
        a = evt_getArg8_inline();
        obj = func_80038150();
        c = *g_evtCurObj->unk0++;
        if (obj != NULL && a < obj->unk4) {
            func_800384D0(obj, a, c, 1);
        } else {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x16:
        a = evt_getArg8_inline();
        func_800384D0(func_8004B040_inline(CUR_UID()), a, *g_evtCurObj->unk0++, 0);
        return 0;
    case 0x18:
        a = evt_getArg8_inline();
        if (func_8004B040_inline(CUR_UID()) != NULL && a < func_8004B040_inline(CUR_UID())->unk4) {
            func_800384D0(func_8004B040_inline(CUR_UID()), a, *g_evtCurObj->unk0++, 0);
            return 0;
        }
        g_evtCurObj->unk0 = g_evtInstrStart;
        D_8007B6F8 = 1;
        return 1;
    case 0x1A:
        a = evt_getArg8_inline();
        if (func_8004B040_inline(CUR_UID()) != NULL && a < func_8004B040_inline(CUR_UID())->unk4) {
            func_800384D0(func_8004B040_inline(CUR_UID()), a, *g_evtCurObj->unk0++, 1);
        } else {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x97:
        obj = func_80038150();
        if (obj != NULL) {
            obj->unkA = 0;
            obj->unkE = 2;
            func_80038278(obj);
        }
        return obj == g_evtCurObj;
    case 0x93:
        D_8007B704->unkF &= 0xC0;
        D_8007B704->unkF |= evt_getArg8_inline() & 0x3F;
        return 0;
    case 0x6A:
        g_evtState->unk16F8 = 1;
        return 0;
    case 0xF0:
        g_evtState->unk16F8 = 0;
        return 0;
    case 0xD5:
        for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
            Unk8004AB64 *o = n->unk4;
            if (o->unk5 == 4) {
                o->unk2C = o->unkF;
                if (!(o->unkF & 0x20)) {
                    o->unkF &= 0xFE;
                }
            }
        }
        return 0;
    case 0xD6:
        for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
            Unk8004AB64 *o = n->unk4;
            if (o->unk5 == 4) {
                o->unkF &= 0xFE;
                o->unkF |= o->unk2C & 1;
            }
        }
        return 0;
    case 0x4B:
        if (D_8007B704->unk5 == 4) {
            a = evt_getArg8_inline();
            if (g_evtMode == 1 && po->unk34 != 0xFFFF) {
                func_800C7570(po->unk3B, (a << 2) & 0xFFFC);
            }
            po->unk3C = evt_getArg8_inline();
            po->unk3D = evt_getArg8_inline();
        } else {
            evt_getArg8_inline();
            evt_getArg8_inline();
            evt_getArg8_inline();
        }
        return 0;
    case 0xA3:
        evt_getArg8_inline();
        evt_getArg8_inline();
        evt_getArg8_inline();
        evt_getArg8_inline();
        return 0;
    case 0xDF:
        po->unk2D = evt_getArg8_inline();
        return 0;
    case 0x3B:
        g_evtCurObj->unk11 = evt_getArg8_inline();
        return 0;
    case 0xBF:
        po = func_80038150();
    case 0x1D:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = g_evtMode == 1 && po->unk34 != 0xFFFF;
        if (c) {
            func_800C3E18(po->unk3B, 0);
        }
        po->pos.vx = po->unk40.vx = a;
        po->pos.vy = po->unk40.vy = -0x8000;
        po->pos.vz = po->unk40.vz = b;
        if (c) {
            func_800C3E18(po->unk3B, 1);
        }
        if (po->unk5 == 4) {
            po->unk78 = 0x7FFFFFFF;
            po->unk30 = 0;
            po->unk50 &= 0xFFCF;
        }
        D_8007AEA4 = 1;
        return 0;
    case 0xAD:
        po = func_80038150();
    case 0xA1:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        d = g_evtMode == 1 && po->unk34 != 0xFFFF;
        if (d) {
            func_800C3E18(po->unk3B, 0);
        }
        po->pos.vx = po->unk40.vx = a;
        po->pos.vy = po->unk40.vy = b;
        po->pos.vz = po->unk40.vz = c;
        if (po->unk5 == 4) {
            po->unk78 = 0x7FFFFFFF;
            po->unk30 = 0;
            po->unk50 &= 0xFFCF;
        }
        return 0;
    case 0x87:
        po = func_80038150();
    case 0x36:
        po->pos.rotY = evt_getArg8_inline() << 4;
        return 0;
    case 0x37:
        po->pos.unk20 = evt_getArg8_inline() << 4;
        po->unk24 = evt_getArg8_inline() << 4;
        return 0;
    case 0x8B:
        actor->unk6A = evt_getArg8_inline();
        actor->unk6B = evt_getArg8_inline();
        actor->unk50 |= 3;
        return 0;
    case 0x47:
        actor->unk50 &= 0xFFF8;
        actor->unk50 |= evt_getArg8_inline() & 7;
        return 0;
    case 0x91:
        actor->unk6E = evt_getArg8_inline() ? 0xA7 : 4;
        return 0;
    case 0x99:
        actor->unk73 = evt_getArg8_inline();
        if (actor->unk73 == 0) {
            actor->unk73 = 0x10;
        }
        return 0;
    case 0xBB:
        actor = func_80038150();
    case 0x56:
        a = evt_getArg8_inline() << 4;
        func_800398D0(actor, a, 1, evt_getArg8_inline());
        return 0;
    case 0xA7:
        func_800398D0(actor, evt_getArg8_inline() << 4, 1, actor->unk73);
        return 0;
    case 0x9B:
        a = evt_getArg16_inline() - actor->pos.vx;
        b = evt_getArg16_inline() - actor->pos.vz;
        if (a | b) {
            func_800398D0(actor, func_80052F24(a, b), 1, actor->unk73);
        }
        return 0;
    case 0x51:
        obj = func_80038150();
        a = evt_getArg8_inline();
        if (obj != NULL) {
            func_800398D0(actor, func_80052F24(obj->pos.vx - actor->pos.vx, obj->pos.vz - actor->pos.vz), 1, a);
        }
        return 0;
    case 0xBC:
        actor = func_80038150();
    case 0x50:
        if (actor->unkF & 0x80) {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x66:
        a = evt_getArg8_inline();
        func_8004EC74(a, evt_getArg16_inline());
        return 0;
    case 0xE3:
        D_8007BBD8 = evt_getArg8_inline();
        return 0;
    case 0x7C:
        a = evt_getArg16_inline();
        func_8004ED00(a, evt_getArg8_inline());
        return 0;
    case 0x53:
        func_8004EFA0();
        return 0;
    case 0x1F:
        D_8007B704->unk10 = evt_getArg8_inline();
        c = evt_getArg8_inline();
        evt_execInstr_window(D_8007B704, c, D_8007B704->unk10);
        b = evt_getArg16_inline();
        msg_open(b, D_8007B704->unk10, func_80039660(D_8007B704, b, c), c);
        D_8007B704->unkD = 0xFE;
        return 1;
    case 0x20:
        D_8007B704->unk10 = evt_getArg8_inline();
        c = evt_getArg8_inline();
        evt_execInstr_window(D_8007B704, c, D_8007B704->unk10);
        b = evt_getArg16_inline();
        msg_open(b, D_8007B704->unk10, func_80039660(D_8007B704, b, c), c);
        return 0;
    case 0x95:
        po = func_80038150();
        D_8007B704->unk10 = evt_getArg8_inline();
        c = evt_getArg8_inline();
        evt_execInstr_window(po, c, D_8007B704->unk10);
        b = evt_getArg16_inline();
        msg_open(b, D_8007B704->unk10, func_80039660(po, b, c), c);
        D_8007B704->unkD = 0xFE;
        return 1;
    case 0x96:
        po = func_80038150();
        D_8007B704->unk10 = evt_getArg8_inline();
        c = evt_getArg8_inline();
        evt_execInstr_window(po, c, D_8007B704->unk10);
        b = evt_getArg16_inline();
        msg_open(b, D_8007B704->unk10, func_80039660(po, b, c), c);
        return 0;
    case 0x21:
        a = evt_getArg8_inline();
        func_8004EA4C(a);
        return 0;
    case 0xEB:
        func_8004E924();
        win_closeAll();
        return 0;
    case 0x54:
        D_8007B704->unk10 = evt_getArg8_inline();
        D_8007B704->unkD = 0xFE;
        return 1;
    case 0x8E:
        func_8004ED3C();
        return 0;
    case 0xD0:
        btl_setBanner((s32)func_8004F058(evt_getArg16_inline()), 3);
        return 0;
    case 0x25:
        actor->unk63 = 0xFF;
        actor->unk78 = 0x7FFFFFFF;
        actor->unk30 = 0;
        actor->unk50 &= 0xFFCF;
        return 0;
    case 0x98:
        actor->unk63 = evt_getArg8_inline();
        return 0;
    case 0x23:
        a = evt_getArg16_inline();
        if (func_80038990(a, 0, evt_getArg16_inline(), 0)) {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0xA5:
        a = evt_getArg16_inline();
        if (func_80038990(a, 0, evt_getArg16_inline(), 1)) {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x24:
        po = func_80038150();
        if (func_80038990(po->pos.vx, po->pos.vy, po->pos.vz, (s32)po)) {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
        }
        D_8007B6F8 = 1;
        return 1;
    case 0xA0:
        if (func_80038990(D_8007AE24, 0, D_8007AE28, 0)) {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0xA2:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        if (func_80038990(a, b, c, 2)) {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0xE8:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        if (func_80038990(a, b, c, 3)) {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
        }
        return 1;
    case 0x55:
        a = evt_getArg8_inline();
        if (a == 0) {
            a = 0xFF;
        }
        actor->unk5F = a;
        return 0;
    case 0x26:
        actor->unk5C = evt_getArg8_inline();
        return 0;
    case 0xA6:
        actor->unk5E = evt_getArg8_inline();
        return 0;
    case 0x9C:
        actor->unk62 = 8;
        actor->unk60 = 0;
        actor->unk61 = actor->unk90 - 1;
        evt_execInstr_anim(actor, actor->unk76);
        if (actor->unk7 == g_evtState->playerUid) {
            D_8007B710++;
        }
        return 0;
    case 0x9D:
        ((Unk8E0 *)func_8001E350(actor->unk3B))->unk0 &= ~0x10;
        actor->unk61 = 0xFF;
        actor->unk62 = 0;
        actor->unk60 = actor->unk91 + 1;
        evt_execInstr_anim(actor, actor->unk76);
        if (actor->unk7 == g_evtState->playerUid) {
            D_8007B710++;
        }
        return 0;
    case 0xE2:
        actor->unk94 = evt_getArg16_inline();
        actor->unkAA = evt_getArg16_inline();
        actor->unk96 = evt_getArg16_inline();
        a = evt_getArg8_inline();
        if (a == 0) {
            a = 8;
        }
        actor->unk50 |= 8;
        ((Unk8E0 *)func_8001E350(actor->unk3B))->unk0 |= 0x10;
        actor->unk60 = actor->unk90;
        actor->unk61 = actor->unk91;
        evt_execInstr_anim(actor, actor->unk76);
        if (actor->unk7 == g_evtState->playerUid) {
            D_8007B710++;
        }
        actor->unk68 = 0;
        actor->unk92 = a;
        actor->unkA4 = actor->pos.vx;
        actor->unkA6 = actor->pos.vy;
        actor->unkA8 = actor->pos.vz;
        actor->unk5D = ((actor->unk61 - actor->unk60) << 4) / a;
        if (g_evtMode == 1 && po->unk34 != 0xFFFF) {
            func_800C3E18(po->unk3B, 0);
        }
        return 0;
    case 0xDC:
        b = actor->unk92;
        a = ++actor->unk68;
        actor->pos.vx = ((s16)actor->unkA4 * (b - a) + actor->unk94 * a) / b;
        actor->pos.vy = (s16)actor->unkA6 + a * a * 8 + a * (actor->unkAA - (s16)actor->unkA6) / b - a * (b << 3);
        actor->pos.vz = ((s16)actor->unkA8 * (b - a) + actor->unk96 * a) / b;
        if (a >= b) {
            return 0;
        }
        g_evtCurObj->unk0 = g_evtInstrStart;
        D_8007B6F8 = 1;
        return 1;
    case 0x29: {
        u32 cnt;
        pt = ((Unk80039C38Quad *)D_8007B704)->unk18;
        cnt = ((Unk80039C38Quad *)D_8007B704)->unk14 = evt_getArg8_inline();
        while (cnt-- != 0) {
            pt->unk0 = evt_getArg16_inline();
            pt->unk2 = evt_getArg16_inline();
            pt++;
        }
        return 0;
    }
    case 0xA4:
        po = func_8004B040_inline(g_evtState->playerUid);
        if (po != NULL) {
            Unk80039C38Pt *p = ((Unk80039C38Quad *)D_8007B704)->unk18;
            s32 dx = p[1].unk0 - p[0].unk0;
            s32 dz = p[1].unk2 - p[0].unk2;
            s32 t = (dx * dx + dz * dz) >> 8;
            if (t != 0) {
                t = (dx * (po->pos.vx - p[0].unk0) + dz * (po->pos.vz - p[0].unk2)) / t;
                if (t < 0) {
                    t = 0;
                } else if (t > 0x100) {
                    t = 0x100;
                }
            }
            D_8007AE24 = ((t * dx) >> 8) + p[0].unk0;
            D_8007AE28 = ((t * dz) >> 8) + p[0].unk2;
        } else {
            D_8007AE28 = 0;
            D_8007AE24 = 0;
        }
        return 0;
    case 0x9E:
        po = func_8004B040_inline(g_evtState->playerUid);
        if (po != NULL) {
            s32 *sp = (s32 *)po + po->unk9 + po->unkA;
            Unk16D8 *nn;
            sp[0] = po->unk0 - g_evtState->unk16F0;
            sp[1] = po->unkD | po->unk4 << 8 | 0xFF << 16 | g_evtCurObj->unk7 << 24;
            po->unk4 = 0;
            po->unkD = 0;
            po->unkA += 2;
            po->unk0 = D_8006E658;
            g_evtState->unk16EC = 0;
            for (nn = g_evtState->unk16D8; nn != NULL; nn = nn->unk0) {
                nn->unk4->unkF |= 6;
            }
        }
        return 0;
    case 0x76: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)D_8007B704;
        q->unk14 = 2;
        q->unk18[0].unk0 = q->unk18[1].unk0 = evt_getArg16_inline();
        q->unk18[0].unk2 = q->unk18[1].unk2 = evt_getArg16_inline();
        return 0;
    }
    case 0x77: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)D_8007B704;
        Unk80039C38Pt *p;
        a = q->unk14;
        p = &q->unk18[(a > 0 ? a : 1) - 1];
        p->unk0 = evt_getArg16_inline();
        p->unk2 = evt_getArg16_inline();
        return 0;
    }
    case 0x78: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)D_8007B704;
        s32 k = q->unk14;
        if (k >= 1 && k < 8) {
            q->unk18[k] = q->unk18[k - 1];
            q->unk14++;
        }
        return 0;
    }
    case 0x2B:
        evt_setNextLocation(evt_getArg16_inline());
        return 4;
    case 0xB6:
        evt_setNextLocation(evt_getArg16_inline());
        return 5;
    case 0xAE:
        func_80021680(evt_getArg16_inline() & 0xFFFF);
        return 7;
    case EVT_OP_GAME_OVER:
        return 8;
    case 0x2A:
        D_8007BD2C = evt_getArg8_inline();
        g_gameState->unk1C->unk848 = -1;
        a = evt_getArg16_inline();
        g_gameState->unk1C->unk571 = (a >> 15) & 1;
        evt_setNextBattle(a & 0x7FFF);
        return 3;
    case 0x8C:
        D_8007BD2C = evt_getArg8_inline();
        g_gameState->unk1C->unk848 = evt_getArg8_inline();
        a = evt_getArg16_inline();
        g_gameState->unk1C->unk571 = (a >> 15) & 1;
        evt_setNextBattle(a & 0x7FFF);
        return 3;
    case 0x57:
        if ((g_evtState->unk16EF = evt_getArg8_inline()) == 0) {
            g_evtEncounterChance = 0;
        }
        return 0;
    case 0x3C:
        g_evtEncounterTable.unk8 = evt_getArg8_inline();
        g_evtEncounterTable.unk0 = evt_getArg16_inline();
        g_evtEncounterTable.unk2 = evt_getArg16_inline();
        g_evtEncounterTable.unk4 = evt_getArg16_inline();
        g_evtEncounterTable.unk6 = evt_getArg16_inline();
        return 0;
    case 0x2C:
        g_evtState->playerUid = g_evtCurObj->unk7;
        return 0;
    case 0x2D:
        g_evtState->unk16EC = 0;
        func_8004FF98(0);
        D_8007B710 = 0;
        D_8007BBC8 = 0;
        return 0;
    case 0x2E: {
        Unk8007AECC *e;
        g_evtState->unk16EC = 1;
        func_8004FF98(1);
        e = g_evtState;
        if (e->unk1700 >= 0) {
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            e->unk1700 = (*g_randPtr++ + 0xC8) * 4;
        }
        return 0;
    }
    case 0x2F:
        po->unk34 = evt_getArg16_inline();
        po->unk3B = evt_createModel(g_evtCurObj);
        g_evtCurObj->unkF |= 1;
        po->unk32 = -(evt_getArg8_inline() << 2);
        return 0;
    case 0xD4:
        m = func_8004AFF0_inline(D_8007B704);
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        if (m != NULL) {
            m->unk74 = a;
            m->unk76 = b;
            m->unk78 = c;
        }
        return 0;
    case 0x4C:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        o = func_80038150();
        m2 = func_8004AFF0_inline(o);
        a = evt_getArg8_inline();
        if (m != NULL && m2 != NULL) {
            m->unk1C = m2;
            m->unk4 = a;
        }
        return 0;
    case 0x4D:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        if (m != NULL) {
            m->unk1C = NULL;
        }
        return 0;
    case 0x9F:
        po = func_80038150();
        m = func_8004AFF0_inline(po);
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg8_inline();
        if (m != NULL) {
            m->unk54 = (s8)a << 6;
            m->unk56 = (s8)b << 6;
            m->unk58 = (s8)c << 6;
            m->unk0 |= 2;
        }
        po->unk2F = b;
        return 0;
    case 0xC0:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        b = evt_getArg8_inline();
        if (m != NULL) {
            m->unk50->unk0[b].unk4 |= 1;
            m->unk50->unk0[b].unkC = 0;
            m->unk50->unk0[b].unk12 = 0x1000;
        }
        return 0;
    case 0xC1:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        b = evt_getArg8_inline();
        if (m != NULL) {
            m->unk50->unk0[b].unk4 |= 3;
            m->unk50->unk0[b].unkC = 0;
            m->unk50->unk0[b].unk12 = 0x1000;
        }
        return 0;
    case 0xC2:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        b = evt_getArg8_inline();
        if (m != NULL) {
            m->unk50->unk0[b].unk4 &= 0xFC;
        }
        return 0;
    case 0xCC: {
        ((Unk8E0 *)func_8001E350(po->unk3B))->unk0 |= evt_getArg16_inline();
        return 0;
    }
    case 0xCD: {
        ((Unk8E0 *)func_8001E350(po->unk3B))->unk0 &= ~evt_getArg16_inline();
        return 0;
    }
    case 0x4E:
        str = g_evtCurObj->unk0 - 1;
        while (*str++ != 0) {
        }
        func_80052D0C(&str);
        D_8007B6F8 = 1;
        g_evtCurObj->unk0 = str + 1;
        return 0;
    case 0x4F:
        D_8007B6F8 = 1;
        g_evtCurObj->unk0++;
        return 6;
    case 0x33:
        actor->unk54 = evt_getArg16_inline();
        return 0;
    case 0x34:
        actor->unk56 = evt_getArg16_inline();
        return 0;
    case 0x35:
        actor->unk58 = evt_getArg16_inline();
        return 0;
    case 0x7A:
        actor->unk40.unk20 = evt_getArg16_inline();
        return 0;
    case 0x7B:
        actor->unk40.rotY = evt_getArg16_inline();
        return 0;
    case 0x52:
        actor->unk74 = evt_getArg16_inline();
        return 0;
    case 0x94:
        actor->unk76 = evt_getArg16_inline();
        actor->unk90 = evt_getArg8_inline();
        actor->unk91 = evt_getArg8_inline();
        return 0;
    case 0x3F:
        actor->unk62 = (evt_getArg8_inline() << 3) & 0x38;
        actor->unk63 = evt_getArg8_inline();
        return 0;
    case 0x3E:
        actor->unk69 = evt_getArg8_inline();
        return 0;
    case 0x86:
        actor->unk7C.b[0] = evt_getArg8_inline();
        actor->unk7C.b[1] = evt_getArg8_inline();
        actor->unk7C.b[2] = evt_getArg8_inline();
        actor->unk7C.b[3] = evt_getArg8_inline();
        return 0;
    case 0x3D:
        actor->unk60 = evt_getArg8_inline();
        actor->unk61 = evt_getArg8_inline();
        return 0;
    case 0xBD:
        actor = func_80038150();
    case 0x40:
        evt_execInstr_anim(actor, evt_getArg16_inline());
        if (actor->unk7 == g_evtState->playerUid) {
            D_8007B710++;
        }
        return 0;
    case 0xBE:
        actor = func_80038150();
    case 0x41:
        if (!(actor->unk62 & 0x80)) {
            return 0;
        }
        g_evtCurObj->unk0 = g_evtInstrStart;
        D_8007B6F8 = 1;
        return 1;
    case 0x42:
        func_800499D8((Unk8004B9B8 *)actor);
        return 0;
    case 0xB5:
        obj = func_80038150();
        if (obj != NULL) {
            if (obj != po) {
                actor->unkA0 = obj;
                actor->unk62 = 0x80;
            } else {
                actor->unkA0 = NULL;
                actor->unk62 = 0;
            }
        }
        return 0;
    case 0x43: {
        Unk8004AB64 *o;
        a = g_evtCurObj->unk7 + 0x40;
        o = func_8004B040_inline(a);
        if (o != NULL) {
            func_80036978(o);
        }
        func_800367D0(evt_getArg8_inline(), a, 0x14, 0)->unk5 = 1;
        return 0;
    }
    case 0x44:
        if (func_8004B040_inline(g_evtCurObj->unk7 + 0x40) != NULL) {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
            return 1;
        }
        return 0;
    case 0x45: {
        Unk8004AB64 *o = func_8004B040_inline(g_evtCurObj->unk7 + 0x40);
        if (o != NULL) {
            func_80036978(o);
        }
        return 0;
    }
    case 0x38:
        switch (g_evtCurObj->unk4) {
        case 0: {
            s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
            btl_setEnemyCmd(x, z, 0x36, evt_getArg8_inline());
            break;
        }
        case 1: {
            s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
            btl_setEnemyCmd(x, z, 0x35, evt_getArg8_inline());
            break;
        }
        case 3: {
            s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
            btl_setEnemyCmd(x, z, 0x2F, evt_getArg8_inline());
            g_evtCurObj->unkB = 0;
            break;
        }
        }
        return 0;
    case 0xE5: {
        s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
        btlseq_startDirect(x, z, evt_getArg8_inline());
        return 0;
    }
    case 0x4A:
        a = evt_getArg8_inline();
        btl_setBattleVar(a, evt_getArg16_inline());
        return 0;
    case 0x1B:
        if (evt_getArg8_inline()) {
            g_gameState->unk1C->unk56F |= 8;
        } else {
            g_gameState->unk1C->unk56F &= 0xF7;
        }
        return 0;
    case 0xE1:
        func_801F0FF4();
        D_8007B718 = 1;
        return 0;
    case 0xDE:
        a = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)a < 9) {
            u8 *src = func_8004F058(evt_getArg16_inline());
            u8 *dst = g_gameState->unk1C->characters[a].unk0;
            u8 *end = dst + 9;
            while (dst < end) {
                *dst++ = *src++;
            }
            *dst = 0xFF;
        } else {
            evt_getArg16_inline();
        }
        return 0;
    case 0xDB:
        a = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)a < 9) {
            g_gameState->unk1C->characters[a].unk34.w = (g_gameState->unk1C->characters[a].unk34.w & ~0x400) | ((evt_getArg8_inline() & 1) << 10);
        }
        return 0;
    case 0x67:
        g_evtState->unk16E8 = evt_getArg8_inline() << 4;
        g_evtState->unk16EA = evt_getArg8_inline() << 4;
        return 0;
    case 0x6B:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        game_setClearColor(a, b, evt_getArg8_inline());
        return 0;
    case 0x58:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        func_800C0DB0(a, b, evt_getArg16_inline());
        return 0;
    case 0x6F:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg8_inline();
        func_800C0DF4(a, b, c, evt_getArg8_inline());
        return 0;
    case 0x70:
        a = evt_getArg8_inline();
        func_800C0038(a, evt_getArg8_inline());
        return 0;
    case 0x71:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        func_800C0E94(a, b, evt_getArg8_inline());
        return 0;
    case 0x72:
        a = evt_getArg16_inline();
        func_800C1024(a);
        return 0;
    case 0x73:
        func_800C1100();
        return 0;
    case 0x74:
        func_800C104C();
        return 0;
    case 0x59:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg8_inline();
        func_800C1368(a, b, c, evt_getArg8_inline());
        return 0;
    case 0x5A:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        func_800C11E8(a, b, c, evt_getArg16_inline());
        return 0;
    case 0x5B:
        a = evt_getArg8_inline();
        func_800C142C(a, evt_getArg8_inline());
        return 0;
    case 0x5C:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg16_inline();
        func_800C147C(a, b, c, evt_getArg16_inline());
        return 0;
    case 0x5D:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg16_inline();
        func_800C15BC(a, b, c, evt_getArg16_inline());
        return 0;
    case 0x5E:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        func_800C1614(a, b, evt_getArg16_inline());
        return 0;
    case 0x92:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg8_inline();
        f = evt_getArg8_inline();
        {
            s32 g = evt_getArg8_inline();
            e = evt_getArg8_inline();
            func_800C02D0(a, b, c, f, g, e, evt_getArg8_inline());
        }
        return 0;
    case 0xE6:
        a = evt_getArg8_inline();
        func_800C14DC(a, evt_getArg8_inline());
        return 0;
    case 0x5F:
        a = evt_getArg8_inline();
        func_800C16B0(a, evt_getArg8_inline());
        return 0;
    case 0x60:
        a = evt_getArg8_inline();
        func_800C17AC(a, evt_getArg8_inline());
        return 0;
    case 0x61:
        a = evt_getArg8_inline();
        func_800C17F0(a, evt_getArg16_inline());
        return 0;
    case 0x63:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        func_800C1868(a, b, evt_getArg8_inline());
        return 0;
    case 0x64:
        a = evt_getArg8_inline();
        func_800C18A0(a, evt_getArg8_inline());
        return 0;
    case 0x65:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        func_800C18D4(a, b, evt_getArg8_inline());
        return 0;
    case 0xCA:
        a = evt_getArg8_inline();
        func_800C196C(a, evt_getArg8_inline());
        return 0;
    case 0xE7:
        a = evt_getArg8_inline();
        func_800C16F4(a, evt_getArg8_inline());
        return 0;
    case 0x9A:
        a = evt_getArg16_inline();
        func_800C778C(a, evt_getArg8_inline());
        return 0;
    case 0xCB:
        a = evt_getArg8_inline();
        func_800C47E4(a, evt_getArg8_inline());
        return 0;
    case 0xA8:
        a = evt_getArg8_inline();
        if (po->unk34 != 0xFFFF) {
            func_800C3E18(po->unk3B, a);
        }
        return 0;
    case 0x27:
        a = evt_getArg8_inline();
        func_800C7464(a);
        return 0;
    case 0x1E:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        d = evt_getArg16_inline();
        func_800C1A8C(a, b, c, d, evt_getArg16_inline());
        return 0;
    case 0xC3:
        a = evt_getArg8_inline();
        func_800C1668(a, evt_getArg8_inline());
        return 0;
    case 0xC9:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        d = evt_getArg16_inline();
        func_800C119C(a, b, c, d, evt_getArg16_inline());
        return 0;
    case 0xED:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        func_800C11C8(a, b, evt_getArg16_inline());
        return 0;
    case 0xE4:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg16_inline();
        d = evt_getArg16_inline();
        func_800C152C(a, b, c, d, evt_getArg8_inline());
        return 0;
    case 0x7E:
        a = evt_getArg8_inline();
        func_800C0BCC(a);
        return 0;
    case 0xEA: {
        s16 x, z;
        func_800C0E6C(&x, &z);
        D_8007AE2C = x;
        D_8007AE30 = z;
        return 0;
    }
    case 0x8F:
        obj = func_80038150();
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        if (func_800B2CC0(obj->unk3B, a, b, evt_getArg8_inline())) {
            g_evtCurObj->unk0 = g_evtInstrStart;
            D_8007B6F8 = 1;
            return 1;
        }
        return 0;
    case 0x88:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        func_800ABA98(po->unk3B, a, b, c, evt_getArg16_inline());
        return 0;
    case 0x3A:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        a = evt_getArg8_inline();
        if (m != NULL) {
            m->unk14 |= 1 << a;
        }
        return 0;
    case 0x39:
        o = func_80038150();
        m = func_8004AFF0_inline(o);
        a = evt_getArg8_inline();
        if (m != NULL) {
            m->unk14 &= ~(1 << a);
        }
        return 0;
    case 0x68:
        func_80050068(evt_getArg8_inline());
        return 0;
    case 0xD7:
        func_80050080(evt_getArg8_inline());
        return 0;
    case 0xEF:
        func_8004FF98(evt_getArg8_inline());
        return 0;
    case 0x90:
        g_evtState->unk1700 = -1;
        return 0;
    case 0xEE:
        g_evtState->unk1700 = 0;
        return 0;
    case 0xB9:
        a = evt_getArg8_inline();
        g_padPortMask[a] |= evt_getArg16_inline();
        return 0;
    case 0xBA:
        a = evt_getArg8_inline();
        g_padPortMask[a] &= ~evt_getArg16_inline();
        return 0;
    case 0xC5:
        a = evt_getArg16_inline();
        g_gameState->unk1C->unk83C(a, evt_getArg16_inline(), 0, 0, 0);
        return 0;
    case 0xC6:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = func_8004B914_inline();
        g_gameState->unk1C->unk83C(a, b, c, 0, 0);
        return 0;
    case 0xC7:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = func_8004B914_inline();
        g_gameState->unk1C->unk83C(a, b, c, evt_getArg8_inline(), 0);
        return 0;
    case 0xC8:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = func_8004B914_inline();
        d = evt_getArg8_inline();
        g_gameState->unk1C->unk83C(a, b, c, d, evt_getArg8_inline());
        return 0;
    case 0x89: {
        u8 vol, pan;
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        func_800B1A20(a, b, evt_getArg16_inline(), &vol, &pan);
        D_8007AE38 = vol;
        D_8007AE3C = (pan * evt_getArg8_inline()) >> 7;
        if (D_8007AE3C >= 0x80) {
            D_8007AE3C = 0x7F;
        }
        return 0;
    }
    case 0x8A: {
        u8 vol, pan;
        po = func_80038150();
        func_800B1A20(po->pos.vx, po->pos.vy, po->pos.vz, &vol, &pan);
        D_8007AE38 = vol;
        D_8007AE3C = (pan * evt_getArg8_inline()) >> 7;
        if (D_8007AE3C >= 0x80) {
            D_8007AE3C = 0x7F;
        }
        return 0;
    }
    case 0xB3:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg16_inline();
        d = evt_getArg16_inline();
        func_800AFAC0(a, b, c, d, evt_getArg16_inline());
        return 0;
    case 0xDA:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg8_inline();
        d = evt_getArg16_inline();
        func_800AFAC0(a, b, c, d, evt_getArg16_inline());
        return 0;
    case 0x75:
        a = evt_getArg8_inline();
        menu_open(a, evt_getArg8_inline());
        func_8004A9E8();
        return 1;
    case 0xAC:
        a = evt_getArg16_inline();
        func_800B5004((a >> 14) & 3, a & 0x3FFF);
        return 1;
    case 0xAA:
        g_padPortMask[0] &= ~0x40000;
        return 0;
    case 0xAB:
        g_padPortMask[0] |= 0x40000;
        return 1;
    case 0x48:
        a = evt_getArg16_inline();
        b = evt_getArg8_inline();
        if (a < 0x100) {
            item_add(a, b);
        } else if (a < 0x200) {
            keyItem_add(a - 0x100);
        } else {
            while (--b != -1) {
                func_8006238C(a - 0x200);
            }
        }
        return 0;
    case 0x49:
        a = evt_getArg16_inline();
        b = evt_getArg8_inline();
        if (a < 0x100) {
            item_remove(a, b);
        } else if (a < 0x200) {
            keyItem_remove(a - 0x100);
        }
        return 0;
    case 0xCE:
        if ((g_gameState->unk1C->gil += func_8004B914_inline()) > 9999999) {
            g_gameState->unk1C->gil = 9999999;
        }
        return 0;
    case 0xCF:
        if ((g_gameState->unk1C->gil -= func_8004B914_inline()) > 9999999) {
            g_gameState->unk1C->gil = 0;
        }
        return 0;
    case 0xB4:
        a = evt_getArg16_inline();
        a |= (a >> 4) & 0xE0;
        for (i = 0; i < 9; i++) {
            func_80033C0C(i);
        }
        for (i = 0; i < 9; i++) {
            if (a & 1) {
                func_80033BD0(i);
            }
            a >>= 1;
        }
        return 0;
    case 0xFE:
        a = evt_execInstr_chr(evt_getArg8_inline());
        b = evt_getArg8_inline();
        c = evt_getArg8_inline();
        if ((u32)a < 9) {
            Character *ch = &g_gameState->unk1C->characters[a];
            d = evt_getArg8_inline();
            if (d != 0xFF) {
                ch->unkA = d;
            }
            d = evt_getArg8_inline();
            if (d != 0xFF) {
                ch->unk34.w = (ch->unk34.w & 0xFFFE0FFF) | ((d & 0x1F) << 12);
            }
            func_800333B0(a, b & 0xFF, c);
        } else {
            evt_getArg8_inline();
            evt_getArg8_inline();
        }
        return 0;
    case 0xDD:
        a = evt_execInstr_chr(evt_getArg8_inline());
        for (i = 0; i < 4; i++) {
            Character *ch = g_gameState->unk1C->party[i];
            if (ch != NULL && (ch->unk34.w & 0xF) == a) {
                break;
            }
        }
        if (i < 4) {
            party_setMember(i, -1);
            evt_execInstr_party();
        }
        return 0;
    case 0xB2:
        D_8007AE70.unk0 = evt_getArg8_inline();
        a = 0;
        for (i = 8; i >= 0; i--) {
            a <<= 1;
            a |= (g_gameState->unk1C->characters[i].unk34.w >> 11) & 1;
        }
        b = evt_getArg16_inline();
        for (i = 0; i < 9; i++) {
            D_8007AE70.unk10[i] = b & 1;
            b >>= 1;
        }
        for (i = 0; i < 4; i++) {
            Character *ch = g_gameState->unk1C->party[i];
            if (ch != NULL) {
                b = ch->unk34.w & 0xF;
                a &= ~(1 << b);
                D_8007AE70.unk4[i] = b;
            } else {
                D_8007AE70.unk4[i] = 0xFF;
            }
        }
        for (b = 0, i = 0; i < 9 && b < 8 && a != 0; i++, a >>= 1) {
            if (a & 1) {
                D_8007AE70.unk8[b++] = i;
            }
        }
        for (; b < 8; b++) {
            D_8007AE70.unk8[b] = 0xFF;
        }
        menu_open(3, (s32)&D_8007AE70);
        return 1;
    case 0xE9:
        evt_execInstr_party();
        return 0;
    case EVT_OP_SET_HP:
        a = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)a < 9) {
            Character *ch = &g_gameState->unk1C->characters[a];
            a = evt_getArg16_inline();
            if (ch->maxHp < a) {
                a = ch->maxHp;
            }
            ch->hp = a;
        }
        return 0;
    case EVT_OP_SET_MP:
        a = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)a < 9) {
            Character *ch = &g_gameState->unk1C->characters[a];
            a = evt_getArg16_inline();
            if (ch->maxMp < a) {
                a = ch->maxMp;
            }
            ch->mp = a;
        }
        return 0;
    case 0xF3:
        a = evt_execInstr_chr(evt_getArg8_inline());
        func_8003470C(a, evt_getArg8_inline());
        return 0;
    case 0xF4:
        a = evt_execInstr_chr(evt_getArg8_inline());
        func_80034220(a, evt_getArg8_inline());
        return 0;
    case 0xD9:
        a = evt_execInstr_chr(evt_getArg8_inline());
        func_80053960(&g_gameState->unk1C->characters[a], evt_getArg8_inline() & 0xFF);
        return 0;
    case 0x62:
        a = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)a < 9) {
            g_gameState->unk1C->characters[a].unk34.w = (g_gameState->unk1C->characters[a].unk34.w & ~0x200) | ((evt_getArg8_inline() & 1) << 9);
        }
        return 0;
    case 0xE0:
        if (g_gameState->unk1C->unk568 < 0x7FFF) {
            g_gameState->unk1C->unk568++;
        }
        return 0;
    case 0xB0: {
        u8 *src = func_8004F058(evt_getArg16_inline());
        u8 *dst = D_8007AE90;
        u8 *end = dst + 0xF;
        while (dst < end) {
            *dst++ = *src++;
        }
        *dst = 0xFF;
        g_gameState->unk1C->unk80C = D_8007AE90;
        return 0;
    }
    case 0xB1:
        g_gameState->unk1C->unk80C = D_8007AE8C;
        return 0;
    case 0x69:
        timer_setSeconds(evt_getArg16_inline());
        return 0;
    case 0x8D:
        timer_enable(evt_getArg8_inline());
        return 0;
    case 0x7D:
        timer_setRunning(evt_getArg8_inline());
        return 0;
    case 0xEC:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        func_8002F8BC(a & 1);
        func_8002F93C(((0x80 - evt_getArg8_inline()) << 4) & 0xFFF0);
        c = evt_getArg8_inline();
        d = evt_getArg8_inline();
        func_8002F8E4(1);
        func_800320B8(((a >> 2) & 1) + 1, (a >> 1) & 1, b, c | (d << 8) | (evt_getArg8_inline() << 16));
        return 0;
    case 0xA9:
        obj = func_80038150();
        if (obj != NULL) {
            sv.vx = obj->pos.vx;
            sv.vy = obj->pos.vy;
            sv.vz = obj->pos.vz;
            func_8004BA20(&sv, &D_8007AE2C, &D_8007AE30);
        }
        return 0;
    case 0x28:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        fld_movieCommand(a, b, evt_getArg8_inline());
        return 0;
    case 0xB7:
        if (g_evtMode == 3) {
            VECTOR *v = func_800AE750();
            actor->pos.vx = v->vx;
            actor->pos.vy = v->vy;
            actor->pos.vz = v->vz;
            actor->unk50 |= 0x40;
        }
        return 0;
    case 0xB8:
        if (g_evtMode == 3) {
            VECTOR *v = func_800AE744();
            actor->pos.vx = v->vx;
            actor->pos.vy = v->vy;
            actor->pos.vz = v->vz;
            actor->unk50 |= 0x80;
        }
        return 0;
    case 0xC4:
        a = evt_getArg8_inline();
        w_frameSetParameter(a, evt_getArg16_inline());
        return 0;
    case 0xD1:
        evt_clearGlobalVars();
        return 0;
    case 0xF6:
        func_80054FB8(evt_getArg8_inline());
        return 0;
    case 0xF7:
        func_80054FE0(evt_getArg8_inline());
        return 0;
    case 0xF8:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        func_80054E88(a, b, evt_getArg8_inline());
        return 0;
    case 0xF9:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        func_80055060(a, b, evt_getArg8_inline());
        return 0;
    case 0xFA:
        func_800550C0(evt_getArg16_inline());
        return 0;
    case 0xFB:
        func_800550D0(evt_getArg8_inline());
        return 0;
    case 0xFC:
        a = evt_getArg8_inline();
        func_800550F0(a, evt_getArg8_inline());
        return 0;
    case 0xFD:
        a = evt_getArg8_inline();
        ((s32 (*)(s32, s32))g_gameState->unk1C->unk8BC)(a & 0xFF, evt_getArg16_inline() & 0xFFFF);
        return 0;
    case 0xFF:
        /* padding between the two jump tables in the original rodata */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        op = g_evtArgFlags | 0xFF00;
        g_evtArgFlags = *g_evtCurObj->unk0++;
        switch (op) {
        case 0xFF00:
            a = evt_getArg8_inline();
            func_800C7F34(a, evt_getArg8_inline());
            return 0;
        case 0xFF01:
            a = evt_getArg8_inline();
            func_800C7F7C(a, evt_getArg8_inline());
            return 0;
        case 0xFF02:
            a = evt_getArg8_inline();
            func_800C7FBC(a, evt_getArg8_inline());
            return 0;
        case 0xFF03:
            a = evt_getArg8_inline();
            func_800C8010(a, evt_getArg8_inline());
            return 0;
        case 0xFF04:
            a = evt_getArg8_inline();
            func_800C8044(a, evt_getArg8_inline());
            return 0;
        case 0xFF05:
            a = evt_getArg8_inline();
            func_800C8068(a, evt_getArg16_inline());
            return 0;
        case 0xFF06:
            a = evt_getArg8_inline();
            func_800C808C(a, evt_getArg8_inline());
            return 0;
        case 0xFF07:
            a = evt_getArg8_inline();
            b = evt_getArg16_inline();
            func_800C80B0(a, b, evt_getArg16_inline());
            return 0;
        case 0xFF08:
            a = evt_getArg8_inline();
            func_800C80E4(a, evt_getArg8_inline());
            return 0;
        case 0xFF09:
            a = evt_getArg8_inline();
            func_800C7AF8(a, evt_getArg8_inline());
            return 0;
        case 0xFF0A:
            a = evt_getArg8_inline();
            func_800C6408(a, evt_getArg8_inline());
            return 0;
        case 0xFF0B:
            a = evt_getArg8_inline();
            func_800C7B68(a, evt_getArg8_inline());
            return 0;
        case 0xFF0C:
            a = evt_getArg8_inline();
            func_800C7CDC(a, evt_getArg8_inline());
            return 0;
        case 0xFF0D:
            a = evt_getArg8_inline();
            func_800C7BC4(a, evt_getArg16_inline());
            return 0;
        case 0xFF0E:
            a = evt_getArg8_inline();
            func_800C7C04(a, evt_getArg8_inline());
            return 0;
        case 0xFF0F:
            a = evt_getArg8_inline();
            b = evt_getArg8_inline();
            func_800C7C70(a, b, evt_getArg8_inline());
            return 0;
        case 0xFF10:
            a = evt_getArg8_inline();
            b = evt_getArg8_inline();
            func_800C7D30(a, b, evt_getArg8_inline());
            return 0;
        case 0xFF11:
            a = evt_getArg8_inline();
            func_800C7DF0(a, evt_getArg8_inline());
            return 0;
        }
        return 0;
    default:
        switch (g_evtMode) {
        case 1:
            return func_800500A8(po, op);
        case 2:
            return func_80050410(po, op);
        case 3:
            return func_80050744(po, op);
        }
        return 1;
    }
}

void func_800475D8(Unk8004AB64 *obj) {
    SVECTOR v;
    u8 dz[4];
    s32 dist;
    s32 speed;
    s32 dir;
    s32 analog;
    s32 pad;
    s32 x, y;
    s32 run;
    s32 cur, d;
    s32 c, k;
    Unk8004AB64 *o;
    s32 m;

    o = NULL;
    speed = 30;
    dir = 0x8000;
    analog = 0;
    memset(&v, 0, 8);
    pad = func_8004E8D4();
    if (((g_padBuf[0].unk0 >> 12) & 0xF) == 7) {
        x = g_padAnalog[0].unk0[2];
        y = g_padAnalog[0].unk0[3];
        pad_getAnalogThresholds((Unk4 *)dz);
        /* The do-while(0) (a macro in the original?) doubles y's reference count in flow,
           which puts y ahead of x in global-alloc order (y s0, x s1). */
        if (x < dz[2] || dz[3] < x) {
            analog = 1;
        } else {
            do {
                if (y < dz[0] || dz[1] < y) {
                    analog = 1;
                }
            } while (0);
        }
        x -= 0x80;
        if (analog) {
            y = 0x80 - y;
            dir = func_80052F24(x, y);
            x *= x;
            m = y * y;
            if (m < x) {
                m = x;
            }
            speed = (m * 16 - m) * 4 >> 14;
        }
    }
    if (!analog) {
        run = ((GameConfig *)&g_gameState->unk24)->unk0_6;
        if ((pad & 0x10000) ? (run ^ 1) != 0 : run != 0) {
            speed = 60;
        }
        if (pad & 0x80) {
            dir = 0x400;
        }
        if (pad & 0x20) {
            dir = -0x400;
        }
        if (pad & 0x40) {
            if (dir == 0x8000) {
                dir = 0;
            } else {
                dir -= dir >> 1;
            }
        }
        if (pad & 0x10) {
            if (dir == 0x8000) {
                dir = 0x800;
            } else {
                dir += dir >> 1;
            }
        }
    }
    D_8007AEAC = dir != 0x8000;
    if (dir != 0x8000) {
        if (analog) {
            dir += g_evtState->unk16E8;
        } else {
            dir += g_evtState->unk16EA;
        }
        dir &= 0xFFF;
        cur = obj->pos.rotY & 0xFFF;
        d = dir - cur;
        if (d < -0x7FF) {
            cur -= 0x1000;
        } else if (d >= 0x800) {
            cur += 0x1000;
        }
        obj->pos.rotY = ((cur + dir) >> 1) & 0xFFF;
        if (g_evtState->unk16F8 != 0 && speed > 30) {
            speed = 30;
        }
        if (speed > 30) {
            D_8007AEC4 += 2;
        } else {
            D_8007AEC4 += 1;
        }
        obj->unk5C = speed;
        v.vz = -speed;
        func_800530A8(&v, 0, dir);
        obj->pos.vx += v.vx;
        obj->pos.vz += v.vz;
        o = func_80038618(obj, 0, &dist);
        D_8007AEB4 = ((o->unkF >> 4) ^ 1) & 1;
        if (D_8007AEB4 == 0) {
            D_8007AEB0 = 0;
        }
        if (o != NULL && dist <= 0 && D_8007AEB0 >= 0) {
            obj->unk98 = o;
            obj->unk9C = dist;
            c = func_800531D0(obj, o, dir);
            if ((u32)(c + 0x400) <= 0x800) {
                obj->pos.vx -= v.vx;
                obj->pos.vz -= v.vz;
                k = math_sin(c);
                v.vx = (v.vx * k) >> 13;
                v.vz = (v.vz * k) >> 13;
                func_800530A8(&v, 0, 0x400);
                obj->pos.vx += v.vx;
                obj->pos.vz += v.vz;
                if (func_80038618(obj, 0, &dist) != NULL && dist <= 0) {
                    obj->pos.vx -= v.vx;
                    obj->pos.vz -= v.vz;
                }
                D_8007AEA8 = 2;
            }
        }
    } else {
        /* No code: flow deletes the dead store and jump2 the empty test, but the test keeps
           o = NULL live up to here (the s3 zero that analog and memset's argument copy), and
           its loop-weighted references put o ahead of obj in global-alloc order. */
        do {
            if (o != NULL && o != obj) {
                o = NULL;
            }
        } while (0);
    }
    D_8007AEB0 = D_8007AEA8 != 0 ? (D_8007AEB0 < 25 ? D_8007AEB0 + D_8007AEB4 : -25)
                                 : (D_8007AEB0 < 0 ? D_8007AEB0 + 1 : 0);
}

s32 func_80047AB8(Unk8004AB64 *p, s32 b) {
    s32 *sp;
    s32 ok;
    s32 x;
    u32 hi;
    s32 w;

    if (!(p->unk50 & 0x400)) {
        ok = 0;
        if (!(p->unk62 & 0x80) && p->unk6E == 0xA7) {
            ok = 0 < p->unk4;
        }
        if (ok) {
            p->unk70 = b >> 4;
            sp = &((s32 *)p + p->unk9)[p->unkA];
            sp[0] = p->unk0 - g_evtState->unk16F0;
            w = p->unkD | (p->unk4 << 8);
            hi = g_evtCurObj->unk7 << 24;
            x = hi | 0xFF0000;
            sp[1] = w | x;
            p->unk0 = (u8 *)&p->unk6E;
            p->unk4 = 0;
            p->unkD = 0;
            p->unkA += 2;
        }
    }
    return 1;
}

/* point 500 units ahead of o at angle a, at head height */
static inline void func_80047E1C_ahead(Unk8004AB64 *o, s32 a, VECTOR *out) {
    SVECTOR v;
    MATRIX m;

    m.m[0][2] = math_sin(a);
    m.m[1][1] = 0x1000;
    m.m[2][2] = math_cos(a);
    m.m[0][0] = m.m[0][1] = m.m[1][0] = m.m[1][2] = m.m[2][0] = m.m[2][1] = 0;
    m.t[0] = o->pos.vx;
    m.t[1] = o->pos.vy;
    m.t[2] = o->pos.vz;
    SetRotMatrix(&m);
    SetTransMatrix(&m);
    v.vx = 0;
    v.vy = (o->unk32 * (s8)o->unk2F) >> 6;
    v.vz = -500;
    gte_ldv0(&v);
    gte_rtv0tr();
    gte_stlvnl(out);
}

void func_80047B90(Unk8004AB64 *o, Unk80047B90 *m) {
    VECTOR out;
    s32 d;

    func_80047E1C_ahead(o, o->pos.rotY, &out);
    d = func_80052EC0(out.vx - o->unk84, out.vy - o->unk86, out.vz - o->unk88);
    if (d < 100) {
        d = 100;
        o->unk50 &= 0xF8FF;
    }
    o->unk84 += (out.vx - o->unk84) * 100 / d;
    o->unk86 += (out.vy - o->unk86) * 100 / d;
    o->unk88 += (out.vz - o->unk88) * 100 / d;
    m->unk0 |= 1;
    m->unk48 = o->unk84;
    m->unk4A = o->unk86;
    m->unk4C = o->unk88;
}

/* move the look-at point towards t */
static inline void func_80047E1C_approach(Unk8004AB64 *o, Unk80047B90 *mdl, VECTOR *t) {
    s32 d;

    d = func_80052EC0(t->vx - o->unk84, t->vy - o->unk86, t->vz - o->unk88);
    if (d < 100) {
        d = 100;
    }
    o->unk84 += (t->vx - o->unk84) * 100 / d;
    o->unk86 += (t->vy - o->unk86) * 100 / d;
    o->unk88 += (t->vz - o->unk88) * 100 / d;
    mdl->unk0 |= 1;
    mdl->unk48 = o->unk84;
    mdl->unk4A = o->unk86;
    mdl->unk4C = o->unk88;
}

/* is the look-at point still in front of o (within ~80 degrees)? */
static inline s32 func_80047E1C_infront(Unk8004AB64 *o) {
    s32 ok = 1;
    s32 d;

    if (!(o->unk50 & 0x400)) {
        d = (func_80052F24(o->unk84 - o->pos.vx, o->unk88 - o->pos.vz) - o->pos.rotY) & 0xFFF;
        if (d > 0x800) {
            d -= 0x1000;
        }
        ok = (u32)(d + 0x37F) < 0x6FF;
        if (!ok) {
            o->unk50 &= ~0x100;
        }
    }
    return ok;
}

static inline s32 func_80047E1C_infront2(Unk8004AB64 *o, s32 *pa) {
    s32 ok = 1;
    s32 a;
    s32 d;

    if (!(o->unk50 & 0x400)) {
        a = func_80052F24(o->unk84 - o->pos.vx, o->unk88 - o->pos.vz);
        d = (a - o->pos.rotY) & 0xFFF;
        if (d > 0x800) {
            d -= 0x1000;
        }
        ok = (u32)(d + 0x37F) < 0x6FF;
        if (!ok) {
            o->unk50 &= ~0x100;
        }
        *pa = a;
    }
    return ok;
}

void func_80047E1C(Unk8004AB64 *o) {
    VECTOR t;
    s32 ang;
    s32 save;
    s32 x, y, z;
    Unk80047B90 *mdl;
    Unk8004AB64 *tgt;
    Unk8004AB64 *best;
    Unk8004AB64 *q;
    Unk16D8 *n;
    u8 mask;
    s32 bestd;
    s32 d;
    s32 a;
    s32 ok;

    save = 0;
    mdl = (Unk80047B90 *)((Unk8E0 *)func_8001E350(o->unk3B))->unk8;
    tgt = NULL;
    if ((o->unk50 & 4) && D_8007AE68 != NULL && D_8007AE68 != o &&
        func_80052E48(o->pos.vx - D_8007AE68->pos.vx, o->pos.vz - D_8007AE68->pos.vz, 0) >= 1000) {
        tgt = D_8007AE68;
        o->unk50 |= 0x200;
    }
    if (tgt != NULL || ((o->unk50 & 1) && !(o->unk62 & 2))) {
        if (o->unkF & 0x80) {
            save = o->pos.rotY;
            o->pos.rotY = o->unk80;
        }
        if (tgt == NULL) {
            best = NULL;
            x = o->pos.vx;
            y = o->pos.vy;
            z = o->pos.vz;
            mask = o->unk6B;
            bestd = 0;
            for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
                q = n->unk4;
                if (q != o && q->unk5 == 4 && (u32)(q->pos.vy - y + 399) < 799 && (q->unk50 & 2) &&
                    (mask & q->unk6A)) {
                    d = func_80052DEC(q->pos.vx - x, q->pos.vz - z, 0);
                    if (d >= 1000) {
                        a = func_800531D0(o, q, o->pos.rotY);
                        d -= 4000000;
                        if (a < 0) {
                            a = -a;
                        }
                        a -= 640;
                        if (a < 0) {
                            d += a;
                            if (d < bestd) {
                                bestd = d;
                                best = q;
                            }
                        }
                    }
                }
            }
            tgt = best;
        }
        if (tgt != NULL) {
            if (!(o->unk50 & 0x100)) {
                if (!(o->unkF & 0x80)) {
                    o->unk50 |= 0x100;
                    func_80047E1C_ahead(o, o->pos.rotY, &t);
                    o->unk84 = t.vx;
                    o->unk86 = t.vy;
                    o->unk88 = t.vz;
                }
            }
            if ((o->unk50 & 0x100) && func_80047E1C_infront(o)) {
                {
                    t.vx = tgt->pos.vx;
                    t.vy = tgt->pos.vy;
                    t.vz = tgt->pos.vz;
                    ang = func_80052F24(t.vx - o->pos.vx, t.vz - o->pos.vz);
                    d = (ang - o->pos.rotY) & 0xFFF;
                    if (d > 0x800) {
                        d -= 0x1000;
                    }
                    if ((u32)(d + 0x27F) < 0x4FF) {
                        t.vy = (((o->pos.vy << 6) + o->unk32 * (s8)o->unk2F) * 3 + (t.vy << 6) +
                                tgt->unk32 * (s8)tgt->unk2F) >> 8;
                        func_80047E1C_approach(o, mdl, &t);
                    } else if (func_80047AB8(o, ang)) {
                        a = d > 0x280 ? 0x280 : -0x280;
                        func_80047E1C_ahead(o, a + o->pos.rotY, &t);
                        func_80047E1C_approach(o, mdl, &t);
                    }
                }
            }
        } else if ((o->unk50 & 0x100) && func_80047E1C_infront2(o, &ang) && func_80047AB8(o, ang)) {
            {
                func_80047B90(o, mdl);
            }
        }
        if (o->unkF & 0x80) {
            o->pos.rotY = save;
        }
    } else if ((o->unk50 & 0x100) && func_80047E1C_infront(o)) {
        func_80047B90(o, mdl);
    }
    if (!(o->unk50 & 0x100)) {
        mdl->unk0 &= ~1;
    }
}

void func_8004883C(void) {
    Unk8007AECC *e = g_evtState;
    Unk8004AB64 *p;
    Unk8004AB64 *o;
    Unk8007AECC *e2;

    if (e == NULL || e->unk16EC == 0) {
        return;
    }
    e->unk1700 -= e->unk1700 > 0;
    if (e->unk1700 != 0) {
        return;
    }
    o = func_8004B040_inline(e->playerUid);
    if (o == NULL || o->unk5 != 4) {
        return;
    }
    p = o;
    if (p->unk3A != p->unk38 - 1) {
        return;
    }
    e2 = g_evtState;
    if (e2->unk1700 >= 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        e2->unk1700 = (*g_randPtr++ + 0xC8) * 2;
    }
    if (p->unk74 != 0 && !(p->unk62 & 0x84)) {
        func_8004883C_idle(p);
        if (p->unk7 == g_evtState->playerUid) {
            D_8007B710++;
        }
        p->unk62 |= 2;
    }
}

/* Nearest kind-3 object whose script has entry `key` and that func_80053124
 * accepts for p's position. */
static inline Unk8004AB64 *func_80048A5C_find(Unk8004AB64 *p, s32 key) {
    Unk8004AB64 *found = NULL;
    s32 x = p->pos.vx;
    Unk16D8 *n = g_evtState->unk16D8;
    s32 z = p->pos.vz;
    Unk8004AB64 *o;

    for (; n != NULL; n = n->unk0) {
        o = n->unk4;
        if (o->unk5 == 3 && func_800384D0_find(o->unk6, key) != NULL && func_80053124(o, x, z)) {
            found = o;
            break;
        }
    }
    return found;
}

void func_80048A5C(Unk8004AB64 *p) {
    s32 btn;
    s32 t;
    Unk8004AB64 *o;

    t = func_8004E8EC();
    btn = t & (g_evtMode == 1 ? 0xA0000 : 0x20000);
    if (btn && (o = func_80038618(p, 4, NULL)) != NULL) {
        D_8007AE20 = func_800531D0(p, o, p->pos.rotY);
        if ((u32)(D_8007AE20 + 0x3FF) < 0x7FF) {
            p->unk93 = o->unk7;
            if (btn == 0x80000 && func_800384D0(o, 1, 8, 0)) {
                return;
            }
            if (func_800384D0(o, 1, 3, 0)) {
                return;
            }
        }
        o = NULL;
    } else {
        o = func_80038618(p, 2, NULL);
    }
    if (o != NULL) {
        func_800384D0(o, 1, 2, 0);
    }
    if (btn) {
        o = func_80048A5C_find(p, 3);
        if (o != NULL) {
            if (btn == 0x80000 && func_800384D0(o, 1, 8, 0)) {
                return;
            }
            func_800384D0(o, 1, 3, 0);
            o = NULL;
        } else {
            o = func_80048A5C_find(p, 2);
        }
    } else {
        o = func_80048A5C_find(p, 2);
    }
    if (o != NULL) {
        func_800384D0(o, 1, 2, 0);
    }
}

void func_80048E80(void) {
    s32 i;
    s32 m;
    Unk8007AED0 *o;

    if (g_gameState->unk1C->unk8E4.ovl65->unkD80 != 0) {
        i = 3;
        D_8007AE10[i] = btl_getUnitMask(1);
        m = (u16)D_8007AE10[i];
        for (i = 4; i < 8; i++) {
            o = D_8007AED0[i];
            if (o != NULL) {
                switch (o->unkE) {
                case 1:
                    if (!(m & 0x10)) {
                        o->unkE = 3;
                    }
                    break;
                case 3:
                    if (m & 0x10) {
                        o->unkE = 1;
                    }
                    break;
                }
            }
            m >>= 1;
        }
    }
}

/* turn towards unk52 by unk82 per frame (unk6C 0x7FFE / 0x7FFF: direction) */
static inline void func_80048F54_turn(Unk8004AB64 *p) {
    s32 a;

    if ((s16)p->unk6C == 0x7FFE) {
        a = p->pos.rotY + (s16)p->unk82;
        if (p->unk52 < a) {
            p->pos.rotY = a;
        } else {
            p->pos.rotY = p->unk52;
            p->unkF &= 0x7F;
        }
    } else if ((s16)p->unk6C == 0x7FFF) {
        a = p->pos.rotY + (s16)p->unk82;
        if (a >= p->unk52) {
            p->pos.rotY = p->unk52;
            p->unkF &= 0x7F;
        } else {
            p->pos.rotY = a;
        }
    }
}

/* remember the position at the start of the frame */
static inline void func_80048F54_save(Unk8004AB64 *p) {
    p->unk40.vx = p->pos.vx;
    p->unk40.vy = p->pos.vy;
    p->unk40.vz = p->pos.vz;
}

s32 func_80048F54(void) {
    Unk16D8 *n;
    Unk8004AB64 *o;
    Unk8004AB64 *p;
    Unk8004AB64 *t;
    Unk8004AB64 *t1;
    Unk8004AB64 *x;
    Unk16D8 *y;
    Unk8004AB64 *z;
    Unk800C748C *q;
    s32 world;
    s32 r;
    s32 dist;
    u8 b;
    s32 h;
    s32 moved;
    s32 dx, dy, dz;
    s32 f;
    u32 anim;
    Window *w;
    Unk16D8 *l;
    Unk16D8 *m;
    Unk8007AECC *s;

    D_8007AEAC = 0;
    world = g_evtMode == 2;
    for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
        o = n->unk4;
        if (o->unk5 == 4) {
            z = o;
            if (o->unk7 == g_evtState->playerUid && o->unkE == 1) {
                if (g_evtState->unk16EC != 0) {
                    if (g_evtMode == 1) {
                        o->unk40.vx = o->pos.vx;
                        o->unk40.vy = o->pos.vy;
                        o->unk40.vz = o->pos.vz;
                        func_800475D8(o);
                    } else if (g_evtMode == 3) {
                        D_8007AEAC = D_800CCE54;
                        func_80048A5C(o);
                        t1 = func_80038618(o, 0, &dist);
                        if (t1 != NULL && dist <= 0 &&
                            (u32)(func_800531D0(o, t1, func_80052F24(o->pos.vx - o->unk40.vx, o->pos.vz - o->unk40.vz)) + 0x400) <= 0x800) {
                            o->pos.vx = o->unk40.vx;
                            o->pos.vy = o->unk40.vy;
                            o->pos.vz = o->unk40.vz;
                        }
                    }
                    if (D_8007AEAC != 0) {
                        s = g_evtState;
                        if (s->unk1700 >= 0) {
                            s->unk1700 = (evt_rollEncounter_rand() + 200) * 4;
                        }
                    }
                } else {
                    func_80048F54_save(o);
                }
            } else {
                func_80048F54_save(z);
            }
            if (o->unkE == 1 && !world) {
                func_80049C14(o);
            }
        }
    }
    if (world) {
        func_80048E80();
    }
    l = (Unk16D8 *)D_8007B6FC;
    D_8007AEA4 = 0;
    if (l == NULL) {
        l = g_evtState->unk16D8;
    }
    r = func_80052870(l);
    if (r == 6) {
        r = 0;
    } else {
        D_8007B6FC = 0;
    }
    D_8007AEBC = NULL;
    D_8007AEB8 = NULL;
    if (g_evtMode == 1) {
        func_800C1B5C(&b);
        D_8007AEC8 = b != D_8007AEC0;
        D_8007AEC0 = b;
    }
    if (world) {
        for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->unkB == 2 && func_800384D0(o, 3, 5, 0)) {
                o->unkB = 1;
            }
        }
    } else {
        for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->unk5 == 4) {
                p = o;
                if (g_evtMode == 1) {
                    f = func_8002170C(p);
                    if ((p->unk7 != g_evtState->playerUid || D_8007AEB0 >= 0) && (f & 1) &&
                        (t = func_80038618(p, 0, NULL)) != NULL) {
                        func_800C7720(p->unk3B);
                        if (t->unkF & 0x10) {
                            D_8007AEB0 = 0;
                        }
                        if (o->unk7 == g_evtState->playerUid) {
                            D_8007AEA8 = 2;
                        }
                    }
                    if (o->unk7 == g_evtState->playerUid) {
                        func_8004F74C(p);
                        if (g_evtState->unk16EC != 0) {
                            func_80048A5C(p);
                        }
                    }
                } else if (g_evtMode == 3) {
                    if (p->unk50 & 0x40) {
                        VECTOR *v = func_800AE750();
                        v->vx = p->pos.vx;
                        v->vy = p->pos.vy;
                        D_8007AEB8 = p;
                        v->vz = p->pos.vz;
                    } else if (p->unk50 & 0x80) {
                        VECTOR *v = func_800AE744();
                        v->vx = p->pos.vx;
                        v->vy = p->pos.vy;
                        D_8007AEBC = p;
                        v->vz = p->pos.vz;
                    }
                }
                if (p->unk3E != 0xFF) {
                    w = func_8004ED94(p->unk3E);
                    if (w != NULL) {
                        func_80039444(p, w);
                    } else {
                        p->unk3E = 0xFF;
                        if (D_8007AE68 == p) {
                            D_8007AE6C = 30;
                        }
                    }
                }
                moved = 0;
                dx = p->pos.vx - p->unk40.vx;
                dy = p->pos.vy - p->unk40.vy;
                dz = p->pos.vz - p->unk40.vz;
                if (dx != 0 || dz != 0) {
                    moved = 1;
                }
                if (g_evtMode == 1) {
                    if (p->unkF & 0x80) {
                        func_80048F54_turn(p);
                    }
                    if (p->unk34 != 0xFFFF) {
                        func_80047E1C(p);
                    }
                }
                if (g_evtMode == 3 && (p->unkF & 0x80)) {
                    func_80048F54_turn(p);
                }
                if (moved) {
                    if ((p->unk62 & 0x86) == 0 || (p->unk62 & 0x86) == 0x82) {
                        p->unk62 &= 0x7D;
                        anim = p->unk5C >= p->unk5E ? (u16)p->unk58 : (u16)p->unk56;
                        goto set_anim;
                    }
                } else if (!(p->unk62 & 0x84) && (D_8007AEA8 == 0 || o->unk7 != g_evtState->playerUid)) {
                    anim = (u16)p->unk54;
                set_anim:
                    if (anim != p->unk36) {
                        p->unk36 = anim;
                        p->unk3A = 0;
                        p->unk64 = 0;
                        p->unk38 = evt_getAnimFrameCount(p->unk34, anim);
                        p->unk5D = p->unk69;
                    }
                }
                if (moved && o->unk7 == g_evtState->playerUid && D_8007AEAC != 0) {
                    g_evtEncounterDist += func_80052EC0(dx, dy, dz);
                    if (evt_rollEncounter(p)) {
                        g_evtEncounterPending = 1;
                    }
                }
            }
        }
    }
    if (D_8007AEA8 > 0) {
        D_8007AEA8--;
    }
    if (D_8007AE6C > 0 && --D_8007AE6C == 0) {
        for (y = g_evtState->unk16D8; y != NULL; y = y->unk0) {
            x = y->unk4;
            if (x->unk5 == 4 && (x->unk50 & 0x200)) {
                x->unk50 |= 0x400;
            }
        }
        D_8007AE68 = NULL;
    }
    if (g_evtEncounterPending != 0 && D_8007AEA4 == 0) {
        g_evtEncounterPending = 0;
        r = 3;
    }
    if (r == 3 || r == 7) {
        if (g_evtMode == 1) {
            for (m = g_evtState->unk16D8; m != NULL; m = m->unk0) {
                p = m->unk4;
                if (p->unk5 == 4 && p->unk34 != 0xFFFF) {
                    q = func_800C748C(p->unk3B);
                    func_800C75B4(p->unk3B, &h);
                    p->unk26 = h / 4;
                    if (q != NULL) {
                        p->pos.vx = p->unk40.vx;
                        p->pos.vy = p->unk40.vy;
                        p->pos.vz = p->unk40.vz;
                        p->unk2E = q->unk8;
                        p->unk28 = q->unkA;
                        p->unk2A = q->unk0;
                    }
                }
            }
            func_800B3444(0, 0, 0);
        }
        if (r == 7) {
            D_80079630 = D_80077E60;
        }
    }
    return r;
}

void func_800499D8(Unk8004B9B8 *p) {
    s32 v;
    u32 *q;

    if (!(p->unk62 & 8)) {
        v = p->unk54;
        if (v != p->unk36) {
            p->unk36 = v;
            p->unk3A = 0;
            p->unk64 = 0;
            p->unk38 = evt_getAnimFrameCount(p->unk34, v);
            p->unk5D = p->unk69;
        }
        p->unk3A = 0;
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        p->unk5D = p->unk7C[*g_randPtr++ & 3];
        p->unk62 &= 0x39;
        if (p->unkF & 0x80) {
            func_8004B9B8_inline(p);
        }
        if (p->unk50 & 8) {
            p->unk50 &= ~8;
            q = (u32 *)func_8001E350(p->unk3B);
            *q &= ~0x10;
            p->unk60 = 0;
            p->unk61 = 0xFF;
        }
    } else {
        p->unk62 = (p->unk62 | 4) & 0x7D;
        p->unk3A = p->unk39;
    }
}

static inline void func_80049B48_bounce(Unk8004AB64 *o, u8 f) {
    u8 t;
    u8 pos;

    if (f & 0x20) {
        pos = o->unk3A;
        if (!(f & 0x40)) {
            o->unk3A = pos - 2;
        } else {
            o->unk3A = pos + 2;
        }
        o->unk62 ^= 0x40;
        t = o->unk60;
        o->unk60 = o->unk61;
        o->unk61 = t;
    } else if (f & 0x10) {
        o->unk3A = o->unk60;
    }
}

void func_80049B48(Unk8004AB64 *o) {
    u8 f = o->unk62;

    if (f & 0x30) {
        if (o->unk63 != 0) {
            o->unk63--;
            if (o->unk63 != 0) {
                func_80049B48_bounce(o, f);
            } else {
                func_800499D8(o);
            }
        } else {
            func_80049B48_bounce(o, f);
        }
    } else {
        func_800499D8(o);
    }
}

void func_80049C14(Unk8004AB64 *p) {
    Unk8004AB64 *q;
    u8 f;
    s32 last;
    s32 t;
    s32 lo;
    u8 old;
    s32 t2, lo2, t3, lo3;
    u8 old2, old3;

    f = p->unk62;
    if (f & 4) {
        return;
    }
    if (f & 0x80) {
        if (p->unkA0 != NULL) {
            q = p->unkA0;
            if (q->unk36 == (u16)q->unk54) {
                p->unk36 = p->unk54;
            } else if (q->unk36 == (u16)q->unk56) {
                p->unk36 = p->unk56;
            } else if (q->unk36 == (u16)q->unk58) {
                p->unk36 = p->unk58;
            }
            p->unk3A = q->unk3A;
        } else if (!(f & 0x40)) {
            last = p->unk61;
            if (p->unk38 - 1 < last) {
                last = p->unk38 - 1;
            }
            t = p->unk3A * 16 + (s8)p->unk64 + p->unk5D;
            lo = t & 0xF;
            p->unk64 = lo;
            if (p->unk62 & 0x40) {
                p->unk64 = lo - 0xF;
            }
            old = p->unk3A;
            t >>= 4;
            p->unk3A = t;
            p->unk39 = old;
            if (last < t) {
                func_80049B48(p);
            }
            if (p->unkF & 0x80) {
                p->pos.rotY = (p->pos.rotY + p->unk6C) & 0xFFF;
                p->unk80 = (p->unk80 + p->unk82) & 0xFFF;
            }
        } else {
            t2 = p->unk3A * 16 + (s8)p->unk64 - p->unk5D;
            t2 += 0xF;
            lo2 = t2 & 0xF;
            p->unk64 = lo2;
            if (p->unk62 & 0x40) {
                p->unk64 = lo2 - 0xF;
            }
            old2 = p->unk3A;
            t2 >>= 4;
            p->unk3A = t2;
            p->unk39 = old2;
            if (t2 < p->unk61) {
                func_80049B48(p);
            }
        }
    } else {
        lo3 = p->unk3A * 16 + (s8)p->unk64;
        if (f & 0x40) {
            t3 = lo3 - p->unk5D;
        } else {
            t3 = lo3 + p->unk5D;
        }
        if (p->unk62 & 0x40) {
            t3 += 0xF;
        }
        lo3 = t3 & 0xF;
        p->unk64 = lo3;
        if (p->unk62 & 0x40) {
            p->unk64 = lo3 - 0xF;
        }
        old3 = p->unk3A;
        t3 >>= 4;
        p->unk3A = t3;
        p->unk39 = old3;
        if (t3 >= p->unk38) {
            p->unk3A = 0;
            if (p->unk36 == (u16)p->unk54) {
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                p->unk5D = p->unk7C.b[*g_randPtr++ & 3];
            }
        }
    }
}

u8 *func_80049ED8(u8 *p) {
    s32 mask = 0;
    s32 i = 0;
    s32 c;
    s32 n;
    u8 *dst;
    ScriptEntry *e;
    u16 *sz;
    s32 bit;
    u8 *src;

    for (; i < 4; i++) {
        c = func_8004E7A4(i);
        if (c >= 0) {
            mask |= 1 << c;
        }
    }
    n = p[3];
    D_8007ADF8 = p + 0x80;
    D_8007ADFC = n;
    if (D_8007ADFC >= 10) {
        dst = D_8007ADF8 + ((ScriptEntry *)D_8007ADF8)[D_8007ADFC - 9].unk0;
    } else {
        dst = D_8007ADF8 + D_8007ADFC * 8;
    }
    bit = 1;
    for (e = &((ScriptEntry *)D_8007ADF8)[D_8007ADFC - 9]; e < &((ScriptEntry *)D_8007ADF8)[D_8007ADFC]; e++) {
        if (!(e->unk5 & 1)) {
            mask |= bit;
        }
        if (mask & bit) {
            if (e->unk0 != 0) {
                src = D_8007ADF8 + e->unk0;
                if (src != dst && e->unk2 != 0) {
                    memmove(dst, src, e->unk2);
                    e->unk0 = dst - D_8007ADF8;
                }
                dst += e->unk2 & ~3;
            }
        }
        bit <<= 1;
    }
    D_8007AE00 = mask;
    return dst;
}

void evt_init(u8 *data) {
    Unk4 t;
    s32 k;
    u8 mode;
    Unk8007AECCNode *n;
    Unk16D8 *m;
    Unk16D8 *q;
    Unk16D8 *l;
    s32 i;
    s32 r;
    s32 d;
    Unk8004AB64 *o;
    Unk8007AECC *p;
    GameState *gs;

    t = D_80011A44;
    pad_setAnalogThresholds(&t);
    k = data[3];
    D_8007ADF8 = data + 0x80;
    gs = g_gameState;
    D_8007AE8C = data + 0x2C;
    gs->unk1C->unk810 = data + 4;
    D_8007ADFC = k;
    gs->unk1C->unk80C = data + 0x2C;
    mode = g_gameState->mode;
    switch (mode) {
    case 1:
        g_evtMode = mode;
        break;
    case 2:
        g_evtMode = mode;
        break;
    case 3:
        g_evtMode = mode;
        break;
    case 8:
        g_evtMode = 4;
        break;
    }
    if ((D_80077E60.unk16EE == 1 || D_80077E60.unk16EE == 3) && g_evtMode == 2) {
        D_80079630 = D_80077E60;
    }
    g_evtState = &D_80077E60;
    for (n = g_evtState->unk1450; n < &g_evtState->unk1450[31]; n++) {
        n->unk8 = n + 1;
    }
    n->unk8 = NULL;
    g_evtState->unk15D4 = g_evtState->unk1450;
    g_evtState->unk15D0 = NULL;
    for (m = g_evtState->unk15D8; m < &g_evtState->unk15D8[31]; m++) {
        m->unk0 = m + 1;
    }
    m->unk0 = NULL;
    g_evtState->unk16E0 = g_evtState->unk15D8;
    g_evtState->unk16DC = NULL;
    g_evtState->unk16D8 = NULL;
    func_80050044();
    func_80050080(0);
    for (i = 0; i < 0x50; i++) {
        g_evtState->unk0[i] = 0;
    }
    g_evtState->unk16EC = 0;
    g_evtState->playerUid = 0;
    g_evtState->unk1700 = 0;
    func_8004FF98(0);
    D_8007B6FC = 0;
    g_evtState->unk16F8 = 0;
    D_8007AE68 = NULL;
    D_8007B710 = 10;
    D_8007BBC8 = 10;
    D_8007BBD4 = 0;
    D_8007B718 = 0;
    D_8007AEC8 = 0;
    D_8007AEC0 = 0;
    g_evtEncounterDist = -0x5A0;
    p = g_evtState;
    p->unk16EF = 0;
    g_evtEncounterChance = 0;
    g_evtEncounterPending = 0;
    p->unk16E8 = 0;
    p->unk16EA = 0;
    D_8007BD2C = 0;
    func_8004B3A8_inline(0, 0);
    g_evtState->unk16D8->unk4->unkE = 2;
    evt_execInstr_party();
    for (i = 7; i >= 0; i--) {
        D_8007AED0[i] = NULL;
    }
    D_8007AE04 = 0;
    if (g_evtMode == 2) {
        for (i = 0; i < 4; i++) {
            r = func_8004E7A4(i);
            if (r >= 0) {
                evt_execInstr_newactor(r + D_8007ADFC - 9, 0);
            }
        }
        g_evtState->unk16FC = (s32)g_evtState->unk16DC;
    } else {
        g_gameState->unk1C->unk570 = 0;
    }
    D_8007AE04 = 4;
    if ((g_evtMode == 1 && D_80079630.unk16EE == g_evtMode && D_80079630.unk16F4 == (s16)g_gameState->unk1C->fieldId) ||
        (g_evtMode == 3 && D_80079630.unk16EE == g_evtMode && D_80079630.unk16F4 == g_gameState->unk1C->unk7DC) ||
        (g_gameState->unkA == 9 && D_8007AD1E != 0)) {
        d = data - D_80079630.unk16F0;
        D_80077E60 = D_80079630;
        for (q = g_evtState->unk16D8; q != NULL; q = q->unk0) {
            o = q->unk4;
            if (o->unk0 != NULL) {
                o->unk0 += d;
            }
            if (o->unk5 == 4 && g_evtMode == 1) {
                func_800AA3F4(o->unk6);
            }
        }
        func_800384D0(func_8004B040_inline(0), 0, 10, 0);
        for (l = g_evtState->unk16D8; l != NULL; l = l->unk0) {
            o = l->unk4;
            if (o->unk7 != 0) {
                o->unk12 = o->unkE;
                o->unkE = 3;
            }
        }
    } else {
        func_80048F54();
    }
    g_evtState->unk16F0 = data;
    g_evtState->unk16EE = g_evtMode;
    g_evtState->unk16F4 = g_evtMode == 1   ? g_gameState->unk1C->fieldId
                          : g_evtMode == 3 ? g_gameState->unk1C->unk7DC
                                            : 0;
}

void func_8004A8A4(void) {
    Unk16D8 *n;
    Unk8004AB64 *o;
    s16 *q;

    if ((g_evtMode == 1 || g_evtMode == 3 || g_gameState->unkA == 9) && D_8007AD1E != 0) {
        for (n = g_evtState->unk16D8; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->unk5 == 4 && o->unk34 != 0xFFFF) {
                o->unk3B = evt_createModel(o);
                if (g_evtMode == 1) {
                    q = (s16 *)func_800C748C(o->unk3B);
                    func_800C7570(o->unk3B, o->unk26 * 4);
                    if (q != NULL) {
                        q[4] = q[2] = o->unk2E;
                        q[5] = q[3] = o->unk28;
                        q[0] = o->unk2A;
                    }
                }
            }
        }
        D_8007AD1E = 0;
    }
}

void func_8004A9E8(void) {
    u8 *s;
    s32 n;
    func_8004E958();
    func_8004FF98(0);
    if (g_evtMode == 3) {
        s = D_800CF478;
        n = (func_80037D6C(0xC0) * 2) & 0x7F;
        while (n != 0) {
            while (*s++ != 0) {
            }
            n--;
        }
        g_gameState->unk1C->unk80C = s;
        while (*s++ != 0) {
        }
        g_gameState->unk1C->unk810 = s;
        func_800384D0(func_8004B040_inline(0), 1, 4, 0);
    }
}

s32 func_8004AAF0(void) {
    Unk8007AECC *p = g_evtState;
    Unk16D8 *n;
    s32 lim;
    s32 cnt = 0;
    if (p != NULL) {
        n = p->unk16D8;
        if (n != NULL) {
            lim = D_8007ADFC - 9;
            do {
                Unk8004AB64 *e = n->unk4;
                s32 t = 0;
                if (e->unk6 < lim) {
                    t = e->unk5 == 4;
                }
                cnt += t;
                n = n->unk0;
            } while (n != NULL);
        }
    }
    return cnt;
}

s32 func_8004AB64(Unk8004AB64 *a) {
    s32 t = a->unk6 + 9;
    return t - D_8007ADFC;
}

s32 evt_battleTrigger(s32 op, s32 bits, s32 v, s32 w) {
    s32 r = 0;
    s32 i;
    Unk8004AB64 *o;
    s32 a;
    s32 b;

    i = 0;
    if (!(bits & 1)) {
        bits >>= 1;
        i = 1;
        while (i < 8 && !(bits & 1)) {
            bits >>= 1;
            i++;
        }
    }
    if (i < 8) {
        o = D_8007AED0[i];
        if (op == 0x2F) {
            if (o->unk4 >= 4) {
                o->unkB = 2;
                r = 1;
            }
        } else {
            a = 2;
            b = 7;
            switch (op) {
            case 0x35:
                b = 6;
                D_8007AE10[0] = v;
                a = 1;
                break;
            case 0x36:
                b = 9;
                D_8007AE10[0] = v;
                a = 0;
                break;
            case 0x37:
                break;
            }
            D_8007AE34 = w;
            r = func_800384D0(o, a, b, 0);
        }
    }
    return r;
}

void func_8004AC80(void) {
    g_evtState = NULL;
    g_evtLastEncounter = 0;
    D_8007954E = 0;
    D_8007AD1E = 0;
}

void func_8004ACA4(Unk8004AB64 *o, s32 dx, s32 dy) {
    SVECTOR v;
    SVECTOR *pv = &v;
    s32 p[2];
    volatile s32 sxy;
    GameData *gd;

    v.vx = o->pos.vx;
    v.vy = o->pos.vy + ((o->unk32 * (s8)o->unk2F) >> 6) - 50;
    v.vz = o->pos.vz;
    if (g_evtMode == 1) {
        func_800C0F08();
    }
    gd = g_gameState->unk1C;
    gte_SetRotMatrix(&gd->unk18);
    gte_SetTransMatrix(&gd->unk18);
    gte_ldv0(pv);
    gte_rtps();
    gte_stsxy((s32 *)&sxy);
    if (g_evtMode == 1) {
        func_800C0FB0();
    }
    p[0] = *(s16 *)&sxy;
    p[1] = sxy >> 16;
    func_800515B8();
    func_800512B8(p[0] + dx, p[1] + dy);
    func_80051610();
}

Unk8004AB64 *func_8004AE00(void) {
    Unk8004AB64 *p = NULL;
    Unk8004AB64 *r;

    if (g_evtState != NULL) {
        p = func_8004B040_inline(g_evtState->playerUid);
    }
    if (p != NULL && p->unk5 == 4) {
        r = p;
    } else {
        r = NULL;
    }
    return r;
}

s32 func_8004AE84(void) {
    return g_evtState->unk16EC;
}

Unk16D8 *func_8004AE9C(void) {
    Unk8007AECC *p = g_evtState;
    Unk16D8 *r = NULL;
    if (p != NULL) {
        r = p->unk16D8;
    }
    return r;
}

s32 func_8004AEBC(void) {
    Unk8007AECC *p = g_evtState;
    s32 r = 0;
    if (p != NULL) {
        r = p->unk16FC;
    }
    return r;
}

s32 evt_getArg8(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        r = *g_evtCurObj->unk0++;
    }
    g_evtArgFlags >>= 1;
    D_8007B6F8 = 1;
    return r;
}

s32 evt_getArg16(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        Unk8004AB64 *pp = g_evtCurObj;
        r = *pp->unk0++;
        r |= *pp->unk0++ << 8;
        r = (s16)r;
    }
    g_evtArgFlags >>= 1;
    D_8007B6F8 = 1;
    return r;
}

s32 func_8004AFF0(Unk8004AB64 *p) {
    s32 *q;

    if (p != NULL) {
        if (p->unk34 == 0xFFFF) {
            return 0;
        }
        q = (s32 *)func_8001E350(p->unk3B);
        if (q != NULL) {
            return q[2];
        }
    }
    return 0;
}

Unk8004AB64 *func_8004B040(s32 id) {
    return func_8004B040_inline(id);
}

s32 func_8004B098(s32 id) {
    Unk8004AB64 *p = func_8004B040_inline(id);
    s32 r;

    if (p != NULL) {
        if (p->unk5 == 4) {
            r = p->unk3B;
        } else {
            r = -1;
        }
    } else {
        r = -1;
    }
    return r;
}

s32 func_8004B120(s32 i) {
    s32 t = g_evtState->unk16E4[i] + 9;
    u32 r = t - D_8007ADFC;
    s32 ret = 0;
    if (r < 9) {
        ret = r;
    }
    return ret;
}

void func_8004B158(s32 a, s32 b, s32 c) {
    func_800384D0(func_8004B040_inline(0), 1, 0x8000 | ((b << 8) & 0x3F00) | ((a << 2) & 0xFC) | (c & 3), 0);
}

s32 func_8004B1EC(void) {
    return (s32)D_8007AEB8;
}

s32 func_8004B1FC(void) {
    return (s32)D_8007AEBC;
}

void func_8004B20C(s32 a) {
    Unk8007AECC *p = g_evtState;
    s32 v;
    if (p->unk1700 >= 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        v = *g_randPtr++ + 200;
        if (a == 0) {
            p->unk1700 = v * 4;
        } else {
            p->unk1700 = v * 2;
        }
    }
}

void func_8004B2A0(void) {
    Unk8007AECCNode *p = g_evtState->unk1450;
    for (; p < &g_evtState->unk1450[31]; p++) {
        p->unk8 = p + 1;
    }
    p->unk8 = NULL;
    g_evtState->unk15D4 = g_evtState->unk1450;
    g_evtState->unk15D0 = NULL;
}

void func_8004B2F4(Unk8004AB64 *key) {
    func_8004B2F4_inline(key);
}

void func_8004B378(void) {
    Unk16D8 *p = g_evtState->unk15D8;
    for (; p < &g_evtState->unk15D8[31]; p++) {
        p->unk0 = p + 1;
    }
    p->unk0 = NULL;
    g_evtState->unk16E0 = g_evtState->unk15D8;
    g_evtState->unk16DC = NULL;
    g_evtState->unk16D8 = NULL;
}

void func_8004B3D0(s32 a, s32 b) {
    Unk800367D0 *p = func_800367D0(a, b, 0x38, 0x10);
    p->unk5 = 3;
    p->unkF = 1;
}

u16 func_8004B404(s32 arg0) {
    return func_8004B404_inline(arg0);
}

void func_8004B4A8(s32 i, s16 v) {
    D_8007AE10[i & 7] = v;
}

Unk8004AB64 *func_8004B4C4(s32 id) {
    return func_8004B4C4_inline(id);
}

s32 func_8004B574(void) {
    s32 n = 0;
    s32 v = evt_popResult();
    s16 bit;
    for (bit = 1; bit != 0; bit <<= 1) {
        n += (v & bit) != 0;
    }
    return n;
}

s32 func_8004B5CC(void) {
    u8 buf[8];
    s32 bits = evt_popResult();
    s32 n = 0;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (bits & 1) {
            buf[n++] = i;
        }
        bits >>= 1;
    }
    if (n != 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        return 1 << buf[(n * *g_randPtr++) >> 8];
    }
    return 0;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/event", D_80011A44);

s32 func_8004B68C(Unk8004AB64 *o, s32 prop) {
    s32 r;
    s32 i;
    void *u;

    switch (prop) {
    case 0:
        return o->pos.vx;
    case 1:
        return o->pos.vy;
    case 2:
        return o->pos.vz;
    case 3:
        return (u8)((u16)o->pos.rotY >> 4);
    case 4:
        return o->unkF;
    case 5:
        return o->unk7;
    case 6:
        return o->unk4;
    case 7:
        return o->unk3A;
    }
    r = 0;
    for (i = 0; i < 8; i++) {
        if (D_8007AED0[i] == o) {
            break;
        }
    }
    if (i < 8) {
        u = ((void *(*)(u16))btl_getUnitById)(1 << i);
        if (u != NULL) {
            r = btl_getUnitAttr(u, prop);
        }
    }
    return r;
}

void func_8004B7A4(Unk800219D8 *arg0, s32 arg1, s32 arg2) {
    s32 i;
    s32 (*fn)(s32, s32);
    s32 r;

    if (arg0 != NULL) {
        i = 0;
        if (arg1 == 2) {
            arg0->unk22 = arg2 * 16;
            return;
        }
        for (; i < 8; i++) {
            if (D_8007AED0[i] == arg0) {
                break;
            }
        }
        if (i < 8) {
            if (arg1 == 0x20) {
                fn = (s32 (*)(s32, s32))btl_findUnitByBit;
            } else {
                fn = btl_getUnitById;
            }
            r = fn((1 << i) & 0xFFFF, i);
            if (r != 0) {
                btl_setUnitAttr(r, arg1, arg2);
            }
        }
    }
}

s32 func_8004B85C(s32 slot) {
    return func_8004B85C_find(func_8004B85C_key(slot), slot);
}

s32 func_8004B914(void) {
    s32 v;
    Unk8004AB64 *pp;

    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        v = evt_popResult();
    } else {
        pp = g_evtCurObj;
        v = *pp->unk0++;
        v |= *pp->unk0++ << 8;
        v |= *pp->unk0++ << 16;
    }
    g_evtArgFlags >>= (D_8007B6F8 = 1);
    return v;
}

void func_8004B9B8(Unk8004B9B8 *p) {
    func_8004B9B8_inline(p);
}

void func_8004BA20(SVECTOR *v, s32 *x, s32 *y) {
    DVECTOR s;
    GameData *gd;

    if (x != NULL && y != NULL) {
        if (g_evtMode == 1) {
            func_800C0F08();
        }
        gd = g_gameState->unk1C;
        gte_SetRotMatrix(&gd->unk18);
        gte_SetTransMatrix(&gd->unk18);
        gte_ldv0(v);
        gte_rtps();
        gte_stsxy(&s);
        if (g_evtMode == 1) {
            func_800C0FB0();
        }
        *x = s.vx;
        *y = s.vy;
    }
}

void func_8004BB30(void *p) {
    s32 i;
    s32 bit;
    for (i = 0; i < 8; i++) {
        if (D_8007AED0[i] == p) {
            break;
        }
    }
    if (i < 8) {
        bit = 1 << i;
        D_8007AE12 = bit;
    }
}
