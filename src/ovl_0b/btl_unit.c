#include "ovl_0b.h"

void btl_initEquipElements(Character *c, BtlUnit *u) {
    s32 i;

    u->elemBoost = 0;
    u->elemWeak = 0;
    u->elemHalf = 0;
    u->elemAbsorb = 0;
    u->elemGuard = 0;
    for (i = 0; i < EQUIP_SLOT_COUNT; i++) {
        if (c->equip[i] != ITEM_NONE) {
            btl_orElements(&u->elemGuard, g_equipStatBonus[g_itemTable[c->equip[i]].statBonus].elemGuard);
            u->elemBoost |= g_equipStatBonus[g_itemTable[c->equip[i]].statBonus].elemBoost;
        }
    }
}

void btl_getWeaponOffset(Unk800BC1E0 *weapon, s16 *out) {
    if (!(weapon->flags & 8)) {
        out[0] = 0;
        out[1] = weapon->reach;
    } else {
        out[0] = weapon->reach;
        out[1] = 0;
    }
    out[2] = weapon->offsetZ;
}

void btl_updateUnitModel(BtlUnit *u, s32 *ot) {
    BtlModel *m = u->model;
    BtlUnitA0 *obj = u->obj;
    BtlUnit *body;
    SVECTOR rotCopy;
    SVECTOR rot;
    s32 i;
    s32 n;
    s32 rotKeysLo;
    BtlMotion *motion;
    s32 shadowBones;
    BtlModel *weapon;

    if ((u->flags >> BTL_UNIT_MODEL_PART_BIT) & 1) {
        body = btl_findBodyUnit();
        u->rot[0] = body->rot[0];
        u->rot[1] = body->rot[1];
        u->rot[2] = body->rot[2];
        u->home->pos[0] = u->model->bones[u->baseBone].tx << 16;
        u->home->pos[1] = 0;
        u->home->pos[2] = u->model->bones[u->baseBone].tz << 16;
        u->pos[0] = u->home->pos[0];
        u->pos[1] = u->home->pos[1];
        u->pos[2] = u->home->pos[2];
        return;
    }
    if ((u->flags & BTL_UNIT_PARTY) && (s32)m->motion == u->motions[BTL_MOTION_FLEE]) {
        memset(&rot, 0, 8);
        rot.vx = u->rot[0];
        rot.vy = 0x800;
        rot.vz = u->rot[2];
        rotCopy = rot;
        math_rotMatrixZYX(&rotCopy, &m->matrix);
    } else {
        memset(&rot, 0, 8);
        rot.vx = u->rot[0];
        rot.vy = u->rot[1];
        rot.vz = u->rot[2];
        rotCopy = rot;
        math_rotMatrixZYX(&rotCopy, &m->matrix);
    }
    m->matrix.t[0] = u->pos[0] >> 16;
    m->matrix.t[1] = u->pos[1] >> 16;
    m->matrix.t[2] = u->pos[2] >> 16;
    m->prims = 0;
    if (obj->frame >= m->motion->frameCount) {
        if (!(u8)btl_setIdleMotion(u)) {
            u->obj->frame = 0;
        } else if (u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) {
            u->obj->frame--;
        }
    }
    btl_applyModelScale(m);
    motion = m->motion;
    if (IS_OFFSET(motion->rotKeys)) {
        rotKeysLo = motion->rotKeysLo;
        motion->rotKeys = (s32)motion + motion->rotKeys;
        if (rotKeysLo != 0) {
            motion->rotKeysLo = (s32)m->motion + rotKeysLo;
        }
    }
    *ot = (s32)btl_calcBoneMatrices(m, obj->frame, (u8 *)*ot);
    if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) && !((u->flags >> BTL_UNIT_MOTION_HELD_BIT) & 1)) {
        obj->frame++;
    }
    n = m->header->meshCount;
    btl_updateModelVisibility(m);
    u->model->flags &= ~BTL_MODEL_DRAWN;
    for (i = 0; i < n; i++) {
        if (!(m->hiddenMeshes & (1 << i))) {
            btl_projectMesh(m, i);
            *ot = (s32)btl_drawMesh(m, i, (u8 *)*ot);
            u->model->flags |= BTL_MODEL_DRAWN;
        }
    }
    if (!(u->status & BTL_STATUS_JUMP)) {
        btl_updateTexAnims((TexAnimSet *)m->texAnims);
    }
    if (u->weaponModel != NULL) {
        u->weaponModel->prims = 0;
        *ot = (s32)btl_calcBoneMatrices(u->weaponModel, 0, (u8 *)*ot);
        weapon = u->weaponModel;
        n = weapon->header->meshCount;
        btl_updateModelVisibility(weapon);
        u->weaponModel->flags &= ~BTL_MODEL_DRAWN;
        for (i = 0; i < n; i++) {
            if (!(u->weaponModel->hiddenMeshes & (1 << i))) {
                btl_projectMesh(u->weaponModel, i);
                *ot = (s32)btl_drawMesh(u->weaponModel, i, (u8 *)*ot);
                u->weaponModel->flags |= BTL_MODEL_DRAWN;
            }
        }
    }
    if ((u->flags >> BTL_UNIT_SHADOW_BIT) & 1) {
        shadowBones = (u->shadowDirBone << 8) | u->shadowBone;
        if (u->flags & BTL_UNIT_PARTY) {
            if (!(g_gameState->gameData->ctx.battle->cmdFlags & BTL_CMDF_ESCAPE)) {
                if (u->escapeStance != g_gameState->gameData->ctx.battle->escapeHeld) {
                    btl_setIdleMotion(u);
                }
                u->escapeStance = g_gameState->gameData->ctx.battle->escapeHeld;
            }
            *ot = (s32)btl_drawShadow(g_gameState->gameData->ctx.battle->work->shadows, BTL_UNIT_INDEX(u->flags), 0, shadowBones, (u8 *)*ot);
        } else if (u->koState < BTL_KO_ENEMY_FADE) {
            *ot = (s32)btl_drawShadow(g_gameState->gameData->ctx.battle->work->shadows, BTL_UNIT_INDEX(u->flags) + 9, 0, shadowBones, (u8 *)*ot);
        }
    }
}

s32 btl_setIdleMotion(BtlUnit *u) {
    GameData *g = g_gameState->gameData;
    Battle *b = g->ctx.battle;
    BtlCmd *cmd = b->curCmd;
    s32 cur;
    s32 *motions;
    BtlModel *m;

    if (u->status & BTL_STATUS_KO) {
        if ((u->flags & (BTL_UNIT_PARTY | BTL_UNIT_HIT_MOTION)) == BTL_UNIT_PARTY && u != cmd->caster && u->koState == BTL_KO_NONE) {
            cur = (s32)u->model->motion;
            if (cur != u->motions[BTL_MOTION_KO] && cur != u->motions[BTL_MOTION_READY]) {
                u->model->motion = (BtlMotion *)u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
            }
        }
        return 0;
    }
    if ((u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) || ((u->flags >> BTL_UNIT_HIT_MOTION_BIT) & 1)
        || ((u->flags & BTL_UNIT_PARTY) && g->characters[BTL_UNIT_INDEX(u->flags)].info.b.kind == CHR_KIND_VIVI && cmd->cmdNo == BTL_CMD_SWD_MAG)) {
        return 0;
    }
    {
        if (u == cmd->caster && (u32)(cmd->cmdNo - BTL_CMD_ENEMY_REACT) >= 7) {
            cur = (s32)u->model->motion;
            if (cur == u->motions[BTL_MOTION_IDLE_TO_READY] || cur == u->motions[BTL_MOTION_WEAK_TO_READY]) {
                u->model->motion = (BtlMotion *)u->motions[BTL_MOTION_READY];
            }
            return 0;
        }
        if (u->flags & BTL_UNIT_PARTY) {
            if (!(u->flags & (BTL_UNIT_COVERING | BTL_UNIT_DODGED))) {
                if ((b->escapeHeld != 0 || (b->cmdFlags & BTL_CMDF_ESCAPE)) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
                    u->model->motion = (BtlMotion *)u->motions[BTL_MOTION_FLEE];
                } else if (u->status & BTL_STATUS_DEFEND) {
                    u->model->motion = (BtlMotion *)u->motions[BTL_MOTION_DEFEND_IDLE];
                } else if ((u->flags >> BTL_UNIT_CMD_WAITING_BIT) & 1) {
                    if ((s32)u->model->motion == u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1]) {
                        u->model->motion = (BtlMotion *)u->motions[((u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1) + BTL_MOTION_IDLE_TO_READY];
                    } else {
                        u->model->motion = (BtlMotion *)u->motions[BTL_MOTION_READY];
                    }
                } else {
                    m = u->model;
                    motions = u->motions;
                    cur = (s32)m->motion;
                    if (cur == motions[BTL_MOTION_IDLE] && (u->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP))) {
                        m->motion = (BtlMotion *)motions[BTL_MOTION_IDLE_TO_WEAK];
                    } else if ((cur == motions[BTL_MOTION_IDLE_WEAK] || cur == motions[BTL_MOTION_GET_UP]) && !(u->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP))) {
                        m->motion = (BtlMotion *)motions[BTL_MOTION_WEAK_TO_IDLE];
                    } else {
                        u->model->motion = (BtlMotion *)u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
                    }
                }
            }
        } else {
            u->model->motion = (BtlMotion *)u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
        }
        u->obj->frame = 0;
        return 1;
    }
}

void btl_startHitMotion(BtlUnit *unit) {
    u32 flags;
    if (unit->popupFlags & BTL_POPUP_CRITICAL) {
        unit->pos[2] += (unit->obj->rotY == 0) ? 0xC80000 : 0xFF380000;
    }
    flags = unit->flags;
    if (flags & BTL_UNIT_PARTY) {
        if ((g_gameState->gameData->ctx.battle->curCmd->ability->flags2 & 0x81) == 0x81) {
            unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_HIT_HEAVY];
        } else {
            unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_HIT];
        }
    } else if (unit->hp == 0 && (u8)(g_gameState->gameData->ctx.battle->enemies + BTL_UNIT_INDEX(flags))->flags >> BTL_ENEMYF_DEATH_MOTION_BIT) {
        unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_HIT_HEAVY];
        btl_playDeathCry(unit, g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type->deathCry);
    } else {
        if ((unit->flags >> BTL_UNIT_MODEL_PART_BIT) & 1) {
            unit = btl_findBodyUnit();
        }
        unit->model->motion = (BtlMotion *)unit->motions[((unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1) + BTL_MOTION_HIT];
    }
    unit->flags |= BTL_UNIT_HIT_MOTION;
    unit->obj->frame = 0;
}

void btl_endPartyHitMotion(BtlUnit *unit) {
    BtlModel *m = unit->model;
    s32 *motions = unit->motions;
    s32 cur = (s32)m->motion;
    if (cur == motions[BTL_MOTION_HIT]) {
        if (unit->status & BTL_STATUS_KO) {
            unit->koState = BTL_KO_START;
            unit->flags &= ~BTL_UNIT_HIT_MOTION;
        } else if ((unit->flags >> BTL_UNIT_CMD_WAITING_BIT) & 1) {
            m->motion = (BtlMotion *)motions[BTL_MOTION_IDLE_TO_READY];
        } else {
            unit->flags &= ~BTL_UNIT_HIT_MOTION;
        }
    } else if (cur == motions[BTL_MOTION_HIT_HEAVY]) {
        if (unit->status & BTL_STATUS_KO) {
            unit->koState = BTL_KO_PARTY_SET;
            unit->flags &= ~BTL_UNIT_HIT_MOTION;
            unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_KO];
        } else {
            m->motion = (BtlMotion *)motions[BTL_MOTION_GET_UP];
        }
    } else if (cur == motions[BTL_MOTION_GET_UP]) {
        if ((unit->flags >> BTL_UNIT_CMD_WAITING_BIT) & 1) {
            m->motion = (BtlMotion *)motions[BTL_MOTION_WEAK_TO_READY];
        } else if (unit->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) {
            unit->flags &= ~BTL_UNIT_HIT_MOTION;
        } else {
            m->motion = (BtlMotion *)motions[BTL_MOTION_WEAK_TO_IDLE];
        }
    } else {
        unit->flags &= ~BTL_UNIT_HIT_MOTION;
    }
    unit->obj->frame = 0;
}

void btl_endEnemyHitMotion(BtlUnit *unit) {
    BtlModel *m;
    s32 *motions;
    unit->flags &= ~BTL_UNIT_HIT_MOTION;
    if (unit->status & BTL_STATUS_KO) {
        m = unit->model;
        motions = unit->motions;
        if ((s32)m->motion == motions[BTL_MOTION_HIT_HEAVY] && ((u8)(g_gameState->gameData->ctx.battle->enemies + BTL_UNIT_INDEX(unit->flags))->flags >> BTL_ENEMYF_DEATH_MOTION_BIT)) {
            m->motion = (BtlMotion *)motions[BTL_MOTION_KO];
            unit->obj->frame--;
            unit->koState = BTL_KO_ENEMY_SOUND;
            unit->flags |= BTL_UNIT_MOTION_HELD;
            return;
        }
        if (!(((g_gameState->gameData->ctx.battle->enemies + BTL_UNIT_INDEX(unit->flags))->flags >> BTL_ENEMYF_SCRIPT_DEATH_BIT) & 1) && !((unit->flags >> BTL_UNIT_KILLED_BIT) & 1)) {
            unit->koState = BTL_KO_START;
            return;
        }
        unit->model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
    }
}

void btl_updateKo(BtlUnit *u) {
    BtlEnemy *e;
    BtlModel *m;
    u32 weak;

    if (u->koState == BTL_KO_NONE) {
        return;
    }
    if (!(u->flags & BTL_UNIT_PARTY)) {
        e = &g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(u->flags)];
        switch (u->koState) {
        case BTL_KO_START:
            if (!((u->flags >> BTL_UNIT_MODEL_PART_BIT) & 1)) {
                u->model->motion = (BtlMotion *)u->motions[4 + ((u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1)];
                u->obj->frame = 0;
                u->koState++;
            } else {
                u->koState = BTL_KO_ENEMY_REMOVE;
            }
            btl_playDeathCry(u, e->type->deathCry);
            return;
        case BTL_KO_ENEMY_DYING:
            if (u->obj->frame >= u->model->motion->frameCount - 1) {
                u->flags |= BTL_UNIT_MOTION_HELD;
                u->koState++;
            }
            return;
        case BTL_KO_ENEMY_SOUND:
            btl_playUnitSound(u, SE_ENEMY_DEATH_1, 0x7F);
            btl_playUnitSound(u, SE_ENEMY_DEATH_2, 0x7F);
            u->koState++;
        case BTL_KO_ENEMY_FADE:
            btl_setUnitFade(u, e->flags & BTL_ENEMYF_FADE);
            if ((e->flags & BTL_ENEMYF_FADE) == 0) {
                u->koState = BTL_KO_ENEMY_REMOVE;
                return;
            }
            e->flags = (e->flags & ~BTL_ENEMYF_FADE) | (((e->flags & BTL_ENEMYF_FADE) - 2) & BTL_ENEMYF_FADE);
            return;
        case BTL_KO_ENEMY_REMOVE:
            if (btl_hasEnemyFlag(u, BTL_ENEMY_DRAGON) != 0) {
                if (g_gameState->gameData->dragonKills < RECORD_MAX) {
                    g_gameState->gameData->dragonKills++;
                }
            }
            if (g_gameState->gameData->battleResult != BATTLE_END_ESCAPE) {
                btl_addEnemyRewards(e->type);
            }
            u->koState++;
            if (g_gameState->gameData->ctx.battle->phase != BTL_PHASE_END ||
                (u32)(g_gameState->gameData->ctx.battle->subPhase - BTL_END_STOPPED) >= 2) /* not STOPPED or ESCAPE */ {
                btl_checkSideDefeated(u);
            }
            btl_unlinkUnit(u);
            return;
        }
    } else {
        /* padded: the first jump table is 8-aligned in its object */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (u->koState) {
        case BTL_KO_START:
            m = u->model;
            if ((s32)m->motion == u->motions[BTL_MOTION_READY]) {
                m->motion = (BtlMotion *)u->motions[32];
                u->obj->frame = 0;
                return;
            }
            if ((s32)m->motion == u->motions[32] && u->obj->frame < m->motion->frameCount) {
                break;
            }
            u->koState = BTL_KO_PARTY_FALL;
        case BTL_KO_PARTY_FALL:
            if ((weak = (u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1) == 1) {
                u->model->motion = (BtlMotion *)u->motions[8];
                u->koState = BTL_KO_PARTY_DOWN;
            } else {
                u->model->motion = (BtlMotion *)u->motions[BTL_MOTION_IDLE_TO_WEAK];
                u->koState = BTL_KO_PARTY_COLLAPSE;
            }
            u->obj->frame = 0;
            return;
        case BTL_KO_PARTY_COLLAPSE:
            if (u->obj->frame >= u->model->motion->frameCount) {
                u->model->motion = (BtlMotion *)u->motions[8];
                u->obj->frame = 0;
                u->koState++;
            }
            break;
        case BTL_KO_PARTY_DOWN:
            if (u->obj->frame < u->model->motion->frameCount) {
                break;
            }
        case BTL_KO_PARTY_SET:
            if ((u8)btl_setKoMotion(u)) {
                btl_checkSideDefeated(u);
            }
            break;
        }
    }
}

s32 btl_waitHitMotions(BtlCmd *cmd) {
    BtlUnit *u;
    s32 done;
    u32 covered = cmd->info.b.covered;
    do {
        done = 1;
        if (covered) {
            return done;
        }
    } while (0);
    for (u = g_gameState->gameData->ctx.battle->unitList; u != 0; u = u->next) {
        if ((u->flags >> BTL_UNIT_HIT_MOTION_BIT) & 1) {
            done = 0;
            if (u->obj->frame >= u->model->motion->frameCount) {
                u->pos[2] = u->home->pos[2];
                if (u->flags & BTL_UNIT_PARTY) {
                    btl_endPartyHitMotion(u);
                } else {
                    btl_endEnemyHitMotion(u);
                }
                btl_setIdleMotion(u);
            }
        }
    }
    return done;
}
