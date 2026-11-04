#include "ovl_0b.h"

void btl_initEquipElements(Character *chr, BtlUnit *unit) {
    s32 i;

    unit->elemBoost = 0;
    unit->elemWeak = 0;
    unit->elemHalf = 0;
    unit->elemAbsorb = 0;
    unit->elemGuard = 0;
    for (i = 0; i < EQUIP_SLOT_COUNT; i++) {
        if (chr->equip[i] != ITEM_NONE) {
            btl_orElements(&unit->elemGuard, g_equipStatBonus[g_itemTable[chr->equip[i]].statBonus].elemGuard);
            unit->elemBoost |= g_equipStatBonus[g_itemTable[chr->equip[i]].statBonus].elemBoost;
        }
    }
}

void btl_getWeaponOffset(Unk800BC1E0 *weapon, s16 *out) {
    if (!(weapon->flags & BTL_WEAPON_REACH_X)) {
        out[0] = 0;
        out[1] = weapon->reach;
    } else {
        out[0] = weapon->reach;
        out[1] = 0;
    }
    out[2] = weapon->offsetZ;
}

void btl_updateUnitModel(BtlUnit *unit, s32 *ot) {
    BtlModel *model = unit->model;
    BtlUnitA0 *obj = unit->obj;
    BtlUnit *body;
    SVECTOR rotCopy;
    SVECTOR rot;
    s32 i;
    s32 meshCount;
    s32 rotKeysLo;
    BtlMotion *motion;
    s32 shadowBones;
    BtlModel *weapon;

    if ((unit->flags >> BTL_UNIT_MODEL_PART_BIT) & 1) {
        body = btl_findBodyUnit();
        unit->rot[0] = body->rot[0];
        unit->rot[1] = body->rot[1];
        unit->rot[2] = body->rot[2];
        unit->home->pos[0] = PSX_NULLABLE(unit->model->bones)[unit->baseBone].tx << 16;
        unit->home->pos[1] = 0;
        unit->home->pos[2] = PSX_NULLABLE(unit->model->bones)[unit->baseBone].tz << 16;
        unit->pos[0] = unit->home->pos[0];
        unit->pos[1] = unit->home->pos[1];
        unit->pos[2] = unit->home->pos[2];
        return;
    }
    if ((unit->flags & BTL_UNIT_PARTY) && (s32)model->motion == unit->motions[BTL_MOTION_FLEE]) {
        memset(&rot, 0, 8);
        rot.vx = unit->rot[0];
        rot.vy = 0x800;
        rot.vz = unit->rot[2];
        rotCopy = rot;
        math_rotMatrixZYX(&rotCopy, &model->matrix);
    } else {
        memset(&rot, 0, 8);
        rot.vx = unit->rot[0];
        rot.vy = unit->rot[1];
        rot.vz = unit->rot[2];
        rotCopy = rot;
        math_rotMatrixZYX(&rotCopy, &model->matrix);
    }
    model->matrix.t[0] = unit->pos[0] >> 16;
    model->matrix.t[1] = unit->pos[1] >> 16;
    model->matrix.t[2] = unit->pos[2] >> 16;
    model->prims = 0;
    if (obj->frame >= model->motion->frameCount) {
        if (!(u8)btl_setIdleMotion(unit)) {
            unit->obj->frame = 0;
        } else if (unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) {
            unit->obj->frame--;
        }
    }
    btl_applyModelScale(model);
    motion = model->motion;
    if (IS_OFFSET(motion->rotKeys)) {
        rotKeysLo = motion->rotKeysLo;
        motion->rotKeys = (s32)motion + motion->rotKeys;
        if (rotKeysLo != 0) {
            motion->rotKeysLo = (s32)model->motion + rotKeysLo;
        }
    }
    *ot = (s32)btl_calcBoneMatrices(model, obj->frame, (u8 *)*ot);
    if (!(unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) && !((unit->flags >> BTL_UNIT_MOTION_HELD_BIT) & 1)) {
        obj->frame++;
    }
    meshCount = model->header->meshCount;
    btl_updateModelVisibility(model);
    unit->model->flags &= ~BTL_MODEL_DRAWN;
    for (i = 0; i < meshCount; i++) {
        if (!(model->hiddenMeshes & (1 << i))) {
            btl_projectMesh(model, i);
            *ot = (s32)btl_drawMesh(model, i, (u8 *)*ot);
            unit->model->flags |= BTL_MODEL_DRAWN;
        }
    }
    if (!(unit->status & BTL_STATUS_JUMP)) {
        btl_updateTexAnims((TexAnimSet *)model->texAnims);
    }
    if (unit->weaponModel != NULL) {
        unit->weaponModel->prims = 0;
        *ot = (s32)btl_calcBoneMatrices(unit->weaponModel, 0, (u8 *)*ot);
        weapon = unit->weaponModel;
        meshCount = weapon->header->meshCount;
        btl_updateModelVisibility(weapon);
        unit->weaponModel->flags &= ~BTL_MODEL_DRAWN;
        for (i = 0; i < meshCount; i++) {
            if (!(unit->weaponModel->hiddenMeshes & (1 << i))) {
                btl_projectMesh(unit->weaponModel, i);
                *ot = (s32)btl_drawMesh(unit->weaponModel, i, (u8 *)*ot);
                unit->weaponModel->flags |= BTL_MODEL_DRAWN;
            }
        }
    }
    if ((unit->flags >> BTL_UNIT_SHADOW_BIT) & 1) {
        shadowBones = (unit->shadowDirBone << 8) | unit->shadowBone;
        if (unit->flags & BTL_UNIT_PARTY) {
            if (!(g_gameState->gameData->ctx.battle->cmdFlags & BTL_CMDF_ESCAPE)) {
                if (unit->escapeStance != g_gameState->gameData->ctx.battle->escapeHeld) {
                    btl_setIdleMotion(unit);
                }
                unit->escapeStance = g_gameState->gameData->ctx.battle->escapeHeld;
            }
            *ot = (s32)btl_drawShadow(g_gameState->gameData->ctx.battle->work->shadows, BTL_UNIT_INDEX(unit->flags), 0, shadowBones, (u8 *)*ot);
        } else if (unit->koState < BTL_KO_ENEMY_FADE) {
            *ot = (s32)btl_drawShadow(g_gameState->gameData->ctx.battle->work->shadows, BTL_UNIT_INDEX(unit->flags) + 9, 0, shadowBones, (u8 *)*ot);
        }
    }
}

s32 btl_setIdleMotion(BtlUnit *unit) {
    GameData *gd = g_gameState->gameData;
    Battle *battle = gd->ctx.battle;
    BtlCmd *cmd = battle->curCmd;
    s32 cur;
    s32 *motions;
    BtlModel *model;

    if (unit->status & BTL_STATUS_KO) {
        if ((unit->flags & (BTL_UNIT_PARTY | BTL_UNIT_HIT_MOTION)) == BTL_UNIT_PARTY && unit != PSX_NULLABLE(cmd)->caster && unit->koState == BTL_KO_NONE) {
            cur = (s32)unit->model->motion;
            if (cur != unit->motions[BTL_MOTION_KO] && cur != unit->motions[BTL_MOTION_READY]) {
                unit->model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
            }
        }
        return 0;
    }
    if ((unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP)) || ((unit->flags >> BTL_UNIT_HIT_MOTION_BIT) & 1)
        || ((unit->flags & BTL_UNIT_PARTY) && gd->characters[BTL_UNIT_INDEX(unit->flags)].info.b.kind == CHR_KIND_VIVI && PSX_NULLABLE(cmd)->cmdNo == BTL_CMD_SWD_MAG)) {
        return 0;
    }
    {
        if (unit == PSX_NULLABLE(cmd)->caster && (u32)(PSX_NULLABLE(cmd)->cmdNo - BTL_CMD_ENEMY_REACT) >= 7) {
            cur = (s32)unit->model->motion;
            if (cur == unit->motions[BTL_MOTION_IDLE_TO_READY] || cur == unit->motions[BTL_MOTION_WEAK_TO_READY]) {
                unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_READY];
            }
            return 0;
        }
        if (unit->flags & BTL_UNIT_PARTY) {
            if (!(unit->flags & (BTL_UNIT_COVERING | BTL_UNIT_DODGED))) {
                if ((battle->escapeHeld != 0 || (battle->cmdFlags & BTL_CMDF_ESCAPE)) && !(unit->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
                    unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_FLEE];
                } else if (unit->status & BTL_STATUS_DEFEND) {
                    unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_DEFEND_IDLE];
                } else if ((unit->flags >> BTL_UNIT_CMD_WAITING_BIT) & 1) {
                    if ((s32)unit->model->motion == unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1]) {
                        unit->model->motion = (BtlMotion *)unit->motions[((unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1) + BTL_MOTION_IDLE_TO_READY];
                    } else {
                        unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_READY];
                    }
                } else {
                    model = unit->model;
                    motions = unit->motions;
                    cur = (s32)model->motion;
                    if (cur == motions[BTL_MOTION_IDLE] && (unit->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP))) {
                        model->motion = (BtlMotion *)motions[BTL_MOTION_IDLE_TO_WEAK];
                    } else if ((cur == motions[BTL_MOTION_IDLE_WEAK] || cur == motions[BTL_MOTION_GET_UP]) && !(unit->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP))) {
                        model->motion = (BtlMotion *)motions[BTL_MOTION_WEAK_TO_IDLE];
                    } else {
                        unit->model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
                    }
                }
            }
        } else {
            unit->model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
        }
        unit->obj->frame = 0;
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
        if ((g_gameState->gameData->ctx.battle->curCmd->ability->flags2 & BTL_ABIL2_HEAVY_HIT) == BTL_ABIL2_HEAVY_HIT) {
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
    BtlModel *model = unit->model;
    s32 *motions = unit->motions;
    s32 cur = (s32)model->motion;
    if (cur == motions[BTL_MOTION_HIT]) {
        if (unit->status & BTL_STATUS_KO) {
            unit->koState = BTL_KO_START;
            unit->flags &= ~BTL_UNIT_HIT_MOTION;
        } else if ((unit->flags >> BTL_UNIT_CMD_WAITING_BIT) & 1) {
            model->motion = (BtlMotion *)motions[BTL_MOTION_IDLE_TO_READY];
        } else {
            unit->flags &= ~BTL_UNIT_HIT_MOTION;
        }
    } else if (cur == motions[BTL_MOTION_HIT_HEAVY]) {
        if (unit->status & BTL_STATUS_KO) {
            unit->koState = BTL_KO_PARTY_SET;
            unit->flags &= ~BTL_UNIT_HIT_MOTION;
            unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_KO];
        } else {
            model->motion = (BtlMotion *)motions[BTL_MOTION_GET_UP];
        }
    } else if (cur == motions[BTL_MOTION_GET_UP]) {
        if ((unit->flags >> BTL_UNIT_CMD_WAITING_BIT) & 1) {
            model->motion = (BtlMotion *)motions[BTL_MOTION_WEAK_TO_READY];
        } else if (unit->status & (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) {
            unit->flags &= ~BTL_UNIT_HIT_MOTION;
        } else {
            model->motion = (BtlMotion *)motions[BTL_MOTION_WEAK_TO_IDLE];
        }
    } else {
        unit->flags &= ~BTL_UNIT_HIT_MOTION;
    }
    unit->obj->frame = 0;
}

void btl_endEnemyHitMotion(BtlUnit *unit) {
    BtlModel *model;
    s32 *motions;
    unit->flags &= ~BTL_UNIT_HIT_MOTION;
    if (unit->status & BTL_STATUS_KO) {
        model = unit->model;
        motions = unit->motions;
        if ((s32)model->motion == motions[BTL_MOTION_HIT_HEAVY] && ((u8)(g_gameState->gameData->ctx.battle->enemies + BTL_UNIT_INDEX(unit->flags))->flags >> BTL_ENEMYF_DEATH_MOTION_BIT)) {
            model->motion = (BtlMotion *)motions[BTL_MOTION_KO];
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

void btl_updateKo(BtlUnit *unit) {
    BtlEnemy *enemy;
    BtlModel *model;
    u32 weak;

    if (unit->koState == BTL_KO_NONE) {
        return;
    }
    if (!(unit->flags & BTL_UNIT_PARTY)) {
        enemy = &g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)];
        switch (unit->koState) {
        case BTL_KO_START:
            if (!((unit->flags >> BTL_UNIT_MODEL_PART_BIT) & 1)) {
                unit->model->motion = (BtlMotion *)unit->motions[4 + ((unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1)];
                unit->obj->frame = 0;
                unit->koState++;
            } else {
                unit->koState = BTL_KO_ENEMY_REMOVE;
            }
            btl_playDeathCry(unit, enemy->type->deathCry);
            return;
        case BTL_KO_ENEMY_DYING:
            if (unit->obj->frame >= unit->model->motion->frameCount - 1) {
                unit->flags |= BTL_UNIT_MOTION_HELD;
                unit->koState++;
            }
            return;
        case BTL_KO_ENEMY_SOUND:
            btl_playUnitSound(unit, SE_ENEMY_DEATH_1, SND_VOL_MAX);
            btl_playUnitSound(unit, SE_ENEMY_DEATH_2, SND_VOL_MAX);
            unit->koState++;
        case BTL_KO_ENEMY_FADE:
            btl_setUnitFade(unit, enemy->flags & BTL_ENEMYF_FADE);
            if ((enemy->flags & BTL_ENEMYF_FADE) == 0) {
                unit->koState = BTL_KO_ENEMY_REMOVE;
                return;
            }
            enemy->flags = (enemy->flags & ~BTL_ENEMYF_FADE) | (((enemy->flags & BTL_ENEMYF_FADE) - 2) & BTL_ENEMYF_FADE);
            return;
        case BTL_KO_ENEMY_REMOVE:
            if (btl_hasEnemyFlag(unit, BTL_ENEMY_DRAGON) != 0) {
                if (g_gameState->gameData->dragonKills < RECORD_MAX) {
                    g_gameState->gameData->dragonKills++;
                }
            }
            if (g_gameState->gameData->battleResult != BATTLE_END_ESCAPE) {
                btl_addEnemyRewards(enemy->type);
            }
            unit->koState++;
            if (g_gameState->gameData->ctx.battle->phase != BTL_PHASE_END ||
                (u32)(g_gameState->gameData->ctx.battle->subPhase - BTL_END_STOPPED) >= 2) /* not STOPPED or ESCAPE */ {
                btl_checkSideDefeated(unit);
            }
            btl_unlinkUnit(unit);
            return;
        }
    } else {
        /* padded: the first jump table is 8-aligned in its object */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (unit->koState) {
        case BTL_KO_START:
            model = unit->model;
            if ((s32)model->motion == unit->motions[BTL_MOTION_READY]) {
                model->motion = (BtlMotion *)unit->motions[32];
                unit->obj->frame = 0;
                return;
            }
            if ((s32)model->motion == unit->motions[32] && unit->obj->frame < model->motion->frameCount) {
                break;
            }
            unit->koState = BTL_KO_PARTY_FALL;
        case BTL_KO_PARTY_FALL:
            if ((weak = (unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1) == 1) {
                unit->model->motion = (BtlMotion *)unit->motions[8];
                unit->koState = BTL_KO_PARTY_DOWN;
            } else {
                unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_IDLE_TO_WEAK];
                unit->koState = BTL_KO_PARTY_COLLAPSE;
            }
            unit->obj->frame = 0;
            return;
        case BTL_KO_PARTY_COLLAPSE:
            if (unit->obj->frame >= unit->model->motion->frameCount) {
                unit->model->motion = (BtlMotion *)unit->motions[8];
                unit->obj->frame = 0;
                unit->koState++;
            }
            break;
        case BTL_KO_PARTY_DOWN:
            if (unit->obj->frame < unit->model->motion->frameCount) {
                break;
            }
        case BTL_KO_PARTY_SET:
            if ((u8)btl_setKoMotion(unit)) {
                btl_checkSideDefeated(unit);
            }
            break;
        }
    }
}

s32 btl_waitHitMotions(BtlCmd *cmd) {
    BtlUnit *unit;
    s32 done;
    u32 covered = cmd->info.b.covered;
    do {
        done = 1;
        if (covered) {
            return done;
        }
    } while (0);
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != 0; unit = unit->next) {
        if ((unit->flags >> BTL_UNIT_HIT_MOTION_BIT) & 1) {
            done = 0;
            if (unit->obj->frame >= unit->model->motion->frameCount) {
                unit->pos[2] = unit->home->pos[2];
                if (unit->flags & BTL_UNIT_PARTY) {
                    btl_endPartyHitMotion(unit);
                } else {
                    btl_endEnemyHitMotion(unit);
                }
                btl_setIdleMotion(unit);
            }
        }
    }
    return done;
}
