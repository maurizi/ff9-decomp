#include "ovl_0c.h"

void wm_relocatePack(WmPack *pack) {
    s32 i;
    WmPackEntry *entry;

    i = 0;
    pack->unk4 += (s32)pack;
    for (; i < pack->count; i++) {
        entry = (WmPackEntry *)((s32)pack->entries[i] + (s32)pack);
        pack->entries[i] = entry;
        entry->verts += (s32)pack;
        entry->polys += (s32)pack;
    }
}

void wm_setupWorldData(void) {
    s32 unused[26];
    void **files[6]; /* files[6] below overruns into the saved s0 slot, as in the original */
    void *formTex;
    void **formPack;
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
        g_worldTexAnimPack = db_lookupRegistered(DB_GET_ADDR, 0x411, 0);
        g_worldDataPack = db_lookupRegistered(DB_GET_ADDR, 0x41B, 0);
        g_worldChunkTable = db_lookupRegistered(DB_GET_ADDR, 0x425, 0);
        g_worldChunkTableT = db_lookupRegistered(DB_GET_ADDR, 0x42F, 0);
        files[4] = db_lookupRegistered(DB_GET_ADDR, 0x439, 0);
        files[5] = db_lookupRegistered(DB_GET_ADDR, 0x443, 0);
    } else {
        g_worldTexAnimPack = db_lookupRegistered(DB_GET_ADDR, 0x414, 0);
        g_worldDataPack = db_lookupRegistered(DB_GET_ADDR, 0x41E, 0);
        g_worldChunkTable = db_lookupRegistered(DB_GET_ADDR, 0x428, 0);
        g_worldChunkTableT = db_lookupRegistered(DB_GET_ADDR, 0x432, 0);
        files[4] = db_lookupRegistered(DB_GET_ADDR, 0x43C, 0);
        files[5] = db_lookupRegistered(DB_GET_ADDR, 0x446, 0);
    }
    files[0] = db_lookupRegistered(DB_GET_ADDR, 0x5DC, 0);
    files[1] = db_lookupRegistered(DB_GET_ADDR, 0x5DD, 0);
    files[2] = db_lookupRegistered(DB_GET_ADDR, 0x5DE, 0);
    files[3] = db_lookupRegistered(DB_GET_ADDR, 0x5DF, 0);
    files[6] = db_lookupRegistered(DB_GET_ADDR, 0x5E0, 0);
    w = g_worldChunkTable->width;
    h = g_worldChunkTable->height;
    g_worldChunksW = w;
    g_worldChunksH = h;
    g_worldCellsW = w * 2;
    g_worldCellsH = h * 2;
    g_worldWidth = w << 14;
    g_worldDepth = h << 14;
    *(WmCellMap *)g_worldChunkFlags = *(WmCellMap *)files[0];
    wm_applyStoryForms();
    w_frameExtruct(g_worldTexAnimPack);
    w_frameExtruct((s32 *)g_worldDataPack);
    w_frameExtruct((s32 *)files[1]);
    w_frameExtruct((s32 *)files[2]);
    w_frameExtruct((s32 *)files[3]);
    w_frameExtruct((s32 *)files[4]);
    wm_loadImagePack(files[6]);
    formPack = files[5];
    w_frameExtruct((s32 *)formPack);
    for (i = 0; i < 16; i++) {
        formTex = w_framePackGetPtr(formPack, i + g_worldFormFlags[i] * 16);
        w_frameExtruct(formTex);
        wm_loadTimPack(formTex, 0, 0, 0, 0);
    }
    wm_loadTimPack((s32 *)files[4], 0, 0, 0, 0);
    wm_loadTimPack((s32 *)files[1], 0, 0, 0, 0);
    if (g_gameState->gameData->chocograph != 0) {
        wm_loadTim((u32 *)files[3], g_gameState->gameData->chocograph - 1, 0x318, 0xB0, 0x280, 0xF6, 2);
    }
    if (*g_worldStoryCounterPtr >= 0x1766) {
        g_worldNaviPage = 1;
    } else {
        g_worldNaviPage = 0;
    }
    wm_loadTim((u32 *)files[2], g_worldNaviPage * 2, 0, 0, 0, 0, 0);
    wm_loadTim((u32 *)files[2], (g_worldNaviPage << 1) | 1, 0, 0, 0, 0, 1);
    wm_loadTim((u32 *)files[2], 4, 0, 0, 0, 0, 0);
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
            wm_loadTim((u32 *)files[2], g_worldTitleId * 2 + 5, 0x300, 0x40, 0x280, 0xF4, 0);
            wm_loadTim((u32 *)files[2], g_worldTitleId * 2 + 6, 0x320, 0x40, 0x290, 0xF4, 0);
        } else {
            wm_loadTim((u32 *)files[2], g_worldTitleId * 2 + 5, 0x300, 0x40, 0x280, 0xF4, 0);
            wm_loadTim((u32 *)files[2], g_worldTitleId * 2 + 6, 0x300, 0x60, 0x290, 0xF4, 0);
        }
    }
    for (i = 0x35; i < 0x43; i++) {
        wm_relocatePack(w_framePackGetPtr(g_worldDataPack, i));
    }
    g_worldDefaultBlocks = w_framePackGetPtr(g_worldDataPack, 0x42);
    g_worldAreaNames = w_framePackGetPtr(g_worldDataPack, 0);
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
    s32 enabledBits;
    WmEncounter *tbl;
    u16 encNo;

    i = 0;
    tbl = g_worldEncounterTable;
    row = g_worldEncounterEventRows;
    enabledBits = g_evtGlobalVars.b[0xC2] | (g_evtGlobalVars.b[0xC6] << 8);
    do {
        if ((enabledBits >> i) & 1) {
            j = 0;
            do {
                encNo = (*row)[j];
                if (encNo != 0xFFFF) {
                    tbl[encNo - 1].battle[3] = tbl[encNo - 1].battle[2];
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
    WmPack *pack;
    MATRIX *backdropMat;

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
        pack = w_framePackGetPtr(g_worldDataPack, 0x3B);
        m2.t[0] = 0;
        m2.t[1] = 0;
        m2.t[2] = 0;
        backdropMat = &out;
        CompMatrix(&m1, &m2, backdropMat);
        SetRotMatrix(backdropMat);
        SetTransMatrix(backdropMat);
        wm_drawBackdrop((s32)pack->entries[0], 0);
    }
}

void wm_updateMap(void) {
    s32 playerType;
    s32 flags;
    s32 i;
    s32 dx;
    s32 dz;
    s32 shake;
    s32 h;
    s32 dist;

    playerType = g_worldPlayer->actorType;
    flags = g_worldActorTypeInfo[playerType].cellAttr;
    if (g_worldMusicStarted != 0) {
        wm_updateChunks();
        wm_updateVisibleBlocks();
    }
    for (i = 0; i < 3; i++) {
        dx = (g_worldPlayer->pos.vx >> 8) - (g_worldAmbientSeX[i] >> 8);
        dz = (g_worldPlayer->pos.vz >> 8) - (g_worldAmbientSeZ[i] >> 8);
        g_worldAmbientSeDist[i] = SquareRoot0(dx * dx + dz * dz);
        if (g_worldFormFlags[WM_FORM_CLAY] != 0 && i == 0) {
            g_worldAmbientSeDist[0] = 100;
        }
        dist = g_worldAmbientSeDist[i];
        if (i == 0 && g_worldDataSet == WM_DATASET_DISC4) {
            continue;
        }
        if (dist < 0x3F) {
            if (g_worldAmbientSeOn[i] == 0) {
                g_worldAmbientSeOn[i] = 1;
                wm_sePlay(g_worldAmbientSe[i], 0);
            } else {
                dz = 0x7F - dist * 2;
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
        shake = 0x78 - g_worldAmbientSeDist[2] * 3;
        if (shake != 0 && g_worldMapLoaded != 0) {
            g_worldCameraTarget[1] += rand() % shake - shake / 2;
            g_worldCameraTarget[0] += rand() % shake - shake / 2;
            g_worldCameraTarget[2] += rand() % shake - shake / 2;
        }
    }
    if (g_worldAmbientSeDist[1] < 0x22 && evt_isControlEnabled() != 0 && WM_ATTR_TOPOGRAPHY(flags) == 0x17 && playerType - 1 < 7U) {
        g_worldPlayer->pos.vz += (0x22 - g_worldAmbientSeDist[1]) * 8;
        if (g_worldPlayer->pos.vx == g_worldPlayer->prevPos.vx && g_worldPlayer->pos.vz == g_worldPlayer->prevPos.vz) {
            g_worldPlayer->prevPos.vx++;
        }
    }
    if (g_evtGlobalVars.b[EVT_GB_WM_FILE] == 8 || g_evtGlobalVars.b[EVT_GB_WM_FILE] == 0) {
        dx = (g_worldPlayer->pos.vx >> 3) - 0x790A;
        dz = (g_worldPlayer->pos.vz >> 3) + 0x5ACB;
        dist = SquareRoot0(dx * dx + ((g_worldPlayer->pos.vy >> 3) + 0x66) * ((g_worldPlayer->pos.vy >> 3) + 0x66) * 8 + dz * dz);
        if (dist < 0x47C) {
            h = 0x47C - dist;
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
    s32 form;
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
        for (form = 0; form < 7; form++) {
            switch (g_worldFormStatusTable[form][i]) {
            case WM_FORM_STATUS(WM_FORM_SGATE):
                WM_CELL(18, 14) |= WM_CELL_FORM;
                WM_CELL(17, 12) |= WM_CELL_FORM;
                g_worldFormFlags[WM_FORM_SGATE] = 1;
                break;
            case WM_FORM_STATUS(WM_FORM_CLAY):
                WM_CELL(13, 12) |= WM_CELL_FORM;
                WM_CELL(14, 12) |= WM_CELL_FORM;
                g_worldFormFlags[WM_FORM_CLAY] = 1;
                break;
            case WM_FORM_STATUS(WM_FORM_LIND):
                WM_CELL(13, 16) |= WM_CELL_FORM;
                WM_CELL(13, 17) |= WM_CELL_FORM;
                WM_CELL(14, 16) |= WM_CELL_FORM;
                WM_CELL(14, 17) |= WM_CELL_FORM;
                g_worldFormFlags[WM_FORM_LIND] = 1;
                break;
            case WM_FORM_STATUS(WM_FORM_KUROMA):
                WM_CELL(14, 6) |= WM_CELL_FORM;
                WM_CELL(21, 10) |= WM_CELL_FORM;
                WM_CELL(22, 14) |= WM_CELL_FORM;
                g_worldFormFlags[WM_FORM_KUROMA] = 1;
                break;
            case WM_FORM_STATUS(WM_FORM_ALEX):
                WM_CELL(19, 10) |= WM_CELL_FORM;
                WM_CELL(19, 11) |= WM_CELL_FORM;
                WM_CELL(20, 10) |= WM_CELL_FORM;
                WM_CELL(20, 11) |= WM_CELL_FORM;
                g_worldFormFlags[WM_FORM_ALEX] = 1;
                break;
            case WM_FORM_STATUS(WM_FORM_HOKORA_MIZU):
                WM_CELL(3, 9) |= WM_CELL_FORM;
                WM_CELL(9, 1) |= WM_CELL_FORM;
                g_worldFormFlags[WM_FORM_HOKORA_MIZU] = 1;
                break;
            case WM_FORM_STATUS(WM_FORM_HOKORA_HI):
                WM_CELL(7, 1) |= WM_CELL_FORM;
                WM_CELL(8, 1) |= WM_CELL_FORM;
                WM_CELL(14, 15) |= WM_CELL_FORM;
                g_worldFormFlags[WM_FORM_HOKORA_HI] = 1;
                break;
            }
            /* the original second jump table is 8-aligned */
            __asm__(".rdata\n\t.word 0\n\t.text");
            /* the debug print switches on the status value but compares it with the form indices */
            if (g_worldMapDebugFlags & WM_DEBUG_FORM) {
                switch (g_worldFormStatusTable[form][i]) {
                case WM_FORM_SGATE:
                    printf("BGI Form : SGATE\n");
                    break;
                case WM_FORM_CLAY:
                    printf("BGI Form : CLAY\n");
                    break;
                case WM_FORM_LIND:
                    printf("BGI Form : LIND\n");
                    break;
                case WM_FORM_KUROMA:
                    printf("BGI Form : KUROMA\n");
                    break;
                case WM_FORM_ALEX:
                    printf("BGI Form : ALEX\n");
                    break;
                case WM_FORM_HOKORA_MIZU:
                    printf("BGI Form : HOKORA(MIZU)\n");
                    break;
                case WM_FORM_HOKORA_HI:
                    printf("BGI Form : HOKORA(HI)\n");
                    break;
                }
            }
        }
    }
    printf("General Global Flag = %x\n", g_evtGeneralFlags);
    if (g_evtGeneralFlags & EVT_GB_GENERAL_MOGNET_FORM) {
        printf("- Replace Mog Net -\n");
        WM_CELL(16, 1) |= WM_CELL_FORM;
        WM_CELL(13, 4) |= WM_CELL_FORM;
        WM_CELL(14, 5) |= WM_CELL_FORM;
        g_worldFormFlags[WM_FORM_MOG_NET] = 1;
    }
    if (g_evtGeneralFlags & EVT_GB_GENERAL_CHOCOBO_FORM) {
        printf("- Replace Chocobo Heaven -\n");
        WM_CELL(0, 0) |= WM_CELL_FORM;
        WM_CELL(16, 14) |= WM_CELL_FORM;
        WM_CELL(9, 17) |= WM_CELL_FORM;
        g_worldFormFlags[WM_FORM_CHOCOBO_HEAVEN] = 1;
    }
}

/* pick the encounter table entry for the player's area, zone and fog */
WmEncounter *wm_getEncounterEntry(void) {
    u8 zone;
    s32 area;
    s32 lo;
    s32 hi;
    s32 i;
    u32 topo;
    s32 fog;
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
        topo = g_worldEncounterTable[i].topoPattern >> 2;
        fog = g_worldEncounterTable[i].zoneFog & 1;
        if (g_worldMapDebugFlags & WM_DEBUG_ENCOUNTER) {
            printf("Matching:%d  ( T:%d / F:%d )\n", i, topo, fog);
        }
        if (topo == (u8)g_worldActorTypeInfo[g_worldPlayer->actorType].cellAttr >> 2 && fog == g_worldFog) {
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
    printf(s_wmStrEncounterNoScene);
    return &g_worldEncounterTable[i + add - 1];
}

s32 wm_worldToView(VECTOR *pos, VECTOR *outRel, VECTOR *outView, s32 *outScale) {
    VECTOR camRel;
    s32 scale;

    camRel.vx = pos->vx - g_worldCameraEye[0];
    camRel.vy = pos->vy - g_worldCameraEye[1];
    camRel.vz = pos->vz - g_worldCameraEye[2];
    if (camRel.vx > g_worldWidth - 0x8000) {
        camRel.vx -= g_worldWidth;
    }
    if (camRel.vz > g_worldDepth - 0x8000) {
        camRel.vz -= g_worldDepth;
    }
    if (camRel.vx < 0x8000 - g_worldWidth) {
        camRel.vx = g_worldWidth + camRel.vx;
    }
    if (camRel.vz < 0x8000 - g_worldDepth) {
        camRel.vz = g_worldDepth + camRel.vz;
    }
    ApplyMatrixLV(g_worldViewMatrix, &camRel, outView);
    scale = 0x1000 - ((outView->vz >> 4) + (outView->vz >> 6));
    camRel.vy = ((pos->vy * scale) >> 12) - g_worldCameraEye[1];
    ApplyMatrixLV(g_worldViewMatrix, &camRel, outView);
    outRel->vx = pos->vx - g_worldCameraTarget[0];
    outRel->vy = ((pos->vy * scale) >> 12) - g_worldCameraTarget[1];
    outRel->vz = pos->vz - g_worldCameraTarget[2];
    if (outRel->vx > 0xFFFF) {
        outRel->vx -= 0x60000;
    }
    if (outRel->vz > 0xFFFF) {
        outRel->vz -= 0x50000;
    }
    if (outRel->vx < -0xFFFF) {
        outRel->vx += 0x60000;
    }
    if (outRel->vz < -0xFFFF) {
        outRel->vz += 0x50000;
    }
    if (outScale != NULL) {
        *outScale = scale;
    }
    if (outRel->vx < -0x8000 || outRel->vx > 0x7FFF || outRel->vz > 0x7FFF || outRel->vz < -0x8000) {
        return 0;
    }
    return 1;
}

void wm_mapTerm(void) {
}

void wm_mapSystemTerm(void) {
}

void wm_wrapPos(s32 *x, s32 *z) {
    if (*x < 0) {
        *x += g_worldWidth;
    }
    if (*x >= g_worldWidth) {
        *x -= g_worldWidth;
    }
    if (*z > 0) {
        *z -= g_worldDepth;
    }
    if (*z <= -g_worldDepth) {
        *z += g_worldDepth;
    }
}

void wm_posToCell(s32 posX, s32 posZ, s32 *outCell) {
    s32 x;
    s32 z;

    wm_wrapPos_inline(&posX, &posZ);
    x = posX / 0x2000;
    z = posZ / 0x2000;
    *outCell = -z * g_worldCellsW + x;
}

void wm_posToChunk(s32 posX, s32 posZ, s32 *outChunk) {
    s32 x;
    s32 z;

    wm_wrapPos_inline(&posX, &posZ);
    x = posX / 0x4000;
    z = posZ / 0x4000;
    *outChunk = -z * g_worldChunksW + x;
}

void wm_cellToPos(s32 cell, s32 *outX, s32 *outZ) {
    *outX = (cell % g_worldCellsW) << 13;
    *outZ = -((cell / g_worldCellsW) << 13);
}

void wm_cellToChunk(s32 cell, s32 *outChunk) {
    *outChunk = cell / g_worldCellsW / 2 * g_worldChunksW + cell % g_worldCellsW / 2;
}

void wm_chunkToPos(s32 chunk, s32 *outX, s32 *outZ) {
    *outX = (chunk % g_worldChunksW) << 14;
    *outZ = -((chunk / g_worldChunksW) << 14);
}

void wm_getChunkEntry(s32 chunk, s32 variant, WmChunkEntry *out) {
    WmChunkEntry *cells;

    cells = g_worldChunkTable->entries;
    out->sector = cells[chunk * 2 + variant].sector;
    out->packSectors = cells[chunk * 2 + variant].packSectors;
    out->texSectors = cells[chunk * 2 + variant].texSectors;
}

void wm_getChunkEntryT(s32 chunk, s32 variant, WmChunkEntry *out) {
    s32 k;
    WmChunkEntry *cells;

    k = chunk % g_worldChunksW * g_worldChunksH + chunk / g_worldChunksW;
    cells = g_worldChunkTableT->entries;
    out->sector = cells[k * 2 + variant].sector;
    out->packSectors = cells[k * 2 + variant].packSectors;
    out->texSectors = cells[k * 2 + variant].texSectors;
}

s32 wm_getAreaZone(s32 area) {
    return g_worldAreaZone[area];
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
    s32 speed;
    s32 cell;
    s32 chunk;

    if (g_worldMoveMode < WM_MOVE_FAST_WALK) {
        if (g_worldMoveMode >= WM_MOVE_HILDA_GARDE) {
            speed = g_worldPlayerSpeed;
            if (speed >= 0x200) {
                speed = 0x1FF;
            }
            if (speed < 0x100) {
                speed = 0x100;
            }
            g_worldChunkLookPos[0] = g_worldCameraEye[0] + ((g_worldCameraDir[0] * g_worldMoveModeInfo->lookAhead * speed) >> 9);
            g_worldChunkLookPos[2] = g_worldCameraEye[2] + ((g_worldCameraDir[2] * g_worldMoveModeInfo->lookAhead * speed) >> 9);
        } else {
            g_worldChunkLookPos[0] = g_worldCameraEye[0] + g_worldCameraDir[0] * g_worldMoveModeInfo->lookAhead;
            g_worldChunkLookPos[2] = g_worldCameraEye[2] + g_worldCameraDir[2] * g_worldMoveModeInfo->lookAhead;
        }
    } else {
        g_worldChunkLookPos[0] = g_worldCameraEye[0] + g_worldCameraDir[0] * g_worldMoveModeInfo->lookAhead;
        g_worldChunkLookPos[2] = g_worldCameraEye[2] + g_worldCameraDir[2] * g_worldMoveModeInfo->lookAhead;
    }
    wm_wrapPos(&g_worldChunkLookPos[0], &g_worldChunkLookPos[2]);
    wm_posToCell(g_worldChunkLookPos[0] - 0x6000, g_worldChunkLookPos[2] + 0x6000, &cell);
    wm_cellToChunk(cell, &chunk);
    if (chunk != g_worldChunkCenter) {
        wm_requestChunks(chunk);
    }
    wm_readChunks();
    g_worldChunkCenter = chunk;
}

void wm_requestChunks(s32 chunk) {
    s32 missing[16];
    s32 x;
    s32 z;
    s32 id;
    s32 missCount;
    s32 i;
    s32 j;
    s32 k;
    WmChunkSlot *slot;
    WmChunkSlot *h;
    u8 chunkFlags;

    missCount = 0;
    h = &g_worldChunkHead;
    wm_chunkToPos(chunk, &x, &z);
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            wm_posToChunk(x + (i << 14), z - (j << 14), &id);
            slot = h->next;
            for (k = 0; k < 16; k++, slot = slot->next) {
                if (slot->chunkId == id) {
                    chunkFlags = g_worldChunkFlags[slot->chunkId];
                    if (chunkFlags & WM_CELL_TEX_MASK) {
                        w_readtxRequest(chunkFlags);
                    }
                    slot->prev->next = slot->next;
                    slot->next->prev = slot->prev;
                    slot->next = h->next;
                    slot->prev = h;
                    h->next->prev = slot;
                    h->next = slot;
                    break;
                }
            }
            if (k == 16) {
                missing[missCount++] = id;
            }
        }
    }
    for (k = 0; k < missCount; k++) {
        if (k == 4) {
            g_worldChunkGeneration++;
    g_worldChunkGeneration %= 4;
        }
        wm_chunkRequest(missing[k]);
        chunkFlags = g_worldChunkFlags[missing[k]];
        if (chunkFlags & WM_CELL_TEX_MASK) {
            w_readtxRequest(chunkFlags);
        }
    }
    wm_chunkListCompact();
    wm_readtxCompact();
    g_worldChunkGeneration++;
    g_worldChunkGeneration %= 4;
}

/* request texture pack chunkId: reuse a free or matching slot and move it behind the loaded ones */
s32 wm_chunkRequest(s16 chunkId) {
    WmChunkSlot *prev;
    WmChunkSlot *slot;
    u32 slotIdx;

    slotIdx = 0;
    prev = &g_worldChunkHead;
    while (prev->next->state == WM_CHUNK_READING) {
        if (prev->next->chunkId == chunkId) {
            return 0;
        }
        prev = prev->next;
        slotIdx++;
    }
    slot = prev->next;
    do {
        if (slot->chunkId == -1) {
            if (slotIdx >= 16) {
                slot->chunkId = chunkId;
                slot->prev->next = slot->next;
                slot->next->prev = slot->prev;
                slot->next = prev->next;
                slot->prev = prev;
                g_worldChunkListDirty++;
                prev->next->prev = slot;
                prev->next = slot;
            } else {
                slot->chunkId = chunkId;
                slot->state = WM_CHUNK_WANTED;
            }
            break;
        }
        if (slot->chunkId == chunkId) {
            slot->prev->next = slot->next;
            slot->next->prev = slot->prev;
            slot->next = prev->next;
            slot->prev = prev;
            prev->next->prev = slot;
            prev->next = slot;
            break;
        }
        slot = slot->next;
        slotIdx++;
    } while (slot != &g_worldChunkTail);
    if (slot != &g_worldChunkTail) {
        slot->generation = g_worldChunkGeneration;
    }
    return slotIdx != 32;
}

void wm_readChunks(void) {
    WmChunkEntry baseEntry;
    WmChunkEntry formEntry;
    WmChunkSlot *slot;
    u8 *cell;
    s32 id;
    s16 id2;
    s32 i;
    s32 texBuf;
    s32 base;
    WmChunkSlot *h;

    if (g_worldCdUser == WM_CD_FREE) {
        wm_findChunkRun(g_worldChunkRun, &g_worldChunkRunCount[g_worldChunkRunSlot], &g_worldChunkRunDir[g_worldChunkRunSlot]);
        if (g_worldChunkRunDir[g_worldChunkRunSlot] == 0) {
            goto end;
        }
        for (i = 0; i < g_worldChunkRunCount[g_worldChunkRunSlot]; i++) {
            slot = g_worldChunkRun[i];
            h = &g_worldChunkHead;
            id2 = slot->chunkId;
            id = slot->chunkId;
            slot->prev->next = slot->next;
            slot->next->prev = slot->prev;
            slot->next = h->next;
            slot->prev = &g_worldChunkHead;
            cell = &g_worldChunkFlags[id];
            h->next->prev = slot;
            h->next = slot;
            slot->state = WM_CHUNK_READING;
            if (g_worldChunkRunDir[g_worldChunkRunSlot] == 1) {
                wm_getChunkEntry(id, 0, &baseEntry);
                wm_getChunkEntry(id, 1, &formEntry);
            } else {
                wm_getChunkEntryT(id, 0, &baseEntry);
                wm_getChunkEntryT(id, 1, &formEntry);
            }
            if ((*cell & WM_CELL_FORM) && formEntry.packSectors == 0) {
                if (g_worldMapDebugFlags & WM_DEBUG_FORM) {
                    printf(s_wmStrBlockNoForm, id2);
                }
                *cell &= (u8)~WM_CELL_FORM;
            }
            texBuf = (*cell & WM_CELL_TEX_MASK) ? wm_readtxFind(*cell, 0) : 0;
            slot->hasData = 1;
            if (!(*cell & WM_CELL_FORM)) {
                g_worldChunkReadReqs[i * 4 + 0].size = baseEntry.packSectors << 11;
                g_worldChunkReadReqs[i * 4 + 0].dest = slot->pack;
                g_worldChunkReadReqs[i * 4 + 1].size = baseEntry.texSectors << 11;
                g_worldChunkReadReqs[i * 4 + 1].dest = (void *)texBuf;
                g_worldChunkReadReqs[i * 4 + 2].size = formEntry.packSectors << 11;
                g_worldChunkReadReqs[i * 4 + 2].dest = NULL;
                g_worldChunkReadReqs[i * 4 + 3].size = formEntry.texSectors << 11;
                g_worldChunkReadReqs[i * 4 + 3].dest = NULL;
                if (g_worldChunkReadReqs[i * 4 + 1].size == 0) {
                    g_worldChunkReadReqs[i * 4 + 1].dest = NULL;
                }
                if (g_worldChunkReadReqs[i * 4 + 0].size == 0) {
                    g_worldChunkReadReqs[i * 4 + 0].dest = NULL;
                    slot->hasData = 0;
                    g_worldChunkRunPacks[i] = NULL;
                    g_worldChunkRunTex[i] = NULL;
                } else {
                    g_worldChunkRunPacks[i] = g_worldChunkReadReqs[i * 4 + 0].dest;
                    g_worldChunkRunTex[i] = g_worldChunkReadReqs[i * 4 + 1].dest;
                }
            } else {
                g_worldChunkReadReqs[i * 4 + 0].size = baseEntry.packSectors << 11;
                g_worldChunkReadReqs[i * 4 + 0].dest = NULL;
                g_worldChunkReadReqs[i * 4 + 1].size = baseEntry.texSectors << 11;
                g_worldChunkReadReqs[i * 4 + 1].dest = NULL;
                g_worldChunkReadReqs[i * 4 + 2].size = formEntry.packSectors << 11;
                g_worldChunkReadReqs[i * 4 + 2].dest = slot->pack;
                g_worldChunkReadReqs[i * 4 + 3].size = formEntry.texSectors << 11;
                g_worldChunkReadReqs[i * 4 + 3].dest = (void *)texBuf;
                if (g_worldChunkReadReqs[i * 4 + 3].size == 0) {
                    g_worldChunkReadReqs[i * 4 + 3].dest = NULL;
                }
                if (g_worldChunkReadReqs[i * 4 + 2].size == 0) {
                    g_worldChunkReadReqs[i * 4 + 2].dest = NULL;
                    slot->hasData = 0;
                    g_worldChunkRunPacks[i] = NULL;
                    g_worldChunkRunTex[i] = NULL;
                } else {
                    g_worldChunkRunPacks[i] = g_worldChunkReadReqs[i * 4 + 2].dest;
                    g_worldChunkRunTex[i] = g_worldChunkReadReqs[i * 4 + 3].dest;
                }
            }
        }
        if (g_worldChunkRunDir[g_worldChunkRunSlot] == 1) {
            wm_getChunkEntry(g_worldChunkRun[0]->chunkId, 0, &baseEntry);
            base = g_worldImageSectors[3];
        } else {
            wm_getChunkEntryT(g_worldChunkRun[0]->chunkId, 0, &baseEntry);
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
            cd_readChunks(baseEntry.sector + base, g_worldChunkReadList, g_worldChunkReadCount, 0);
        }
    }
end:
    if (g_worldCdUser == WM_CD_CHUNKS && cd_getState() == 0) {
        wm_chunksLoaded();
    }
}

u8 wm_findChunkRun(WmChunkSlot **out, s32 *count, s32 *dir) {
    WmChunkSlot *h[10];
    WmChunkSlot *vertRun[10];
    s32 hhi;
    s32 hlo;
    s32 vhi;
    s32 vlo;
    s32 i;
    WmChunkSlot *slot;

    hhi = 4;
    hlo = 4;
    *dir = 0;
    *count = 0;
    vhi = 4;
    vlo = 4;
    for (slot = g_worldChunkHead.next; slot->pack != NULL; slot = slot->next) {
        if (slot->state == WM_CHUNK_WANTED && slot->chunkId != -1 && slot->generation == g_worldChunkRunGeneration) {
            h[hlo] = slot;
            h[hhi] = slot;
            vertRun[vlo] = slot;
            vertRun[vhi] = slot;
            for (i = 0; i < 4; i++) {
                for (slot = g_worldChunkHead.next; slot->pack != NULL; slot = slot->next) {
                    if (slot->state == WM_CHUNK_WANTED) {
                        if (h[hlo]->chunkId - 1 == slot->chunkId) {
                            h[--hlo] = slot;
                        }
                        if (h[hhi]->chunkId + 1 == slot->chunkId) {
                            h[++hhi] = slot;
                        }
                        if (vertRun[vlo]->chunkId - 24 == slot->chunkId) {
                            vertRun[--vlo] = slot;
                        }
                        if (vertRun[vhi]->chunkId + 24 == slot->chunkId) {
                            vertRun[++vhi] = slot;
                        }
                    }
                }
            }
            if (hhi - hlo < vhi - vlo) {
                *dir = 24;
                for (i = vlo; i <= vhi; i++) {
                    out[(*count)++] = vertRun[i];
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
        if (st == WM_CHUNK_READING) {
            g_worldChunkRun[i]->state = WM_CHUNK_READY;
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
    WmChunkSlot *dst;
    WmChunkSlot *src;
    u32 i;

    if (g_worldChunkListDirty != 0) {
        src = g_worldChunkHead.next;
        g_worldChunkListDirty = 0;
        dst = src;
        for (i = 0; i < 16; i++) {
            src = src->next;
        }
        while (1) {
            if (src == &g_worldChunkTail) {
                break;
            }
            while (dst->pack != 0) {
                dst = dst->next;
            }
            dst->pack = src->pack;
            src->pack = 0;
            src->chunkId = -1;
            src->state = 0;
            src = src->next;
        }
    }
}

void wm_chunkNop(void) {
}

/* initialise the texture list */
void wm_blockListInit(void) {
    u32 i;

    g_worldBlockListDirty = 0;
    g_worldBlockNodes[0].prev = &g_worldBlockHead;
    g_worldBlockNodes[0].pos = &g_worldBlockPositions[0];
    g_worldBlockNodes[0].next = &g_worldBlockNodes[1];
    g_worldBlockNodes[0].block = NULL;
    g_worldBlockNodes[0].state = WM_BLOCK_NONE;
    g_worldBlockNodes[0].cell = -1;
    g_worldBlockNodes[0].slot = 0;
    for (i = 1; i < 39; i++) {
        g_worldBlockNodes[i].prev = &g_worldBlockNodes[i - 1];
        g_worldBlockNodes[i].next = &g_worldBlockNodes[i + 1];
        g_worldBlockNodes[i].block = NULL;
        g_worldBlockNodes[i].pos = NULL;
        g_worldBlockNodes[i].state = WM_BLOCK_NONE;
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
    g_worldBlockNodes[i].state = WM_BLOCK_NONE;
    g_worldBlockNodes[i].cell = -1;
    g_worldBlockNodes[i].slot = -1;
    g_worldBlockHead.prev = NULL;
    g_worldBlockHead.next = &g_worldBlockNodes[0];
    g_worldBlockTail.next = &g_worldBlockNodes[20];
    g_worldBlockTail.prev = NULL;
}

void wm_drawBlocks(void) {
    WmBlockNode *node;
    WmBlockPos *pos;
    MATRIX mat;
    WmQuad quad = g_worldBlockBackQuad;
    long sxy[4];
    VECTOR relPos;
    VECTOR viewPos;
    long interp;
    long flag;
    long xy3;
    u8 snd;
    s32 far;
    s32 k;
    u32 *ot;
    POLY_F4 *f4;

    node = g_worldBlockHead.next;
    while (node->cell != -1) {
        pos = node->pos;
        g_worldBlockMatrix.t[0] = pos->pos[0] - g_worldCameraTarget[0];
        g_worldBlockMatrix.t[1] = pos->pos[1] - g_worldCameraTarget[1];
        g_worldBlockMatrix.t[2] = pos->pos[2] - g_worldCameraTarget[2];
        CompMatrix(g_worldViewMatrix, &g_worldBlockMatrix, &mat);
        SetRotMatrix(&mat);
        SetTransMatrix(&mat);
        wm_worldToView((VECTOR *)pos, &relPos, &viewPos, 0);
        far = viewPos.vz < -10000 || viewPos.vz > 11000;
        k = node->cell % g_worldCellsW + node->cell / g_worldCellsW;
        if (node->state == WM_BLOCK_SET) {
            wm_drawBlock((u8 *)node->block, (node->cell % g_worldCellsW + node->cell / g_worldCellsW) % 2, far);
            if (RotTransPers4(&quad.v[0], &quad.v[1], &quad.v[2], &quad.v[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &interp, &flag) < 7000
                && g_worldPlayer->pos.vy > -7000) {
                f4 = (POLY_F4 *)g_worldPrimPtr;
                ot = (u32 *)g_worldOt[*g_worldBufIndex];
                *(s32 *)&f4->x0 = sxy[0];
                *(s32 *)&f4->x1 = sxy[1];
                *(s32 *)&f4->x2 = sxy[2];
                xy3 = sxy[3];
                setlen(f4, PRIM_LEN(POLY_F4));
                setRGB0(f4, 0, 0, 0);
                f4->code = GPU_POLY_F4;
                *(s32 *)&f4->x3 = xy3;
                addPrim(&ot[0xFFA], f4);
                f4++;
                g_worldPrimPtr = (s32)f4;
            }
        }
        if (node->block != NULL) {
            snd = node->block->fxId;
            if (snd != 0) {
                wm_drawBlockFx(snd);
            }
        }
        node = node->next;
    }
}

void wm_updateVisibleBlocks(void) {
    VECTOR pos;
    VECTOR p2;
    s16 sx[20];
    s16 sy[20];
    s32 z[20];
    VECTOR delta;
    WmNearCell swap;
    MATRIX mat;
    SVECTOR sv;
    VECTOR out;
    s32 cell;
    long flag;
    long interp;
    long sxy;
    s32 nearCount;
    s32 i;
    s32 j;
    s32 k;
    s32 hit;
    s32 cnt;
    s32 cnt2;
    s32 last;
    s32 id;
    WmBlock *cellBlock;
    SVECTOR *verts;
    WmScrRect *scrRect;
    WmNearCell *nearCell;
    WmBlockNode *nd;
    s32 lim;
    s32 prevIdx;

    pos.vy = 0;
    wm_blockListInit();
    pen_locate(10, 10);
    nearCount = 0;
    if (g_worldMoveMode < WM_MOVE_CHOCOBO_FLY) {
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
            cellBlock = (WmBlock *)wm_getCellBlock(cell, NULL);
            if (cellBlock != NULL) {
                wm_cellToPos(cell, &pos.vx, &pos.vz);
                g_worldBlockMatrix.t[0] = pos.vx - g_worldCameraTarget[0];
                g_worldBlockMatrix.t[1] = pos.vy - g_worldCameraTarget[1];
                g_worldBlockMatrix.t[2] = pos.vz - g_worldCameraTarget[2];
                CompMatrix(g_worldViewMatrix, &g_worldBlockMatrix, &mat);
                SetRotMatrix(&mat);
                SetTransMatrix(&mat);
                hit = 0;
                if (mat.t[2] >= -0x1D4B) {
                    k = (wm_getCameraViewState() == 0) * 2;
                    if (g_worldMoveModeInfo->controlType == WM_CTRL_FLY) {
                        k += 4;
                    }
                    if (g_worldMoveModeInfo->controlType == WM_CTRL_STEER) {
                        k += 8;
                    }
                    verts = cellBlock->verts;
                    for (cnt = 0; cnt < cellBlock->outlineCount; cnt++) {
                        sv = verts[cellBlock->outline[cnt]];
                        RotTrans(&sv, &out, &flag);
                        sv.vy = sv.vy * (0x1000 - (out.vz >> 4)) >> 12;
                        z[cnt] = RotTransPers(&sv, &sxy, &interp, &flag);
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
                    delta.vx = p2.vx - g_worldCameraEye[0];
                    delta.vz = p2.vz - g_worldCameraEye[2];
                    if (delta.vx > 0xFFFF) {
                        delta.vx -= g_worldWidth;
                    }
                    if (delta.vx < -0xFFFF) {
                        delta.vx += g_worldWidth;
                    }
                    if (delta.vz > 0xFFFF) {
                        delta.vz -= g_worldDepth;
                    }
                    if (delta.vz < -0xFFFF) {
                        delta.vz += g_worldDepth;
                    }
                    g_worldNearCells[nearCount].dist = SquareRoot0((delta.vx >> 2) * (delta.vx >> 2) + (delta.vz >> 2) * (delta.vz >> 2));
                    g_worldNearCells[nearCount].block = cellBlock;
                    g_worldNearCells[nearCount].cell = cell;
                    nearCount++;
                }
            }
        }
    }
    for (j = 0; j < nearCount - 1; j++) {
        for (i = nearCount - 1; j < i; i = prevIdx) {
            prevIdx = i - 1;
            if (g_worldNearCells[prevIdx].dist > g_worldNearCells[i].dist) {
                swap = g_worldNearCells[i];
                g_worldNearCells[i] = g_worldNearCells[prevIdx];
                g_worldNearCells[prevIdx] = swap;
            }
        }
    }
    lim = 16;
    if (g_worldMoveMode == WM_MOVE_BOAT) {
        lim = 14;
    }
    if (g_worldFog == 0) {
        lim++;
    }
    if (lim < nearCount) {
        nearCount = lim;
    }
    cnt2 = 0;
    last = -1;
    for (i = 0; i < nearCount; i++) {
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
            u8 fx = nd->block->fxId;
            if (fx != 0) {
                wm_updateBlockFx(fx);
            }
        }
        nd = nd->next;
    }
}

s32 wm_blockListRequest(s16 cell) {
    WmBlockNode *node = g_worldBlockHead.next;
    u32 i = 0;

    do {
        if (node->cell == -1) {
            if (i >= 20) {
                node->cell = cell;
                node->prev->next = node->next;
                node->next->prev = node->prev;
                node->next = g_worldBlockHead.next;
                node->prev = &g_worldBlockHead;
                g_worldBlockListDirty++;
                g_worldBlockHead.next->prev = node;
                g_worldBlockHead.next = node;
            } else {
                node->cell = cell;
                node->state = WM_BLOCK_NONE;
            }
            break;
        }
        if (node->cell == cell) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            node->next = g_worldBlockHead.next;
            node->prev = &g_worldBlockHead;
            g_worldBlockHead.next->prev = node;
            g_worldBlockHead.next = node;
            break;
        }
        node = node->next;
        i++;
    } while (i < 40);
    return i != 40;
}

void wm_blockListResolve(void) {
    WmBlockNode *node;
    WmChunkSlot *slot;
    u32 i;
    u32 j;
    s32 id;
    s16 colParity;
    s32 k;

    node = g_worldBlockHead.next;
    for (i = 0; i < 20; i++, node = node->next) {
        if (node->cell < 0) {
            return;
        }
        wm_cellToChunk(node->cell, &id);
        node->state = WM_BLOCK_NONE;
        slot = g_worldChunkHead.next;
        for (j = 0; j < 16; j++, slot = slot->next) {
            if (id == slot->chunkId) {
                if (slot->state == WM_CHUNK_READY) {
                    node->state = WM_BLOCK_SET;
                    wm_cellToPos(node->cell, &node->pos->pos[0], &node->pos->pos[2]);
                    node->pos->pos[1] = 0;
                    colParity = node->cell % 2;
                    k = colParity + (node->cell / g_worldCellsW) % 2 * 2;
                    if (slot->hasData != 0) {
                        node->block = slot->pack->blocks[k];
                        node->block->texArea = slot->pack->texArea;
                    } else {
                        node->block = g_worldDefaultBlocks->blocks[k];
                        node->block->texArea = 0;
                    }
                    break;
                }
                node->state = WM_BLOCK_SET;
                node->pos->pos[1] = 0;
                colParity = node->cell % 2;
                k = colParity + (node->cell / g_worldCellsW) % 2 * 2;
                wm_cellToPos(node->cell, &node->pos->pos[0], &node->pos->pos[2]);
                node->block = g_worldDefaultBlocks->blocks[k];
                node->block->texArea = 0;
            }
        }
    }
}

s32 wm_getCellBlock(s16 cell, u8 *outMissing) {
    s32 id;
    s32 i;
    WmChunkSlot *slot;
    s16 colParity;
    s32 k;

    slot = g_worldChunkHead.next;
    wm_cellToChunk(cell, &id);
    for (i = 0; i < 16; i++, slot = slot->next) {
        if (id == slot->chunkId && slot->state == WM_CHUNK_READY) {
            if (outMissing != NULL) {
                *outMissing = 0;
            }
            colParity = cell % 2;
            k = colParity + (cell / g_worldCellsW) % 2 * 2;
            if (slot->hasData != 0) {
                return (s32)slot->pack->blocks[k];
            }
            return (s32)g_worldDefaultBlocks->blocks[k];
        }
    }
    if (outMissing != NULL) {
        *outMissing = 1;
    }
    colParity = cell % 2;
    k = colParity + (cell / g_worldCellsW) % 2 * 2;
    return (s32)g_worldDefaultBlocks->blocks[k];
}

void wm_initBlockMatrix(void) {
    g_worldBlockMatrix.m[2][2] = ONE;
    g_worldBlockMatrix.m[1][1] = ONE;
    g_worldBlockMatrix.m[0][0] = ONE;
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
    WmBlockNode *dst;
    WmBlockNode *src;

    if (g_worldBlockListDirty != 0) {
        dst = g_worldBlockHead.next;
        g_worldBlockListDirty = 0;
        src = dst;
        for (i = 0; i < 20; i++) {
            src = src->next;
        }
        for (i = 0; i < 20; i++) {
            if (dst->pos == 0) {
                dst->block = src->block;
                dst->pos = src->pos;
                src->block = 0;
                src->pos = 0;
                src->cell = -1;
                src->state = 0;
                src = src->next;
            }
            dst = dst->next;
        }
    }
}

s32 wm_getGroundHeight(VECTOR *inPos, u32 *outAttr, s32 *outIdx, WmPolyCache *cache, u16 *outHeight) {
    VECTOR pos;
    VECTOR rel;
    VECTOR cellOrg;
    s32 cell;
    u8 flag;
    s32 cellBlock;

    pos = *inPos;
    wm_wrapPos(&pos.vx, &pos.vz);
    wm_posToCell(pos.vx, pos.vz, &cell);
    cellBlock = wm_getCellBlock(cell, &flag);
    if (flag == 0 && cellBlock != 0) {
        wm_cellToPos(cell, &cellOrg.vx, &cellOrg.vz);
        rel.vx = pos.vx - cellOrg.vx;
        rel.vy = pos.vy;
        rel.vz = pos.vz - cellOrg.vz;
        /* callers pass a 16-bit variable but a word is stored */
        *(s32 *)outHeight = wm_getBlockHeight(cell, (WmBlock *)cellBlock, &rel, outAttr, outIdx, cache);
        if ((u32)((u8)g_worldMoveMode - WM_MOVE_CHOCOBO_REEF) < 2) {
            if (*outAttr - 0x38 < 2) {
                *outAttr = 0x36;
            }
        }
        return 1;
    }
    *(s32 *)outHeight = pos.vy;
    return 0;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", s_wmStrEncounterNoScene);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", s_wmStrBlockNoForm);

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/tu_800BA528", g_worldBlockBackQuad);

void wm_naviInit(void) {
    s32 i;
    u8 *globalVars;

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
    g_worldNaviMode = (globalVars = g_evtGlobalVars.b)[0x64];
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
    case WM_NAVI_MINI:
        if (g_worldVehicleActor != NULL && g_worldVehicleActor->state == EVT_STATE_RUNNING && (g_worldVehicleActor->flags & EVT_OBJ_ENABLED) &&
            !((u8)g_worldMoveMode >= WM_MOVE_BOAT && (u8)g_worldMoveMode < WM_MOVE_FAST_WALK)) {
            CVECTOR c0 = g_worldNaviVehicleColorA;
            CVECTOR c1 = g_worldNaviVehicleColorB;
            wm_naviWorldToMap(g_worldVehicleActor->pos.vx, g_worldVehicleActor->pos.vz, &x, &z, page * 2);
            wm_naviDrawMarker(x, z, 1, 1, c0, c1, 0x40, 0xC8, 0x82, 1);
        }
        if (g_worldMountActor != NULL && g_worldMountActor->state == EVT_STATE_RUNNING && (g_worldMountActor->flags & EVT_OBJ_ENABLED) &&
            !((u8)g_worldMoveMode >= WM_MOVE_CHOCOBO_FIELD && (u8)g_worldMoveMode < WM_MOVE_BOAT)) {
            CVECTOR c0 = g_worldNaviMountColorA;
            CVECTOR c2 = g_worldNaviMountColorB;
            wm_naviWorldToMap(g_worldMountActor->pos.vx, g_worldMountActor->pos.vz, &x, &z, page * 2);
            wm_naviDrawMarker(x, z, 1, 1, c0, c2, 0x40, 0xC8, 0x82, 1);
        }
        wm_naviDrawMiniMap();
        if (g_gameState->gameData->chocograph != 0) {
            wm_naviDrawChocograph();
        }
        break;
    case WM_NAVI_FULL:
        if (g_worldNaviVisible != 0) {
            if (g_worldVehicleActor != NULL && g_worldVehicleActor->state == EVT_STATE_RUNNING && (g_worldVehicleActor->flags & EVT_OBJ_ENABLED) &&
                !((u8)g_worldMoveMode >= WM_MOVE_BOAT && (u8)g_worldMoveMode < WM_MOVE_FAST_WALK)) {
                CVECTOR c0 = g_worldNaviVehicleColorA;
                CVECTOR c3 = g_worldNaviVehicleColorB;
                wm_naviWorldToMap(g_worldVehicleActor->pos.vx, g_worldVehicleActor->pos.vz, &x, &z, page * 2 + 1);
                wm_naviDrawMarker(x, z, 1, 1, c0, c3, 0x40, 0x28, 9, 1);
            }
            if (g_worldMountActor != NULL && g_worldMountActor->state == EVT_STATE_RUNNING && (g_worldMountActor->flags & EVT_OBJ_ENABLED) &&
                !((u8)g_worldMoveMode >= WM_MOVE_CHOCOBO_FIELD && (u8)g_worldMoveMode < WM_MOVE_BOAT)) {
                CVECTOR c0 = g_worldNaviMountColorA;
                CVECTOR c1 = g_worldNaviMountColorB;
                wm_naviWorldToMap(g_worldMountActor->pos.vx, g_worldMountActor->pos.vz, &x, &z, page * 2 + 1);
                wm_naviDrawMarker(x, z, 1, 1, c0, c1, 0x40, 0x28, 9, 1);
            }
            if (g_worldNaviCursorDraw != 0) {
                msg_setOtz(5);
                pen_begin();
                pen_setPos((g_worldNaviCursorX >> 12) + 18, (g_worldNaviCursorY >> 12) + 6);
                pen_drawSprite(UI_SPRITE_HAND);
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
    s32 speed;
    s32 best;
    s32 bi;
    s32 i;
    s32 ex, ez;
    s32 fx, fz;
    s32 stickLen;
    s32 dist;
    s32 last;
    s32 lim;

    if (g_worldNaviActive != 0) {
        pad = 0;
        if (g_worldNaviMode == WM_NAVI_FULL) {
            dir = 1;
            stick.vx = 0;
            stick.vy = 0;
            if (g_worldNaviPrevMode == WM_NAVI_MINI) {
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
                stickLen = SquareRoot0(stick.vx * stick.vx + stick.vy * stick.vy) << 5;
                speed = stickLen + (stickLen >> 1);
            } else {
                speed = 0;
            }
            if (speed != 0) {
                g_worldNaviCursorX += math_sin(ang) * speed >> 12;
                g_worldNaviCursorY -= math_cos(ang) * speed >> 12;
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
                if (g_evtGlobalVars.b[EVT_GB_NAVI_PLACE] == 0) {
                    best = 0x7FFFFFFF;
                    bi = -1;
                    for (i = 0; i < 64; i++) {
                        if ((u8)wm_naviIsPlaceKnown(i)) {
                            fx = ((g_worldNaviPlaces[g_worldNaviPage][i].mapX << 12) - g_worldNaviCursorX) >> 12;
                            fz = ((g_worldNaviPlaces[g_worldNaviPage][i].mapY << 12) - g_worldNaviCursorY) >> 12;
                            dist = SquareRoot0(fx * fx + fz * fz);
                            if ((g_worldNaviPage != 1 || ((u32)(i - 6) >= 4 && i != 15)) && dist < best) {
                                best = dist;
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
                if (g_evtGlobalVars.b[EVT_GB_NAVI_PLACE] != 0 && dir != 0) {
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
                g_evtGlobalVars.b[EVT_GB_NAVI_PLACE] = 0;
            } else {
                g_evtGlobalVars.b[EVT_GB_NAVI_PLACE] = last + 1;
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

void wm_naviDrawMarker(s32 x, s32 y, s32 halfW, s32 halfH, CVECTOR color0, CVECTOR color1, s32 pulseSpeed, s32 offsetX, s32 offsetY, s32 textured) {
    CVECTOR color;
    u32 *ot;

    ot = (u32 *)g_worldOt[*g_worldBufIndex];
    color.r = wm_lerp(color0.r, color1.r, math_cos((g_worldFrameCount * pulseSpeed) & 0xFFF));
    color.g = wm_lerp(color0.g, color1.g, math_cos((g_worldFrameCount * pulseSpeed) & 0xFFF));
    color.b = wm_lerp(color0.b, color1.b, math_cos((g_worldFrameCount * pulseSpeed) & 0xFFF));
    if (textured == 0) {
        setXYWH((POLY_F4 *)g_worldPrimPtr, x - halfW + offsetX, y - halfH + offsetY, halfW * 2 + 1, halfH * 2 + 1);
        setRGB0((POLY_F4 *)g_worldPrimPtr, color.r, color.g, color.b);
        setPolyF4((POLY_F4 *)g_worldPrimPtr);
        addPrim(&ot[10], (POLY_F4 *)g_worldPrimPtr);
        g_worldPrimPtr += sizeof(POLY_F4);
    } else {
        setXYWH((POLY_FT4 *)g_worldPrimPtr, x + offsetX - 1, y + offsetY - 1, 4, 4);
        setUV4((POLY_FT4 *)g_worldPrimPtr, 0, 0xFA, 4, 0xFA, 0, 0xFE, 4, 0xFE);
        setRGB0((POLY_FT4 *)g_worldPrimPtr, color.r, color.g, color.b);
        setPolyFT4((POLY_FT4 *)g_worldPrimPtr);
        ((POLY_FT4 *)g_worldPrimPtr)->tpage = getTPage(0, 1, 704, 0);
        ((POLY_FT4 *)g_worldPrimPtr)->clut = getClut(704, 255);
        setSemiTrans((POLY_FT4 *)g_worldPrimPtr, 1);
        addPrim(&ot[10], (POLY_FT4 *)g_worldPrimPtr);
        g_worldPrimPtr += sizeof(POLY_FT4);
        setXYWH((POLY_FT4 *)g_worldPrimPtr, x + offsetX - 1, y + offsetY - 1, 4, 4);
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
    VECTOR arrowPts[4];
    SVECTOR rot;
    SVECTOR rot0;
    MATRIX rotMat;
    s32 x;
    s32 y;
    s32 *px;
    s32 *py;
    u32 *ot;
    s32 i;
    s32 pulse;
    POLY_FT4 *poly;
    s32 coneAng0;
    s32 coneAng1;
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
    math_rotMatrixXYZ(&rot, &rotMat);
    for (i = 0; i < 4; i++) {
        ApplyMatrix(&rotMat, &arrow.v[i], &arrowPts[i]);
    }
    px = &x;
    py = &y;
    wm_naviWorldToMap(g_worldPlayer->pos.vx, g_worldPlayer->pos.vz, px, py, g_worldNaviPage * 2 + 1);
    pulse = (math_sin(g_worldFrameCount << 6) >> 7) + 0x80;
    poly = (POLY_FT4 *)g_worldPrimPtr;
    setRGB0(poly, pulse, pulse, pulse);
    poly->x0 = x + arrowPts[0].vx + 0x28;
    poly->y0 = y + arrowPts[0].vy + 9;
    poly->x1 = x + arrowPts[1].vx + 0x28;
    poly->y1 = y + arrowPts[1].vy + 9;
    poly->x2 = x + arrowPts[2].vx + 0x28;
    poly->y2 = y + arrowPts[2].vy + 9;
    poly->x3 = x + arrowPts[3].vx + 0x28;
    poly->y3 = y + arrowPts[3].vy + 9;
    setUV4(poly, 0x20, 0xD0, 0x40, 0xD0, 0x20, 0xD7, 0x40, 0xD7);
    poly->tpage = GetTPage(0, 0, 0x300, 0xD0);
    poly->clut = GetClut(0x280, 0xF5);
    setPolyFT4(poly);
    if (g_worldNaviVisible) {
        addPrim(&ot[10], poly);
    }
    poly++;
    setRGB0(poly, 0x80, 0x80, 0x80);
    poly->x0 = x + arrowPts[0].vx + 0x28;
    poly->y0 = y + arrowPts[0].vy + 9;
    poly->x1 = x + arrowPts[1].vx + 0x28;
    poly->y1 = y + arrowPts[1].vy + 9;
    poly->x2 = x + arrowPts[2].vx + 0x28;
    poly->y2 = y + arrowPts[2].vy + 9;
    poly->x3 = x + arrowPts[3].vx + 0x28;
    poly->y3 = y + arrowPts[3].vy + 9;
    setUV4(poly, 0, 0xD0, 0x20, 0xD0, 0, 0xD7, 0x20, 0xD7);
    poly->tpage = GetTPage(0, 0, 0x300, 0xD0);
    poly->clut = GetClut(0x280, 0xF5);
    setPolyFT4(poly);
    if (g_worldNaviVisible) {
        addPrim(&ot[10], poly);
    }
    poly++;
    g_worldPrimPtr = (s32)poly;
    coneAng0 = 0xF00 - g_worldCamera->yaw;
    coneAng1 = 0x1100 - g_worldCamera->yaw;
    if (coneAng0 < 0) {
        coneAng0 += 0x1000;
    }
    if (coneAng1 < 0) {
        coneAng1 += 0x1000;
    }
    coneAng0 %= 0x1000;
    coneAng1 %= 0x1000;
    ax = math_sin(coneAng0) * 0x18 >> 12;
    ay = math_cos(coneAng0) * 0x18 >> 12;
    bx = math_sin(coneAng1) * 0x18 >> 12;
    by = math_cos(coneAng1) * 0x18 >> 12;
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
    VECTOR arrowPts[4];
    SVECTOR rot;
    SVECTOR rot0;
    MATRIX rotMat;
    s32 x;
    s32 y;
    s32 *px;
    s32 *py;
    u32 *ot;
    s32 i;
    s32 pulse;
    POLY_FT4 *poly;
    s32 coneAng0;
    s32 coneAng1;
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
    pulse = (math_sin(g_worldFrameCount << 6) >> 7) + 0x80;
    math_rotMatrixXYZ(&rot, &rotMat);
    for (i = 0; i < 4; i++) {
        ApplyMatrix(&rotMat, &arrow.v[i], &arrowPts[i]);
    }
    px = &x;
    py = &y;
    wm_naviWorldToMap(g_worldPlayer->pos.vx, g_worldPlayer->pos.vz, px, py, g_worldNaviPage * 2);
    poly = (POLY_FT4 *)g_worldPrimPtr;
    setRGB0(poly, pulse, pulse, pulse);
    poly->x0 = x + arrowPts[0].vx + 200;
    poly->y0 = y + arrowPts[0].vy + 130;
    poly->x1 = x + arrowPts[1].vx + 200;
    poly->y1 = y + arrowPts[1].vy + 130;
    poly->x2 = x + arrowPts[2].vx + 200;
    poly->y2 = y + arrowPts[2].vy + 130;
    poly->x3 = x + arrowPts[3].vx + 200;
    poly->y3 = y + arrowPts[3].vy + 130;
    setUV4(poly, 0, 0xD0, 0x20, 0xD0, 0, 0xD7, 0x20, 0xD7);
    poly->tpage = GetTPage(0, 0, 0x300, 0xD0);
    poly->clut = GetClut(0x280, 0xF5);
    setPolyFT4(poly);
    addPrim(&ot[10], poly);
    poly++;
    g_worldPrimPtr = (s32)poly;
    coneAng0 = 0xF00 - g_worldCamera->yaw;
    coneAng1 = 0x1100 - g_worldCamera->yaw;
    if (coneAng0 < 0) {
        coneAng0 += 0x1000;
    }
    if (coneAng1 < 0) {
        coneAng1 += 0x1000;
    }
    coneAng0 %= 0x1000;
    coneAng1 %= 0x1000;
    ax = math_sin(coneAng0) * 12 >> 12;
    ay = math_cos(coneAng0) * 12 >> 12;
    bx = math_sin(coneAng1) * 12 >> 12;
    by = math_cos(coneAng1) * 12 >> 12;
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
    POLY_FT4 *poly;
    u32 fadeOutFrame;
    u32 endFrame;

    ot = (u32 *)g_worldOt[*g_worldBufIndex];
    g_worldTitlePhase = 0;
    fadeOutFrame = g_worldTitleDelay + 0x78;
    endFrame = g_worldTitleDelay + 0xB4;
    if ((u32)g_worldTitleDelay < (u32)g_worldReadyFrameCount) {
        g_worldTitlePhase = 1;
    }
    if (fadeOutFrame < (u32)g_worldReadyFrameCount) {
        g_worldTitlePhase = 2;
    }
    if (endFrame < (u32)g_worldReadyFrameCount) {
        g_worldTitlePhase = 3;
    }
    if (endFrame <= (u32)g_worldReadyFrameCount) {
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
        poly = g_worldTitlePrim;
        setaddr(poly, getaddr(&ot[10]));
        g_worldTitlePrim = poly + 1;
        setaddr(&ot[10], poly);
    }
    g_worldPrimPtr = (s32)g_worldTitlePrim;
}

void wm_naviDrawPlaces(void) {
    CVECTOR c0;
    s32 i;
    u8 page0Only;

    c0 = wm_naviDrawPlaces_color();
    {
        CVECTOR c1 = wm_naviDrawPlaces_color();

        for (i = 0; i < 64; i++) {
            if ((g_worldNaviPlaces[g_worldNaviPage][i].mapX | g_worldNaviPlaces[g_worldNaviPage][i].mapY) != 0) {
                page0Only = 0;
                if (i >= 7 && (i < 11 || i == 15)) {
                    page0Only = 1;
                }
                if ((g_worldNaviPage == 0 || !page0Only) && (u8)wm_naviIsPlaceKnown(i)) {
                    wm_naviDrawMarker(g_worldNaviPlaces[g_worldNaviPage][i].mapX, g_worldNaviPlaces[g_worldNaviPage][i].mapY, 1, 1, c0, c1, 16, 40, 9, 1);
                }
            }
        }
    }
}

s32 wm_naviIsPlaceKnown(u32 place) {
    u32 bit = 0;
    u16 knownBits = 0;

    if (place < 0x10) {
        knownBits = g_evtGlobalVars.h[0x2E];
        bit = place;
    }
    if (place - 0x10 < 0x10) {
        knownBits = g_evtGlobalVars.h[0x2F];
        bit = place - 0x10;
    }
    if (place - 0x20 < 0x10) {
        knownBits = g_evtGlobalVars.h[0x30];
        bit = place - 0x20;
    }
    if (place - 0x30 < 0x10) {
        knownBits = g_evtGlobalVars.h[0x31];
        bit = place - 0x30;
    }
    if ((g_worldNaviPlaces[g_worldNaviPage][place].mapX | g_worldNaviPlaces[g_worldNaviPage][place].mapY) == 0) {
        return 0;
    }
    return (knownBits >> bit) & 1;
}

void wm_naviDrawChocograph(void) {
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

/* draw the four compass frame pieces at (x, y) */
void wm_naviDrawFrame(s32 x, s32 y) {
    POLY_FT4 *poly;
    s16 clut;
    u32 *ot;
    s32 i;

    poly = (POLY_FT4 *)g_worldPrimPtr;
    clut = GetClut(0x280, 0xF0);
    ot = (u32 *)g_worldOt[*g_worldBufIndex];
    for (i = 0; i < 4; i++) {
        SetPolyFT4(poly);
        if (i < 2) {
            setXYWH(poly, g_worldNaviFrameParts[i].x + x, g_worldNaviFrameParts[i].y + y, g_worldNaviFrameParts[i].w,
                    g_worldNaviFrameParts[i].h);
        } else {
            /* rotated a quarter turn */
            setXY4(poly, g_worldNaviFrameParts[i].x + x, g_worldNaviFrameParts[i].y + y + g_worldNaviFrameParts[i].w,
                   g_worldNaviFrameParts[i].x + x, y + g_worldNaviFrameParts[i].y - (i != 2),
                   g_worldNaviFrameParts[i].x + x + g_worldNaviFrameParts[i].h,
                   g_worldNaviFrameParts[i].y + y + g_worldNaviFrameParts[i].w,
                   g_worldNaviFrameParts[i].x + x + g_worldNaviFrameParts[i].h, y + g_worldNaviFrameParts[i].y - (i != 2));
        }
        setUVWH(poly, g_worldNaviFrameParts[i].u, g_worldNaviFrameParts[i].v, g_worldNaviFrameParts[i].w, g_worldNaviFrameParts[i].h);
        setRGB0(poly, 0x80, 0x80, 0x80);
        poly->tpage = getTPage(0, 0, 768, 0);
        poly->clut = clut;
        addPrim(&ot[10], poly);
        poly++;
    }
    g_worldPrimPtr = (s32)poly;
}

void wm_naviWorldToMap(s32 worldX, s32 worldZ, s32 *outX, s32 *outY, u8 mapMode) {
    switch (mapMode) {
    case 0:
        *outX = ((worldX - 0x30000) * 14 / 16 >> 11) + 15;
        *outY = ((-0x22000 - worldZ) * 14 / 16 >> 11) + 6;
        break;
    case 1:
        *outX = (worldX - 0x30000) / 819 * 14 / 16 + 31;
        *outY = (-0x22000 - worldZ) / 819 * 14 / 16 + 12;
        break;
    case 2:
        *outX = worldX >> 12;
        *outY = -worldZ >> 12;
        break;
    case 3:
        *outX = worldX / 1638;
        *outY = -worldZ / 1638;
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
