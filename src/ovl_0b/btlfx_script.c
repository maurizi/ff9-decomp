#include "ovl_0b.h"

void btlfx_reset(void) {
    g_btlfxState.phase = -1;
    g_btlfxState.partyReload = 0;
    g_btlfxState.modelSwap = 0;
}

s32 btlfx_resolveFirstUse(s32 arg0) {
    s32 id;
    s32 i;
    u8 *p;
    s32 bit;
    GameState *g;

    id = arg0;
    if (btlfx_getScaledRadius(0) >= 0x385) {
        for (p = g_btlfxLargeTargetFx[0]; *p != 0xFF; p += 2) {
            if (p[0] == arg0) {
                id = p[1];
                break;
            }
        }
    }
    bit = 1;
    g = g_gameState;
    for (i = 0; i < 16; i++, bit <<= 1) {
        if (g_btlfxFirstUsePairs[i][0] == id && !(g->gameData->firstUseFxSeen & bit)) {
            id = g_btlfxFirstUsePairs[i][1];
            g->gameData->firstUseFxSeen |= bit;
            break;
        }
        if (g_btlfxFirstUsePairs[i][1] == id) {
            if (!(g->gameData->firstUseFxSeen & bit)) {
                g->gameData->firstUseFxSeen |= bit;
            }
            break;
        }
    }
    return id;
}

s32 btlfx_start(s32 arg0) {
    if (g_btlfxState.phase != -1) {
        return -1;
    }
    g_btlfxState.phase = 0;
    g_btlfxState.effectId = btlfx_resolveFirstUse(arg0);
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
    g_btlfxState.flags = 0x8000;
    btlcam_focusCaster();
    g_btlfxEnv->workBuf = g_gameState->gameData->ctx.battle->work->loadBuf;
    return 0;
}

void btlfx_retryOp(void) {
    g_btlfxState.retry = 1;
    g_btlfxState.pc -= 3;
}

u8 *btlfx_findScript(s32 arg0) {
    s32 i;
    u8 *p;
    s32 n;
    for (i = 0, p = PSX_RAM(u8 *, 0x801E7700); i < 0x200; i++) {
        n = p[0];
        if (p[1] & 0x80) {
            n += 0x100;
        }
        if (n == arg0) {
            break;
        }
        p += (p[1] & 0x7F) * 3 + 2;
    }
    return p + 2;
}

void btlfx_swapVramPages(s32 arg0, u32 *arg1, s32 arg2) {
    RECT r;
    s32 i;
    s32 bit = 1;
    u32 *p;
    if (arg0 != 2) {
        p = (u32 *)(g_gameState->gameData->ctx.battle->partyBufs[0] + 0x50000);
    } else {
        arg2 = 1 << arg1[0x1FFF];
        p = arg1;
    }
    r.w = 0x40;
    r.h = 0x80;
    btlfx_resetMaskBit();
    for (i = 0; i < 4; i++) {
        if (bit & arg2) {
            p -= 0x2000;
            r.x = (i >> 1) * 64 + 0x380;
            r.y = (i & 1) << 7;
            if (arg0 == 0) {
                StoreImage(&r, p);
            } else {
                LoadImage(&r, p);
            }
        }
        if (i & 1) {
            bit <<= 1;
        }
    }
}

void btlfx_callScript(s32 arg0) {
    g_btlfxInCall = 1;
    g_btlfxReturnPc = g_btlfxState.pc;
    g_btlfxState.pc = btlfx_findScript(arg0);
}

void btlfx_runScript(void) {
    RECT r;
    u8 *p;
    s16 op;
    s16 a;
    s16 b;
    s32 cont;
    u16 hdr;
    s32 n;
    s32 k;
    Unk800FEC40 *d = &g_btlfxState;

    if (d->partyReload != 0 && btl_reloadParty(0) == 0) {
        d->partyReload = 0;
        btlfx_restorePartyLooks();
    }
    if (d->modelSwap != 0 && btl_swapPartyModel(d->modelSwap - 1, 0) == 0) {
        d->modelSwap = 0;
        btlfx_restoreUnitLook(0x10);
    }
    switch (d->phase) {
    case 0:
        r.x = 0x300;
        r.y = 0x140;
        r.w = 0x40;
        r.h = 0x10;
        btlfx_resetMaskBit();
        StoreImage(&r, PSX_RAM(u32 *, 0x801E7700));
        g_btlfxInCall = 0;
        d->phase = 1;
        break;
    case 1:
        hdr = PSX_RAM(u16 *, 0x801E7700)[d->effectId];
        d->flags = hdr & 0xFFFE;
        g_btlfxEnv->targets->kind &= ~8;
        if (hdr & 0xC00) {
            btlfx_swapVramPages(0, NULL, (hdr & 0xC00) >> 10);
        }
        if (hdr & 0x8000) {
            r.x = 0x300;
            r.y = 0x150;
            r.w = 0x40;
            r.h = 0xB0;
            btlfx_resetMaskBit();
            StoreImage(&r, PSX_RAM(u32 *, 0x801E7700));
            d->phase = 2;
            d->flags &= ~8;
            d->flags &= ~4;
            break;
        }
        d->ending = 0;
        btlfx_updateCameraEnable();
        btlcam_beginEffect();
        if (!(g_btlfxEnv->targets->kind & 1)) {
            btlfx_playCmdCamera(d->flags);
        }
        if (btlfx_loadFile(d->effectId) == 0) {
            k = (u8)d->flags >> 4;
            if (k == 1) {
                g_btlfxEnv->targets->kind |= 8;
            }
            btlfx_startCasterTask(k);
            d->pc = g_btlfxFileScript;
            d->phase = 3;
        }
        break;
    case 3:
        if (g_btlfxEnv->loader->stage == 3) {
            d->phase = 4;
            g_btlfxEnv->camera->flags |= 2;
        }
        break;
    case 2:
        d->pc = btlfx_findScript(d->effectId);
        d->phase = 4;
        btlfx_showCasterCmdName();
        break;
    case 4:
        /* padded: a zero word follows the first jump table */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        cont = 1;
        do {
            if (d->wait != 0) {
                if (--d->wait > 0) {
                    return;
                }
                d->wait = 0;
            }
            p = d->pc;
            op = p[0];
            a = p[1];
            b = p[2];
            d->pc = p + 3;
            if (op < 0x20) {
                switch (op) {
                case 0:
                    cont = 0;
                    d->pc -= 3;
                    if (btl_areMarkersVisible() == 0) {
                        btl_setMarkersVisible(1);
                    }
                    if (g_btlfxTasks.count == 0) {
                        if (d->flags & 1) {
                            if (d->ending == 0) {
                                btlcam_endEffect();
                                btlfx_initParticleSystem();
                                btlfx_stopAllSounds();
                                btlfx_initSounds();
                                btlfx_reloadCommonData(0);
                                d->ending = 1;
                            } else if (btlfx_reloadCommonData(1) != 0) {
                                d->flags &= ~1;
                            }
                        } else {
                            d->phase = -1;
                            g_btlfxEnv->targets = NULL;
                            btlcam_endEffect();
                            btlfx_initParticleSystem();
                            btlfx_stopAllSounds();
                            btlfx_initSounds();
                            vib_clear();
                        }
                    }
                    break;
                case 1:
                    if (a == 0) {
                        d->wait = b;
                        cont = 0;
                    } else if (d->signals.b[b] == 0) {
                        cont = 0;
                        d->pc -= 3;
                        d->signals.b[b] = 0;
                    }
                    break;
                case 2:
                    d->signals.b[a] = b;
                    break;
                case 3:
                    btlfx_loadNextBlock();
                    break;
                case 4:
                    if (g_btlfxEnv->loader->stage < a) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case 5:
                    d->fileId = a;
                    d->file = btlfx_findFileSlot(a);
                    break;
                case 6:
                    switch (a) {
                    case 2:
                        if (g_btlfxEnv->loader->commonTexLoaded == 0) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    case 1:
                        if (g_btlfxEnv->loader->summonDataState != 2) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    case 0:
                        if (g_btlfxEnv->loader->summonState != 2) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    }
                    break;
                case 8:
                    if (btlfx_isEnemyUnit(0x10) != 0) {
                        g_btlfxState.seqSync = 1;
                        if (!(g_btlfxEnv->targets->kind & 0x10) && d->seqRelease == 0) {
                            cont = 0;
                            d->pc -= 3;
                        }
                    }
                    break;
                case 9:
                    if (btlfx_waitSoundBank(a, b)) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case 10:
                    if (btlfx_loadSoundBank(0)) {
                        cont = 0;
                        btlfx_retryOp();
                    }
                    break;
                case 11:
                    if (a != 0) {
                        if (d->partyReload != 0) {
                            d->pc -= 3;
                        }
                        cont = 0;
                    } else {
                        btl_reloadParty(1);
                        d->partyReload = 1;
                        cont = 0;
                    }
                    break;
                case 12:
                    if (g_btlfxEnv->loader->bufState != 2) {
                        cont = 0;
                        d->pc -= 3;
                    }
                    break;
                case 13:
                    if (a != 0) {
                        if (d->modelSwap != 0) {
                            d->pc -= 3;
                        }
                        cont = 0;
                    } else {
                        n = btlfx_getUnitSlot(b);
                        btl_swapPartyModel(n, 1);
                        n++;
                        d->modelSwap = n;
                        cont = 0;
                    }
                    break;
                case 7:
                    switch (a) {
                    case 0:
                        btlfx_swapVramPages(1, NULL, (d->flags & 0xC00) >> 10);
                        break;
                    case 1:
                        btlfx_swapVramPages(2, (u32 *)btlfx_getFileData(-1, b), 0);
                        break;
                    case 2:
                        if (g_btlfxEnv->loader->commonTexCount < b) {
                            cont = 0;
                            btlfx_retryOp();
                        }
                        break;
                    }
                    break;
                case 14:
                    if (g_btlfxInCall != 0) {
                        g_btlfxInCall = 0;
                        d->pc = g_btlfxReturnPc;
                    }
                    break;
                case 15:
                    if (b == 0) {
                        btlfx_copyWords(g_btlfxVibData, (s32 *)btlfx_getFileData(d->file, a), 0x708);
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
                case 20:
                    break;
                }
            } else {
                if (op < 0x80) {
                    btlfx_spawnTask(op - 0x20, d->file, a, b);
                } else {
                    btlfx_spawnTask(op, d->file, a, b);
                }
            }
        } while (cont);
        break;
    }
}

void btlfx_initTasks(void) {
    s32 i;
    Unk80100C78Entry *p;
    g_btlfxTasks.count = 0;
    btlfx_heapInit(&g_btlfxTaskHeap, 0x1800);
    p = g_btlfxTasks.tasks;
    for (i = 10; i >= 0; i--) {
        p->state.w = 0;
        p++;
    }
}

s32 btlfx_allocTaskMem(Unk80100C78Entry *arg0, s32 arg1) {
    s32 r = 0;
    s32 size = arg1 + 4;
    s32 i = 0;
    Heap *h = g_btlfxLoader.heaps[arg0->file];
    for (; i < 2; i++) {
        if (arg0->sharedHeap != 0 && h != NULL) {
            if (btlfx_heapWrapIfFull(h, size) == 0) {
                r = btlfx_heapAlloc(h, size);
                break;
            }
            arg0->sharedHeap = 0;
        } else {
            if (btlfx_heapWrapIfFull(&g_btlfxTaskHeap, size) == 0) {
                r = btlfx_heapAlloc(&g_btlfxTaskHeap, size);
                break;
            }
            arg0->sharedHeap = 1;
        }
    }
    return r;
}

void btlfx_spawnTask(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Unk80100C78Entry *e;
    s32 i;
    EffFn *tbl;
    s32 idx;
    Unk80100C78 *d = &g_btlfxTasks;
    EffInit init;
    s32 args[2];

    e = d->tasks;
    i = 0;
loop:
    if (e->state.h != 0) {
        i++;
        e++;
        if (i < 11) {
            goto loop;
        }
    }
    d->count++;
    e->state.h = 1;
    e->sharedHeap = 1;
    e->frame = 0;
    e->file = arg1;
    if (arg0 >= 0x80) {
        e->handler = g_btlfxModuleOps[arg1][arg0 - 0x80];
    } else {
        if (arg0 < 0x30) {
            e->handler = g_btlfxResidentOps[arg0];
        } else {
            e->handler = g_btlfxUnitOps[arg0 - 0x30];
        }
    }
    init.poolCount = 0;
    init.workSize = 0;
    e->pool = 0;
    e->work = 0;
    if (e->handler(0, 0, 0, 0, &init) == 0) {
        if (init.poolCount != 0) {
            e->pool = (void *)btlfx_allocTaskMem(e, btlfx_poolSize(init.elemSize, init.poolCount));
            btlfx_poolInit(e->pool, e, init.elemSize, init.poolCount, init.elemFn);
        }
        if (init.workSize != 0) {
            e->work = (void *)btlfx_allocTaskMem(e, btlfx_align4(init.workSize));
        }
    }
    args[0] = arg2;
    args[1] = arg3;
    if (e->handler(1, e->work, g_btlfxEnv, e, args) != 0) {
        e->state.h = 0;
        d->count--;
        if (e->work != 0) {
            btlfx_heapFree(e->work);
        }
        if (e->pool != 0) {
            btlfx_heapFree(e->pool);
        }
    }
}

s32 btlfx_updateTasks(void) {
    Unk80100C78 *d = &g_btlfxTasks;
    Unk80100C78Entry *e;
    s32 n;
    s32 i;
    s32 r;

    e = d->tasks;
    n = 0;
    for (i = 0; i < 11; i++, e++) {
        if (e->state.h != 0) {
            n++;
            g_btlfxEnv->otZ = 0;
            if (e->handler(2, e->work, g_btlfxEnv, e, NULL) != 0) {
                e->state.h = 2;
            } else {
                e->frame++;
            }
            if (e->pool != NULL) {
                r = btlfx_poolUpdate(e->pool);
            } else {
                r = 0;
            }
            if (r == 0 && e->state.h == 2) {
                e->state.h = 0;
                d->count--;
                if (e->work != NULL) {
                    btlfx_heapFree(e->work);
                }
                if (e->pool != NULL) {
                    btlfx_heapFree(e->pool);
                }
            }
        }
    }
    return n;
}

s32 btlfx_moveUnitArc(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4, s16 *arg5) {
    s16 sp10[4];
    s32 t;
    if (arg2 == 1) {
        btlfx_getUnitPos(arg0, arg5);
    }
    t = (arg2 << 12) / arg3;
    btlfx_lerpVec(arg5, arg1, t, sp10);
    if (arg4 != 0) {
        sp10[1] -= (math_sin(t >> 1) * arg4) >> 12;
    }
    btlfx_setUnitPos(arg0, sp10);
    return arg2 >= arg3;
}

s32 btlfx_moveUnitLinear(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s16 *arg4, s32 arg5) {
    s16 sp10[4];
    s32 t;
    if (arg2 == 0) {
        switch (arg5) {
        case 0:
            btlfx_getUnitHomePos(arg0, arg4);
            break;
        case 1:
            btlfx_getUnitPos(arg0, arg4);
            break;
        default:
            return 1;
        }
    }
    t = ((arg2 + 1) << 12) / arg3;
    if (t > 0x1000) {
        t = 0x1000;
    }
    btlfx_lerpVec(arg4, arg1, t, sp10);
    btlfx_setUnitPos(0x10, sp10);
    return arg2 >= arg3 - 1;
}

s32 btlfx_opSetParamsA(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 1) {
        g_btlfxOpArgs.unk0 = arg4[0];
        g_btlfxOpArgs.unk1 = arg4[1];
    }
    return 1;
}

s32 btlfx_opSetParamsB(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 *arg4) {
    if (arg0 == 1) {
        g_btlfxOpArgs.unk2 = arg4[0];
        g_btlfxOpArgs.unk3 = arg4[4];
    }
    return 1;
}

s32 btlfx_opSwingSound(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        if (btlfx_isPartyUnit(arg1[0]) == 0 || arg1[1] == arg0) {
            return 1;
        }
        return 0;
    } else {
        return btlfx_stepSwingSound(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opHitSound(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = g_btlfxOpArgs.unk0;
        arg1[3] = g_btlfxOpArgs.unk1;
        if (btlfx_isPartyUnit(arg1[0]) == 0) {
            return 1;
        }
        if (arg1[3] == arg0) {
            return 1;
        }
    } else {
        return btlfx_stepHitSound(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opUnitSound(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepUnitSound(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 btlfx_opPlayMotion(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepPlayMotion(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 btlfx_opPlayMotionWait(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepPlayMotionWait(arg1, arg2, arg3);
    } else {
        arg1->unit = arg4[0];
        arg1->motion = arg4[1];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opPlayMotionHold(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepPlayMotionHold(arg1, arg2, arg3);
    } else {
        arg1->unit = arg4[0];
        arg1->motion = arg4[1];
        arg1->trimFrames = g_btlfxOpArgs.unk0;
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opPlayAttackMotion(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepPlayAttackMotion(arg1, arg2, arg3);
    } else {
        arg1->unit = arg4[0];
        arg1->motion = arg4[1];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opPlayAttackMotionHold(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepPlayAttackMotionHold(arg1, arg2, arg3);
    } else {
        arg1->unit = arg4[0];
        arg1->motion = arg4[1];
        arg1->trimFrames = g_btlfxOpArgs.unk0;
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opPlayAttackReturnMotion(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepPlayAttackReturnMotion(arg1, arg2, arg3);
    } else {
        arg1->unit = arg4[0];
        arg1->motion = arg4[1];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opResetMotion(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepResetMotion(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 btlfx_opFileMotion(s32 arg0, u8 *arg1, s32 arg2, Unk800F78A40 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepFileMotion(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
    }
    return 0;
}

s32 btlfx_opFileMotionWait(s32 arg0, Unk800D8EA4 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 8;
    } else if (arg0 != 1) {
        return btlfx_stepFileMotionWait(arg1, arg2, arg3);
    } else {
        arg1->unit = arg4[0];
        arg1->motion = arg4[1];
        g_btlfxState.motionDone = 0;
    }
    return 0;
}

s32 btlfx_opReleaseMotion(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepReleaseMotion(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 btlfx_opMoveToUnit(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x18;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        if (btlfx_isSameUnit(arg1[0], arg1[1]) != 0) {
            g_btlfxState.moveDone = arg0;
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepMoveToUnit((Unk800D9134 *)arg1, arg2, (s32 *)arg3);
    }
    return 0;
}

s32 btlfx_opRunHome(s32 arg0, Unk800D9260 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 != 1) {
        return btlfx_stepRunHome(arg1, arg2, arg3);
    } else {
        arg1->unit = arg4[0];
        arg1->target = arg4[1];
        g_btlfxState.moveDone = 0;
    }
    return 0;
}

s32 btlfx_opArcAtTarget(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unit = 0x10;
        arg1->target = arg4[0];
        arg1->frames = arg4[1];
        arg1->dist = g_btlfxOpArgs.unk0 | (g_btlfxOpArgs.unk1 << 8);
        arg1->dist2 = g_btlfxOpArgs.unk2 | (g_btlfxOpArgs.unk3 << 8);
        if (btlfx_isSameUnit(arg1->unit, arg1->target) != 0) {
            g_btlfxState.moveDone = arg0;
            return 1;
        }
        if (btlfx_isPartyUnit(arg1->target) == 0) {
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepArcAtTarget(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opJumpForward(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unit = 0x10;
        arg1->target = arg4[0];
        arg1->unk2 = arg4[1];
        arg1->playMotion = 1;
        arg1->dist = g_btlfxOpArgs.unk0 | (g_btlfxOpArgs.unk1 << 8);
        if ((v = arg1->unk2) == arg0 && btlfx_isPartyUnit(arg1->target) != 0) {
            g_btlfxState.moveDone = v;
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepJumpForward(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opJumpHome(s32 arg0, Unk800D955C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    if (arg0 == 0) {
        *arg4 = 0x18;
    } else if (arg0 == 1) {
        arg1->unit = 0x10;
        arg1->target = arg4[0];
        arg1->unk2 = arg4[1];
        arg1->playMotion = 1;
        if ((v = arg1->unk2) == arg0 && btlfx_isPartyUnit(arg1->target) != 0) {
            g_btlfxState.moveDone = v;
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepJumpHome(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opMoveToPoint(s32 arg0, Unk800D7E14 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 != 1) {
        return btlfx_stepMoveToPoint(arg1, arg2, arg3);
    } else {
        arg1->unit = arg4[0];
        arg1->point = arg4[1];
        arg1->frames = g_btlfxOpArgs.unk0;
        do {
            g_btlfxState.moveDone = 0;
        } while (0);
    }
    return 0;
}

s32 btlfx_opMoveTowardTarget(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unit = 0x10;
        arg1->target = arg4[0];
        arg1->frames = arg4[1];
        arg1->dist = g_btlfxOpArgs.unk0 | (g_btlfxOpArgs.unk1 << 8);
        arg1->unk2 = g_btlfxOpArgs.unk2;
        if (btlfx_isSameUnit(arg1->unit, arg1->target) != 0 || btlfx_isPartyUnit(arg1->target) != 0) {
            g_btlfxState.moveDone = arg0;
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepMoveTowardTarget(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opRunToTarget(s32 arg0, Unk800D7C9C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x1C;
    } else if (arg0 == 1) {
        arg1->unit = 0x10;
        arg1->target = arg4[0];
        arg1->frames = arg4[1];
        arg1->dist = g_btlfxOpArgs.unk0 | (g_btlfxOpArgs.unk1 << 8);
        if (btlfx_isSameUnit(arg1->unit, arg1->target) != 0) {
            g_btlfxState.moveDone = arg0;
            return 1;
        }
        if (btlfx_isPartyUnit(arg1->target) != 0) {
            return 1;
        }
        g_btlfxState.moveDone = 0;
    } else {
        return btlfx_stepRunToTarget(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opFaceTarget(s32 arg0, Unk800D805C *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x14;
    } else if (arg0 == 1) {
        arg1->unit = arg4[0];
        arg1->target = arg4[1];
        arg1->frames = g_btlfxOpArgs.unk0;
        arg1->turnAway = g_btlfxOpArgs.unk1;
        arg1->snapToFront = g_btlfxOpArgs.unk2;
        arg1->unk8 = 0;
        if (btlfx_isSameUnit(arg1->unit, arg1->target) != 0) {
            g_btlfxState.turnDone = arg0;
            return 1;
        }
        g_btlfxState.turnDone = 0;
    } else {
        return btlfx_stepFaceTarget(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opFaceHome(s32 arg0, Unk800D8124 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 0x10;
    } else if (arg0 == 1) {
        arg1->unit = arg4[0];
        arg1->target = arg4[1];
        arg1->frames = g_btlfxOpArgs.unk0;
        arg1->unk2 = g_btlfxOpArgs.unk1;
        if (arg1->unk2 == 2 && btlfx_isPartyUnit(arg1->target) != 0) {
            g_btlfxState.turnDone = arg0;
            return 1;
        }
        g_btlfxState.turnDone = 0;
    } else {
        return btlfx_stepFaceHome(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opTurnUnit(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepTurnUnit(arg1, arg2, arg3);
    } else {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = g_btlfxOpArgs.unk0;
    }
    return 0;
}

s32 btlfx_opWaitKindTime(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    s32 w;
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = g_btlfxOpArgs.unk0;
        if (btlfx_getKindTiming(arg1[0], arg1[2]) == 0) {
            g_btlfxState.waitDone = arg0;
            return 1;
        }
        if ((v = arg1[2]) == arg0 && (w = btlfx_isSameSide(arg1[0], arg1[1])) == v) {
            g_btlfxState.waitDone = w;
            return 1;
        }
        g_btlfxState.waitDone = 0;
    } else {
        return btlfx_stepWaitKindTime(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opWaitOtherSide(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = g_btlfxOpArgs.unk0;
        if (btlfx_isSameSide(arg1[0], arg1[1]) == 0) {
            g_btlfxState.waitDone = arg0;
            return 1;
        }
        g_btlfxState.waitDone = 0;
    } else {
        return btlfx_stepWaitOtherSide(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opWaitSameSide(s32 arg0, u8 *arg1, s32 arg2, s32 *arg3, s32 *arg4) {
    s32 v;
    u8 a, b;
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        arg1[2] = g_btlfxOpArgs.unk0;
        v = btlfx_isSameSide(arg1[0], arg1[1]);
        if (v == arg0) {
            g_btlfxState.waitDone = v;
            return 1;
        }
        g_btlfxState.waitDone = 0;
    } else {
        return btlfx_stepWaitSameSide(arg1, arg2, arg3);
    }
    return 0;
}

s32 btlfx_opCallWeaponScript(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 != 1) {
        return btlfx_stepCallWeaponScript(arg1, arg2, arg3);
    } else {
        *arg1 = *arg4;
    }
    return 0;
}

s32 btlfx_opSyncOpposed(s32 arg0, u8 *arg1, s32 arg2, s32 arg3, s32 *arg4) {
    if (arg0 == 0) {
        *arg4 = 4;
    } else if (arg0 == 1) {
        arg1[0] = arg4[0];
        arg1[1] = arg4[1];
        if (btlfx_isSameSide(arg1[0], arg1[1]) == arg0) {
            return 1;
        }
    } else {
        return btlfx_stepSyncOpposed(arg1, arg2, arg3);
    }
    return 0;
}

void btlfx_playUnitMotion(s32 arg0, s32 arg1) {
    if (btlfx_isPartyUnit(arg0) != 0) {
        btlfx_setUnitMotion(arg0, btlfx_getUnitMotion(arg0, arg1));
    }
}

void btlfx_setFileMotion(Unk800F78A40 *arg0, s32 arg1, s32 arg2) {
    if (btlfx_isPartyUnit(arg1) != 0) {
        btlfx_setUnitMotion(arg1, btlfx_getFileData(arg0->file, arg2));
    }
}

void btlfx_resetUnitMotion(s32 arg0, u8 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    btlfx_setUnitMotion(arg0, u->motions[0]);
    if (arg1) {
        u->rot[0] = u->obj->rotX;
        u->rot[1] = u->obj->rotY;
        u->rot[2] = u->obj->rotZ;
    }
}

void btlfx_setUnitMotion(s32 arg0, s32 arg1) {
    if (btlfx_isPartyUnit(arg0) != 0) {
        BtlUnit *u = btl_getUnit(arg0);
        BtlUnit9C *m = u->model;
        if (m != NULL) {
            m->motion = (BtlUnit9CC *)arg1;
            u->obj->frame = 0;
        }
    }
}

s32 btlfx_getUnitMotion(s32 arg0, s32 arg1) {
    return btl_getUnit(arg0)->motions[arg1];
}

void btlfx_setMotionFrame(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 >= u->model->motion->frameCount) {
        arg1 = u->model->motion->frameCount;
    }
    u->obj->frame = arg1;
}

s32 btlfx_getMotionLength(s32 arg0) {
    return btl_getUnit(arg0)->model->motion->frameCount;
}

u8 btlfx_getMotionFrame(s32 arg0) {
    return btl_getUnit(arg0)->obj->frame;
}

void btlfx_turnUnit(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    s16 a[4];
    s16 b[4];
    s16 c[4];
    switch (arg1) {
    case 0:
        u->rot[1] = u->obj->rotY + 0x800;
        break;
    case 1:
        btlfx_getUnitPos(arg0, a);
        b[0] = 0;
        b[1] = 0;
        b[2] = 0;
        btlfx_aimAngles(a, b, c);
        btlfx_setUnitRot(arg0, c);
        break;
    case 2:
        u->rot[0] = u->obj->rotX;
        u->rot[1] = u->obj->rotY;
        u->rot[2] = u->obj->rotZ;
        break;
    }
}

void btlfx_playSwingSound(s32 arg0) {
    BtlUnit *u = btl_getUnit(arg0);
    if (btlfx_isPartyUnit(arg0) != 0) {
        btl_soundCmd(0xD000, btl_getWeaponSound((u8)u->flags >> 5, 0), 0, btl_soundPan(arg0), 0x7F);
    }
}

void btlfx_playHitSound(s32 arg0, s32 arg1, s32 arg2) {
    BtlUnit *a = btl_getUnit(arg0);
    BtlUnit *b = btl_getUnit(arg1);
    if (btlfx_isPartyUnit(arg0) != 0) {
        btl_playHitSound(a, b);
    }
}

u32 btlfx_holdMotion(s32 arg0) {
    BtlUnit *u = btl_getUnit(arg0);
    u32 f = u->flags;
    u->flags = f | 0x200000;
    return (f >> 21) & 1;
}

void btlfx_setMotionHeld(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    u->flags = (u->flags & ~0x200000) | ((arg1 & 1) << 21);
}

s32 btlfx_isPartyUnit(s32 arg0) {
    if (arg0 == 0xFF) {
        arg0 = 0;
    }
    return btl_getUnit(arg0)->flags & 1;
}

s32 btlfx_isSameSide(s32 arg0, s32 arg1) {
    return btlfx_isPartyUnit(arg0) == btlfx_isPartyUnit(arg1);
}

s32 btlfx_isSameUnit(s32 arg0, s32 arg1) {
    BtlUnit *a;
    if (arg0 == 0xFF || arg1 == 0xFF) {
        return 0;
    }
    a = btl_getUnit(arg0);
    return a == btl_getUnit(arg1);
}

s32 btlfx_getUnitKind(s32 arg0) {
    s32 idx = (btl_getUnit(arg0)->flags >> 1) & 0xF;
    return g_gameState->gameData->characters[idx].info.b.kind;
}

s32 btlfx_getUnitWeapon(s32 arg0) {
    s32 idx = (btl_getUnit(arg0)->flags >> 1) & 0xF;
    return g_gameState->gameData->characters[idx].equip[0];
}

void btlfx_setUnitVisible(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 != 0) {
        u->flags &= ~0x400000;
    } else {
        u->flags |= 0x400000;
    }
}

void btlfx_setWeaponVisible(s32 arg0, s32 arg1) {
    BtlUnit *u = btl_getUnit(arg0);
    if (arg1 != 0) {
        btl_showWeapon(u);
    } else {
        btl_hideWeapon(u);
    }
}

s32 btlfx_isUnitActive(s32 arg0) {
    return btlfx_getUnitStatus(arg0, (BTL_STATUS_PETRIFY | BTL_STATUS_VENOM | BTL_STATUS_KO | BTL_STATUS_STOP | BTL_STATUS_FREEZE)) == 0;
}

u32 btlfx_getUnitStatus(s32 arg0, u32 arg1) {
    return btl_getUnit(arg0)->status & arg1;
}

s32 func_800D8C9C(void) {
    return 0;
}

s32 func_800D8CA4(void) {
    return -1;
}

void func_800D8CAC(void) {
}

s32 btlfx_stepSwingSound(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_playSwingSound(*arg0);
    return 1;
}

s32 btlfx_stepHitSound(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_playHitSound(arg0[0], arg0[1], arg0[2]);
    return 1;
}

s32 btlfx_stepUnitSound(u8 *arg0, s32 arg1, s32 arg2) {
    BtlUnit *u = btl_getUnit(arg0[0]);
    switch (arg0[1]) {
    case 0:
        btl_playUnitSound(u, 0x456, 0x7F);
        break;
    case 1:
        btl_playUnitSound(u, 0xAC0, 0x7F);
        break;
    }
    return 1;
}

s32 btlfx_stepPlayMotion(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_playUnitMotion(arg0[0], arg0[1]);
    return 1;
}

s32 btlfx_stepPlayMotionWait(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unit, arg0->motion);
        arg0->lastFrame = btlfx_getMotionLength(arg0->unit) - 1;
    }
    if (v >= arg0->lastFrame) {
        g_btlfxState.motionDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepPlayMotionHold(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unit, arg0->motion);
        arg0->lastFrame = btlfx_getMotionLength(arg0->unit) - arg0->trimFrames - 1;
    }
    if (v >= arg0->lastFrame) {
        g_btlfxState.motionDone = 1;
        btlfx_holdMotion(arg0->unit);
        return 1;
    }
    return 0;
}

s32 btlfx_stepPlayAttackMotion(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unit, 0x17);
        arg0->lastFrame = btlfx_getMotionLength(arg0->unit) - 1;
    }
    if (v >= arg0->lastFrame) {
        g_btlfxState.motionDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepPlayAttackMotionHold(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_playUnitMotion(arg0->unit, 0x17);
        arg0->lastFrame = btlfx_getMotionLength(arg0->unit) - arg0->trimFrames - 1;
    }
    if (v >= arg0->lastFrame) {
        btlfx_holdMotion(arg0->unit);
        g_btlfxState.motionDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepPlayAttackReturnMotion(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (btlfx_isPartyUnit(arg0->motion) == 0) {
        if (v == 0) {
            btlfx_playUnitMotion(arg0->unit, 0x19);
            arg0->lastFrame = btlfx_getMotionLength(arg0->unit) - 1;
        }
        if (v < arg0->lastFrame) {
            return 0;
        }
    }
    g_btlfxState.motionDone = 1;
    return 1;
}

s32 btlfx_stepResetMotion(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_resetUnitMotion(*arg0, 1);
    return 1;
}

s32 btlfx_stepFileMotion(u8 *arg0, s32 arg1, Unk800F78A40 *arg2) {
    btlfx_setFileMotion(arg2, arg0[0], arg0[1]);
    return 1;
}

s32 btlfx_stepFileMotionWait(Unk800D8EA4 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_setFileMotion((Unk800F78A40 *)arg2, arg0->unit, arg0->motion);
        arg0->lastFrame = btlfx_getMotionLength(arg0->unit) - 1;
    }
    if (v >= arg0->lastFrame) {
        g_btlfxState.motionDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepReleaseMotion(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_setMotionHeld(*arg0, 0);
    return 1;
}

s32 btlfx_stepMoveToUnit(Unk800D9134 *p, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 *pos;
    s16 c[4];
    s32 n = *arg2;
    s32 v;
    if (btlfx_isPartyUnit(p->target) == 0) {
        if (n == 0) {
            p->frames = btlfx_getKindTiming(p->unit, 0);
            pos = p->to;
            btlfx_getUnitPos(p->target, pos);
            v = p->frames;
            if (v == 1) {
                btlfx_setUnitPos(p->unit, p->to);
                g_btlfxState.moveDone = v;
                return 1;
            }
        }
        if (btlfx_moveUnitArc(p->unit, p->to, n + 1, p->frames, 0, p->from) != 0) {
            g_btlfxState.moveDone = 1;
            return 1;
        }
        return 0;
    }
    btlfx_getUnitHomePos(p->target, b);
    btlfx_getUnitRot(p->target, c);
    btlfx_pointAhead(b, c, 1000, a);
    btlfx_setUnitPos(p->unit, a);
    g_btlfxState.moveDone = 1;
    return 1;
}

s32 btlfx_stepRunHome(Unk800D9260 *arg0, s32 arg1, s32 *arg2) {
    s16 sp18[4];
    s32 n = *arg2;
    s32 v;
    btlfx_getUnitHomePos(arg0->unit, sp18);
    if (btlfx_isPartyUnit(arg0->target) == 0) {
        if (n == 0) {
            v = btlfx_getKindTiming(arg0->unit, 3);
            arg0->frames = v;
            if (v == 1) {
                btlfx_setUnitPos(arg0->unit, sp18);
                g_btlfxState.moveDone = v;
                return 1;
            }
        }
        if (btlfx_moveUnitArc(arg0->unit, sp18, n + 1, arg0->frames, 0, arg0->from) != 0) {
            g_btlfxState.moveDone = 1;
            return 1;
        }
        return 0;
    }
    btlfx_setUnitPos(arg0->unit, sp18);
    g_btlfxState.moveDone = 1;
    return 1;
}

s32 btlfx_stepArcAtTarget(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s32 m;
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unit, a);
        btlfx_getUnitHomePos(arg0->target, b);
        c[0] = 0;
        c[1] = 0;
        c[2] = 0;
        if (a[1] == 0) {
            if (b[1] != 0) {
                b[1] = 0;
            }
        } else {
            b[1] = a[1];
        }
        btlfx_pointAhead(b, c, arg0->dist, a);
        btlfx_pointAhead(b, c, arg0->dist2, arg0->to);
        btlfx_setUnitPos(arg0->unit, a);
        m = arg0->frames;
        if (m == 1) {
            btlfx_setUnitPos(arg0->unit, arg0->to);
            g_btlfxState.moveDone = m;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unit, arg0->to, v + 1, arg0->frames, 0, arg0->from) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepJumpForward(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unit, a);
        btlfx_getUnitRot(arg0->unit, b);
        btlfx_pointAhead(a, b, arg0->dist, arg0->to);
        if (arg0->playMotion != 0) {
            btlfx_playUnitMotion(arg0->unit, 0x1D);
        }
        arg0->frames = btlfx_getMotionLength(arg0->unit);
    }
    if (btlfx_moveUnitLinear(arg0->unit, arg0->to, v, arg0->frames, arg0->from, 0) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepJumpHome(Unk800D955C *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitHomePos(arg0->unit, arg0->to);
        if (arg0->playMotion != 0) {
            btlfx_playUnitMotion(arg0->unit, 0x1E);
        }
        arg0->frames = btlfx_getMotionLength(arg0->unit);
    }
    if (btlfx_moveUnitLinear(arg0->unit, arg0->to, v, arg0->frames, arg0->from, 1) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepMoveToPoint(Unk800D7E14 *arg0, s32 arg1, s32 arg2) {
    s16 sp18[4];
    s32 n = *(s32 *)arg2;
    s32 v;
    switch (arg0->point) {
    case 0:
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[2] = -0x100;
        break;
    case 1:
        btlfx_getUnitHomePos(arg0->unit, sp18);
        break;
    case 2:
        sp18[0] = ((Unk800F78A4 *)arg1)->targets->centerX;
        sp18[1] = ((Unk800F78A4 *)arg1)->targets->centerY;
        sp18[2] = ((Unk800F78A4 *)arg1)->targets->centerZ;
        break;
    default:
        sp18[0] = 0;
        sp18[1] = 0;
        sp18[2] = 0;
        break;
    }
    if (n == 0) {
        v = arg0->frames;
        if (v == 1) {
            btlfx_setUnitPos(arg0->unit, sp18);
            g_btlfxState.moveDone = v;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unit, sp18, n + 1, arg0->frames, 0, arg0->from) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepMoveTowardTarget(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s32 m;
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unit, a);
        btlfx_getStrikePos(arg0->unit, arg0->target, b);
        if (a[1] == 0) {
            if (b[1] != 0) {
                b[1] = 0;
            }
        } else {
            b[1] = a[1];
        }
        btlfx_aimAngles(b, a, c);
        btlfx_pointAhead(b, c, arg0->dist, arg0->to);
        arg0->to[0] -= a[0];
        arg0->to[1] -= a[1];
        arg0->to[2] -= a[2];
        arg0->to[0] *= arg0->unk2;
        arg0->to[1] *= arg0->unk2;
        arg0->to[2] *= arg0->unk2;
        arg0->to[0] /= 10;
        arg0->to[1] /= 10;
        arg0->to[2] /= 10;
        arg0->to[0] += a[0];
        arg0->to[1] += a[1];
        arg0->to[2] += a[2];
        m = arg0->frames;
        if (m == 1) {
            btlfx_setUnitPos(arg0->unit, arg0->to);
            g_btlfxState.moveDone = m;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unit, arg0->to, v + 1, arg0->frames, 0, arg0->from) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepRunToTarget(Unk800D7C9C *arg0, s32 arg1, s32 *arg2) {
    s16 a[4];
    s16 b[4];
    s16 c[4];
    s32 m;
    s32 v = *arg2;
    if (v == 0) {
        btlfx_getUnitPos(arg0->unit, a);
        btlfx_getStrikePos(arg0->unit, arg0->target, b);
        if (a[1] == 0) {
            if (b[1] != 0) {
                b[1] = 0;
            }
        } else {
            b[1] = a[1];
        }
        btlfx_aimAngles(b, a, c);
        btlfx_pointAhead(b, c, arg0->dist, arg0->to);
        m = arg0->frames;
        if (m == 1) {
            btlfx_setUnitPos(arg0->unit, arg0->to);
            g_btlfxState.moveDone = m;
            return 1;
        }
    }
    if (btlfx_moveUnitArc(arg0->unit, arg0->to, v + 1, arg0->frames, 0, arg0->from) != 0) {
        g_btlfxState.moveDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepFaceTarget(Unk800D805C *arg0, s32 arg1, s32 *arg2) {
    s16 pos[4];
    s16 rot[4];
    s16 tgt[4];
    s32 v = *arg2;

    if (arg0->snapToFront == 0 || btlfx_isPartyUnit(arg0->target) == 0) {
        btl_getUnit(arg0->unit);
        if (v == 0) {
            btlfx_getUnitPos(arg0->unit, pos);
            if (arg0->target == 0xFF) {
                tgt[0] = ((Unk800F78A4 *)arg1)->targets->centerX;
                tgt[1] = ((Unk800F78A4 *)arg1)->targets->centerY;
                tgt[2] = ((Unk800F78A4 *)arg1)->targets->centerZ;
            } else {
                btlfx_getUnitHomePos(arg0->target, tgt);
            }
            btlfx_aimAngles(pos, tgt, arg0->delta);
            arg0->delta[0] = 0;
            if (arg0->turnAway == 1) {
                arg0->delta[1] += 0x800;
            }
        }
        btlfx_getUnitRot(arg0->unit, rot);
        rot[0] += arg0->delta[0] / arg0->frames;
        rot[1] += arg0->delta[1] / arg0->frames;
        rot[2] += arg0->delta[2] / arg0->frames;
        btlfx_setUnitRot(arg0->unit, rot);
        if (v != arg0->frames - 1) {
            goto end;
        }
        g_btlfxState.turnDone = 1;
        return 1;
    }
    if (arg0->target == 0xFF) {
        tgt[0] = ((Unk800F78A4 *)arg1)->targets->centerX;
        tgt[1] = ((Unk800F78A4 *)arg1)->targets->centerY;
        tgt[2] = ((Unk800F78A4 *)arg1)->targets->centerZ;
    } else {
        btlfx_getUnitHomePos(arg0->target, tgt);
    }
    btlfx_getUnitRot(arg0->unit, rot);
    rot[1] = 0x800;
    btlfx_setUnitRot(arg0->unit, rot);
    g_btlfxState.turnDone = 1;
    return 1;
end:
    return 0;
}

s32 btlfx_stepFaceHome(Unk800D8124 *arg0, s32 arg1, s32 *arg2) {
    s16 rot[4];
    BtlUnit *u;
    s32 v = *arg2;
    BtlUnitA0 *a;

    u = btl_getUnit(arg0->unit);
    if (arg0->unk2 == 0 || btlfx_isPartyUnit(arg0->target) == 0) {
        if (v == 0) {
            arg0->delta[0] = u->obj->rotX - u->rot[0];
            arg0->delta[1] = u->obj->rotY - u->rot[1];
            arg0->delta[2] = u->obj->rotZ - u->rot[2];
        }
        btlfx_getUnitRot(arg0->unit, rot);
        rot[0] += arg0->delta[0] / arg0->frames;
        rot[1] += arg0->delta[1] / arg0->frames;
        rot[2] += arg0->delta[2] / arg0->frames;
        btlfx_setUnitRot(arg0->unit, rot);
        if (v != arg0->frames - 1) {
            goto end;
        }
        g_btlfxState.turnDone = 1;
        return 1;
    }
    u->rot[0] = u->obj->rotX;
    a = u->obj;
    u->rot[1] = a->rotY;
    *(volatile s16 *)&u->rot[2] = a->rotZ;
    return g_btlfxState.turnDone = 1;
end:
    return 0;
}

s32 btlfx_stepTurnUnit(u8 *arg0, s32 arg1, s32 arg2) {
    btlfx_turnUnit(arg0[0], arg0[2]);
    return 1;
}

s32 btlfx_stepWaitKindTime(u8 *arg0, s32 arg1, s32 *arg2) {
    s32 v = *arg2;
    if (v == btlfx_getKindTiming(arg0[0], arg0[2]) - 1) {
        g_btlfxState.waitDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepWaitOtherSide(u8 *arg0, s32 arg1, s32 *arg2) {
    if (*arg2 == arg0[2] - 1) {
        g_btlfxState.waitDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepWaitSameSide(u8 *arg0, s32 arg1, s32 *arg2) {
    if (*arg2 == arg0[2] - 1) {
        g_btlfxState.waitDone = 1;
        return 1;
    }
    return 0;
}

s32 btlfx_stepCallWeaponScript(u8 *arg0, s32 arg1, s32 arg2) {
    s32 v = btlfx_getWeaponScript(*arg0);
    g_btlfxWeaponScriptId = v;
    btlfx_callScript(v);
    return 1;
}

s32 btlfx_stepSyncOpposed(u8 *arg0, s32 arg1, s32 arg2) {
    return 1;
}

s32 btlfx_getKindTiming(s32 arg0, s32 arg1) {
    s32 i = btlfx_getUnitKind(arg0);
    return g_btlfxKindTimings[arg1][i];
}

s32 btlfx_getWeaponScript(s32 arg0) {
    s32 v = btlfx_getUnitWeapon(arg0);
    if (v >= 0x55) {
        v = 0;
    }
    return g_btlfxWeaponScripts[v];
}
