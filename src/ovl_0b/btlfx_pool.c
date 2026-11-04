#include "ovl_0b.h"

s32 btlfx_poolNopZero(void) {
    return 0;
}

s32 btlfx_poolSize(s32 elemSize, s32 count) {
    return (btlfx_align4(elemSize) + 4) * count + 0xC;
}

void btlfx_poolInit(Unk800DA1B8 *pool, Unk80100C78Entry *task, s32 elemSize, s32 count, s32 elemFn) {
    s32 sz;
    s32 i;
    u8 *p;

    /* sz in two statements and the do-while around p += sz give sz and p the reference counts
       the original's register allocation needs (sz v1, p a0, i a1). */
    sz = btlfx_align4(elemSize);
    sz += 4;
    p = (u8 *)pool->elems;
    i = 0;
    pool->stride = sz;
    pool->count = count;
    pool->task = (Unk800DA1B88 *)task;
    pool->elemFn = (void *)elemFn;
    if (count > 0) {
    loop:
        *(s32 *)p = 0;
        i++;
        do {
            p += sz;
        } while (0);
        if (i < count) goto loop;
    }
}

void btlfx_poolClear(Unk800DA1B8 *pool) {
    s32 i;
    s32 *p = pool->elems;
    for (i = 0; i < pool->count; i++) {
        *p = 0;
        p = (s32 *)((u8 *)p + pool->stride);
    }
}

void *btlfx_poolSpawn(s16 *p) {
    Unk800DA1B8 *pool;
    Unk800DA1B8 *x;
    s16 *slot;
    s32 i;
    s32 stride;
    u8 *data;

    x = (Unk800DA1B8 *)p;
    pool = (Unk800DA1B8 *)p;
    p = (s16 *)((Unk800DA1B8 *)p)->elems;
    stride = x->stride;
    for (i = 0; i < pool->count; i++) {
        slot = p;
        if (*p == 0) {
            break;
        }
        p = (s16 *)((u8 *)p + stride);
    }
    if (i >= pool->count) {
        return NULL;
    }
    data = (u8 *)(p + 2);
    slot[0] = 1;
    slot[1] = 0;
    pool->elemFn(1, data, g_btlfxEnv, pool->task, 0, pool, pool->task->work);
    /* Probably macro residue: a dead store that flow deletes (no code). Its only effect is that p is
       mentioned last, which keeps p (not pool) the CSE class head so `p += 12` and the header reads stay
       on the original registers. */
    p = NULL;
    return data;
}

s32 btlfx_poolUpdate(Unk800DA1B8 *poolArg) {
    Unk800DA1B8 *pool = poolArg;
    s16 *p = (s16 *)pool->elems;
    s32 i = 0;
    s32 live = i;
    s32 (*fn)(s32, void *, Unk800F78A4 *, Unk800DA1B88 *, s32, struct Unk800DA1B8 *, s32) = pool->elemFn;
    s32 stride = pool->stride;
    for (; i < pool->count; i++, p = (s16 *)((u8 *)p + stride)) {
        if (p[0] != 0) {
            g_btlfxEnv->otZ = 0;
            live++;
            if (fn(2, p + 2, g_btlfxEnv, pool->task, p[1], pool, pool->task->work) != 0) {
                *(s32 *)p = 0;
            } else {
                p[1]++;
            }
        }
    }
    return live;
}

s32 btlfx_findFileSlot(s32 block) {
    s32 i;
    for (i = 0; i < 2; i++) {
        if (block == g_btlfxLoader.slotBlock.h[i]) {
            break;
        }
    }
    return i;
}

s32 btlfx_getFileData(s32 slot, s32 index) {
    s32 *base;
    if (index < 0x40) {
        base = g_btlfxEnv->loader->data[slot];
    } else if (index < 0x80) {
        base = RAM_BTLFX_RING(s32 *);
        index -= 0x40;
    } else if (index < 0xC0) {
        index -= 0x80;
        base = g_btlfxLoader.summon->data[0];
        index &= 0x7F;
    } else {
        base = g_gameState->gameData->ctx.battle->work->loadBuf;
        index -= 0xC0;
    }
    return (s32)base + base[index];
}

s32 btlfx_getFileDataSize(s32 slot, s32 index) {
    s32 start = btlfx_getFileData(slot, index);
    return btlfx_getFileData(slot, index + 1) - start;
}

/* takes size bytes from the effect work area (g_btlfxLoader.workCursor/workLeft), returns the old pointer */
s32 btlfx_takeWork(s32 size) {
    Unk80100DC0 *loader = &g_btlfxLoader;
    s32 old = loader->workCursor;

    loader->workLeft -= size;
    loader->workCursor = old + size;
    return old;
}

void btlfx_resetLoader(void) {
    g_btlfxLoader.state = BTLFX_LD_IDLE;
}

s32 btlfx_loadFile(s32 effectId) {
    s32 ofs;
    s32 base;

    if (g_btlfxLoader.state != 0) {
        return -1;
    }
    ofs = ((FileEntry16 *)file_find(DIR_BTL_EFFECTS, effectId))->id;
    base = g_gameState->gameData->ctx.battle->dataSector;
    g_btlfxLoader.slotBlock.w = -1;
    g_btlfxLoader.effectId = effectId;
    g_btlfxLoader.slot = 0;
    g_btlfxLoader.block = 0;
    g_btlfxLoader.state = BTLFX_LD_READ_HEADER;
    g_btlfxLoader.stage = BTLFX_STAGE_NONE;
    g_btlfxLoader.summonState = BTLFX_BLOCK_NONE;
    g_btlfxLoader.summonDataState = BTLFX_BLOCK_NONE;
    g_btlfxLoader.commonTexLoaded = 0;
    g_btlfxLoader.bufState = BTLFX_BLOCK_NONE;
    g_btlfxLoader.commonTexCount = 0;
    g_btlfxLoader.soundState = BTLFX_BLOCK_NONE;
    g_btlfxLoader.soundUploading = 0;
    g_btlfxLoader.soundBlocks = 0;
    g_btlfxLoader.soundUploaded = 0;
    g_btlfxLoader.modArg = 0;
    g_btlfxLoader.heaps[0] = 0;
    g_btlfxLoader.heaps[1] = 0;
    g_btlfxLoader.sector = base + ofs;
    return 0;
}

s32 btlfx_loadNextBlock(void) {
    if (g_btlfxLoader.state != 0) {
        return -1;
    }
    g_btlfxLoader.state = BTLFX_LD_NEXT_BLOCK;
    g_btlfxLoader.stage = BTLFX_STAGE_NONE;
    g_btlfxLoader.summonState = BTLFX_BLOCK_NONE;
    g_btlfxLoader.bufState = BTLFX_BLOCK_NONE;
    g_btlfxLoader.soundState = BTLFX_BLOCK_NONE;
    g_btlfxLoader.soundBlocks = 0;
    g_btlfxLoader.soundUploaded = 0;
    g_btlfxLoader.block++;
    g_btlfxLoader.slot = (g_btlfxLoader.slot + 1) & 1;
    g_btlfxLoader.slotBlock.h[g_btlfxLoader.slot] = -1;
    return 0;
}

s32 btlfx_loadSoundBank(s32 force) {
    if (force == 0) {
        if (g_btlfxLoader.state != BTLFX_LD_WAIT_SOUND && g_btlfxLoader.state != BTLFX_LD_IDLE) {
            return 1;
        }
    }
    akao_uploadSeBank(g_gameState->gameData->ctx.battle->work->loadBuf, 0);
    g_btlfxLoader.soundUploaded = 1;
    g_btlfxLoader.soundUploading = 1;
    return 0;
}

s32 btlfx_waitSoundBank(s32 opArgA, s32 opArgB) {
    if (g_btlfxLoader.soundUploading != 0) {
        if (akao_getSpuTransferState() == 0) {
            g_btlfxLoader.soundUploading = 0;
            akao_sendCmdED(0x80, opArgB);
            return 0;
        }
    }
    return 1;
}

void btlfx_markCommonVram(RECT *rect) {
    if (rect->x >= 0x300 && rect->x + rect->w < 0x381) {
        if (rect->y >= 0x100 && rect->y + rect->h < 0x201) {
            g_btlfxState.flags |= BTLFX_HDR_COMMON_VRAM;
        }
    }
}

void btlfx_uploadTextures(RECT *rect, u32 *pixels, Unk800DA790 *loader) {
    s32 i;
    s32 count = loader->cmdArg;
    Unk800DA790 *ldr;
    RECT *texRect;
    rect->w = 0x40;
    rect->h = 0x80;
    btlfx_resetMaskBit();
    for (i = 0; i < count; i++) {
        ldr = loader;
        texRect = ldr->texRects;
        if (++ldr->texIndex >= 2) {
            rect->x = texRect->x;
            rect->y = texRect->y + 0x80;
            goto next;
        }
        rect->x = texRect->x;
        rect->y = texRect->y;
        if ((u16)texRect->h == 0x80) {
        next:
            ldr->texIndex = 0;
            ldr->texRects++;
        }
        LoadImage(rect, pixels);
        btlfx_markCommonVram(rect);
        pixels += 0x1000;
    }
}

void btlfx_pollDrawSync(void) {
    if (g_btlfxDrawSyncState == 2) {
        EnterCriticalSection();
        if (DrawSync(1) == 0) {
            g_btlfxDrawSyncState = 1;
            FlushCache();
        }
        ExitCriticalSection();
    }
}

s32 btlfx_drawSyncStep(void) {
    s32 ret = 0;
    switch (g_btlfxDrawSyncState) {
    case 0:
        g_btlfxDrawSyncState = 2;
        break;
    case 1:
        g_btlfxDrawSyncState = 0;
        ret = 1;
        break;
    case 2:
        break;
    }
    return ret;
}

void btlfx_updateLoader(void) {
    union {
        RECT r;
        s32 w;
    } rect;
    Unk80100DC0 *loader = &g_btlfxLoader;
    s32 cont;
    s16 stateIdx;
    u16 *cmdPtr;
    u8 *dst;
    u32 readSize;
    s32 dataSize;
    s32 k;
    s32 imageCount;
    s32 ringOfs;
    s32 tmp; /* heap block size, then the common-texture bit (separate locals don't match) */
    u8 *src;
    Unk80100DC054 *dataSummon;
    s32 i;
    s32 pixelCount;
    s32 texIdx;
    s32 *hdr;
    u16 *img;
    Unk80100DC054 *summon;
    u8 *base;
    s32 bit;
    Heap *heap;
    s32 ni;
    u8 *modHdr;
    Unk800DA92CFn fn;

    if (loader->state != 0) {
        do {
            stateIdx = loader->state - 1;
            cont = 0;
            switch (stateIdx) {
            case BTLFX_LD_READ_HEADER - 1:
                cd_readAsync(loader->sector, 0x800, g_btlfxFileHeader, NULL);
                loader->state = BTLFX_LD_WAIT_HEADER;
                loader->sector++;
                break;
            case BTLFX_LD_WAIT_HEADER - 1:
                if (cd_getState() == 0) {
                    cmdPtr = g_btlfxFileHeader;
                    cont = 1;
                    loader->cmd = cmdPtr + 1;
                    loader->state = BTLFX_LD_NEXT_BLOCK;
                    loader->blockCount = cmdPtr[0];
                }
                break;
            case BTLFX_LD_NEXT_BLOCK - 1:
                if (loader->block < loader->blockCount) {
cmdPtr = loader->cmd; cont = 1; loader->blockType = cmdPtr[0]; loader->cmdsLeft = cmdPtr[1]; loader->cmd += 2; loader->state = BTLFX_LD_NEXT_CMD;
                } else {
                    loader->state = BTLFX_LD_IDLE;
                }
                break;
            case BTLFX_LD_NEXT_CMD - 1:
                __asm__(".section .rodata\n\t.word 0\n\t.section .text");
                cmdPtr = loader->cmd;
                loader->cmdKind = ((u8 *)cmdPtr)[0];
                loader->cmdArg = ((u8 *)cmdPtr)[1];
                if (g_btlfxEnv->targets->kind & BTL_FXK_PARTY_UNLOAD) {
                    if (loader->cmdKind == BTLFX_LDCMD_SUMMON_TEX || loader->cmdKind == BTLFX_LDCMD_COMMON_TEX) {
                        if (btlfx_countVisibleParty() != 0) {
                            break;
                        }
                    }
                }
                dst = RAM_BTLFX_MODULE(u8 *);
                if (loader->blockType == 1 && loader->slot != 0) {
                    dst = RAM_BTLFX_MODULE_HI(u8 *);
                }
                readSize = cmdPtr[1] << 11;
                cmdPtr += 2;
                switch (loader->cmdKind) {
                case BTLFX_LDCMD_DATA:
                    dataSize = readSize;
                    loader->stage = BTLFX_STAGE_DATA;
                    if (loader->cmdArg != 0) {
ringOfs = *cmdPtr++ << 11; readSize += ringOfs; loader->ringBase = RAM_BTLFX_RING(u8 *) + ringOfs; dst = RAM_BTLFX_RING(u8 *);
                        loader->ringSize = 0xC800 - ringOfs;
                        loader->data[loader->slot] = loader->ringCursor = loader->ringBase;
                    } else {
                        dst = loader->data[loader->slot] = loader->ringCursor;
                    }
                    loader->ringCursor += dataSize;
                    if (loader->ringCursor >= loader->ringBase + loader->ringSize) {
                        loader->ringCursor = loader->ringBase;
                    }
                    break;
                case BTLFX_LDCMD_RING:
                    dst = loader->ringCursor;
                    loader->ringCursor = dst + readSize;
                    break;
                case BTLFX_LDCMD_TEX_MORE:
                    dst = (u8 *)loader->texData;
                    break;
                case BTLFX_LDCMD_CODE:
                    loader->stage = BTLFX_STAGE_CODE;
                    break;
                case BTLFX_LDCMD_SUMMON_TEX:
                    loader->summonState = BTLFX_BLOCK_READING;
                    btl_unloadPartyModels();
                    dst = (u8 *)g_gameState->gameData->ctx.battle->partyBufs[0];
                    loader->summon = (Unk80100DC054 *)dst;
                    ((Unk80100DC054 *)dst)->unk4C = 0;
                    for (i = 0; i < 16; i++) {
                        loader->summon->data[i] = 0;
                    }
                    break;
                case BTLFX_LDCMD_SUMMON_MODEL:
                    dst = (u8 *)loader->summon + loader->summon->dataOfs;
                    break;
                case BTLFX_LDCMD_SUMMON_DATA:
                    loader->summonDataState = BTLFX_BLOCK_READING;
                    dataSummon = loader->summon;
                    loader->workCursor = (s32)dataSummon->work;
                    loader->workLeft = dataSummon->workSize;
                    dst = (u8 *)loader->workCursor;
                    dataSummon->data[0] = (s32 *)dst;
                    loader->workLeft -= readSize;
                    loader->workCursor += readSize;
                    break;
                case BTLFX_LDCMD_SOUND:
                    loader->soundState = BTLFX_BLOCK_READING;
                    dst = (u8 *)g_gameState->gameData->ctx.battle->work->loadBuf;
                    break;
                case BTLFX_LDCMD_LOADBUF:
                    loader->bufState = BTLFX_BLOCK_READING;
                    dst = (u8 *)g_gameState->gameData->ctx.battle->work->loadBuf;
                    break;
                case BTLFX_LDCMD_COMMON_TEX:
                    if (g_btlfxEnv->targets->kind & BTL_FXK_PARTY_UNLOAD) {
                        btl_unloadPartyModels();
                        dst = (u8 *)g_gameState->gameData->ctx.battle->partyBufs[0];
                    } else {
                        dst = (u8 *)g_gameState->gameData->ctx.battle->work->loadBuf;
                    }
                    break;
                }
                loader->cmd = cmdPtr;
                cd_readAsync(loader->sector, readSize, dst, NULL);
                loader->state = BTLFX_LD_WAIT_CMD;
                loader->sector += readSize >> 11;
                break;
            case BTLFX_LD_WAIT_CMD - 1:
                if (cd_getState() != 0) {
                    break;
                }
                if (loader->cmdKind == BTLFX_LDCMD_COMMON_TEX && (loader->cmdArg & 0xF) && g_btlfxState.castEnded == 0) {
                    break;
                }
                if (loader->cmdKind == BTLFX_LDCMD_CODE && btlfx_drawSyncStep() == 0) {
                    break;
                }
                switch (loader->cmdKind) {
                case BTLFX_LDCMD_DATA:
                    if (loader->cmdArg != 0 && btlfx_getFileDataSize(0, 0x40) != 0) {
                        loader->modArg = btlfx_getFileData(0, 0x40);
                    }
                    tmp = btlfx_getFileDataSize(loader->slot, 0);
                    if (tmp != 0) {
                        heap = (Heap *)btlfx_getFileData(loader->slot, 0);
                        loader->heaps[loader->slot] = heap;
                        btlfx_heapInit(heap, tmp);
                    } else {
                        loader->heaps[loader->slot] = NULL;
                    }
                    break;
                case BTLFX_LDCMD_TEX:
                    dst = RAM_BTLFX_MODULE(u8 *);
                    if (loader->blockType == 1 && loader->slot != 0) {
                        dst = RAM_BTLFX_MODULE_HI(u8 *);
                    }
                    hdr = (s32 *)dst;
                    img = (u16 *)(dst + hdr[1]);
                    k = hdr[0];
                    imageCount = hdr[2];
                    btlfx_resetMaskBit();
                    for (i = 0; i < imageCount; i++) {
                        rect.r.x = img[0];
                        rect.r.y = img[1];
                        rect.r.w = img[2];
                        rect.r.h = img[3];
                        pixelCount = img[2] * img[3];
                        img += 4;
                        LoadImage(&rect.r, (u32 *)img);
                        img += pixelCount;
                    }
                    hdr = (s32 *)(dst + k);
                    img = (u16 *)(dst + hdr[0]);
                    loader->texRects = (RECT *)(hdr + 2);
                    loader->texIndex = 0;
                    loader->texData = (u32 *)img;
                    btlfx_uploadTextures(&rect.r, (u32 *)img, (Unk800DA790 *)loader);
                    break;
                case BTLFX_LDCMD_TEX_MORE:
                    btlfx_uploadTextures(&rect.r, loader->texData, (Unk800DA790 *)loader);
                    break;
                case BTLFX_LDCMD_CODE:
                    dst = RAM_BTLFX_MODULE(u8 *);
                    if (loader->blockType == 1 && loader->slot != 0) {
                        dst = RAM_BTLFX_MODULE_HI(u8 *);
                    }
                    modHdr = *(u8 **)dst;
                    g_btlfxModuleOps[loader->slot] = (EffFn *)(modHdr + 8);
                    fn = ((Unk800DA92CFn *)modHdr)[1];
                    if (fn != NULL) {
                        fn(loader->modArg, g_btlfxEnv, g_btlfxEnv->exports);
                    }
                    loader->stage = BTLFX_STAGE_READY;
                    loader->slotBlock.h[loader->slot] = loader->block;
                    break;
                case BTLFX_LDCMD_SUMMON_TEX:
                    texIdx = 0;
                    summon = loader->summon;
                    dst = (u8 *)summon + summon->dataOfs;
                    rect.r.x = 0x100;
                    rect.r.y = 0xE6;
                    rect.r.w = 0x100;
                    rect.r.h = summon->clutRows;
                    btlfx_resetMaskBit();
                    LoadImage(&rect.r, (u32 *)(dst + summon->clutOfs));
                    k = summon->texCount;
                    for (src = dst; texIdx < k; src += 0x4000, texIdx++) {
                        rect.r.x = (summon->texTpage[texIdx] & TPAGE_X_MASK) << 6;
                        rect.r.y = summon->texY[texIdx] + ((summon->texTpage[texIdx] & TPAGE_Y_BIT) << 4);
                        rect.r.w = 0x40;
                        rect.r.h = 0x80;
                        LoadImage(&rect.r, (u32 *)src);
                        ((s32 *)g_btlfxSummonTexPos)[texIdx * 2] = ((s32 *)g_btlfxSummonTexPos)[texIdx * 2 + 1] = rect.w;
                    }
                    break;
                case BTLFX_LDCMD_SUMMON_MODEL:
                    summon = loader->summon;
                    dst = (u8 *)summon;
                    summon->modelHdr = dst + summon->dataOfs;
                    summon->texAnims = (BtlUnit9C50 *)(dst + summon->texAnimOfs);
                    for (ni = 0; ni < summon->motionCount; ni++) {
                        summon->motions[ni] += (s32)dst;
                    }
                    summon->work = dst + summon->workOfs;
                    loader->workCursor = (s32)summon->work;
                    summon->workSize = 0x50000 - summon->workOfs;
                    loader->workLeft = summon->workSize;
                    btlfx_initSummonModels(&summon->model, 1);
                    loader->summonModel = Hi_RegisterSummonModel(summon, summon->motions[0] != (s32)summon->texAnims ? (s16 *)g_btlfxSummonTexPos : NULL);
                    loader->summonState = BTLFX_BLOCK_DONE;
                    break;
                case BTLFX_LDCMD_SUMMON_DATA:
                    loader->summonDataState = BTLFX_BLOCK_DONE;
                    break;
                case BTLFX_LDCMD_SOUND:
                    loader->soundState = BTLFX_BLOCK_DONE;
                    loader->soundUploading = 1;
                    if (loader->soundBlocks == 0) {
                        btlfx_loadSoundBank(1);
                    }
                    break;
                case BTLFX_LDCMD_LOADBUF:
                    loader->bufState = BTLFX_BLOCK_DONE;
                    break;
                case BTLFX_LDCMD_COMMON_TEX:
                    tmp = 1;
                    loader->commonTexLoaded = 1;
                    if (g_btlfxEnv->targets->kind & BTL_FXK_PARTY_UNLOAD) {
                        btl_unloadPartyModels();
                        base = (void *)g_gameState->gameData->ctx.battle->partyBufs[0];
                    } else {
                        base = (void *)g_gameState->gameData->ctx.battle->work->loadBuf;
                    }
                    rect.r.w = 0x40;
                    rect.r.h = 0x80;
                    btlfx_resetMaskBit();
                    for (i = 0; i < 8; i++) {
                        if (tmp & loader->cmdArg) {
                            if (i < 4) {
                                rect.r.x = ((i >> 1) << 6) + 0x300;
                            } else {
                                rect.r.x = (((i - 4) >> 1) << 6) + 0x140;
                            }
                            rect.r.y = ((i & 1) << 7) + 0x100;
                            LoadImage(&rect.r, (u32 *)base);
                            btlfx_markCommonVram(&rect.r);
                            base += 0x4000;
                        }
                        if (i >= 4 || (i & 1)) {
                            tmp <<= 1;
                        }
                    }
                    loader->commonTexCount++;
                    break;
                }
                if (--loader->cmdsLeft <= 0) {
                    loader->state = BTLFX_LD_IDLE;
                    break;
                }
                if (loader->cmdKind == BTLFX_LDCMD_SOUND) {
                    if (loader->soundBlocks == 0) {
                        loader->state = BTLFX_LD_WAIT_SPU;
                    } else {
                        loader->state = BTLFX_LD_WAIT_SOUND;
                    }
                    cont = 0;
                    loader->soundBlocks++;
                } else {
                    cont = 0;
                    if (loader->cmdKind < BTLFX_LDCMD_DATA || loader->cmdKind == BTLFX_LDCMD_SUMMON_TEX || loader->cmdKind == BTLFX_LDCMD_COMMON_TEX) {
                        cont = 0;
                    } else {
                        cont = 1;
                    }
                    loader->state = BTLFX_LD_NEXT_CMD;
                }
                break;
            case BTLFX_LD_WAIT_SPU - 1:
                if (akao_getSpuTransferState() == 0) {
                    loader->state = BTLFX_LD_NEXT_CMD;
                    cont = 1;
                }
                break;
            case BTLFX_LD_WAIT_SOUND - 1:
                if (loader->soundUploaded != 0) {
                    loader->state = BTLFX_LD_WAIT_SPU;
                    loader->soundUploaded = 0;
                }
                break;
            }
        } while (cont);
    }
}
