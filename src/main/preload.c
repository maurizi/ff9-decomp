#include "ff9.h"

void preload_clearEntry(PreloadEntry *e) {
    preload_setEntry(e, 0, -1, 0, 0, 0, -1, -1);
}

void preload_setEntry(PreloadEntry *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    s32 t;
    p->unk0 = 0;
    p->unk2 = a;
    p->fileId = b;
    p->unk8 = c;
    p->unkC = 0;
    t = d;
    p->unk10 = t / 2048 * 2048;
    p->unk14 = e;
    p->unk18 = t = f;
    p->unk1C = g;
    p->unk20 = 0;
    p->unk22 = 0;
    p->unk24 = 0;
    p->unk26 = 0;
}

void preload_setVramRect(Unk800219D8 *a, s16 b, s16 c, s16 d, s16 e) {
    a->unk20 = b;
    a->unk22 = c;
    a->unk24 = d;
    a->unk0 |= 0x20;
    a->unk26 = e;
}

s32 preload_isUsed(PreloadEntry *a) {
    s32 r = 0;
    if (a != NULL) {
        r = a->fileId != -1;
    }
    return r;
}

s32 preload_isBusy(PreloadEntry *a) {
    return a->unk0 & 1;
}

s32 preload_isDone(PreloadEntry *p) {
    u16 f;
    if (p->unk2 == 0) {
        return preload_isLoaded(p);
    }
    if (p->unk2 != 1) {
        return 0;
    }
    f = p->unk0;
    if ((f & 0x82) && !(f & 1)) {
        return 1;
    }
    return 0;
}

s32 preload_isLoaded(PreloadEntry *p) {
    u16 f = p->unk0;
    if ((f & 0x82) && !(f & 1) && p->unk2 == 0 && !(f & 0x20) && p->unk8 != 0) {
        return 1;
    }
    return 0;
}

s32 preload_isInVram(PreloadEntry *p) {
    u16 f = p->unk0;
    if ((f & 0x82) && !(f & 1)) {
        if ((f & 0x20) && p->unk24 != 0 && p->unk26 != 0) {
            return 1;
        }
    }
    return 0;
}

PreloadEntry *preload_makeChunk(PreloadEntry *dst, PreloadEntry *src) {
    s32 len;
    s32 t;

    if (preload_isLoaded(src)) {
        preload_clearEntry(dst);
    } else {
        preload_clearEntry(dst);
        dst->unk2 = src->unk2;
        dst->fileId = src->fileId;
        t = src->unk8;
        t += src->unkC;
        dst->unk8 = t;
        len = src->unk8 + src->unk10 - t;
        dst->unk10 = len;
        t = src->unk14;
        if (len < t) {
            t = len;
        }
        dst->unk14 = t;
        dst->unk14 = t / 2048 * 2048;
        t = dst->unk14;
        dst->unk18 = src->unk18 + (src->unkC >> 11);
        dst->unk1C = dst->unk14;
    }
    return dst;
}

s32 preload_wait(PreloadEntry *e) {
    s32 r = 0;
    if (e->unk0 & 1) {
        while (cd_syncAsync(NULL) == 0) {
        }
        r = 1;
    }
    return r;
}

void preload_init() {
    s32 i;
    PreloadQueue *p;

    p = g_gameState->unk1C->unk8C8;
    p->state = 4;
    p->unk0 = 0;
    p->count = 0;
    p->cur = -1;
    for (i = 5; i >= 0; i--) {
        preload_clearEntry(&p->unk4[i]);
    }
    preload_clearEntry(&p->unk4[6]);
    p->unk11C = 0;
}

void preload_setHook(s32 a) {
    g_gameState->unk1C->unk8C8->unk11C = a;
}

PreloadEntry *preload_add(void) {
    PreloadQueue *t = g_gameState->unk1C->unk8C8;
    PreloadEntry *e = NULL;
    if (t->count < 6) {
        e = &t->unk4[t->count];
        preload_clearEntry(e);
        t->count++;
    }
    return e;
}

PreloadEntry *preload_next(void) {
    PreloadQueue *p = g_gameState->unk1C->unk8C8;
    PreloadEntry *e;

    while (p->cur < p->count - 1) {
        p->cur++;
        e = &p->unk4[p->cur];
        if (e->fileId != -1) {
            e->unk0 |= 0x40;
            return e;
        }
    }
    return NULL;
}

PreloadEntry *preload_getCurrent(void) {
    PreloadQueue *p = g_gameState->unk1C->unk8C8;
    return &p->unk4[p->cur];
}

PreloadEntry *preload_find(s32 id) {
    PreloadQueue *t = g_gameState->unk1C->unk8C8;
    s32 i;
    id &= 0xFFFFFF;
    for (i = t->count - 1; i >= 0; i--) {
        PreloadEntry *e = &t->unk4[i];
        if ((e->fileId & 0xFFFFFF) == id) {
            return e;
        }
    }
    return NULL;
}

s32 preload_startRead(void) {
    PreloadEntry *e = &g_gameState->unk1C->unk8C8->unk4[6];
    PreloadEntry *p = preload_getCurrent();
    u8 r;
    s32 ret;

    if (p == NULL) {
        return 0;
    }
    do {
        if (!preload_isUsed(p)) {
            return -1;
        }
        if (p->unk2 == 0) {
            r = cd_readAsync(g_gameState->unk20 + e->unk18, e->unk1C, (void *)e->unk8, preload_onReadDone);
        } else if (p->unk2 == 1) {
            r = cd_seekAsync(g_gameState->unk20 + e->unk18, preload_onReadDone);
        } else {
            goto done;
        }
        if (r == 0) {
            return -1;
        }
    done:
        p->unk0 |= 1;
        ret = 0;
    } while (0);
    return ret;
}

s32 preload_poll(void) {
    PreloadEntry *p = preload_getCurrent();
    s32 t;

    if (p == NULL) {
        return 0;
    }
    if (preload_isUsed(p) == 0) {
        return -1;
    }
    if (p->unk0 & 1) {
        if (p->unk0 & 0x80) {
            return -1;
        }
        t = p->unk0 & 2;
        return t == 0;
    }
    return 0;
}

void preload_finishRead(void) {
    while (preload_poll() > 0) {
    }
    preload_clearBusy();
}

void preload_clearBusy(void) {
    PreloadEntry *e = preload_getCurrent();
    if (e != NULL) {
        e->unk0 &= 0xFF7E;
    }
}

PreloadEntry *preload_waitCurrent(void) {
    PreloadEntry *p = preload_getCurrent();
    if (preload_isUsed(p) == 0) {
        return NULL;
    }
    if (preload_isBusy(p) != 0) {
        preload_wait(p);
    }
    while (preload_poll() > 0) {
    }
    return p;
}

s32 preload_isReady(s32 id) {
    s32 r = 0;
    PreloadEntry *p = preload_find(id);

    if (p != NULL) {
        if (preload_isBusy(p)) {
            preload_finishRead();
        }
        if (preload_isInVram(p) || preload_isLoaded(p)) {
            r = 1;
        }
    } else {
        p = preload_getCurrent();
        if (p != NULL && preload_isUsed(p)) {
            if (((p->fileId >> 16) & 0xFF) == ((id >> 16) & 0xFF) && (p->fileId & 0xFFFF) != (id & 0xFFFF)) {
                preload_wait(p);
            }
        }
    }
    return r;
}

s32 preload_overlaps(u32 addr, s32 size) {
    PreloadQueue *c = g_gameState->unk1C->unk8C8;
    s32 i;
    PreloadEntry *e;
    u32 end;
    s32 n;

    end = addr + size;
    for (i = c->count - 1; i >= 0; i--) {
        e = &c->unk4[i];
        if (e->fileId != -1 && e->unk2 == 0) {
            if (e->unk1C < e->unk10) {
                n = e->unk1C;
            } else {
                n = e->unk10;
            }
            if (e->unk8 < end && addr < e->unk8 + n) {
                return 1;
            }
        }
    }
    return 0;
}

s32 preload_discardRange(u32 addr, s32 size) {
    PreloadQueue *q = g_gameState->unk1C->unk8C8;
    PreloadEntry *e;
    s32 ret = 0;
    s32 i;
    u32 end = addr + size;

    for (i = q->count - 1; i >= 0; i--) {
        e = &q->unk4[i];
        if (e->fileId != -1 && (*(u32 *)&e->unk0 & 0x60) == 0x40 && e->unk8 != 0 && e->unkC != 0 &&
            e->unk8 < end && addr < e->unk8 + e->unkC) {
            if (preload_wait(e)) {
                ret = 1;
            }
            preload_clearEntry(e);
        }
    }
    return ret;
}

void preload_remove(s32 id) {
    PreloadEntry *e = preload_find(id);
    if (e != NULL) {
        preload_wait(e);
        preload_clearEntry(e);
    }
}

void preload_reset(void) {
    PreloadQueue *p = g_gameState->unk1C->unk8C8;
    s32 i;

    for (i = p->count - 1; i >= 0; i--) {
        preload_wait(&p->unk4[i]);
        preload_clearEntry(&p->unk4[i]);
    }
    p->state = 4;
    p->count = 0;
    p->cur = -1;
}

s32 preload_update(void) {
    PreloadQueue *t = g_gameState->unk1C->unk8C8;
    PreloadEntry *e;

    if (t->unk11C != NULL && t->unk11C(t) != 0) {
        return 1;
    }
    for (;;) {
        switch (t->state) {
        case 4:
            if (preload_next() != NULL) {
                t->state = 5;
            } else {
                t->state = 0;
            }
            break;
        case 5:
            e = preload_getCurrent();
            if (preload_isUsed(e) == 0 || preload_isDone(e) != 0) {
                t->state = 4;
                break;
            }
            preload_makeChunk(&t->unk4[6], e);
            t->state = 8;
            break;
        case 8:
            if (cd_getState() != 0) {
                return 0;
            }
            t->state = 9;
            return 0;
        case 9:
            if (preload_startRead() == -1) {
                t->state = 5;
                return 0;
            }
            t->state = 10;
            return 0;
        case 10:
            if (preload_poll() > 0) {
                return 0;
            }
            preload_clearBusy();
            t->state = 5;
            return 0;
        case 0:
        default:
            return 0;
        }
    }
}

void preload_onReadDone(u8 *arg0) {
    PreloadQueue *p = g_gameState->unk1C->unk8C8;
    PreloadEntry *e;

    if (p != NULL) {
        e = preload_getCurrent();
        if (e != NULL) {
            if (preload_isUsed(e) && *arg0 == 0) {
                PreloadEntry *x = &p->unk4[6];
                if (e->unk2 == 0) {
                    e->unkC += x->unk1C;
                }
                e->unk0 |= 2;
            } else {
                e->unk0 |= 0x80;
            }
        }
    }
}
