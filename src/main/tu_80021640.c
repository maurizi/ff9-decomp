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
    g_gameState->gameData->unk8B2 = a;
}

s32 evt_createModel(EvtObject *a) {
    return g_gameState->gameData->unk838(a->unk34 | DB_MODEL, a);
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
    if (p->unk5 != 4) {
        return 0;
    }
    if (p->unk34 == 0xFFFF) {
    ret0:
        return 0;
    }
    return bgi_updateActor(p->unk3B);
}

void mbg_resetState(void) {
    g_mbgState = 0;
}

CdReadCtx *cdRead_init(void *arg0, s32 a, s32 b, void *c, s32 d, s32 e, void (*f)(CdReadCtx *)) {
    CdReadCtx *p = arg0;
    if (p != NULL) {
        p->unk0 = a;
        p->unk4 = b;
        p->unk8 = c;
        p->unkC = d;
        p->unk20 = c;
        p->unk10 = e;
        p->unk14 = f;
        p->unk18 = b;
        p->unk1C = 1;
    }
    return p;
}

s32 cdRead_update(void *arg0) {
    CdReadCtx *p = arg0;
    s32 n;

    if (p != NULL) {
        switch (p->unk1C) {
        case 1:
            if (cd_getState() == 0) {
                p->unk1C = 2;
            }
            break;
        case 2:
            n = p->unk4 - p->unkC;
            if (p->unk10 != -1 && p->unk10 < n) {
                n = p->unk10;
            }
            if (n != 0) {
                cd_readAsync(g_gameState->imgSector + p->unk0 + p->unkC / 2048, n, p->unk8, NULL);
            }
            p->unk10 = n;
            p->unk1C = 3;
            p->unk20 = p->unk8 + n;
            return 3;
        case 3:
            if (cd_getState() == 0) {
                if (p->unk14 != NULL) {
                    p->unk14(p);
                }
                p->unk1C = 0;
            }
            break;
        default:
            return 0;
        }
        return p->unk1C;
    }
    return 0;
}

s32 cdRead_getBytesRead(Unk80021910 *a) {
    if (a->unk1C >= 3) {
        return g_cdLoad.bytesRead;
    }
    return 0;
}
