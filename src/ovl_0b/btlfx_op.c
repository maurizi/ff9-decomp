#include "ovl_0b.h"

void btlfx_initCommonData(void) {
    u8 primBuf[0x40];
    RECT rect;
    g_btlfxCommonTable = g_btlfxCommonData;
    g_btlfxCommonInit = 1;
    g_btlfxEnv->prim = primBuf;
    btlfx_reloadCommonData(0);
    while (btlfx_reloadCommonData(1) == 0) {
    }
    rect.x = 0;
    rect.y = 0xF0;
    rect.w = 0x100;
    rect.h = 2;
    LoadImage(&rect, BTLFX_COMMON_CLUTS(u32 *));
    DrawSync(0);
    g_btlfxEnv->prim = 0;
}

s32 btlfx_reloadCommonData(s32 poll) {
    RECT rect;
    s32 ret = 0;
    s32 i;
    u32 *p = RAM_BTLFX_MODULE(u32 *);
    if (poll == 0) {
        FileEntry *file = file_find(DIR_COMMON, FILE_BTLFX_COMMON);
        cd_readAsync(g_gameState->imgSector + file->sector, (file->link - file->sector) << 11, RAM_BTLFX_MODULE(void *), 0);
    } else if (cd_getState() == 0) {
        btlfx_resetMaskBit();
        rect.w = 0x40;
        rect.h = 0x80;
        for (i = 0; i < BTLFX_COMMON_PAGES; i++) {
            rect.x = (i >> 1) * 64 + VRAM_BTLFX_COMMON_X;
            rect.y = (i & 1) * 128 + VRAM_BTLFX_COMMON_Y;
            LoadImage(&rect, p);
            p += BTLFX_COMMON_PAGE_WORDS;
        }
        ret = 1;
    }
    return ret;
}

s32 btlfx_stepForward(s32 unitId, s32 targetId, s32 frame) {
    s32 homePos[2];
    s32 stepPos[2];
    s32 pos[2];
    s32 rot[2];
    s32 frames;
    s32 ratio;
    btlfx_getUnitHomePos(unitId, (s16 *)homePos);
    pos[0] = homePos[0];
    pos[1] = homePos[1];
    if (targetId == 0xFF) {
        targetId = 0;
    }
    if (btlfx_isEnemyUnit(targetId) != 0) {
        btlfx_getUnitRot(unitId, (s16 *)rot);
        btlfx_pointAhead((s16 *)pos, (s16 *)rot, 400, (s16 *)stepPos);
        if (frame == 0) {
            btlfx_playUnitMotion(unitId, BTL_MOTION_STEP_FORWARD);
        }
        frames = btlfx_getMotionLength(unitId);
        if (frame < frames) {
            btlfx_applyFloatHeight(unitId, (s16 *)homePos);
            btlfx_applyFloatHeight(unitId, (s16 *)stepPos);
        }
        ratio = ((frame + 1) << 12) / frames;
        if (ratio > ONE) {
            ratio = ONE;
        }
        btlfx_lerpVec((s16 *)homePos, (s16 *)stepPos, ratio, (s16 *)pos);
        btlfx_setUnitPos(unitId, (s16 *)pos);
        return frame >= frames;
    }
    return 1;
}

void btlfx_turnUnitTowardFast(s32 unitId, s16 *targetPos, s32 startRot, s32 frame, s32 unusedFlag) {
    btlfx_getMotionLength(unitId);
    btlfx_turnUnitToward(unitId, targetPos, (s16 *)startRot, 2, 0, frame, 3);
}

s32 btlfx_moveUnitHome(s32 unitId, s32 frame, s16 *startPos, s32 stopped) {
    s16 homePos[4];
    s16 pos[4];
    s32 frames;
    s32 ratio;
    btlfx_getUnitHomePos(unitId, homePos);
    if (frame == 0) {
        btlfx_getUnitPos(unitId, startPos);
        if (startPos[0] == homePos[0] && startPos[2] == homePos[2]) {
            return -1;
        }
        if (stopped == 0) {
            btlfx_playUnitMotion(unitId, BTL_MOTION_STEP_HOME);
        }
        btlfx_turnUnitToward(unitId, homePos, 0, 0, 0, 0, 0);
    }
    if (stopped == 0) {
        stopped = btlfx_getMotionLength(unitId);
    } else {
        stopped = 0x18;
    }
    frames = stopped;
    if (frame < frames) {
        btlfx_applyFloatHeight(unitId, startPos);
        btlfx_applyFloatHeight(unitId, homePos);
    }
    ratio = ((frame + 1) << 12) / frames;
    if (ratio > ONE) {
        ratio = ONE;
    }
    btlfx_lerpVec(startPos, homePos, ratio, pos);
    btlfx_setUnitPos(unitId, pos);
    return frame >= frames;
}

u16 *btlfx_getCommonSprite(s32 index) {
    s32 *table = PSX_RAM(s32 *, 0x801E9700);
    return (u16 *)(table[index] + (s32)table);
}

s32 btlfx_getCommonData(s32 index) {
    return (s32)g_btlfxCommonTable + g_btlfxCommonTable[index];
}

s32 btlfx_opSetResArgsA(s32 mode, s32 work, s32 env, s32 entry, u8 *args) {
    switch (mode) {
    case 1:
        g_btlfxResArgs.mode = args[0];
        g_btlfxResArgs.data = args[4];
        break;
    }
    return 1;
}

s32 btlfx_opSetResArgsB(s32 mode, s32 work, s32 env, s32 entry, u8 *args) {
    if (mode == 1) {
        g_btlfxResArgs.offX = args[0];
        g_btlfxResArgs.offY = args[4];
    }
    return 1;
}

s32 btlfx_opSetFrameRate(s32 mode, s32 work, s32 env, s32 entry, s32 *args) {
    switch (mode) {
    case 1:
        btl_setVsyncMode(60 / *args);
        break;
    }
    return 1;
}

s32 btlfx_opBounce(s32 mode, Unk800E4634 *work, Unk800E4634Arg2 *env, s32 *entry, s32 *args) {
    s32 step;
    s32 i;

    if (mode == 0) {
        *args = 0x40;
        return 0;
    }
    if (mode == 1) {
        work->point = 0;
        work->timer = 0;
        if (env->targets->bounceCount == 0) {
            work->timer = 0xFFFF;
            if (btlfx_isEnemyUnit(BTLFX_UNIT_CASTER) != 0) {
                work->point = 0;
            } else {
                work->point = 12;
            }
        }
        btlfx_playBounceCamera();
        g_btlfxState.signals.b[2] = 0;
        {
            s32 j;
            for (j = 3; j >= 0; j--) {
                work->emitters[j] = NULL;
            }
        }
        return 0;
    }
    step = *entry;
    if (work->timer == 0xFFFF) {
        if (--work->point < 0) {
            g_btlfxState.signals.b[2] = 1;
            return 1;
        }
    } else if (work->timer != 0) {
        work->timer++;
        if ((work->timer >= 5 && work->point >= 0) || (work->timer >= 15 && work->point < 0)) {
            work->timer = 0;
            if (work->point < 0) {
                g_btlfxState.signals.b[2] = 1;
                g_btlfxState.castEnded = 1;
                return 1;
            }
        }
        for (i = 0; i < 4; i++) {
            if (work->emitters[i] != NULL) {
                work->emitters[i]->rot.vx = work->rot[0];
                work->emitters[i]->rot.vy = work->rot[1];
                work->emitters[i]->rot.vz = work->rot[2];
                if (work->emitters[i]->state == BTLFX_EMIT_DONE) {
                    work->emitters[i] = NULL;
                }
            }
        }
    } else if (btlfx_getBouncePos(work->point, (s16 *)&work->points[work->point], work->rot) == 0) {
        work->emitters[work->point] = btlfx_startEmitter(btlfx_getCommonData(BTLFX_COMMON_EMIT_BOUNCE), (s32)&work->points[work->point]);
        if (work->emitters[work->point] != NULL) {
            work->emitters[work->point]->otZ = 0x73;
            if (work->emitters[work->point] != NULL) {
                work->emitters[work->point]->elems[0].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_CAST_A);
                if (work->emitters[work->point] != NULL) {
                    work->emitters[work->point]->elems[1].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_CAST_B);
                }
            }
        }
        work->rot[0] = 0x400;
        work->timer = 1;
        work->rot[1] += 0x800;
        btlfx_playSoundSet(BTLFX_SNDSET_BOUNCE, work->point + 8);
        work->point++;
    } else {
        work->point = -1;
        work->timer = 1;
    }
    *entry = step;
    return 0;
}

void btlfx_startCasterTask(s32 casterKind) {
    s32 target = BTLFX_UNIT_NONE;
    s32 kindArg = casterKind * 16;
    if (g_btlfxEnv->targets->targetCount == 1) {
        target = 0;
    }
    btlfx_spawnTask(BTLFX_RES_CAST, -1, target, kindArg);
}

void btlfx_playSoundSet(s32 set, s32 unit) {
    s32 i;
    for (i = 0; i < 3; i++) {
        btlfx_playCommonSound(g_btlfxSoundSets[set][i], unit);
    }
}

Pool6CEntry *btlfx_startCastEmitter_def(s32 unitId, s32 kind, Unk800E4A3CArgs pos) {
    Pool6CEntry *emitter;
    Unk800E4A3CArgs *posRef;

    emitter = NULL;
    pos.b = pos.a;
    /* Read back through a pointer: CSE then keeps the load, which reload_cse turns into the original's
       `move v0,a2` for the 0x30 store (a direct `pos.c = pos.b` stores a2 twice). */
    posRef = &pos;
    pos.c = posRef->b;
    switch (kind) {
    case BTLFX_CASTK_WHITE:
    case BTLFX_CASTK_BLACK:
        emitter = btlfx_startEmitter(btlfx_getCommonData(BTLFX_COMMON_EMIT_CAST), (s32)pos.a);
        if (kind == BTLFX_CASTK_WHITE) {
            if (emitter != NULL) {
                emitter->elems[0].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_CAST_A);
                goto common;
            }
        } else {
            if (emitter != NULL) {
                emitter->elems[0].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_BLACK_CAST);
            common:
                if (emitter != NULL) {
                    emitter->elems[1].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_CAST_B);
                    emitter->otZ = 8;
                }
            }
        }
        if (btlfx_isUnitKind(BTLFX_UNIT_CASTER, 0) == 0) {
            goto zero;
        }
        btlfx_playSoundSet(BTLFX_SNDSET_CAST_EIKO, unitId);
        break;
    case BTLFX_CASTK_SUMMON:
    case BTLFX_CASTK_SUMMON_SHORT:
        pos.b[1] -= 0x40;
        emitter = btlfx_startEmitter(btlfx_getCommonData(BTLFX_COMMON_EMIT_SUMMON_CAST), (s32)pos.a);
        if (emitter != NULL) {
            emitter->elems[0].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_SUMMON_A);
            emitter->elems[1].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_CAST_B);
            emitter->elems[2].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_SUMMON_C);
            emitter->target = *(s32 *)&pos.b;
            emitter->otZ = 0x20;
        }
        btlfx_playSoundSet(BTLFX_SNDSET_SUMMON_CAST, unitId);
        break;
    case BTLFX_CASTK_ENEMY:
        emitter = btlfx_startEmitter(btlfx_getCommonData(BTLFX_COMMON_EMIT_ENEMY_CAST), (s32)pos.a);
        if (emitter != NULL) {
            emitter->elems[0].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_CAST_A);
            emitter->elems[1].tex = btlfx_getCommonData(BTLFX_COMMON_TEX_CAST_B);
            emitter->otZ = 0x80;
        }
    zero:
        btlfx_playSoundSet(BTLFX_SNDSET_CAST, unitId);
        break;
    }
    return emitter;
}

void btlfx_setPartyTexAnim(s32 unitId, s32 mode, s32 second) {
    if (btlfx_isEnemyUnit(unitId) == 0) {
        btlfx_setUnitTexAnim(unitId, mode, second);
    }
}

void btlfx_endCastIfNoBounce(void) {
    if (g_btlfxEnv->targets->bounceCount == 0) {
        g_btlfxState.castEnded = 1;
    }
}

s32 btlfx_opCast(s32 mode, Unk800E4C88 *work, Unk800F78A4 *env, s32 *entry, s32 *args) {
    s16 targetPos[4];
    s32 step;
    s32 noCast;
    s32 fade;
    s32 motion;
    s32 fadeStep;
    s32 *argp;
    s32 level;
    s32 enemy;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        argp = args;
        work->phase = 0;
        work->frame = 0;
        work->unit = (argp[1] & 0xF) + 0x10;
        work->target = argp[0];
        work->emitter = NULL;
        if (env->state->flags & BTLFX_HDR_CAMERA) {
            btl_setMarkersVisible(0);
        }
        if (env->targets->bounceCount != 0 && work->target != BTLFX_UNIT_NONE) {
            if (env->targets->bounceCount == 1) {
                work->target += 8;
            } else {
                work->target = 0xFF;
            }
        }
        if (env->targets->kind & BTL_FXK_SWD_MAG) {
            work->unit = BTLFX_UNIT_PARTNER;
            work->target = BTLFX_UNIT_CASTER;
        }
        work->kind = (u8)argp[1] >> 4;
        work->castMode = ((u32)g_btlfxState.flags & BTLFX_HDR_CAST_MODE) >> BTLFX_HDR_CAST_MODE_SHIFT;
        g_btlfxState.signals.b[2] = 0;
        g_btlfxState.signals.b[3] = 0;
        noCast = env->targets->kind & BTL_FXK_NO_CAST;
        enemy = btlfx_isEnemyUnit(work->unit);
        work->loopFrames = 100;
        btlfx_setPartyTexAnim(work->unit, 1, 0);
        if ((noCast != 0 || work->kind == BTLFX_CASTK_NONE) && enemy == 0) {
            work->motionMode = BTLFX_CAST_NONE;
            work->phase = 2;
        } else if (enemy != 0) {
            work->motionMode = BTLFX_CAST_ENEMY;
            work->phase = 2;
            if (noCast == 0 && work->kind != BTLFX_CASTK_NONE) {
                btlfx_getUnitBodyPos(work->unit, 0, work->pos);
                work->emitter = btlfx_startCastEmitter(work->unit, BTLFX_CASTK_ENEMY, work->pos, work->pos2);
            } else {
                work->emitter = NULL;
            }
        } else if (env->targets->kind & BTL_FXK_ITEM) {
            work->motionMode = BTLFX_CAST_ITEM;
            work->phase = 2;
        } else {
            work->motionMode = BTLFX_CAST_PARTY;
        }
    } else {
    step = *entry;
    switch (work->phase) {
    case 0:
        if (btlfx_stepForward(work->unit, work->target, step) != 0) {
            motion = work->motionMode;
            if (motion != BTLFX_CAST_ITEM) {
                btlfx_playUnitMotion(work->unit, BTL_MOTION_CAST_START);
                work->phase = 1;
                goto set_t;
            }
            btlfx_playUnitMotion(work->unit, BTL_MOTION_ITEM);
            work->phase = motion;
            btlfx_showCasterCmdName();
            goto set_t;
        }
        break;
    case 1:
        if (work->target != BTLFX_UNIT_NONE && g_btlfxState.effectId != BTLFX_EFFECT_ROULETTE) {
            btlfx_getUnitPos(work->target, targetPos);
            btlfx_turnUnitTowardFast(work->unit, targetPos, (s32)work->startRot, step, 0);
        }
        if (step >= btlfx_getMotionLength(work->unit) - 1) {
            btlfx_playUnitMotion(work->unit, BTL_MOTION_CAST_LOOP);
            step = -1;
            work->phase = 2;
            btlfx_getUnitPos(work->unit, work->pos);
            work->emitter = btlfx_startCastEmitter(work->unit, work->kind, work->pos, work->pos2);
            btlfx_setPartyTexAnim(work->unit, 0, 0);
            btlfx_showCasterCmdName();
        }
        break;
    case 2:
        if (work->emitter != NULL && work->emitter->state == BTLFX_EMIT_DONE) {
            work->emitter = NULL;
        }
        if (step == 0) {
            level = ((u32)g_btlfxState.flags & BTLFX_HDR_BG_FADE) >> BTLFX_HDR_BG_FADE_SHIFT;
            if (level != BTLFX_BG_FADE_NONE) {
                btlfx_startFadeBackground(level, work->kind == BTLFX_CASTK_SUMMON ? 0xC : 0x14);
            }
        }
        fadeStep = step - 9;
        if (work->kind == BTLFX_CASTK_SUMMON) {
            if (fadeStep >= 0) {
                if (fadeStep < 16) {
                    fade = (fadeStep * 255) >> 4;
                } else {
                    fade = 255;
                }
                if (fadeStep == 0) {
                    work->loopFrames = btlfx_getMotionLength(work->unit);
                    if (env->targets->kind & BTL_FXK_NO_CAST) {
                        btlfx_showCasterCmdName();
                    }
                }
                switch (work->castMode) {
                case 0:
                    btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, fade, fade, fade);
                    if (fadeStep == 16) {
                        btlfx_fadeUnitGroup(BTLFX_FADE_ALL, 0);
                    }
                    break;
                case 1:
                    btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, fade, fade, fade);
                    if (fadeStep == 16) {
                        btlfx_fadeUnitGroup(BTLFX_FADE_ALL, 0);
                    }
                    break;
                case 2:
                    btlfx_fadeUnitGroup(BTLFX_FADE_ALL, 0x80 - (fade >> 1));
                    break;
                case 3:
                    btlfx_fadeUnitGroup(BTLFX_FADE_PARTY, 0x80 - (fade >> 1));
                    break;
                }
            }
            if (step % work->loopFrames == 0 && step >= 25 && work->frame >= 27 && g_btlfxState.signals.b[2] != 0) {
                btlfx_stopEmitter(work->emitter);
                g_btlfxState.signals.b[3] = 1;
                g_btlfxState.castEnded = 1;
                env->camera->paused = 0;
                btlfx_setPartyTexAnim(work->unit, 2, 0);
                return 1;
            }
        } else if (work->motionMode == BTLFX_CAST_PARTY) {
            if (step % btlfx_getMotionLength(work->unit) == 0 && step > 0 && work->frame >= 27 && g_btlfxState.signals.b[2] != 0) {
                btlfx_playUnitMotion(work->unit, BTL_MOTION_CAST_END);
                step = -1;
                work->phase = 3;
            }
        } else if (work->motionMode == BTLFX_CAST_ENEMY) {
            if (g_btlfxState.signals.b[2] != 0 && work->frame >= 27) {
                work->phase = 3;
            set_t:
                step = -1;
            }
        } else if (work->motionMode == BTLFX_CAST_ITEM) {
            if (g_btlfxState.signals.b[2] != 0) {
                work->phase = 0;
                goto set_t;
            }
        } else if (g_btlfxState.signals.b[2] != 0) {
            g_btlfxState.signals.b[3] = 1;
            btlfx_endCastIfNoBounce();
            env->camera->paused = 0;
            btlfx_setPartyTexAnim(work->unit, 2, 0);
            btlfx_showCasterCmdName();
            return 1;
        }
        break;
    case 3:
        if (step == 0 && work->motionMode != BTLFX_CAST_ENEMY) {
            env->camera->paused = 0;
            btlfx_setPartyTexAnim(work->unit, 0, 1);
        }
        if (work->motionMode != BTLFX_CAST_ITEM) {
            if (work->emitter != NULL) {
                if (work->emitter->state == BTLFX_EMIT_DONE) {
                    work->emitter = NULL;
                }
                if (work->emitter != NULL && step < 5) {
                    level = (step << 12) >> 2;
                    work->emitter->alpha = ONE - level;
                    if (work->emitter != NULL) {
                        work->emitter->scale = level * 2 + ONE;
                    }
                    if (step != 4) {
                        goto skip;
                    }
                    btlfx_stopEmitter(work->emitter);
                }
            }
            g_btlfxState.signals.b[3] = 1;
            btlfx_endCastIfNoBounce();
        } else {
            g_btlfxState.signals.b[3] = 1;
            if (work->target != BTLFX_UNIT_NONE) {
                btlfx_getUnitPos(work->target, targetPos);
                btlfx_turnUnitTowardFast(work->unit, targetPos, (s32)work->startRot, step, 0);
            }
        }
    skip:
        if (work->motionMode == BTLFX_CAST_ENEMY) {
            if (step >= 5) {
                return 1;
            }
            break;
        }
        if (step >= btlfx_getMotionLength(work->unit) - 1) {
            if (work->motionMode == BTLFX_CAST_PARTY || work->motionMode == BTLFX_CAST_ITEM) {
                btlfx_playUnitMotion(work->unit, BTL_MOTION_READY);
            }
            btlfx_setPartyTexAnim(work->unit, 2, 0);
            g_btlfxState.signals.b[3] = 1;
            btlfx_endCastIfNoBounce();
            return 1;
        }
        break;
    }
    *entry = step;
    work->frame++;
    }
    return 0;
}

s32 btlfx_opReturnHome(s32 mode, Unk800E5538 *work, Unk800F78A4 *env, s32 *entry, s32 *args) {
    s32 step;

    if (mode == 0) {
        *args = 0x28;
        return 0;
    }
    if (mode == 1) {
        work->phase = 0;
        work->unit = *args;
        if (env->targets->kind & BTL_FXK_SWD_MAG) {
            work->unit = BTLFX_UNIT_PARTNER;
        }
        if (env->targets->kind & BTL_FXK_NO_CAST) {
            return 1;
        }
        if (btlfx_isEnemyUnit(work->unit) != 0) {
            return 1;
        }
        work->stopped = btl_hasStatus(btl_getUnit(work->unit), BTL_STATUS_STOP);
        return 0;
    }
    step = *entry;
    if (work->phase == 0) {
        if (btlfx_moveUnitHome(work->unit, step, work->startPos, work->stopped) != 0) {
            step = -1;
            work->phase = 1;
            if (btlfx_isCmdPendingOnAutoPotion(work->unit) != 0) {
                btlfx_playUnitMotion(work->unit, BTL_MOTION_READY);
                return 1;
            }
            if (work->stopped == 0) {
                btlfx_playUnitMotion(work->unit, BTL_MOTION_READY_TO_IDLE);
            }
        }
    } else {
        btlfx_turnUnitTowardFast(work->unit, NULL, (s32)work->startRot, step, 1);
        if (step >= btlfx_getMotionLength(work->unit) - 1) {
            btlfx_resetUnitMotion(work->unit, 1);
            return 1;
        }
    }
    *entry = step;
    return 0;
}

s32 btlfx_opSetResArgsC(s32 mode, s32 work, s32 env, s32 entry, u8 *args) {
    switch (mode) {
    case 1:
        g_btlfxResArgs2.unit = args[0];
        g_btlfxResArgs2.otBias = args[4];
        break;
    }
    return 1;
}

s32 btlfx_opSetSpriteClut(s32 mode, s32 work, s32 env, s32 entry, s32 *args) {
    s32 idx;
    s32 which;
    u16 *sprite;
    if (mode == 1) {
        idx = args[1] & BTLFX_SPRCLUT_INDEX;
        which = (args[1] & BTLFX_SPRCLUT_SLOT) >> BTLFX_SPRCLUT_SLOT_SHIFT;
        sprite = btlfx_getCommonSprite(args[0]);
        if (which >= 0) {
            if (which < 2) {
                sprite[1] = g_btlfxCommonCluts[idx];
            } else if (which < 4) {
                sprite[2] = g_btlfxCommonCluts[idx];
            }
        }
    }
    return 1;
}

s32 btlfx_opDrawOnUnit(s32 mode, Unk800E57B0 *work, Unk800F78A4 *env, s32 *entry, s32 *args) {
    SVECTOR rot;
    s16 targetPos[4];
    s32 param0;
    s32 param1;
    Pool6CEntry *emitter;
    u16 *sprite;

    if (mode == 0) {
        *args = 0x2C;
    } else if (mode == 1) {
        param0 = args[0];
        param1 = args[1];
        work->kind = g_btlfxResArgs.mode >> 6;
        work->scale = g_btlfxDrawScales[g_btlfxResArgs.mode & 0xF];
        work->unit = g_btlfxResArgs2.unit;
        work->otBias = g_btlfxResArgs2.otBias * 8;
        work->frame = 0;
        if (work->unit != BTLFX_UNIT_NONE) {
            btlfx_getUnitBodyPos(work->unit, 0, &work->pos.vx);
        } else {
            *(s32 *)&work->pos.vx = *(s32 *)&env->targets->centerX;
            *(s32 *)&work->pos.vz = *(s32 *)&env->targets->centerZ;
            work->pos.vy -= 0xC0;
        }
        switch (work->kind) {
        case BTLFX_DRAWON_PARTICLES:
            work->dataId = g_btlfxResArgs.data;
            work->colorIdx = param0;
            work->roll = (s8)param1 << 8;
            work->data = btlfx_getCommonSprite(work->dataId);
            ((u16 *)work->data)[1] = g_btlfxCommonCluts[work->colorIdx];
            work->frames = *(u16 *)work->data & BTLFX_SPRITE_FRAMES;
            work->offX = (s8)g_btlfxResArgs.offX * 4;
            work->offY = (s8)g_btlfxResArgs.offY * 4;
            break;
        case BTLFX_DRAWON_QUADS:
            work->dataId = g_btlfxResArgs.data;
            work->colorIdx = param0;
            work->roll = (s8)param1 << 8;
            work->data = btlfx_getCommonSprite(work->dataId);
            ((u16 *)work->data)[2] = g_btlfxCommonCluts[work->colorIdx];
            work->frames = *(u16 *)work->data & BTLFX_SPRITE_FRAMES;
            break;
        case BTLFX_DRAWON_EMITTER:
            work->emitterId = g_btlfxResArgs.data;
            work->dataId = param0;
            work->frames = g_btlfxResArgs.mode & 0xF;
            work->scale = param1;
            break;
        case BTLFX_DRAWON_RINGS:
            work->dataId = param0;
            work->frames = g_btlfxResArgs.data;
            work->data = btlfx_getCommonSprite(work->dataId);
            work->roll = 0;
            break;
        default:
            return 0;
        }
    } else {
        if (work->unit != BTLFX_UNIT_NONE) {
            env->otZ = btlfx_addRadiusSixth(work->unit, work->otBias);
            rot.vy = btlfx_compareDepth(BTLFX_UNIT_CASTER, work->unit) << 11;
            rot.pad = 0;
            rot.vx = 0;
            rot.vz = work->roll;
        }
        switch (work->kind) {
        case BTLFX_DRAWON_PARTICLES:
            btlfx_drawParticles(work->data, &work->pos, &rot, work->scale, work->frame, 1, ONE, work->offX, work->offY);
            break;
        case BTLFX_DRAWON_QUADS:
            btlfx_drawAnimQuads(work->data, &work->pos, &rot, work->scale, work->frame, 1, 0x1000);
            break;
        case BTLFX_DRAWON_EMITTER:
            if (work->frame == work->frames) {
                if (work->unit != BTLFX_UNIT_NONE) {
                    if (work->unit == BTLFX_UNIT_CASTER) {
                        btlfx_getWeaponPos(work->unit, 1, &work->pos.vx);
                        btlfx_getUnitBodyPos(0, 0, targetPos);
                        btlfx_aimAngles(&work->pos.vx, targetPos, work->rot);
                        if (btl_getUnit(0) == btl_getUnit(work->unit) && work->emitterId == 0x36) {
                            return 1;
                        }
                    } else {
                        btlfx_getUnitBodyPos(work->unit, 0, &work->pos.vx);
                    }
                }
                emitter = btlfx_startEmitter((s32)btlfx_getCommonSprite(work->emitterId), (s32)&work->pos);
                if (emitter != NULL) {
                    emitter->otZ = work->otBias;
                }
                sprite = btlfx_getCommonSprite(work->dataId);
                if (emitter != NULL) {
                    emitter->elems->tex = (s32)sprite;
                    emitter->elems->kind = work->scale;
                }
                emitter->elems->frameCount = *sprite & BTLFX_SPRITE_FRAMES; /* outside the NULL check in the original */
                work->data = emitter;
            }
            if (work->frames < work->frame) {
                emitter = work->data;
                if (work->unit == BTLFX_UNIT_CASTER && emitter != NULL) {
                    emitter->rot.vx = work->rot[0];
                    emitter->rot.vy = work->rot[1];
                    emitter->rot.vz = work->rot[2];
                }
                if (emitter->state == BTLFX_EMIT_DONE) {
                    return 1;
                }
            }
            goto next;
        case BTLFX_DRAWON_RINGS:
            btlfx_drawRings(work->data, &work->pos, &rot, work->scale, work->frame, 1, math_cos((work->frame << 10) / work->frames));
            break;
        default:
            env->otZ = 0;
            goto inc;
        }
        if (work->frame >= work->frames - 1) {
            return 1;
        }
    next:
        env->otZ = 0;
    inc:
        work->frame++;
    }
    return 0;
}

void btlfx_spawnDrawOnUnit(s8 unit, s8 otBias, s8 emitterId, s32 dataId, s32 scale, s32 frames) {
    g_btlfxResArgs2.unit = unit;
    g_btlfxResArgs2.otBias = otBias;
    g_btlfxResArgs.data = emitterId;
    g_btlfxResArgs.mode = frames | (BTLFX_DRAWON_EMITTER << 6);
    btlfx_spawnTask(BTLFX_RES_DRAW_ON_UNIT, g_btlfxEnv->state->file, dataId, scale);
}

s32 btlfx_opApplyResult(s32 mode, void *work, Unk800F78A4 *env, void *entry, s32 *args) {
    s32 id;
    s32 *argp;

    if (mode == 1) {
        argp = args;
        id = argp[0];
        if (id == BTLFX_UNIT_NONE) {
            id = -1;
        } else if (id == BTLFX_UNIT_RANDOM_TARGET) {
            id = btlfx_rand() % g_btlfxEnv->targets->targetCount;
        }
        if (argp[1] == 0) {
            btlfx_execCmd(id);
            return 1;
        }
        if (argp[1] == 1) {
            if (g_btlfxWeaponScriptId == BTLFX_EFFECT_BLOOD_SWORD || (u32)((u16)g_btlfxState.effectId - (BTLFX_EFFECT_ATTACK + CHR_KIND_STEINER)) >= 2 || id != BTLFX_UNIT_CASTER) {
                btlfx_showPopups(id);
            }
            return 1;
        }
    }
    return 1;
}

s32 btlfx_opAddCameraChoice(s32 mode, s32 work, s32 env, Unk800F78A40 *entry, s32 *args) {
    s32 cameraId;
    Unk800FEC40 *state;
    s16 *h;

    if (mode == 1) {
        cameraId = args[0];
        if (cameraId == 0xFF) {
            h = g_btlfxState.cameras.h;
            h[0] = 0;
            h[1] = 0;
        } else {
            state = &g_btlfxState;
            if (state->cameras.b.count < 3) {
                state->cameras.b.ids[state->cameras.b.count] = cameraId;
                state->cameras.b.count++;
            }
        }
    }
    return 1;
}

s32 btlfx_opPlayCamera(s32 mode, s32 work, s32 env, Unk800F78A40 *entry, s32 *args) {
    s32 camera;
    s32 pick;
    if (mode == 1) {
        camera = 0;
        switch (args[1]) {
        case 0:
            camera = btlfx_getFileData(entry->file, args[0]);
            break;
        case 1:
            camera = btlfx_getFileData(entry->file, g_btlfxState.cameras.b.ids[g_btlfxState.cameraPick]);
            break;
        case 2:
            if (g_btlfxState.cameras.b.count != 0) {
                g_btlfxState.cameraPick = btlfx_rand() % g_btlfxState.cameras.b.count;
            }
            camera = btlfx_getFileData(entry->file, g_btlfxState.cameras.b.ids[g_btlfxState.cameraPick]);
            break;
        case 3:
            pick = g_btlfxEnv->camera->effectPick;
            if (pick >= g_btlfxState.cameras.b.count) {
                pick = g_btlfxState.cameras.b.count - 1;
            }
            camera = btlfx_getFileData(entry->file, g_btlfxState.cameras.b.ids[pick]);
            break;
        }
        if (g_btlfxCameraOn != 0) {
            btlcam_play(camera);
        }
    }
    return 1;
}

void btlfx_startFadeBackground(s32 level, s32 frames) {
    s32 brightness;
    if (level < 0) {
        brightness = 0x80;
    } else {
        brightness = g_btlfxCastBgBrightness[level];
    }
    btlfx_spawnTask(BTLFX_RES_FADE_BACKGROUND, -1, brightness, frames);
}

s32 btlfx_opFadeBackground(s32 mode, Unk800E6270 *work, s32 env, s32 *entry, s32 *args) {
    s32 step;
    s32 brightness;
    if (mode == 0) {
        *args = 8;
        return 0;
    }
    if (mode == 1) {
        work->from = btlbg_getBrightness() & 0xFF;
        work->delta = args[0];
        if (work->delta == 0xFF) {
            work->delta = 0x80;
        }
        work->delta -= work->from;
        if ((work->frames = args[1]) == 0 || work->delta == 0) {
            btlbg_setBrightness(work->from + work->delta);
            return 1;
        }
        return 0;
    }
    step = *entry;
    brightness = work->from + step * work->delta / work->frames;
    if (brightness < 0x10) {
        brightness = 0;
    }
    btlbg_setBrightness(brightness);
    if (step < work->frames) {
        *entry = step;
        return 0;
    }
    return 1;
}

void btlfx_startFadeUnits(s32 frames, s32 flags) {
    btlfx_spawnTask(BTLFX_RES_FADE_UNITS, -1, frames & 0xFF, flags & 0xFF);
}

s32 btlfx_opFadeUnits(s32 mode, Unk800E6270 *work, s32 env, s32 *entry, s32 *args) {
    s16 delta;
    s32 step;
    if (mode == 0) {
        *args = 0xC;
        return 0;
    }
    if (mode == 1) {
        if (args[1] & BTLFX_FADE_OUT) {
            work->from = 0x80;
            delta = -0x80;
        } else {
            work->from = 0;
            delta = 0x80;
        }
        work->delta = delta;
        work->group = args[1] & BTLFX_FADE_GROUP;
        if ((work->frames = args[0]) != 0) {
            return 0;
        }
        btlfx_fadeUnitGroup(work->group, work->from + work->delta);
        return 1;
    }
    step = *entry;
    btlfx_fadeUnitGroup(work->group, work->from + step * work->delta / work->frames);
    if (step < work->frames) {
        *entry = step;
        return 0;
    }
    return 1;
}

s32 btlfx_opPlaySound(s32 mode, s32 work, s32 env, Unk800F78A40 *entry, s32 *args) {
    if (mode == 1) {
        s32 unit = -1;
        if (args[1] != 0xFF) {
            unit = args[1];
        }
        btlfx_playFileSound(args[0], entry->file, unit);
    }
    return 1;
}

void btlfx_startScreenFade(s32 mode, s32 frames) {
    btlfx_spawnTask(BTLFX_RES_SCREEN_FADE, -1, mode, frames);
}

s32 btlfx_opScreenFade(s32 mode, Unk800E6410 *work, s32 env, s32 *entry, s32 *args) {
    s32 step;
    s32 *argp;
    if (mode == 0) {
        *args = 0xC;
        return 0;
    }
    if (mode == 1) {
        argp = args;
        if (argp[0] == 2) {
            return 1;
        }
        if (argp[0] == 1) {
            work->shade = 0;
        } else if (argp[0] == 0) {
            work->shade = 0xFF;
        }
        if ((work->frames = argp[1]) != 0) {
            return 0;
        }
        btlfx_drawScreenTint(g_btlfxEnv->ot + 1, 0xFF, work->shade, work->shade, work->shade);
        return 1;
    }
    step = *entry;
    btlfx_drawScreenTint(g_btlfxEnv->ot + 1, 0xFF, work->shade, work->shade, work->shade);
    if (step < work->frames) {
        *entry = step;
        return 0;
    }
    return 1;
}

void btlfx_sampleGradient(u8 *p, CVECTOR *out, s32 x) {
    Grad *grad = (Grad *)p;
    s32 cnt = grad->count;
    s32 i;
    s32 val; /* key length, then the blend ratio (separate locals don't match) */

    p += 8;
    if (grad->total == 0) {
        i = cnt = 0; /* i: running start */
        p += 4;
    loop:
        val = p[-1];
        if (val != 0) {
            ((u16 *)p)[1] = i;
            i += val;
            cnt++;
            ((u16 *)p)[0] = val;
            p += 8;
            goto loop;
        }
        p = (u8 *)grad + 8;
        grad->total = i;
        grad->count = cnt;
    }
    if (grad->total < x) {
        x = grad->total;
    }
    p += cnt * 8 - 8;
    for (i = 0; i < cnt; i++, p -= 8) {
        if (x >= ((u16 *)p)[3]) {
            break;
        }
    }
    val = x - ((u16 *)p)[3];
    val = (val << 12) / ((u16 *)p)[2];
    LoadAverageCol(p, p + 8, ONE - val, val, (u8 *)out);
}

void btlfx_drawScrollMove(s32 scroll, s32 dstX, s32 dstY, s32 width, s32 height, s32 srcX, s32 srcY) {
    RECT tailRect;
    RECT headRect;
    DR_MOVE *move;
    s32 split = scroll % height;
    s32 rest;
    move = (DR_MOVE *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(move + 2);
    rest = height - split;
    if (split != 0) {
        headRect.x = srcX;
        headRect.y = srcY;
        headRect.w = width;
        headRect.h = split;
        SetDrawMove(move, &headRect, dstX, dstY + rest);
        AddPrim(&g_btlfxEnv->ot[OT_LAST], move);
    }
    move++;
    if (rest != 0) {
        tailRect.x = srcX;
        tailRect.y = srcY + split;
        tailRect.w = width;
        tailRect.h = rest;
        SetDrawMove(move, &tailRect, dstX, dstY);
        AddPrim(&g_btlfxEnv->ot[OT_LAST], move);
    }
}

void btlfx_drawScreenGrid(u32 otz, s32 centerX, s32 centerY, s32 cellW, s32 cellH, s32 cols, s32 rows, s32 ofsX, s32 ofsY,
                   SVECTOR *rot, VECTOR *scale, CVECTOR *color, s32 abr, void (*vertexFn)(SVECTOR *)) {
    XY32 prevRow[2][18];
    SVECTOR vert[4];
    SVECTOR uv[4];
    MATRIX mat;
    long ox;
    long oy;
    s32 cx;
    s32 cy;
    s32 tp0;
    s32 tp1;
    POLY_FT4 *poly;
    s32 x;
    s32 y;
    s32 row;
    s32 col;
    s32 tp;
    s32 shifted;
    s32 prevUv;
    s32 sx;
    s32 sy;
    s32 ty;
    u32 len;

    ReadGeomOffset(&ox, &oy);
    poly = (POLY_FT4 *)g_btlfxEnv->prim;
    g_btlfxEnv->prim = (u8 *)(poly + cols * rows);
    if (abr != BTLFX_ABR_OPAQUE) {
        color->cd = 0x2E;
    } else {
        color->cd = 0x2C;
    }
    if (abr == BTLFX_ABR_OPAQUE) {
        abr = 0;
    }
    tp0 = GetTPage(2, abr, g_btlfxEnv->dispEnv->disp.x, g_btlfxEnv->dispEnv->disp.y);
    tp1 = GetTPage(2, abr, g_btlfxEnv->dispEnv->disp.x + 0x80, g_btlfxEnv->dispEnv->disp.y) << 16;
    ofsX *= cellW;
    tp0 <<= 16;
    ofsY *= cellH;
    cx = centerX;
    cy = centerY;
    SetGeomOffset(centerX, centerY);
    math_rotMatrixYXZ(rot, &mat);
    scale->vz = ONE;
    ScaleMatrix(&mat, scale);
    mat.t[1] = 0;
    mat.t[0] = 0;
    mat.t[2] = ReadGeomScreen();
    gte_SetRotMatrix(&mat);
    gte_SetTransMatrix(&mat);
    gte_ldvz012_0();
    for (y = 0, row = 0; row <= rows; row++, y += cellH) {
        for (x = 0, col = 0; col <= cols; col++, x += cellW) {
            ((XY32 *)&vert[0])->w = (*prevRow)[col].w;
            sx = x - ofsX;
            prevUv = prevRow[1][col].w;
            uv[1].vx = (vert[1].vx = sx) + cx;
            uv[1].vy = (vert[1].vy = y - ofsY) + cy;
            *(s32 *)&uv[0] = prevUv;
            prevRow[1][col].w = *(s32 *)&uv[1];
            if (vertexFn != NULL) {
                vertexFn(&vert[1]);
            }
            prevRow[0][col].w = *(s32 *)&vert[1];
            if (col != 0 && row != 0) {
                gte_ldxy3(&vert[1], &vert[0], &vert[3]);
                *(u32 *)&poly->r0 = *(u32 *)color;
                gte_rtpt_b();
                if (uv[0].vx < 0x100 && uv[1].vx < 0x100 && uv[2].vx < 0x100 && uv[3].vx < 0x100) {
                    shifted = 0;
                    tp = tp0;
                } else {
                    shifted = 1;
                    tp = tp1;
                    uv[0].vx -= 0x80;
                    uv[1].vx -= 0x80;
                    uv[2].vx -= 0x80;
                    uv[3].vx -= 0x80;
                }
                gte_stsxy3(&poly->x0, &poly->x1, &poly->x2);
                gte_ldxy0(&vert[2]);
                if (otz < OT_LEN) {
                    len = 0x09000000;
                    addPrimLen(&g_btlfxEnv->ot[otz], poly, len);
                }
                gte_rtps_b();
                *(u32 *)&poly->u0 = uv[1].vx | (uv[1].vy << 8);
                *(u32 *)&poly->u1 = uv[0].vx | (uv[0].vy << 8) | tp;
                *(u32 *)&poly->u2 = uv[3].vx | (uv[3].vy << 8);
                *(u32 *)&poly->u3 = uv[2].vx | (uv[2].vy << 8);
                gte_stsxy(&poly->x3);
                if (shifted) {
                    uv[0].vx += 0x80;
                    uv[1].vx += 0x80;
                    uv[2].vx += 0x80;
                    uv[3].vx += 0x80;
                }
                poly++;
            }
            *(s32 *)&vert[2] = *(s32 *)&vert[0];
            *(s32 *)&vert[3] = *(s32 *)&vert[1];
            *(s32 *)&uv[2] = *(s32 *)&uv[0];
            *(s32 *)&uv[3] = *(s32 *)&uv[1];
        }
    }
    SetGeomOffset(ox, oy);
}

void btlfx_pushViewRotation(SVECTOR *rot) {
    MATRIX mat;
    SVECTOR zeroRot;
    memset((s32)&zeroRot, 0, sizeof(zeroRot));
    if (rot == 0) {
        rot = &zeroRot;
    }
    math_rotMatrixYXZ(rot, &mat);
    mat.t[1] = 0;
    mat.t[0] = 0;
    mat.t[2] = ReadGeomScreen();
    g_btlfxSavedView = *g_btlfxEnv->viewMatrix;
    *g_btlfxEnv->viewMatrix = mat;
}

void btlfx_popViewMatrix(void) {
    *g_btlfxEnv->viewMatrix = g_btlfxSavedView;
}

void btlfx_pushGeomScreen(s32 dist) {
    s32 *saved = &g_btlfxSavedGeomScreen;
    *saved = ReadGeomScreen();
    SetGeomScreen(dist);
}

void btlfx_popGeomScreen(void) {
    SetGeomScreen(g_btlfxSavedGeomScreen);
}

void btlfx_drawBgOffscreen(s32 x, s32 y, s32 w, s32 h, u32 *ot, s32 camX, s32 camY, s32 camZ) {
    long ox;
    long oy;
    s32 savedRain;
    s32 geomScreen;
    DrawEnvPair *envs;

    savedRain = g_btlbgRainCount;
    envs = (DrawEnvPair *)g_btlfxEnv->prim;
    g_btlbgRainCount = 0;
    g_btlfxEnv->prim = (u8 *)(envs + 1);
    SetDefDrawEnv(&envs->env, x, y, w, h);
    envs->env.isbg = 1;
    envs->env.dtd = 0;
    envs->env.dfe = 0;
    envs->env.r0 = 7;
    envs->env.g0 = 7;
    envs->env.b0 = 7;
    envs->prev = *(DRAWENV *)&g_btlfxEnv->drawEnv;
    ClearOTag(ot, BTL_BG_OT_LEN);
    geomScreen = ReadGeomScreen();
    ReadGeomOffset(&ox, &oy);
    SetGeomScreen(0x200);
    SetGeomOffset(w >> 1, h >> 1);
    g_btlfxBgTopView.t[0] = -camX;
    g_btlfxBgTopView.t[1] = camY;
    g_btlfxBgTopView.t[2] = camZ + 0x2900;
    btlbg_setLayerMask(BTLBG_SHOW_FRONT);
    g_btlfxEnv->prim = btlbg_draw(&g_btlfxBgTopView, g_gameState->gameData->ctx.battle->work->bgData, (s32)g_btlfxEnv->prim, NULL, (s32)ot);
    btlbg_setLayerMask(BTLBG_SHOW_ALL);
    EnterCriticalSection();
    DrawOTagEnv(ot, &envs->env);
    PutDrawEnv(&envs->prev);
    ExitCriticalSection();
    g_btlbgRainCount = savedRain;
    SetGeomScreen(geomScreen);
    SetGeomOffset(ox, oy);
}

void btlfx_setTargets(BtlUnitList *list) {
    g_btlfxEnv->targets = list;
}

void btlfx_initEnv(void) {
    GameData *gd;
    g_btlfxEnv->otZ = 0;
    gd = g_gameState->gameData;
    g_btlfxEnv->ot = (s32 *)gd->ot[gd->drawBuf];
    g_btlfxEnv->dispEnv = &g_gameState->dispEnv[g_gameState->gameData->drawBuf];
    g_btlfxEnv->drawEnv = &g_gameState->drawEnv[g_gameState->gameData->drawBuf];
    g_btlfxEnv->viewMatrix = &g_gameState->gameData->viewMatrix;
}

void btlfx_beginPrims(u8 *prim, s32 *ot) {
    Unk800F78A4 *env = g_btlfxEnv;
    GameState *gs = g_gameState;
    env->otZ = 0;
    env->ot = ot;
    env->dispEnv = &gs->dispEnv[gs->gameData->drawBuf];
    env->drawEnv = &gs->drawEnv[gs->gameData->drawBuf];
    env->viewMatrix = &gs->gameData->viewMatrix;
    env->prim = prim;
}

u8 *btlfx_endPrims(void) {
    return g_btlfxEnv->prim;
}

BtlUnit *btl_getUnit(s32 unitId) {
    if (unitId == BTLFX_UNIT_CASTER) {
        return g_btlfxEnv->targets->caster;
    }
    if (unitId == BTLFX_UNIT_PARTNER) {
        return g_btlfxEnv->targets->partner;
    }
    if (unitId < BTLFX_UNIT_BOUNCED) {
        if (unitId < 0 || unitId >= g_btlfxEnv->targets->targetCount) {
            SystemError(0x53, 0x136);
        }
        return g_btlfxEnv->targets->targets[unitId];
    }
    if (unitId - BTLFX_UNIT_BOUNCED < 0 || unitId - BTLFX_UNIT_BOUNCED >= g_btlfxEnv->targets->bounceCount) {
        SystemError(0x53, 0x136);
    }
    return g_btlfxEnv->targets->bounced[unitId - BTLFX_UNIT_BOUNCED];
}

s32 btl_isModelPart(BtlUnit *unit) {
    return (unit->flags & (BTL_UNIT_PARTY | BTL_UNIT_MODEL_PART)) == BTL_UNIT_MODEL_PART;
}

void btlfx_clearTargets(void) {
    g_btlfxEnv->targets = NULL;
}

s32 btlfx_getUnitRadius(s32 unitId) {
    s32 useModel;
    BtlUnit *unit;
    u32 flags;
    if (unitId & BTLFX_UNIT_MODEL_RADIUS) {
        unitId &= 0xFF;
        useModel = 1;
    } else {
        useModel = 0;
    }
    unit = btl_getUnit(unitId);
    flags = unit->flags;
    if ((flags & BTL_UNIT_PARTY) || useModel != 0) {
        return unit->model->header->radius;
    }
    return g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(flags)].type->radius;
}

s32 btl_getUnitHeight(BtlUnit *unit) {
    return unit->model->header->height;
}

s32 btlfx_getUnitHeight(s32 unitId) {
    return btl_getUnitHeight(btl_getUnit(unitId));
}

void btl_getBaseBonePos(BtlUnit *unit, u16 *out) {
    BtlBone *bone = &PSX_NULLABLE(unit->model->bones)[unit->baseBone];
    out[0] = bone->tx;
    out[1] = bone->ty;
    out[2] = bone->tz;
}

void btlfx_getUnitHomePos(s32 unitId, s16 *out) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (btl_isModelPart(unit) != 0) {
        out[0] = unit->pos[0] >> 16;
        out[1] = unit->pos[1] >> 16;
        out[2] = unit->pos[2] >> 16;
    } else {
        out[0] = unit->home->pos[0] >> 16;
        out[1] = unit->home->pos[1] >> 16;
        out[2] = unit->home->pos[2] >> 16;
    }
}

void btl_getUnitPos(BtlUnit *unit, s16 *out) {
    if (btl_isModelPart(unit) != 0 || !(unit->flags & BTL_UNIT_PARTY)) {
        btl_getBaseBonePos(unit, (u16 *)out);
        out[1] = 0;
    } else {
        out[0] = unit->pos[0] >> 16;
        out[1] = unit->pos[1] >> 16;
        out[2] = unit->pos[2] >> 16;
    }
}

void btlfx_getUnitPos(s32 unitId, s16 *out) {
    btl_getUnitPos(btl_getUnit(unitId), out);
}

void btl_getUnitBodyPos(BtlUnit *unit, s32 part, s16 *out) {
    s32 height;
    if (btl_isModelPart(unit) != 0) {
        btl_getBaseBonePos(unit, (u16 *)out);
        return;
    }
    btl_getUnitPos(unit, out);
    if (unit->flags & BTL_UNIT_PARTY) {
        height = btl_getUnitHeight(unit);
        if (part == 0) {
            height >>= 1;
        }
        if ((u8)btl_hasStatus(unit, BTL_STATUS_FLOAT)) {
            height -= 0x80;
        }
        out[1] -= height;
    } else {
        btl_getBaseBonePos(unit, (u16 *)out);
        if (part == 1) {
            out[1] = PSX_NULLABLE(unit->model->bones)[g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(unit->flags)].type->bones[2]].ty;
        }
    }
}

void btlfx_applyFloatHeight(s32 unitId, s16 *pos) {
    if (btl_hasStatus(btl_getUnit(unitId), BTL_STATUS_FLOAT) & 0xFF) {
        pos[1] -= 0x80;
    }
}

void btlfx_getUnitBodyPos(s32 unitId, s32 part, s16 *out) {
    btl_getUnitBodyPos(btl_getUnit(unitId), part, out);
}

void btlfx_setUnitPos(s32 unitId, s16 *pos) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (btl_isModelPart(unit) == 0) {
        unit->pos[0] = pos[0] << 16;
        unit->pos[1] = pos[1] << 16;
        unit->pos[2] = pos[2] << 16;
    }
}

s32 btlfx_getBouncePos(s32 index, s16 *pos, s16 *rot) {
    BtlUnitList *list = g_btlfxEnv->targets;
    BtlUnit *unit = list->bounced[index];
    if (index >= list->bounceCount) {
        return 1;
    }
    btl_getUnitBodyPos(unit, 0, pos);
    rot[0] = unit->rot[0];
    rot[1] = unit->rot[1];
    rot[2] = unit->rot[2];
    return 0;
}

void btlfx_getUnitRot(s32 unitId, s16 *out) {
    BtlUnit *unit = btl_getUnit(unitId);
    s16 *unitRot = unit->rot;
    out[0] = unitRot[0];
    out[1] = unitRot[1] + 0x800;
    out[2] = unitRot[2];
}

void btlfx_setUnitRot(s32 unitId, s16 *rot) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (btl_isModelPart(unit) == 0) {
        s16 *unitRot = unit->rot;
        unitRot[0] = rot[0];
        unitRot[1] = rot[1] - 0x800;
        unitRot[2] = rot[2];
    }
}

s32 btlfx_isEnemyUnit(s32 unitId) {
    return (btl_getUnit(unitId)->flags & BTL_UNIT_PARTY) ^ 1;
}

void btlfx_turnUnitToward(s32 unitId, s16 *targetPos, s16 *startRot, s32 axes, s32 roll, s32 frame, s32 frames) {
    SVec4 rot;
    SVec4 curRot;
    SVec4 pos;
    s32 ratio;

    if (frames < frame) {
        return;
    }
    btl_getUnit(unitId);
    if (frames == 0) {
        frames = 1;
    }
    ratio = (frame << 12) / frames;
    if (frame == 0 || startRot == NULL) {
        if (startRot == NULL) {
            startRot = curRot.h;
            ratio = ONE;
        }
        btlfx_getUnitRot(unitId, startRot);
    }
    if (targetPos != NULL) {
        btlfx_getUnitPos(unitId, pos.h);
        if (pos.w[0] != ((SVec4 *)targetPos)->w[0] || pos.h[2] != targetPos[2]) {
            btlfx_aimAngles(pos.h, targetPos, rot.h);
            if (!(axes & 1)) {
                rot.h[0] = 0;
            }
            if (!(axes & 2)) {
                rot.h[1] += 0x800;
            }
        } else {
            rot.w[0] = ((SVec4 *)startRot)->w[0];
            rot.w[1] = ((SVec4 *)startRot)->w[1];
        }
    } else {
        rot.h[0] = 0;
        rot.h[1] = 0;
    }
    rot.h[2] = roll;
    btlfx_turnToward(startRot, rot.h, ONE - ratio);
    btlfx_setUnitRot(unitId, rot.h);
}

void btlfx_getUnitScale(s32 unitId, VECTOR *out) {
    BtlUnit *unit = btl_getUnit(unitId);
    BtlModel *model = unit->model;
    if (!(model->flags & BTL_MODEL_SCALED)) {
        out->vz = ONE;
        out->vy = ONE;
        out->vx = ONE;
    } else {
        out->vx = model->scaleX;
        out->vy = unit->model->scaleY;
        out->vz = unit->model->scaleZ;
    }
}

void btlfx_setUnitScale(s32 unitId, s32 *scale) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (btl_isModelPart(unit) == 0) {
        if (scale[0] == ONE && scale[1] == scale[0] && scale[2] == scale[1]) {
            unit->model->flags &= ~BTL_MODEL_SCALED;
        } else {
            unit->model->flags |= BTL_MODEL_SCALED;
            unit->model->scaleX = scale[0];
            unit->model->scaleY = scale[1];
            unit->model->scaleZ = scale[2];
        }
    }
}

void btlfx_getStrikePos(s32 strikerId, s32 targetId, s16 *out) {
    s16 strikerPos[4];
    s16 targetPos[4];
    s16 dir[4];
    if (btl_getUnit(targetId) != btl_getUnit(strikerId)) {
        btlfx_getUnitPos(strikerId, strikerPos);
        btlfx_getUnitPos(targetId, targetPos);
        btlfx_aimAngles(targetPos, strikerPos, dir);
        btlfx_pointAhead(targetPos, dir, btlfx_getUnitRadius(targetId), out);
    } else {
        btlfx_getUnitPos(strikerId, out);
    }
}

s32 btlfx_compareDepth(s32 idA, s32 idB) {
    s16 posA[4];
    s16 posB[4];
    s32 nearer;
    btlfx_getUnitPos(idA, posA);
    btlfx_getUnitPos(idB, posB);
    nearer = posA[2] <= posB[2];
    if (((g_btlfxEnv->camera->yaw - 0x400) & 0xFFF) > 0x800) {
        nearer = -nearer;
    }
    return nearer;
}

s32 btlfx_addRadiusSixth(s32 unitId, s32 bias) {
    return (u16)(btl_getUnit(unitId)->model->header->radius / 6) + bias;
}

u32 btlfx_getUnitSlot(s32 unitId) {
    return (u8)btl_getUnit(unitId)->flags >> BTL_UNIT_SLOT_SHIFT;
}

void btlfx_getWeaponPos(s32 unitId, s32 tip, s16 *out) {
    BtlUnit *unit;
    s32 bone;
    MATRIX mat;

    SVECTOR ofs;

    unit = btl_getUnit(unitId);
    bone = (g_gameState->gameData->characters + BTL_UNIT_INDEX(unit->flags))->weaponBone;
    if (tip == 0) {
        btlfx_getUnitBonePos(unitId, bone, (u16 *)out);
    } else {
        btlfx_getUnitBoneMatrix(unitId, bone, (BtlBone *)&mat);
        gte_SetRotMatrix(&mat);
        gte_SetTransMatrix(&mat);
        btl_getWeaponOffset((Unk800BC1E0 *)unit->weapon, (s16 *)&ofs);
        gte_ldv0(&ofs);
        gte_rtv0tr();
        gte_stlvnl(mat.t);
        out[0] = mat.t[0];
        out[1] = mat.t[1];
        out[2] = mat.t[2];
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
}

void btlfx_setUnitBrightness(BtlUnit *unit, s32 level) {
    s32 withWeapon;
    s16 shade;
    if (level >= 0x100) {
        withWeapon = 0;
        level -= 0x100;
    } else {
        withWeapon = 1;
    }
    if ((unit->flags & BTL_UNIT_PARTY) && unit->weaponModel != NULL && withWeapon && (*(u32 *)unit->weaponModel & (BTL_MODEL_OFFSCREEN | BTL_MODEL_DRAWN)) == BTL_MODEL_DRAWN) {
        shade = level - 0x80;
        btl_setModelRgb(unit->weaponModel, shade, shade, shade);
        if (level < 0x46) {
            btl_setModelAbr(unit->weaponModel, TPAGE_ABR_ADD);
        }
    }
    if ((*(u32 *)unit->model & (BTL_MODEL_OFFSCREEN | BTL_MODEL_DRAWN)) == BTL_MODEL_DRAWN) {
        shade = level - 0x80;
        btl_setModelRgb(unit->model, shade, shade, shade);
        if (level < 0x46) {
            btl_setModelAbr(unit->model, TPAGE_ABR_ADD);
        }
    }
}

void btlfx_fadeUnit(BtlUnit *unit, s32 level) {
    if (btl_isModelPart(unit) == 0) {
        if (level < 8) {
            if (!((unit->flags >> BTL_UNIT_HIDDEN_BIT) & 1)) {
                unit->flags |= BTL_UNIT_HIDDEN;
                btlfx_setUnitBrightness(unit, level);
            }
        } else if ((unit->flags >> BTL_UNIT_HIDDEN_BIT) & 1) {
            if (!(unit->status & BTL_STATUS_JUMP)) {
                unit->flags &= ~BTL_UNIT_HIDDEN;
            }
        } else {
            btlfx_setUnitBrightness(unit, level);
        }
    }
}

void btlfx_fadeUnitById(s32 unitId, s32 level) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (level < 0) {
        if (level == -1) {
            btl_hideMeshes(unit, 0xFFFF);
        } else {
            btl_showMeshes(unit, 0xFFFF);
        }
    } else {
        btlfx_fadeUnit(unit, level);
    }
}

void btlfx_setUnitLight(s32 unitId, s32 level) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (btl_isModelPart(unit) == 0) {
        BtlModel *model = unit->model;
        s16 shade = level - 0x80;
        if ((*(u32 *)model & (BTL_MODEL_OFFSCREEN | BTL_MODEL_DRAWN)) == BTL_MODEL_DRAWN) {
            btl_setModelRgb(model, shade, shade, shade);
        }
    }
}

s32 btlfx_fadeUnitGroup(u32 type, s32 level) {
    BtlUnit *unit;
    s32 faded = 0;
    for (unit = g_gameState->gameData->ctx.battle->unitList; unit != NULL; unit = unit->next) {
        switch (type) {
        case BTLFX_FADE_PARTY:
            if (unit->flags & BTL_UNIT_PARTY) {
                btlfx_fadeUnit(unit, level);
                faded++;
            }
            break;
        case BTLFX_FADE_ENEMIES:
            if (!(unit->flags & BTL_UNIT_PARTY)) {
                btlfx_fadeUnit(unit, level);
                faded++;
            }
            break;
        case BTLFX_FADE_CASTER:
            if (unit == btl_getUnit(BTLFX_UNIT_CASTER)) {
                btlfx_fadeUnit(unit, level);
                faded++;
            }
            break;
        case BTLFX_FADE_ALL:
            btlfx_fadeUnit(unit, level);
            faded++;
            break;
        case BTLFX_FADE_CASTER_SIDE:
            if (btlfx_isEnemyUnit(BTLFX_UNIT_CASTER)) {
                if (!(unit->flags & BTL_UNIT_PARTY)) {
                    btlfx_fadeUnit(unit, level);
                    faded++;
                }
            } else if (unit != btl_getUnit(BTLFX_UNIT_CASTER) && (unit->flags & BTL_UNIT_PARTY)) {
                btlfx_fadeUnit(unit, level);
                faded++;
            }
            break;
        }
    }
    return faded;
}

void btlfx_clearUnitStatus(s32 unitId) {
    if (btlfx_isEnemyUnit(unitId) != 0) {
        btl_removeStatuses(btl_getUnit(unitId), ~(BTL_STATUS_DEATH_IMMUNE | BTL_STATUS_KO | BTL_STATUS_LOW_HP | BTL_STATUS_STOP));
    } else {
        btl_clearUnitStatus(btl_getUnit(unitId));
    }
}

s32 btlfx_nextUnitPos(s32 id, s16 *pos, BtlUnit **iter, s32 *out) {
    BtlUnit *unit;
    s32 isSelf = 0;

    if (*iter == (BtlUnit *)-1) {
        unit = g_gameState->gameData->ctx.battle->unitList;
    } else {
        if (*iter == NULL) {
            return -2;
        }
        unit = *iter;
    }
    btl_getUnitBodyPos(unit, 0, pos);
    if (((u8)btl_hasStatus(unit, BTL_STATUS_KO) && unit != btl_getUnit(id)) || !((unit->flags >> BTL_UNIT_TARGETABLE_BIT) & 1)) {
        *iter = unit->next;
        return -1;
    }
    if (unit == btl_getUnit(id)) {
        isSelf = 1;
    }
    *out = unit->flags & BTL_UNIT_PARTY;
    *iter = unit->next;
    return isSelf;
}

void btlfx_signalScriptCue(void) {
    g_evtGlobalVars.b[EVT_GB_BATTLE_AI_FLAGS] |= EVT_GB_AI_EFFECT_CUE;
}

void btlfx_reapplyMini(BtlUnit *unit) {
    if (unit->status & BTL_STATUS_MINI) {
        unit->model->flags |= BTL_MODEL_SCALED;
        unit->model->scaleX = 0x800;
        unit->model->scaleY = 0x800;
        unit->model->scaleZ = 0x800;
    }
}

void btlfx_reapplyVanish(BtlUnit *unit) {
    if (unit->status & BTL_STATUS_VANISH) {
        btl_hideMeshes(unit, unit->vanishMeshes);
    }
}

s32 btlfx_getScaledRadius(s32 unitId) {
    VECTOR scale;
    btlfx_getUnitScale(unitId, &scale);
    return btl_getUnit(unitId)->model->header->radius * scale.vx >> 12;
}

s32 btlfx_isCmdPendingOnAutoPotion(s32 unitId) {
    s32 ret = 0;
    if (g_gameState->gameData->ctx.battle->curCmd->cmdNo == BTL_CMD_AUTO_POTION) {
        ret = btl_isCmdPending(btl_getUnit(unitId)->cmd) != 0;
    }
    return ret;
}

s32 btlfx_resetIdleMotion(s32 mode, s32 unitId) {
    s32 ret = 0;
    BtlUnit *unit = btl_getUnit(unitId);
    if (mode == 0) {
        if ((u8)btl_hasStatus(unit, BTL_STATUS_KO)) {
            unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_KO];
        } else if ((unit->flags >> BTL_UNIT_CMD_WAITING_BIT) & 1) {
            unit->model->motion = (BtlMotion *)unit->motions[BTL_MOTION_READY];
        } else {
            unit->model->motion = (BtlMotion *)unit->motions[(unit->flags >> BTL_UNIT_WEAK_STANCE_BIT) & 1];
        }
    } else if ((u8)btl_hasStatus(unit, BTL_STATUS_KO) || BTL_UNIT_INDEX(unit->flags) == 4) {
        btlfx_resetIdleMotion(0, BTLFX_UNIT_CASTER);
        ret = 1;
    } else {
        btlfx_playUnitMotion(BTLFX_UNIT_CASTER, BTL_MOTION_CAST_END);
    }
    return ret;
}

s32 btlfx_isUnitKind(s32 unitId, s32 kindIdx) {
    u32 flags = btl_getUnit(unitId)->flags;
    u8 kind = g_btlfxUnitKinds[kindIdx];
    return ((flags & BTL_UNIT_PARTY) ? g_gameState->gameData->characters[BTL_UNIT_INDEX(flags)].info.b.kind : CHR_KIND_NONE) == kind;
}

void btlfx_copyWords(s32 *dst, s32 *src, u32 size) {
    u32 i;
    u32 rem = size & 3;
    size >>= 2;
    if (rem) {
        size++;
    }
    for (i = 0; i < size;) {
        i++;
        *dst++ = *src++;
    }
}

s32 *btlfx_swapModelData(s32 modelId, s32 *save) {
    s32 id = (modelId & 0xFFFF) | DB_MODEL;
    u32 size = (u32)db_lookupRegistered(DB_GET_SIZE, id, 0);
    s32 *modelData = db_lookupRegistered(DB_GET_ADDR, id, 0);
    if (*save == 0x1234) {
        btlfx_copyWords(modelData, save + 1, size);
    } else {
        btlfx_copyWords(save + 1, modelData, size);
        *save = 0x1234;
    }
    return modelData;
}

void btlfx_moveSummonToHeap(s32 sizeKb) {
    btl_unloadEnemyModels();
    btlfx_copyWords(g_gameState->gameData->ctx.battle->work->heapStart, g_btlfxLoader.summon->data[0], sizeKb << 10);
    g_btlfxLoader.summon->data[0] = g_gameState->gameData->ctx.battle->work->heapStart;
}

void btl_forEachModelVertex(BtlModelHeader *header, void (*fn)(SVECTOR *, s32)) {
    s32 i;
    s32 j;
    s32 k;
    s32 vtxBase;
    BtlMesh *mesh;
    BtlModelHeader *hdr = header;
    for (i = 0; i < hdr->meshCount; i++) {
        mesh = &hdr->meshes[i];
        vtxBase = 0;
        for (j = 0; j < hdr->boneCount; j++) {
            for (k = 0; k < mesh->vertexCounts[j]; k++) {
                fn(&mesh->vertices[vtxBase + k], vtxBase + k);
            }
            vtxBase += mesh->vertexCounts[j];
        }
    }
}

void btlfx_setPartnerByArg(s32 argIndex) {
    g_btlfxEnv->targets->partner = btl_getUnitById(g_btlfxEnv->targets->fxArgs[argIndex]);
}

void btlfx_restorePartyLooks(void) {
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    while (unit != NULL) {
        if (unit->flags & BTL_UNIT_PARTY) {
            btl_reapplyStatusLook(unit);
            btlfx_reapplyMini(unit);
            btlfx_reapplyVanish(unit);
        }
        unit = unit->next;
    }
    if (g_btlfxEnv->state->effectId == BTLFX_EFFECT_ARK_SHORT) {
        btl_setIdleMotion(btl_getUnit(BTLFX_UNIT_CASTER));
    }
}

void btlfx_restoreUnitLook(s32 unitId) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (unit->flags & BTL_UNIT_PARTY) {
        btl_reapplyStatusLook(unit);
        btlfx_reapplyMini(unit);
        btlfx_reapplyVanish(unit);
    }
}

s32 btlfx_countVisibleParty(void) {
    BtlUnit *unit = g_gameState->gameData->ctx.battle->unitList;
    s32 visible = 0;
    while (unit != NULL) {
        visible += (unit->flags & (BTL_UNIT_PARTY | BTL_UNIT_HIDDEN)) == BTL_UNIT_PARTY;
        unit = unit->next;
    }
    return visible;
}

/* weapon texture animation: mode 0 = start anim (second ? 1 : 0), 1 = stop anim 2, 2 = start anim 2 */
void btlfx_setUnitTexAnim(s32 unitId, s32 mode, s32 second) {
    BtlUnit *unit = btl_getUnit(unitId);

    switch (mode) {
    case 0:
        ((BtlUnit9C50Arr *)PSX_NULLABLE(unit->model->texAnims))->e[second ? 1 : 0].flags |= (BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
        ((BtlUnit9C50Arr *)PSX_NULLABLE(unit->model->texAnims))->e[second ? 1 : 0].pos = 0;
        ((BtlUnit9C50Arr *)PSX_NULLABLE(unit->model->texAnims))->e[second ? 1 : 0].lastFrame = TEXANIM_FORCE_UPLOAD;
        break;
    case 1:
        ((BtlUnit9C50Arr *)PSX_NULLABLE(unit->model->texAnims))->e[2].flags &= ~(BTL_TEXANIM_PLAY | BTL_TEXANIM_ONCE);
        break;
    case 2:
        ((BtlUnit9C50Arr *)PSX_NULLABLE(unit->model->texAnims))->e[2].flags |= BTL_TEXANIM_PLAY;
        ((BtlUnit9C50Arr *)PSX_NULLABLE(unit->model->texAnims))->e[2].pos = 0;
        ((BtlUnit9C50Arr *)PSX_NULLABLE(unit->model->texAnims))->e[2].lastFrame = TEXANIM_FORCE_UPLOAD;
        break;
    }
}

void btl_setMarkersVisible(s32 visible) {
    if (visible != 0) {
        g_gameState->gameData->ctx.battle->cmdFlags |= BTL_CMDF_MARKERS;
    } else {
        g_gameState->gameData->ctx.battle->cmdFlags &= ~BTL_CMDF_MARKERS;
    }
}

s32 btl_areMarkersVisible(void) {
    return (g_gameState->gameData->ctx.battle->cmdFlags >> BTL_CMDF_MARKERS_BIT) & 1;
}

void btlfx_showCasterCmdName(void) {
    if (btlfx_isEnemyUnit(BTLFX_UNIT_CASTER) == 0) {
        btl_showCmdName(g_gameState->gameData->ctx.battle->curCmd);
    }
}

s32 btlfx_isResultDeferred(void) {
    s32 deferred = 0;
    if (btlfx_isEnemyUnit(BTLFX_UNIT_CASTER) != 0) {
        if (g_btlfxEnv->targets->fxArgs[0] == 0) {
            return deferred;
        }
        deferred = (u8)(g_btlfxEnv->state->flags >> 8) >> 7;
    }
    return deferred;
}

void btlfx_execCmd(s32 unitId) {
    s32 i;
    if (btlfx_isResultDeferred() == 0) {
        if (unitId == -1) {
            for (i = 0; i < g_btlfxEnv->targets->targetCount; i++) {
                btl_execCmd(g_btlfxEnv->targets->targets[i]);
            }
        } else {
            btl_execCmd(btl_getUnit(unitId));
        }
    }
}

void btlfx_showPopups(s32 unitId) {
    s32 i;
    if (btlfx_isResultDeferred() == 0) {
        if (unitId == -1) {
            for (i = 0; i < g_btlfxEnv->targets->targetCount; i++) {
                btl_spawnPopups(g_btlfxEnv->targets->targets[i]);
            }
        } else {
            btl_spawnPopups(btl_getUnit(unitId));
        }
    }
}

void btl_setVsyncMode(s32 mode) {
    g_gameState->vsyncMode = mode;
}

/* Void in the original, but effect modules read $v0 after calling it (export 0x338): it holds the u16 from
   Hi_RegisterTexListModel / Hi_RegisterGouEffModel. A u16 return adds an andi, so this stays void; ports must
   return that value (docs/effect-modules.md). */
void btlfx_registerEffectModel(Unk800E8F50 *data, s32 abr) {
    s32 i;
    s32 texCount;
    Unk800E8F50 *h;
    Unk800E8F50Entry *tex;
    void *model;
    h = data;
    if (data->magic != BTLFX_SUMMON_MAGIC) {
        SystemError(0x53, 0x190);
    }
    model = (u8 *)data + h->modelOfs;
    if (h->textured != 0) {
        texCount = (data->modelOfs - 8) >> 3;
        tex = h->tex;
        for (i = 0; i < texCount; i++) {
            tex[i].tpage |= (abr & 3) << 5;
        }
        Hi_RegisterTexListModel(model, (BtlTexInfo *)tex, (s16 *)((u8 *)h + h->texPosOfs), texCount);
    } else {
        Hi_RegisterGouEffModel(model);
    }
}

void btlfx_registerSummonModel(Unk800E9000 *data, s32 *src, s32 motionCount, Unk80100DC054 *slot) {
    Unk800E9000 *h = data;
    s32 *srcPtr;
    s32 i;
    s32 texCount;
    s32 k2;
    Unk800E9000Ent *tex;
    Unk800E9000Ent *texPos;

    if (h->magic != BTLFX_SUMMON_MAGIC) {
        SystemError(0x53, 0x190);
    }
    texCount = (h->modelOfs - 8) >> 3;
    slot->motionCount = motionCount;
    slot->texAnims = NULL;
    slot->texCount = texCount;
    slot->modelHdr = (u8 *)h + h->modelOfs;
    srcPtr = src;
    for (i = 0; i < motionCount; i++, srcPtr++) {
        slot->motions[i] = *srcPtr;
    }
    texPos = (Unk800E9000Ent *)((u8 *)h + h->texPosOfs);
    tex = h->tex;
    for (i = 0; i < texCount; i++) {
        slot->texTpage[i] = tex[i].tpage;
        slot->texClut[i] = tex[i].clut;
        slot->texY[i] = texPos[i].clut;
    }
    btlfx_initSummonModels(&slot->model, 1);
    g_btlfxLoader.summon = slot;
    g_btlfxLoader.summonModel = Hi_RegisterSummonModel(slot, NULL);
}

BtlUnit *btlfx_getFirstTarget(void) {
    if (g_btlfxEnv->targets->bounceCount == 0) {
        return btl_getUnit(0);
    }
    return btl_getUnit(8);
}

void btlfx_updateCameraEnable(void) {
    s32 on;
    BtlUnit *caster;
    BtlUnit *target;
    s32 camOff;
    u32 noCamera;
    if (g_btlfxEnv->state->flags & BTLFX_HDR_CAMERA) {
        on = 1;
    } else {
        caster = btl_getUnit(BTLFX_UNIT_CASTER);
        target = btl_getUnit(0);
        camOff = (g_gameState->config >> CFG_BIT_CAMERA) & 1;
        if (camOff == 1 || (u8)btl_hasStatus(caster, BTL_STATUS_MINI) || (u8)btl_hasStatus(caster, BTL_STATUS_VANISH)) {
            on = 0;
        } else {
            if (target->flags & BTL_UNIT_PARTY) {
                noCamera = g_gameState->gameData->ctx.battle->scene.flags >> BTL_SCENE_NO_PARTY_FX_CAMERA_BIT;
            } else {
                noCamera = g_gameState->gameData->ctx.battle->scene.flags >> BTL_SCENE_NO_ENEMY_FX_CAMERA_BIT;
            }
            noCamera &= 1;
            on = noCamera == 0;
        }
    }
    g_btlfxCameraOn = on;
}

void btlfx_playCmdCamera(s32 flags) {
    u32 hi;
    s32 msg;
    BtlUnit *caster;
    BtlUnit *target;

    if (flags == 0xFFFF) {
        return;
    }
    hi = flags & BTLFX_HDR_CASTER;
    if (g_btlfxCameraOn == 0 || hi == BTLFX_HDR_CASTER || (g_btlfxEnv->targets->kind & BTL_FXK_ITEM)) {
        return;
    }
    caster = btl_getUnit(BTLFX_UNIT_CASTER);
    if ((u8)btl_hasStatus(caster, BTL_STATUS_MINI) != 0 || (u8)btl_hasStatus(caster, BTL_STATUS_VANISH) != 0) {
        return;
    }
    target = btlfx_getFirstTarget();
    if (g_btlfxEnv->targets->kind & BTL_FXK_SWD_MAG) {
        msg = btlfx_getCommonData(g_btlfxCmdCameras[4]);
    } else if (caster->flags & BTL_UNIT_PARTY) {
        if ((u8)btl_hasStatus(caster, BTL_STATUS_TRANCE) == 0) {
            if ((hi >> BTLFX_HDR_CASTER_SHIFT) == BTLFX_CASTK_SUMMON || (hi >> BTLFX_HDR_CASTER_SHIFT) == BTLFX_CASTK_SUMMON_SHORT) {
                if (BTL_UNIT_INDEX(caster->flags) == CHR_DAGGER) {
                    msg = btlfx_getCommonData(g_btlfxCmdCameras[2]);
                } else {
                    msg = btlfx_getCommonData(g_btlfxCmdCameras[3]);
                }
            } else if (target->flags & BTL_UNIT_PARTY) {
                msg = btl_getSceneData(BTL_SCENE_CAM_CAST_PARTY);
            } else {
                msg = btl_getSceneData(BTL_SCENE_CAM_CAST_ENEMY);
            }
        } else if (target->flags & BTL_UNIT_PARTY) {
            msg = btlfx_getCommonData(g_btlfxCmdCameras[0]);
        } else {
            msg = btlfx_getCommonData(g_btlfxCmdCameras[1]);
        }
    } else {
        msg = g_gameState->gameData->ctx.battle->enemies[BTL_UNIT_INDEX(caster->flags)].type->camera;
    }
    if (msg != 0) {
        btlcam_play(msg);
    }
}

void btlfx_playBounceCamera(void) {
    u8 camera;
    if (g_btlfxCameraOn != 0 && g_btlfxEnv->targets->bounceCount != 0) {
        if (btl_getUnit(8)->flags & BTL_UNIT_PARTY) {
            camera = g_btlfxBounceCameraParty;
        } else {
            camera = g_btlfxBounceCameraEnemy;
        }
        btlcam_play(btlfx_getCommonData(camera));
    }
}

void btlfx_initSounds(void) {
    s32 i;
    g_btlfxSounds.count = 0;
    for (i = 5; i >= 0; i--) {
        g_btlfxSounds.slots[i].sound = 0;
    }
}

void btlfx_updateSounds(void) {
    s32 playing = 0;
    s32 i;
    Unk80104E30 *sounds = &g_btlfxSounds;
    Unk80104E30Entry *slot = sounds->slots;
    for (i = 0; i < BTLFX_SOUND_SLOTS; i++, slot++) {
        if (slot->sound != 0) {
            if (akao_isSePlaying(slot->sound) == 0) {
                slot->sound = 0;
            } else {
                playing++;
                akao_sendCmdA2(0, slot->sound, btl_soundPan(slot->unit));
                slot->age++;
            }
        }
    }
    sounds->count = playing;
}

s32 btlfx_allocSound(s32 unit) {
    Unk80104E30Entry *best = NULL;
    s32 bestbit = 0;
    s32 max = -1;
    s32 bit = 1;
    Unk80104E30 *sounds = &g_btlfxSounds;
    Unk80104E30Entry *slot = sounds->slots;
    s32 i;
    for (i = 0; i < BTLFX_SOUND_SLOTS; i++, slot++, bit <<= 1) {
        if (slot->sound == 0) {
            break;
        }
        if (max < slot->age) {
            max = slot->age;
            best = slot;
            bestbit = bit;
        }
    }
    if (i >= BTLFX_SOUND_SLOTS) {
        bit = bestbit;
        slot = best;
    }
    if (akao_isSePlaying(bit) != 0) {
        akao_stopSe(0, bit);
    }
    if (slot != NULL) {
        slot->sound = bit;
        slot->unit = unit;
        slot->age = 0;
    }
    return bit;
}

void btlfx_stopAllSounds(void) {
    akao_stopSe(0, BTLFX_SOUND_ALL);
}

Unk80104E34 *btlfx_findSound(s32 sound) {
    Unk80104E34 *slot;
    s32 i;
    if (sound != 0) {
        slot = g_btlfxSoundSlots;
        for (i = 0; i < BTLFX_SOUND_SLOTS; i++, slot++) {
            if (slot->sound == sound) {
                return slot;
            }
        }
    }
    return NULL;
}

s32 btl_soundPan(s32 unit) {
    SVECTOR pos;
    SVECTOR *posPtr;
    DVECTOR sxy;
    s32 x;
    s32 sign;

    if (unit == -1) {
        return 0x80;
    }
    if (PSX_IS_PTR_BITS(unit, 0x90000000)) { /* KSEG0 or scratchpad (bit 28) address: a position */
        posPtr = (SVECTOR *)unit;
    } else {
        posPtr = &pos;
        btlfx_getUnitPos(unit, (s16 *)posPtr);
    }
    gte_SetRotMatrix(g_btlfxEnv->viewMatrix);
    gte_SetTransMatrix(g_btlfxEnv->viewMatrix);
    gte_ldv0(posPtr);
    gte_rtps();
    gte_stsxy(&sxy);
    x = sxy.vx - 160;
    sign = (x < 0) ? -1 : 1;
    x = (x >= 0) ? x : -x;
    if (x > 170) {
        x = 170;
    }
    return sign * (x * 63 / 170) + 0x80;
}

void btlfx_stopSound(s32 sound) {
    Unk80104E34 *slot = btlfx_findSound(sound);
    if (slot != NULL) {
        akao_stopSe(0, sound);
        slot->sound = 0;
    }
}

void btlfx_setSoundUnit(s32 sound, s32 unit) {
    Unk80104E34 *slot = btlfx_findSound(sound);
    if (slot != NULL) {
        slot->unit = unit;
    }
}

void btlfx_setAllSoundsUnit(s32 unit) {
    Unk80104E34 *slot = g_btlfxSoundSlots;
    s32 i;
    for (i = 0; i < BTLFX_SOUND_SLOTS; i++, slot++) {
        if (slot->sound != 0) {
            slot->unit = unit;
        }
    }
}

s32 btlfx_playFileSound(s32 soundId, s32 file, s32 unit) {
    s32 sound = btlfx_allocSound(unit);
    if (sound != 0) {
        akao_playSe(btlfx_getFileData(file, soundId), sound, btl_soundPan(unit), 0x7F);
    }
    return sound;
}

void btlfx_replaceSound(s32 soundId, s32 file, s32 unit, s32 oldSound) {
    btlfx_stopSound(oldSound);
    btlfx_playFileSound(soundId, file, unit);
}

s32 btlfx_playCommonSound(s32 soundId, s32 unit) {
    s32 sound = btlfx_allocSound(unit);
    if (sound != 0) {
        akao_playSe(btlfx_getCommonData(soundId), sound, btl_soundPan(unit), 0x7F);
    }
    return sound;
}
