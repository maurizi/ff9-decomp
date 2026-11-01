#include "ovl_0b.h"

s32 btl_update(void) {
    GameData *g = g_gameState->gameData;
    Battle *b = g->ctx.battle;
    BtlUnit *u;
    u16 id;
    s32 n;

    btlcam_update();
    switch (b->phase) {
    case BTL_PHASE_INTRO:
        btl_updateIntro(g, b);
        break;
    case BTL_PHASE_SCRIPT:
        btl_updateUnits(g, b);
        break;
    case BTL_PHASE_ENTRANCE:
        if ((u8)btl_updateUnits(g, b)) {
            b->phase = BTL_PHASE_COMBAT;
            b->partyBrightness = 0;
            b->tick &= 0xF;
            for (u = b->unitList; u != NULL; u = u->next) {
                if (!(u->flags & BTL_UNIT_PARTY) && (id = btl_getEntranceSound()) != 0xFFFF) {
                    btl_playUnitSound(u, id, SND_VOL_MAX);
                    break;
                }
            }
            if (g_evtGlobalVars.b[EVT_GB_BATTLE_NO_HUD] == 0) {
                btlui_enable(1);
                btlui_setAtbEnabled(1);
            }
            n = b->scene.flags & BTL_SCENE_AMBUSH;
            if (n == BTL_AMBUSH_BACK) {
                btl_showMessage(BTL_MSG_BACK_ATTACK);
            } else if (n == BTL_AMBUSH_PREEMPTIVE) {
                btl_showMessage(BTL_MSG_PREEMPTIVE);
            }
        }
        break;
    case BTL_PHASE_COMBAT:
        btl_updateCombat(g, b);
        break;
    case BTL_PHASE_END:
        btlui_setAtbEnabled(0);
        btl_victory(g, b);
        if (b->subPhase != BTL_END_ESCAPE) {
            b->escapeHeld = 0;
        }
        break;
    case BTL_PHASE_EXIT:
        btl_updateUnits(g, b);
        if (b->subPhase == 1) {
            fade_out(0x20);
            b->subPhase++;
        } else if (b->subPhase == 2) {
            if (b->exitTimer++ > 0x20) {
                return (u8)btl_saveParty(b);
            }
        }
        break;
    case BTL_PHASE_EXIT_DEFEAT:
    case BTL_PHASE_EXIT_OTHER:
        if ((u8)btl_updateUnits(g, b)) {
            g->battleFlags |= GD_BTL_EXIT_PRELOAD;
            if (b->subPhase == 0) {
                if (g->battleResult != 5) {
                    fade_out(0x20);
                }
                b->subPhase++;
            } else if (b->subPhase == 1) {
                if (b->exitTimer++ > 0x20) {
                    return (u8)btl_saveParty(b);
                }
            }
        }
        break;
    }
    evt_update();
    b->tick++;
    return 0;
}

s32 btl_updateUnits(GameData *gd, Battle *battle) {
    BtlUnit *u = battle->unitList;
    s32 done = 1;
    s32 prim = battle->work->prims[gd->drawBuf];
    BtlModel *m;
    BtlModel *n;
    u32 f;
    u32 g;
    for (; u != NULL; u = u->next) {
        if (!((u->flags >> BTL_UNIT_HIDDEN_BIT) & 1)) {
            btl_updateUnitModel(u, &prim);
            if (battle->phase == BTL_PHASE_ENTRANCE) {
                n = u->model;
                g = u->flags & ~BTL_UNIT_MOTION_HELD;
                u->flags = g;
                if (u->obj->frame >= n->motion->frameCount) {
                    if (!(u->status & BTL_STATUS_KO)) {
                        n->motion = (BtlMotion *)u->motions[(g >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
                    }
                    u->obj->frame = 0;
                }
                if (!(u->status & BTL_STATUS_PETRIFY)) {
                    if ((s32)u->model->motion != u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1] && (s32)u->model->motion != u->motions[BTL_MOTION_KO]) {
                        done = 0;
                    }
                }
            } else if (battle->phase == BTL_PHASE_EXIT) {
                f = u->flags;
                if ((f & BTL_UNIT_PARTY) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) && ((battle->scene.flags >> BTL_SCENE_VICTORY_BIT) & 1) && (g_gameState->gameData->characters + BTL_UNIT_INDEX(f))->info.b.victoryPose) {
                    m = u->model;
                    if (u->obj->frame >= m->motion->frameCount) {
                        m->motion = (BtlMotion *)u->motions[19];
                        u->obj->frame = 0;
                    }
                }
            }
            btl_updateUnitTint(u);
        }
        btl_updateKo(u);
    }
    if (battle->phase == BTL_PHASE_EXIT_DEFEAT && !((battle->scene.flags >> BTL_SCENE_NO_GAMEOVER_BIT) & 1) && (u8)btl_playVictoryMusic(gd, 0x1E, MUSIC_DEFEAT) == 0) {
        done = 0;
    }
    btl_drawScene(gd, battle, prim);
    return done;
}

void btl_victory(GameData *gd, Battle *battle) {
    s32 prim;
    s32 done;
    BtlUnit *u;
    u32 gil;

    prim = battle->work->prims[gd->drawBuf];
    done = 1;
    if (g_btlfxEnv->state->phase != BTLFX_PHASE_IDLE || battle->cmdQueue.next != NULL || battle->curCmd != NULL) {
        done = 0;
    }
    for (u = battle->unitList; u != NULL; u = u->next) {
        if (!((u->flags >> BTL_UNIT_HIDDEN_BIT) & 1)) {
            btl_updateUnitModel(u, &prim);
            switch (battle->subPhase) {
            case BTL_END_VICTORY:
                btl_updateHpStatus(u);
                if ((!(u->flags & BTL_UNIT_PARTY) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP))) || ((u->flags & BTL_UNIT_PARTY) && (u->status & BTL_STATUS_KO) && u->model->motion != (BtlMotion *)u->motions[BTL_MOTION_KO])) {
                    done = 0;
                }
                break;
            case BTL_END_DEFEAT:
                if ((u->flags & BTL_UNIT_PARTY) && u->model->motion != (BtlMotion *)u->motions[BTL_MOTION_KO] && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP))) {
                    done = 0;
                }
                break;
            case BTL_END_STOPPED:
                btl_updateHpStatus(u);
                if (u->flags & BTL_UNIT_PARTY) {
                    btl_removeStatuses(u, (BTL_STATUS_CONFUSE | BTL_STATUS_STOP | BTL_STATUS_DEFEND | BTL_STATUS_FREEZE));
                }
                if (battle->cmdPhase != 0) {
                    done = 0;
                }
                if (u->status & BTL_STATUS_KO) {
                    if (u->koState == BTL_KO_NONE && btl_isCmdPending(u->koCmd) == 0) {
                        btl_setCmd(u->koCmd, BTL_CMD_KO, 0, u->unitBit, 0);
                    }
                    if (u->koState != BTL_KO_DONE) {
                        done = 0;
                    }
                } else if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE))) {
                    if (u->model->motion != (BtlMotion *)u->motions[BTL_MOTION_IDLE] &&
                        u->model->motion != (BtlMotion *)u->motions[BTL_MOTION_IDLE_WEAK]) {
                        done = 0;
                    }
                }
                break;
            case BTL_END_ESCAPE:
                if (u->status & BTL_STATUS_KO) {
                    if (u->koState == BTL_KO_NONE) {
                        u->koState = BTL_KO_START;
                    }
                    if (u->model->motion != (BtlMotion *)u->motions[BTL_MOTION_KO]) {
                        done = 0;
                    } else if (btl_isCmdQueued(u, BTL_CMD_AUTO_LIFE) != 0) {
                        done = 0;
                    }
                } else if ((u->flags & BTL_UNIT_PARTY) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
                    u->pos[2] += 0xFF9C0000;
                    btl_setUnitBrightness(u, (s8)battle->fleeBrightness);
                    if ((s8)battle->fleeBrightness <= 0) {
                        u->flags |= BTL_UNIT_HIDDEN;
                    } else {
                        done = 0;
                        if ((s8)battle->fleeBrightness == 0x20) {
                            btl_soundCmd(SND_SE_PLAY, SE_FLEE_1, 0, SND_PAN_CENTER, SND_VOL_MAX);
                            btl_soundCmd(SND_SE_PLAY, SE_FLEE_2, 0, SND_PAN_CENTER, SND_VOL_MAX);
                            btl_soundCmd(SND_SE_PLAY, SE_FLEE_3, 0, SND_PAN_CENTER, SND_VOL_MAX);
                            battle->fleeBrightness -= 2;
                        }
                    }
                }
                break;
            }
            btl_updateUnitTint(u);
        }
        btl_updateKo(u);
    }
    if (battle->subPhase == BTL_END_ESCAPE && (s8)battle->fleeBrightness < 0x20) {
        battle->fleeBrightness -= 2;
    }
    if (done) {
        btlui_enable(0);
        switch (battle->subPhase) {
        case BTL_END_VICTORY:
        case BTL_END_SCRIPT_VICTORY:
            gd->battleFlags |= GD_BTL_EXIT_PRELOAD;
            gd->battleResult = BATTLE_END_VICTORY;
            if ((battle->scene.flags >> BTL_SCENE_VICTORY_BIT) & 1) {
                if ((u8)btl_playVictoryMusic(gd, 0x1E, MUSIC_VICTORY) == 0) {
                    break;
                }
                for (u = battle->unitList; u != NULL; u = u->next) {
                    if (u->flags & BTL_UNIT_PARTY) {
                        btl_removeStatuses(u, (BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_AUTO_LIFE | BTL_STATUS_DEFEND | BTL_STATUS_POISON | BTL_STATUS_SLEEP | BTL_STATUS_REGEN | BTL_STATUS_HASTE | BTL_STATUS_SLOW | BTL_STATUS_FLOAT | BTL_STATUS_SHELL | BTL_STATUS_PROTECT | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_VANISH | BTL_STATUS_DOOM | BTL_STATUS_MINI | BTL_STATUS_REFLECT | BTL_STATUS_GRADUAL_PETRIFY));
                        if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP))) {
                            if (u->hp != 0) {
                                u->rot[1] = u->obj->rotY = (u->flags & BTL_UNIT_PARTY) * 0x800;
                                u->flags = (u->flags & ~BTL_UNIT_WEAK_STANCE) | ((btl_hasStatus(u, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << BTL_UNIT_WEAK_STANCE_BIT);
                                u->flags &= ~BTL_UNIT_CMD_WAITING;
                                if (((g_gameState->gameData->characters + BTL_UNIT_INDEX(u->flags))->info.w >> 10) & 1) {
                                    u->model->motion = (BtlMotion *)u->motions[0x12];
                                } else {
                                    u->model->motion = (BtlMotion *)u->motions[(u->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
                                }
                                u->obj->frame = 0;
                            } else {
                                btl_addStatus(u, BTL_STATUS_KO);
                            }
                        }
                    }
                }
                btlcam_playScene(btl_getSceneData(2));
            } else if ((battle->scene.flags >> BTL_SCENE_KEEP_MUSIC_BIT) & 1) {
                gd->battleFlags |= GD_BTL_KEEP_MUSIC;
            }
            battle->phase = BTL_PHASE_EXIT;
            break;
        case BTL_END_DEFEAT:
            gd->battleResult = BATTLE_END_DEFEAT;
            battle->phase = BTL_PHASE_EXIT_DEFEAT;
            break;
        case BTL_END_STOPPED:
            if ((battle->scene.flags & (BTL_SCENE_KEEP_MUSIC | BTL_SCENE_VICTORY)) == BTL_SCENE_KEEP_MUSIC) {
                gd->battleFlags |= GD_BTL_KEEP_MUSIC;
            }
            battle->phase = BTL_PHASE_SCRIPT;
            btl_clearCmdQueue(battle);
            btl_initCmdQueue(battle);
            for (u = battle->unitList; u != NULL; u = u->next) {
                btl_initUnitCmds(u);
                if (!(u->flags & BTL_UNIT_PARTY)) {
                    u->obj->rotY = 0;
                    u->rot[1] = 0;
                }
            }
            break;
        case BTL_END_ESCAPE:
            gil = g_btlRewards.gil;
            gd->battleFlags |= GD_BTL_EXIT_PRELOAD;
            for (u = battle->unitList; u != NULL; u = u->next) {
                if (!(u->flags & BTL_UNIT_PARTY)) {
                    gil += g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(u->flags)].type->gil;
                }
            }
            if ((battle->scene.flags & (BTL_SCENE_KEEP_MUSIC | BTL_SCENE_VICTORY)) == BTL_SCENE_KEEP_MUSIC) {
                gd->battleFlags |= GD_BTL_KEEP_MUSIC;
            }
            gd->battleResult = BATTLE_END_ESCAPE;
            battle->phase = BTL_PHASE_EXIT_OTHER;
            btl_clearRewards();
            if (gd->battleFlags & GD_BTL_FLED) {
                gil /= 10;
                if (gil < gd->gil) {
                    gd->gil -= gil;
                } else {
                    gil = gd->gil;
                    gd->gil = 0;
                }
                btl_showMessage(BTL_MSG_DROPPED_GIL, gil);
            } else if ((u8)btl_partyHasSupport(1, SA_BIT(SA_FLEE_GIL)) != 0) {
                g_btlRewards.fleeGil = 1;
                g_btlRewards.gil = gil / 10;
            }
            break;
        }
        if (battle->phase != BTL_PHASE_END) {
            if (battle->phase == BTL_PHASE_EXIT && !((battle->scene.flags >> BTL_SCENE_VICTORY_BIT) & 1)) {
                battle->subPhase = 1;
            } else {
                battle->subPhase = 0;
            }
        }
    } else {
        btl_processCmdQueue(battle);
    }
    btl_drawScene(gd, battle, prim);
}

void btl_updateIntro(GameData *gd, Battle *battle) {
    u32 *ot;
    BtlUnit *u;
    u8 enemyLoadFlags;
    u8 partyLoadFlags;

    ot = (u32 *)battle->work->prims[gd->drawBuf];
    btl_loadModelsStep(0);
    if ((battle->flags & BTL_FLAG_ENEMIES_LOADED) && (*(u32 *)&battle->fleeBrightness & ((BTL_LOAD_ENEMIES_INIT | BTL_LOAD_ENEMY_GREY) << 8)) == BTL_LOAD_ENEMIES_INIT << 8) {
        btl_makeEnemyGreyCluts();
        battle->loadFlags |= BTL_LOAD_ENEMY_GREY;
    } else if ((battle->flags & BTL_FLAG_PARTY_LOADED) && (*(u32 *)&battle->fleeBrightness & ((BTL_LOAD_PARTY_INIT | BTL_LOAD_PARTY_GREY) << 8)) == BTL_LOAD_PARTY_INIT << 8) {
        btl_makePartyGreyCluts();
        battle->loadFlags |= BTL_LOAD_PARTY_GREY;
    }
    for (u = battle->unitList; u != NULL; u = u->next) {
        if ((u->flags >> BTL_UNIT_HIDDEN_BIT) & 1) {
            continue;
        }
        btl_updateUnitModel(u, (s32 *)&ot);
        if (u->flags & BTL_UNIT_PARTY) {
            btl_setUnitBrightness(u, (s8)battle->partyBrightness);
        } else if (!((u->flags >> BTL_UNIT_MODEL_PART_BIT) & 1)) {
            btl_setUnitBrightness(u, (s8)battle->enemyBrightness);
        }
        btl_updateUnitTint(u);
    }
    if (battle->loadFlags & BTL_LOAD_ENEMIES_SHOWN) {
        btlseq_update();
    }
    if (battle->flags & BTL_FLAG_BG_LOADED) {
        ot = (u32 *)btlbg_draw(&gd->viewMatrix, battle->work->bgData, (s32)ot, gd->ot[gd->drawBuf],
                                  battle->work->ot[gd->drawBuf]);
    }
    ot = (u32 *)btl_drawBanner((u8 *)ot);
    if (battle->flags & BTL_FLAG_ENEMIES_LOADED) {
        enemyLoadFlags = battle->loadFlags;
        if (!(enemyLoadFlags & BTL_LOAD_ENEMIES_SHOWN)) {
            if (!(enemyLoadFlags & BTL_LOAD_ENEMIES_INIT)) {
                btl_saveEnemyCluts(ot);
                btl_initEnemies(battle);
                ot = btl_initEnemyState(battle, ot);
            } else if (enemyLoadFlags & BTL_LOAD_ENEMY_GREY) {
                if ((s8)battle->enemyBrightness >= 0x20) {
                    battle->loadFlags = enemyLoadFlags | BTL_LOAD_ENEMIES_SHOWN;
                } else {
                    battle->enemyBrightness += 4;
                }
            }
        }
    }
    if (battle->flags & BTL_FLAG_PARTY_LOADED) {
        partyLoadFlags = battle->loadFlags;
        if (!(partyLoadFlags & BTL_LOAD_PARTY_SHOWN)) {
            if (!(partyLoadFlags & BTL_LOAD_PARTY_INIT)) {
                btl_savePartyCluts(ot);
                btl_initParty(battle);
            } else if (partyLoadFlags & BTL_LOAD_PARTY_GREY) {
                if ((s8)battle->partyBrightness >= 0x20) {
                    battle->loadFlags = partyLoadFlags | BTL_LOAD_PARTY_SHOWN;
                } else {
                    battle->partyBrightness += 4;
                }
            }
        }
    }
}

void btl_initBattleState(void) {
    GameData *gd;
    s32 v;
    g_gameState->gameData->battleFlags = 0;
    gd = g_gameState->gameData;
    v = CHR_KIND_STEINER;
    if (gd->darkBattle != 0) {
        v = CHR_KIND_STEINER_DARK;
    }
    gd->characters[CHR_STEINER].info.w = (gd->characters[CHR_STEINER].info.w & ~CHR_INFO_KIND) | (v << CHR_INFO_KIND_SHIFT);
    g_gameState->gameData->battleResult = 0;
    btl_initState();
    btlfx_initAll();
    btlcam_init();
    btl_clearPopups();
    btl_initBanner();
}
