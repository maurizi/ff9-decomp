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
                if (!(u->flags & 1) && (id = btl_getEntranceSound()) != 0xFFFF) {
                    btl_playUnitSound(u, id, 0x7F);
                    break;
                }
            }
            if (g_evtGlobalVars.b[EVT_GB_BATTLE_NO_HUD] == 0) {
                btlui_enable(1);
                btlui_setAtbEnabled(1);
            }
            n = b->scene.flags & 3;
            if (n == 0) {
                btl_showMessage(BTL_MSG_BACK_ATTACK);
            } else if (n == 1) {
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
        if (b->subPhase != 3) {
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
            g->battleFlags |= 0x40;
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

s32 btl_updateUnits(GameData *arg0, Battle *arg1) {
    BtlUnit *u = arg1->unitList;
    s32 r = 1;
    s32 sp = arg1->work->prims[arg0->drawBuf];
    BtlUnit9C *m;
    BtlUnit9C *n;
    u32 f;
    u32 g;
    for (; u != NULL; u = u->next) {
        if (!((u->flags >> 22) & 1)) {
            btl_updateUnitModel(u, &sp);
            if (arg1->phase == BTL_PHASE_ENTRANCE) {
                n = u->model;
                g = u->flags & ~0x200000;
                u->flags = g;
                if (u->obj->frame >= n->motion->frameCount) {
                    if (!(u->status & BTL_STATUS_KO)) {
                        n->motion = (BtlUnit9CC *)u->motions[(g >> 18) & 1];
                    }
                    u->obj->frame = 0;
                }
                if (!(u->status & BTL_STATUS_PETRIFY)) {
                    if ((s32)u->model->motion != u->motions[(u->flags >> 18) & 1] && (s32)u->model->motion != u->motions[4]) {
                        r = 0;
                    }
                }
            } else if (arg1->phase == BTL_PHASE_EXIT) {
                f = u->flags;
                if ((f & 1) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP)) && ((arg1->scene.flags >> 6) & 1) && (g_gameState->gameData->characters + ((f >> 1) & 0xF))->info.b.unkA) {
                    m = u->model;
                    if (u->obj->frame >= m->motion->frameCount) {
                        m->motion = (BtlUnit9CC *)u->motions[19];
                        u->obj->frame = 0;
                    }
                }
            }
            btl_updateUnitTint(u);
        }
        btl_updateKo(u);
    }
    if (arg1->phase == BTL_PHASE_EXIT_DEFEAT && !((arg1->scene.flags >> 4) & 1) && (u8)btl_playVictoryMusic(arg0, 0x1E, 6) == 0) {
        r = 0;
    }
    btl_drawScene(arg0, arg1, sp);
    return r;
}

void btl_victory(GameData *arg0, Battle *arg1) {
    s32 sp18;
    s32 done;
    BtlUnit *u;
    u32 gil;

    sp18 = arg1->work->prims[arg0->drawBuf];
    done = 1;
    if (g_btlfxEnv->state->phase != -1 || arg1->cmdQueue.next != NULL || arg1->curCmd != NULL) {
        done = 0;
    }
    for (u = arg1->unitList; u != NULL; u = u->next) {
        if (!((u->flags >> 22) & 1)) {
            btl_updateUnitModel(u, &sp18);
            switch (arg1->subPhase) {
            case BTL_END_VICTORY:
                btl_updateHpStatus(u);
                if ((!(u->flags & 1) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP))) || ((u->flags & 1) && (u->status & BTL_STATUS_KO) && u->model->motion != (BtlUnit9CC *)u->motions[4])) {
                    done = 0;
                }
                break;
            case BTL_END_DEFEAT:
                if ((u->flags & 1) && u->model->motion != (BtlUnit9CC *)u->motions[4] && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP))) {
                    done = 0;
                }
                break;
            case BTL_END_STOPPED:
                btl_updateHpStatus(u);
                if (u->flags & 1) {
                    btl_removeStatuses(u, (BTL_STATUS_CONFUSE | BTL_STATUS_STOP | BTL_STATUS_DEFEND | BTL_STATUS_FREEZE));
                }
                if (arg1->cmdPhase != 0) {
                    done = 0;
                }
                if (u->status & BTL_STATUS_KO) {
                    if (u->koState == 0 && btl_isCmdPending(u->koCmd) == 0) {
                        btl_setCmd(u->koCmd, BTL_CMD_KO, 0, u->unitBit, 0);
                    }
                    if (u->koState != 6) {
                        done = 0;
                    }
                } else if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_STOP | BTL_STATUS_FREEZE))) {
                    if (u->model->motion != (BtlUnit9CC *)u->motions[0] &&
                        u->model->motion != (BtlUnit9CC *)u->motions[1]) {
                        done = 0;
                    }
                }
                break;
            case BTL_END_ESCAPE:
                if (u->status & BTL_STATUS_KO) {
                    if (u->koState == 0) {
                        u->koState = 1;
                    }
                    if (u->model->motion != (BtlUnit9CC *)u->motions[4]) {
                        done = 0;
                    } else if (btl_isCmdQueued(u, BTL_CMD_AUTO_LIFE) != 0) {
                        done = 0;
                    }
                } else if ((u->flags & 1) && !(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_ZOMBIE | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_SLEEP | BTL_STATUS_FREEZE | BTL_STATUS_JUMP))) {
                    u->pos[2] += 0xFF9C0000;
                    btl_setUnitBrightness(u, (s8)arg1->fleeBrightness);
                    if ((s8)arg1->fleeBrightness <= 0) {
                        u->flags |= 0x400000;
                    } else {
                        done = 0;
                        if ((s8)arg1->fleeBrightness == 0x20) {
                            btl_soundCmd(0xD000, 0xB5A, 0, 0x80, 0x7F);
                            btl_soundCmd(0xD000, 0xB5B, 0, 0x80, 0x7F);
                            btl_soundCmd(0xD000, 0xB5C, 0, 0x80, 0x7F);
                            arg1->fleeBrightness -= 2;
                        }
                    }
                }
                break;
            }
            btl_updateUnitTint(u);
        }
        btl_updateKo(u);
    }
    if (arg1->subPhase == BTL_END_ESCAPE && (s8)arg1->fleeBrightness < 0x20) {
        arg1->fleeBrightness -= 2;
    }
    if (done) {
        btlui_enable(0);
        switch (arg1->subPhase) {
        case BTL_END_VICTORY:
        case BTL_END_SCRIPT_VICTORY:
            arg0->battleFlags |= 0x40;
            arg0->battleResult = 1;
            if ((arg1->scene.flags >> 6) & 1) {
                if ((u8)btl_playVictoryMusic(arg0, 0x1E, 5) == 0) {
                    break;
                }
                for (u = arg1->unitList; u != NULL; u = u->next) {
                    if (u->flags & 1) {
                        btl_removeStatuses(u, (BTL_STATUS_CONFUSE | BTL_STATUS_BERSERK | BTL_STATUS_STOP | BTL_STATUS_AUTO_LIFE | BTL_STATUS_DEFEND | BTL_STATUS_POISON | BTL_STATUS_SLEEP | BTL_STATUS_REGEN | BTL_STATUS_HASTE | BTL_STATUS_SLOW | BTL_STATUS_FLOAT | BTL_STATUS_SHELL | BTL_STATUS_PROTECT | BTL_STATUS_HEAT | BTL_STATUS_FREEZE | BTL_STATUS_VANISH | BTL_STATUS_DOOM | BTL_STATUS_MINI | BTL_STATUS_REFLECT | BTL_STATUS_GRADUAL_PETRIFY));
                        if (!(u->status & (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP))) {
                            if (u->hp != 0) {
                                u->rot[1] = u->obj->rotY = (u->flags & 1) * 0x800;
                                u->flags = (u->flags & ~0x40000) | ((btl_hasStatus(u, (BTL_STATUS_VENOM | BTL_STATUS_LOW_HP | BTL_STATUS_POISON | BTL_STATUS_SLEEP)) & 1) << 18);
                                u->flags &= ~0x80000;
                                if (((g_gameState->gameData->characters + ((u->flags >> 1) & 0xF))->info.w >> 10) & 1) {
                                    u->model->motion = (BtlUnit9CC *)u->motions[0x12];
                                } else {
                                    u->model->motion = (BtlUnit9CC *)u->motions[(u->flags >> 18) & 1];
                                }
                                u->obj->frame = 0;
                            } else {
                                btl_addStatus(u, BTL_STATUS_KO);
                            }
                        }
                    }
                }
                btlcam_playScene(btl_getSceneData(2));
            } else if ((arg1->scene.flags >> 14) & 1) {
                arg0->battleFlags |= 8;
            }
            arg1->phase = BTL_PHASE_EXIT;
            break;
        case BTL_END_DEFEAT:
            arg0->battleResult = 3;
            arg1->phase = BTL_PHASE_EXIT_DEFEAT;
            break;
        case BTL_END_STOPPED:
            if ((arg1->scene.flags & 0x4040) == 0x4000) {
                arg0->battleFlags |= 8;
            }
            arg1->phase = BTL_PHASE_SCRIPT;
            btl_clearCmdQueue(arg1);
            btl_initCmdQueue(arg1);
            for (u = arg1->unitList; u != NULL; u = u->next) {
                btl_initUnitCmds(u);
                if (!(u->flags & 1)) {
                    u->obj->rotY = 0;
                    u->rot[1] = 0;
                }
            }
            break;
        case BTL_END_ESCAPE:
            gil = g_btlRewards.gil;
            arg0->battleFlags |= 0x40;
            for (u = arg1->unitList; u != NULL; u = u->next) {
                if (!(u->flags & 1)) {
                    gil += g_gameState->gameData->ctx.battle->enemies[(u->flags >> 1) & 0xF].type->gil;
                }
            }
            if ((arg1->scene.flags & 0x4040) == 0x4000) {
                arg0->battleFlags |= 8;
            }
            arg0->battleResult = 4;
            arg1->phase = BTL_PHASE_EXIT_OTHER;
            btl_clearRewards();
            if (arg0->battleFlags & 4) {
                gil /= 10;
                if (gil < arg0->gil) {
                    arg0->gil -= gil;
                } else {
                    gil = arg0->gil;
                    arg0->gil = 0;
                }
                btl_showMessage(BTL_MSG_DROPPED_GIL, gil);
            } else if ((u8)btl_partyHasSupport(1, 0x4000) != 0) {
                g_btlRewards.fleeGil = 1;
                g_btlRewards.gil = gil / 10;
            }
            break;
        }
        if (arg1->phase != BTL_PHASE_END) {
            if (arg1->phase == BTL_PHASE_EXIT && !((arg1->scene.flags >> 6) & 1)) {
                arg1->subPhase = 1;
            } else {
                arg1->subPhase = 0;
            }
        }
    } else {
        btl_processCmdQueue(arg1);
    }
    btl_drawScene(arg0, arg1, sp18);
}

void btl_updateIntro(GameData *arg0, Battle *arg1) {
    u32 *ot;
    BtlUnit *u;
    u8 f;
    u8 g;

    ot = (u32 *)arg1->work->prims[arg0->drawBuf];
    btl_loadModelsStep(0);
    if ((arg1->flags & 2) && (*(u32 *)&arg1->fleeBrightness & 0x2800) == 0x800) {
        btl_makeEnemyGreyCluts();
        arg1->loadFlags |= 0x20;
    } else if ((arg1->flags & 4) && (*(u32 *)&arg1->fleeBrightness & 0x5000) == 0x1000) {
        btl_makePartyGreyCluts();
        arg1->loadFlags |= 0x40;
    }
    for (u = arg1->unitList; u != NULL; u = u->next) {
        if ((u->flags >> 22) & 1) {
            continue;
        }
        btl_updateUnitModel(u, (s32 *)&ot);
        if (u->flags & 1) {
            btl_setUnitBrightness(u, (s8)arg1->partyBrightness);
        } else if (!((u->flags >> 23) & 1)) {
            btl_setUnitBrightness(u, (s8)arg1->enemyBrightness);
        }
        btl_updateUnitTint(u);
    }
    if (arg1->loadFlags & 2) {
        btlseq_update();
    }
    if (arg1->flags & 1) {
        ot = (u32 *)btlbg_draw(&arg0->viewMatrix, arg1->work->bgData, (s32)ot, arg0->ot[arg0->drawBuf],
                                  arg1->work->ot[arg0->drawBuf]);
    }
    ot = (u32 *)btl_drawBanner((u8 *)ot);
    if (arg1->flags & 2) {
        f = arg1->loadFlags;
        if (!(f & 2)) {
            if (!(f & 8)) {
                btl_saveEnemyCluts(ot);
                btl_initEnemies(arg1);
                ot = btl_initEnemyState(arg1, ot);
            } else if (f & 0x20) {
                if ((s8)arg1->enemyBrightness >= 0x20) {
                    arg1->loadFlags = f | 2;
                } else {
                    arg1->enemyBrightness += 4;
                }
            }
        }
    }
    if (arg1->flags & 4) {
        g = arg1->loadFlags;
        if (!(g & 4)) {
            if (!(g & 0x10)) {
                btl_savePartyCluts(ot);
                btl_initParty(arg1);
            } else if (g & 0x40) {
                if ((s8)arg1->partyBrightness >= 0x20) {
                    arg1->loadFlags = g | 4;
                } else {
                    arg1->partyBrightness += 4;
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
    v = 7;
    if (gd->darkBattle != 0) {
        v = 8;
    }
    gd->characters[3].info.w = (gd->characters[3].info.w & ~0x1F0) | (v << 4);
    g_gameState->gameData->battleResult = 0;
    btl_initState();
    btlfx_initAll();
    btlcam_init();
    btl_clearPopups();
    btl_initBanner();
}
