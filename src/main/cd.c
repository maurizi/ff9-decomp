#include "ff9.h"

void cd_startRequest(CdRequest *req) {
    volatile s32 st = g_cdLoad.state;
    u32 sectors;

    g_cdLoad.hasQueued = 0;
    g_cdLoad.error = 0;
    g_cdLoad.cur = *req;
    g_cdLoad.state = CD_SETLOC;
    sectors = (u32)(req->size + 0x7FF) >> 11;
    g_cdLoad.chunkIndex = 0;
    g_cdLoad.chunkOfs = 0;
    g_cdLoad.bytesRead = 0;
    g_cdLoad.sectorsLeft = sectors;
    CdReadyCallback(NULL);
    CdSyncCallback(req->syncCallback);
    if (req->flags & CDREQ_SEEK) {
        g_cdLoad.state = CD_SETLOC;
        CdIntToPos(req->sector, &g_cdLoad.pos);
        cd_controlF(CdlSetloc, (u8 *)&g_cdLoad.pos);
    } else if (st != CD_PAUSING) {
        g_cdLoad.state = CD_PAUSING;
        cd_controlF(CdlPause, NULL);
    } else {
        g_cdLoad.state = CD_IDLE;
        if (g_cdLoad.cur.doneCallback != NULL) {
            g_cdLoad.cur.doneCallback(&g_cdLoad);
        }
    }
}

void cd_onDataReady(u8 intr, u8 *result) {
    u8 err = 0;
    u32 pos[3];
    s32 prev;
    s32 sector;

    if (result != NULL) {
        g_cdResult = *(Unk8 *)result;
    }
    if (intr == CdlDataReady) {
        CdGetSector(pos, 3);
        sector = CdPosToInt((CdlLOC *)pos);
        prev = g_cdLoad.lastSector;
        if (prev == 0) {
            if (sector != g_cdLoad.cur.sector) {
                err = 1;
            }
        } else {
            if (sector != prev + 1) {
                err = 1;
            }
        }
        if (err == 0) {
            g_cdLoad.lastSector = sector;
            cd_onSectorReady();
            if (g_cdLoad.sectorsLeft == 0 || g_cdLoad.hasQueued != 0) {
                CdReadyCallback(0);
                g_cdLoad.state = CD_PAUSING;
                cd_controlF(CdlPause, 0);
            }
        }
    } else {
        err = 1;
    }
    if (err != 0) {
        CdReadyCallback(0);
        g_cdLoad.state = CD_PAUSING;
        g_cdLoad.error = 1;
        g_cdLoad.errorCount++;
        cd_controlF(CdlPause, 0);
    }
}

void cd_onSectorReady(void) {
    CdChunk *chunks;

    if (g_cdLoad.cur.chunks == (CdChunk *)-1) {
        chunks = &g_cdLoad.cur.chunk;
    } else {
        chunks = g_cdLoad.cur.chunks;
    }
    if (chunks[g_cdLoad.chunkIndex].dest != NULL) {
        CdGetSector((u32 *)chunks[g_cdLoad.chunkIndex].dest + g_cdLoad.chunkOfs / 4, 0x200);
    }
    g_cdLoad.chunkOfs += 0x800;
    g_cdLoad.bytesRead += 0x800;
    g_cdLoad.sectorsLeft--;
    if (g_cdLoad.chunkOfs >= chunks[g_cdLoad.chunkIndex].size) {
        g_cdLoad.chunkOfs = 0;
        g_cdLoad.chunkIndex++;
        if (g_cdLoad.sectorsLeft != 0) {
            while (chunks[g_cdLoad.chunkIndex].size == 0) {
                g_cdLoad.chunkIndex++;
            }
        }
    }
}

s32 cd_init(void) {
    s32 val = 1;
    s32 ok;

    while (CdInit() == 0) {
    }
    ok = val;
    if (ok) {
        CdSetDebug(0);
        val = CdlModeSpeed; /* passed as the param pointer itself */
        CdControlB(CdlSetmode, (u8 *)val, 0);
        VSync(3);
    }
    return ok;
}

void cd_resetState(void) {
    CdReset(1);
    CdSyncCallback(0);
    CdReadyCallback(NULL);
    memset(&g_cdLoad, 0, 0x98C);
}

s32 cd_findFile(char *name) {
    CdlFILE file;
    s32 sector;
    if (CdSearchFile(&file, name) != NULL) {
        sector = CdPosToInt(&file.pos);
    } else {
        printf("Cannot find file. (%s)\n", name);
        sector = -1;
    }
    return sector;
}

s32 cd_getState(void) {
    return g_cdLoad.state;
}

s32 cd_cancel(void) {
    u8 result[8];
    s32 prevState = g_cdLoad.state;
    if (prevState != 0) {
        CdReadyCallback(NULL);
        CdSyncCallback(0);
        g_cdLoad.hasQueued = 0;
        g_cdLoad.error = 0;
        g_cdLoad.state = CD_IDLE;
        while (CdControlB(CdlPause, NULL, result) == 0) {
        }
    }
    return prevState;
}

void cd_abort(void) {
    CdFlush();
    CdSyncCallback(0);
    CdReadyCallback(NULL);
    g_cdLoad.state = CD_IDLE;
}

void cd_stopMotor(void) {
    CdControlB(CdlSetmode, NULL, NULL);
    VSync(3);
    CdControlB(CdlStop, NULL, NULL);
}

u8 cd_seekAsync(s32 sector, void (*doneCb)(u8 *)) {
    CdRequest req;
    req.flags = CDREQ_SEEK;
    req.syncCallback = cd_onSync;
    req.sector = sector;
    req.size = 0;
    req.chunks = NULL;
    req.readyCallback = cd_onDataReady;
    req.doneCallback = doneCb;
    return cd_submit(&req);
}

u8 cd_readAsync(s32 sector, s32 size, void *dest, void (*doneCb)(u8 *)) {
    g_cdReadRequest.flags = CDREQ_SEEK | CDREQ_READ;
    g_cdReadRequest.chunks = (CdChunk *)-1;
    g_cdReadRequest.syncCallback = cd_onSync;
    g_cdReadRequest.sector = sector;
    g_cdReadRequest.size = size;
    g_cdReadRequest.chunk.size = size;
    g_cdReadRequest.chunk.dest = dest;
    g_cdReadRequest.readyCallback = cd_onDataReady;
    g_cdReadRequest.doneCallback = doneCb;
    return cd_submit(&g_cdReadRequest);
}

u8 cd_readChunks(s32 sector, CdChunk *chunks, s32 count, void (*cb)(u8 *)) {
    CdRequest req;
    s32 i;
    s32 total = 0;

    for (i = 0; i < count; i++) {
        total += chunks[i].size;
    }
    req.flags = CDREQ_SEEK | CDREQ_READ;
    req.syncCallback = cd_onSync;
    req.sector = sector;
    req.size = total;
    req.chunks = chunks;
    req.readyCallback = cd_onDataReady;
    req.doneCallback = cb;
    return cd_submit(&req);
}

u8 cd_syncAsync(void (*doneCb)(u8 *)) {
    CdRequest req;
    req.flags = CDREQ_PAUSE;
    req.doneCallback = doneCb;
    req.sector = 0;
    req.size = 0;
    req.chunks = NULL;
    req.syncCallback = cd_onSync;
    req.readyCallback = 0;
    return cd_submit(&req);
}

s32 cd_seekSync(s32 sector) {
    CdRequest req;
    req.flags = CDREQ_SEEK;
    req.syncCallback = cd_onSync;
    req.sector = sector;
    req.size = 0;
    req.chunks = NULL;
    req.readyCallback = cd_onDataReady;
    req.doneCallback = NULL;
    if (cd_submit(&req) == 0) {
        return 0;
    }
    while (cd_getState() != 0) {
    }
    return 1;
}

s32 cd_readSync(s32 sector, s32 size, void *dest) {
    g_cdReadRequest.flags = CDREQ_SEEK | CDREQ_READ;
    g_cdReadRequest.chunks = (CdChunk *)-1;
    g_cdReadRequest.syncCallback = cd_onSync;
    g_cdReadRequest.sector = sector;
    g_cdReadRequest.size = size;
    g_cdReadRequest.chunk.size = size;
    g_cdReadRequest.chunk.dest = dest;
    g_cdReadRequest.readyCallback = cd_onDataReady;
    g_cdReadRequest.doneCallback = NULL;
    if (cd_submit(&g_cdReadRequest) == 0) {
        return 0;
    }
    while (cd_getState() != 0) {
    }
    return 1;
}

s32 cd_sync(void) {
    CdRequest req;
    req.flags = CDREQ_PAUSE;
    req.sector = 0;
    req.size = 0;
    req.chunks = NULL;
    req.syncCallback = cd_onSync;
    req.readyCallback = 0;
    req.doneCallback = NULL;
    if (cd_submit(&req) == 0) {
        return 0;
    }
    while (cd_getState() != 0) {
    }
    return 1;
}

u8 cd_seekStreamAsync(s32 sector) {
    CdRequest req;
    req.flags = CDREQ_SEEK | CDREQ_STREAM;
    req.sector = sector;
    req.size = 0;
    req.chunks = NULL;
    req.doneCallback = NULL;
    req.syncCallback = cd_onSync;
    req.readyCallback = 0;
    return cd_submit(&req);
}

u8 cd_readStreamAsync(s32 sector, void (*readyCb)()) {
    CdRequest req;
    req.flags = CDREQ_SEEK | CDREQ_READ | CDREQ_STREAM;
    req.sector = sector;
    req.size = 0;
    req.chunks = NULL;
    req.syncCallback = cd_onSync;
    req.readyCallback = readyCb;
    req.doneCallback = NULL;
    return cd_submit(&req);
}

s32 cd_restartAt(s32 sector) {
    if (g_cdLoad.state != CD_RESTART) {
        CdReadyCallback(NULL);
        g_cdLoad.cur.sector = sector;
        g_cdLoad.state = CD_RESTART;
        cd_controlF(CdlPause, NULL);
    }
    return 1;
}

u8 cd_pauseAsync(void (*doneCb)(u8 *)) {
    CdRequest req;
    req.flags = CDREQ_PAUSE;
    req.doneCallback = doneCb;
    req.sector = 0;
    req.size = 0;
    req.chunks = NULL;
    req.syncCallback = cd_onSync;
    req.readyCallback = 0;
    return cd_submit(&req);
}

s32 cd_pause(void) {
    if (g_cdLoad.state == CD_IDLE) {
        return 1;
    }
    g_cdLoad.hasQueued = 0;
    g_cdLoad.error = 0;
    CdReadyCallback(NULL);
    g_cdLoad.state = CD_PAUSING;
    cd_controlF(CdlPause, NULL);
    return 1;
}

u8 cd_checkShellOpen(void) {
    CdControlB(CdlNop, NULL, g_cdResult.b);
    return cd_isShellOpen();
}

s32 cd_isShellOpen(void) {
    return (g_cdResult.b[0] >> 4) & 1; /* CdlStatShellOpen */
}

u8 cd_submit(CdRequest *req) {
    volatile s32 st = g_cdLoad.state;
    g_cdLoad.errorCount = 0;
    if (st == CD_IDLE) {
        if (!(req->flags & CDREQ_PAUSE)) {
            goto run;
        }
        if (req->doneCallback != NULL) {
            req->doneCallback(&g_cdLoad);
        }
        return 1;
    }
    if (g_cdLoad.hasQueued == 0) {
        g_cdLoad.queued = *req;
        g_cdLoad.hasQueued = 1;
        if (g_cdLoad.state == CD_IDLE) {
        run:
            cd_startRequest(req);
        }
        return 1;
    }
    return 0;
}

void cd_onSync(u8 com, u8 *param) {
    void (*fn)(s32);

    if (param != NULL) {
        g_cdResult = *(Unk8 *)param;
    }
    if (g_cdLoad.hasQueued != 0) {
        cd_onSyncQueued(com);
        return;
    }
    if (com == CdlDiskError) {
        cd_onDiskError(CdlDiskError);
        return;
    }
    if (com == CdlComplete) {
        fn = g_cdStateHandlers[g_cdLoad.state];
        if (fn != NULL) {
            fn(CdlComplete);
        }
    }
}

void cd_onSyncQueued(u8 status) {
    if (status == CdlDiskError || status == CdlComplete) {
        if (g_cdLoad.state != CD_READING) {
            if (g_cdLoad.cur.doneCallback != NULL) {
                g_cdLoad.cur.doneCallback(&g_cdLoad);
            }
            cd_startRequest(&g_cdLoad.queued);
        } else if (!(g_cdLoad.cur.flags & CDREQ_STREAM)) {
            CdReadyCallback(NULL);
            g_cdLoad.state = CD_PAUSING;
            cd_controlF(CdlPause, 0);
        }
    }
}

void cd_onDiskError(s32 unused) {
    if (g_cdLoad.state != CD_READING || !(g_cdLoad.cur.flags & CDREQ_STREAM)) {
        g_cdLoad.error = 1;
        g_cdLoad.errorCount++;
        CdReadyCallback(NULL);
        cd_controlF(CdlPause, NULL);
    }
}

void cd_onSetloc(u8 status) {
    u8 param[8];
    s32 st;
    s32 k;

    k = CdlModeSpeed | CdlModeSize1;
    if (CdMode() != k) {
        st = CD_SETMODE;
        param[0] = k;
        k = CdlSetmode;
        g_cdLoad.state = st;
        cd_controlF(k, param);
    } else {
        cd_startTransfer(status);
    }
}

s32 cd_startTransfer() {
    s32 arg;

    if (g_cdLoad.cur.flags & CDREQ_READ) {
        g_cdLoad.lastSector = 0;
        g_cdLoad.state = CD_READING;
        if (g_cdLoad.cur.flags & CDREQ_STREAM) {
            CdReadyCallback((void (*)())cd_onStreamReady);
            arg = CdlReadS;
        } else {
            CdReadyCallback(g_cdLoad.cur.readyCallback);
            arg = CdlReadN;
        }
    } else {
        g_cdLoad.state = CD_PAUSING;
        arg = CdlSeekL;
        if (g_cdLoad.cur.flags & CDREQ_STREAM) {
            arg = CdlSeekP;
        }
    }
    cd_controlF(arg, 0);
}

void cd_onPaused(void) {
    CdReadyCallback(NULL);
    CdSyncCallback(0);
    if (g_cdLoad.error == 0) {
        g_cdLoad.state = CD_IDLE;
        if (g_cdLoad.cur.doneCallback != NULL) {
            g_cdLoad.cur.doneCallback(&g_cdLoad);
        }
    } else {
        g_cdRetryRequest = g_cdLoad.cur;
        cd_startRequest(&g_cdRetryRequest);
    }
}

void cd_onRestart(void) {
    g_cdRestartRequest = g_cdLoad.cur;
    cd_startRequest(&g_cdRestartRequest);
}

void cd_controlF(u8 com, u8 *param) {
    CdControlF(com, param);
}

void cd_onStreamReady(u8 com, u8 *param) {
    if (param != NULL) {
        g_cdResult = *(Unk8 *)param;
    }
    if (g_cdLoad.hasQueued != 0) {
        CdReadyCallback(NULL);
        g_cdLoad.state = CD_PAUSING;
        cd_controlF(CdlPause, 0);
    } else {
        g_cdLoad.cur.readyCallback(com, param);
    }
}
