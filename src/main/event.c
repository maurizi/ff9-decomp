#include "ff9.h"

s32 evt_getSysVar(s32 op) {
    s32 r;
    u8 buf[8];
    EvtObject *o;
    s32 id;

    switch ((u32)op) {
    case 0:
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        r = *g_randPtr++;
        break;
    case 1:
        fldbg_getCamera(buf);
        r = buf[0];
        break;
    case 2:
        r = g_evtState->controlEnabled;
        break;
    case 3:
        r = fld_soundCommand(0xC00, -1, 0, 0, 0);
        break;
    case 4:
        r = g_evtTalkAngle;
        break;
    case 5:
        id = ((u8 *)&((u32 *)g_evtCurObj + g_evtCurObj->stackOfs)[g_evtCurObj->sp])[-1];
        o = evt_getObject_inline(id);
        r = -1;
        if (o != NULL) {
            r = o->scriptIdx - (g_evtScriptCount - 9);
        }
        break;
    case 6:
        r = g_gameState->gameData->gil;
        break;
    case 7:
        r = g_evtStepCount;
        break;
    case 8:
        r = g_msgSyncTag;
        break;
    case 9:
        r = msg_getChoiceIndex();
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
        r = g_gameState->gameData->frogCount;
        break;
    case 17:
        r = timer_getSeconds();
        break;
    case 18:
        r = card_getGameResult();
        break;
    case 19:
        r = card_countAll();
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
        r = (s16)g_gameState->gameData->battleId;
        break;
    case 25:
        r = (u8)btlseq_isBusy();
        break;
    case 26:
        r = g_evtMode == 4;
        break;
    case 27:
        r = g_gameState->gameData->battleResult;
        break;
    case 28:
        r = (D_8007AE34 >> 8) & 0xFF;
        break;
    case 29:
        r = (u8)D_8007AE34;
        break;
    case 30:
        r = g_gameState->gameData->ctx.ovl65->phase;
        break;
    case 31:
        r = g_gameState->gameData->ctx.ovl65->unk1340;
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

EvtObject *evt_getObjectArg(void) {
    return evt_resolveObjectId_inline(evt_getArg8_inline2());
}

/* Find the entry point for `key` in script `idx`. */
static inline u8 *evt_callObjectFunc_find(s32 idx, s32 key) {
    u8 *h = g_evtScripts + *(u16 *)(g_evtScripts + idx * 8);
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

void evt_returnFromFunc(EvtObject *o) {
    s32 v;
    Unk16D8 *n;
    Unk16D8 *m;
    EvtObject *t;
    EvtObject *q;
    u32 *base;
    u32 *p;

    if (o->sp >= 2) {
        p = (u32 *)o + o->stackOfs + o->sp;
        p--;
        v = p[0];
        if (!(*(u32 *)&o->level & 0xFF0000FF)) {
            for (m = g_evtState->objects; m != NULL; m = m->unk0) {
                t = m->unk4;
                if (t->uid != 0) {
                    t->state = t->savedState;
                }
            }
        }
        o->ip = g_evtState->scriptBase + p[-1];
        o->wait = v;
        o->sp -= 2;
        o->level = v >> 8;
        if (((v >> 16) & 0xFF) != 0xFF) {
            for (n = g_evtState->objects; n != NULL; n = n->unk0) {
                if (n->unk4->uid == (u32)v >> 24) {
                    break;
                }
            }
            q = NULL;
            if (n != NULL) {
                q = n->unk4;
            }
            if (q != NULL) {
                if (q->wait == 0xFF) {
                    q->wait = 0;
                    return;
                }
                base = (u32 *)q + q->stackOfs;
                p = base + (q->sp - 1);
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
    } else if (o->state == 2) {
        o->state = 1;
        o->ip = evt_callObjectFunc_find(o->scriptIdx, 1);
        o->level = 7;
    } else {
        o->ip = NULL;
    }
}

s32 evt_callObjectFunc(EvtObject *o, s32 lv, s32 key, s32 save) {
    u8 *e = NULL;
    u32 *slot;
    u16 w;
    u32 hi;
    u32 x;

    if (o != NULL && lv < o->level) {
        e = evt_callObjectFunc_find(o->scriptIdx, key);
        if (e != NULL) {
            slot = &((u32 *)o + o->stackOfs)[o->sp];
            slot[0] = o->ip - g_evtState->scriptBase;
            w = o->wait | (o->level << 8);
            hi = g_evtCurObj->uid << 24;
            if (save) {
                x = (g_evtCurObj->level << 16);
                x = x | w;
                x |= hi;
            } else {
                x = hi | 0xFF0000;
                x = x | w;
            }
            slot[1] = x;
            if (save) {
                g_evtCurObj->wait = 0xFF;
            }
            o->ip = e;
            o->level = lv;
            o->wait = 0;
            o->sp += 2;
        }
    }
    return e != NULL;
}

void *evt_findNearestObject(EvtObject *o, s32 flags, s32 *dist) {
    EvtObject *best = NULL;
    s32 bestd = 0;
    s32 x, y, z;
    s32 back;
    s32 r;
    Unk16D8 *n;
    EvtObject *q;
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
        r = o->talkRadius * 4;
    } else {
        r = o->collisionRadius * 4;
    }
    for (n = g_evtState->objects; n != NULL; n = n->unk0) {
        q = n->unk4;
        a = 0;
        if (back) {
            a = 1;
        } else {
            g = o->flags;
            if (q->uid == g_evtState->playerUid ? (g & 2) : (g & 4)) {
                a = 1;
            }
        }
        s = q->flags;
        if (best) {
            f = s;
        } else {
            f = q->flags;
        }
        if (back) {
            b = f & 8;
        } else if (o->uid == g_evtState->playerUid) {
            b = f & 2;
        } else {
            b = f & 4;
        }
        if (q == o || (a && b)) {
            continue;
        }
        if ((flags & 6) && evt_callObjectFunc_find(q->scriptIdx, back ? 3 : 2) == NULL) {
            continue;
        }
        if (q->kind != 4) {
            continue;
        }
        {
            EvtObject *p = q;
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
            rr = p->talkRadius * 4;
        } else {
            if (a) {
                rr = p->collisionRadius * 4;
            } else {
                rr = p->collisionRadius * 4;
            }
        }
        if (p->collisionEllipse != 0) {
            s = math_cos(evt_angleToObject(p, o, p->pos.rotY));
            rr = (rr * math_isqrt(((s * s) >> 4) * p->collisionEllipse + 0x1000000)) >> 12;
        }
        rr += r;
        if (flags & 6) {
            rr += 0x3C + p->speed;
        }
        d = math_distSq2(dx, dz, rr);
        if (back) {
            s = evt_angleToObject(o, p, o->pos.rotY);
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

s32 evt_moveObjectTo(s32 x, s32 y, s32 z, s32 flags) {
    EvtObject *p;
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

    p = g_evtActor;
    speed = p->speed;
    dx = x - p->pos.vx;
    dz = z - p->pos.vz;
    if (p->moveFlags & 0x20) {
        ry = p->pos.rotY & 0xFFF;
    } else {
        ry = math_atan2(dx, dz);
        if (!(flags & 1)) {
            cur = p->pos.rotY & 0xFFF;
            d = ry - cur;
            if (d < -0x800) {
                d += 0x1000;
            } else if (d >= 0x800) {
                d -= 0x1000;
            }
            if (d == 0) {
                p->moveFlags |= 0x20;
            }
            step = p->moveTurnRate * 8;
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
        rx = math_atan2(-dy, -math_length3(dx, 0, dz)) & 0xFFF;
        cur = p->pitch & 0xFFF;
        d = rx - cur;
        if (d < -0x800) {
            d += 0x1000;
        } else if (d >= 0x800) {
            d -= 0x1000;
        }
        step = p->moveTurnRate * 8;
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
        p->pitch = rx;
    } else {
        rx = 0;
        dy = 0;
    }
    func_8005309C(v, rx, ry, speed);
    p->pos.vx += v[0];
    p->pos.vy += v[1];
    p->pos.vz += v[2];
    hit = evt_findNearestObject(p, 0, &dist);
    if (hit != 0 && dist <= 0) {
        p->pos.vx -= v[0];
        p->pos.vy -= v[1];
        p->pos.vz -= v[2];
    }
    if (p->animLoops != 0 && p->animLoops != 0xFF) {
        p->animLoops--;
    }
    r = p->animLoops != 0;
    if (flags < 0) {
        r = 0;
        if (hit != flags || dist > 0) {
            r = 1;
        }
    }
    dist = math_dist2(dx, dy, dz);
    if (dist < speed * speed || ((p->moveFlags & 0x10) && p->moveDistSq < dist)) {
        r = 0;
    }
    if (p->moveDistSq < 0x7FFFFFFF && dist < p->moveDistSq) {
        p->moveFlags |= 0x10;
    }
    p->moveDistSq = dist;
    if (r == 0) {
        p->moveDistSq = 0x7FFFFFFF;
        p->pitch = 0;
        p->moveFlags &= ~0x30;
    }
    if (p->uid == g_evtState->playerUid) {
        g_evtPlayerAnimCount++;
    }
    return r;
}

s32 evt_rollEncounter(EvtObject *p) {
    s32 id = 0;

    if (g_evtState->controlEnabled != 0 && g_evtEncounterDist > 0x3C0) {
        g_evtEncounterDist = 0;
        g_evtEncounterChance += g_evtState->encounterRate;
        if (evt_rollEncounter_rand() < (g_evtEncounterChance >> 3)) {
            g_evtEncounterChance = 0;
            id = evt_rollEncounter_pick();
            if (id == g_evtLastEncounter) {
                id = evt_rollEncounter_pick();
            }
            g_evtLastEncounter = id;
            evt_setNextBattle(id);
            g_gameState->gameData->enemyGroup = -1;
        }
    }
    return id != 0;
}

/* screen position of an actor's speech-bubble anchor */
static inline void evt_placeBalloon_pos(EvtObject *obj, s32 *x, s32 *y) {
    SVECTOR sv;

    sv.vx = obj->pos.vx;
    sv.vy = obj->pos.vy + ((obj->height * (s8)obj->scale) >> 6) - 50;
    sv.vz = obj->pos.vz;
    if (obj->kind == 4) {
        sv.vx += obj->balloonOfsX;
        sv.vy += obj->balloonOfsY;
        sv.vz += obj->balloonOfsZ;
    }
    evt_projectToScreen(&sv, x, y);
}

static inline void evt_placeBalloon_x(WinLayout *w, s32 sx, s32 right, s32 *ptx) {
    s32 width = w->w;
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
    w->x = (l + r) >> 1;
}

static inline void evt_placeBalloon_y(WinLayout *w, s32 sy, s32 *below, s32 fixed) {
    s32 h = w->h;
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
    w->y = ty;
}

void evt_placeBalloon(EvtObject *obj, WinLayout *w, s32 dir, s32 fixed) {
    s32 sx, sy;
    s32 ox, oy;
    s32 tx;
    s32 below;
    s32 right;
    s32 id;
    EvtObject *other;

    if (obj->kind == 4) {
        obj->balloonOfsX = D_8007AE5C;
        obj->balloonOfsY = D_8007AE60;
        obj->balloonOfsZ = D_8007AE64;
    }
    evt_placeBalloon_pos(obj, &sx, &sy);
    if (dir < 0) {
        if (obj->uid == g_evtState->playerUid) {
            id = obj->talkTarget;
        } else {
            id = g_evtState->playerUid;
        }
        other = evt_getObject_inline(id);
        if (other != NULL) {
            evt_placeBalloon_pos(other, &ox, &oy);
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
    evt_placeBalloon_x(w, sx, right, &tx);
    evt_placeBalloon_y(w, sy, &below, fixed);
    if (fixed == 0 && msg_overlapsRect(w)) {
        below ^= 1;
        evt_placeBalloon_y(w, sy, &below, 0);
    }
    w->tailOffset = tx - w->x;
    w->tailMode = (below << 1) | right;
}

/* bubble x/y targets for evt_followBalloon (evt_placeBalloon_x/_y without the flip logic) */
static inline s32 evt_followBalloon_x(s32 width, s32 sx, s32 right, s32 *ptx) {
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

static inline s32 evt_followBalloon_y(s32 h, s32 *sy, s32 below) {
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

void evt_followBalloon(EvtObject *obj, Window *w) {
    s32 sx, sy;
    s32 tx;
    s32 ox, oy;
    s32 right, below;
    s32 x, y;

    ox = w->x;
    oy = w->y;
    evt_placeBalloon_pos(obj, &sx, &sy);
    below = (w->tailMode >> 1) & 1;
    right = w->tailMode & 1;
    x = evt_followBalloon_x(w->w - 3, sx, right, &tx);
    y = evt_followBalloon_y(w->h - 3, &sy, below);
    w->x = (ox + x) >> 1;
    w->y = (oy + y) >> 1;
    w->tailX = tx - w->x - right * 16;
    w->cur.x += w->x - ox;
    w->cur.y += w->y - oy;
}

WinLayout *evt_getMsgLayout(EvtObject *obj, s32 id, s32 flags) {
    WinLayout *w = msg_getLayout(id, &D_8007AE5C, &D_8007AE60, &D_8007AE64);
    s32 ww;
    s32 wh;

    w->tailOffset = 0;
    w->tailMode = 0;
    if ((flags & 0x40) && w->w < 0x72) {
        w->w = 0x72;
    }
    if (flags & 0x80) {
        if (w->w < 0x1D) {
            w->w = 0x1D;
        }
        switch (w->anchor) {
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
            evt_placeBalloon(obj, w, w->anchor & 3, 0);
            break;
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
            evt_placeBalloon(obj, w, w->anchor & 3, 1);
            break;
        default:
            evt_placeBalloon(obj, w, -1, 0);
            break;
        }
    } else {
        ww = w->w;
        wh = w->h;
        switch (w->anchor) {
        case 0x53:
            w->x = 12;
            w->y = 12;
            break;
        case 0x55:
            w->x = 160 - (ww >> 1);
            w->y = 12;
            break;
        case 0x52:
            w->x = 308 - ww;
            w->y = 12;
            break;
        case 0x5C:
            w->x = 160 - (ww >> 1);
            w->y = 74 - (wh >> 1);
            break;
        case 0x51:
            w->x = 12;
            w->y = -48 - wh;
            break;
        case 0x54:
            w->x = 160 - (ww >> 1);
            w->y = -48 - wh;
            break;
        case 0x50:
            w->x = 308 - ww;
            w->y = -48 - wh;
            break;
        case 0:
            w->x = 160 - (ww >> 1);
            w->y = 110 - (wh >> 1);
            break;
        }
    }
    if ((flags & 4) && w->w < 0x1D) {
        w->w = 0x1D;
    }
    w->tailMode |= flags & 0x30;
    return w;
}

void evt_turnObject(EvtObject *p, s32 d, s32 wrap, s32 speed) {
    s32 cur;
    s32 rate;
    s32 frames;
    s16 a;

    p->turnTarget = d & 0xFFF;
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
    p->animStart = 0;
    p->animEnd = 0xFF;
    p->turnStartDir = cur;
    p->turnPrevAnim = p->anim;
    p->flags |= 0x80;
    if (speed == 0) {
        speed = 0x10;
    }
    if (d < 0) {
        frames = evt_getAnimFrameCount(p->modelId, (u16)p->prevPos.rotX);
        rate = frames * 16 / speed;
        if (-d < 0x400) {
            rate = rate * -d >> 10;
        }
        if (rate >= 6) {
            p->turnRate = (d + 0x400) / rate;
            p->animFlags = 0;
            evt_execInstr_anim(p, (u16)p->prevPos.rotX);
            if (p->uid == g_evtState->playerUid) {
                g_evtPlayerAnimCount++;
            }
            p->animSpeed = frames * 16 / rate;
        } else {
            p->turnRate = 0x7FFE;
            a = cur;
            if (p->turnTarget >= cur) {
                a = cur | 0x1000;
            }
            p->pos.rotY = a;
        }
    } else {
        frames = evt_getAnimFrameCount(p->modelId, (u16)p->prevPos.rotY);
        rate = frames * 16 / speed;
        if (d < 0x400) {
            rate = rate * d >> 10;
        }
        if (rate >= 6) {
            p->turnRate = (d - 0x400) / rate;
            p->animFlags = 0;
            evt_execInstr_anim(p, (u16)p->prevPos.rotY);
            if (p->uid == g_evtState->playerUid) {
                g_evtPlayerAnimCount++;
            }
            p->animSpeed = frames * 16 / rate;
        } else {
            p->turnRate = 0x7FFF;
            a = cur;
            if (cur >= p->turnTarget) {
                a = cur | 0xF000;
            }
            p->pos.rotY = a;
        }
    }
    if (rate <= 0) {
        rate = 1;
    }
    p->turnStep = d / rate;
}

s32 evt_execInstr(void) {
    EvtObject *po = g_evtActor;
    EvtObject *actor = po;
    EvtObject *obj;
    EvtObject *o;
    Unk16D8 *n;
    s32 op;
    s32 a, b, c, d, e, f, i;
    SVECTOR sv;
    Unk80039C38Pt *pt;
    Unk8004AFF0 *m, *m2;
    u8 *str;

    g_evtInstrStart = g_evtCurObj->ip;
    op = *g_evtCurObj->ip++;
    g_evtArgFlags = *g_evtCurObj->ip++;
    g_evtArgRead = 0;

    switch (op) {
    case EVT_OP_NEW_CODE:
        func_8004B3A8_inline(g_evtArgFlags, *g_evtCurObj->ip++);
        g_evtArgRead = 1;
        return 0;
    case EVT_OP_NEW_REGION:
        evt_createRegion_inline(g_evtArgFlags, *g_evtCurObj->ip++);
        g_evtArgRead = 1;
        return 0;
    case EVT_OP_NEW_ACTOR:
        a = g_evtArgFlags;
        b = *g_evtCurObj->ip++;
        if ((u32)(a - 0xFB) < 4) {
            a = g_evtState->partyUids[a - 0xFB];
        }
        evt_execInstr_newactor(a, b);
        g_evtArgRead = 1;
        return 0;
    case 0x46:
    case 0x79:
    case 0xD2:
    case 0xD3:
        return 0;
    case EVT_OP_CALL:
        a = evt_getArg8_inline();
        obj = evt_getObjectArg(); /* reads the script before the key byte */
        evt_callObjectFunc(obj, a, *g_evtCurObj->ip++, 0);
        return 0;
    case EVT_OP_CALL_WAIT_FREE:
        a = evt_getArg8_inline();
        obj = evt_getObjectArg();
        c = *g_evtCurObj->ip++;
        if (obj != NULL && a < obj->level) {
            evt_callObjectFunc(obj, a, c, 0);
            return 0;
        }
        g_evtCurObj->ip = g_evtInstrStart;
        g_evtArgRead = 1;
        return 1;
    case EVT_OP_CALL_SYNC:
        a = evt_getArg8_inline();
        obj = evt_getObjectArg();
        c = *g_evtCurObj->ip++;
        if (obj != NULL && a < obj->level) {
            evt_callObjectFunc(obj, a, c, 1);
        } else {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case 0x16:
        a = evt_getArg8_inline();
        o = evt_getObject_inline(CUR_UID());
        evt_callObjectFunc(o, a, *g_evtCurObj->ip++, 0);
        return 0;
    case 0x18:
        a = evt_getArg8_inline();
        if (evt_getObject_inline(CUR_UID()) != NULL && a < evt_getObject_inline(CUR_UID())->level) {
            o = evt_getObject_inline(CUR_UID());
            evt_callObjectFunc(o, a, *g_evtCurObj->ip++, 0);
            return 0;
        }
        g_evtCurObj->ip = g_evtInstrStart;
        g_evtArgRead = 1;
        return 1;
    case 0x1A:
        a = evt_getArg8_inline();
        if (evt_getObject_inline(CUR_UID()) != NULL && a < evt_getObject_inline(CUR_UID())->level) {
            o = evt_getObject_inline(CUR_UID());
            evt_callObjectFunc(o, a, *g_evtCurObj->ip++, 1);
        } else {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case 0x97:
        obj = evt_getObjectArg();
        if (obj != NULL) {
            obj->sp = 0;
            obj->state = 2;
            evt_returnFromFunc(obj);
        }
        return obj == g_evtCurObj;
    case EVT_OP_SET_OBJ_FLAGS:
        g_evtActor->flags &= 0xC0;
        g_evtActor->flags |= evt_getArg8_inline() & 0x3F;
        return 0;
    case EVT_OP_WALK_ONLY:
        g_evtState->walkOnly = 1;
        return 0;
    case 0xF0:
        g_evtState->walkOnly = 0;
        return 0;
    case 0xD5:
        for (n = g_evtState->objects; n != NULL; n = n->unk0) {
            EvtObject *o = n->unk4;
            if (o->kind == 4) {
                o->savedFlags = o->flags;
                if (!(o->flags & 0x20)) {
                    o->flags &= 0xFE;
                }
            }
        }
        return 0;
    case 0xD6:
        for (n = g_evtState->objects; n != NULL; n = n->unk0) {
            EvtObject *o = n->unk4;
            if (o->kind == 4) {
                o->flags &= 0xFE;
                o->flags |= o->savedFlags & 1;
            }
        }
        return 0;
    case EVT_OP_SET_RADIUS:
        if (g_evtActor->kind == 4) {
            a = evt_getArg8_inline();
            if (g_evtMode == 1 && po->modelId != 0xFFFF) {
                bgi_setActorRadius(po->modelSlot, (a << 2) & 0xFFFC);
            }
            po->collisionRadius = evt_getArg8_inline();
            po->talkRadius = evt_getArg8_inline();
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
        po->collisionEllipse = evt_getArg8_inline();
        return 0;
    case 0x3B:
        g_evtCurObj->actorType = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_POS_XZ_OBJ:
        po = evt_getObjectArg();
    case EVT_OP_SET_POS_XZ:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = g_evtMode == 1 && po->modelId != 0xFFFF;
        if (c) {
            bgi_setActorActive(po->modelSlot, 0);
        }
        po->pos.vx = po->prevPos.vx = a;
        po->pos.vy = po->prevPos.vy = -0x8000;
        po->pos.vz = po->prevPos.vz = b;
        if (c) {
            bgi_setActorActive(po->modelSlot, 1);
        }
        if (po->kind == 4) {
            po->moveDistSq = 0x7FFFFFFF;
            po->pitch = 0;
            po->moveFlags &= 0xFFCF;
        }
        g_evtPosSetThisFrame = 1;
        return 0;
    case EVT_OP_SET_POS_OBJ:
        po = evt_getObjectArg();
    case EVT_OP_SET_POS:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        d = g_evtMode == 1 && po->modelId != 0xFFFF;
        if (d) {
            bgi_setActorActive(po->modelSlot, 0);
        }
        po->pos.vx = po->prevPos.vx = a;
        po->pos.vy = po->prevPos.vy = b;
        po->pos.vz = po->prevPos.vz = c;
        if (po->kind == 4) {
            po->moveDistSq = 0x7FFFFFFF;
            po->pitch = 0;
            po->moveFlags &= 0xFFCF;
        }
        return 0;
    case EVT_OP_SET_DIR_OBJ:
        po = evt_getObjectArg();
    case EVT_OP_SET_DIR:
        po->pos.rotY = evt_getArg8_inline() << 4;
        return 0;
    case EVT_OP_SET_ROT_XZ:
        po->pos.rotX = evt_getArg8_inline() << 4;
        po->rotZ = evt_getArg8_inline() << 4;
        return 0;
    case EVT_OP_SET_LOOK:
        actor->lookParam0 = evt_getArg8_inline();
        actor->lookParam1 = evt_getArg8_inline();
        actor->moveFlags |= 3;
        return 0;
    case EVT_OP_SET_MOVE_FLAGS:
        actor->moveFlags &= 0xFFF8;
        actor->moveFlags |= evt_getArg8_inline() & 7;
        return 0;
    case EVT_OP_SET_TALK_TURN:
        actor->talkTurnScript = evt_getArg8_inline() ? 0xA7 : 4;
        return 0;
    case EVT_OP_SET_TURN_SPEED:
        actor->turnSpeed = evt_getArg8_inline();
        if (actor->turnSpeed == 0) {
            actor->turnSpeed = 0x10;
        }
        return 0;
    case 0xBB:
        actor = evt_getObjectArg();
    case EVT_OP_TURN:
        a = evt_getArg8_inline() << 4;
        evt_turnObject(actor, a, 1, evt_getArg8_inline());
        return 0;
    case 0xA7:
        evt_turnObject(actor, evt_getArg8_inline() << 4, 1, actor->turnSpeed);
        return 0;
    case EVT_OP_TURN_TO_POINT:
        a = evt_getArg16_inline() - actor->pos.vx;
        b = evt_getArg16_inline() - actor->pos.vz;
        if (a | b) {
            evt_turnObject(actor, math_atan2(a, b), 1, actor->turnSpeed);
        }
        return 0;
    case EVT_OP_TURN_TO_OBJ:
        obj = evt_getObjectArg();
        a = evt_getArg8_inline();
        if (obj != NULL) {
            evt_turnObject(actor, math_atan2(obj->pos.vx - actor->pos.vx, obj->pos.vz - actor->pos.vz), 1, a);
        }
        return 0;
    case 0xBC:
        actor = evt_getObjectArg();
    case EVT_OP_WAIT_TURN:
        if (actor->flags & 0x80) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_MSG_NUMBER:
        a = evt_getArg8_inline();
        msg_setNumber(a, evt_getArg16_inline());
        return 0;
    case EVT_OP_MSG_SYNC_TAG:
        g_msgSyncTag = evt_getArg8_inline();
        return 0;
    case 0x7C:
        a = evt_getArg16_inline();
        msg_setChoiceMask(a, evt_getArg8_inline());
        return 0;
    case 0x53:
        msg_keepWindowsOnInit();
        return 0;
    case EVT_OP_MSG_WAIT:
        g_evtActor->windowId = evt_getArg8_inline();
        c = evt_getArg8_inline();
        evt_execInstr_window(g_evtActor, c, g_evtActor->windowId);
        b = evt_getArg16_inline();
        msg_open(b, g_evtActor->windowId, evt_getMsgLayout(g_evtActor, b, c), c);
        g_evtActor->wait = 0xFE;
        return 1;
    case EVT_OP_MSG:
        g_evtActor->windowId = evt_getArg8_inline();
        c = evt_getArg8_inline();
        evt_execInstr_window(g_evtActor, c, g_evtActor->windowId);
        b = evt_getArg16_inline();
        msg_open(b, g_evtActor->windowId, evt_getMsgLayout(g_evtActor, b, c), c);
        return 0;
    case EVT_OP_MSG_OBJ_WAIT:
        po = evt_getObjectArg();
        g_evtActor->windowId = evt_getArg8_inline();
        c = evt_getArg8_inline();
        evt_execInstr_window(po, c, g_evtActor->windowId);
        b = evt_getArg16_inline();
        msg_open(b, g_evtActor->windowId, evt_getMsgLayout(po, b, c), c);
        g_evtActor->wait = 0xFE;
        return 1;
    case EVT_OP_MSG_OBJ:
        po = evt_getObjectArg();
        g_evtActor->windowId = evt_getArg8_inline();
        c = evt_getArg8_inline();
        evt_execInstr_window(po, c, g_evtActor->windowId);
        b = evt_getArg16_inline();
        msg_open(b, g_evtActor->windowId, evt_getMsgLayout(po, b, c), c);
        return 0;
    case EVT_OP_MSG_CLOSE:
        a = evt_getArg8_inline();
        msg_close(a);
        return 0;
    case 0xEB:
        msg_resetWindows();
        win_closeAll();
        return 0;
    case EVT_OP_MSG_WAIT_WIN:
        g_evtActor->windowId = evt_getArg8_inline();
        g_evtActor->wait = 0xFE;
        return 1;
    case 0x8E:
        func_8004ED3C();
        return 0;
    case 0xD0:
        btl_setBanner((s32)msg_getText(evt_getArg16_inline()), 3);
        return 0;
    case EVT_OP_MOVE_RESET:
        actor->animLoops = 0xFF;
        actor->moveDistSq = 0x7FFFFFFF;
        actor->pitch = 0;
        actor->moveFlags &= 0xFFCF;
        return 0;
    case EVT_OP_SET_MOVE_COUNT:
        actor->animLoops = evt_getArg8_inline();
        return 0;
    case EVT_OP_MOVE_XZ:
        a = evt_getArg16_inline();
        if (evt_moveObjectTo(a, 0, evt_getArg16_inline(), 0)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_MOVE_XZ_NOTURN:
        a = evt_getArg16_inline();
        if (evt_moveObjectTo(a, 0, evt_getArg16_inline(), 1)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_MOVE_TO_OBJ:
        po = evt_getObjectArg();
        if (evt_moveObjectTo(po->pos.vx, po->pos.vy, po->pos.vz, (s32)po)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        g_evtArgRead = 1;
        return 1;
    case 0xA0:
        if (evt_moveObjectTo(D_8007AE24, 0, D_8007AE28, 0)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_MOVE_XYZ:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        if (evt_moveObjectTo(a, b, c, 2)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case 0xE8:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        if (evt_moveObjectTo(a, b, c, 3)) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
        }
        return 1;
    case EVT_OP_SET_MOVE_TURN:
        a = evt_getArg8_inline();
        if (a == 0) {
            a = 0xFF;
        }
        actor->moveTurnRate = a;
        return 0;
    case EVT_OP_SET_WALK_SPEED:
        actor->speed = evt_getArg8_inline();
        return 0;
    case 0xA6:
        actor->runSpeed = evt_getArg8_inline();
        return 0;
    case EVT_OP_ANIM_FWD:
        actor->animFlags = 8;
        actor->animStart = 0;
        actor->animEnd = actor->jumpAnimStart - 1;
        evt_execInstr_anim(actor, actor->jumpAnim);
        if (actor->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        return 0;
    case EVT_OP_ANIM_BACK:
        ((ModelSlot *)game_getModel(actor->modelSlot))->flags &= ~0x10;
        actor->animEnd = 0xFF;
        actor->animFlags = 0;
        actor->animStart = actor->jumpAnimEnd + 1;
        evt_execInstr_anim(actor, actor->jumpAnim);
        if (actor->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        return 0;
    case EVT_OP_JUMP:
        actor->jumpTargetX = evt_getArg16_inline();
        actor->jumpTargetY = evt_getArg16_inline();
        actor->jumpTargetZ = evt_getArg16_inline();
        a = evt_getArg8_inline();
        if (a == 0) {
            a = 8;
        }
        actor->moveFlags |= 8;
        ((ModelSlot *)game_getModel(actor->modelSlot))->flags |= 0x10;
        actor->animStart = actor->jumpAnimStart;
        actor->animEnd = actor->jumpAnimEnd;
        evt_execInstr_anim(actor, actor->jumpAnim);
        if (actor->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        actor->jumpFrame = 0;
        actor->jumpFrames = a;
        actor->jumpStartX = actor->pos.vx;
        actor->jumpStartY = actor->pos.vy;
        actor->jumpStartZ = actor->pos.vz;
        actor->animSpeed = ((actor->animEnd - actor->animStart) << 4) / a;
        if (g_evtMode == 1 && po->modelId != 0xFFFF) {
            bgi_setActorActive(po->modelSlot, 0);
        }
        return 0;
    case EVT_OP_JUMP_STEP:
        b = actor->jumpFrames;
        a = ++actor->jumpFrame;
        actor->pos.vx = ((s16)actor->jumpStartX * (b - a) + actor->jumpTargetX * a) / b;
        actor->pos.vy = (s16)actor->jumpStartY + a * a * 8 + a * (actor->jumpTargetY - (s16)actor->jumpStartY) / b - a * (b << 3);
        actor->pos.vz = ((s16)actor->jumpStartZ * (b - a) + actor->jumpTargetZ * a) / b;
        if (a >= b) {
            return 0;
        }
        g_evtCurObj->ip = g_evtInstrStart;
        g_evtArgRead = 1;
        return 1;
    case EVT_OP_SET_PATH: {
        u32 cnt;
        pt = ((Unk80039C38Quad *)g_evtActor)->unk18;
        cnt = ((Unk80039C38Quad *)g_evtActor)->unk14 = evt_getArg8_inline();
        while (cnt-- != 0) {
            pt->unk0 = evt_getArg16_inline();
            pt->unk2 = evt_getArg16_inline();
            pt++;
        }
        return 0;
    }
    case 0xA4:
        po = evt_getObject_inline(g_evtState->playerUid);
        if (po != NULL) {
            Unk80039C38Pt *p = ((Unk80039C38Quad *)g_evtActor)->unk18;
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
        po = evt_getObject_inline(g_evtState->playerUid);
        if (po != NULL) {
            s32 *sp = (s32 *)po + po->stackOfs + po->sp;
            Unk16D8 *nn;
            sp[0] = po->ip - g_evtState->scriptBase;
            sp[1] = po->wait | po->level << 8 | 0xFF << 16 | g_evtCurObj->uid << 24;
            po->level = 0;
            po->wait = 0;
            po->sp += 2;
            po->ip = D_8006E658;
            g_evtState->controlEnabled = 0;
            for (nn = g_evtState->objects; nn != NULL; nn = nn->unk0) {
                nn->unk4->flags |= 6;
            }
        }
        return 0;
    case 0x76: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)g_evtActor;
        q->unk14 = 2;
        q->unk18[0].unk0 = q->unk18[1].unk0 = evt_getArg16_inline();
        q->unk18[0].unk2 = q->unk18[1].unk2 = evt_getArg16_inline();
        return 0;
    }
    case 0x77: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)g_evtActor;
        Unk80039C38Pt *p;
        a = q->unk14;
        p = &q->unk18[(a > 0 ? a : 1) - 1];
        p->unk0 = evt_getArg16_inline();
        p->unk2 = evt_getArg16_inline();
        return 0;
    }
    case 0x78: {
        Unk80039C38Quad *q = (Unk80039C38Quad *)g_evtActor;
        s32 k = q->unk14;
        if (k >= 1 && k < 8) {
            q->unk18[k] = q->unk18[k - 1];
            q->unk14++;
        }
        return 0;
    }
    case EVT_OP_GOTO_FIELD:
        evt_setNextLocation(evt_getArg16_inline());
        return 4;
    case EVT_OP_GOTO_WORLD:
        evt_setNextLocation(evt_getArg16_inline());
        return 5;
    case 0xAE:
        evt_setCardOpponent(evt_getArg16_inline() & 0xFFFF);
        return 7;
    case EVT_OP_GAME_OVER:
        return 8;
    case EVT_OP_BATTLE:
        g_swirlVariant = evt_getArg8_inline();
        g_gameState->gameData->enemyGroup = -1;
        a = evt_getArg16_inline();
        g_gameState->gameData->unk571 = (a >> 15) & 1;
        evt_setNextBattle(a & 0x7FFF);
        return 3;
    case 0x8C:
        g_swirlVariant = evt_getArg8_inline();
        g_gameState->gameData->enemyGroup = evt_getArg8_inline();
        a = evt_getArg16_inline();
        g_gameState->gameData->unk571 = (a >> 15) & 1;
        evt_setNextBattle(a & 0x7FFF);
        return 3;
    case EVT_OP_ENCOUNTER_RATE:
        if ((g_evtState->encounterRate = evt_getArg8_inline()) == 0) {
            g_evtEncounterChance = 0;
        }
        return 0;
    case EVT_OP_ENCOUNTER_TABLE:
        g_evtEncounterTable.pattern = evt_getArg8_inline();
        g_evtEncounterTable.battle0 = evt_getArg16_inline();
        g_evtEncounterTable.battle1 = evt_getArg16_inline();
        g_evtEncounterTable.battle2 = evt_getArg16_inline();
        g_evtEncounterTable.battle3 = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_PLAYER:
        g_evtState->playerUid = g_evtCurObj->uid;
        return 0;
    case EVT_OP_DISABLE_CONTROL:
        g_evtState->controlEnabled = 0;
        msg_startHereIcon(0);
        g_evtPlayerAnimCount = 0;
        g_evtMsgCount = 0;
        return 0;
    case EVT_OP_ENABLE_CONTROL: {
        EvtState *e;
        g_evtState->controlEnabled = 1;
        msg_startHereIcon(1);
        e = g_evtState;
        if (e->idleTimer >= 0) {
            if (g_randPtr >= g_randTableEnd) {
                rand_refill();
            }
            e->idleTimer = (*g_randPtr++ + 0xC8) * 4;
        }
        return 0;
    }
    case EVT_OP_SET_MODEL:
        po->modelId = evt_getArg16_inline();
        po->modelSlot = evt_createModel(g_evtCurObj);
        g_evtCurObj->flags |= 1;
        po->height = -(evt_getArg8_inline() << 2);
        return 0;
    case 0xD4:
        m = evt_getObjectModelData_inline(g_evtActor);
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        if (m != NULL) {
            m->attachX = a;
            m->attachY = b;
            m->attachZ = c;
        }
        return 0;
    case EVT_OP_ATTACH:
        o = evt_getObjectArg();
        m = evt_getObjectModelData_inline(o);
        o = evt_getObjectArg();
        m2 = evt_getObjectModelData_inline(o);
        a = evt_getArg8_inline();
        if (m != NULL && m2 != NULL) {
            m->parent = m2;
            m->parentBone = a;
        }
        return 0;
    case EVT_OP_DETACH:
        o = evt_getObjectArg();
        m = evt_getObjectModelData_inline(o);
        if (m != NULL) {
            m->parent = NULL;
        }
        return 0;
    case EVT_OP_MODEL_SCALE:
        po = evt_getObjectArg();
        m = evt_getObjectModelData_inline(po);
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg8_inline();
        if (m != NULL) {
            m->scaleX = (s8)a << 6;
            m->scaleY = (s8)b << 6;
            m->scaleZ = (s8)c << 6;
            m->flags |= 2;
        }
        po->scale = b;
        return 0;
    case EVT_OP_TEX_ANIM_PLAY:
        o = evt_getObjectArg();
        m = evt_getObjectModelData_inline(o);
        b = evt_getArg8_inline();
        if (m != NULL) {
            m->texAnims->unk0[b].flags |= 1;
            m->texAnims->unk0[b].pos = 0;
            m->texAnims->unk0[b].lastFrame = 0x1000;
        }
        return 0;
    case 0xC1:
        o = evt_getObjectArg();
        m = evt_getObjectModelData_inline(o);
        b = evt_getArg8_inline();
        if (m != NULL) {
            m->texAnims->unk0[b].flags |= 3;
            m->texAnims->unk0[b].pos = 0;
            m->texAnims->unk0[b].lastFrame = 0x1000;
        }
        return 0;
    case 0xC2:
        o = evt_getObjectArg();
        m = evt_getObjectModelData_inline(o);
        b = evt_getArg8_inline();
        if (m != NULL) {
            m->texAnims->unk0[b].flags &= 0xFC;
        }
        return 0;
    case EVT_OP_MODEL_FLAGS_SET: {
        ((ModelSlot *)game_getModel(po->modelSlot))->flags |= evt_getArg16_inline();
        return 0;
    }
    case EVT_OP_MODEL_FLAGS_CLEAR: {
        ((ModelSlot *)game_getModel(po->modelSlot))->flags &= ~evt_getArg16_inline();
        return 0;
    }
    case 0x4E:
        str = g_evtCurObj->ip - 1;
        while (*str++ != 0) {
        }
        func_80052D0C(&str);
        g_evtArgRead = 1;
        g_evtCurObj->ip = str + 1;
        return 0;
    case 0x4F:
        g_evtArgRead = 1;
        g_evtCurObj->ip++;
        return 6;
    case EVT_OP_SET_STAND_ANIM:
        actor->standAnim = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_WALK_ANIM:
        actor->walkAnim = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_RUN_ANIM:
        actor->runAnim = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_TURN_LEFT_ANIM:
        actor->prevPos.rotX = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_TURN_RIGHT_ANIM:
        actor->prevPos.rotY = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_IDLE_ANIM:
        actor->idleAnim = evt_getArg16_inline();
        return 0;
    case EVT_OP_SET_JUMP_ANIM:
        actor->jumpAnim = evt_getArg16_inline();
        actor->jumpAnimStart = evt_getArg8_inline();
        actor->jumpAnimEnd = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_ANIM_LOOP:
        actor->animFlags = (evt_getArg8_inline() << 3) & 0x38;
        actor->animLoops = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_ANIM_SPEED:
        actor->defaultAnimSpeed = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_IDLE_SPEEDS:
        actor->idleAnimSpeeds.b[0] = evt_getArg8_inline();
        actor->idleAnimSpeeds.b[1] = evt_getArg8_inline();
        actor->idleAnimSpeeds.b[2] = evt_getArg8_inline();
        actor->idleAnimSpeeds.b[3] = evt_getArg8_inline();
        return 0;
    case EVT_OP_SET_ANIM_RANGE:
        actor->animStart = evt_getArg8_inline();
        actor->animEnd = evt_getArg8_inline();
        return 0;
    case EVT_OP_ANIM_OBJ:
        actor = evt_getObjectArg();
    case EVT_OP_ANIM:
        evt_execInstr_anim(actor, evt_getArg16_inline());
        if (actor->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        return 0;
    case EVT_OP_WAIT_ANIM_OBJ:
        actor = evt_getObjectArg();
    case EVT_OP_WAIT_ANIM:
        if (!(actor->animFlags & 0x80)) {
            return 0;
        }
        g_evtCurObj->ip = g_evtInstrStart;
        g_evtArgRead = 1;
        return 1;
    case EVT_OP_ANIM_END:
        evt_onAnimEnd(actor);
        return 0;
    case 0xB5:
        obj = evt_getObjectArg();
        if (obj != NULL) {
            if (obj != po) {
                actor->animSyncSource = obj;
                actor->animFlags = 0x80;
            } else {
                actor->animSyncSource = NULL;
                actor->animFlags = 0;
            }
        }
        return 0;
    case EVT_OP_RUN_THREAD: {
        EvtObject *o;
        a = g_evtCurObj->uid + 0x40;
        o = evt_getObject_inline(a);
        if (o != NULL) {
            evt_deleteObject(o);
        }
        evt_createObject(evt_getArg8_inline(), a, 0x14, 0)->kind = 1;
        return 0;
    }
    case EVT_OP_WAIT_THREAD:
        if (evt_getObject_inline(g_evtCurObj->uid + 0x40) != NULL) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
            return 1;
        }
        return 0;
    case EVT_OP_KILL_THREAD: {
        EvtObject *o = evt_getObject_inline(g_evtCurObj->uid + 0x40);
        if (o != NULL) {
            evt_deleteObject(o);
        }
        return 0;
    }
    case 0x38:
        switch (g_evtCurObj->level) {
        case 0: {
            s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
            btl_setEnemyCmd(x, z, BTL_CMD_ENEMY_DEATH, evt_getArg8_inline());
            break;
        }
        case 1: {
            s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
            btl_setEnemyCmd(x, z, BTL_CMD_ENEMY_COUNTER, evt_getArg8_inline());
            break;
        }
        case 3: {
            s32 x = (u16)D_8007AE10[1], z = (u16)D_8007AE10[0];
            btl_setEnemyCmd(x, z, BTL_CMD_ENEMY_TURN, evt_getArg8_inline());
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
            g_gameState->gameData->battleFlags |= 8;
        } else {
            g_gameState->gameData->battleFlags &= 0xF7;
        }
        return 0;
    case 0xE1:
        rewards_requestClose();
        D_8007B718 = 1;
        return 0;
    case 0xDE:
        a = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)a < 9) {
            u8 *src = msg_getText(evt_getArg16_inline());
            u8 *dst = g_gameState->gameData->characters[a].name;
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
            g_gameState->gameData->characters[a].info.w = (g_gameState->gameData->characters[a].info.w & ~0x400) | ((evt_getArg8_inline() & 1) << 10);
        }
        return 0;
    case EVT_OP_SET_DIR_OFFSETS:
        g_evtState->analogDirOfs = evt_getArg8_inline() << 4;
        g_evtState->padDirOfs = evt_getArg8_inline() << 4;
        return 0;
    case EVT_OP_CLEAR_COLOR:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        game_setClearColor(a, b, evt_getArg8_inline());
        return 0;
    case 0x58:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        fldbg_moveOrigin(a, b, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_SCROLL_TO:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg8_inline();
        fldbg_scrollTo(a, b, c, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_SCROLL_TO_PLAYER:
        a = evt_getArg8_inline();
        fldbg_scrollToPlayer(a, evt_getArg8_inline());
        return 0;
    case 0x71:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        fldbg_setFollow(a, b, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_FOLLOW_HEIGHT:
        a = evt_getArg16_inline();
        fldbg_setFollowHeight(a);
        return 0;
    case 0x73:
        fldbg_enableScrollLimits();
        return 0;
    case EVT_OP_BG_LIMITS_OFF:
        fldbg_disableScrollLimits();
        return 0;
    case EVT_OP_BG_LAYER_COLOR:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg8_inline();
        fldbg_setOverlayColor(a, b, c, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_LAYER_MOVE:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        fldbg_moveOverlay(a, b, c, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_SHOW:
        a = evt_getArg8_inline();
        fldbg_showOverlay(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_LAYER_LOOP:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg16_inline();
        fldbg_setOverlayLoop(a, b, c, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_PARALLAX:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg16_inline();
        fldbg_setOverlayParallax(a, b, c, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_POS:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        fldbg_setOverlayPos(a, b, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_FOLLOW_LAYER:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg8_inline();
        f = evt_getArg8_inline();
        {
            s32 g = evt_getArg8_inline();
            e = evt_getArg8_inline();
            fldbg_addFollowOverlay(a, b, c, f, g, e, evt_getArg8_inline());
        }
        return 0;
    case 0xE6:
        a = evt_getArg8_inline();
        fldbg_setOverlayScreen(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_FRAME:
        a = evt_getArg8_inline();
        fldbg_setAnimFrame(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_PLAY:
        a = evt_getArg8_inline();
        fldbg_playAnim(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_SPEED:
        a = evt_getArg8_inline();
        fldbg_setAnimSpeed(a, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_ANIM_DELAY:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        fldbg_setAnimStepDelay(a, b, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_FLAGS:
        a = evt_getArg8_inline();
        fldbg_setAnimFlags(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_RANGE:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        fldbg_playAnimRange(a, b, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_RESET:
        a = evt_getArg8_inline();
        fldbg_resetAnim(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_ANIM_SHOW_FRAME:
        a = evt_getArg8_inline();
        fldbg_showAnimFrame(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BGI_TRI_ACTIVE:
        a = evt_getArg16_inline();
        bgi_setTriangleActive(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BGI_FLOOR_ACTIVE:
        a = evt_getArg8_inline();
        bgi_setFloorActive(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BGI_ACTOR_ACTIVE:
        a = evt_getArg8_inline();
        if (po->modelId != 0xFFFF) {
            bgi_setActorActive(po->modelSlot, a);
        }
        return 0;
    case EVT_OP_BG_MASK_FILTER:
        a = evt_getArg8_inline();
        bgi_setMaskFilter(a);
        return 0;
    case EVT_OP_BG_SCROLL_LIMITS:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        d = evt_getArg16_inline();
        fldbg_setScrollLimits(a, b, c, d, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_VIEWPORT:
        a = evt_getArg8_inline();
        fldbg_setOverlayViewport(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_VIEWPORT:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        d = evt_getArg16_inline();
        fldbg_setViewport(a, b, c, d, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_VIEWPORT_RATE:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        fldbg_setViewportScrollRate(a, b, evt_getArg16_inline());
        return 0;
    case EVT_OP_BG_LAYER_SKEW:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg16_inline();
        d = evt_getArg16_inline();
        fldbg_setOverlaySkewLoop(a, b, c, d, evt_getArg8_inline());
        return 0;
    case EVT_OP_BG_CAMERA:
        a = evt_getArg8_inline();
        fldbg_setCamera(a);
        return 0;
    case EVT_OP_GET_SCROLL: {
        s16 x, z;
        fldbg_getScroll(&x, &z);
        D_8007AE2C = x;
        D_8007AE30 = z;
        return 0;
    }
    case EVT_OP_OBJ_COLOR:
        obj = evt_getObjectArg();
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        if (fld_setObjectColor(obj->modelSlot, a, b, evt_getArg8_inline())) {
            g_evtCurObj->ip = g_evtInstrStart;
            g_evtArgRead = 1;
            return 1;
        }
        return 0;
    case EVT_OP_MODEL_CMD:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        c = evt_getArg16_inline();
        fld_modelCommand(po->modelSlot, a, b, c, evt_getArg16_inline());
        return 0;
    case EVT_OP_MESH_HIDE:
        o = evt_getObjectArg();
        m = evt_getObjectModelData_inline(o);
        a = evt_getArg8_inline();
        if (m != NULL) {
            m->hiddenGroups |= 1 << a;
        }
        return 0;
    case EVT_OP_MESH_SHOW:
        o = evt_getObjectArg();
        m = evt_getObjectModelData_inline(o);
        a = evt_getArg8_inline();
        if (m != NULL) {
            m->hiddenGroups &= ~(1 << a);
        }
        return 0;
    case EVT_OP_PLAYER_BUBBLE:
        msg_showPlayerBubble(evt_getArg8_inline());
        return 0;
    case EVT_OP_ATE_PROMPT:
        msg_setAtePrompt(evt_getArg8_inline());
        return 0;
    case EVT_OP_HERE_ICON:
        msg_startHereIcon(evt_getArg8_inline());
        return 0;
    case EVT_OP_IDLE_OFF:
        g_evtState->idleTimer = -1;
        return 0;
    case 0xEE:
        g_evtState->idleTimer = 0;
        return 0;
    case 0xB9:
        a = evt_getArg8_inline();
        g_padPortMask[a] |= evt_getArg16_inline();
        return 0;
    case 0xBA:
        a = evt_getArg8_inline();
        g_padPortMask[a] &= ~evt_getArg16_inline();
        return 0;
    case EVT_OP_SOUND:
        a = evt_getArg16_inline();
        g_gameState->gameData->soundHook(a, evt_getArg16_inline(), 0, 0, 0);
        return 0;
    case 0xC6:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg24_inline();
        g_gameState->gameData->soundHook(a, b, c, 0, 0);
        return 0;
    case 0xC7:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg24_inline();
        g_gameState->gameData->soundHook(a, b, c, evt_getArg8_inline(), 0);
        return 0;
    case 0xC8:
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        c = evt_getArg24_inline();
        d = evt_getArg8_inline();
        g_gameState->gameData->soundHook(a, b, c, d, evt_getArg8_inline());
        return 0;
    case EVT_OP_SOUND_POS: {
        u8 vol, pan;
        a = evt_getArg16_inline();
        b = evt_getArg16_inline();
        fld_getSoundPanVolume(a, b, evt_getArg16_inline(), &vol, &pan);
        D_8007AE38 = vol;
        D_8007AE3C = (pan * evt_getArg8_inline()) >> 7;
        if (D_8007AE3C >= 0x80) {
            D_8007AE3C = 0x7F;
        }
        return 0;
    }
    case EVT_OP_SOUND_POS_OBJ: {
        u8 vol, pan;
        po = evt_getObjectArg();
        fld_getSoundPanVolume(po->pos.vx, po->pos.vy, po->pos.vz, &vol, &pan);
        D_8007AE38 = vol;
        D_8007AE3C = (pan * evt_getArg8_inline()) >> 7;
        if (D_8007AE3C >= 0x80) {
            D_8007AE3C = 0x7F;
        }
        return 0;
    }
    case EVT_OP_SPRITE_CMD:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg16_inline();
        d = evt_getArg16_inline();
        fld_spriteCommand(a, b, c, d, evt_getArg16_inline());
        return 0;
    case 0xDA:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        c = evt_getArg8_inline();
        d = evt_getArg16_inline();
        fld_spriteCommand(a, b, c, d, evt_getArg16_inline());
        return 0;
    case EVT_OP_MENU:
        a = evt_getArg8_inline();
        menu_open(a, evt_getArg8_inline());
        evt_onMenuOpen();
        return 1;
    case 0xAC:
        a = evt_getArg16_inline();
        fld_requestDiscChange((a >> 14) & 3, a & 0x3FFF);
        return 1;
    case EVT_OP_PAD_UNLOCK_MENU:
        g_padPortMask[0] &= ~0x40000;
        return 0;
    case EVT_OP_PAD_LOCK_MENU:
        g_padPortMask[0] |= 0x40000;
        return 1;
    case EVT_OP_ADD_ITEM:
        a = evt_getArg16_inline();
        b = evt_getArg8_inline();
        if (a < 0x100) {
            item_add(a, b);
        } else if (a < 0x200) {
            keyItem_add(a - 0x100);
        } else {
            while (--b != -1) {
                card_add(a - 0x200);
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
        if ((g_gameState->gameData->gil += evt_getArg24_inline()) > 9999999) {
            g_gameState->gameData->gil = 9999999;
        }
        return 0;
    case 0xCF:
        if ((g_gameState->gameData->gil -= evt_getArg24_inline()) > 9999999) {
            g_gameState->gameData->gil = 0;
        }
        return 0;
    case EVT_OP_SET_RECRUITED:
        a = evt_getArg16_inline();
        a |= (a >> 4) & 0xE0;
        for (i = 0; i < 9; i++) {
            chr_clearRecruited(i);
        }
        for (i = 0; i < 9; i++) {
            if (a & 1) {
                chr_setRecruited(i);
            }
            a >>= 1;
        }
        return 0;
    case EVT_OP_PARTY_JOIN:
        a = evt_execInstr_chr(evt_getArg8_inline());
        b = evt_getArg8_inline();
        c = evt_getArg8_inline();
        if ((u32)a < 9) {
            Character *ch = &g_gameState->gameData->characters[a];
            d = evt_getArg8_inline();
            if (d != 0xFF) {
                ch->classFlags = d;
            }
            d = evt_getArg8_inline();
            if (d != 0xFF) {
                ch->info.w = (ch->info.w & 0xFFFE0FFF) | ((d & 0x1F) << 12);
            }
            chr_join(a, b & 0xFF, c);
        } else {
            evt_getArg8_inline();
            evt_getArg8_inline();
        }
        return 0;
    case EVT_OP_PARTY_REMOVE:
        a = evt_execInstr_chr(evt_getArg8_inline());
        for (i = 0; i < 4; i++) {
            Character *ch = g_gameState->gameData->party[i];
            if (ch != NULL && (ch->info.w & 0xF) == a) {
                break;
            }
        }
        if (i < 4) {
            party_setMember(i, -1);
            evt_execInstr_party();
        }
        return 0;
    case 0xB2:
        D_8007AE70.size = evt_getArg8_inline();
        a = 0;
        for (i = 8; i >= 0; i--) {
            a <<= 1;
            a |= (g_gameState->gameData->characters[i].info.w >> 11) & 1;
        }
        b = evt_getArg16_inline();
        for (i = 0; i < 9; i++) {
            D_8007AE70.fixed[i] = b & 1;
            b >>= 1;
        }
        for (i = 0; i < 4; i++) {
            Character *ch = g_gameState->gameData->party[i];
            if (ch != NULL) {
                b = ch->info.w & 0xF;
                a &= ~(1 << b);
                D_8007AE70.party[i] = b;
            } else {
                D_8007AE70.party[i] = 0xFF;
            }
        }
        for (b = 0, i = 0; i < 9 && b < 8 && a != 0; i++, a >>= 1) {
            if (a & 1) {
                D_8007AE70.reserve[b++] = i;
            }
        }
        for (; b < 8; b++) {
            D_8007AE70.reserve[b] = 0xFF;
        }
        menu_open(3, (s32)&D_8007AE70);
        return 1;
    case 0xE9:
        evt_execInstr_party();
        return 0;
    case EVT_OP_SET_HP:
        a = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)a < 9) {
            Character *ch = &g_gameState->gameData->characters[a];
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
            Character *ch = &g_gameState->gameData->characters[a];
            a = evt_getArg16_inline();
            if (ch->maxMp < a) {
                a = ch->maxMp;
            }
            ch->mp = a;
        }
        return 0;
    case 0xF3:
        a = evt_execInstr_chr(evt_getArg8_inline());
        chr_resetAp(a, evt_getArg8_inline());
        return 0;
    case 0xF4:
        a = evt_execInstr_chr(evt_getArg8_inline());
        chr_learnAbility(a, evt_getArg8_inline());
        return 0;
    case EVT_OP_CURE_STATUS:
        a = evt_execInstr_chr(evt_getArg8_inline());
        chr_cureStatusBit(&g_gameState->gameData->characters[a], evt_getArg8_inline() & 0xFF);
        return 0;
    case 0x62:
        a = evt_execInstr_chr(evt_getArg8_inline());
        if ((u32)a < 9) {
            g_gameState->gameData->characters[a].info.w = (g_gameState->gameData->characters[a].info.w & ~0x200) | ((evt_getArg8_inline() & 1) << 9);
        }
        return 0;
    case 0xE0:
        if (g_gameState->gameData->frogCount < 0x7FFF) {
            g_gameState->gameData->frogCount++;
        }
        return 0;
    case 0xB0: {
        u8 *src = msg_getText(evt_getArg16_inline());
        u8 *dst = g_evtLocationNameBuf;
        u8 *end = dst + 0xF;
        while (dst < end) {
            *dst++ = *src++;
        }
        *dst = 0xFF;
        g_gameState->gameData->locationName = g_evtLocationNameBuf;
        return 0;
    }
    case 0xB1:
        g_gameState->gameData->locationName = g_evtLocationName;
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
    case EVT_OP_FADE:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        fade_clearEase(a & 1);
        fade_setAngle(((0x80 - evt_getArg8_inline()) << 4) & 0xFFF0);
        c = evt_getArg8_inline();
        d = evt_getArg8_inline();
        fade_setOtz(1);
        game_startFade(((a >> 2) & 1) + 1, (a >> 1) & 1, b, c | (d << 8) | (evt_getArg8_inline() << 16));
        return 0;
    case EVT_OP_OBJ_SCREEN_POS:
        obj = evt_getObjectArg();
        if (obj != NULL) {
            sv.vx = obj->pos.vx;
            sv.vy = obj->pos.vy;
            sv.vz = obj->pos.vz;
            evt_projectToScreen(&sv, &D_8007AE2C, &D_8007AE30);
        }
        return 0;
    case EVT_OP_MOVIE:
        a = evt_getArg8_inline();
        b = evt_getArg16_inline();
        fld_movieCommand(a, b, evt_getArg8_inline());
        return 0;
    case 0xB7:
        if (g_evtMode == 3) {
            VECTOR *v = wm_getCameraEye();
            actor->pos.vx = v->vx;
            actor->pos.vy = v->vy;
            actor->pos.vz = v->vz;
            actor->moveFlags |= 0x40;
        }
        return 0;
    case 0xB8:
        if (g_evtMode == 3) {
            VECTOR *v = wm_getCameraTarget();
            actor->pos.vx = v->vx;
            actor->pos.vy = v->vy;
            actor->pos.vz = v->vz;
            actor->moveFlags |= 0x80;
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
        vib_play(evt_getArg8_inline());
        return 0;
    case EVT_OP_VIB_SET_PLAYING:
        vib_setPlaying(evt_getArg8_inline());
        return 0;
    case EVT_OP_VIB_TRACK:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        vib_selectTrack(a, b, evt_getArg8_inline());
        return 0;
    case 0xF9:
        a = evt_getArg8_inline();
        b = evt_getArg8_inline();
        vib_setTrackEnabled(a, b, evt_getArg8_inline());
        return 0;
    case 0xFA:
        vib_setRate(evt_getArg16_inline());
        return 0;
    case 0xFB:
        vib_setLoopMode(evt_getArg8_inline());
        return 0;
    case 0xFC:
        a = evt_getArg8_inline();
        vib_setRange(a, evt_getArg8_inline());
        return 0;
    case 0xFD:
        a = evt_getArg8_inline();
        ((s32 (*)(s32, s32))g_gameState->gameData->preloadRequest)(a & 0xFF, evt_getArg16_inline() & 0xFFFF);
        return 0;
    case 0xFF:
        /* padding between the two jump tables in the original rodata */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        op = g_evtArgFlags | 0xFF00;
        g_evtArgFlags = *g_evtCurObj->ip++;
        switch (op) {
        case EVT_OP_BGI_FLOOR_MOVE_START:
            a = evt_getArg8_inline();
            bgi_startFloorMove(a, evt_getArg8_inline());
            return 0;
        case 0xFF01:
            a = evt_getArg8_inline();
            bgi_stopFloorMove(a, evt_getArg8_inline());
            return 0;
        case 0xFF02:
            a = evt_getArg8_inline();
            bgi_setFloorMovePlaying(a, evt_getArg8_inline());
            return 0;
        case 0xFF03:
            a = evt_getArg8_inline();
            bgi_setFloorMoveMode(a, evt_getArg8_inline());
            return 0;
        case 0xFF04:
            a = evt_getArg8_inline();
            bgi_setFloorMoveFloor(a, evt_getArg8_inline());
            return 0;
        case 0xFF05:
            a = evt_getArg8_inline();
            bgi_setFloorMoveSpeed(a, evt_getArg16_inline());
            return 0;
        case 0xFF06:
            a = evt_getArg8_inline();
            bgi_setFloorMoveCurve(a, evt_getArg8_inline());
            return 0;
        case 0xFF07:
            a = evt_getArg8_inline();
            b = evt_getArg16_inline();
            bgi_setFloorMoveRange(a, b, evt_getArg16_inline());
            return 0;
        case 0xFF08:
            a = evt_getArg8_inline();
            bgi_setFloorMoveAxis(a, evt_getArg8_inline());
            return 0;
        case 0xFF09:
            a = evt_getArg8_inline();
            bgi_setAnimFrame(a, evt_getArg8_inline());
            return 0;
        case 0xFF0A:
            a = evt_getArg8_inline();
            bgi_showAnimFrame(a, evt_getArg8_inline());
            return 0;
        case 0xFF0B:
            a = evt_getArg8_inline();
            bgi_setAnimPlaying(a, evt_getArg8_inline());
            return 0;
        case 0xFF0C:
            a = evt_getArg8_inline();
            bgi_setAnimMode(a, evt_getArg8_inline());
            return 0;
        case 0xFF0D:
            a = evt_getArg8_inline();
            bgi_setAnimRate(a, evt_getArg16_inline());
            return 0;
        case 0xFF0E:
            a = evt_getArg8_inline();
            bgi_setAnimFrameValues(a, evt_getArg8_inline());
            return 0;
        case 0xFF0F:
            a = evt_getArg8_inline();
            b = evt_getArg8_inline();
            bgi_setAnimFrameValue(a, b, evt_getArg8_inline());
            return 0;
        case 0xFF10:
            a = evt_getArg8_inline();
            b = evt_getArg8_inline();
            bgi_playAnimRange(a, b, evt_getArg8_inline());
            return 0;
        case 0xFF11:
            a = evt_getArg8_inline();
            bgi_resetAnim(a, evt_getArg8_inline());
            return 0;
        }
        return 0;
    default:
        switch (g_evtMode) {
        case 1:
            return evt_modelOpField(po, op);
        case 2:
            return evt_modelOpBattle(po, op);
        case 3:
            return evt_modelOpWorld(po, op);
        }
        return 1;
    }
}

void evt_movePlayer(EvtObject *obj) {
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
    EvtObject *o;
    s32 m;

    o = NULL;
    speed = 30;
    dir = 0x8000;
    analog = 0;
    memset(&v, 0, 8);
    pad = msg_getPadHeld();
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
            dir = math_atan2(x, y);
            x *= x;
            m = y * y;
            if (m < x) {
                m = x;
            }
            speed = (m * 16 - m) * 4 >> 14;
        }
    }
    if (!analog) {
        run = ((GameConfig *)&g_gameState->config)->movementRun;
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
    g_evtPlayerMoved = dir != 0x8000;
    if (dir != 0x8000) {
        if (analog) {
            dir += g_evtState->analogDirOfs;
        } else {
            dir += g_evtState->padDirOfs;
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
        if (g_evtState->walkOnly != 0 && speed > 30) {
            speed = 30;
        }
        if (speed > 30) {
            g_evtStepCount += 2;
        } else {
            g_evtStepCount += 1;
        }
        obj->speed = speed;
        v.vz = -speed;
        func_800530A8(&v, 0, dir);
        obj->pos.vx += v.vx;
        obj->pos.vz += v.vz;
        o = evt_findNearestObject(obj, 0, &dist);
        g_evtPushable = ((o->flags >> 4) ^ 1) & 1;
        if (g_evtPushable == 0) {
            g_evtPushFrames = 0;
        }
        if (o != NULL && dist <= 0 && g_evtPushFrames >= 0) {
            obj->collidedObject = o;
            obj->collisionDepth = dist;
            c = evt_angleToObject(obj, o, dir);
            if ((u32)(c + 0x400) <= 0x800) {
                obj->pos.vx -= v.vx;
                obj->pos.vz -= v.vz;
                k = math_sin(c);
                v.vx = (v.vx * k) >> 13;
                v.vz = (v.vz * k) >> 13;
                func_800530A8(&v, 0, 0x400);
                obj->pos.vx += v.vx;
                obj->pos.vz += v.vz;
                if (evt_findNearestObject(obj, 0, &dist) != NULL && dist <= 0) {
                    obj->pos.vx -= v.vx;
                    obj->pos.vz -= v.vz;
                }
                g_evtPlayerBlocked = 2;
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
    g_evtPushFrames = g_evtPlayerBlocked != 0 ? (g_evtPushFrames < 25 ? g_evtPushFrames + g_evtPushable : -25)
                                 : (g_evtPushFrames < 0 ? g_evtPushFrames + 1 : 0);
}

s32 func_80047AB8(EvtObject *p, s32 b) {
    s32 *sp;
    s32 ok;
    s32 x;
    u32 hi;
    s32 w;

    if (!(p->moveFlags & 0x400)) {
        ok = 0;
        if (!(p->animFlags & 0x80) && p->talkTurnScript == 0xA7) {
            ok = 0 < p->level;
        }
        if (ok) {
            p->unk70 = b >> 4;
            sp = &((s32 *)p + p->stackOfs)[p->sp];
            sp[0] = p->ip - g_evtState->scriptBase;
            w = p->wait | (p->level << 8);
            hi = g_evtCurObj->uid << 24;
            x = hi | 0xFF0000;
            sp[1] = w | x;
            p->ip = (u8 *)&p->talkTurnScript;
            p->level = 0;
            p->wait = 0;
            p->sp += 2;
        }
    }
    return 1;
}

/* point 500 units ahead of o at angle a, at head height */
static inline void evt_updateLookAt_ahead(EvtObject *o, s32 a, VECTOR *out) {
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
    v.vy = (o->height * (s8)o->scale) >> 6;
    v.vz = -500;
    gte_ldv0(&v);
    gte_rtv0tr();
    gte_stlvnl(out);
}

void evt_relaxLookAt(EvtObject *o, Unk80047B90 *m) {
    VECTOR out;
    s32 d;

    evt_updateLookAt_ahead(o, o->pos.rotY, &out);
    d = math_length3(out.vx - o->lookAtX, out.vy - o->lookAtY, out.vz - o->lookAtZ);
    if (d < 100) {
        d = 100;
        o->moveFlags &= 0xF8FF;
    }
    o->lookAtX += (out.vx - o->lookAtX) * 100 / d;
    o->lookAtY += (out.vy - o->lookAtY) * 100 / d;
    o->lookAtZ += (out.vz - o->lookAtZ) * 100 / d;
    m->flags |= 1;
    m->lookAtX = o->lookAtX;
    m->lookAtY = o->lookAtY;
    m->lookAtZ = o->lookAtZ;
}

/* move the look-at point towards t */
static inline void evt_updateLookAt_approach(EvtObject *o, Unk80047B90 *mdl, VECTOR *t) {
    s32 d;

    d = math_length3(t->vx - o->lookAtX, t->vy - o->lookAtY, t->vz - o->lookAtZ);
    if (d < 100) {
        d = 100;
    }
    o->lookAtX += (t->vx - o->lookAtX) * 100 / d;
    o->lookAtY += (t->vy - o->lookAtY) * 100 / d;
    o->lookAtZ += (t->vz - o->lookAtZ) * 100 / d;
    mdl->flags |= 1;
    mdl->lookAtX = o->lookAtX;
    mdl->lookAtY = o->lookAtY;
    mdl->lookAtZ = o->lookAtZ;
}

/* is the look-at point still in front of o (within ~80 degrees)? */
static inline s32 evt_updateLookAt_infront(EvtObject *o) {
    s32 ok = 1;
    s32 d;

    if (!(o->moveFlags & 0x400)) {
        d = (math_atan2(o->lookAtX - o->pos.vx, o->lookAtZ - o->pos.vz) - o->pos.rotY) & 0xFFF;
        if (d > 0x800) {
            d -= 0x1000;
        }
        ok = (u32)(d + 0x37F) < 0x6FF;
        if (!ok) {
            o->moveFlags &= ~0x100;
        }
    }
    return ok;
}

static inline s32 evt_updateLookAt_infront2(EvtObject *o, s32 *pa) {
    s32 ok = 1;
    s32 a;
    s32 d;

    if (!(o->moveFlags & 0x400)) {
        a = math_atan2(o->lookAtX - o->pos.vx, o->lookAtZ - o->pos.vz);
        d = (a - o->pos.rotY) & 0xFFF;
        if (d > 0x800) {
            d -= 0x1000;
        }
        ok = (u32)(d + 0x37F) < 0x6FF;
        if (!ok) {
            o->moveFlags &= ~0x100;
        }
        *pa = a;
    }
    return ok;
}

void evt_updateLookAt(EvtObject *o) {
    VECTOR t;
    s32 ang;
    s32 save;
    s32 x, y, z;
    Unk80047B90 *mdl;
    EvtObject *tgt;
    EvtObject *best;
    EvtObject *q;
    Unk16D8 *n;
    u8 mask;
    s32 bestd;
    s32 d;
    s32 a;
    s32 ok;

    save = 0;
    mdl = (Unk80047B90 *)((ModelSlot *)game_getModel(o->modelSlot))->model;
    tgt = NULL;
    if ((o->moveFlags & 4) && g_evtLookTarget != NULL && g_evtLookTarget != o &&
        math_dist2(o->pos.vx - g_evtLookTarget->pos.vx, o->pos.vz - g_evtLookTarget->pos.vz, 0) >= 1000) {
        tgt = g_evtLookTarget;
        o->moveFlags |= 0x200;
    }
    if (tgt != NULL || ((o->moveFlags & 1) && !(o->animFlags & 2))) {
        if (o->flags & 0x80) {
            save = o->pos.rotY;
            o->pos.rotY = o->turnStartDir;
        }
        if (tgt == NULL) {
            best = NULL;
            x = o->pos.vx;
            y = o->pos.vy;
            z = o->pos.vz;
            mask = o->lookParam1;
            bestd = 0;
            for (n = g_evtState->objects; n != NULL; n = n->unk0) {
                q = n->unk4;
                if (q != o && q->kind == 4 && (u32)(q->pos.vy - y + 399) < 799 && (q->moveFlags & 2) &&
                    (mask & q->lookParam0)) {
                    d = math_distSq2(q->pos.vx - x, q->pos.vz - z, 0);
                    if (d >= 1000) {
                        a = evt_angleToObject(o, q, o->pos.rotY);
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
            if (!(o->moveFlags & 0x100)) {
                if (!(o->flags & 0x80)) {
                    o->moveFlags |= 0x100;
                    evt_updateLookAt_ahead(o, o->pos.rotY, &t);
                    o->lookAtX = t.vx;
                    o->lookAtY = t.vy;
                    o->lookAtZ = t.vz;
                }
            }
            if ((o->moveFlags & 0x100) && evt_updateLookAt_infront(o)) {
                {
                    t.vx = tgt->pos.vx;
                    t.vy = tgt->pos.vy;
                    t.vz = tgt->pos.vz;
                    ang = math_atan2(t.vx - o->pos.vx, t.vz - o->pos.vz);
                    d = (ang - o->pos.rotY) & 0xFFF;
                    if (d > 0x800) {
                        d -= 0x1000;
                    }
                    if ((u32)(d + 0x27F) < 0x4FF) {
                        t.vy = (((o->pos.vy << 6) + o->height * (s8)o->scale) * 3 + (t.vy << 6) +
                                tgt->height * (s8)tgt->scale) >> 8;
                        evt_updateLookAt_approach(o, mdl, &t);
                    } else if (func_80047AB8(o, ang)) {
                        a = d > 0x280 ? 0x280 : -0x280;
                        evt_updateLookAt_ahead(o, a + o->pos.rotY, &t);
                        evt_updateLookAt_approach(o, mdl, &t);
                    }
                }
            }
        } else if ((o->moveFlags & 0x100) && evt_updateLookAt_infront2(o, &ang) && func_80047AB8(o, ang)) {
            {
                evt_relaxLookAt(o, mdl);
            }
        }
        if (o->flags & 0x80) {
            o->pos.rotY = save;
        }
    } else if ((o->moveFlags & 0x100) && evt_updateLookAt_infront(o)) {
        evt_relaxLookAt(o, mdl);
    }
    if (!(o->moveFlags & 0x100)) {
        mdl->flags &= ~1;
    }
}

void evt_updateIdleTimer(void) {
    EvtState *e = g_evtState;
    EvtObject *p;
    EvtObject *o;
    EvtState *e2;

    if (e == NULL || e->controlEnabled == 0) {
        return;
    }
    e->idleTimer -= e->idleTimer > 0;
    if (e->idleTimer != 0) {
        return;
    }
    o = evt_getObject_inline(e->playerUid);
    if (o == NULL || o->kind != 4) {
        return;
    }
    p = o;
    if (p->animFrame != p->animFrames - 1) {
        return;
    }
    e2 = g_evtState;
    if (e2->idleTimer >= 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        e2->idleTimer = (*g_randPtr++ + 0xC8) * 2;
    }
    if (p->idleAnim != 0 && !(p->animFlags & 0x84)) {
        evt_updateIdleTimer_idle(p);
        if (p->uid == g_evtState->playerUid) {
            g_evtPlayerAnimCount++;
        }
        p->animFlags |= 2;
    }
}

/* Nearest kind-3 object whose script has entry `key` and that evt_isInRegion
 * accepts for p's position. */
static inline EvtObject *evt_checkPlayerAction_find(EvtObject *p, s32 key) {
    EvtObject *found = NULL;
    s32 x = p->pos.vx;
    Unk16D8 *n = g_evtState->objects;
    s32 z = p->pos.vz;
    EvtObject *o;

    for (; n != NULL; n = n->unk0) {
        o = n->unk4;
        if (o->kind == 3 && evt_callObjectFunc_find(o->scriptIdx, key) != NULL && evt_isInRegion(o, x, z)) {
            found = o;
            break;
        }
    }
    return found;
}

void evt_checkPlayerAction(EvtObject *p) {
    s32 btn;
    s32 t;
    EvtObject *o;

    t = msg_getPadPressed();
    btn = t & (g_evtMode == 1 ? (BTN_CROSS | BTN_SQUARE) : BTN_CROSS);
    if (btn && (o = evt_findNearestObject(p, 4, NULL)) != NULL) {
        g_evtTalkAngle = evt_angleToObject(p, o, p->pos.rotY);
        if ((u32)(g_evtTalkAngle + 0x3FF) < 0x7FF) {
            p->talkTarget = o->uid;
            if (btn == BTN_SQUARE && evt_callObjectFunc(o, 1, 8, 0)) {
                return;
            }
            if (evt_callObjectFunc(o, 1, 3, 0)) {
                return;
            }
        }
        o = NULL;
    } else {
        o = evt_findNearestObject(p, 2, NULL);
    }
    if (o != NULL) {
        evt_callObjectFunc(o, 1, 2, 0);
    }
    if (btn) {
        o = evt_checkPlayerAction_find(p, 3);
        if (o != NULL) {
            if (btn == BTN_SQUARE && evt_callObjectFunc(o, 1, 8, 0)) {
                return;
            }
            evt_callObjectFunc(o, 1, 3, 0);
            o = NULL;
        } else {
            o = evt_checkPlayerAction_find(p, 2);
        }
    } else {
        o = evt_checkPlayerAction_find(p, 2);
    }
    if (o != NULL) {
        evt_callObjectFunc(o, 1, 2, 0);
    }
}

void evt_updateBattleObjects(void) {
    s32 i;
    s32 m;
    Unk8007AED0 *o;

    if (g_gameState->gameData->ctx.ovl65->phase != 0) {
        i = 3;
        D_8007AE10[i] = btl_getUnitMask(1);
        m = (u16)D_8007AE10[i];
        for (i = 4; i < 8; i++) {
            o = g_evtUnitObjects[i];
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
static inline void evt_updateObjects_turn(EvtObject *p) {
    s32 a;

    if ((s16)p->turnRate == 0x7FFE) {
        a = p->pos.rotY + (s16)p->turnStep;
        if (p->turnTarget < a) {
            p->pos.rotY = a;
        } else {
            p->pos.rotY = p->turnTarget;
            p->flags &= 0x7F;
        }
    } else if ((s16)p->turnRate == 0x7FFF) {
        a = p->pos.rotY + (s16)p->turnStep;
        if (a >= p->turnTarget) {
            p->pos.rotY = p->turnTarget;
            p->flags &= 0x7F;
        } else {
            p->pos.rotY = a;
        }
    }
}

/* remember the position at the start of the frame */
static inline void evt_updateObjects_save(EvtObject *p) {
    p->prevPos.vx = p->pos.vx;
    p->prevPos.vy = p->pos.vy;
    p->prevPos.vz = p->pos.vz;
}

s32 evt_updateObjects(void) {
    Unk16D8 *n;
    EvtObject *o;
    EvtObject *p;
    EvtObject *t;
    EvtObject *t1;
    EvtObject *x;
    Unk16D8 *y;
    EvtObject *z;
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
    EvtState *s;

    g_evtPlayerMoved = 0;
    world = g_evtMode == 2;
    for (n = g_evtState->objects; n != NULL; n = n->unk0) {
        o = n->unk4;
        if (o->kind == 4) {
            z = o;
            if (o->uid == g_evtState->playerUid && o->state == 1) {
                if (g_evtState->controlEnabled != 0) {
                    if (g_evtMode == 1) {
                        o->prevPos.vx = o->pos.vx;
                        o->prevPos.vy = o->pos.vy;
                        o->prevPos.vz = o->pos.vz;
                        evt_movePlayer(o);
                    } else if (g_evtMode == 3) {
                        g_evtPlayerMoved = g_worldPlayerStepped;
                        evt_checkPlayerAction(o);
                        t1 = evt_findNearestObject(o, 0, &dist);
                        if (t1 != NULL && dist <= 0 &&
                            (u32)(evt_angleToObject(o, t1, math_atan2(o->pos.vx - o->prevPos.vx, o->pos.vz - o->prevPos.vz)) + 0x400) <= 0x800) {
                            o->pos.vx = o->prevPos.vx;
                            o->pos.vy = o->prevPos.vy;
                            o->pos.vz = o->prevPos.vz;
                        }
                    }
                    if (g_evtPlayerMoved != 0) {
                        s = g_evtState;
                        if (s->idleTimer >= 0) {
                            s->idleTimer = (evt_rollEncounter_rand() + 200) * 4;
                        }
                    }
                } else {
                    evt_updateObjects_save(o);
                }
            } else {
                evt_updateObjects_save(z);
            }
            if (o->state == 1 && !world) {
                evt_stepAnim(o);
            }
        }
    }
    if (world) {
        evt_updateBattleObjects();
    }
    l = (Unk16D8 *)D_8007B6FC;
    g_evtPosSetThisFrame = 0;
    if (l == NULL) {
        l = g_evtState->objects;
    }
    r = evt_runScripts(l);
    if (r == 6) {
        r = 0;
    } else {
        D_8007B6FC = 0;
    }
    D_8007AEBC = NULL;
    D_8007AEB8 = NULL;
    if (g_evtMode == 1) {
        fldbg_getCamera(&b);
        g_evtCameraChanged = b != g_evtCamera;
        g_evtCamera = b;
    }
    if (world) {
        for (n = g_evtState->objects; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->unkB == 2 && evt_callObjectFunc(o, 3, 5, 0)) {
                o->unkB = 1;
            }
        }
    } else {
        for (n = g_evtState->objects; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->kind == 4) {
                p = o;
                if (g_evtMode == 1) {
                    f = evt_updateActorWalkmesh(p);
                    if ((p->uid != g_evtState->playerUid || g_evtPushFrames >= 0) && (f & 1) &&
                        (t = evt_findNearestObject(p, 0, NULL)) != NULL) {
                        bgi_revertActor(p->modelSlot);
                        if (t->flags & 0x10) {
                            g_evtPushFrames = 0;
                        }
                        if (o->uid == g_evtState->playerUid) {
                            g_evtPlayerBlocked = 2;
                        }
                    }
                    if (o->uid == g_evtState->playerUid) {
                        msg_drawHereIcon(p);
                        if (g_evtState->controlEnabled != 0) {
                            evt_checkPlayerAction(p);
                        }
                    }
                } else if (g_evtMode == 3) {
                    if (p->moveFlags & 0x40) {
                        VECTOR *v = wm_getCameraEye();
                        v->vx = p->pos.vx;
                        v->vy = p->pos.vy;
                        D_8007AEB8 = p;
                        v->vz = p->pos.vz;
                    } else if (p->moveFlags & 0x80) {
                        VECTOR *v = wm_getCameraTarget();
                        v->vx = p->pos.vx;
                        v->vy = p->pos.vy;
                        D_8007AEBC = p;
                        v->vz = p->pos.vz;
                    }
                }
                if (p->balloonWindow != 0xFF) {
                    w = msg_getWindow(p->balloonWindow);
                    if (w != NULL) {
                        evt_followBalloon(p, w);
                    } else {
                        p->balloonWindow = 0xFF;
                        if (g_evtLookTarget == p) {
                            D_8007AE6C = 30;
                        }
                    }
                }
                moved = 0;
                dx = p->pos.vx - p->prevPos.vx;
                dy = p->pos.vy - p->prevPos.vy;
                dz = p->pos.vz - p->prevPos.vz;
                if (dx != 0 || dz != 0) {
                    moved = 1;
                }
                if (g_evtMode == 1) {
                    if (p->flags & 0x80) {
                        evt_updateObjects_turn(p);
                    }
                    if (p->modelId != 0xFFFF) {
                        evt_updateLookAt(p);
                    }
                }
                if (g_evtMode == 3 && (p->flags & 0x80)) {
                    evt_updateObjects_turn(p);
                }
                if (moved) {
                    if ((p->animFlags & 0x86) == 0 || (p->animFlags & 0x86) == 0x82) {
                        p->animFlags &= 0x7D;
                        anim = p->speed >= p->runSpeed ? (u16)p->runAnim : (u16)p->walkAnim;
                        goto set_anim;
                    }
                } else if (!(p->animFlags & 0x84) && (g_evtPlayerBlocked == 0 || o->uid != g_evtState->playerUid)) {
                    anim = (u16)p->standAnim;
                set_anim:
                    if (anim != p->anim) {
                        p->anim = anim;
                        p->animFrame = 0;
                        p->animSubFrame = 0;
                        p->animFrames = evt_getAnimFrameCount(p->modelId, anim);
                        p->animSpeed = p->defaultAnimSpeed;
                    }
                }
                if (moved && o->uid == g_evtState->playerUid && g_evtPlayerMoved != 0) {
                    g_evtEncounterDist += math_length3(dx, dy, dz);
                    if (evt_rollEncounter(p)) {
                        g_evtEncounterPending = 1;
                    }
                }
            }
        }
    }
    if (g_evtPlayerBlocked > 0) {
        g_evtPlayerBlocked--;
    }
    if (D_8007AE6C > 0 && --D_8007AE6C == 0) {
        for (y = g_evtState->objects; y != NULL; y = y->unk0) {
            x = y->unk4;
            if (x->kind == 4 && (x->moveFlags & 0x200)) {
                x->moveFlags |= 0x400;
            }
        }
        g_evtLookTarget = NULL;
    }
    if (g_evtEncounterPending != 0 && g_evtPosSetThisFrame == 0) {
        g_evtEncounterPending = 0;
        r = 3;
    }
    if (r == 3 || r == 7) {
        if (g_evtMode == 1) {
            for (m = g_evtState->objects; m != NULL; m = m->unk0) {
                p = m->unk4;
                if (p->kind == 4 && p->modelId != 0xFFFF) {
                    q = bgi_getActor(p->modelSlot);
                    bgi_getActorRadius(p->modelSlot, &h);
                    p->savedWalkRadius = h / 4;
                    if (q != NULL) {
                        p->pos.vx = p->prevPos.vx;
                        p->pos.vy = p->prevPos.vy;
                        p->pos.vz = p->prevPos.vz;
                        p->savedWalkFloor = q->walkFloor;
                        p->savedWalkTri = q->walkTri;
                        p->savedWalkFlags = q->walkFlags;
                    }
                }
            }
            fld_enableRain(0, 0, 0);
        }
        if (r == 7) {
            g_evtSavedState = g_evtStateData;
        }
    }
    return r;
}

void evt_onAnimEnd(EvtObject *p) {
    s32 v;
    u32 *q;

    if (!(p->animFlags & 8)) {
        v = (u16)p->standAnim;
        if (v != p->anim) {
            p->anim = v;
            p->animFrame = 0;
            p->animSubFrame = 0;
            p->animFrames = evt_getAnimFrameCount(p->modelId, v);
            p->animSpeed = p->defaultAnimSpeed;
        }
        p->animFrame = 0;
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        p->animSpeed = p->idleAnimSpeeds.b[*g_randPtr++ & 3];
        p->animFlags &= 0x39;
        if (p->flags & 0x80) {
            func_8004B9B8_inline(p);
        }
        if (p->moveFlags & 8) {
            p->moveFlags &= ~8;
            q = (u32 *)game_getModel(p->modelSlot);
            *q &= ~0x10;
            p->animStart = 0;
            p->animEnd = 0xFF;
        }
    } else {
        p->animFlags = (p->animFlags | 4) & 0x7D;
        p->animFrame = p->animPrevFrame;
    }
}

static inline void evt_onAnimLimit_bounce(EvtObject *o, u8 f) {
    u8 t;
    u8 pos;

    if (f & 0x20) {
        pos = o->animFrame;
        if (!(f & 0x40)) {
            o->animFrame = pos - 2;
        } else {
            o->animFrame = pos + 2;
        }
        o->animFlags ^= 0x40;
        t = o->animStart;
        o->animStart = o->animEnd;
        o->animEnd = t;
    } else if (f & 0x10) {
        o->animFrame = o->animStart;
    }
}

void evt_onAnimLimit(EvtObject *o) {
    u8 f = o->animFlags;

    if (f & 0x30) {
        if (o->animLoops != 0) {
            o->animLoops--;
            if (o->animLoops != 0) {
                evt_onAnimLimit_bounce(o, f);
            } else {
                evt_onAnimEnd(o);
            }
        } else {
            evt_onAnimLimit_bounce(o, f);
        }
    } else {
        evt_onAnimEnd(o);
    }
}

void evt_stepAnim(EvtObject *p) {
    EvtObject *q;
    u8 f;
    s32 last;
    s32 t;
    s32 lo;
    u8 old;
    s32 t2, lo2, t3, lo3;
    u8 old2, old3;

    f = p->animFlags;
    if (f & 4) {
        return;
    }
    if (f & 0x80) {
        if (p->animSyncSource != NULL) {
            q = p->animSyncSource;
            if (q->anim == (u16)q->standAnim) {
                p->anim = p->standAnim;
            } else if (q->anim == (u16)q->walkAnim) {
                p->anim = p->walkAnim;
            } else if (q->anim == (u16)q->runAnim) {
                p->anim = p->runAnim;
            }
            p->animFrame = q->animFrame;
        } else if (!(f & 0x40)) {
            last = p->animEnd;
            if (p->animFrames - 1 < last) {
                last = p->animFrames - 1;
            }
            t = p->animFrame * 16 + (s8)p->animSubFrame + p->animSpeed;
            lo = t & 0xF;
            p->animSubFrame = lo;
            if (p->animFlags & 0x40) {
                p->animSubFrame = lo - 0xF;
            }
            old = p->animFrame;
            t >>= 4;
            p->animFrame = t;
            p->animPrevFrame = old;
            if (last < t) {
                evt_onAnimLimit(p);
            }
            if (p->flags & 0x80) {
                p->pos.rotY = (p->pos.rotY + p->turnRate) & 0xFFF;
                p->turnStartDir = (p->turnStartDir + p->turnStep) & 0xFFF;
            }
        } else {
            t2 = p->animFrame * 16 + (s8)p->animSubFrame - p->animSpeed;
            t2 += 0xF;
            lo2 = t2 & 0xF;
            p->animSubFrame = lo2;
            if (p->animFlags & 0x40) {
                p->animSubFrame = lo2 - 0xF;
            }
            old2 = p->animFrame;
            t2 >>= 4;
            p->animFrame = t2;
            p->animPrevFrame = old2;
            if (t2 < p->animEnd) {
                evt_onAnimLimit(p);
            }
        }
    } else {
        lo3 = p->animFrame * 16 + (s8)p->animSubFrame;
        if (f & 0x40) {
            t3 = lo3 - p->animSpeed;
        } else {
            t3 = lo3 + p->animSpeed;
        }
        if (p->animFlags & 0x40) {
            t3 += 0xF;
        }
        lo3 = t3 & 0xF;
        p->animSubFrame = lo3;
        if (p->animFlags & 0x40) {
            p->animSubFrame = lo3 - 0xF;
        }
        old3 = p->animFrame;
        t3 >>= 4;
        p->animFrame = t3;
        p->animPrevFrame = old3;
        if (t3 >= p->animFrames) {
            p->animFrame = 0;
            if (p->anim == (u16)p->standAnim) {
                if (g_randPtr >= g_randTableEnd) {
                    rand_refill();
                }
                p->animSpeed = p->idleAnimSpeeds.b[*g_randPtr++ & 3];
            }
        }
    }
}

u8 *evt_loadScripts(u8 *p) {
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
        c = msg_getPartyMemberId(i);
        if (c >= 0) {
            mask |= 1 << c;
        }
    }
    n = p[3];
    g_evtScripts = p + 0x80;
    g_evtScriptCount = n;
    if (g_evtScriptCount >= 10) {
        dst = g_evtScripts + ((ScriptEntry *)g_evtScripts)[g_evtScriptCount - 9].offset;
    } else {
        dst = g_evtScripts + g_evtScriptCount * 8;
    }
    bit = 1;
    for (e = &((ScriptEntry *)g_evtScripts)[g_evtScriptCount - 9]; e < &((ScriptEntry *)g_evtScripts)[g_evtScriptCount]; e++) {
        if (!(e->flags & 1)) {
            mask |= bit;
        }
        if (mask & bit) {
            if (e->offset != 0) {
                src = g_evtScripts + e->offset;
                if (src != dst && e->size != 0) {
                    memmove(dst, src, e->size);
                    e->offset = dst - g_evtScripts;
                }
                dst += e->size & ~3;
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
    EvtAllocNode *n;
    Unk16D8 *m;
    Unk16D8 *q;
    Unk16D8 *l;
    s32 i;
    s32 r;
    s32 d;
    EvtObject *o;
    EvtState *p;
    GameState *gs;

    t = D_80011A44;
    pad_setAnalogThresholds(&t);
    k = data[3];
    g_evtScripts = data + 0x80;
    gs = g_gameState;
    g_evtLocationName = data + 0x2C;
    gs->gameData->unk810 = data + 4;
    g_evtScriptCount = k;
    gs->gameData->locationName = data + 0x2C;
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
    if ((g_evtStateData.mode == 1 || g_evtStateData.mode == 3) && g_evtMode == 2) {
        g_evtSavedState = g_evtStateData;
    }
    g_evtState = &g_evtStateData;
    for (n = g_evtState->allocNodes; n < &g_evtState->allocNodes[31]; n++) {
        n->next = n + 1;
    }
    n->next = NULL;
    g_evtState->allocFree = g_evtState->allocNodes;
    g_evtState->allocUsed = NULL;
    for (m = g_evtState->listNodes; m < &g_evtState->listNodes[31]; m++) {
        m->unk0 = m + 1;
    }
    m->unk0 = NULL;
    g_evtState->listFree = g_evtState->listNodes;
    g_evtState->objectsTail = NULL;
    g_evtState->objects = NULL;
    msg_resetPlayerBubble();
    msg_setAtePrompt(0);
    for (i = 0; i < 0x50; i++) {
        g_evtState->fieldVars[i] = 0;
    }
    g_evtState->controlEnabled = 0;
    g_evtState->playerUid = 0;
    g_evtState->idleTimer = 0;
    msg_startHereIcon(0);
    D_8007B6FC = 0;
    g_evtState->walkOnly = 0;
    g_evtLookTarget = NULL;
    g_evtPlayerAnimCount = 10;
    g_evtMsgCount = 10;
    g_msgReady = 0;
    D_8007B718 = 0;
    g_evtCameraChanged = 0;
    g_evtCamera = 0;
    g_evtEncounterDist = -0x5A0;
    p = g_evtState;
    p->encounterRate = 0;
    g_evtEncounterChance = 0;
    g_evtEncounterPending = 0;
    p->analogDirOfs = 0;
    p->padDirOfs = 0;
    g_swirlVariant = 0;
    func_8004B3A8_inline(0, 0);
    g_evtState->objects->unk4->state = 2;
    evt_execInstr_party();
    for (i = 7; i >= 0; i--) {
        g_evtUnitObjects[i] = NULL;
    }
    D_8007AE04 = 0;
    if (g_evtMode == 2) {
        for (i = 0; i < 4; i++) {
            r = msg_getPartyMemberId(i);
            if (r >= 0) {
                evt_execInstr_newactor(r + g_evtScriptCount - 9, 0);
            }
        }
        g_evtState->battleObjectsTail = (s32)g_evtState->objectsTail;
    } else {
        g_gameState->gameData->rainCount = 0;
    }
    D_8007AE04 = 4;
    if ((g_evtMode == 1 && g_evtSavedState.mode == g_evtMode && g_evtSavedState.locationId == (s16)g_gameState->gameData->fieldId) ||
        (g_evtMode == 3 && g_evtSavedState.mode == g_evtMode && g_evtSavedState.locationId == g_gameState->gameData->worldId) ||
        (g_gameState->prevMode == 9 && g_evtModelsPending != 0)) {
        d = data - g_evtSavedState.scriptBase;
        g_evtStateData = g_evtSavedState;
        for (q = g_evtState->objects; q != NULL; q = q->unk0) {
            o = q->unk4;
            if (o->ip != NULL) {
                o->ip += d;
            }
            if (o->kind == 4 && g_evtMode == 1) {
                fld_queueModelBundle(o->scriptIdx);
            }
        }
        evt_callObjectFunc(evt_getObject_inline(0), 0, 10, 0);
        for (l = g_evtState->objects; l != NULL; l = l->unk0) {
            o = l->unk4;
            if (o->uid != 0) {
                o->savedState = o->state;
                o->state = 3;
            }
        }
    } else {
        evt_updateObjects();
    }
    g_evtState->scriptBase = data;
    g_evtState->mode = g_evtMode;
    g_evtState->locationId = g_evtMode == 1   ? g_gameState->gameData->fieldId
                          : g_evtMode == 3 ? g_gameState->gameData->worldId
                                            : 0;
}

void evt_createModels(void) {
    Unk16D8 *n;
    EvtObject *o;
    s16 *q;

    if ((g_evtMode == 1 || g_evtMode == 3 || g_gameState->prevMode == 9) && g_evtModelsPending != 0) {
        for (n = g_evtState->objects; n != NULL; n = n->unk0) {
            o = n->unk4;
            if (o->kind == 4 && o->modelId != 0xFFFF) {
                o->modelSlot = evt_createModel(o);
                if (g_evtMode == 1) {
                    q = (s16 *)bgi_getActor(o->modelSlot);
                    bgi_setActorRadius(o->modelSlot, o->savedWalkRadius * 4);
                    if (q != NULL) {
                        q[4] = q[2] = o->savedWalkFloor;
                        q[5] = q[3] = o->savedWalkTri;
                        q[0] = o->savedWalkFlags;
                    }
                }
            }
        }
        g_evtModelsPending = 0;
    }
}

void evt_onMenuOpen(void) {
    u8 *s;
    s32 n;
    msg_closeAll();
    msg_startHereIcon(0);
    if (g_evtMode == 3) {
        s = D_800CF478;
        n = (evt_getSysVar(WM_PARAM_AREA) * 2) & 0x7F;
        while (n != 0) {
            while (*s++ != 0) {
            }
            n--;
        }
        g_gameState->gameData->locationName = s;
        while (*s++ != 0) {
        }
        g_gameState->gameData->unk810 = s;
        evt_callObjectFunc(evt_getObject_inline(0), 1, 4, 0);
    }
}

s32 evt_countNpcActors(s32 unused) { /* the overlays pass 0 */
    EvtState *p = g_evtState;
    Unk16D8 *n;
    s32 lim;
    s32 cnt = 0;
    if (p != NULL) {
        n = p->objects;
        if (n != NULL) {
            lim = g_evtScriptCount - 9;
            do {
                EvtObject *e = n->unk4;
                s32 t = 0;
                if (e->scriptIdx < lim) {
                    t = e->kind == 4;
                }
                cnt += t;
                n = n->unk0;
            } while (n != NULL);
        }
    }
    return cnt;
}

s32 evt_getObjectChrIndex(EvtObject *a) {
    s32 t = a->scriptIdx + 9;
    return t - g_evtScriptCount;
}

s32 evt_battleTrigger(s32 op, s32 bits, s32 v, s32 w) {
    s32 r = 0;
    s32 i;
    EvtObject *o;
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
        o = g_evtUnitObjects[i];
        if (op == BTL_CMD_ENEMY_TURN) {
            if (o->level >= 4) {
                o->unkB = 2;
                r = 1;
            }
        } else {
            a = 2;
            b = 7;
            switch (op) {
            case BTL_CMD_ENEMY_COUNTER:
                b = 6;
                D_8007AE10[0] = v;
                a = 1;
                break;
            case BTL_CMD_ENEMY_DEATH:
                b = 9;
                D_8007AE10[0] = v;
                a = 0;
                break;
            case 0x37:
                break;
            }
            D_8007AE34 = w;
            r = evt_callObjectFunc(o, a, b, 0);
        }
    }
    return r;
}

void evt_reset(void) {
    g_evtState = NULL;
    g_evtLastEncounter = 0;
    D_8007954E = 0;
    g_evtModelsPending = 0;
}

void evt_setDrawPosAtObject(EvtObject *o, s32 dx, s32 dy) {
    SVECTOR v;
    SVECTOR *pv = &v;
    s32 p[2];
    volatile s32 sxy;
    GameData *gd;

    v.vx = o->pos.vx;
    v.vy = o->pos.vy + ((o->height * (s8)o->scale) >> 6) - 50;
    v.vz = o->pos.vz;
    if (g_evtMode == 1) {
        fldbg_applyGeomOffset();
    }
    gd = g_gameState->gameData;
    gte_SetRotMatrix(&gd->viewMatrix);
    gte_SetTransMatrix(&gd->viewMatrix);
    gte_ldv0(pv);
    gte_rtps();
    gte_stsxy((s32 *)&sxy);
    if (g_evtMode == 1) {
        fldbg_resetGeomOffset();
    }
    p[0] = *(s16 *)&sxy;
    p[1] = sxy >> 16;
    pen_begin();
    pen_setPos(p[0] + dx, p[1] + dy);
    pen_end();
}

EvtObject *evt_getPlayer(void) {
    EvtObject *p = NULL;
    EvtObject *r;

    if (g_evtState != NULL) {
        p = evt_getObject_inline(g_evtState->playerUid);
    }
    if (p != NULL && p->kind == 4) {
        r = p;
    } else {
        r = NULL;
    }
    return r;
}

s32 evt_isControlEnabled(void) {
    return g_evtState->controlEnabled;
}

Unk16D8 *evt_getObjectList(void) {
    EvtState *p = g_evtState;
    Unk16D8 *r = NULL;
    if (p != NULL) {
        r = p->objects;
    }
    return r;
}

Unk16D8 *evt_getBattleObjects(void) {
    EvtState *p = g_evtState;
    Unk16D8 *r = NULL;
    if (p != NULL) {
        r = p->battleObjectsTail;
    }
    return r;
}

s32 evt_getArg8(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        r = *g_evtCurObj->ip++;
    }
    g_evtArgFlags >>= 1;
    g_evtArgRead = 1;
    return r;
}

s32 evt_getArg16(void) {
    s32 r;
    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        r = evt_popResult();
    } else {
        EvtObject *pp = g_evtCurObj;
        r = *pp->ip++;
        r |= *pp->ip++ << 8;
        r = (s16)r;
    }
    g_evtArgFlags >>= 1;
    g_evtArgRead = 1;
    return r;
}

s32 evt_getObjectModelData(EvtObject *p) {
    s32 *q;

    if (p != NULL) {
        if (p->modelId == 0xFFFF) {
            return 0;
        }
        q = (s32 *)game_getModel(p->modelSlot);
        if (q != NULL) {
            return q[2];
        }
    }
    return 0;
}

EvtObject *evt_getObject(s32 id) {
    return evt_getObject_inline(id);
}

s32 evt_getModelSlot(s32 id) {
    EvtObject *p = evt_getObject_inline(id);
    s32 r;

    if (p != NULL) {
        if (p->kind == 4) {
            r = p->modelSlot;
        } else {
            r = -1;
        }
    } else {
        r = -1;
    }
    return r;
}

s32 evt_getPartyChrIndex(s32 i) {
    s32 t = g_evtState->partyUids[i] + 9;
    u32 r = t - g_evtScriptCount;
    s32 ret = 0;
    if (r < 9) {
        ret = r;
    }
    return ret;
}

void evt_callWorldCellTrigger(s32 a, s32 b, s32 c) {
    evt_callObjectFunc(evt_getObject_inline(0), 1, 0x8000 | ((b << 8) & 0x3F00) | ((a << 2) & 0xFC) | (c & 3), 0);
}

s32 evt_getWorldCamEyeObj(void) {
    return (s32)D_8007AEB8;
}

s32 evt_getWorldCamTargetObj(void) {
    return (s32)D_8007AEBC;
}

void evt_resetIdleTimer(s32 a) {
    EvtState *p = g_evtState;
    s32 v;
    if (p->idleTimer >= 0) {
        if (g_randPtr >= g_randTableEnd) {
            rand_refill();
        }
        v = *g_randPtr++ + 200;
        if (a == 0) {
            p->idleTimer = v * 4;
        } else {
            p->idleTimer = v * 2;
        }
    }
}

void func_8004B2A0(void) {
    EvtAllocNode *p = g_evtState->allocNodes;
    for (; p < &g_evtState->allocNodes[31]; p++) {
        p->next = p + 1;
    }
    p->next = NULL;
    g_evtState->allocFree = g_evtState->allocNodes;
    g_evtState->allocUsed = NULL;
}

void func_8004B2F4(EvtObject *key) {
    func_8004B2F4_inline(key);
}

void evt_initObjectList(void) {
    Unk16D8 *p = g_evtState->listNodes;
    for (; p < &g_evtState->listNodes[31]; p++) {
        p->unk0 = p + 1;
    }
    p->unk0 = NULL;
    g_evtState->listFree = g_evtState->listNodes;
    g_evtState->objectsTail = NULL;
    g_evtState->objects = NULL;
}

void evt_createRegion(s32 a, s32 b) {
    EvtObject *p = evt_createObject(a, b, 0x38, 0x10);
    p->kind = 3;
    p->flags = 1;
}

u16 func_8004B404(s32 arg0) {
    return func_8004B404_inline(arg0);
}

void func_8004B4A8(s32 i, s16 v) {
    D_8007AE10[i & 7] = v;
}

EvtObject *evt_resolveObjectId(s32 id) {
    return evt_resolveObjectId_inline(id);
}

s32 evt_popBitCount(void) {
    s32 n = 0;
    s32 v = evt_popResult();
    s16 bit;
    for (bit = 1; bit != 0; bit <<= 1) {
        n += (v & bit) != 0;
    }
    return n;
}

s32 evt_popRandomBit(void) {
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

s32 evt_getObjectProp(EvtObject *o, s32 prop) {
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
        return o->flags;
    case 5:
        return o->uid;
    case 6:
        return o->level;
    case 7:
        return o->animFrame;
    }
    r = 0;
    for (i = 0; i < 8; i++) {
        if (g_evtUnitObjects[i] == o) {
            break;
        }
    }
    if (i < 8) {
        u = btl_getUnitById(1 << i);
        if (u != NULL) {
            r = btl_getUnitAttr(u, prop);
        }
    }
    return r;
}

void evt_setObjectProp(EvtObject *arg0, s32 arg1, s32 arg2) {
    s32 i;
    struct BtlUnit *(*fn)(u16);
    struct BtlUnit *r;

    if (arg0 != NULL) {
        i = 0;
        if (arg1 == 2) {
            arg0->pos.rotY = arg2 * 16;
            return;
        }
        for (; i < 8; i++) {
            if (g_evtUnitObjects[i] == arg0) {
                break;
            }
        }
        if (i < 8) {
            if (arg1 == 0x20) {
                fn = btl_findUnitByBit;
            } else {
                fn = btl_getUnitById;
            }
            r = fn(1 << i);
            if (r != NULL) {
                btl_setUnitAttr(r, arg1, arg2);
            }
        }
    }
}

s32 func_8004B85C(s32 slot) {
    return func_8004B85C_find(func_8004B85C_key(slot), slot);
}

s32 evt_getArg24(void) {
    s32 v;
    EvtObject *pp;

    if (g_evtArgFlags & 1) {
        evt_evalExpr();
        v = evt_popResult();
    } else {
        pp = g_evtCurObj;
        v = *pp->ip++;
        v |= *pp->ip++ << 8;
        v |= *pp->ip++ << 16;
    }
    g_evtArgFlags >>= (g_evtArgRead = 1);
    return v;
}

void func_8004B9B8(EvtObject *p) {
    func_8004B9B8_inline(p);
}

void evt_projectToScreen(SVECTOR *v, s32 *x, s32 *y) {
    DVECTOR s;
    GameData *gd;

    if (x != NULL && y != NULL) {
        if (g_evtMode == 1) {
            fldbg_applyGeomOffset();
        }
        gd = g_gameState->gameData;
        gte_SetRotMatrix(&gd->viewMatrix);
        gte_SetTransMatrix(&gd->viewMatrix);
        gte_ldv0(v);
        gte_rtps();
        gte_stsxy(&s);
        if (g_evtMode == 1) {
            fldbg_resetGeomOffset();
        }
        *x = s.vx;
        *y = s.vy;
    }
}

void func_8004BB30(void *p) {
    s32 i;
    s32 bit;
    for (i = 0; i < 8; i++) {
        if (g_evtUnitObjects[i] == p) {
            break;
        }
    }
    if (i < 8) {
        bit = 1 << i;
        D_8007AE12 = bit;
    }
}
