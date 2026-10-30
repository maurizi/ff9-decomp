#include "ovl_0b.h"

s32 func_800DA0FC(void) {
    return 0;
}

s32 btlfx_poolSize(s32 arg0, s32 arg1) {
    return (btlfx_align4(arg0) + 4) * arg1 + 0xC;
}

void btlfx_poolInit(Unk800DA1B8 *arg0, Unk80100C78Entry *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sz;
    s32 i;
    u8 *p;

    /* sz in two statements and the do-while around p += sz give sz and p the reference counts
       the original's register allocation needs (sz v1, p a0, i a1). */
    sz = btlfx_align4(arg2);
    sz += 4;
    p = (u8 *)arg0->elems;
    i = 0;
    arg0->stride = sz;
    arg0->count = arg3;
    arg0->task = (Unk800DA1B88 *)arg1;
    arg0->elemFn = (void *)arg4;
    if (arg3 > 0) {
    loop:
        *(s32 *)p = 0;
        i++;
        do {
            p += sz;
        } while (0);
        if (i < arg3) goto loop;
    }
}

void btlfx_poolClear(Unk800DA1B8 *arg0) {
    s32 i;
    s32 *p = arg0->elems;
    for (i = 0; i < arg0->count; i++) {
        *p = 0;
        p = (s32 *)((u8 *)p + arg0->stride);
    }
}

void *btlfx_poolSpawn(s16 *p) {
    Unk800DA1B8 *pool;
    Unk800DA1B8 *x;
    s16 *e;
    s32 i;
    s32 stride;
    u8 *d;

    x = (Unk800DA1B8 *)p;
    pool = (Unk800DA1B8 *)p;
    p = (s16 *)((Unk800DA1B8 *)p)->elems;
    stride = x->stride;
    for (i = 0; i < pool->count; i++) {
        e = p;
        if (*p == 0) {
            break;
        }
        p = (s16 *)((u8 *)p + stride);
    }
    if (i >= pool->count) {
        return NULL;
    }
    d = (u8 *)(p + 2);
    e[0] = 1;
    e[1] = 0;
    pool->elemFn(1, d, g_btlfxEnv, pool->task, 0, pool, pool->task->work);
    /* Probably macro residue: a dead store that flow deletes (no code). Its only effect is that p is
       mentioned last, which keeps p (not pool) the CSE class head so `p += 12` and the header reads stay
       on the original registers. */
    p = NULL;
    return d;
}

s32 btlfx_poolUpdate(Unk800DA1B8 *arg0) {
    Unk800DA1B8 *pool = arg0;
    s16 *p = (s16 *)pool->elems;
    s32 i = 0;
    s32 n = i;
    s32 (*fn)(s32, void *, Unk800F78A4 *, Unk800DA1B88 *, s32, struct Unk800DA1B8 *, s32) = pool->elemFn;
    s32 stride = pool->stride;
    for (; i < pool->count; i++, p = (s16 *)((u8 *)p + stride)) {
        if (p[0] != 0) {
            g_btlfxEnv->otZ = 0;
            n++;
            if (fn(2, p + 2, g_btlfxEnv, pool->task, p[1], pool, pool->task->work) != 0) {
                *(s32 *)p = 0;
            } else {
                p[1]++;
            }
        }
    }
    return n;
}

s32 btlfx_findFileSlot(s32 arg0) {
    s32 i;
    for (i = 0; i < 2; i++) {
        if (arg0 == g_btlfxLoader.slotBlock.h[i]) {
            break;
        }
    }
    return i;
}

s32 btlfx_getFileData(s32 arg0, s32 arg1) {
    s32 *base;
    if (arg1 < 0x40) {
        base = g_btlfxEnv->loader->data[arg0];
    } else if (arg1 < 0x80) {
        base = PSX_RAM(s32 *, 0x801F1700);
        arg1 -= 0x40;
    } else if (arg1 < 0xC0) {
        arg1 -= 0x80;
        base = D_80100E14->data[0];
        arg1 &= 0x7F;
    } else {
        base = g_gameState->gameData->ctx.battle->work->loadBuf;
        arg1 -= 0xC0;
    }
    return (s32)base + base[arg1];
}

s32 btlfx_getFileDataSize(s32 arg0, s32 arg1) {
    s32 v = btlfx_getFileData(arg0, arg1);
    return btlfx_getFileData(arg0, arg1 + 1) - v;
}

/* takes arg0 bytes from the effect work area (g_btlfxLoader.workCursor/workLeft), returns the old pointer */
s32 btlfx_takeWork(s32 arg0) {
    Unk80100DC0 *d = &g_btlfxLoader;
    s32 p = d->workCursor;

    d->workLeft -= arg0;
    d->workCursor = p + arg0;
    return p;
}

void func_800DA4E8(void) {
    D_80100DCC = 0;
}

s32 btlfx_loadFile(s32 arg0) {
    s32 ofs;
    s32 base;

    if (g_btlfxLoader.state != 0) {
        return -1;
    }
    ofs = ((FileEntry16 *)file_find(0xD, arg0))->unk0;
    base = g_gameState->gameData->ctx.battle->dataSector;
    g_btlfxLoader.slotBlock.w = -1;
    g_btlfxLoader.effectId = arg0;
    g_btlfxLoader.slot = 0;
    g_btlfxLoader.block = 0;
    g_btlfxLoader.state = 1;
    g_btlfxLoader.stage = 0;
    g_btlfxLoader.summonState = 0;
    g_btlfxLoader.summonDataState = 0;
    g_btlfxLoader.commonTexLoaded = 0;
    g_btlfxLoader.bufState = 0;
    g_btlfxLoader.commonTexCount = 0;
    g_btlfxLoader.soundState = 0;
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
    g_btlfxLoader.state = 3;
    g_btlfxLoader.stage = 0;
    g_btlfxLoader.summonState = 0;
    g_btlfxLoader.bufState = 0;
    g_btlfxLoader.soundState = 0;
    g_btlfxLoader.soundBlocks = 0;
    g_btlfxLoader.soundUploaded = 0;
    g_btlfxLoader.block++;
    g_btlfxLoader.slot = (g_btlfxLoader.slot + 1) & 1;
    g_btlfxLoader.slotBlock.h[g_btlfxLoader.slot] = -1;
    return 0;
}

s32 btlfx_loadSoundBank(s32 arg0) {
    if (arg0 == 0) {
        if (D_80100DCC != 7 && D_80100DCC != 0) {
            return 1;
        }
    }
    akao_uploadSeBank(g_gameState->gameData->ctx.battle->work->loadBuf, 0);
    g_btlfxLoader.soundUploaded = 1;
    g_btlfxLoader.soundUploading = 1;
    return 0;
}

s32 btlfx_waitSoundBank(s32 arg0, s32 arg1) {
    if (g_btlfxLoader.soundUploading != 0) {
        if (akao_getSpuTransferState() == 0) {
            g_btlfxLoader.soundUploading = 0;
            akao_sendCmdED(0x80, arg1);
            return 0;
        }
    }
    return 1;
}

void btlfx_markCommonVram(RECT *arg0) {
    if (arg0->x >= 0x300 && arg0->x + arg0->w < 0x381) {
        if (arg0->y >= 0x100 && arg0->y + arg0->h < 0x201) {
            g_btlfxState.flags |= 1;
        }
    }
}

void btlfx_uploadTextures(RECT *arg0, u32 *arg1, Unk800DA790 *arg2) {
    s32 i;
    s32 n = arg2->cmdArg;
    Unk800DA790 *e;
    RECT *r;
    arg0->w = 0x40;
    arg0->h = 0x80;
    btlfx_resetMaskBit();
    for (i = 0; i < n; i++) {
        e = arg2;
        r = e->texRects;
        if (++e->texIndex >= 2) {
            arg0->x = r->x;
            arg0->y = r->y + 0x80;
            goto next;
        }
        arg0->x = r->x;
        arg0->y = r->y;
        if ((u16)r->h == 0x80) {
        next:
            e->texIndex = 0;
            e->texRects++;
        }
        LoadImage(arg0, arg1);
        btlfx_markCommonVram(arg0);
        arg1 += 0x1000;
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
    } r;
    Unk80100DC0 *d = &g_btlfxLoader;
    s32 cont;
    s16 st;
    u16 *p;
    u8 *dst;
    u32 rd;
    s32 sz;
    s32 k;
    s32 cnt2;
    s32 x;
    s32 m;
    u8 *src;
    Unk80100DC054 *e2;
    s32 i;
    s32 n;
    s32 cnt;
    s32 *hdr;
    u16 *q;
    Unk80100DC054 *e;
    u8 *base;
    s32 bit;
    Heap *h;
    s32 ni;
    u8 *fh;
    Unk800DA92CFn fn;

    if (d->state != 0) {
        do {
            st = d->state - 1;
            cont = 0;
            switch (st) {
            case 0:
                cd_readAsync(d->sector, 0x800, g_btlfxFileHeader, NULL);
                d->state = 2;
                d->sector++;
                break;
            case 1:
                if (cd_getState() == 0) {
                    p = g_btlfxFileHeader;
                    cont = 1;
                    d->cmd = p + 1;
                    d->state = 3;
                    d->blockCount = p[0];
                }
                break;
            case 2:
                if (d->block < d->blockCount) {
p = d->cmd; cont = 1; d->blockType = p[0]; d->cmdsLeft = p[1]; d->cmd += 2; d->state = 4;
                } else {
                    d->state = 0;
                }
                break;
            case 3:
                __asm__(".section .rodata\n\t.word 0\n\t.section .text");
                p = d->cmd;
                d->cmdKind = ((u8 *)p)[0];
                d->cmdArg = ((u8 *)p)[1];
                if (g_btlfxEnv->targets->kind & 8) {
                    if (d->cmdKind == 4 || d->cmdKind == 9) {
                        if (btlfx_countVisibleParty() != 0) {
                            break;
                        }
                    }
                }
                dst = PSX_RAM(u8 *, 0x801E7700);
                if (d->blockType == 1 && d->slot != 0) {
                    dst = PSX_RAM(u8 *, 0x801EC700);
                }
                rd = p[1] << 11;
                p += 2;
                switch (d->cmdKind) {
                case 2:
                    sz = rd;
                    d->stage = 1;
                    if (d->cmdArg != 0) {
x = *p++ << 11; rd += x; d->ringBase = PSX_RAM(u8 *, 0x801F1700) + x; dst = PSX_RAM(u8 *, 0x801F1700);
                        d->ringSize = 0xC800 - x;
                        d->data[d->slot] = d->ringCursor = d->ringBase;
                    } else {
                        dst = d->data[d->slot] = d->ringCursor;
                    }
                    d->ringCursor += sz;
                    if (d->ringCursor >= d->ringBase + d->ringSize) {
                        d->ringCursor = d->ringBase;
                    }
                    break;
                case 10:
                    dst = d->ringCursor;
                    d->ringCursor = dst + rd;
                    break;
                case 1:
                    dst = (u8 *)d->texData;
                    break;
                case 3:
                    d->stage = 2;
                    break;
                case 4:
                    d->summonState = 1;
                    btl_unloadPartyModels();
                    dst = (u8 *)g_gameState->gameData->ctx.battle->partyBufs[0];
                    d->summon = (Unk80100DC054 *)dst;
                    ((Unk80100DC054 *)dst)->unk4C = 0;
                    for (i = 0; i < 16; i++) {
                        d->summon->data[i] = 0;
                    }
                    break;
                case 5:
                    dst = (u8 *)d->summon + d->summon->dataOfs;
                    break;
                case 6:
                    d->summonDataState = 1;
                    e2 = d->summon;
                    d->workCursor = (s32)e2->work;
                    d->workLeft = e2->workSize;
                    dst = (u8 *)d->workCursor;
                    e2->data[0] = (s32 *)dst;
                    d->workLeft -= rd;
                    d->workCursor += rd;
                    break;
                case 7:
                    d->soundState = 1;
                    dst = (u8 *)g_gameState->gameData->ctx.battle->work->loadBuf;
                    break;
                case 8:
                    d->bufState = 1;
                    dst = (u8 *)g_gameState->gameData->ctx.battle->work->loadBuf;
                    break;
                case 9:
                    if (g_btlfxEnv->targets->kind & 8) {
                        btl_unloadPartyModels();
                        dst = (u8 *)g_gameState->gameData->ctx.battle->partyBufs[0];
                    } else {
                        dst = (u8 *)g_gameState->gameData->ctx.battle->work->loadBuf;
                    }
                    break;
                }
                d->cmd = p;
                cd_readAsync(d->sector, rd, dst, NULL);
                d->state = 5;
                d->sector += rd >> 11;
                break;
            case 4:
                if (cd_getState() != 0) {
                    break;
                }
                if (d->cmdKind == 9 && (d->cmdArg & 0xF) && D_800FEC65 == 0) {
                    break;
                }
                if (d->cmdKind == 3 && btlfx_drawSyncStep() == 0) {
                    break;
                }
                switch (d->cmdKind) {
                case 2:
                    if (d->cmdArg != 0 && btlfx_getFileDataSize(0, 0x40) != 0) {
                        d->modArg = btlfx_getFileData(0, 0x40);
                    }
                    m = btlfx_getFileDataSize(d->slot, 0);
                    if (m != 0) {
                        h = (Heap *)btlfx_getFileData(d->slot, 0);
                        d->heaps[d->slot] = h;
                        btlfx_heapInit(h, m);
                    } else {
                        d->heaps[d->slot] = NULL;
                    }
                    break;
                case 0:
                    dst = PSX_RAM(u8 *, 0x801E7700);
                    if (d->blockType == 1 && d->slot != 0) {
                        dst = PSX_RAM(u8 *, 0x801EC700);
                    }
                    hdr = (s32 *)dst;
                    q = (u16 *)(dst + hdr[1]);
                    k = hdr[0];
                    cnt2 = hdr[2];
                    btlfx_resetMaskBit();
                    for (i = 0; i < cnt2; i++) {
                        r.r.x = q[0];
                        r.r.y = q[1];
                        r.r.w = q[2];
                        r.r.h = q[3];
                        n = q[2] * q[3];
                        q += 4;
                        LoadImage(&r.r, (u32 *)q);
                        q += n;
                    }
                    hdr = (s32 *)(dst + k);
                    q = (u16 *)(dst + hdr[0]);
                    d->texRects = (RECT *)(hdr + 2);
                    d->texIndex = 0;
                    d->texData = (u32 *)q;
                    btlfx_uploadTextures(&r.r, (u32 *)q, (Unk800DA790 *)d);
                    break;
                case 1:
                    btlfx_uploadTextures(&r.r, d->texData, (Unk800DA790 *)d);
                    break;
                case 3:
                    dst = PSX_RAM(u8 *, 0x801E7700);
                    if (d->blockType == 1 && d->slot != 0) {
                        dst = PSX_RAM(u8 *, 0x801EC700);
                    }
                    fh = *(u8 **)dst;
                    D_80100C70[d->slot] = (EffFn *)(fh + 8);
                    fn = ((Unk800DA92CFn *)fh)[1];
                    if (fn != NULL) {
                        fn(d->modArg, g_btlfxEnv, g_btlfxEnv->exports);
                    }
                    d->stage = 3;
                    d->slotBlock.h[d->slot] = d->block;
                    break;
                case 4:
                    cnt = 0;
                    e = d->summon;
                    dst = (u8 *)e + e->dataOfs;
                    r.r.x = 0x100;
                    r.r.y = 0xE6;
                    r.r.w = 0x100;
                    r.r.h = e->clutRows;
                    btlfx_resetMaskBit();
                    LoadImage(&r.r, (u32 *)(dst + e->clutOfs));
                    k = e->texCount;
                    for (src = dst; cnt < k; src += 0x4000, cnt++) {
                        r.r.x = (e->texTpage[cnt] & 0xF) << 6;
                        r.r.y = e->texY[cnt] + ((e->texTpage[cnt] & 0x10) << 4);
                        r.r.w = 0x40;
                        r.r.h = 0x80;
                        LoadImage(&r.r, (u32 *)src);
                        ((s32 *)D_80100D90)[cnt * 2] = ((s32 *)D_80100D90)[cnt * 2 + 1] = r.w;
                    }
                    break;
                case 5:
                    e = d->summon;
                    dst = (u8 *)e;
                    e->modelHdr = dst + e->dataOfs;
                    e->texAnims = (BtlUnit9C50 *)(dst + e->texAnimOfs);
                    for (ni = 0; ni < e->motionCount; ni++) {
                        e->motions[ni] += (s32)dst;
                    }
                    e->work = dst + e->workOfs;
                    d->workCursor = (s32)e->work;
                    e->workSize = 0x50000 - e->workOfs;
                    d->workLeft = e->workSize;
                    btlfx_initSummonModels(&e->model, 1);
                    d->summonModel = Hi_RegisterSummonModel(e, e->motions[0] != (s32)e->texAnims ? (s16 *)D_80100D90 : NULL);
                    d->summonState = 2;
                    break;
                case 6:
                    d->summonDataState = 2;
                    break;
                case 7:
                    d->soundState = 2;
                    d->soundUploading = 1;
                    if (d->soundBlocks == 0) {
                        btlfx_loadSoundBank(1);
                    }
                    break;
                case 8:
                    d->bufState = 2;
                    break;
                case 9:
                    m = 1;
                    d->commonTexLoaded = 1;
                    if (g_btlfxEnv->targets->kind & 8) {
                        btl_unloadPartyModels();
                        base = (void *)g_gameState->gameData->ctx.battle->partyBufs[0];
                    } else {
                        base = (void *)g_gameState->gameData->ctx.battle->work->loadBuf;
                    }
                    r.r.w = 0x40;
                    r.r.h = 0x80;
                    btlfx_resetMaskBit();
                    for (i = 0; i < 8; i++) {
                        if (m & d->cmdArg) {
                            if (i < 4) {
                                r.r.x = ((i >> 1) << 6) + 0x300;
                            } else {
                                r.r.x = (((i - 4) >> 1) << 6) + 0x140;
                            }
                            r.r.y = ((i & 1) << 7) + 0x100;
                            LoadImage(&r.r, (u32 *)base);
                            btlfx_markCommonVram(&r.r);
                            base += 0x4000;
                        }
                        if (i >= 4 || (i & 1)) {
                            m <<= 1;
                        }
                    }
                    d->commonTexCount++;
                    break;
                }
                if (--d->cmdsLeft <= 0) {
                    d->state = 0;
                    break;
                }
                if (d->cmdKind == 7) {
                    if (d->soundBlocks == 0) {
                        d->state = 6;
                    } else {
                        d->state = 7;
                    }
                    cont = 0;
                    d->soundBlocks++;
                } else {
                    cont = 0;
                    if (d->cmdKind < 2 || d->cmdKind == 4 || d->cmdKind == 9) {
                        cont = 0;
                    } else {
                        cont = 1;
                    }
                    d->state = 4;
                }
                break;
            case 5:
                if (akao_getSpuTransferState() == 0) {
                    d->state = 4;
                    cont = 1;
                }
                break;
            case 6:
                if (d->soundUploaded != 0) {
                    d->state = 6;
                    d->soundUploaded = 0;
                }
                break;
            }
        } while (cont);
    }
}
