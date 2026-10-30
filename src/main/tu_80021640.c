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
    return ((u16 *)db_lookupRegistered(4, b | DB_ANIM, 0))[1];
}

s32 evt_updateActorWalkmesh(EvtObject *p) {
    if (g_gameState->mode == 3) {
        goto ret0;
    }
    if (p->kind != 4) {
        return 0;
    }
    if (p->modelId == 0xFFFF) {
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
        p->state = 1;
    }
    return p;
}

s32 cdRead_update(void *arg0) {
    CdReadCtx *p = arg0;
    s32 n;

    if (p != NULL) {
        switch (p->state) {
        case 1:
            if (cd_getState() == 0) {
                p->state = 2;
            }
            break;
        case 2:
            n = p->size - p->offset;
            if (p->chunk != -1 && p->chunk < n) {
                n = p->chunk;
            }
            if (n != 0) {
                cd_readAsync(g_gameState->imgSector + p->sector + p->offset / 2048, n, p->dest, NULL);
            }
            p->chunk = n;
            p->state = 3;
            p->end = p->dest + n;
            return 3;
        case 3:
            if (cd_getState() == 0) {
                if (p->onDone != NULL) {
                    p->onDone(p);
                }
                p->state = 0;
            }
            break;
        default:
            return 0;
        }
        return p->state;
    }
    return 0;
}

s32 cdRead_getBytesRead(Unk80021910 *a) {
    if (a->unk1C >= 3) {
        return g_cdLoad.bytesRead;
    }
    return 0;
}
