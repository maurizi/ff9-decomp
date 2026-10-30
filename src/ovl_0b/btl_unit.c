#include "ovl_0b.h"

void btl_initEquipElements(Character *c, BtlUnit *u) {
    s32 i;

    u->elemBoost = 0;
    u->elemWeak = 0;
    u->elemHalf = 0;
    u->elemAbsorb = 0;
    u->elemGuard = 0;
    for (i = 0; i < 5; i++) {
        if (c->equip[i] != 0xFF) {
            func_800BDACC(&u->elemGuard, g_equipStatBonus[g_itemTable[c->equip[i]].statBonus].elemGuard);
            u->elemBoost |= g_equipStatBonus[g_itemTable[c->equip[i]].statBonus].elemBoost;
        }
    }
}

void func_800BC1E0(Unk800BC1E0 *arg0, s16 *arg1) {
    if (!(arg0->unk0 & 8)) {
        arg1[0] = 0;
        arg1[1] = arg0->unk8;
    } else {
        arg1[0] = arg0->unk8;
        arg1[1] = 0;
    }
    arg1[2] = arg0->unkA;
}

void btl_updateUnitModel(BtlUnit *u, s32 *ot) {
    BtlUnit9C *m = u->model;
    BtlUnitA0 *a = u->obj;
    BtlUnit *v;
    SVECTOR tmp;
    SVECTOR rot;
    s32 i;
    s32 n;
    s32 t;
    BtlUnit9CC *p;
    s32 id;
    BtlUnit9C *w;

    if ((u->flags >> 23) & 1) {
        v = btl_findBodyUnit();
        u->rot[0] = v->rot[0];
        u->rot[1] = v->rot[1];
        u->rot[2] = v->rot[2];
        u->home->pos[0] = u->model->bones[u->baseBone].tx << 16;
        u->home->pos[1] = 0;
        u->home->pos[2] = u->model->bones[u->baseBone].tz << 16;
        u->pos[0] = u->home->pos[0];
        u->pos[1] = u->home->pos[1];
        u->pos[2] = u->home->pos[2];
        return;
    }
    if ((u->flags & 1) && (s32)m->motion == u->motions[17]) {
        memset(&rot, 0, 8);
        rot.vx = u->rot[0];
        rot.vy = 0x800;
        rot.vz = u->rot[2];
        tmp = rot;
        math_rotMatrixZYX(&tmp, &m->matrix);
    } else {
        memset(&rot, 0, 8);
        rot.vx = u->rot[0];
        rot.vy = u->rot[1];
        rot.vz = u->rot[2];
        tmp = rot;
        math_rotMatrixZYX(&tmp, &m->matrix);
    }
    m->matrix.t[0] = u->pos[0] >> 16;
    m->matrix.t[1] = u->pos[1] >> 16;
    m->matrix.t[2] = u->pos[2] >> 16;
    m->prims = 0;
    if (a->frame >= m->motion->frameCount) {
        if (!(u8)btl_setIdleMotion(u)) {
            u->obj->frame = 0;
        } else if (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) {
            u->obj->frame--;
        }
    }
    btl_applyModelScale(m);
    p = m->motion;
    if (IS_OFFSET(p->rotKeys)) {
        t = p->rotKeysLo;
        p->rotKeys = (s32)p + p->rotKeys;
        if (t != 0) {
            p->rotKeysLo = (s32)m->motion + t;
        }
    }
    *ot = (s32)btl_calcBoneMatrices(m, a->frame, (u8 *)*ot);
    if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) && !((u->flags >> 21) & 1)) {
        a->frame++;
    }
    n = m->header->meshCount;
    btl_updateModelVisibility(m);
    u->model->flags &= ~0x20;
    for (i = 0; i < n; i++) {
        if (!(m->hiddenMeshes & (1 << i))) {
            btl_projectMesh(m, i);
            *ot = (s32)btl_drawMesh(m, i, (u8 *)*ot);
            u->model->flags |= 0x20;
        }
    }
    if (!(u->status & BTL_STATUS_JUMP)) {
        btl_updateTexAnims((TexAnimSet *)m->texAnims);
    }
    if (u->weaponModel != NULL) {
        u->weaponModel->prims = 0;
        *ot = (s32)btl_calcBoneMatrices(u->weaponModel, 0, (u8 *)*ot);
        w = u->weaponModel;
        n = w->header->meshCount;
        btl_updateModelVisibility(w);
        u->weaponModel->flags &= ~0x20;
        for (i = 0; i < n; i++) {
            if (!(u->weaponModel->hiddenMeshes & (1 << i))) {
                btl_projectMesh(u->weaponModel, i);
                *ot = (s32)btl_drawMesh(u->weaponModel, i, (u8 *)*ot);
                u->weaponModel->flags |= 0x20;
            }
        }
    }
    if ((u->flags >> 24) & 1) {
        id = (u->shadowDirBone << 8) | u->shadowBone;
        if (u->flags & 1) {
            if (!(g_gameState->gameData->ctx.battle->cmdFlags & 1)) {
                if (u->escapeStance != g_gameState->gameData->ctx.battle->escapeHeld) {
                    btl_setIdleMotion(u);
                }
                u->escapeStance = g_gameState->gameData->ctx.battle->escapeHeld;
            }
            *ot = (s32)btl_drawShadow(g_gameState->gameData->ctx.battle->work->shadows, (u->flags >> 1) & 0xF, 0, id, (u8 *)*ot);
        } else if (u->koState < 4) {
            *ot = (s32)btl_drawShadow(g_gameState->gameData->ctx.battle->work->shadows, ((u->flags >> 1) & 0xF) + 9, 0, id, (u8 *)*ot);
        }
    }
}

s32 btl_setIdleMotion(BtlUnit *u) {
    GameData *g = g_gameState->gameData;
    Battle *b = g->ctx.battle;
    BtlCmd *cmd = b->curCmd;
    s32 cur;
    s32 *a;
    BtlUnit9C *m;

    if (u->status & BTL_STATUS_KO) {
        if ((u->flags & 0x20001) == 1 && u != cmd->caster && u->koState == 0) {
            cur = (s32)u->model->motion;
            if (cur != u->motions[4] && cur != u->motions[9]) {
                u->model->motion = (BtlUnit9CC *)u->motions[(u->flags >> 18) & 1];
            }
        }
        return 0;
    }
    if ((u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) || ((u->flags >> 17) & 1)
        || ((u->flags & 1) && g->characters[(u->flags >> 1) & 0xF].info.b.kind == 2 && cmd->cmdNo == BTL_CMD_SWD_MAG)) {
        return 0;
    }
    {
        if (u == cmd->caster && (u32)(cmd->cmdNo - 0x37) >= 7) {
            cur = (s32)u->model->motion;
            if (cur == u->motions[10] || cur == u->motions[11]) {
                u->model->motion = (BtlUnit9CC *)u->motions[9];
            }
            return 0;
        }
        if (u->flags & 1) {
            if (!(u->flags & 0x06000000)) {
                if ((b->escapeHeld != 0 || (b->cmdFlags & 1)) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
                    u->model->motion = (BtlUnit9CC *)u->motions[17];
                } else if (u->status & BTL_STATUS_DEFEND) {
                    u->model->motion = (BtlUnit9CC *)u->motions[13];
                } else if ((u->flags >> 19) & 1) {
                    if ((s32)u->model->motion == u->motions[(u->flags >> 18) & 1]) {
                        u->model->motion = (BtlUnit9CC *)u->motions[((u->flags >> 18) & 1) + 10];
                    } else {
                        u->model->motion = (BtlUnit9CC *)u->motions[9];
                    }
                } else {
                    m = u->model;
                    a = u->motions;
                    cur = (s32)m->motion;
                    if (cur == a[0] && (u->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP))) {
                        m->motion = (BtlUnit9CC *)a[7];
                    } else if ((cur == a[1] || cur == a[6]) && !(u->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP))) {
                        m->motion = (BtlUnit9CC *)a[5];
                    } else {
                        u->model->motion = (BtlUnit9CC *)u->motions[(u->flags >> 18) & 1];
                    }
                }
            }
        } else {
            u->model->motion = (BtlUnit9CC *)u->motions[(u->flags >> 18) & 1];
        }
        u->obj->frame = 0;
        return 1;
    }
}

void btl_startHitMotion(BtlUnit *arg0) {
    u32 f;
    if (arg0->popupFlags & 4) {
        arg0->pos[2] += (arg0->obj->rotY == 0) ? 0xC80000 : 0xFF380000;
    }
    f = arg0->flags;
    if (f & 1) {
        if ((g_gameState->gameData->ctx.battle->curCmd->ability->unkB & 0x81) == 0x81) {
            arg0->model->motion = (BtlUnit9CC *)arg0->motions[3];
        } else {
            arg0->model->motion = (BtlUnit9CC *)arg0->motions[2];
        }
    } else if (arg0->hp == 0 && (u8)(g_gameState->gameData->ctx.battle->enemies + ((f >> 1) & 0xF))->flags >> 7) {
        arg0->model->motion = (BtlUnit9CC *)arg0->motions[3];
        btl_playDeathCry(arg0, g_gameState->gameData->ctx.battle->enemies[(arg0->flags >> 1) & 0xF].type->deathCry);
    } else {
        if ((arg0->flags >> 23) & 1) {
            arg0 = btl_findBodyUnit();
        }
        arg0->model->motion = (BtlUnit9CC *)arg0->motions[((arg0->flags >> 18) & 1) + 2];
    }
    arg0->flags |= 0x20000;
    arg0->obj->frame = 0;
}

void btl_endPartyHitMotion(BtlUnit *arg0) {
    BtlUnit9C *m = arg0->model;
    s32 *a = arg0->motions;
    s32 c = (s32)m->motion;
    if (c == a[2]) {
        if (arg0->status & BTL_STATUS_KO) {
            arg0->koState = 1;
            arg0->flags &= ~0x20000;
        } else if ((arg0->flags >> 19) & 1) {
            m->motion = (BtlUnit9CC *)a[10];
        } else {
            arg0->flags &= ~0x20000;
        }
    } else if (c == a[3]) {
        if (arg0->status & BTL_STATUS_KO) {
            arg0->koState = 5;
            arg0->flags &= ~0x20000;
            arg0->model->motion = (BtlUnit9CC *)arg0->motions[4];
        } else {
            m->motion = (BtlUnit9CC *)a[6];
        }
    } else if (c == a[6]) {
        if ((arg0->flags >> 19) & 1) {
            m->motion = (BtlUnit9CC *)a[11];
        } else if (arg0->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) {
            arg0->flags &= ~0x20000;
        } else {
            m->motion = (BtlUnit9CC *)a[5];
        }
    } else {
        arg0->flags &= ~0x20000;
    }
    arg0->obj->frame = 0;
}

void btl_endEnemyHitMotion(BtlUnit *arg0) {
    BtlUnit9C *m;
    s32 *a;
    arg0->flags &= ~0x20000;
    if (arg0->status & BTL_STATUS_KO) {
        m = arg0->model;
        a = arg0->motions;
        if ((s32)m->motion == a[3] && ((u8)(g_gameState->gameData->ctx.battle->enemies + ((arg0->flags >> 1) & 0xF))->flags >> 7)) {
            m->motion = (BtlUnit9CC *)a[4];
            arg0->obj->frame--;
            arg0->koState = 3;
            arg0->flags |= 0x200000;
            return;
        }
        if (!(((g_gameState->gameData->ctx.battle->enemies + ((arg0->flags >> 1) & 0xF))->flags >> 6) & 1) && !((arg0->flags >> 20) & 1)) {
            arg0->koState = 1;
            return;
        }
        arg0->model->motion = (BtlUnit9CC *)arg0->motions[(arg0->flags >> 18) & 1];
    }
}

void btl_updateKo(BtlUnit *u) {
    BtlEnemy *e;
    BtlUnit9C *m;
    u32 f;

    if (u->koState == 0) {
        return;
    }
    if (!(u->flags & 1)) {
        e = &g_gameState->gameData->ctx.battle->enemies[(u->flags >> 1) & 0xF];
        switch (u->koState) {
        case 1:
            if (!((u->flags >> 23) & 1)) {
                u->model->motion = (BtlUnit9CC *)u->motions[4 + ((u->flags >> 18) & 1)];
                u->obj->frame = 0;
                u->koState++;
            } else {
                u->koState = 5;
            }
            btl_playDeathCry(u, e->type->deathCry);
            return;
        case 2:
            if (u->obj->frame >= u->model->motion->frameCount - 1) {
                u->flags |= 0x200000;
                u->koState++;
            }
            return;
        case 3:
            btl_playUnitSound(u, 0x79, 0x7F);
            btl_playUnitSound(u, 0x78, 0x7F);
            u->koState++;
        case 4:
            btl_setUnitFade(u, e->flags & 0x3F);
            if ((e->flags & 0x3F) == 0) {
                u->koState = 5;
                return;
            }
            e->flags = (e->flags & ~0x3F) | (((e->flags & 0x3F) - 2) & 0x3F);
            return;
        case 5:
            if (btl_hasEnemyFlag(u, 8) != 0) {
                if (g_gameState->gameData->dragonKills < 9999) {
                    g_gameState->gameData->dragonKills++;
                }
            }
            if (g_gameState->gameData->battleResult != 4) {
                btl_addEnemyRewards(e->type);
            }
            u->koState++;
            if (g_gameState->gameData->ctx.battle->phase != BTL_PHASE_END ||
                (u32)(g_gameState->gameData->ctx.battle->subPhase - 2) >= 2) {
                btl_checkSideDefeated(u);
            }
            btl_unlinkUnit(u);
            return;
        }
    } else {
        /* padded: the first jump table is 8-aligned in its object */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (u->koState) {
        case 1:
            m = u->model;
            if ((s32)m->motion == u->motions[9]) {
                m->motion = (BtlUnit9CC *)u->motions[32];
                u->obj->frame = 0;
                return;
            }
            if ((s32)m->motion == u->motions[32] && u->obj->frame < m->motion->frameCount) {
                break;
            }
            u->koState = 2;
        case 2:
            if ((f = (u->flags >> 18) & 1) == 1) {
                u->model->motion = (BtlUnit9CC *)u->motions[8];
                u->koState = 4;
            } else {
                u->model->motion = (BtlUnit9CC *)u->motions[7];
                u->koState = 3;
            }
            u->obj->frame = 0;
            return;
        case 3:
            if (u->obj->frame >= u->model->motion->frameCount) {
                u->model->motion = (BtlUnit9CC *)u->motions[8];
                u->obj->frame = 0;
                u->koState++;
            }
            break;
        case 4:
            if (u->obj->frame < u->model->motion->frameCount) {
                break;
            }
        case 5:
            if ((u8)btl_setKoMotion(u)) {
                btl_checkSideDefeated(u);
            }
            break;
        }
    }
}

s32 btl_waitHitMotions(BtlCmd *arg0) {
    BtlUnit *u;
    s32 r;
    u32 bit = arg0->info.b.covered;
    do {
        r = 1;
        if (bit) {
            return r;
        }
    } while (0);
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if ((u->flags >> 17) & 1) {
            r = 0;
            if (u->obj->frame >= u->model->motion->frameCount) {
                u->pos[2] = u->home->pos[2];
                if (u->flags & 1) {
                    btl_endPartyHitMotion(u);
                } else {
                    btl_endEnemyHitMotion(u);
                }
                btl_setIdleMotion(u);
            }
        }
    }
    return r;
}
