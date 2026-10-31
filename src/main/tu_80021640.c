#include "ff9.h"

void evt_setNextLocation(s32 a) {
    game_setNextScene(a);
}

void evt_setNextBattle(u16 a) {
    game_setNextScene(a);
}

void evt_setCardOpponent(a)
    s16 a;
{
    g_gameState->gameData->cardOpponent = a;
}

s32 evt_createModel(EvtObject *a) {
    return g_gameState->gameData->modelHook(a->modelId | DB_MODEL, a);
}

s32 evt_getAnimFrameCount(a, b)
    s32 a;
    u16 b;
{
    return ((u16 *)db_lookupRegistered(DB_GET_ADDR, b | DB_ANIM, 0))[1];
}

s32 evt_updateActorWalkmesh(EvtObject *p) {
    if (g_gameState->mode == GS_MODE_WORLD) {
        goto ret0;
    }
    if (p->kind != EVT_KIND_ACTOR) {
        return 0;
    }
    if (p->modelId == EVT_MODEL_NONE) {
    ret0:
        return 0;
    }
    return bgi_updateActor(p->modelSlot);
}

void mbg_resetState(void) {
    g_mbgState = 0;
}

CdReadCtx *cdRead_init(void *arg0, s32 a, s32 b, void *c, s32 d, s32 e, void (*f)(CdReadCtx *)) {
    CdReadCtx *p = arg0;
    if (p != NULL) {
        p->sector = a;
        p->size = b;
        p->dest = c;
        p->offset = d;
        p->end = c;
        p->chunk = e;
        p->onDone = f;
        p->unk18 = b;
        p->state = CDREAD_WAIT_CD;
    }
    return p;
}

s32 cdRead_update(void *arg0) {
    CdReadCtx *p = arg0;
    s32 n;

    if (p != NULL) {
        switch (p->state) {
        case CDREAD_WAIT_CD:
            if (cd_getState() == 0) {
                p->state = CDREAD_START;
            }
            break;
        case CDREAD_START:
            n = p->size - p->offset;
            if (p->chunk != -1 && p->chunk < n) {
                n = p->chunk;
            }
            if (n != 0) {
                cd_readAsync(g_gameState->imgSector + p->sector + p->offset / 2048, n, p->dest, NULL);
            }
            p->chunk = n;
            p->state = CDREAD_READING;
            p->end = p->dest + n;
            return 3;
        case CDREAD_READING:
            if (cd_getState() == 0) {
                if (p->onDone != NULL) {
                    p->onDone(p);
                }
                p->state = CDREAD_DONE;
            }
            break;
        default:
            return 0;
        }
        return p->state;
    }
    return 0;
}

s32 cdRead_getBytesRead(CdReadCtx *a) {
    if (a->state >= CDREAD_READING) {
        return g_cdLoad.bytesRead;
    }
    return 0;
}
