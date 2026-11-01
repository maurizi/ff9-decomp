#include "ff9.h"

void preload_clearEntry(PreloadEntry *entry) {
    preload_setEntry(entry, 0, -1, 0, 0, 0, -1, -1);
}

void preload_setEntry(PreloadEntry *entry, s32 kind, s32 fileId, s32 dest, s32 bufSize, s32 chunkSize, s32 sector, s32 readSize) {
    s32 tmp;
    entry->flags = 0;
    entry->kind = kind;
    entry->fileId = fileId;
    entry->dest = dest;
    entry->loaded = 0;
    tmp = bufSize;
    entry->bufSize = tmp / 2048 * 2048;
    entry->chunkSize = chunkSize;
    entry->sector = tmp = sector;
    entry->readSize = readSize;
    entry->vramX = 0;
    entry->vramY = 0;
    entry->vramW = 0;
    entry->vramH = 0;
}

void preload_setVramRect(PreloadEntry *entry, s16 x, s16 y, s16 w, s16 h) {
    entry->vramX = x;
    entry->vramY = y;
    entry->vramW = w;
    entry->flags |= PRELOAD_IN_VRAM;
    entry->vramH = h;
}

s32 preload_isUsed(PreloadEntry *entry) {
    s32 used = 0;
    if (entry != NULL) {
        used = entry->fileId != -1;
    }
    return used;
}

s32 preload_isBusy(PreloadEntry *entry) {
    return entry->flags & PRELOAD_BUSY;
}

s32 preload_isDone(PreloadEntry *entry) {
    u16 flags;
    if (entry->kind == PRELOAD_KIND_READ) {
        return preload_isLoaded(entry);
    }
    if (entry->kind != PRELOAD_KIND_SEEK) {
        return 0;
    }
    flags = entry->flags;
    if ((flags & (PRELOAD_READ_OK | PRELOAD_READ_FAILED)) && !(flags & PRELOAD_BUSY)) {
        return 1;
    }
    return 0;
}

s32 preload_isLoaded(PreloadEntry *entry) {
    u16 flags = entry->flags;
    if ((flags & (PRELOAD_READ_OK | PRELOAD_READ_FAILED)) && !(flags & PRELOAD_BUSY) && entry->kind == PRELOAD_KIND_READ && !(flags & PRELOAD_IN_VRAM) && entry->dest != 0) {
        return 1;
    }
    return 0;
}

s32 preload_isInVram(PreloadEntry *entry) {
    u16 flags = entry->flags;
    if ((flags & (PRELOAD_READ_OK | PRELOAD_READ_FAILED)) && !(flags & PRELOAD_BUSY)) {
        if ((flags & PRELOAD_IN_VRAM) && entry->vramW != 0 && entry->vramH != 0) {
            return 1;
        }
    }
    return 0;
}

PreloadEntry *preload_makeChunk(PreloadEntry *dst, PreloadEntry *src) {
    s32 len;
    s32 tmp;

    if (preload_isLoaded(src)) {
        preload_clearEntry(dst);
    } else {
        preload_clearEntry(dst);
        dst->kind = src->kind;
        dst->fileId = src->fileId;
        tmp = src->dest;
        tmp += src->loaded;
        dst->dest = tmp;
        len = src->dest + src->bufSize - tmp;
        dst->bufSize = len;
        tmp = src->chunkSize;
        if (len < tmp) {
            tmp = len;
        }
        dst->chunkSize = tmp;
        dst->chunkSize = tmp / 2048 * 2048;
        tmp = dst->chunkSize;
        dst->sector = src->sector + (src->loaded >> 11);
        dst->readSize = dst->chunkSize;
    }
    return dst;
}

s32 preload_wait(PreloadEntry *entry) {
    s32 waited = 0;
    if (entry->flags & PRELOAD_BUSY) {
        while (cd_syncAsync(NULL) == 0) {
        }
        waited = 1;
    }
    return waited;
}

void preload_init() {
    s32 i;
    PreloadQueue *queue;

    queue = g_gameState->gameData->preloadQueue;
    queue->state = PRELOAD_Q_NEXT_ENTRY;
    queue->unk0 = 0;
    queue->count = 0;
    queue->cur = -1;
    for (i = 5; i >= 0; i--) {
        preload_clearEntry(&queue->entries[i]);
    }
    preload_clearEntry(&queue->entries[6]);
    queue->hook = 0;
}

void preload_setHook(s32 hook) {
    g_gameState->gameData->preloadQueue->hook = hook;
}

PreloadEntry *preload_add(void) {
    PreloadQueue *queue = g_gameState->gameData->preloadQueue;
    PreloadEntry *entry = NULL;
    if (queue->count < 6) {
        entry = &queue->entries[queue->count];
        preload_clearEntry(entry);
        queue->count++;
    }
    return entry;
}

PreloadEntry *preload_next(void) {
    PreloadQueue *queue = g_gameState->gameData->preloadQueue;
    PreloadEntry *entry;

    while (queue->cur < queue->count - 1) {
        queue->cur++;
        entry = &queue->entries[queue->cur];
        if (entry->fileId != -1) {
            entry->flags |= PRELOAD_STARTED;
            return entry;
        }
    }
    return NULL;
}

PreloadEntry *preload_getCurrent(void) {
    PreloadQueue *queue = g_gameState->gameData->preloadQueue;
    return &queue->entries[queue->cur];
}

PreloadEntry *preload_find(s32 id) {
    PreloadQueue *queue = g_gameState->gameData->preloadQueue;
    s32 i;
    id &= 0xFFFFFF;
    for (i = queue->count - 1; i >= 0; i--) {
        PreloadEntry *entry = &queue->entries[i];
        if ((entry->fileId & 0xFFFFFF) == id) {
            return entry;
        }
    }
    return NULL;
}

s32 preload_startRead(void) {
    PreloadEntry *chunk = &g_gameState->gameData->preloadQueue->entries[6];
    PreloadEntry *entry = preload_getCurrent();
    u8 ok;
    s32 started;

    if (entry == NULL) {
        return 0;
    }
    do {
        if (!preload_isUsed(entry)) {
            return -1;
        }
        if (entry->kind == PRELOAD_KIND_READ) {
            ok = cd_readAsync(g_gameState->imgSector + chunk->sector, chunk->readSize, (void *)chunk->dest, preload_onReadDone);
        } else if (entry->kind == PRELOAD_KIND_SEEK) {
            ok = cd_seekAsync(g_gameState->imgSector + chunk->sector, preload_onReadDone);
        } else {
            goto done;
        }
        if (ok == 0) {
            return -1;
        }
    done:
        entry->flags |= PRELOAD_BUSY;
        started = 0;
    } while (0);
    return started;
}

s32 preload_poll(void) {
    PreloadEntry *entry = preload_getCurrent();
    s32 readOk;

    if (entry == NULL) {
        return 0;
    }
    if (preload_isUsed(entry) == 0) {
        return -1;
    }
    if (entry->flags & PRELOAD_BUSY) {
        if (entry->flags & PRELOAD_READ_FAILED) {
            return -1;
        }
        readOk = entry->flags & PRELOAD_READ_OK;
        return readOk == 0;
    }
    return 0;
}

void preload_finishRead(void) {
    while (preload_poll() > 0) {
    }
    preload_clearBusy();
}

void preload_clearBusy(void) {
    PreloadEntry *entry = preload_getCurrent();
    if (entry != NULL) {
        entry->flags &= (u16)~(PRELOAD_BUSY | PRELOAD_READ_FAILED);
    }
}

PreloadEntry *preload_waitCurrent(void) {
    PreloadEntry *entry = preload_getCurrent();
    if (preload_isUsed(entry) == 0) {
        return NULL;
    }
    if (preload_isBusy(entry) != 0) {
        preload_wait(entry);
    }
    while (preload_poll() > 0) {
    }
    return entry;
}

s32 preload_isReady(s32 id) {
    s32 ready = 0;
    PreloadEntry *entry = preload_find(id);

    if (entry != NULL) {
        if (preload_isBusy(entry)) {
            preload_finishRead();
        }
        if (preload_isInVram(entry) || preload_isLoaded(entry)) {
            ready = 1;
        }
    } else {
        entry = preload_getCurrent();
        if (entry != NULL && preload_isUsed(entry)) {
            if (((entry->fileId >> 16) & 0xFF) == ((id >> 16) & 0xFF) && (entry->fileId & 0xFFFF) != (id & 0xFFFF)) {
                preload_wait(entry);
            }
        }
    }
    return ready;
}

s32 preload_overlaps(u32 addr, s32 size) {
    PreloadQueue *queue = g_gameState->gameData->preloadQueue;
    s32 i;
    PreloadEntry *entry;
    u32 end;
    s32 len;

    end = addr + size;
    for (i = queue->count - 1; i >= 0; i--) {
        entry = &queue->entries[i];
        if (entry->fileId != -1 && entry->kind == PRELOAD_KIND_READ) {
            if (entry->readSize < entry->bufSize) {
                len = entry->readSize;
            } else {
                len = entry->bufSize;
            }
            if (entry->dest < end && addr < entry->dest + len) {
                return 1;
            }
        }
    }
    return 0;
}

s32 preload_discardRange(u32 addr, s32 size) {
    PreloadQueue *queue = g_gameState->gameData->preloadQueue;
    PreloadEntry *entry;
    s32 discarded = 0;
    s32 i;
    u32 end = addr + size;

    for (i = queue->count - 1; i >= 0; i--) {
        entry = &queue->entries[i];
        if (entry->fileId != -1 && (*(u32 *)&entry->flags & (PRELOAD_IN_VRAM | PRELOAD_STARTED)) == PRELOAD_STARTED && entry->dest != 0 && entry->loaded != 0 &&
            entry->dest < end && addr < entry->dest + entry->loaded) {
            if (preload_wait(entry)) {
                discarded = 1;
            }
            preload_clearEntry(entry);
        }
    }
    return discarded;
}

void preload_remove(s32 id) {
    PreloadEntry *entry = preload_find(id);
    if (entry != NULL) {
        preload_wait(entry);
        preload_clearEntry(entry);
    }
}

void preload_reset(void) {
    PreloadQueue *queue = g_gameState->gameData->preloadQueue;
    s32 i;

    for (i = queue->count - 1; i >= 0; i--) {
        preload_wait(&queue->entries[i]);
        preload_clearEntry(&queue->entries[i]);
    }
    queue->state = PRELOAD_Q_NEXT_ENTRY;
    queue->count = 0;
    queue->cur = -1;
}

s32 preload_update(void) {
    PreloadQueue *queue = g_gameState->gameData->preloadQueue;
    PreloadEntry *entry;

    if (queue->hook != NULL && queue->hook(queue) != 0) {
        return 1;
    }
    for (;;) {
        switch (queue->state) {
        case PRELOAD_Q_NEXT_ENTRY:
            if (preload_next() != NULL) {
                queue->state = PRELOAD_Q_NEXT_CHUNK;
            } else {
                queue->state = PRELOAD_Q_IDLE;
            }
            break;
        case PRELOAD_Q_NEXT_CHUNK:
            entry = preload_getCurrent();
            if (preload_isUsed(entry) == 0 || preload_isDone(entry) != 0) {
                queue->state = PRELOAD_Q_NEXT_ENTRY;
                break;
            }
            preload_makeChunk(&queue->entries[6], entry);
            queue->state = PRELOAD_Q_WAIT_CD;
            break;
        case PRELOAD_Q_WAIT_CD:
            if (cd_getState() != 0) {
                return 0;
            }
            queue->state = PRELOAD_Q_START_READ;
            return 0;
        case PRELOAD_Q_START_READ:
            if (preload_startRead() == -1) {
                queue->state = PRELOAD_Q_NEXT_CHUNK;
                return 0;
            }
            queue->state = PRELOAD_Q_READING;
            return 0;
        case PRELOAD_Q_READING:
            if (preload_poll() > 0) {
                return 0;
            }
            preload_clearBusy();
            queue->state = PRELOAD_Q_NEXT_CHUNK;
            return 0;
        case PRELOAD_Q_IDLE:
        default:
            return 0;
        }
    }
}

void preload_onReadDone(u8 *result) {
    PreloadQueue *queue = g_gameState->gameData->preloadQueue;
    PreloadEntry *entry;

    if (queue != NULL) {
        entry = preload_getCurrent();
        if (entry != NULL) {
            if (preload_isUsed(entry) && *result == 0) {
                PreloadEntry *x = &queue->entries[6];
                if (entry->kind == PRELOAD_KIND_READ) {
                    entry->loaded += x->readSize;
                }
                entry->flags |= PRELOAD_READ_OK;
            } else {
                entry->flags |= PRELOAD_READ_FAILED;
            }
        }
    }
}
