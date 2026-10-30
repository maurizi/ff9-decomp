#include "ovl_0c.h"

void wm_relocatePack(WmPack *arg0) {
    s32 i;
    WmPackEntry *e;

    i = 0;
    arg0->unk4 += (s32)arg0;
    for (; i < arg0->count; i++) {
        e = (WmPackEntry *)((s32)arg0->entries[i] + (s32)arg0);
        arg0->entries[i] = e;
        e->verts += (s32)arg0;
        e->polys += (s32)arg0;
    }
}

void wm_setupWorldData(void) {
    s32 unused[26];
    void **f[6]; /* f[6] below overruns into the saved s0 slot, as in the original */
    void *p;
    void **t;
    WmTexScrollTbl *dst;
    s32 i;
    s32 acc;
    s32 w;
    s32 h;

    acc = 0;
    wm_initBlockMatrix();
    i = 0;
    wm_chunkListInit();
    do {
        g_worldEncounterZoneStart[i] = acc;
        acc += g_worldZoneEncounterCount[i] * 2;
        i++;
    } while (i < 26);
    while (cd_getState() != 0) {
    }
    if (g_worldDataSet == WM_DATASET_DISC1) {
        g_worldTexAnimPack = db_lookupRegistered(4, 0x411, 0);
        g_worldDataPack = db_lookupRegistered(4, 0x41B, 0);
        g_worldChunkTable = db_lookupRegistered(4, 0x425, 0);
        g_worldChunkTableT = db_lookupRegistered(4, 0x42F, 0);
        f[4] = db_lookupRegistered(4, 0x439, 0);
        f[5] = db_lookupRegistered(4, 0x443, 0);
    } else {
        g_worldTexAnimPack = db_lookupRegistered(4, 0x414, 0);
        g_worldDataPack = db_lookupRegistered(4, 0x41E, 0);
        g_worldChunkTable = db_lookupRegistered(4, 0x428, 0);
        g_worldChunkTableT = db_lookupRegistered(4, 0x432, 0);
        f[4] = db_lookupRegistered(4, 0x43C, 0);
        f[5] = db_lookupRegistered(4, 0x446, 0);
    }
    f[0] = db_lookupRegistered(4, 0x5DC, 0);
    f[1] = db_lookupRegistered(4, 0x5DD, 0);
    f[2] = db_lookupRegistered(4, 0x5DE, 0);
    f[3] = db_lookupRegistered(4, 0x5DF, 0);
    f[6] = db_lookupRegistered(4, 0x5E0, 0);
    w = g_worldChunkTable->width;
    h = g_worldChunkTable->height;
    g_worldChunksW = w;
    g_worldChunksH = h;
    g_worldCellsW = w * 2;
    g_worldCellsH = h * 2;
    g_worldWidth = w << 14;
    g_worldDepth = h << 14;
    *(WmCellMap *)g_worldChunkFlags = *(WmCellMap *)f[0];
    wm_applyStoryForms();
    w_frameExtruct(g_worldTexAnimPack);
    w_frameExtruct((s32 *)g_worldDataPack);
    w_frameExtruct((s32 *)f[1]);
    w_frameExtruct((s32 *)f[2]);
    w_frameExtruct((s32 *)f[3]);
    w_frameExtruct((s32 *)f[4]);
    wm_loadImagePack(f[6]);
    t = f[5];
    w_frameExtruct((s32 *)t);
    for (i = 0; i < 16; i++) {
        p = w_framePackGetPtr(t, i + g_worldFormFlags[i] * 16);
        w_frameExtruct(p);
        wm_loadTimPack(p, 0, 0, 0, 0);
    }
    wm_loadTimPack((s32 *)f[4], 0, 0, 0, 0);
    wm_loadTimPack((s32 *)f[1], 0, 0, 0, 0);
    if (g_gameState->gameData->chocograph != 0) {
        wm_loadTim((u32 *)f[3], g_gameState->gameData->chocograph - 1, 0x318, 0xB0, 0x280, 0xF6, 2);
    }
    if (*g_worldStoryCounterPtr >= 0x1766) {
        g_worldNaviPage = 1;
    } else {
        g_worldNaviPage = 0;
    }
    wm_loadTim((u32 *)f[2], g_worldNaviPage * 2, 0, 0, 0, 0, 0);
    wm_loadTim((u32 *)f[2], (g_worldNaviPage << 1) | 1, 0, 0, 0, 0, 1);
    wm_loadTim((u32 *)f[2], 4, 0, 0, 0, 0, 0);
    g_worldTitleId = -1;
    switch (*g_worldStoryCounterPtr) {
    case 0x960:
        g_worldTitleId = 0;
        break;
    case 0x1766:
        g_worldTitleId = 1;
        break;
    case 0x2585:
        g_worldTitleId = 2;
        break;
    case 0x26A2:
        g_worldTitleId = 3;
        break;
    }
    if (g_worldTitleId != -1) {
        if (g_worldTitleWide == 0) {
            wm_loadTim((u32 *)f[2], g_worldTitleId * 2 + 5, 0x300, 0x40, 0x280, 0xF4, 0);
            wm_loadTim((u32 *)f[2], g_worldTitleId * 2 + 6, 0x320, 0x40, 0x290, 0xF4, 0);
        } else {
            wm_loadTim((u32 *)f[2], g_worldTitleId * 2 + 5, 0x300, 0x40, 0x280, 0xF4, 0);
            wm_loadTim((u32 *)f[2], g_worldTitleId * 2 + 6, 0x300, 0x60, 0x290, 0xF4, 0);
        }
    }
    for (i = 0x35; i < 0x43; i++) {
        wm_relocatePack(w_framePackGetPtr(g_worldDataPack, i));
    }
    g_worldDefaultBlocks = w_framePackGetPtr(g_worldDataPack, 0x42);
    D_800CF478 = w_framePackGetPtr(g_worldDataPack, 0);
    g_worldEncounterTable = w_framePackGetPtr(g_worldDataPack, 3);
    g_worldWeatherData = *(WmWeatherData *)w_framePackGetPtr(g_worldDataPack, 5);
    dst = (WmTexScrollTbl *)g_worldTexScrolls;
    *dst = *(WmTexScrollTbl *)w_framePackGetPtr(g_worldDataPack, 6);
    g_worldEncounterEventRows = w_framePackGetPtr(g_worldDataPack, 4);
}

void wm_applyEncounterEvents(void) {
    s32 i;
    s32 j;
    u16 (*row)[12];
    s32 mask;
    WmEncounter *tbl;
    u16 v;

    i = 0;
    tbl = g_worldEncounterTable;
    row = g_worldEncounterEventRows;
    mask = g_evtGlobalVars.b[0xC2] | (g_evtGlobalVars.b[0xC6] << 8);
    do {
        if ((mask >> i) & 1) {
            j = 0;
            do {
                v = (*row)[j];
                if (v != 0xFFFF) {
                    tbl[v - 1].battle[3] = tbl[v - 1].battle[2];
                }
                j++;
            } while (j < 12);
        }
        i++;
        row++;
    } while (i < 9);
    for (i = 0; i < 3; i++) {
        g_worldAmbientSeDist[i] = 0xFFFF;
        g_worldAmbientSeOn[i] = 0;
    }
    if (*g_worldStoryCounterPtr - 0x1B4EU < 0x1004) {
        g_worldBlockFx7Spin = 0;
    } else {
        g_worldBlockFx7Spin = 1;
    }
    wm_blockListInit();
    wm_initTextureLists();
}

void wm_drawMap(void) {
    WmPack *p;
    MATRIX *o;

    SetBackColor(g_worldWeatherData.work[0].backColor.c[0], g_worldWeatherData.work[0].backColor.c[2], g_worldWeatherData.work[0].backColor.c[4]);
    SetLightMatrix(&g_worldMapLightMatrix);
    SetColorMatrix(&g_worldMapColorMatrix);
    wm_updateTextureLists();
    wm_drawBlocks();
    SetBackColor(g_worldWeatherData.work[0].backdropBackColor.c[0], g_worldWeatherData.work[0].backdropBackColor.c[2], g_worldWeatherData.work[0].backdropBackColor.c[4]);
    if (g_worldBackdropOn != 0) {
        MATRIX m1 = { { { 0x1000, 0, 0 }, { 0, 0x1000, 0 }, { 0, 0, 0x1000 } }, { 0, 0xE10, 0 } };
        MATRIX out;
        MATRIX m2 = *g_worldViewMatrix;

        if (g_worldFog != 0) {
            m1.t[1] = -g_worldCameraEye[1] - 0x9C4;
        } else {
            m1.t[1] = -g_worldCameraEye[1] - 0x258;
        }
        SetLightMatrix(&g_worldBackdropLightMatrix);
        SetColorMatrix(&g_worldBackdropColorMatrix);
        p = w_framePackGetPtr(g_worldDataPack, 0x3B);
        m2.t[0] = 0;
        m2.t[1] = 0;
        m2.t[2] = 0;
        o = &out;
        CompMatrix(&m1, &m2, o);
        SetRotMatrix(o);
        SetTransMatrix(o);
        wm_drawBackdrop((s32)p->entries[0], 0);
    }
}

void wm_updateMap(void) {
    s32 m;
    s32 flags;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 h;
    s32 v;

    m = g_worldPlayer->actorType;
    flags = g_worldActorTypeInfo[m].cellAttr;
    if (g_worldMusicStarted != 0) {
        wm_updateChunks();
        wm_updateVisibleBlocks();
    }
    for (i = 0; i < 3; i++) {
        dx = (g_worldPlayer->pos.vx >> 8) - (g_worldAmbientSeX[i] >> 8);
        dz = (g_worldPlayer->pos.vz >> 8) - (g_worldAmbientSeZ[i] >> 8);
        g_worldAmbientSeDist[i] = SquareRoot0(dx * dx + dz * dz);
        if (g_worldFormFlags[5] != 0 && i == 0) {
            g_worldAmbientSeDist[0] = 100;
        }
        v = g_worldAmbientSeDist[i];
        if (i == 0 && g_worldDataSet == WM_DATASET_DISC4) {
            continue;
        }
        if (v < 0x3F) {
            if (g_worldAmbientSeOn[i] == 0) {
                g_worldAmbientSeOn[i] = 1;
                wm_sePlay(g_worldAmbientSe[i], 0);
            } else {
                dz = 0x7F - v * 2;
                if (dz > 0) {
                    wm_seSetVolume(g_worldAmbientSe[i], dz);
                }
            }
        } else {
            if (g_worldAmbientSeOn[i] == 1) {
                g_worldAmbientSeOn[i] = 0;
                wm_seStop(g_worldAmbientSe[i]);
            }
        }
    }
    if (g_worldAmbientSeDist[2] < 0x28) {
        d = 0x78 - g_worldAmbientSeDist[2] * 3;
        if (d != 0 && g_worldMapLoaded != 0) {
            g_worldCameraTarget[1] += rand() % d - d / 2;
            g_worldCameraTarget[0] += rand() % d - d / 2;
            g_worldCameraTarget[2] += rand() % d - d / 2;
        }
    }
    if (g_worldAmbientSeDist[1] < 0x22 && evt_isControlEnabled() != 0 && WM_ATTR_TOPOGRAPHY(flags) == 0x17 && m - 1 < 7U) {
        g_worldPlayer->pos.vz += (0x22 - g_worldAmbientSeDist[1]) * 8;
        if (g_worldPlayer->pos.vx == g_worldPlayer->prevPos.vx && g_worldPlayer->pos.vz == g_worldPlayer->prevPos.vz) {
            g_worldPlayer->prevPos.vx++;
        }
    }
    if (g_evtGlobalVars.b[0x66] == 8 || g_evtGlobalVars.b[0x66] == 0) {
        dx = (g_worldPlayer->pos.vx >> 3) - 0x790A;
        dz = (g_worldPlayer->pos.vz >> 3) + 0x5ACB;
        v = SquareRoot0(dx * dx + ((g_worldPlayer->pos.vy >> 3) + 0x66) * ((g_worldPlayer->pos.vy >> 3) + 0x66) * 8 + dz * dz);
        if (v < 0x47C) {
            h = 0x47C - v;
            if (h > 0x400) {
                h = 0x400;
            }
            if (h > 0x10) {
                h >>= 4;
                wrain_set(h, 0x40);
                g_worldPointFxOn = 1;
                g_gameState->gameData->rainCount = h >> 4;
                return;
            }
        }
    }
    g_worldPointFxOn = 0;
    g_gameState->gameData->rainCount = 0;
}

/* mark the map cells changed by the story progress */
void wm_applyStoryForms(void) {
    s32 i;
    s32 n;
    u8 *p;

    i = 15;
    p = &g_worldFormFlags[i];
    for (; i >= 0; i--) {
        *p-- = 0;
    }
    for (i = 10; i >= 0; i--) {
        if (g_worldFormStoryThresholds[i] <= *g_worldStoryCounterPtr) {
            break;
        }
    }
    if (g_worldMapDebugFlags & WM_DEBUG_FORM) {
        printf("Scene:%d / Status:%d\n", *g_worldStoryCounterPtr, i);
    }
    if (i >= 0 && g_worldDataSet == WM_DATASET_DISC1) {
        for (n = 0; n < 7; n++) {
            switch (g_worldFormStatusTable[n][i]) {
            case 1:
                WM_CELL(18, 14) |= 0x80;
                WM_CELL(17, 12) |= 0x80;
                g_worldFormFlags[0] = 1;
                break;
            case 6:
                WM_CELL(13, 12) |= 0x80;
                WM_CELL(14, 12) |= 0x80;
                g_worldFormFlags[5] = 1;
                break;
            case 5:
                WM_CELL(13, 16) |= 0x80;
                WM_CELL(13, 17) |= 0x80;
                WM_CELL(14, 16) |= 0x80;
                WM_CELL(14, 17) |= 0x80;
                g_worldFormFlags[4] = 1;
                break;
            case 9:
                WM_CELL(14, 6) |= 0x80;
                WM_CELL(21, 10) |= 0x80;
                WM_CELL(22, 14) |= 0x80;
                g_worldFormFlags[8] = 1;
                break;
            case 3:
                WM_CELL(19, 10) |= 0x80;
                WM_CELL(19, 11) |= 0x80;
                WM_CELL(20, 10) |= 0x80;
                WM_CELL(20, 11) |= 0x80;
                g_worldFormFlags[2] = 1;
                break;
            case 11:
                WM_CELL(3, 9) |= 0x80;
                WM_CELL(9, 1) |= 0x80;
                g_worldFormFlags[10] = 1;
                break;
            case 4:
                WM_CELL(7, 1) |= 0x80;
                WM_CELL(8, 1) |= 0x80;
                WM_CELL(14, 15) |= 0x80;
                g_worldFormFlags[3] = 1;
                break;
            }
            /* the original second jump table is 8-aligned */
            __asm__(".rdata\n\t.word 0\n\t.text");
            if (g_worldMapDebugFlags & WM_DEBUG_FORM) {
                switch (g_worldFormStatusTable[n][i]) {
                case 0:
                    printf("BGI Form : SGATE\n");
                    break;
                case 5:
                    printf("BGI Form : CLAY\n");
                    break;
                case 4:
                    printf("BGI Form : LIND\n");
                    break;
                case 8:
                    printf("BGI Form : KUROMA\n");
                    break;
                case 2:
                    printf("BGI Form : ALEX\n");
                    break;
                case 10:
                    printf("BGI Form : HOKORA(MIZU)\n");
                    break;
                case 3:
                    printf("BGI Form : HOKORA(HI)\n");
                    break;
                }
            }
        }
    }
    printf("General Global Flag = %x\n", D_8007AF55);
    if (D_8007AF55 & 0x80) {
        printf("- Replace Mog Net -\n");
        WM_CELL(16, 1) |= 0x80;
        WM_CELL(13, 4) |= 0x80;
        WM_CELL(14, 5) |= 0x80;
        g_worldFormFlags[12] = 1;
    }
    if (D_8007AF55 & 0x40) {
        printf("- Replace Chocobo Heaven -\n");
        WM_CELL(0, 0) |= 0x80;
        WM_CELL(16, 14) |= 0x80;
        WM_CELL(9, 17) |= 0x80;
        g_worldFormFlags[7] = 1;
    }
}

/* pick the encounter table entry for the player's area, zone and fog */
WmEncounter *wm_getEncounterEntry(void) {
    u8 zone;
    s32 area;
    s32 lo;
    s32 hi;
    s32 i;
    u32 t;
    s32 f;
    s32 add;

    area = WM_ATTR_AREA(g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr);
    zone = g_worldAreaZone[area];
    lo = g_worldEncounterZoneStart[zone];
    hi = g_worldEncounterZoneStart[zone + 1];
    if (g_worldMapDebugFlags & WM_DEBUG_ENCOUNTER) {
        printf("- Encount Information -\n");
        printf("Now Area :%d\n", WM_ATTR_AREA(g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr));
        printf("Now Zone :%d\n", zone);
        printf("Now Fog  :%d\n", g_worldFog);
        printf("Topograph:%d\n", (u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2);
    }
    for (i = lo; i < hi; i++) {
        t = g_worldEncounterTable[i].topoPattern >> 2;
        f = g_worldEncounterTable[i].zoneFog & 1;
        if (g_worldMapDebugFlags & WM_DEBUG_ENCOUNTER) {
            printf("Matching:%d  ( T:%d / F:%d )\n", i, t, f);
        }
        if (t == (u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2 && f == g_worldFog) {
            if (g_worldMapDebugFlags & WM_DEBUG_ENCOUNTER) {
                printf("Hit %d encount data\n", i);
            }
            add = 0;
            if (g_worldDataSet == WM_DATASET_DISC4 && i < 100) {
                add = 0xFE;
            }
            return &g_worldEncounterTable[i + add];
        }
    }
    printf(D_800A7F48);
    return &g_worldEncounterTable[i + add - 1];
}

s32 wm_worldToView(VECTOR *arg0, VECTOR *arg1, VECTOR *arg2, s32 *arg3) {
    VECTOR v;
    s32 s;

    v.vx = arg0->vx - g_worldCameraEye[0];
    v.vy = arg0->vy - g_worldCameraEye[1];
    v.vz = arg0->vz - g_worldCameraEye[2];
    if (v.vx > g_worldWidth - 0x8000) {
        v.vx -= g_worldWidth;
    }
    if (v.vz > g_worldDepth - 0x8000) {
        v.vz -= g_worldDepth;
    }
    if (v.vx < 0x8000 - g_worldWidth) {
        v.vx = g_worldWidth + v.vx;
    }
    if (v.vz < 0x8000 - g_worldDepth) {
        v.vz = g_worldDepth + v.vz;
    }
    ApplyMatrixLV(g_worldViewMatrix, &v, arg2);
    s = 0x1000 - ((arg2->vz >> 4) + (arg2->vz >> 6));
    v.vy = ((arg0->vy * s) >> 12) - g_worldCameraEye[1];
    ApplyMatrixLV(g_worldViewMatrix, &v, arg2);
    arg1->vx = arg0->vx - g_worldCameraTarget[0];
    arg1->vy = ((arg0->vy * s) >> 12) - g_worldCameraTarget[1];
    arg1->vz = arg0->vz - g_worldCameraTarget[2];
    if (arg1->vx > 0xFFFF) {
        arg1->vx -= 0x60000;
    }
    if (arg1->vz > 0xFFFF) {
        arg1->vz -= 0x50000;
    }
    if (arg1->vx < -0xFFFF) {
        arg1->vx += 0x60000;
    }
    if (arg1->vz < -0xFFFF) {
        arg1->vz += 0x50000;
    }
    if (arg3 != NULL) {
        *arg3 = s;
    }
    if (arg1->vx < -0x8000 || arg1->vx > 0x7FFF || arg1->vz > 0x7FFF || arg1->vz < -0x8000) {
        return 0;
    }
    return 1;
}

void wm_mapTerm(void) {
}

void wm_mapSystemTerm(void) {
}

void wm_wrapPos(s32 *arg0, s32 *arg1) {
    if (*arg0 < 0) {
        *arg0 += g_worldWidth;
    }
    if (*arg0 >= g_worldWidth) {
        *arg0 -= g_worldWidth;
    }
    if (*arg1 > 0) {
        *arg1 -= g_worldDepth;
    }
    if (*arg1 <= -g_worldDepth) {
        *arg1 += g_worldDepth;
    }
}

void wm_posToCell(s32 arg0, s32 arg1, s32 *arg2) {
    s32 x;
    s32 z;

    wm_wrapPos_inline(&arg0, &arg1);
    x = arg0 / 0x2000;
    z = arg1 / 0x2000;
    *arg2 = -z * g_worldCellsW + x;
}

void wm_posToChunk(s32 arg0, s32 arg1, s32 *arg2) {
    s32 x;
    s32 z;

    wm_wrapPos_inline(&arg0, &arg1);
    x = arg0 / 0x4000;
    z = arg1 / 0x4000;
    *arg2 = -z * g_worldChunksW + x;
}

void wm_cellToPos(s32 arg0, s32 *arg1, s32 *arg2) {
    *arg1 = (arg0 % g_worldCellsW) << 13;
    *arg2 = -((arg0 / g_worldCellsW) << 13);
}

void wm_cellToChunk(s32 arg0, s32 *arg1) {
    *arg1 = arg0 / g_worldCellsW / 2 * g_worldChunksW + arg0 % g_worldCellsW / 2;
}

void wm_chunkToPos(s32 arg0, s32 *arg1, s32 *arg2) {
    *arg1 = (arg0 % g_worldChunksW) << 14;
    *arg2 = -((arg0 / g_worldChunksW) << 14);
}

void wm_getChunkEntry(s32 arg0, s32 arg1, WmChunkEntry *arg2) {
    WmChunkEntry *cells;

    cells = g_worldChunkTable->entries;
    arg2->sector = cells[arg0 * 2 + arg1].sector;
    arg2->packSectors = cells[arg0 * 2 + arg1].packSectors;
    arg2->texSectors = cells[arg0 * 2 + arg1].texSectors;
}

void wm_getChunkEntryT(s32 arg0, s32 arg1, WmChunkEntry *arg2) {
    s32 k;
    WmChunkEntry *cells;

    k = arg0 % g_worldChunksW * g_worldChunksH + arg0 / g_worldChunksW;
    cells = g_worldChunkTableT->entries;
    arg2->sector = cells[k * 2 + arg1].sector;
    arg2->packSectors = cells[k * 2 + arg1].packSectors;
    arg2->texSectors = cells[k * 2 + arg1].texSectors;
}

s32 wm_getAreaZone(s32 arg0) {
    return g_worldAreaZone[arg0];
}

/* initialise the texture pack list */
void wm_chunkListInit(void) {
    u32 i;

    g_worldChunkListDirty = 0;
    g_worldChunkSlots[0].prev = &g_worldChunkHead;
    g_worldChunkSlots[0].state = 0;
    g_worldChunkSlots[0].next = &g_worldChunkSlots[1];
    g_worldChunkSlots[0].chunkId = -1;
    g_worldChunkSlots[0].generation = 0;
    g_worldChunkSlots[0].pack = (WmChunkPack *)g_worldChunkBuffer;
    for (i = 1; i < 31; i++) {
        g_worldChunkSlots[i].prev = &g_worldChunkSlots[i - 1];
        g_worldChunkSlots[i].next = &g_worldChunkSlots[i + 1];
        g_worldChunkSlots[i].pack = NULL;
        g_worldChunkSlots[i].state = 0;
        g_worldChunkSlots[i].chunkId = -1;
        g_worldChunkSlots[i].generation = 0;
    }
    for (i = 1; i < 16; i++) {
        g_worldChunkSlots[i].pack = (WmChunkPack *)(g_worldChunkBuffer + i * 0x4800);
        g_worldChunkSlots[i].generation = 0;
    }
    i = 31;
    g_worldChunkSlots[i].prev = &g_worldChunkSlots[i - 1];
    g_worldChunkSlots[i].next = &g_worldChunkTail;
    g_worldChunkSlots[i].chunkId = -1;
    g_worldChunkSlots[i].pack = NULL;
    g_worldChunkSlots[i].state = 0;
    g_worldChunkSlots[i].generation = 0;
    g_worldChunkHead.prev = NULL;
    g_worldChunkHead.next = &g_worldChunkSlots[0];
    g_worldChunkTail.next = &g_worldChunkSlots[16];
    g_worldChunksLoadedCount = 0;
    g_worldChunkCenter = -1;
    g_worldChunkTail.prev = NULL;
    g_worldMapLoaded = 0;
    wm_readtxReset();
    wm_frmbufReset();
}

void wm_updateChunks(void) {
    s32 t;
    s32 a;
    s32 b;

    if (g_worldMoveMode < 10) {
        if (g_worldMoveMode >= 8) {
            t = g_worldPlayerSpeed;
            if (t >= 0x200) {
                t = 0x1FF;
            }
            if (t < 0x100) {
                t = 0x100;
            }
            g_worldChunkLookPos[0] = g_worldCameraEye[0] + ((g_worldCameraDir[0] * g_worldMoveModeInfo->lookAhead * t) >> 9);
            g_worldChunkLookPos[2] = g_worldCameraEye[2] + ((g_worldCameraDir[2] * g_worldMoveModeInfo->lookAhead * t) >> 9);
        } else {
            g_worldChunkLookPos[0] = g_worldCameraEye[0] + g_worldCameraDir[0] * g_worldMoveModeInfo->lookAhead;
            g_worldChunkLookPos[2] = g_worldCameraEye[2] + g_worldCameraDir[2] * g_worldMoveModeInfo->lookAhead;
        }
    } else {
        g_worldChunkLookPos[0] = g_worldCameraEye[0] + g_worldCameraDir[0] * g_worldMoveModeInfo->lookAhead;
        g_worldChunkLookPos[2] = g_worldCameraEye[2] + g_worldCameraDir[2] * g_worldMoveModeInfo->lookAhead;
    }
    wm_wrapPos(&g_worldChunkLookPos[0], &g_worldChunkLookPos[2]);
    wm_posToCell(g_worldChunkLookPos[0] - 0x6000, g_worldChunkLookPos[2] + 0x6000, &a);
    wm_cellToChunk(a, &b);
    if (b != g_worldChunkCenter) {
        wm_requestChunks(b);
    }
    wm_readChunks();
    g_worldChunkCenter = b;
}

void wm_requestChunks(s32 arg0) {
    s32 missing[16];
    s32 x;
    s32 z;
    s32 id;
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    WmChunkSlot *p;
    WmChunkSlot *h;
    u8 c;

    n = 0;
    h = &g_worldChunkHead;
    wm_chunkToPos(arg0, &x, &z);
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            wm_posToChunk(x + (i << 14), z - (j << 14), &id);
            p = h->next;
            for (k = 0; k < 16; k++, p = p->next) {
                if (p->chunkId == id) {
                    c = g_worldChunkFlags[p->chunkId];
                    if (c & 0x3F) {
                        w_readtxRequest(c);
                    }
                    p->prev->next = p->next;
                    p->next->prev = p->prev;
                    p->next = h->next;
                    p->prev = h;
                    h->next->prev = p;
                    h->next = p;
                    break;
                }
            }
            if (k == 16) {
                missing[n++] = id;
            }
        }
    }
    for (k = 0; k < n; k++) {
        if (k == 4) {
            g_worldChunkGeneration++;
    g_worldChunkGeneration %= 4;
        }
        wm_chunkRequest(missing[k]);
        c = g_worldChunkFlags[missing[k]];
        if (c & 0x3F) {
            w_readtxRequest(c);
        }
    }
    wm_chunkListCompact();
    wm_readtxCompact();
    g_worldChunkGeneration++;
    g_worldChunkGeneration %= 4;
}

/* request texture pack arg0: reuse a free or matching slot and move it behind the loaded ones */
s32 wm_chunkRequest(s16 arg0) {
    WmChunkSlot *prev;
    WmChunkSlot *p;
    u32 n;

    n = 0;
    prev = &g_worldChunkHead;
    while (prev->next->state == 1) {
        if (prev->next->chunkId == arg0) {
            return 0;
        }
        prev = prev->next;
        n++;
    }
    p = prev->next;
    do {
        if (p->chunkId == -1) {
            if (n >= 16) {
                p->chunkId = arg0;
                p->prev->next = p->next;
                p->next->prev = p->prev;
                p->next = prev->next;
                p->prev = prev;
                g_worldChunkListDirty++;
                prev->next->prev = p;
                prev->next = p;
            } else {
                p->chunkId = arg0;
                p->state = 0;
            }
            break;
        }
        if (p->chunkId == arg0) {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            p->next = prev->next;
            p->prev = prev;
            prev->next->prev = p;
            prev->next = p;
            break;
        }
        p = p->next;
        n++;
    } while (p != &g_worldChunkTail);
    if (p != &g_worldChunkTail) {
        p->generation = g_worldChunkGeneration;
    }
    return n != 32;
}

void wm_readChunks(void) {
    WmChunkEntry a;
    WmChunkEntry b;
    WmChunkSlot *n;
    u8 *cell;
    s32 id;
    s16 id2;
    s32 i;
    s32 r;
    s32 base;
    WmChunkSlot *h;

    if (g_worldCdUser == WM_CD_FREE) {
        wm_findChunkRun(g_worldChunkRun, &g_worldChunkRunCount[g_worldChunkRunSlot], &g_worldChunkRunDir[g_worldChunkRunSlot]);
        if (g_worldChunkRunDir[g_worldChunkRunSlot] == 0) {
            goto end;
        }
        for (i = 0; i < g_worldChunkRunCount[g_worldChunkRunSlot]; i++) {
            n = g_worldChunkRun[i];
            h = &g_worldChunkHead;
            id2 = n->chunkId;
            id = n->chunkId;
            n->prev->next = n->next;
            n->next->prev = n->prev;
            n->next = h->next;
            n->prev = &g_worldChunkHead;
            cell = &g_worldChunkFlags[id];
            h->next->prev = n;
            h->next = n;
            n->state = 1;
            if (g_worldChunkRunDir[g_worldChunkRunSlot] == 1) {
                wm_getChunkEntry(id, 0, &a);
                wm_getChunkEntry(id, 1, &b);
            } else {
                wm_getChunkEntryT(id, 0, &a);
                wm_getChunkEntryT(id, 1, &b);
            }
            if ((*cell & 0x80) && b.packSectors == 0) {
                if (g_worldMapDebugFlags & WM_DEBUG_FORM) {
                    printf(D_800A7F94, id2);
                }
                *cell &= 0x7F;
            }
            r = (*cell & 0x3F) ? wm_readtxFind(*cell, 0) : 0;
            n->hasData = 1;
            if (!(*cell & 0x80)) {
                g_worldChunkReadReqs[i * 4 + 0].size = a.packSectors << 11;
                g_worldChunkReadReqs[i * 4 + 0].dest = n->pack;
                g_worldChunkReadReqs[i * 4 + 1].size = a.texSectors << 11;
                g_worldChunkReadReqs[i * 4 + 1].dest = (void *)r;
                g_worldChunkReadReqs[i * 4 + 2].size = b.packSectors << 11;
                g_worldChunkReadReqs[i * 4 + 2].dest = NULL;
                g_worldChunkReadReqs[i * 4 + 3].size = b.texSectors << 11;
                g_worldChunkReadReqs[i * 4 + 3].dest = NULL;
                if (g_worldChunkReadReqs[i * 4 + 1].size == 0) {
                    g_worldChunkReadReqs[i * 4 + 1].dest = NULL;
                }
                if (g_worldChunkReadReqs[i * 4 + 0].size == 0) {
                    g_worldChunkReadReqs[i * 4 + 0].dest = NULL;
                    n->hasData = 0;
                    g_worldChunkRunPacks[i] = NULL;
                    g_worldChunkRunTex[i] = NULL;
                } else {
                    g_worldChunkRunPacks[i] = g_worldChunkReadReqs[i * 4 + 0].dest;
                    g_worldChunkRunTex[i] = g_worldChunkReadReqs[i * 4 + 1].dest;
                }
            } else {
                g_worldChunkReadReqs[i * 4 + 0].size = a.packSectors << 11;
                g_worldChunkReadReqs[i * 4 + 0].dest = NULL;
                g_worldChunkReadReqs[i * 4 + 1].size = a.texSectors << 11;
                g_worldChunkReadReqs[i * 4 + 1].dest = NULL;
                g_worldChunkReadReqs[i * 4 + 2].size = b.packSectors << 11;
                g_worldChunkReadReqs[i * 4 + 2].dest = n->pack;
                g_worldChunkReadReqs[i * 4 + 3].size = b.texSectors << 11;
                g_worldChunkReadReqs[i * 4 + 3].dest = (void *)r;
                if (g_worldChunkReadReqs[i * 4 + 3].size == 0) {
                    g_worldChunkReadReqs[i * 4 + 3].dest = NULL;
                }
                if (g_worldChunkReadReqs[i * 4 + 2].size == 0) {
                    g_worldChunkReadReqs[i * 4 + 2].dest = NULL;
                    n->hasData = 0;
                    g_worldChunkRunPacks[i] = NULL;
                    g_worldChunkRunTex[i] = NULL;
                } else {
                    g_worldChunkRunPacks[i] = g_worldChunkReadReqs[i * 4 + 2].dest;
                    g_worldChunkRunTex[i] = g_worldChunkReadReqs[i * 4 + 3].dest;
                }
            }
        }
        if (g_worldChunkRunDir[g_worldChunkRunSlot] == 1) {
            wm_getChunkEntry(g_worldChunkRun[0]->chunkId, 0, &a);
            base = g_worldImageSectors[3];
        } else {
            wm_getChunkEntryT(g_worldChunkRun[0]->chunkId, 0, &a);
            base = g_worldImageSectors[4];
        }
        g_worldChunkReadCount = 0;
        for (i = 0; i < g_worldChunkRunCount[g_worldChunkRunSlot] * 4; i++) {
            if (g_worldChunkReadReqs[i].size != 0) {
                g_worldChunkReadList[g_worldChunkReadCount].dest = g_worldChunkReadReqs[i].dest;
                g_worldChunkReadList[g_worldChunkReadCount++].size = g_worldChunkReadReqs[i].size;
            }
        }
        g_worldCdUser = WM_CD_CHUNKS;
        if (g_worldChunkReadCount != 0) {
            cd_readChunks(a.sector + base, g_worldChunkReadList, g_worldChunkReadCount, 0);
        }
    }
end:
    if (g_worldCdUser == WM_CD_CHUNKS && cd_getState() == 0) {
        wm_chunksLoaded();
    }
}

u8 wm_findChunkRun(WmChunkSlot **out, s32 *count, s32 *dir) {
    WmChunkSlot *h[10];
    WmChunkSlot *v[10];
    s32 hhi;
    s32 hlo;
    s32 vhi;
    s32 vlo;
    s32 i;
    WmChunkSlot *p;

    hhi = 4;
    hlo = 4;
    *dir = 0;
    *count = 0;
    vhi = 4;
    vlo = 4;
    for (p = g_worldChunkHead.next; p->pack != NULL; p = p->next) {
        if (p->state == 0 && p->chunkId != -1 && p->generation == g_worldChunkRunGeneration) {
            h[hlo] = p;
            h[hhi] = p;
            v[vlo] = p;
            v[vhi] = p;
            for (i = 0; i < 4; i++) {
                for (p = g_worldChunkHead.next; p->pack != NULL; p = p->next) {
                    if (p->state == 0) {
                        if (h[hlo]->chunkId - 1 == p->chunkId) {
                            h[--hlo] = p;
                        }
                        if (h[hhi]->chunkId + 1 == p->chunkId) {
                            h[++hhi] = p;
                        }
                        if (v[vlo]->chunkId - 24 == p->chunkId) {
                            v[--vlo] = p;
                        }
                        if (v[vhi]->chunkId + 24 == p->chunkId) {
                            v[++vhi] = p;
                        }
                    }
                }
            }
            if (hhi - hlo < vhi - vlo) {
                *dir = 24;
                for (i = vlo; i <= vhi; i++) {
                    out[(*count)++] = v[i];
                }
            } else {
                *dir = 1;
                for (i = hlo; i <= hhi; i++) {
                    out[(*count)++] = h[i];
                }
            }
            break;
        }
    }
    g_worldChunkRunGeneration++;
    g_worldChunkRunGeneration %= 4;
    return *count;
}

void wm_chunksLoaded(void) {
    s32 i;
    u8 st;

    for (i = 0; i < g_worldChunkRunCount[g_worldChunkRunSlot]; i++) {
        st = g_worldChunkRun[i]->state;
        if (st == 1) {
            g_worldChunkRun[i]->state = 2;
            if (g_worldChunkRunPacks[i] != NULL) {
                wm_relocatePack(g_worldChunkRunPacks[i]);
            }
            if (g_worldChunkRunTex[i] != NULL) {
                wm_readtxDone(g_worldChunkRunTex[i]);
            }
            if (g_worldMapLoaded == 0) {
                if (++g_worldChunksLoadedCount == 16) {
                    g_worldLoadedFrames = st;
                    g_worldCameraHold = 0;
                    g_worldMapLoaded = st;
                    g_worldState = WM_STATE_RUNNING;
                    g_padPortMask[0] &= ~(BTN_CIRCLE | BTN_CROSS | BTN_SQUARE);
                    wm_snapActorsToFloor();
                }
            }
        }
    }
    g_worldCdUser = WM_CD_FREE;
}

void wm_initTextureLists(void) {
    wm_readtxInit();
    wm_frmbufInit();
    g_worldChunkGeneration = 0;
}

void wm_updateTextureLists(void) {
    wm_readtxUpdate();
    wm_frmbufUpdate();
}

void wm_chunkListCompact(void) {
    WmChunkSlot *a;
    WmChunkSlot *b;
    u32 i;

    if (g_worldChunkListDirty != 0) {
        b = g_worldChunkHead.next;
        g_worldChunkListDirty = 0;
        a = b;
        for (i = 0; i < 16; i++) {
            b = b->next;
        }
        while (1) {
            if (b == &g_worldChunkTail) {
                break;
            }
            while (a->pack != 0) {
                a = a->next;
            }
            a->pack = b->pack;
            b->pack = 0;
            b->chunkId = -1;
            b->state = 0;
            b = b->next;
        }
    }
}

void func_800BD66C(void) {
}

/* initialise the texture list */
void wm_blockListInit(void) {
    u32 i;

    g_worldBlockListDirty = 0;
    g_worldBlockNodes[0].prev = &g_worldBlockHead;
    g_worldBlockNodes[0].pos = &g_worldBlockPositions[0];
    g_worldBlockNodes[0].next = &g_worldBlockNodes[1];
    g_worldBlockNodes[0].block = NULL;
    g_worldBlockNodes[0].state = 0;
    g_worldBlockNodes[0].cell = -1;
    g_worldBlockNodes[0].slot = 0;
    for (i = 1; i < 39; i++) {
        g_worldBlockNodes[i].prev = &g_worldBlockNodes[i - 1];
        g_worldBlockNodes[i].next = &g_worldBlockNodes[i + 1];
        g_worldBlockNodes[i].block = NULL;
        g_worldBlockNodes[i].pos = NULL;
        g_worldBlockNodes[i].state = 0;
        g_worldBlockNodes[i].cell = -1;
        g_worldBlockNodes[i].slot = -1;
    }
    for (i = 1; i < 20; i++) {
        g_worldBlockNodes[i].pos = &g_worldBlockPositions[i];
        g_worldBlockNodes[i].slot = i;
    }
    i = 39;
    g_worldBlockNodes[i].prev = &g_worldBlockNodes[i - 1];
    g_worldBlockNodes[i].next = &g_worldBlockTail;
    g_worldBlockNodes[i].block = NULL;
    g_worldBlockNodes[i].pos = NULL;
    g_worldBlockNodes[i].state = 0;
    g_worldBlockNodes[i].cell = -1;
    g_worldBlockNodes[i].slot = -1;
    g_worldBlockHead.prev = NULL;
    g_worldBlockHead.next = &g_worldBlockNodes[0];
    g_worldBlockTail.next = &g_worldBlockNodes[20];
    g_worldBlockTail.prev = NULL;
}

void wm_drawBlocks(void) {
    WmBlockNode *n;
    WmBlockPos *pos;
    MATRIX m;
    WmQuad quad = g_worldBlockBackQuad;
    long sxy[4];
    VECTOR a;
    VECTOR b;
    long p;
    long flag;
    long xy3;
    u8 snd;
    s32 far;
    s32 k;
    u32 *ot;
    POLY_F4 *f4;

    n = g_worldBlockHead.next;
    while (n->cell != -1) {
        pos = n->pos;
        g_worldBlockMatrix.t[0] = pos->pos[0] - g_worldCameraTarget[0];
        g_worldBlockMatrix.t[1] = pos->pos[1] - g_worldCameraTarget[1];
        g_worldBlockMatrix.t[2] = pos->pos[2] - g_worldCameraTarget[2];
        CompMatrix(g_worldViewMatrix, &g_worldBlockMatrix, &m);
        SetRotMatrix(&m);
        SetTransMatrix(&m);
        wm_worldToView((VECTOR *)pos, &a, &b, 0);
        far = b.vz < -10000 || b.vz > 11000;
        k = n->cell % g_worldCellsW + n->cell / g_worldCellsW;
        if (n->state == 2) {
            wm_drawBlock((u8 *)n->block, (n->cell % g_worldCellsW + n->cell / g_worldCellsW) % 2, far);
            if (RotTransPers4(&quad.v[0], &quad.v[1], &quad.v[2], &quad.v[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag) < 7000
                && g_worldPlayer->pos.vy > -7000) {
                f4 = (POLY_F4 *)g_worldPrimPtr;
                ot = (u32 *)g_worldOt[*g_worldBufIndex];
                *(s32 *)&f4->x0 = sxy[0];
                *(s32 *)&f4->x1 = sxy[1];
                *(s32 *)&f4->x2 = sxy[2];
                xy3 = sxy[3];
                setlen(f4, 5);
                setRGB0(f4, 0, 0, 0);
                f4->code = 0x28;
                *(s32 *)&f4->x3 = xy3;
                addPrim(&ot[0xFFA], f4);
                f4++;
                g_worldPrimPtr = (s32)f4;
            }
        }
        if (n->block != NULL) {
            snd = n->block->fxId;
            if (snd != 0) {
                wm_drawBlockFx(snd);
            }
        }
        n = n->next;
    }
}

void wm_updateVisibleBlocks(void) {
    VECTOR pos;
    VECTOR p2;
    s16 sx[20];
    s16 sy[20];
    s32 z[20];
    VECTOR d;
    WmNearCell tmp;
    MATRIX m;
    SVECTOR sv;
    VECTOR out;
    s32 cell;
    long flag;
    long p;
    long sxy;
    s32 n;
    s32 i;
    s32 j;
    s32 k;
    s32 hit;
    s32 cnt;
    s32 cnt2;
    s32 last;
    s32 id;
    WmBlock *e;
    SVECTOR *verts;
    WmScrRect *r;
    WmNearCell *c;
    WmBlockNode *nd;
    s32 lim;
    s32 t;

    pos.vy = 0;
    wm_blockListInit();
    pen_locate(10, 10);
    n = 0;
    if (g_worldMoveMode < 6) {
        if (g_worldPlayer->pos.vy >= -0x9C3) {
            g_worldBlockCullRects[1].y0 = g_worldBlockCullRects[0].y0 = -0x3C;
        } else {
            g_worldBlockCullRects[1].y0 = g_worldBlockCullRects[0].y0 = -0x1E;
        }
    }
    for (j = -3; j < 4; j++) {
        for (i = -3; i < 4; i++) {
            wm_posToCell(g_worldCameraTarget[0] + (i << 13) + g_worldCameraDir[0] * 2,
                          g_worldCameraTarget[2] - (j << 13) + g_worldCameraDir[2] * 2, &cell);
            e = (WmBlock *)wm_getCellBlock(cell, NULL);
            if (e != NULL) {
                wm_cellToPos(cell, &pos.vx, &pos.vz);
                g_worldBlockMatrix.t[0] = pos.vx - g_worldCameraTarget[0];
                g_worldBlockMatrix.t[1] = pos.vy - g_worldCameraTarget[1];
                g_worldBlockMatrix.t[2] = pos.vz - g_worldCameraTarget[2];
                CompMatrix(g_worldViewMatrix, &g_worldBlockMatrix, &m);
                SetRotMatrix(&m);
                SetTransMatrix(&m);
                hit = 0;
                if (m.t[2] >= -0x1D4B) {
                    k = (wm_getCameraViewState() == 0) * 2;
                    if (g_worldMoveModeInfo->controlType == 1) {
                        k += 4;
                    }
                    if (g_worldMoveModeInfo->controlType == 2) {
                        k += 8;
                    }
                    verts = e->verts;
                    for (cnt = 0; cnt < e->outlineCount; cnt++) {
                        sv = verts[e->outline[cnt]];
                        RotTrans(&sv, &out, &flag);
                        sv.vy = sv.vy * (0x1000 - (out.vz >> 4)) >> 12;
                        z[cnt] = RotTransPers(&sv, &sxy, &p, &flag);
                        sy[cnt] = (u32)sxy >> 16;
                        sx[cnt] = sxy;
                        if (z[cnt] < 0x898) {
                            k |= 1;
                        } else {
                            k &= ~1;
                        }
                        if ((sx[cnt] >= g_worldBlockCullRects[k].x0) & (g_worldBlockCullRects[k].x1 >= sx[cnt]) & (sy[cnt] >= g_worldBlockCullRects[k].y0) & (g_worldBlockCullRects[k].y1 >= sy[cnt])) {
                            hit = 1;
                            break;
                        }
                    }
                }
                if (hit == 1) {
                    p2.vx = pos.vx + 0x1000;
                    p2.vy = pos.vy;
                    p2.vz = pos.vz - 0x1000;
                    d.vx = p2.vx - g_worldCameraEye[0];
                    d.vz = p2.vz - g_worldCameraEye[2];
                    if (d.vx > 0xFFFF) {
                        d.vx -= g_worldWidth;
                    }
                    if (d.vx < -0xFFFF) {
                        d.vx += g_worldWidth;
                    }
                    if (d.vz > 0xFFFF) {
                        d.vz -= g_worldDepth;
                    }
                    if (d.vz < -0xFFFF) {
                        d.vz += g_worldDepth;
                    }
                    g_worldNearCells[n].dist = SquareRoot0((d.vx >> 2) * (d.vx >> 2) + (d.vz >> 2) * (d.vz >> 2));
                    g_worldNearCells[n].block = e;
                    g_worldNearCells[n].cell = cell;
                    n++;
                }
            }
        }
    }
    for (j = 0; j < n - 1; j++) {
        for (i = n - 1; j < i; i = t) {
            t = i - 1;
            if (g_worldNearCells[t].dist > g_worldNearCells[i].dist) {
                tmp = g_worldNearCells[i];
                g_worldNearCells[i] = g_worldNearCells[t];
                g_worldNearCells[t] = tmp;
            }
        }
    }
    lim = 16;
    if (g_worldMoveMode == 7) {
        lim = 14;
    }
    if (g_worldFog == 0) {
        lim++;
    }
    if (lim < n) {
        n = lim;
    }
    cnt2 = 0;
    last = -1;
    for (i = 0; i < n; i++) {
        id = g_worldNearCells[i].block->texArea;
        wm_blockListRequest(g_worldNearCells[i].cell);
        if (id != 0) {
            if (cnt2 < 2) {
                w_frmbufRequest(id);
            }
            if (id != last) {
                cnt2++;
            }
            if (cnt2 == 1) {
                last = id;
            }
        }
    }
    wm_frmbufCompact();
    wm_frmbufTransfer();
    wm_blockListCompact();
    wm_blockListResolve();
    nd = g_worldBlockHead.next;
    g_worldClutAnimFx2 = 0;
    while (nd->cell != -1) {
        {
            u8 s = nd->block->fxId;
            if (s != 0) {
                wm_updateBlockFx(s);
            }
        }
        nd = nd->next;
    }
}

s32 wm_blockListRequest(s16 arg0) {
    WmBlockNode *n = g_worldBlockHead.next;
    u32 i = 0;

    do {
        if (n->cell == -1) {
            if (i >= 20) {
                n->cell = arg0;
                n->prev->next = n->next;
                n->next->prev = n->prev;
                n->next = g_worldBlockHead.next;
                n->prev = &g_worldBlockHead;
                g_worldBlockListDirty++;
                g_worldBlockHead.next->prev = n;
                g_worldBlockHead.next = n;
            } else {
                n->cell = arg0;
                n->state = 0;
            }
            break;
        }
        if (n->cell == arg0) {
            n->prev->next = n->next;
            n->next->prev = n->prev;
            n->next = g_worldBlockHead.next;
            n->prev = &g_worldBlockHead;
            g_worldBlockHead.next->prev = n;
            g_worldBlockHead.next = n;
            break;
        }
        n = n->next;
        i++;
    } while (i < 40);
    return i != 40;
}

void wm_blockListResolve(void) {
    WmBlockNode *n;
    WmChunkSlot *p;
    u32 i;
    u32 j;
    s32 id;
    s16 m;
    s32 k;

    n = g_worldBlockHead.next;
    for (i = 0; i < 20; i++, n = n->next) {
        if (n->cell < 0) {
            return;
        }
        wm_cellToChunk(n->cell, &id);
        n->state = 0;
        p = g_worldChunkHead.next;
        for (j = 0; j < 16; j++, p = p->next) {
            if (id == p->chunkId) {
                if (p->state == 2) {
                    n->state = 2;
                    wm_cellToPos(n->cell, &n->pos->pos[0], &n->pos->pos[2]);
                    n->pos->pos[1] = 0;
                    m = n->cell % 2;
                    k = m + (n->cell / g_worldCellsW) % 2 * 2;
                    if (p->hasData != 0) {
                        n->block = p->pack->blocks[k];
                        n->block->texArea = p->pack->texArea;
                    } else {
                        n->block = g_worldDefaultBlocks->blocks[k];
                        n->block->texArea = 0;
                    }
                    break;
                }
                n->state = 2;
                n->pos->pos[1] = 0;
                m = n->cell % 2;
                k = m + (n->cell / g_worldCellsW) % 2 * 2;
                wm_cellToPos(n->cell, &n->pos->pos[0], &n->pos->pos[2]);
                n->block = g_worldDefaultBlocks->blocks[k];
                n->block->texArea = 0;
            }
        }
    }
}

s32 wm_getCellBlock(s16 arg0, u8 *arg1) {
    s32 id;
    s32 i;
    WmChunkSlot *p;
    s16 m;
    s32 k;

    p = g_worldChunkHead.next;
    wm_cellToChunk(arg0, &id);
    for (i = 0; i < 16; i++, p = p->next) {
        if (id == p->chunkId && p->state == 2) {
            if (arg1 != NULL) {
                *arg1 = 0;
            }
            m = arg0 % 2;
            k = m + (arg0 / g_worldCellsW) % 2 * 2;
            if (p->hasData != 0) {
                return (s32)p->pack->blocks[k];
            }
            return (s32)g_worldDefaultBlocks->blocks[k];
        }
    }
    if (arg1 != NULL) {
        *arg1 = 1;
    }
    m = arg0 % 2;
    k = m + (arg0 / g_worldCellsW) % 2 * 2;
    return (s32)g_worldDefaultBlocks->blocks[k];
}

void wm_initBlockMatrix(void) {
    g_worldBlockMatrix.m[2][2] = 0x1000;
    g_worldBlockMatrix.m[1][1] = 0x1000;
    g_worldBlockMatrix.m[0][0] = 0x1000;
    g_worldBlockMatrix.t[2] = 0;
    g_worldBlockMatrix.t[1] = 0;
    g_worldBlockMatrix.t[0] = 0;
    g_worldBlockMatrix.m[2][1] = 0;
    g_worldBlockMatrix.m[2][0] = 0;
    g_worldBlockMatrix.m[1][2] = 0;
    g_worldBlockMatrix.m[1][0] = 0;
    g_worldBlockMatrix.m[0][2] = 0;
    g_worldBlockMatrix.m[0][1] = 0;
}

void wm_blockListCompact(void) {
    u32 i;
    WmBlockNode *a;
    WmBlockNode *b;

    if (g_worldBlockListDirty != 0) {
        a = g_worldBlockHead.next;
        g_worldBlockListDirty = 0;
        b = a;
        for (i = 0; i < 20; i++) {
            b = b->next;
        }
        for (i = 0; i < 20; i++) {
            if (a->pos == 0) {
                a->block = b->block;
                a->pos = b->pos;
                b->block = 0;
                b->pos = 0;
                b->cell = -1;
                b->state = 0;
                b = b->next;
            }
            a = a->next;
        }
    }
}

s32 wm_getGroundHeight(VECTOR *arg0, u32 *arg1, s32 *arg2, WmPolyCache *arg3, u16 *arg4) {
    VECTOR pos;
    VECTOR rel;
    VECTOR o;
    s32 cell;
    u8 flag;
    s32 r;

    pos = *arg0;
    wm_wrapPos(&pos.vx, &pos.vz);
    wm_posToCell(pos.vx, pos.vz, &cell);
    r = wm_getCellBlock(cell, &flag);
    if (flag == 0 && r != 0) {
        wm_cellToPos(cell, &o.vx, &o.vz);
        rel.vx = pos.vx - o.vx;
        rel.vy = pos.vy;
        rel.vz = pos.vz - o.vz;
        /* callers pass a 16-bit variable but a word is stored */
        *(s32 *)arg4 = wm_getBlockHeight(cell, (WmBlock *)r, &rel, arg1, arg2, arg3);
        if ((u8)g_worldMoveMode - 2U < 2) {
            if (*arg1 - 0x38 < 2) {
                *arg1 = 0x36;
            }
        }
        return 1;
    }
    *(s32 *)arg4 = pos.vy;
    return 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A7F48);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", D_800A7F94);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", g_worldBlockBackQuad);

void wm_naviInit(void) {
    s32 i;
    u8 *b;

    g_worldNaviCursorOn = 1;
    g_worldNaviActive = 1;
    g_worldNaviPrevMode = -1;
    g_worldTitleFade = 0;
    g_worldTitleDelay = 0x14;
    g_worldNaviPlace = 0x3F;
    g_worldNaviPlaceCycled = 0;
    g_worldTitlePhase = 3;
    g_worldNaviCursorDraw = 1;
    g_worldNaviVisible = 1;
    g_worldNaviMode = (b = g_evtGlobalVars.b)[0x64];
    i = 0x2E;
    if (*g_worldStoryCounterPtr >= 0x1766) {
        g_evtGlobalVars.h[i] |= 0x7C0;
        if (g_evtGlobalVars.h[i] & 0xC000) {
            g_evtGlobalVars.h[i] |= 0xC000;
        }
    }
    if (g_worldMapDebugFlags & WM_DEBUG_REQUEST) {
        printf("DEF SET NAVIMODE :%d\n", g_worldNaviMode);
    }
}

/* navi map markers */
void wm_naviDraw(void) {
    s32 x;
    s32 z;
    u8 page;

    if (g_worldTitleId >= 0) {
        wm_drawTitle();
    }
    if (g_worldNaviActive == 0) {
        return;
    }
    page = g_worldNaviPage;
    switch (g_worldNaviMode) {
    case 1:
        if (g_worldVehicleActor != NULL && g_worldVehicleActor->state == 1 && (g_worldVehicleActor->flags & 1) &&
            !((u8)g_worldMoveMode >= 7 && (u8)g_worldMoveMode < 10)) {
            CVECTOR c0 = g_worldNaviVehicleColorA;
            CVECTOR c1 = g_worldNaviVehicleColorB;
            wm_naviWorldToMap(g_worldVehicleActor->pos.vx, g_worldVehicleActor->pos.vz, &x, &z, page * 2);
            wm_naviDrawMarker(x, z, 1, 1, c0, c1, 0x40, 0xC8, 0x82, 1);
        }
        if (g_worldMountActor != NULL && g_worldMountActor->state == 1 && (g_worldMountActor->flags & 1) &&
            !((u8)g_worldMoveMode >= 1 && (u8)g_worldMoveMode < 7)) {
            CVECTOR c0 = g_worldNaviMountColorA;
            CVECTOR c2 = g_worldNaviMountColorB;
            wm_naviWorldToMap(g_worldMountActor->pos.vx, g_worldMountActor->pos.vz, &x, &z, page * 2);
            wm_naviDrawMarker(x, z, 1, 1, c0, c2, 0x40, 0xC8, 0x82, 1);
        }
        wm_naviDrawMiniMap();
        if (g_gameState->gameData->chocograph != 0) {
            func_800C16C0();
        }
        break;
    case 2:
        if (g_worldNaviVisible != 0) {
            if (g_worldVehicleActor != NULL && g_worldVehicleActor->state == 1 && (g_worldVehicleActor->flags & 1) &&
                !((u8)g_worldMoveMode >= 7 && (u8)g_worldMoveMode < 10)) {
                CVECTOR c0 = g_worldNaviVehicleColorA;
                CVECTOR c3 = g_worldNaviVehicleColorB;
                wm_naviWorldToMap(g_worldVehicleActor->pos.vx, g_worldVehicleActor->pos.vz, &x, &z, page * 2 + 1);
                wm_naviDrawMarker(x, z, 1, 1, c0, c3, 0x40, 0x28, 9, 1);
            }
            if (g_worldMountActor != NULL && g_worldMountActor->state == 1 && (g_worldMountActor->flags & 1) &&
                !((u8)g_worldMoveMode >= 1 && (u8)g_worldMoveMode < 7)) {
                CVECTOR c0 = g_worldNaviMountColorA;
                CVECTOR c1 = g_worldNaviMountColorB;
                wm_naviWorldToMap(g_worldMountActor->pos.vx, g_worldMountActor->pos.vz, &x, &z, page * 2 + 1);
                wm_naviDrawMarker(x, z, 1, 1, c0, c1, 0x40, 0x28, 9, 1);
            }
            if (g_worldNaviCursorDraw != 0) {
                msg_setOtz(5);
                pen_begin();
                pen_setPos((g_worldNaviCursorX >> 12) + 18, (g_worldNaviCursorY >> 12) + 6);
                pen_drawSprite(1);
                pen_endSmall();
                msg_setOtz(2);
            }
            wm_naviDrawPlaces();
        }
        wm_naviDrawFullMap();
        break;
    }
}

void wm_naviUpdate(void) {
    VECTOR stick;
    s32 pad;
    s32 dir;
    s32 ang;
    s32 d;
    s32 best;
    s32 bi;
    s32 i;
    s32 ex, ez;
    s32 fx, fz;
    s32 t;
    s32 e;
    s32 last;
    s32 lim;

    if (g_worldNaviActive != 0) {
        pad = 0;
        if (g_worldNaviMode == 2) {
            dir = 1;
            stick.vx = 0;
            stick.vy = 0;
            if (g_worldNaviPrevMode == 1) {
                wm_naviWorldToMap(g_worldPlayer->pos.vx, g_worldPlayer->pos.vz, &g_worldNaviCursorX, &g_worldNaviCursorY, g_worldNaviPage * 2 + 1);
                g_worldNaviCursorX <<= 12;
                g_worldNaviCursorY <<= 12;
            }
            if (g_worldNaviCursorOn != 0) {
                wm_getLeftStickX(&stick.vx);
                wm_getLeftStickY(&stick.vy);
                pad = wm_getPadPressed();
                if (g_worldNaviVisible == 0) {
                    pad = 0;
                    stick.vx = 0;
                    stick.vy = 0;
                }
            }
            stick.vy = -stick.vy;
            ang = (-ratan2(stick.vy, stick.vx) + 0x400) & 0xFFF;
            if (stick.vx | stick.vy) {
                t = SquareRoot0(stick.vx * stick.vx + stick.vy * stick.vy) << 5;
                d = t + (t >> 1);
            } else {
                d = 0;
            }
            if (d != 0) {
                g_worldNaviCursorX += math_sin(ang) * d >> 12;
                g_worldNaviCursorY -= math_cos(ang) * d >> 12;
                if (g_worldNaviCursorX < 0) {
                    g_worldNaviCursorX += 0xF0000;
                }
                if (g_worldNaviCursorY < 0) {
                    g_worldNaviCursorY += 0xC8000;
                }
                if (g_worldNaviCursorX > 0xF0000) {
                    g_worldNaviCursorX -= 0xF0000;
                }
                if (g_worldNaviCursorY > 0xC8000) {
                    g_worldNaviCursorY -= 0xC8000;
                }
            }
            if (pad & (BTN_L1 | BTN_R1)) {
                if (g_evtGlobalVars.b[0x67] == 0) {
                    best = 0x7FFFFFFF;
                    bi = -1;
                    for (i = 0; i < 64; i++) {
                        if ((u8)wm_naviIsPlaceKnown(i)) {
                            fx = ((g_worldNaviPlaces[g_worldNaviPage][i].mapX << 12) - g_worldNaviCursorX) >> 12;
                            fz = ((g_worldNaviPlaces[g_worldNaviPage][i].mapY << 12) - g_worldNaviCursorY) >> 12;
                            e = SquareRoot0(fx * fx + fz * fz);
                            if ((g_worldNaviPage != 1 || ((u32)(i - 6) >= 4 && i != 15)) && e < best) {
                                best = e;
                                bi = i;
                            }
                        }
                    }
                    if (bi != -1) {
                        dir = 0;
                        g_worldNaviPlace = bi;
                        g_worldNaviCursorX = g_worldNaviPlaces[g_worldNaviPage][g_worldNaviPlace].mapX << 12;
                        g_worldNaviCursorY = g_worldNaviPlaces[g_worldNaviPage][g_worldNaviPlace].mapY << 12;
                    }
                }
                if (g_evtGlobalVars.b[0x67] != 0 && dir != 0) {
                    if (pad & BTN_L1) {
                        dir = 1;
                    }
                    if (pad & BTN_R1) {
                        dir = -1;
                    }
                    do {
                        g_worldNaviPlace = (g_worldNaviPlace + dir) & 0x3F;
                        g_worldNaviPlaceCycled = 1;
                    } while (!(u8)wm_naviIsPlaceKnown(g_worldNaviPlace) || (g_worldNaviPlaces[g_worldNaviPage][g_worldNaviPlace].mapX == g_worldNaviPlaces[g_worldNaviPage][g_worldNaviPlace - dir].mapX &&
                             g_worldNaviPlaces[g_worldNaviPage][g_worldNaviPlace].mapY == g_worldNaviPlaces[g_worldNaviPage][g_worldNaviPlace - dir].mapY &&
                             (u8)wm_naviIsPlaceKnown(g_worldNaviPlace - dir)));
                    g_worldNaviCursorX = g_worldNaviPlaces[g_worldNaviPage][g_worldNaviPlace].mapX << 12;
                    g_worldNaviCursorY = g_worldNaviPlaces[g_worldNaviPage][g_worldNaviPlace].mapY << 12;
                }
            }
            last = -1;
            for (i = 0; i < 64; i++) {
                ex = (g_worldNaviPlaces[g_worldNaviPage][i].mapX << 12) - g_worldNaviCursorX;
                ez = (g_worldNaviPlaces[g_worldNaviPage][i].mapY << 12) - g_worldNaviCursorY;
                if (g_worldNaviPage != 1 || ((u32)(i - 6) >= 4 && i != 15)) {
                    lim = g_worldNaviSnapDist;
                    best = ex >= 0 ? ex : -ex;
                    if (best < lim) {
                        ang = ez >= 0 ? ez : -ez;
                        if (ang < lim && (u8)wm_naviIsPlaceKnown(i)) {
                            g_worldNaviCursorX += ex >> 3;
                            g_worldNaviCursorY += ez >> 3;
                            if (best < 0x1000 && ang < 0x1000) {
                                g_worldNaviPlace = i;
                                last = i;
                            }
                        }
                    }
                }
            }
            if (g_worldNaviPlaceCycled == 1) {
                g_worldNaviPlaceCycled = 0;
                g_evtGlobalVars.b[0x67] = 0;
            } else {
                g_evtGlobalVars.b[0x67] = last + 1;
            }
        }
    }
    if ((wm_getPadHeld() & BTN_CIRCLE) && g_worldNaviCursorDraw != 0) {
        g_worldNaviVisible = 0;
    } else {
        g_worldNaviVisible = 1;
    }
    g_worldNaviPrevMode = g_worldNaviMode;
}

void wm_naviDrawMarker(s32 arg0, s32 arg1, s32 arg2, s32 arg3, CVECTOR arg4, CVECTOR arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    CVECTOR c;
    u32 *ot;

    ot = (u32 *)g_worldOt[*g_worldBufIndex];
    c.r = wm_lerp(arg4.r, arg5.r, math_cos((g_worldFrameCount * arg6) & 0xFFF));
    c.g = wm_lerp(arg4.g, arg5.g, math_cos((g_worldFrameCount * arg6) & 0xFFF));
    c.b = wm_lerp(arg4.b, arg5.b, math_cos((g_worldFrameCount * arg6) & 0xFFF));
    if (arg9 == 0) {
        setXYWH((POLY_F4 *)g_worldPrimPtr, arg0 - arg2 + arg7, arg1 - arg3 + arg8, arg2 * 2 + 1, arg3 * 2 + 1);
        setRGB0((POLY_F4 *)g_worldPrimPtr, c.r, c.g, c.b);
        setPolyF4((POLY_F4 *)g_worldPrimPtr);
        addPrim(&ot[10], (POLY_F4 *)g_worldPrimPtr);
        g_worldPrimPtr += sizeof(POLY_F4);
    } else {
        setXYWH((POLY_FT4 *)g_worldPrimPtr, arg0 + arg7 - 1, arg1 + arg8 - 1, 4, 4);
        setUV4((POLY_FT4 *)g_worldPrimPtr, 0, 0xFA, 4, 0xFA, 0, 0xFE, 4, 0xFE);
        setRGB0((POLY_FT4 *)g_worldPrimPtr, c.r, c.g, c.b);
        setPolyFT4((POLY_FT4 *)g_worldPrimPtr);
        ((POLY_FT4 *)g_worldPrimPtr)->tpage = getTPage(0, 1, 704, 0);
        ((POLY_FT4 *)g_worldPrimPtr)->clut = getClut(704, 255);
        setSemiTrans((POLY_FT4 *)g_worldPrimPtr, 1);
        addPrim(&ot[10], (POLY_FT4 *)g_worldPrimPtr);
        g_worldPrimPtr += sizeof(POLY_FT4);
        setXYWH((POLY_FT4 *)g_worldPrimPtr, arg0 + arg7 - 1, arg1 + arg8 - 1, 4, 4);
        setUV4((POLY_FT4 *)g_worldPrimPtr, 0, 0xFA, 4, 0xFA, 0, 0xFE, 4, 0xFE);
        setRGB0((POLY_FT4 *)g_worldPrimPtr, 0x60, 0x60, 0x60);
        setPolyFT4((POLY_FT4 *)g_worldPrimPtr);
        ((POLY_FT4 *)g_worldPrimPtr)->tpage = getTPage(0, 2, 704, 0);
        ((POLY_FT4 *)g_worldPrimPtr)->clut = getClut(704, 255);
        setSemiTrans((POLY_FT4 *)g_worldPrimPtr, 1);
        addPrim(&ot[10], (POLY_FT4 *)g_worldPrimPtr);
        g_worldPrimPtr += sizeof(POLY_FT4);
    }
}

#define TILE(x0_, y0_, x1_, y1_, u0_, v0_, u1_, v1_, cl)                                   \
    setXY4((POLY_FT4 *)g_worldPrimPtr, x0_, y0_, x1_, y0_, x0_, y1_, x1_, y1_);              \
    setUV4((POLY_FT4 *)g_worldPrimPtr, u0_, v0_, u1_, v0_, u0_, v1_, u1_, v1_);              \
    setRGB0((POLY_FT4 *)g_worldPrimPtr, g_worldNaviTileR, g_worldNaviTileG, g_worldNaviTileB);                 \
    ((POLY_FT4 *)g_worldPrimPtr)->tpage = getTPage(0, 0, 640, 0);                                                \
    ((POLY_FT4 *)g_worldPrimPtr)->clut = cl;                                                  \
    setPolyFT4((POLY_FT4 *)g_worldPrimPtr);                                                   \
    addPrim(&ot[10], (POLY_FT4 *)g_worldPrimPtr);                                             \
    g_worldPrimPtr += sizeof(POLY_FT4);

void wm_naviDrawFullMap(void) {
    WmQuad arrow;
    VECTOR v[4];
    SVECTOR rot;
    SVECTOR rot0;
    MATRIX m;
    s32 x;
    s32 y;
    s32 *px;
    s32 *py;
    u32 *ot;
    s32 i;
    s32 c;
    POLY_FT4 *p;
    s32 a;
    s32 b;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;

    ot = (u32 *)g_worldOt[*g_worldBufIndex];
    arrow = g_worldNaviFullMapArrow;
    memset(&rot0, 0, 8);
    rot0.vz = g_worldPlayer->pos.rotY - 0x400;
    rot = rot0;
    math_rotMatrixXYZ(&rot, &m);
    for (i = 0; i < 4; i++) {
        ApplyMatrix(&m, &arrow.v[i], &v[i]);
    }
    px = &x;
    py = &y;
    wm_naviWorldToMap(g_worldPlayer->pos.vx, g_worldPlayer->pos.vz, px, py, g_worldNaviPage * 2 + 1);
    c = (math_sin(g_worldFrameCount << 6) >> 7) + 0x80;
    p = (POLY_FT4 *)g_worldPrimPtr;
    setRGB0(p, c, c, c);
    p->x0 = x + v[0].vx + 0x28;
    p->y0 = y + v[0].vy + 9;
    p->x1 = x + v[1].vx + 0x28;
    p->y1 = y + v[1].vy + 9;
    p->x2 = x + v[2].vx + 0x28;
    p->y2 = y + v[2].vy + 9;
    p->x3 = x + v[3].vx + 0x28;
    p->y3 = y + v[3].vy + 9;
    setUV4(p, 0x20, 0xD0, 0x40, 0xD0, 0x20, 0xD7, 0x40, 0xD7);
    p->tpage = GetTPage(0, 0, 0x300, 0xD0);
    p->clut = GetClut(0x280, 0xF5);
    setPolyFT4(p);
    if (g_worldNaviVisible) {
        addPrim(&ot[10], p);
    }
    p++;
    setRGB0(p, 0x80, 0x80, 0x80);
    p->x0 = x + v[0].vx + 0x28;
    p->y0 = y + v[0].vy + 9;
    p->x1 = x + v[1].vx + 0x28;
    p->y1 = y + v[1].vy + 9;
    p->x2 = x + v[2].vx + 0x28;
    p->y2 = y + v[2].vy + 9;
    p->x3 = x + v[3].vx + 0x28;
    p->y3 = y + v[3].vy + 9;
    setUV4(p, 0, 0xD0, 0x20, 0xD0, 0, 0xD7, 0x20, 0xD7);
    p->tpage = GetTPage(0, 0, 0x300, 0xD0);
    p->clut = GetClut(0x280, 0xF5);
    setPolyFT4(p);
    if (g_worldNaviVisible) {
        addPrim(&ot[10], p);
    }
    p++;
    g_worldPrimPtr = (s32)p;
    a = 0xF00 - g_worldCamera->yaw;
    b = 0x1100 - g_worldCamera->yaw;
    if (a < 0) {
        a += 0x1000;
    }
    if (b < 0) {
        b += 0x1000;
    }
    a %= 0x1000;
    b %= 0x1000;
    ax = math_sin(a) * 0x18 >> 12;
    ay = math_cos(a) * 0x18 >> 12;
    bx = math_sin(b) * 0x18 >> 12;
    by = math_cos(b) * 0x18 >> 12;
    wm_naviWorldToMap(g_worldPlayer->pos.vx, g_worldPlayer->pos.vz, px, py, g_worldNaviPage * 2 + 1);
    x1 = x + ax + 0x28;
    y1 = y + ay + 9;
    x2 = x + bx + 0x28;
    y2 = y + by + 9;
    g_worldNaviConePrim = (POLY_G3 *)g_worldPrimPtr;
    g_worldNaviConePrim->r0 = g_worldNaviConeR;
    g_worldNaviConePrim->g0 = g_worldNaviConeG;
    g_worldNaviConePrim->b0 = g_worldNaviConeB;
    g_worldNaviConePrim->r1 = 0;
    g_worldNaviConePrim->g1 = 0;
    g_worldNaviConePrim->b1 = 0;
    g_worldNaviConePrim->r2 = 0;
    g_worldNaviConePrim->g2 = 0;
    g_worldNaviConePrim->b2 = 0;
    g_worldNaviConePrim->x0 = x + 0x28;
    g_worldNaviConePrim->y0 = y + 9;
    g_worldNaviConePrim->x1 = x1;
    g_worldNaviConePrim->y1 = y1;
    g_worldNaviConePrim->x2 = x2;
    g_worldNaviConePrim->y2 = y2;
    setPolyG3(g_worldNaviConePrim);
    setSemiTrans(g_worldNaviConePrim, 1);
    if (g_worldNaviVisible) {
        addPrim(&ot[10], g_worldNaviConePrim);
    }
    g_worldNaviConePrim++;
    g_worldPrimPtr = (s32)g_worldNaviConePrim;
    setDrawTPage((DR_TPAGE *)g_worldPrimPtr, 0, 1, GetTPage(0, 1, 0, 0));
    addPrim(&ot[10], (DR_TPAGE *)g_worldPrimPtr);
    g_worldPrimPtr += sizeof(DR_TPAGE);
    TILE(0x20, 0xB5, 0x4E, 0xD9, 0, 0xB4, 0x2E, 0xD7, 0x3C6B)
    if (g_worldNaviPage == 0) {
        TILE(0x28, 9, 0x118, 0xD1, 8, 8, 0xF7, 0xCF, 0x3C6A)
    } else {
        TILE(0x28, 9, 0x93, 0x42, 8, 8, 0x73, 0x41, 0x3C6C)
        TILE(0x93, 9, 0x118, 0x5C, 0x73, 8, 0xF8, 0x5B, 0x3CEC)
        TILE(0x28, 0x42, 0x93, 0xD1, 8, 0x41, 0x73, 0xD0, 0x3CAC)
        TILE(0x93, 0x5C, 0x118, 0xD1, 0x73, 0x5B, 0xF8, 0xD0, 0x3C2C)
    }
    TILE(0x20, 1, 0x120, 0xD9, 0, 0, 0xFF, 0xD8, 0x3C6B)
    setXY4((POLY_F4 *)g_worldPrimPtr, 0, 0, 0x140, 0, 0, 0xDA, 0x140, 0xDA);
    setRGB0((POLY_F4 *)g_worldPrimPtr, 0, 0, 0);
    setPolyF4((POLY_F4 *)g_worldPrimPtr);
    addPrim(&ot[10], (POLY_F4 *)g_worldPrimPtr);
    g_worldPrimPtr += sizeof(POLY_F4);
}

void wm_naviDrawMiniMap(void) {
    WmQuad arrow;
    VECTOR v[4];
    SVECTOR rot;
    SVECTOR rot0;
    MATRIX m;
    s32 x;
    s32 y;
    s32 *px;
    s32 *py;
    u32 *ot;
    s32 i;
    s32 c;
    POLY_FT4 *p;
    s32 a;
    s32 b;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 x1;
    s32 y1;
    s32 x2;
    s32 y2;

    ot = (u32 *)g_worldOt[*g_worldBufIndex];
    arrow = g_worldNaviMiniMapArrow;
    memset(&rot0, 0, 8);
    rot0.vz = g_worldPlayer->pos.rotY - 0x400;
    rot = rot0;
    c = (math_sin(g_worldFrameCount << 6) >> 7) + 0x80;
    math_rotMatrixXYZ(&rot, &m);
    for (i = 0; i < 4; i++) {
        ApplyMatrix(&m, &arrow.v[i], &v[i]);
    }
    px = &x;
    py = &y;
    wm_naviWorldToMap(g_worldPlayer->pos.vx, g_worldPlayer->pos.vz, px, py, g_worldNaviPage * 2);
    p = (POLY_FT4 *)g_worldPrimPtr;
    setRGB0(p, c, c, c);
    p->x0 = x + v[0].vx + 200;
    p->y0 = y + v[0].vy + 130;
    p->x1 = x + v[1].vx + 200;
    p->y1 = y + v[1].vy + 130;
    p->x2 = x + v[2].vx + 200;
    p->y2 = y + v[2].vy + 130;
    p->x3 = x + v[3].vx + 200;
    p->y3 = y + v[3].vy + 130;
    setUV4(p, 0, 0xD0, 0x20, 0xD0, 0, 0xD7, 0x20, 0xD7);
    p->tpage = GetTPage(0, 0, 0x300, 0xD0);
    p->clut = GetClut(0x280, 0xF5);
    setPolyFT4(p);
    addPrim(&ot[10], p);
    p++;
    g_worldPrimPtr = (s32)p;
    a = 0xF00 - g_worldCamera->yaw;
    b = 0x1100 - g_worldCamera->yaw;
    if (a < 0) {
        a += 0x1000;
    }
    if (b < 0) {
        b += 0x1000;
    }
    a %= 0x1000;
    b %= 0x1000;
    ax = math_sin(a) * 12 >> 12;
    ay = math_cos(a) * 12 >> 12;
    bx = math_sin(b) * 12 >> 12;
    by = math_cos(b) * 12 >> 12;
    wm_naviWorldToMap(g_worldPlayer->pos.vx, g_worldPlayer->pos.vz, px, py, g_worldNaviPage * 2);
    g_worldNaviConePrim = (POLY_G3 *)g_worldPrimPtr;
    x1 = x + ax + 200;
    y1 = y + ay + 130;
    x2 = x + bx + 200;
    y2 = y + by + 130;
    g_worldNaviConePrim->r0 = 0x8E;
    g_worldNaviConePrim->g0 = 0xC8;
    g_worldNaviConePrim->b0 = 0;
    g_worldNaviConePrim->r1 = 0;
    g_worldNaviConePrim->g1 = 0;
    g_worldNaviConePrim->b1 = 0;
    g_worldNaviConePrim->r2 = 0;
    g_worldNaviConePrim->g2 = 0;
    g_worldNaviConePrim->b2 = 0;
    g_worldNaviConePrim->x0 = x + 200;
    g_worldNaviConePrim->y0 = y + 130;
    g_worldNaviConePrim->x1 = x1;
    g_worldNaviConePrim->y1 = y1;
    g_worldNaviConePrim->x2 = x2;
    g_worldNaviConePrim->y2 = y2;
    setPolyG3(g_worldNaviConePrim);
    setSemiTrans(g_worldNaviConePrim, 1);
    addPrim(&ot[10], g_worldNaviConePrim);
    g_worldNaviConePrim++;
    g_worldPrimPtr = (s32)g_worldNaviConePrim;
    setDrawTPage((DR_TPAGE *)g_worldPrimPtr, 0, 1, GetTPage(0, 1, 0, 0));
    addPrim(&ot[10], (DR_TPAGE *)g_worldPrimPtr);
    g_worldPrimPtr += sizeof(DR_TPAGE);
    setXY4((POLY_FT4 *)g_worldPrimPtr, 200, 130, 296, 130, 200, 210, 296, 210);
    setUV4((POLY_FT4 *)g_worldPrimPtr, 0, 0x80, 0x5F, 0x80, 0, 0xCF, 0x5F, 0xCF);
    setRGB0((POLY_FT4 *)g_worldPrimPtr, 0x80, 0x80, 0x80);
    ((POLY_FT4 *)g_worldPrimPtr)->tpage = getTPage(0, 0, 768, 0);
    ((POLY_FT4 *)g_worldPrimPtr)->clut = getClut(656, 241);
    setPolyFT4((POLY_FT4 *)g_worldPrimPtr);
    setSemiTrans((POLY_FT4 *)g_worldPrimPtr, 1);
    addPrim(&ot[10], (POLY_FT4 *)g_worldPrimPtr);
    g_worldPrimPtr += sizeof(POLY_FT4);
}

void wm_drawTitle(void) {
    u32 *ot;
    POLY_FT4 *p;
    u32 a;
    u32 b;

    ot = (u32 *)g_worldOt[*g_worldBufIndex];
    g_worldTitlePhase = 0;
    a = g_worldTitleDelay + 0x78;
    b = g_worldTitleDelay + 0xB4;
    if ((u32)g_worldTitleDelay < (u32)g_worldReadyFrameCount) {
        g_worldTitlePhase = 1;
    }
    if (a < (u32)g_worldReadyFrameCount) {
        g_worldTitlePhase = 2;
    }
    if (b < (u32)g_worldReadyFrameCount) {
        g_worldTitlePhase = 3;
    }
    if (b <= (u32)g_worldReadyFrameCount) {
        g_padPortMask[0] &= ~0x02080000;
    } else {
        g_padPortMask[0] |= 0x02080000;
    }
    if (g_worldTitlePhase == 1) {
        if (g_worldTitleFade == 0) {
            *g_worldStoryCounterPtr += 10;
        }
        if (g_worldTitleFade != 0x80) {
            g_worldTitleFade += 4;
        }
    }
    if (g_worldTitlePhase == 2) {
        if (g_worldTitleFade != 0) {
            g_worldTitleFade -= 4;
        }
    }
    if (g_worldTitlePhase == 3) {
        g_worldTitleId = -1;
    }
    g_worldTitlePrim = (POLY_FT4 *)g_worldPrimPtr;
    if (g_worldTitleFade != 0 && g_worldTitlePhase != 0) {
        setRGB0(g_worldTitlePrim, g_worldTitleFade, g_worldTitleFade, g_worldTitleFade);
        if (g_worldTitleWide == 0) {
            setXY4(g_worldTitlePrim, 0x60, 0x32, 0xDF, 0x32, 0x60, 0x71, 0xDF, 0x71);
            setUV4(g_worldTitlePrim, 0, 0x40, 0x7F, 0x40, 0, 0x7F, 0x7F, 0x7F);
        } else {
            setXY4(g_worldTitlePrim, 0x20, 0x32, 0x11F, 0x32, 0x20, 0x51, 0x11F, 0x51);
            setUV4(g_worldTitlePrim, 0, 0x40, 0xFF, 0x40, 0, 0x5F, 0xFF, 0x5F);
        }
        g_worldTitlePrim->tpage = getTPage(0, 1, 768, 0);
        g_worldTitlePrim->clut = getClut(640, 244);
        setPolyFT4(g_worldTitlePrim);
        setSemiTrans(g_worldTitlePrim, 1);
        addPrim(&ot[10], g_worldTitlePrim);
        g_worldTitlePrim++;
        setRGB0(g_worldTitlePrim, g_worldTitleFade, g_worldTitleFade, g_worldTitleFade);
        if (g_worldTitleWide == 0) {
            setXY4(g_worldTitlePrim, 0x60, 0x32, 0xDF, 0x32, 0x60, 0x71, 0xDF, 0x71);
            setUV4(g_worldTitlePrim, 0x80, 0x40, 0xFF, 0x40, 0x80, 0x7F, 0xFF, 0x7F);
        } else {
            setXY4(g_worldTitlePrim, 0x20, 0x32, 0x11F, 0x32, 0x20, 0x51, 0x11F, 0x51);
            setUV4(g_worldTitlePrim, 0, 0x60, 0xFF, 0x60, 0, 0x7F, 0xFF, 0x7F);
        }
        g_worldTitlePrim->tpage = getTPage(0, 2, 768, 0);
        g_worldTitlePrim->clut = getClut(640, 244);
        setPolyFT4(g_worldTitlePrim);
        setSemiTrans(g_worldTitlePrim, 1);
        p = g_worldTitlePrim;
        setaddr(p, getaddr(&ot[10]));
        g_worldTitlePrim = p + 1;
        setaddr(&ot[10], p);
    }
    g_worldPrimPtr = (s32)g_worldTitlePrim;
}

void wm_naviDrawPlaces(void) {
    CVECTOR c0;
    s32 i;
    u8 f;

    c0 = wm_naviDrawPlaces_color();
    {
        CVECTOR c1 = wm_naviDrawPlaces_color();

        for (i = 0; i < 64; i++) {
            if ((g_worldNaviPlaces[g_worldNaviPage][i].mapX | g_worldNaviPlaces[g_worldNaviPage][i].mapY) != 0) {
                f = 0;
                if (i >= 7 && (i < 11 || i == 15)) {
                    f = 1;
                }
                if ((g_worldNaviPage == 0 || !f) && (u8)wm_naviIsPlaceKnown(i)) {
                    wm_naviDrawMarker(g_worldNaviPlaces[g_worldNaviPage][i].mapX, g_worldNaviPlaces[g_worldNaviPage][i].mapY, 1, 1, c0, c1, 16, 40, 9, 1);
                }
            }
        }
    }
}

s32 wm_naviIsPlaceKnown(u32 arg0) {
    u32 bit = 0;
    u16 mask = 0;

    if (arg0 < 0x10) {
        mask = g_evtGlobalVars.h[0x2E];
        bit = arg0;
    }
    if (arg0 - 0x10 < 0x10) {
        mask = g_evtGlobalVars.h[0x2F];
        bit = arg0 - 0x10;
    }
    if (arg0 - 0x20 < 0x10) {
        mask = g_evtGlobalVars.h[0x30];
        bit = arg0 - 0x20;
    }
    if (arg0 - 0x30 < 0x10) {
        mask = g_evtGlobalVars.h[0x31];
        bit = arg0 - 0x30;
    }
    if ((g_worldNaviPlaces[g_worldNaviPage][arg0].mapX | g_worldNaviPlaces[g_worldNaviPage][arg0].mapY) == 0) {
        return 0;
    }
    return (mask >> bit) & 1;
}

void func_800C16C0(void) {
    u32 *ot;

    ot = (u32 *)g_worldOt[*g_worldBufIndex];
    ((POLY_FT4 *)g_worldPrimPtr)->y0 = 0x60;
    ((POLY_FT4 *)g_worldPrimPtr)->y1 = 0x60;
    ((POLY_FT4 *)g_worldPrimPtr)->x0 = 0xF9;
    ((POLY_FT4 *)g_worldPrimPtr)->x1 = 0x123;
    ((POLY_FT4 *)g_worldPrimPtr)->y2 = 0x7C;
    ((POLY_FT4 *)g_worldPrimPtr)->x2 = 0xF9;
    ((POLY_FT4 *)g_worldPrimPtr)->x3 = 0x123;
    ((POLY_FT4 *)g_worldPrimPtr)->y3 = 0x7C;
    ((POLY_FT4 *)g_worldPrimPtr)->u0 = 0x30;
    ((POLY_FT4 *)g_worldPrimPtr)->v0 = 0xB0;
    ((POLY_FT4 *)g_worldPrimPtr)->u1 = 0x5A;
    ((POLY_FT4 *)g_worldPrimPtr)->v1 = 0xB0;
    ((POLY_FT4 *)g_worldPrimPtr)->u2 = 0x30;
    ((POLY_FT4 *)g_worldPrimPtr)->v2 = 0xCC;
    ((POLY_FT4 *)g_worldPrimPtr)->u3 = 0x5A;
    ((POLY_FT4 *)g_worldPrimPtr)->v3 = 0xCC;
    ((POLY_FT4 *)g_worldPrimPtr)->r0 = 0x80;
    ((POLY_FT4 *)g_worldPrimPtr)->g0 = 0x80;
    ((POLY_FT4 *)g_worldPrimPtr)->b0 = 0x80;
    ((POLY_FT4 *)g_worldPrimPtr)->tpage = getTPage(1, 0, 768, 0);
    ((POLY_FT4 *)g_worldPrimPtr)->clut = getClut(640, 246);
    setPolyFT4((POLY_FT4 *)g_worldPrimPtr);
    addPrim(&ot[10], g_worldPrimPtr);
    g_worldPrimPtr += sizeof(POLY_FT4);
    wm_naviDrawFrame(0xF5, 0x5C, 9, 0);
}

/* draw the four compass frame pieces at (arg0, arg1) */
void wm_naviDrawFrame(s32 arg0, s32 arg1) {
    POLY_FT4 *p;
    s16 clut;
    u32 *ot;
    s32 i;

    p = (POLY_FT4 *)g_worldPrimPtr;
    clut = GetClut(0x280, 0xF0);
    ot = (u32 *)g_worldOt[*g_worldBufIndex];
    for (i = 0; i < 4; i++) {
        SetPolyFT4(p);
        if (i < 2) {
            setXYWH(p, g_worldNaviFrameParts[i].x + arg0, g_worldNaviFrameParts[i].y + arg1, g_worldNaviFrameParts[i].w,
                    g_worldNaviFrameParts[i].h);
        } else {
            /* rotated a quarter turn */
            setXY4(p, g_worldNaviFrameParts[i].x + arg0, g_worldNaviFrameParts[i].y + arg1 + g_worldNaviFrameParts[i].w,
                   g_worldNaviFrameParts[i].x + arg0, arg1 + g_worldNaviFrameParts[i].y - (i != 2),
                   g_worldNaviFrameParts[i].x + arg0 + g_worldNaviFrameParts[i].h,
                   g_worldNaviFrameParts[i].y + arg1 + g_worldNaviFrameParts[i].w,
                   g_worldNaviFrameParts[i].x + arg0 + g_worldNaviFrameParts[i].h, arg1 + g_worldNaviFrameParts[i].y - (i != 2));
        }
        setUVWH(p, g_worldNaviFrameParts[i].u, g_worldNaviFrameParts[i].v, g_worldNaviFrameParts[i].w, g_worldNaviFrameParts[i].h);
        setRGB0(p, 0x80, 0x80, 0x80);
        p->tpage = getTPage(0, 0, 768, 0);
        p->clut = clut;
        addPrim(&ot[10], p);
        p++;
    }
    g_worldPrimPtr = (s32)p;
}

void wm_naviWorldToMap(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, u8 arg4) {
    switch (arg4) {
    case 0:
        *arg2 = ((arg0 - 0x30000) * 14 / 16 >> 11) + 15;
        *arg3 = ((-0x22000 - arg1) * 14 / 16 >> 11) + 6;
        break;
    case 1:
        *arg2 = (arg0 - 0x30000) / 819 * 14 / 16 + 31;
        *arg3 = (-0x22000 - arg1) / 819 * 14 / 16 + 12;
        break;
    case 2:
        *arg2 = arg0 >> 12;
        *arg3 = -arg1 >> 12;
        break;
    case 3:
        *arg2 = arg0 / 1638;
        *arg3 = -arg1 / 1638;
        break;
    }
}

/* initialise the frame pack slot list */
void wm_readtxInit(void) {
    u32 i;

    g_worldReadtxSlots[0].prev = &g_worldReadtxHead;
    g_worldReadtxSlots[0].next = &g_worldReadtxSlots[1];
    g_worldReadtxSlots[0].extracted = 0;
    g_worldReadtxSlots[0].texId = -1;
    g_worldReadtxSlots[0].pack = g_worldTexBuffers[0];
    for (i = 1; i < 5; i++) {
        g_worldReadtxSlots[i].prev = &g_worldReadtxSlots[i - 1];
        g_worldReadtxSlots[i].next = &g_worldReadtxSlots[i + 1];
        g_worldReadtxSlots[i].pack = NULL;
        g_worldReadtxSlots[i].extracted = 0;
        g_worldReadtxSlots[i].texId = -1;
    }
    for (i = 1; i < 3; i++) {
        g_worldReadtxSlots[i].pack = g_worldTexBuffers[i];
    }
    i = 5;
    g_worldReadtxSlots[i].prev = &g_worldReadtxSlots[i - 1];
    g_worldReadtxSlots[i].next = &g_worldReadtxTail;
    g_worldReadtxSlots[i].texId = -1;
    g_worldReadtxSlots[i].pack = NULL;
    g_worldReadtxSlots[i].extracted = 0;
    g_worldReadtxHead.prev = NULL;
    g_worldReadtxHead.next = &g_worldReadtxSlots[0];
    g_worldReadtxTail.next = &g_worldReadtxSlots[3];
    g_worldReadtxReqCount = 0;
    g_worldReadtxTail.prev = NULL;
    g_worldReadtxDirty = 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", g_worldNaviVehicleColorA);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", g_worldNaviVehicleColorB);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", g_worldNaviMountColorA);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", g_worldNaviMountColorB);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", g_worldNaviFullMapArrow);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", g_worldNaviMiniMapArrow);
