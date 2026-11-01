#include "ff9.h"

void evt_setNextLocation(s32 sceneId) {
    game_setNextScene(sceneId);
}

void evt_setNextBattle(u16 sceneId) {
    game_setNextScene(sceneId);
}

void evt_setCardOpponent(opponent)
    s16 opponent;
{
    g_gameState->gameData->cardOpponent = opponent;
}

s32 evt_createModel(EvtObject *obj) {
    return g_gameState->gameData->modelHook(obj->modelId | DB_MODEL, obj);
}

s32 evt_getAnimFrameCount(unused, animId)
    s32 unused;
    u16 animId;
{
    return ((u16 *)db_lookupRegistered(DB_GET_ADDR, animId | DB_ANIM, 0))[1];
}

s32 evt_updateActorWalkmesh(EvtObject *obj) {
    if (g_gameState->mode == GS_MODE_WORLD) {
        goto ret0;
    }
    if (obj->kind != EVT_KIND_ACTOR) {
        return 0;
    }
    if (obj->modelId == EVT_MODEL_NONE) {
    ret0:
        return 0;
    }
    return bgi_updateActor(obj->modelSlot);
}

void mbg_resetState(void) {
    g_mbgState = 0;
}

CdReadCtx *cdRead_init(void *ctx, s32 sector, s32 size, void *dest, s32 offset, s32 chunk, void (*onDone)(CdReadCtx *)) {
    CdReadCtx *cdCtx = ctx;
    if (cdCtx != NULL) {
        cdCtx->sector = sector;
        cdCtx->size = size;
        cdCtx->dest = dest;
        cdCtx->offset = offset;
        cdCtx->end = dest;
        cdCtx->chunk = chunk;
        cdCtx->onDone = onDone;
        cdCtx->unk18 = size;
        cdCtx->state = CDREAD_WAIT_CD;
    }
    return cdCtx;
}

s32 cdRead_update(void *ctx) {
    CdReadCtx *cdCtx = ctx;
    s32 bytes;

    if (cdCtx != NULL) {
        switch (cdCtx->state) {
        case CDREAD_WAIT_CD:
            if (cd_getState() == 0) {
                cdCtx->state = CDREAD_START;
            }
            break;
        case CDREAD_START:
            bytes = cdCtx->size - cdCtx->offset;
            if (cdCtx->chunk != -1 && cdCtx->chunk < bytes) {
                bytes = cdCtx->chunk;
            }
            if (bytes != 0) {
                cd_readAsync(g_gameState->imgSector + cdCtx->sector + cdCtx->offset / 2048, bytes, cdCtx->dest, NULL);
            }
            cdCtx->chunk = bytes;
            cdCtx->state = CDREAD_READING;
            cdCtx->end = cdCtx->dest + bytes;
            return 3;
        case CDREAD_READING:
            if (cd_getState() == 0) {
                if (cdCtx->onDone != NULL) {
                    cdCtx->onDone(cdCtx);
                }
                cdCtx->state = CDREAD_DONE;
            }
            break;
        default:
            return 0;
        }
        return cdCtx->state;
    }
    return 0;
}

s32 cdRead_getBytesRead(CdReadCtx *ctx) {
    if (ctx->state >= CDREAD_READING) {
        return g_cdLoad.bytesRead;
    }
    return 0;
}
