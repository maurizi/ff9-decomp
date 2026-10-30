#include "ff9.h"

void preload_clearEntry(PreloadEntry *e) {
    preload_setEntry(e, 0, -1, 0, 0, 0, -1, -1);
}

void preload_setEntry(PreloadEntry *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    s32 t;
    p->flags = 0;
    p->kind = a;
    p->fileId = b;
    p->dest = c;
    p->loaded = 0;
    t = d;
    p->bufSize = t / 2048 * 2048;
    p->chunkSize = e;
    p->sector = t = f;
    p->readSize = g;
    p->vramX = 0;
    p->vramY = 0;
    p->vramW = 0;
    p->vramH = 0;
}

void preload_setVramRect(PreloadEntry *a, s16 b, s16 c, s16 d, s16 e) {
    a->vramX = b;
    a->vramY = c;
    a->vramW = d;
    a->flags |= 0x20;
    a->vramH = e;
}

s32 preload_isUsed(PreloadEntry *a) {
    s32 r = 0;
    if (a != NULL) {
        r = a->fileId != -1;
    }
    return r;
}

s32 preload_isBusy(PreloadEntry *a) {
    return a->flags & 1;
}

s32 preload_isDone(PreloadEntry *p) {
    u16 f;
    if (p->kind == 0) {
        return preload_isLoaded(p);
    }
    if (p->kind != 1) {
        return 0;
    }
    f = p->flags;
    if ((f & 0x82) && !(f & 1)) {
        return 1;
    }
    return 0;
}

s32 preload_isLoaded(PreloadEntry *p) {
    u16 f = p->flags;
    if ((f & 0x82) && !(f & 1) && p->kind == 0 && !(f & 0x20) && p->dest != 0) {
        return 1;
    }
    return 0;
}

s32 preload_isInVram(PreloadEntry *p) {
    u16 f = p->flags;
    if ((f & 0x82) && !(f & 1)) {
        if ((f & 0x20) && p->vramW != 0 && p->vramH != 0) {
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
        dst->kind = src->kind;
        dst->fileId = src->fileId;
        t = src->dest;
        t += src->loaded;
        dst->dest = t;
        len = src->dest + src->bufSize - t;
        dst->bufSize = len;
        t = src->chunkSize;
        if (len < t) {
            t = len;
        }
        dst->chunkSize = t;
        dst->chunkSize = t / 2048 * 2048;
        t = dst->chunkSize;
        dst->sector = src->sector + (src->loaded >> 11);
        dst->readSize = dst->chunkSize;
    }
    return dst;
}

s32 preload_wait(PreloadEntry *e) {
    s32 r = 0;
    if (e->flags & 1) {
        while (cd_syncAsync(NULL) == 0) {
        }
        r = 1;
    }
    return r;
}

void preload_init() {
    s32 i;
    PreloadQueue *p;

    p = g_gameState->gameData->preloadQueue;
    p->state = 4;
    p->unk0 = 0;
    p->count = 0;
    p->cur = -1;
    for (i = 5; i >= 0; i--) {
        preload_clearEntry(&p->entries[i]);
    }
    preload_clearEntry(&p->entries[6]);
    p->hook = 0;
}

void preload_setHook(s32 a) {
    g_gameState->gameData->preloadQueue->hook = a;
}

PreloadEntry *preload_add(void) {
    PreloadQueue *t = g_gameState->gameData->preloadQueue;
    PreloadEntry *e = NULL;
    if (t->count < 6) {
        e = &t->entries[t->count];
        preload_clearEntry(e);
        t->count++;
    }
    return e;
}

PreloadEntry *preload_next(void) {
    PreloadQueue *p = g_gameState->gameData->preloadQueue;
    PreloadEntry *e;

    while (p->cur < p->count - 1) {
        p->cur++;
        e = &p->entries[p->cur];
        if (e->fileId != -1) {
            e->flags |= 0x40;
            return e;
        }
    }
    return NULL;
}

PreloadEntry *preload_getCurrent(void) {
    PreloadQueue *p = g_gameState->gameData->preloadQueue;
    return &p->entries[p->cur];
}

PreloadEntry *preload_find(s32 id) {
    PreloadQueue *t = g_gameState->gameData->preloadQueue;
    s32 i;
    id &= 0xFFFFFF;
    for (i = t->count - 1; i >= 0; i--) {
        PreloadEntry *e = &t->entries[i];
        if ((e->fileId & 0xFFFFFF) == id) {
            return e;
        }
    }
    return NULL;
}

s32 preload_startRead(void) {
    PreloadEntry *e = &g_gameState->gameData->preloadQueue->entries[6];
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
        if (p->kind == 0) {
            r = cd_readAsync(g_gameState->imgSector + e->sector, e->readSize, (void *)e->dest, preload_onReadDone);
        } else if (p->kind == 1) {
            r = cd_seekAsync(g_gameState->imgSector + e->sector, preload_onReadDone);
        } else {
            goto done;
        }
        if (r == 0) {
            return -1;
        }
    done:
        p->flags |= 1;
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
    if (p->flags & 1) {
        if (p->flags & 0x80) {
            return -1;
        }
        t = p->flags & 2;
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
        e->flags &= 0xFF7E;
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
    PreloadQueue *c = g_gameState->gameData->preloadQueue;
    s32 i;
    PreloadEntry *e;
    u32 end;
    s32 n;

    end = addr + size;
    for (i = c->count - 1; i >= 0; i--) {
        e = &c->entries[i];
        if (e->fileId != -1 && e->kind == 0) {
            if (e->readSize < e->bufSize) {
                n = e->readSize;
            } else {
                n = e->bufSize;
            }
            if (e->dest < end && addr < e->dest + n) {
                return 1;
            }
        }
    }
    return 0;
}

s32 preload_discardRange(u32 addr, s32 size) {
    PreloadQueue *q = g_gameState->gameData->preloadQueue;
    PreloadEntry *e;
    s32 ret = 0;
    s32 i;
    u32 end = addr + size;

    for (i = q->count - 1; i >= 0; i--) {
        e = &q->entries[i];
        if (e->fileId != -1 && (*(u32 *)&e->flags & 0x60) == 0x40 && e->dest != 0 && e->loaded != 0 &&
            e->dest < end && addr < e->dest + e->loaded) {
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
    PreloadQueue *p = g_gameState->gameData->preloadQueue;
    s32 i;

    for (i = p->count - 1; i >= 0; i--) {
        preload_wait(&p->entries[i]);
        preload_clearEntry(&p->entries[i]);
    }
    p->state = 4;
    p->count = 0;
    p->cur = -1;
}

s32 preload_update(void) {
    PreloadQueue *t = g_gameState->gameData->preloadQueue;
    PreloadEntry *e;

    if (t->hook != NULL && t->hook(t) != 0) {
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
            preload_makeChunk(&t->entries[6], e);
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
    PreloadQueue *p = g_gameState->gameData->preloadQueue;
    PreloadEntry *e;

    if (p != NULL) {
        e = preload_getCurrent();
        if (e != NULL) {
            if (preload_isUsed(e) && *arg0 == 0) {
                PreloadEntry *x = &p->entries[6];
                if (e->kind == 0) {
                    e->loaded += x->readSize;
                }
                e->flags |= 2;
            } else {
                e->flags |= 0x80;
            }
        }
    }
}
