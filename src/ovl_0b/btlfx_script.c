#include "ovl_0b.h"

void btlfx_reset(void) {
    g_btlfxState.phase = BTLFX_PHASE_IDLE;
    g_btlfxState.partyReload = 0;
    g_btlfxState.modelSwap = 0;
}

s32 btlfx_resolveFirstUse(s32 effectId) {
    s32 id;
    s32 i;
    u8 *pair;
    s32 bit;
    GameState *gs;

    id = effectId;
    if (btlfx_getScaledRadius(0) >= 0x385) {
        for (pair = g_btlfxLargeTargetFx[0]; *pair != 0xFF; pair += 2) {
            if (pair[0] == effectId) {
                id = pair[1];
                break;
            }
        }
    }
    bit = 1;
    gs = g_gameState;
    for (i = 0; i < 16; i++, bit <<= 1) {
        if (g_btlfxFirstUsePairs[i][0] == id && !(gs->gameData->firstUseFxSeen & bit)) {
            id = g_btlfxFirstUsePairs[i][1];
            gs->gameData->firstUseFxSeen |= bit;
            break;
        }
        if (g_btlfxFirstUsePairs[i][1] == id) {
            if (!(gs->gameData->firstUseFxSeen & bit)) {
                gs->gameData->firstUseFxSeen |= bit;
            }
            break;
        }
    }
    return id;
}

s32 btlfx_start(s32 effectId) {
    if (g_btlfxState.phase != BTLFX_PHASE_IDLE) {
        return -1;
    }
    g_btlfxState.phase = BTLFX_PHASE_SAVE_VRAM;
    g_btlfxState.effectId = btlfx_resolveFirstUse(effectId);
    g_btlfxState.seqRelease = 0;
    g_btlfxState.seqSync = 0;
    g_btlfxState.wait = 0;
    g_btlfxState.signals.w = 0;
    g_btlfxState.fileId = 0;
    g_btlfxState.file = 0;
    g_btlfxState.pc = 0;
    g_btlfxState.partyReload = 0;
    g_btlfxState.modelSwap = 0;
    g_btlfxState.castEnded = 0;
    g_btlfxState.flags = BTLFX_HDR_RESIDENT;
    btlcam_focusCaster();
    g_btlfxEnv->workBuf = g_gameState->gameData->ctx.battle->work->loadBuf;
    return 0;
}

void btlfx_retryOp(void) {
    g_btlfxState.retry = 1;
    g_btlfxState.pc -= 3;
}

u8 *btlfx_findScript(s32 scriptId) {
    s32 i;
    u8 *entry;
    s32 id;
    for (i = 0, entry = RAM_BTLFX_MODULE(u8 *); i < 0x200; i++) {
        id = entry[0];
        if (entry[1] & BTLFX_SCRENT_ID_HI) {
            id += 0x100;
        }
        if (id == scriptId) {
            break;
        }
        entry += (entry[1] & BTLFX_SCRENT_OPS) * 3 + 2;
    }
    return entry + 2;
}

void btlfx_swapVramPages(s32 mode, u32 *data, s32 pageMask) {
    RECT rect;
    s32 i;
    s32 bit = 1;
    u32 *pixels;
    if (mode != 2) {
        pixels = (u32 *)(g_gameState->gameData->ctx.battle->partyBufs[0] + 0x50000);
    } else {
        pageMask = 1 << data[0x1FFF];
        pixels = data;
    }
    rect.w = 0x40;
    rect.h = 0x80;
    btlfx_resetMaskBit();
    for (i = 0; i < 4; i++) {
        if (bit & pageMask) {
            pixels -= 0x2000;
            rect.x = (i >> 1) * 64 + 0x380;
            rect.y = (i & 1) << 7;
            if (mode == 0) {
                StoreImage(&rect, pixels);
            } else {
                LoadImage(&rect, pixels);
            }
        }
        if (i & 1) {
            bit <<= 1;
        }
    }
}

void btlfx_callScript(s32 scriptId) {
    g_btlfxInCall = 1;
    g_btlfxReturnPc = g_btlfxState.pc;
    g_btlfxState.pc = btlfx_findScript(scriptId);
}

void btlfx_runScript(void) {
    RECT rect;
    u8 *pc;
    s16 op;
    s16 operand0; /* pc[1] and pc[2]: their meaning depends on the opcode */
    s16 operand1;
    s32 cont;
    u16 hdr;
    s32 slot;
    s32 casterMode;
    Unk800FEC40 *state = &g_btlfxState;

    if (state->partyReload != 0 && btl_reloadParty(0) == 0) {
        state->partyReload = 0;
        btlfx_restorePartyLooks();
    }
    if (state->modelSwap != 0 && btl_swapPartyModel(state->modelSwap - 1, 0) == 0) {
        state->modelSwap = 0;
        btlfx_restoreUnitLook(BTLFX_UNIT_CASTER);
    }
    switch (state->phase) {
    case BTLFX_PHASE_SAVE_VRAM:
        rect.x = VRAM_BTLFX_COMMON_X;
        rect.y = 0x140;
        rect.w = 0x40;
        rect.h = 0x10;
        btlfx_resetMaskBit();
        StoreImage(&rect, RAM_BTLFX_MODULE(u32 *));
        g_btlfxInCall = 0;
        state->phase = BTLFX_PHASE_START;
        break;
    case BTLFX_PHASE_START:
        hdr = RAM_BTLFX_MODULE(u16 *)[state->effectId];
        state->flags = hdr & ~BTLFX_HDR_COMMON_VRAM;
        g_btlfxEnv->targets->kind &= ~BTL_FXK_PARTY_UNLOAD;
        if (hdr & BTLFX_HDR_VRAM_SWAP) {
            btlfx_swapVramPages(0, NULL, (hdr & BTLFX_HDR_VRAM_SWAP) >> BTLFX_HDR_VRAM_SWAP_SHIFT);
        }
        if (hdr & BTLFX_HDR_RESIDENT) {
            rect.x = VRAM_BTLFX_COMMON_X;
            rect.y = 0x150;
            rect.w = 0x40;
            rect.h = 0xB0;
            btlfx_resetMaskBit();
            StoreImage(&rect, RAM_BTLFX_MODULE(u32 *));
            state->phase = BTLFX_PHASE_RESIDENT;
            state->flags &= ~BTLFX_HDR_CAMERA;
            state->flags &= ~BTLFX_HDR_PARTNER;
            break;
        }
        state->ending = 0;
        btlfx_updateCameraEnable();
        btlcam_beginEffect();
        if (!(g_btlfxEnv->targets->kind & BTL_FXK_NO_CAST)) {
            btlfx_playCmdCamera(state->flags);
        }
        if (btlfx_loadFile(state->effectId) == 0) {
            casterMode = (u8)state->flags >> BTLFX_HDR_CASTER_SHIFT;
            if (casterMode == BTLFX_CASTK_SUMMON) {
                g_btlfxEnv->targets->kind |= BTL_FXK_PARTY_UNLOAD;
            }
            btlfx_startCasterTask(casterMode);
            state->pc = g_btlfxFileScript;
            state->phase = BTLFX_PHASE_LOADING;
        }
        break;
    case BTLFX_PHASE_LOADING:
        if (g_btlfxEnv->loader->stage == BTLFX_STAGE_READY) {
            state->phase = BTLFX_PHASE_RUN;
            g_btlfxEnv->camera->flags |= BTLCAM_F_EFFECT_RUNNING;
        }
        break;
    case BTLFX_PHASE_RESIDENT:
        state->pc = btlfx_findScript(state->effectId);
        state->phase = BTLFX_PHASE_RUN;
        btlfx_showCasterCmdName();
        break;
    case BTLFX_PHASE_RUN:
        /* padded: a zero word follows the first jump table */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        cont = 1;
        do {
            if (state->wait != 0) {
                if (--state->wait > 0) {
                    return;
                }
                state->wait = 0;
            }
            pc = state->pc;
            op = pc[0];
            operand0 = pc[1];
            operand1 = pc[2];
            state->pc = pc + 3;
            if (op < BTLFX_OP_TASK_FIRST) {
                switch (op) {
                case BTLFX_CTL_END:
                    cont = 0;
                    state->pc -= 3;
                    if (btl_areMarkersVisible() == 0) {
                        btl_setMarkersVisible(1);
                    }
                    if (g_btlfxTasks.count == 0) {
                        if (state->flags & BTLFX_HDR_COMMON_VRAM) {
                            if (state->ending == 0) {
                                btlcam_endEffect();
                                btlfx_initParticleSystem();
                                btlfx_stopAllSounds();
                                btlfx_initSounds();
                                btlfx_reloadCommonData(0);
                                state->ending = 1;
                            } else if (btlfx_reloadCommonData(1) != 0) {
                                state->flags &= ~BTLFX_HDR_COMMON_VRAM;
                            }
                        } else {
                            state->phase = BTLFX_PHASE_IDLE;
                            g_btlfxEnv->targets = NULL;
                            btlcam_endEffect();
                            btlfx_initParticleSystem();
                            btlfx_stopAllSounds();
                            btlfx_initSounds();
                            vib_clear();
                        }
                    }
                    break;
                case BTLFX_CTL_WAIT:
                    if (operand0 == 0) {
                        state->wait = operand1;
                        cont = 0;
                    } else if (state->signals.b[operand1] == 0) {
                        cont = 0;
                        state->pc -= 3;
                        state->signals.b[operand1] = 0;
                    }
                    break;
                case BTLFX_CTL_SIGNAL:
                    state->signals.b[operand0] = operand1;
                    break;
                case BTLFX_CTL_LOAD_NEXT:
                    btlfx_loadNextBlock();
                    break;
                case BTLFX_CTL_WAIT_LOADER:
                    if (g_btlfxEnv->loader->stage < operand0) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case BTLFX_CTL_SET_FILE:
                    state->fileId = operand0;
                    state->file = btlfx_findFileSlot(operand0);
                    break;
                case BTLFX_CTL_WAIT_LOAD:
                    switch (operand0) {
                    case BTLFX_WAITLD_COMMON_TEX:
                        if (g_btlfxEnv->loader->commonTexLoaded == 0) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    case BTLFX_WAITLD_SUMMON_DATA:
                        if (g_btlfxEnv->loader->summonDataState != BTLFX_BLOCK_DONE) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    case BTLFX_WAITLD_SUMMON:
                        if (g_btlfxEnv->loader->summonState != BTLFX_BLOCK_DONE) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    }
                    break;
                case BTLFX_CTL_SEQ_SYNC:
                    if (btlfx_isEnemyUnit(BTLFX_UNIT_CASTER) != 0) {
                        g_btlfxState.seqSync = 1;
                        if (!(g_btlfxEnv->targets->kind & BTL_FXK_BOUNCE) && state->seqRelease == 0) {
                            cont = 0;
                            state->pc -= 3;
                        }
                    }
                    break;
                case BTLFX_CTL_WAIT_SOUND_BANK:
                    if (btlfx_waitSoundBank(operand0, operand1)) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case BTLFX_CTL_LOAD_SOUND_BANK:
                    if (btlfx_loadSoundBank(0)) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case BTLFX_CTL_RELOAD_PARTY:
                    if (operand0 != 0) {
                        if (state->partyReload != 0) {
                            state->pc -= 3;
                        }
                        cont = 0;
                    } else {
                        btl_reloadParty(1);
                        state->partyReload = 1;
                        cont = 0;
                    }
                    break;
                case BTLFX_CTL_WAIT_BUF:
                    if (g_btlfxEnv->loader->bufState != BTLFX_BLOCK_DONE) {
                        cont = 0;
                        state->pc -= 3;
                    }
                    break;
                case BTLFX_CTL_SWAP_MODEL:
                    if (operand0 != 0) {
                        if (state->modelSwap != 0) {
                            state->pc -= 3;
                        }
                        cont = 0;
                    } else {
                        slot = btlfx_getUnitSlot(operand1);
                        btl_swapPartyModel(slot, 1);
                        slot++;
                        state->modelSwap = slot;
                        cont = 0;
                    }
                    break;
                case BTLFX_CTL_VRAM:
                    switch (operand0) {
                    case BTLFX_VRAM_SWAP_BACK:
                        btlfx_swapVramPages(1, NULL, (state->flags & BTLFX_HDR_VRAM_SWAP) >> BTLFX_HDR_VRAM_SWAP_SHIFT);
                        break;
                    case BTLFX_VRAM_SWAP_FILE:
                        btlfx_swapVramPages(2, (u32 *)btlfx_getFileData(-1, operand1), 0);
                        break;
                    case BTLFX_VRAM_WAIT_COMMON_TEX:
                        if (g_btlfxEnv->loader->commonTexCount < operand1) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    }
                    break;
                case BTLFX_CTL_RETURN:
                    if (g_btlfxInCall != 0) {
                        g_btlfxInCall = 0;
                        state->pc = g_btlfxReturnPc;
                    }
                    break;
                case BTLFX_CTL_VIBRATE:
                    if (operand1 == 0) {
                        btlfx_copyWords(g_btlfxVibData, (s32 *)btlfx_getFileData(state->file, operand0), 0x708);
                        vib_init(g_btlfxVibData);
                        vib_selectTrack(0, 0, 1);
                        vib_selectTrack(0, 1, 1);
                    } else {
                        vib_setPlaying(0);
                        vib_selectTrack(1, 0, 1);
                        vib_selectTrack(1, 1, 1);
                    }
                    vib_play(1);
                    break;
                case BTLFX_CTL_NOP:
                    break;
                }
            } else {
                if (op < BTLFX_OP_MODULE_FIRST) {
                    btlfx_spawnTask(op - BTLFX_OP_TASK_FIRST, state->file, operand0, operand1);
                } else {
                    btlfx_spawnTask(op, state->file, operand0, operand1);
                }
            }
        } while (cont);
        break;
    }
}

void btlfx_initTasks(void) {
    s32 i;
    Unk80100C78Entry *task;
    g_btlfxTasks.count = 0;
    btlfx_heapInit(&g_btlfxTaskHeap, 0x1800);
    task = g_btlfxTasks.tasks;
    for (i = 10; i >= 0; i--) {
        task->state.w = 0;
        task++;
    }
}

s32 btlfx_allocTaskMem(Unk80100C78Entry *task, s32 bytes) {
    s32 mem = 0;
    s32 size = bytes + 4;
    s32 i = 0;
    Heap *h = g_btlfxLoader.heaps[task->file];
    for (; i < 2; i++) {
        if (task->sharedHeap != 0 && h != NULL) {
            if (btlfx_heapWrapIfFull(h, size) == 0) {
                mem = btlfx_heapAlloc(h, size);
                break;
            }
            task->sharedHeap = 0;
        } else {
            if (btlfx_heapWrapIfFull(&g_btlfxTaskHeap, size) == 0) {
                mem = btlfx_heapAlloc(&g_btlfxTaskHeap, size);
                break;
            }
            task->sharedHeap = 1;
        }
    }
    return mem;
}

void btlfx_spawnTask(s32 op, s32 file, s32 opArg0, s32 opArg1) {
    Unk80100C78Entry *task;
    s32 i;
    EffFn *tbl;
    s32 idx;
    Unk80100C78 *taskList = &g_btlfxTasks;
    EffInit init;
    s32 args[2];

    task = taskList->tasks;
    i = 0;
loop:
    if (task->state.h != 0) {
        i++;
        task++;
        if (i < 11) {
            goto loop;
        }
    }
    taskList->count++;
    task->state.h = 1;
    task->sharedHeap = 1;
    task->frame = 0;
    task->file = file;
    if (op >= BTLFX_OP_MODULE_FIRST) {
        task->handler = g_btlfxModuleOps[file][op - BTLFX_OP_MODULE_FIRST];
    } else {
        if (op < BTLFX_OP_UNIT_FIRST - BTLFX_OP_TASK_FIRST) {
            task->handler = g_btlfxResidentOps[op];
        } else {
            task->handler = g_btlfxUnitOps[op - (BTLFX_OP_UNIT_FIRST - BTLFX_OP_TASK_FIRST)];
        }
    }
    init.poolCount = 0;
    init.workSize = 0;
    task->pool = 0;
    task->work = 0;
    if (task->handler(0, 0, 0, 0, &init) == 0) {
        if (init.poolCount != 0) {
            task->pool = (void *)btlfx_allocTaskMem(task, btlfx_poolSize(init.elemSize, init.poolCount));
            btlfx_poolInit(task->pool, task, init.elemSize, init.poolCount, init.elemFn);
        }
        if (init.workSize != 0) {
            task->work = (void *)btlfx_allocTaskMem(task, btlfx_align4(init.workSize));
        }
    }
    args[0] = opArg0;
    args[1] = opArg1;
    if (task->handler(1, task->work, g_btlfxEnv, task, args) != 0) {
        task->state.h = 0;
        taskList->count--;
        if (task->work != 0) {
            btlfx_heapFree(task->work);
        }
        if (task->pool != 0) {
            btlfx_heapFree(task->pool);
        }
    }
}

s32 btlfx_updateTasks(void) {
    Unk80100C78 *taskList = &g_btlfxTasks;
    Unk80100C78Entry *task;
    s32 active;
    s32 i;
    s32 poolBusy;

    task = taskList->tasks;
    active = 0;
    for (i = 0; i < 11; i++, task++) {
        if (task->state.h != 0) {
            active++;
            g_btlfxEnv->otZ = 0;
            if (task->handler(2, task->work, g_btlfxEnv, task, NULL) != 0) {
                task->state.h = 2;
            } else {
                task->frame++;
            }
            if (task->pool != NULL) {
                poolBusy = btlfx_poolUpdate(task->pool);
            } else {
                poolBusy = 0;
            }
            if (poolBusy == 0 && task->state.h == 2) {
                task->state.h = 0;
                taskList->count--;
                if (task->work != NULL) {
                    btlfx_heapFree(task->work);
                }
                if (task->pool != NULL) {
                    btlfx_heapFree(task->pool);
                }
            }
        }
    }
    return active;
}

s32 btlfx_moveUnitArc(s32 unitId, s16 *dest, s32 frame, s32 frames, s32 height, s16 *from) {
    s16 pos[4];
    s32 t;
    if (frame == 1) {
        btlfx_getUnitPos(unitId, from);
    }
    t = (frame << 12) / frames;
    btlfx_lerpVec(from, dest, t, pos);
    if (height != 0) {
        pos[1] -= (math_sin(t >> 1) * height) >> 12;
    }
    btlfx_setUnitPos(unitId, pos);
    return frame >= frames;
}

s32 btlfx_moveUnitLinear(s32 unitId, s16 *dest, s32 frame, s32 frames, s16 *from, s32 fromMode) {
    s16 pos[4];
    s32 t;
    if (frame == 0) {
        switch (fromMode) {
        case 0:
            btlfx_getUnitHomePos(unitId, from);
            break;
        case 1:
            btlfx_getUnitPos(unitId, from);
            break;
        default:
            return 1;
        }
    }
    t = ((frame + 1) << 12) / frames;
    if (t > ONE) {
        t = ONE;
    }
    btlfx_lerpVec(from, dest, t, pos);
    btlfx_setUnitPos(BTLFX_UNIT_CASTER, pos);
    return frame >= frames - 1;
}

s32 btlfx_opSetParamsA(s32 mode, s32 work, s32 env, s32 entry, s32 *args) {
    if (mode == 1) {
        g_btlfxOpArgs.paramA[0] = args[0];
        g_btlfxOpArgs.paramA[1] = args[1];
    }
    return 1;
}

s32 btlfx_opSetParamsB(s32 mode, s32 work, s32 env, s32 entry, u8 *args) {
    if (mode == 1) {
        g_btlfxOpArgs.paramB[0] = args[0];
        g_btlfxOpArgs.paramB[1] = args[4];
    }
    return 1;
}

s32 btlfx_opSwingSound(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        work[0] = args[0];
        work[1] = args[1];
        if (btlfx_isPartyUnit(work[0]) == 0 || work[1] == mode) {
            return 1;
        }
        return 0;
    } else {
        return btlfx_stepSwingSound(work, env, entry);
    }
    return 0;
}

s32 btlfx_opHitSound(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        work[0] = args[0];
        work[1] = args[1];
        work[2] = g_btlfxOpArgs.paramA[0];
        work[3] = g_btlfxOpArgs.paramA[1];
        if (btlfx_isPartyUnit(work[0]) == 0) {
            return 1;
        }
        if (work[3] == mode) {
            return 1;
        }
    } else {
        return btlfx_stepHitSound(work, env, entry);
    }
    return 0;
}

s32 btlfx_opUnitSound(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode != 1) {
        return btlfx_stepUnitSound(work, env, entry);
    } else {
        work[0] = args[0];
        work[1] = args[1];
    }
    return 0;
}

s32 btlfx_opPlayMotion(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode != 1) {
        return btlfx_stepPlayMotion(work, env, entry);
    } else {
        work[0] = args[0];
        work[1] = args[1];
    }
    return 0;
}

s32 btlfx_opPlayMotionWait(s32 mode, Unk800D8EA4 *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 8;
    } else if (mode != 1) {
        return btlfx_stepPlayMotionWait(work, env, entry);
    } else {
        work->unit = args[0];
        work->motion = args[1];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opPlayMotionHold(s32 mode, Unk800D8EA4 *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 8;
    } else if (mode != 1) {
        return btlfx_stepPlayMotionHold(work, env, entry);
    } else {
        work->unit = args[0];
        work->motion = args[1];
        work->trimFrames = g_btlfxOpArgs.paramA[0];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opPlayAttackMotion(s32 mode, Unk800D8EA4 *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 8;
    } else if (mode != 1) {
        return btlfx_stepPlayAttackMotion(work, env, entry);
    } else {
        work->unit = args[0];
        work->motion = args[1];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opPlayAttackMotionHold(s32 mode, Unk800D8EA4 *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 8;
    } else if (mode != 1) {
        return btlfx_stepPlayAttackMotionHold(work, env, entry);
    } else {
        work->unit = args[0];
        work->motion = args[1];
        work->trimFrames = g_btlfxOpArgs.paramA[0];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opPlayAttackReturnMotion(s32 mode, Unk800D8EA4 *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 8;
    } else if (mode != 1) {
        return btlfx_stepPlayAttackReturnMotion(work, env, entry);
    } else {
        work->unit = args[0];
        work->motion = args[1];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opResetMotion(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 8;
    } else if (mode != 1) {
        return btlfx_stepResetMotion(work, env, entry);
    } else {
        *work = *args;
    }
    return 0;
}

s32 btlfx_opFileMotion(s32 mode, u8 *work, s32 env, Unk800F78A40 *entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode != 1) {
        return btlfx_stepFileMotion(work, env, entry);
    } else {
        work[0] = args[0];
        work[1] = args[1];
    }
    return 0;
}

s32 btlfx_opFileMotionWait(s32 mode, Unk800D8EA4 *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 8;
    } else if (mode != 1) {
        return btlfx_stepFileMotionWait(work, env, entry);
    } else {
        work->unit = args[0];
        work->motion = args[1];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opReleaseMotion(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode != 1) {
        return btlfx_stepReleaseMotion(work, env, entry);
    } else {
        *work = *args;
    }
    return 0;
}

s32 btlfx_opMoveToUnit(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        work[0] = args[0];
        work[1] = args[1];
        if (btlfx_isSameUnit(work[0], work[1]) != 0) {
            g_btlfxState.moveDone = mode;
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepMoveToUnit((Unk800D9134 *)work, env, (s32 *)entry);
    }
    return 0;
}

s32 btlfx_opRunHome(s32 mode, Unk800D9260 *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 0x10;
    } else if (mode != 1) {
        return btlfx_stepRunHome(work, env, entry);
    } else {
        work->unit = args[0];
        work->target = args[1];
        g_btlfxState.moveDone = 0;
    }
    return 0;
}

s32 btlfx_opArcAtTarget(s32 mode, Unk800D7C9C *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        work->unit = BTLFX_UNIT_CASTER;
        work->target = args[0];
        work->frames = args[1];
        work->dist = g_btlfxOpArgs.paramA[0] | (g_btlfxOpArgs.paramA[1] << 8);
        work->dist2 = g_btlfxOpArgs.paramB[0] | (g_btlfxOpArgs.paramB[1] << 8);
        if (btlfx_isSameUnit(work->unit, work->target) != 0) {
            g_btlfxState.moveDone = mode;
            return 1;
        }
        if (btlfx_isPartyUnit(work->target) == 0) {
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepArcAtTarget(work, env, entry);
    }
    return 0;
}

s32 btlfx_opJumpForward(s32 mode, Unk800D7C9C *work, s32 env, s32 *entry, s32 *args) {
    s32 skip;
    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        work->unit = BTLFX_UNIT_CASTER;
        work->target = args[0];
        work->arg.skipParty = args[1];
        work->playMotion = 1;
        work->dist = g_btlfxOpArgs.paramA[0] | (g_btlfxOpArgs.paramA[1] << 8);
        if ((skip = work->arg.skipParty) == mode && btlfx_isPartyUnit(work->target) != 0) {
            g_btlfxState.moveDone = skip;
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepJumpForward(work, env, entry);
    }
    return 0;
}

s32 btlfx_opJumpHome(s32 mode, Unk800D955C *work, s32 env, s32 *entry, s32 *args) {
    s32 skip;
    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        work->unit = BTLFX_UNIT_CASTER;
        work->target = args[0];
        work->skipParty = args[1];
        work->playMotion = 1;
        if ((skip = work->skipParty) == mode && btlfx_isPartyUnit(work->target) != 0) {
            g_btlfxState.moveDone = skip;
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepJumpHome(work, env, entry);
    }
    return 0;
}

s32 btlfx_opMoveToPoint(s32 mode, Unk800D7E14 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 0x10;
    } else if (mode != 1) {
        return btlfx_stepMoveToPoint(work, env, entry);
    } else {
        work->unit = args[0];
        work->point = args[1];
        work->frames = g_btlfxOpArgs.paramA[0];
        do {
            g_btlfxState.moveDone = 0;
        } while (0);
    }
    return 0;
}

s32 btlfx_opMoveTowardTarget(s32 mode, Unk800D7C9C *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        work->unit = BTLFX_UNIT_CASTER;
        work->target = args[0];
        work->frames = args[1];
        work->dist = g_btlfxOpArgs.paramA[0] | (g_btlfxOpArgs.paramA[1] << 8);
        work->arg.reach = g_btlfxOpArgs.paramB[0];
        if (btlfx_isSameUnit(work->unit, work->target) != 0 || btlfx_isPartyUnit(work->target) != 0) {
            g_btlfxState.moveDone = mode;
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepMoveTowardTarget(work, env, entry);
    }
    return 0;
}

s32 btlfx_opRunToTarget(s32 mode, Unk800D7C9C *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        work->unit = BTLFX_UNIT_CASTER;
        work->target = args[0];
        work->frames = args[1];
        work->dist = g_btlfxOpArgs.paramA[0] | (g_btlfxOpArgs.paramA[1] << 8);
        if (btlfx_isSameUnit(work->unit, work->target) != 0) {
            g_btlfxState.moveDone = mode;
            return 1;
        }
        if (btlfx_isPartyUnit(work->target) != 0) {
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepRunToTarget(work, env, entry);
    }
    return 0;
}

s32 btlfx_opFaceTarget(s32 mode, Unk800D805C *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 0x14;
    } else if (mode == 1) {
        work->unit = args[0];
        work->target = args[1];
        work->frames = g_btlfxOpArgs.paramA[0];
        work->turnAway = g_btlfxOpArgs.paramA[1];
        work->snapToFront = g_btlfxOpArgs.paramB[0];
        work->unk8 = 0;
        if (btlfx_isSameUnit(work->unit, work->target) != 0) {
            g_btlfxState.turnDone = mode;
            return 1;
        }
        g_btlfxState.turnDone = 0;
    } else {
        return btlfx_stepFaceTarget(work, env, entry);
    }
    return 0;
}

s32 btlfx_opFaceHome(s32 mode, Unk800D8124 *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        work->unit = args[0];
        work->target = args[1];
        work->frames = g_btlfxOpArgs.paramA[0];
        work->partyMode = g_btlfxOpArgs.paramA[1];
        if (work->partyMode == BTLFX_FACE_SKIP_PARTY && btlfx_isPartyUnit(work->target) != 0) {
            g_btlfxState.turnDone = mode;
            return 1;
        }
        g_btlfxState.turnDone = 0;
    } else {
        return btlfx_stepFaceHome(work, env, entry);
    }
    return 0;
}

s32 btlfx_opTurnUnit(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode != 1) {
        return btlfx_stepTurnUnit(work, env, entry);
    } else {
        work[0] = args[0];
        work[1] = args[1];
        work[2] = g_btlfxOpArgs.paramA[0];
    }
    return 0;
}

s32 btlfx_opWaitKindTime(s32 mode, u8 *work, s32 env, s32 *entry, s32 *args) {
    s32 waitParam;
    s32 w;
    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        work[0] = args[0];
        work[1] = args[1];
        work[2] = g_btlfxOpArgs.paramA[0];
        if (btlfx_getKindTiming(work[0], work[2]) == 0) {
            g_btlfxState.waitDone = mode;
            return 1;
        }
        if ((waitParam = work[2]) == mode && (w = btlfx_isSameSide(work[0], work[1])) == waitParam) {
            g_btlfxState.waitDone = w;
            return 1;
        }
        g_btlfxState.waitDone = 0;
    } else {
        return btlfx_stepWaitKindTime(work, env, entry);
    }
    return 0;
}

s32 btlfx_opWaitOtherSide(s32 mode, u8 *work, s32 env, s32 *entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        work[0] = args[0];
        work[1] = args[1];
        work[2] = g_btlfxOpArgs.paramA[0];
        if (btlfx_isSameSide(work[0], work[1]) == 0) {
            g_btlfxState.waitDone = mode;
            return 1;
        }
        g_btlfxState.waitDone = 0;
    } else {
        return btlfx_stepWaitOtherSide(work, env, entry);
    }
    return 0;
}

s32 btlfx_opWaitSameSide(s32 mode, u8 *work, s32 env, s32 *entry, s32 *args) {
    s32 sameSide;
    u8 unusedA, unusedB;
    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        work[0] = args[0];
        work[1] = args[1];
        work[2] = g_btlfxOpArgs.paramA[0];
        sameSide = btlfx_isSameSide(work[0], work[1]);
        if (sameSide == mode) {
            g_btlfxState.waitDone = sameSide;
            return 1;
        }
        g_btlfxState.waitDone = 0;
    } else {
        return btlfx_stepWaitSameSide(work, env, entry);
    }
    return 0;
}

s32 btlfx_opCallWeaponScript(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode != 1) {
        return btlfx_stepCallWeaponScript(work, env, entry);
    } else {
        *work = *args;
    }
    return 0;
}

s32 btlfx_opSyncOpposed(s32 mode, u8 *work, s32 env, s32 entry, s32 *args) {
    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        work[0] = args[0];
        work[1] = args[1];
        if (btlfx_isSameSide(work[0], work[1]) == mode) {
            return 1;
        }
    } else {
        return btlfx_stepSyncOpposed(work, env, entry);
    }
    return 0;
}

void btlfx_playUnitMotion(s32 unitId, s32 motionIndex) {
    if (btlfx_isPartyUnit(unitId) != 0) {
        btlfx_setUnitMotion(unitId, btlfx_getUnitMotion(unitId, motionIndex));
    }
}

void btlfx_setFileMotion(Unk800F78A40 *entry, s32 unitId, s32 dataIndex) {
    if (btlfx_isPartyUnit(unitId) != 0) {
        btlfx_setUnitMotion(unitId, btlfx_getFileData(entry->file, dataIndex));
    }
}

void btlfx_resetUnitMotion(s32 unitId, u8 resetRot) {
    BtlUnit *unit = btl_getUnit(unitId);
    btlfx_setUnitMotion(unitId, unit->motions[BTL_MOTION_IDLE]);
    if (resetRot) {
        unit->rot[0] = unit->obj->rotX;
        unit->rot[1] = unit->obj->rotY;
        unit->rot[2] = unit->obj->rotZ;
    }
}

void btlfx_setUnitMotion(s32 unitId, s32 motion) {
    if (btlfx_isPartyUnit(unitId) != 0) {
        BtlUnit *unit = btl_getUnit(unitId);
        BtlModel *model = unit->model;
        if (model != NULL) {
            model->motion = (BtlMotion *)motion;
            unit->obj->frame = 0;
        }
    }
}

s32 btlfx_getUnitMotion(s32 unitId, s32 motionIndex) {
    return btl_getUnit(unitId)->motions[motionIndex];
}

void btlfx_setMotionFrame(s32 unitId, s32 frame) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (frame >= unit->model->motion->frameCount) {
        frame = unit->model->motion->frameCount;
    }
    unit->obj->frame = frame;
}

s32 btlfx_getMotionLength(s32 unitId) {
    return btl_getUnit(unitId)->model->motion->frameCount;
}

u8 btlfx_getMotionFrame(s32 unitId) {
    return btl_getUnit(unitId)->obj->frame;
}

void btlfx_turnUnit(s32 unitId, s32 mode) {
    BtlUnit *unit = btl_getUnit(unitId);
    s16 pos[4];
    s16 origin[4];
    s16 angles[4];
    switch (mode) {
    case 0:
        unit->rot[1] = unit->obj->rotY + 0x800;
        break;
    case 1:
        btlfx_getUnitPos(unitId, pos);
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = 0;
        btlfx_aimAngles(pos, origin, angles);
        btlfx_setUnitRot(unitId, angles);
        break;
    case 2:
        unit->rot[0] = unit->obj->rotX;
        unit->rot[1] = unit->obj->rotY;
        unit->rot[2] = unit->obj->rotZ;
        break;
    }
}

void btlfx_playSwingSound(s32 unitId) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (btlfx_isPartyUnit(unitId) != 0) {
        btl_soundCmd(SND_SE_PLAY, btl_getWeaponSound((u8)unit->flags >> BTL_UNIT_SLOT_SHIFT, 0), 0, btl_soundPan(unitId), SND_VOL_MAX);
    }
}

void btlfx_playHitSound(s32 attackerId, s32 targetId, s32 param) {
    BtlUnit *attacker = btl_getUnit(attackerId);
    BtlUnit *target = btl_getUnit(targetId);
    if (btlfx_isPartyUnit(attackerId) != 0) {
        btl_playHitSound(attacker, target);
    }
}

u32 btlfx_holdMotion(s32 unitId) {
    BtlUnit *unit = btl_getUnit(unitId);
    u32 flags = unit->flags;
    unit->flags = flags | BTL_UNIT_MOTION_HELD;
    return (flags >> BTL_UNIT_MOTION_HELD_BIT) & 1;
}

void btlfx_setMotionHeld(s32 unitId, s32 held) {
    BtlUnit *unit = btl_getUnit(unitId);
    unit->flags = (unit->flags & ~BTL_UNIT_MOTION_HELD) | ((held & 1) << BTL_UNIT_MOTION_HELD_BIT);
}

s32 btlfx_isPartyUnit(s32 unitId) {
    if (unitId == 0xFF) {
        unitId = 0;
    }
    return btl_getUnit(unitId)->flags & BTL_UNIT_PARTY;
}

s32 btlfx_isSameSide(s32 unitIdA, s32 unitIdB) {
    return btlfx_isPartyUnit(unitIdA) == btlfx_isPartyUnit(unitIdB);
}

s32 btlfx_isSameUnit(s32 unitIdA, s32 unitIdB) {
    BtlUnit *unitA;
    if (unitIdA == 0xFF || unitIdB == 0xFF) {
        return 0;
    }
    unitA = btl_getUnit(unitIdA);
    return unitA == btl_getUnit(unitIdB);
}

s32 btlfx_getUnitKind(s32 unitId) {
    s32 idx = BTL_UNIT_INDEX(btl_getUnit(unitId)->flags);
    return g_gameState->gameData->characters[idx].info.b.kind;
}

s32 btlfx_getUnitWeapon(s32 unitId) {
    s32 idx = BTL_UNIT_INDEX(btl_getUnit(unitId)->flags);
    return g_gameState->gameData->characters[idx].equip[0];
}

void btlfx_setUnitVisible(s32 unitId, s32 visible) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (visible != 0) {
        unit->flags &= ~BTL_UNIT_HIDDEN;
    } else {
        unit->flags |= BTL_UNIT_HIDDEN;
    }
}

void btlfx_setWeaponVisible(s32 unitId, s32 visible) {
    BtlUnit *unit = btl_getUnit(unitId);
    if (visible != 0) {
        btl_showWeapon(unit);
    } else {
        btl_hideWeapon(unit);
    }
}

s32 btlfx_isUnitActive(s32 unitId) {
    return btlfx_getUnitStatus(unitId, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) == 0;
}

u32 btlfx_getUnitStatus(s32 unitId, u32 statusMask) {
    return btl_getUnit(unitId)->status & statusMask;
}

s32 btlfx_motionNopZero(void) {
    return 0;
}

s32 btlfx_motionNopNeg(void) {
    return -1;
}

void btlfx_motionNop(void) {
}

s32 btlfx_stepSwingSound(u8 *work, s32 env, s32 entry) {
    btlfx_playSwingSound(*work);
    return 1;
}

s32 btlfx_stepHitSound(u8 *work, s32 env, s32 entry) {
    btlfx_playHitSound(work[0], work[1], work[2]);
    return 1;
}

s32 btlfx_stepUnitSound(u8 *work, s32 env, s32 entry) {
    BtlUnit *unit = btl_getUnit(work[0]);
    switch (work[1]) {
    case 0:
        btl_playUnitSound(unit, SE_BTL_JUMP_LEAP, SND_VOL_MAX);
        break;
    case 1:
        btl_playUnitSound(unit, SE_BTL_SPEAR_RETURN, SND_VOL_MAX);
        break;
    }
    return 1;
}

s32 btlfx_stepPlayMotion(u8 *work, s32 env, s32 entry) {
    btlfx_playUnitMotion(work[0], work[1]);
    return 1;
}

s32 btlfx_stepPlayMotionWait(Unk800D8EA4 *work, s32 env, s32 *entry) {
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_playUnitMotion(work->unit, work->motion);
        work->lastFrame = btlfx_getMotionLength(work->unit) - 1;
    }
    if (frame >= work->lastFrame) {
        g_btlfxState.motionDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepPlayMotionHold(Unk800D8EA4 *work, s32 env, s32 *entry) {
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_playUnitMotion(work->unit, work->motion);
        work->lastFrame = btlfx_getMotionLength(work->unit) - work->trimFrames - 1;
    }
    if (frame >= work->lastFrame) {
        g_btlfxState.motionDone = 1;
        btlfx_holdMotion(work->unit);
        return 1;
    }
    return 0;
}

s32 btlfx_stepPlayAttackMotion(Unk800D8EA4 *work, s32 env, s32 *entry) {
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_playUnitMotion(work->unit, BTL_MOTION_ATTACK);
        work->lastFrame = btlfx_getMotionLength(work->unit) - 1;
    }
    if (frame >= work->lastFrame) {
        g_btlfxState.motionDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepPlayAttackMotionHold(Unk800D8EA4 *work, s32 env, s32 *entry) {
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_playUnitMotion(work->unit, BTL_MOTION_ATTACK);
        work->lastFrame = btlfx_getMotionLength(work->unit) - work->trimFrames - 1;
    }
    if (frame >= work->lastFrame) {
        btlfx_holdMotion(work->unit);
        g_btlfxState.motionDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepPlayAttackReturnMotion(Unk800D8EA4 *work, s32 env, s32 *entry) {
    s32 frame = *entry;
    if (btlfx_isPartyUnit(work->motion) == 0) {
        if (frame == 0) {
            btlfx_playUnitMotion(work->unit, BTL_MOTION_ATTACK_RETURN);
            work->lastFrame = btlfx_getMotionLength(work->unit) - 1;
        }
        if (frame < work->lastFrame) {
            return 0;
        }
    }
    g_btlfxState.motionDone = 1;
    return 1;
}

s32 btlfx_stepResetMotion(u8 *work, s32 env, s32 entry) {
    btlfx_resetUnitMotion(*work, 1);
    return 1;
}

s32 btlfx_stepFileMotion(u8 *work, s32 env, Unk800F78A40 *entry) {
    btlfx_setFileMotion(entry, work[0], work[1]);
    return 1;
}

s32 btlfx_stepFileMotionWait(Unk800D8EA4 *work, s32 env, s32 *entry) {
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_setFileMotion((Unk800F78A40 *)entry, work->unit, work->motion);
        work->lastFrame = btlfx_getMotionLength(work->unit) - 1;
    }
    if (frame >= work->lastFrame) {
        g_btlfxState.motionDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepReleaseMotion(u8 *work, s32 env, s32 entry) {
    btlfx_setMotionHeld(*work, 0);
    return 1;
}

s32 btlfx_stepMoveToUnit(Unk800D9134 *work, s32 env, s32 *entry) {
    s16 dest[4];
    s16 home[4];
    s16 *pos;
    s16 rot[4];
    s32 frame = *entry;
    s32 frames;
    if (btlfx_isPartyUnit(work->target) == 0) {
        if (frame == 0) {
            work->frames = btlfx_getKindTiming(work->unit, 0);
            pos = work->to;
            btlfx_getUnitPos(work->target, pos);
            frames = work->frames;
            if (frames == 1) {
                btlfx_setUnitPos(work->unit, work->to);
                g_btlfxState.moveDone = frames;
                return 1;
            }
        }
        if (btlfx_moveUnitArc(work->unit, work->to, frame + 1, work->frames, 0, work->from) != 0) {
            g_btlfxState.moveDone = 1;
            return 1;
        }
        return 0;
    }
    btlfx_getUnitHomePos(work->target, home);
    btlfx_getUnitRot(work->target, rot);
    btlfx_pointAhead(home, rot, 1000, dest);
    btlfx_setUnitPos(work->unit, dest);
    g_btlfxState.moveDone = 1;
    return 1;
}

s32 btlfx_stepRunHome(Unk800D9260 *work, s32 env, s32 *entry) {
    s16 home[4];
    s32 frame = *entry;
    s32 frames;
    btlfx_getUnitHomePos(work->unit, home);
    if (btlfx_isPartyUnit(work->target) == 0) {
        if (frame == 0) {
            frames = btlfx_getKindTiming(work->unit, 3);
            work->frames = frames;
            if (frames == 1) {
                btlfx_setUnitPos(work->unit, home);
                g_btlfxState.moveDone = frames;
                return 1;
            }
        }
        if (btlfx_moveUnitArc(work->unit, home, frame + 1, work->frames, 0, work->from) != 0) {
            g_btlfxState.moveDone = 1;
            return 1;
        }
        return 0;
    }
    btlfx_setUnitPos(work->unit, home);
    g_btlfxState.moveDone = 1;
    return 1;
}

s32 btlfx_stepArcAtTarget(Unk800D7C9C *work, s32 env, s32 *entry) {
    s16 pos[4];
    s16 home[4];
    s16 angles[4];
    s32 frames;
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_getUnitPos(work->unit, pos);
        btlfx_getUnitHomePos(work->target, home);
        angles[0] = 0;
        angles[1] = 0;
        angles[2] = 0;
        if (pos[1] == 0) {
            if (home[1] != 0) {
                home[1] = 0;
            }
        } else {
            home[1] = pos[1];
        }
        btlfx_pointAhead(home, angles, work->dist, pos);
        btlfx_pointAhead(home, angles, work->dist2, work->to);
        btlfx_setUnitPos(work->unit, pos);
        frames = work->frames;
        if (frames == 1) {
            btlfx_setUnitPos(work->unit, work->to);
            g_btlfxState.moveDone = frames;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(work->unit, work->to, frame + 1, work->frames, 0, work->from) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepJumpForward(Unk800D7C9C *work, s32 env, s32 *entry) {
    s16 pos[4];
    s16 rot[4];
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_getUnitPos(work->unit, pos);
        btlfx_getUnitRot(work->unit, rot);
        btlfx_pointAhead(pos, rot, work->dist, work->to);
        if (work->playMotion != 0) {
            btlfx_playUnitMotion(work->unit, BTL_MOTION_STEP_FORWARD);
        }
        work->frames = btlfx_getMotionLength(work->unit);
    }
    if (btlfx_moveUnitLinear(work->unit, work->to, frame, work->frames, work->from, 0) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepJumpHome(Unk800D955C *work, s32 env, s32 *entry) {
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_getUnitHomePos(work->unit, work->to);
        if (work->playMotion != 0) {
            btlfx_playUnitMotion(work->unit, BTL_MOTION_STEP_HOME);
        }
        work->frames = btlfx_getMotionLength(work->unit);
    }
    if (btlfx_moveUnitLinear(work->unit, work->to, frame, work->frames, work->from, 1) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepMoveToPoint(Unk800D7E14 *work, s32 env, s32 entry) {
    s16 dest[4];
    s32 frame = *(s32 *)entry;
    s32 frames;
    switch (work->point) {
    case 0:
        dest[0] = 0;
        dest[1] = 0;
        dest[2] = -0x100;
        break;
    case 1:
        btlfx_getUnitHomePos(work->unit, dest);
        break;
    case 2:
        dest[0] = ((Unk800F78A4 *)env)->targets->centerX;
        dest[1] = ((Unk800F78A4 *)env)->targets->centerY;
        dest[2] = ((Unk800F78A4 *)env)->targets->centerZ;
        break;
    default:
        dest[0] = 0;
        dest[1] = 0;
        dest[2] = 0;
        break;
    }
    if (frame == 0) {
        frames = work->frames;
        if (frames == 1) {
            btlfx_setUnitPos(work->unit, dest);
            g_btlfxState.moveDone = frames;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(work->unit, dest, frame + 1, work->frames, 0, work->from) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepMoveTowardTarget(Unk800D7C9C *work, s32 env, s32 *entry) {
    s16 pos[4];
    s16 strikePos[4];
    s16 angles[4];
    s32 frames;
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_getUnitPos(work->unit, pos);
        btlfx_getStrikePos(work->unit, work->target, strikePos);
        if (pos[1] == 0) {
            if (strikePos[1] != 0) {
                strikePos[1] = 0;
            }
        } else {
            strikePos[1] = pos[1];
        }
        btlfx_aimAngles(strikePos, pos, angles);
        btlfx_pointAhead(strikePos, angles, work->dist, work->to);
        work->to[0] -= pos[0];
        work->to[1] -= pos[1];
        work->to[2] -= pos[2];
        work->to[0] *= work->arg.reach;
        work->to[1] *= work->arg.reach;
        work->to[2] *= work->arg.reach;
        work->to[0] /= 10;
        work->to[1] /= 10;
        work->to[2] /= 10;
        work->to[0] += pos[0];
        work->to[1] += pos[1];
        work->to[2] += pos[2];
        frames = work->frames;
        if (frames == 1) {
            btlfx_setUnitPos(work->unit, work->to);
            g_btlfxState.moveDone = frames;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(work->unit, work->to, frame + 1, work->frames, 0, work->from) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepRunToTarget(Unk800D7C9C *work, s32 env, s32 *entry) {
    s16 pos[4];
    s16 strikePos[4];
    s16 angles[4];
    s32 frames;
    s32 frame = *entry;
    if (frame == 0) {
        btlfx_getUnitPos(work->unit, pos);
        btlfx_getStrikePos(work->unit, work->target, strikePos);
        if (pos[1] == 0) {
            if (strikePos[1] != 0) {
                strikePos[1] = 0;
            }
        } else {
            strikePos[1] = pos[1];
        }
        btlfx_aimAngles(strikePos, pos, angles);
        btlfx_pointAhead(strikePos, angles, work->dist, work->to);
        frames = work->frames;
        if (frames == 1) {
            btlfx_setUnitPos(work->unit, work->to);
            g_btlfxState.moveDone = frames;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(work->unit, work->to, frame + 1, work->frames, 0, work->from) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepFaceTarget(Unk800D805C *work, s32 env, s32 *entry) {
    s16 pos[4];
    s16 rot[4];
    s16 tgt[4];
    s32 frame = *entry;

    if (work->snapToFront == 0 || btlfx_isPartyUnit(work->target) == 0) {
        btl_getUnit(work->unit);
        if (frame == 0) {
            btlfx_getUnitPos(work->unit, pos);
            if (work->target == BTLFX_UNIT_NONE) {
                tgt[0] = ((Unk800F78A4 *)env)->targets->centerX;
                tgt[1] = ((Unk800F78A4 *)env)->targets->centerY;
                tgt[2] = ((Unk800F78A4 *)env)->targets->centerZ;
            } else {
                btlfx_getUnitHomePos(work->target, tgt);
            }
            btlfx_aimAngles(pos, tgt, work->delta);
            work->delta[0] = 0;
            if (work->turnAway == 1) {
                work->delta[1] += 0x800;
            }
        }
        btlfx_getUnitRot(work->unit, rot);
        rot[0] += work->delta[0] / work->frames;
        rot[1] += work->delta[1] / work->frames;
        rot[2] += work->delta[2] / work->frames;
        btlfx_setUnitRot(work->unit, rot);
        if (frame != work->frames - 1) {
            goto end;
        }
        g_btlfxState.turnDone = 1;
        return 1;
    }
    if (work->target == BTLFX_UNIT_NONE) {
        tgt[0] = ((Unk800F78A4 *)env)->targets->centerX;
        tgt[1] = ((Unk800F78A4 *)env)->targets->centerY;
        tgt[2] = ((Unk800F78A4 *)env)->targets->centerZ;
    } else {
        btlfx_getUnitHomePos(work->target, tgt);
    }
    btlfx_getUnitRot(work->unit, rot);
    rot[1] = 0x800;
    btlfx_setUnitRot(work->unit, rot);
    g_btlfxState.turnDone = 1;
    return 1;
end:
    return 0;
}

s32 btlfx_stepFaceHome(Unk800D8124 *work, s32 env, s32 *entry) {
    s16 rot[4];
    BtlUnit *unit;
    s32 frame = *entry;
    BtlUnitA0 *obj;

    unit = btl_getUnit(work->unit);
    if (work->partyMode == BTLFX_FACE_ALWAYS || btlfx_isPartyUnit(work->target) == 0) {
        if (frame == 0) {
            work->delta[0] = unit->obj->rotX - unit->rot[0];
            work->delta[1] = unit->obj->rotY - unit->rot[1];
            work->delta[2] = unit->obj->rotZ - unit->rot[2];
        }
        btlfx_getUnitRot(work->unit, rot);
        rot[0] += work->delta[0] / work->frames;
        rot[1] += work->delta[1] / work->frames;
        rot[2] += work->delta[2] / work->frames;
        btlfx_setUnitRot(work->unit, rot);
        if (frame != work->frames - 1) {
            goto end;
        }
        g_btlfxState.turnDone = 1;
        return 1;
    }
    unit->rot[0] = unit->obj->rotX;
    obj = unit->obj;
    unit->rot[1] = obj->rotY;
    *(volatile s16 *)&unit->rot[2] = obj->rotZ;
    return g_btlfxState.turnDone = 1;
end:
    return 0;
}

s32 btlfx_stepTurnUnit(u8 *work, s32 env, s32 entry) {
    btlfx_turnUnit(work[0], work[2]);
    return 1;
}

s32 btlfx_stepWaitKindTime(u8 *work, s32 env, s32 *entry) {
    s32 frame = *entry;
    if (frame == btlfx_getKindTiming(work[0], work[2]) - 1) {
        g_btlfxState.waitDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepWaitOtherSide(u8 *work, s32 env, s32 *entry) {
    if (*entry == work[2] - 1) {
        g_btlfxState.waitDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepWaitSameSide(u8 *work, s32 env, s32 *entry) {
    if (*entry == work[2] - 1) {
        g_btlfxState.waitDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepCallWeaponScript(u8 *work, s32 env, s32 entry) {
    s32 scriptId = btlfx_getWeaponScript(*work);
    g_btlfxWeaponScriptId = scriptId;
    btlfx_callScript(scriptId);
    return 1;
}

s32 btlfx_stepSyncOpposed(u8 *work, s32 env, s32 entry) {
    return 1;
}

s32 btlfx_getKindTiming(s32 unitId, s32 timing) {
    s32 kind = btlfx_getUnitKind(unitId);
    return g_btlfxKindTimings[timing][kind];
}

s32 btlfx_getWeaponScript(s32 unitId) {
    s32 weapon = btlfx_getUnitWeapon(unitId);
    if (weapon >= BTLFX_WEAPON_SCRIPT_COUNT) {
        weapon = 0;
    }
    return g_btlfxWeaponScripts[weapon];
}
