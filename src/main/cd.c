#include "ff9.h"

void cd_startRequest(CdRequest *r) {
    volatile s32 st = g_cdLoad.state;
    u32 n;

    g_cdLoad.hasQueued = 0;
    g_cdLoad.unk3 = 0;
    g_cdLoad.cur = *r;
    g_cdLoad.state = 1;
    n = (u32)(r->size + 0x7FF) >> 11;
    g_cdLoad.chunkIndex = 0;
    g_cdLoad.chunkOfs = 0;
    g_cdLoad.bytesRead = 0;
    g_cdLoad.sectorsLeft = n;
    CdReadyCallback(NULL);
    CdSyncCallback(r->syncCallback);
    if (r->flags & 1) {
        g_cdLoad.state = 1;
        CdIntToPos(r->sector, &g_cdLoad.unk8);
        cd_controlF(2, (u8 *)&g_cdLoad.unk8);
    } else if (st != 4) {
        g_cdLoad.state = 4;
        cd_controlF(9, NULL);
    } else {
        g_cdLoad.state = 0;
        if (g_cdLoad.cur.doneCallback != NULL) {
            g_cdLoad.cur.doneCallback(&g_cdLoad);
        }
    }
}

void cd_onDataReady(u8 intr, u8 *result) {
    u8 err = 0;
    u32 pos[3];
    s32 prev;
    s32 n;

    if (result != NULL) {
        g_cdResult = *(Unk8 *)result;
    }
    if (intr == 1) {
        CdGetSector(pos, 3);
        n = CdPosToInt((CdlLOC *)pos);
        prev = g_cdLoad.unk18;
        if (prev == 0) {
            if (n != g_cdLoad.cur.sector) {
                err = 1;
            }
        } else {
            if (n != prev + 1) {
                err = 1;
            }
        }
        if (err == 0) {
            g_cdLoad.unk18 = n;
            cd_onSectorReady();
            if (g_cdLoad.sectorsLeft == 0 || g_cdLoad.hasQueued != 0) {
                CdReadyCallback(0);
                g_cdLoad.state = 4;
                cd_controlF(9, 0);
            }
        }
    } else {
        err = 1;
    }
    if (err != 0) {
        CdReadyCallback(0);
        g_cdLoad.state = 4;
        g_cdLoad.unk3 = 1;
        g_cdLoad.unk1++;
        cd_controlF(9, 0);
    }
}

void cd_onSectorReady(void) {
    CdChunk *c;

    if (g_cdLoad.cur.unkC == (CdChunk *)-1) {
        c = &g_cdLoad.cur.unk1C;
    } else {
        c = g_cdLoad.cur.unkC;
    }
    if (c[g_cdLoad.chunkIndex].dest != NULL) {
        CdGetSector((u32 *)c[g_cdLoad.chunkIndex].dest + g_cdLoad.chunkOfs / 4, 0x200);
    }
    g_cdLoad.chunkOfs += 0x800;
    g_cdLoad.bytesRead += 0x800;
    g_cdLoad.sectorsLeft--;
    if (g_cdLoad.chunkOfs >= c[g_cdLoad.chunkIndex].size) {
        g_cdLoad.chunkOfs = 0;
        g_cdLoad.chunkIndex++;
        if (g_cdLoad.sectorsLeft != 0) {
            while (c[g_cdLoad.chunkIndex].size == 0) {
                g_cdLoad.chunkIndex++;
            }
        }
    }
}

s32 cd_init(void) {
    s32 v = 1;
    s32 ok;

    while (CdInit() == 0) {
    }
    ok = v;
    if (ok) {
        CdSetDebug(0);
        v = 0x80; /* CdlModeSpeed */
        CdControlB(0xE, (u8 *)v, 0);
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
    CdlFILE f;
    s32 r;
    if (CdSearchFile(&f, name) != NULL) {
        r = CdPosToInt(&f.pos);
    } else {
        printf("Cannot find file. (%s)\n", name);
        r = -1;
    }
    return r;
}

s32 cd_getState(void) {
    return g_cdLoad.state;
}

s32 cd_cancel(void) {
    u8 buf[8];
    s32 r = g_cdLoad.state;
    if (r != 0) {
        CdReadyCallback(NULL);
        CdSyncCallback(0);
        g_cdLoad.hasQueued = 0;
        g_cdLoad.unk3 = 0;
        g_cdLoad.state = 0;
        while (CdControlB(9, NULL, buf) == 0) {
        }
    }
    return r;
}

void func_80022B9C(void) {
    CdFlush();
    CdSyncCallback(0);
    CdReadyCallback(NULL);
    g_cdLoad.state = 0;
}

void cd_stopMotor(void) {
    CdControlB(0xE, NULL, NULL);
    VSync(3);
    CdControlB(8, NULL, NULL);
}

u8 cd_seekAsync(s32 a, void (*b)(u8 *)) {
    CdRequest r;
    r.flags = 1;
    r.syncCallback = cd_onSync;
    r.sector = a;
    r.size = 0;
    r.unkC = NULL;
    r.readyCallback = cd_onDataReady;
    r.doneCallback = b;
    return cd_submit(&r);
}

u8 cd_readAsync(s32 a, s32 b, void *c, void (*d)(u8 *)) {
    g_cdReadRequest.flags = 3;
    g_cdReadRequest.unkC = (CdChunk *)-1;
    g_cdReadRequest.syncCallback = cd_onSync;
    g_cdReadRequest.sector = a;
    g_cdReadRequest.size = b;
    g_cdReadRequest.unk1C.size = b;
    g_cdReadRequest.unk1C.dest = c;
    g_cdReadRequest.readyCallback = cd_onDataReady;
    g_cdReadRequest.doneCallback = d;
    return cd_submit(&g_cdReadRequest);
}

u8 cd_readChunks(s32 sector, CdChunk *chunks, s32 n, void (*cb)(u8 *)) {
    CdRequest r;
    s32 i;
    s32 total = 0;

    for (i = 0; i < n; i++) {
        total += chunks[i].size;
    }
    r.flags = 3;
    r.syncCallback = cd_onSync;
    r.sector = sector;
    r.size = total;
    r.unkC = chunks;
    r.readyCallback = cd_onDataReady;
    r.doneCallback = cb;
    return cd_submit(&r);
}

u8 func_80022D3C(void (*a)(u8 *)) {
    CdRequest r;
    r.flags = 4;
    r.doneCallback = a;
    r.sector = 0;
    r.size = 0;
    r.unkC = NULL;
    r.syncCallback = cd_onSync;
    r.readyCallback = 0;
    return cd_submit(&r);
}

s32 cd_seekSync(s32 a) {
    CdRequest r;
    r.flags = 1;
    r.syncCallback = cd_onSync;
    r.sector = a;
    r.size = 0;
    r.unkC = NULL;
    r.readyCallback = cd_onDataReady;
    r.doneCallback = NULL;
    if (cd_submit(&r) == 0) {
        return 0;
    }
    while (cd_getState() != 0) {
    }
    return 1;
}

s32 cd_readSync(s32 a, s32 b, void *c) {
    g_cdReadRequest.flags = 3;
    g_cdReadRequest.unkC = (CdChunk *)-1;
    g_cdReadRequest.syncCallback = cd_onSync;
    g_cdReadRequest.sector = a;
    g_cdReadRequest.size = b;
    g_cdReadRequest.unk1C.size = b;
    g_cdReadRequest.unk1C.dest = c;
    g_cdReadRequest.readyCallback = cd_onDataReady;
    g_cdReadRequest.doneCallback = NULL;
    if (cd_submit(&g_cdReadRequest) == 0) {
        return 0;
    }
    while (cd_getState() != 0) {
    }
    return 1;
}

s32 func_80022E70(void) {
    CdRequest r;
    r.flags = 4;
    r.sector = 0;
    r.size = 0;
    r.unkC = NULL;
    r.syncCallback = cd_onSync;
    r.readyCallback = 0;
    r.doneCallback = NULL;
    if (cd_submit(&r) == 0) {
        return 0;
    }
    while (cd_getState() != 0) {
    }
    return 1;
}

u8 func_80022ED4(s32 a) {
    CdRequest r;
    r.flags = 9;
    r.sector = a;
    r.size = 0;
    r.unkC = NULL;
    r.doneCallback = NULL;
    r.syncCallback = cd_onSync;
    r.readyCallback = 0;
    return cd_submit(&r);
}

u8 func_80022F1C(s32 a, void (*b)()) {
    CdRequest r;
    r.flags = 0xB;
    r.sector = a;
    r.size = 0;
    r.unkC = NULL;
    r.syncCallback = cd_onSync;
    r.readyCallback = b;
    r.doneCallback = NULL;
    return cd_submit(&r);
}

s32 func_80022F64(s32 a) {
    if (g_cdLoad.state != 5) {
        CdReadyCallback(NULL);
        g_cdLoad.cur.sector = a;
        g_cdLoad.state = 5;
        cd_controlF(9, NULL);
    }
    return 1;
}

u8 func_80022FD0(void (*a)(u8 *)) {
    CdRequest r;
    r.flags = 4;
    r.doneCallback = a;
    r.sector = 0;
    r.size = 0;
    r.unkC = NULL;
    r.syncCallback = cd_onSync;
    r.readyCallback = 0;
    return cd_submit(&r);
}

s32 func_80023018(void) {
    if (g_cdLoad.state == 0) {
        return 1;
    }
    g_cdLoad.hasQueued = 0;
    g_cdLoad.unk3 = 0;
    CdReadyCallback(NULL);
    g_cdLoad.state = 4;
    cd_controlF(9, NULL);
    return 1;
}

u8 cd_checkShellOpen(void) {
    CdControlB(1, NULL, g_cdResult.unk0);
    return func_800230A0();
}

s32 func_800230A0(void) {
    return (g_cdResult.unk0[0] >> 4) & 1;
}

u8 cd_submit(CdRequest *r) {
    volatile s32 st = g_cdLoad.state;
    g_cdLoad.unk1 = 0;
    if (st == 0) {
        if (!(r->flags & 4)) {
            goto run;
        }
        if (r->doneCallback != NULL) {
            r->doneCallback(&g_cdLoad);
        }
        return 1;
    }
    if (g_cdLoad.hasQueued == 0) {
        g_cdLoad.queued = *r;
        g_cdLoad.hasQueued = 1;
        if (g_cdLoad.state == 0) {
        run:
            cd_startRequest(r);
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
        func_8002325C(com);
        return;
    }
    if (com == 5) {
        func_80023304(5);
        return;
    }
    if (com == 2) {
        fn = D_80068668[g_cdLoad.state];
        if (fn != NULL) {
            fn(2);
        }
    }
}

void func_8002325C(u8 arg0) {
    if (arg0 == 5 || arg0 == 2) {
        if (g_cdLoad.state != 3) {
            if (g_cdLoad.cur.doneCallback != NULL) {
                g_cdLoad.cur.doneCallback(&g_cdLoad);
            }
            cd_startRequest(&g_cdLoad.queued);
        } else if (!(g_cdLoad.cur.flags & 8)) {
            CdReadyCallback(NULL);
            g_cdLoad.state = 4;
            cd_controlF(9, 0);
        }
    }
}

void func_80023304(s32 unused) {
    if (g_cdLoad.state != 3 || !(g_cdLoad.cur.flags & 8)) {
        g_cdLoad.unk3 = 1;
        g_cdLoad.unk1++;
        CdReadyCallback(NULL);
        cd_controlF(9, NULL);
    }
}

void func_8002336C(u8 a) {
    u8 buf[8];
    s32 st;
    s32 k;

    k = 0xA0;
    if (CdMode() != k) {
        st = 2;
        buf[0] = k;
        k = 0xE;
        g_cdLoad.state = st;
        cd_controlF(k, buf);
    } else {
        func_800233CC(a);
    }
}

s32 func_800233CC() {
    s32 arg;

    if (g_cdLoad.cur.flags & 2) {
        g_cdLoad.unk18 = 0;
        g_cdLoad.state = 3;
        if (g_cdLoad.cur.flags & 8) {
            CdReadyCallback((void (*)())func_800235A0);
            arg = 0x1B;
        } else {
            CdReadyCallback(g_cdLoad.cur.readyCallback);
            arg = 6;
        }
    } else {
        g_cdLoad.state = 4;
        arg = 0x15;
        if (g_cdLoad.cur.flags & 8) {
            arg = 0x16;
        }
    }
    cd_controlF(arg, 0);
}

void func_8002345C(void) {
    CdReadyCallback(NULL);
    CdSyncCallback(0);
    if (g_cdLoad.unk3 == 0) {
        g_cdLoad.state = 0;
        if (g_cdLoad.cur.doneCallback != NULL) {
            g_cdLoad.cur.doneCallback(&g_cdLoad);
        }
    } else {
        D_80076178 = g_cdLoad.cur;
        cd_startRequest(&D_80076178);
    }
}

void func_8002350C(void) {
    D_800761A8 = g_cdLoad.cur;
    cd_startRequest(&D_800761A8);
}

void cd_controlF(u8 com, u8 *param) {
    CdControlF(com, param);
}

void func_800235A0(u8 com, u8 *param) {
    if (param != NULL) {
        g_cdResult = *(Unk8 *)param;
    }
    if (g_cdLoad.hasQueued != 0) {
        CdReadyCallback(NULL);
        g_cdLoad.state = 4;
        cd_controlF(9, 0);
    } else {
        g_cdLoad.cur.readyCallback(com, param);
    }
}
