#include "ff9.h"

void file_loadTim(s32 addr) {
    TimHeader *tim = (TimHeader *)addr;
    TimBlock *blk = &tim->clut;

    if (tim->flag & 8) {
        LoadImage((RECT *)&tim->clut.x, (u32 *)(&tim->clut + 1));
        blk = (TimBlock *)((u32 *)blk + (blk->bnum >> 2));
    }
    LoadImage((RECT *)&blk->x, (u32 *)(blk + 1));
}

void file_getTimRects(TimHeader *tim, RECT **clut, RECT **image) {
    TimBlock *blk;

    if (tim != NULL) {
        blk = &tim->clut;
        if (tim->flag & 8) {
            if (clut != NULL) {
                *clut = (RECT *)&tim->clut.x;
            }
            blk = (TimBlock *)((u32 *)blk + (blk->bnum >> 2));
        }
        if (image != NULL) {
            *image = (RECT *)&blk->x;
        }
    }
}

void file_getTimPos(TimHeader *tim, s32 *cx, s32 *cy, s32 *px, s32 *py) {
    TimBlock *blk;

    if (tim != NULL) {
        blk = &tim->clut;
        if (tim->flag & 8) {
            *cx = tim->clut.x;
            *cy = tim->clut.y;
            blk = (TimBlock *)((u32 *)blk + (blk->bnum >> 2));
        }
        *px = blk->x;
        *py = blk->y;
    }
}

void file_setTimPos(s32 addr, s32 cx, s32 cy, s32 px, s32 py) {
    TimHeader *tim = (TimHeader *)addr;
    TimBlock *blk;

    if (tim != NULL) {
        blk = &tim->clut;
        if (tim->flag & 8) {
            tim->clut.x = cx;
            tim->clut.y = cy;
            blk = (TimBlock *)((u32 *)blk + (blk->bnum >> 2));
        }
        blk->x = px;
        blk->y = py;
    }
}

u32 tpage_getTp(u32 tpage) {
    return (tpage >> 7) & 3;
}

u32 tpage_getAbr(u32 tpage) {
    return (tpage >> 5) & 3;
}

s32 tpage_getX(s32 tpage) {
    return (tpage << 6) & 0x3C0;
}

s32 tpage_getY(s16 tpage) {
    return ((tpage << 4) & 0x100) | (s16)((tpage >> 2) & 0x200);
}

s32 clut_getX(s32 clut) {
    return (clut & 0x3F) << 4;
}

s32 clut_getY(s32 clut) {
    return (s16)clut >> 6;
}

void file_advanceLoadPtr(u8 **ptr, s32 size) {
    *ptr += size + 0xC;
    if (size & 3) {
        *ptr += 4;
        *ptr -= size & 3;
    }
}

s32 file_copyViaVram(s32 dst, s32 src, s32 size, RECT *rc) {
    s32 done = 0;
    RECT *rect;
    u32 left;
    u32 h;
    u32 cols;
    u32 i;

    if (size != 0) {
        while (done < size) {
            left = size - done;
            i = g_vramCopyRectIdx;
            g_vramCopyRectIdx = i + 1;
            rect = &g_vramCopyRects[i];
            if (i >= 5) {
                g_vramCopyRectIdx = 0;
            }
            if (rc != NULL) {
                rect->x = rc->x;
                rect->y = rc->y;
                rect->w = rc->w;
                rect->h = rc->h;
            } else {
                rect->x = g_gameState->drawEnv[g_gameState->gameData->drawBuf].clip.x;
                rect->y = g_gameState->drawEnv[g_gameState->gameData->drawBuf].clip.y;
                rect->w = 320;
                rect->h = 218;
            }
            h = left >> 1;
            cols = h / rect->h;
            if (cols == 0 && left != 0) {
                cols = 1;
                rect->h = h;
                if ((s16)h == 0) {
                    rect->h = h + 1;
                }
            } else if (rect->w < (s32)cols) {
                cols = rect->w;
            }
            rect->w = cols;
            LoadImage(rect, (u32 *)(src + done));
            while (cd_getState() != 0) {
            }
            StoreImage(rect, (u32 *)(dst + done));
            done += rect->w * rect->h * 2;
        }
    }
    return done;
}

void file_initImg(void) {
    CdlFILE fp;
    char *names[4] = { "\\SLUS_012.51;1", "\\SLUS_012.95;1", "\\SLUS_012.96;1", "\\SLUS_012.97;1" };
    s32 i;
    Ff9ImgHeader *hdr;

    g_gameState->disc = DISC_NONE;
    for (i = 0; i < ARRAY_COUNT(names); i++) {
        if (CdSearchFile(&fp, names[i])) {
            g_gameState->disc = 1 << (i % 4);
            break;
        }
    }
    if (g_gameState->disc != DISC_NONE) {
        while (cd_getState()) {
        }
        while ((g_gameState->imgSector = cd_findFile("\\FF9.IMG;1")) == -1) {
            while (cd_getState()) {
            }
        }
        while (cd_getState()) {
        }
        while (cd_getState()) {
        }
        hdr = (Ff9ImgHeader *)((u8 *)g_gameState - 0x1800);
        cd_readAsync(g_gameState->imgSector, 0x800, hdr, NULL);
        while (cd_getState()) {
        }
        g_gameState->dirCount = hdr->dirCount;
        memmove(g_gameState->dirs, hdr->dirs, g_gameState->dirCount * sizeof(DirInfo));
    }
}

void file_loadDirTables(void) {
    GameState *gs;
    u8 *tables;
    DirInfo *dir;
    s32 base;
    s32 size;
    s32 cnt;
    s32 partial;
    u8 *top;
    u8 *data;
    u8 *list8;
    u8 *list11;
    Unk8001DE50 *ent8;
    Unk8001DE50 *ent11;
    Unk8001DE50b *dst;
    u16 key;
    s32 key2; /* s32, unlike key: no zero_extend in the second inner loop, so loop.c can still hoist the constant 2 */

    gs = g_gameState;
    if (!(gs->flags & GS_RELOAD_FILE_TABLES)) {
        return;
    }
    cnt = 0;
    tables = g_loadAreaEnd + 0x14C700;
    size = (gs->dirs[13].tableSector << 11) + gs->dirs[13].count * 2;
    base = gs->dirs[0].tableSector;
    partial = size & 0x800;
    if (partial != 0) {
        size += 0x800 - partial;
    }

    cd_readAsync(gs->imgSector, size, tables, NULL);
    while (cd_getState() != 0) {
    }
    dir = (DirInfo *)(tables + 0x10);
    tables += base << 11;
    do { } while (0); /* empty macro residue: ends cse1's block so the data copy below is kept */

    size = (dir[0].count + 1) * 8;
    /* data == tables; the redundant +/- keeps CSE from folding the copy (the
       original passes a copy of tables in s0 to the first two memmoves) */
    data = tables + (base << 11) - (base << 11);
    top = (u8 *)g_gameState - size;
    gs->dirTables[0] = top;
    memmove(gs->dirTables[0], data, size);

    size = (dir[1].count + 1) * 8;
    top -= size;
    gs->dirTables[1] = top;
    memmove(gs->dirTables[1], data + ((dir[1].tableSector - base) << 11), size);

    size = (dir[10].count + 1) * 8;
    top -= size;
    gs->dirTables[10] = top;
    memmove(gs->dirTables[10], tables + ((dir[10].tableSector - base) << 11), size);

    size = (dir[8].count + 1) * 8;
    top -= size;
    gs->dirTables[8] = top;
    memmove(gs->dirTables[8], tables + ((gs->dirs[8].tableSector - base) << 11), size);

    list8 = tables + ((dir[8].tableSector - base) << 11);
    ent8 = (Unk8001DE50 *)(list8 + dir[8].count * 8) - 1;
    list11 = tables + ((dir[11].tableSector - base) << 11);
    do {
        key = ent8->link;
        ent11 = (Unk8001DE50 *)(list11 + dir[11].count * 8) - 1;
        do {
            if (ent11->id == key) {
                if (ent11->link == 0) {
                    ent11->link = 1;
                    cnt++;
                }
                break;
            }
            ent11--;
        } while ((u8 *)ent11 >= list11);
        ent8--;
    } while ((u8 *)ent8 >= list8);
    size = cnt * 0x10;
    top -= size;
    gs->dirTables[11] = top;
    ent8 = (Unk8001DE50 *)(list8 + dir[8].count * 8) - 1;
    dst = gs->dirTables[11];
    do {
        key2 = ent8->link;
        ent11 = (Unk8001DE50 *)(list11 + dir[11].count * 8) - 1;
        do {
            if (ent11->id == key2) {
                if (ent11->link == 1) {
                    memmove(dst, ent11, 0x10);
                    dst->unk8 = 0xFFFF;
                    dst++;
                    ent11->link = 2;
                }
                break;
            }
            ent11--;
        } while ((u8 *)ent11 >= list11);
        ent8--;
    } while ((u8 *)ent8 >= list8);
    gs->dirs[11].count = cnt * 2;

    size = (dir[12].count + 1) * 8;
    top -= size;
    gs->dirTables[12] = top;
    memmove(gs->dirTables[12], tables + ((dir[12].tableSector - base) << 11), size);

    size = dir[13].count * 2;
    top -= size;
    gs->dirTables[13] = top;
    memmove(gs->dirTables[13], tables + ((dir[13].tableSector - base) << 11), size);
    gs->flags &= ~GS_RELOAD_FILE_TABLES;
}

FileEntry *file_find(s32 dir, s32 id) {
    DirInfo *dirInfo = g_gameState->dirs + dir;
    FileIdEntry *base;
    FileIdEntry *ent;

    if (dirInfo->type == 3) {
        return (FileEntry *)((u16 *)g_gameState->dirTables[13] + id);
    }
    id &= 0xFFFF;
    base = g_gameState->dirTables[dir];
    ent = base + dirInfo->count - 1;
loop:
    if (ent->id == id) {
        return (FileEntry *)ent;
    }
    ent--;
    if (ent >= base) {
        goto loop;
    }
    return NULL;
}

void game_setNextScene(u16 sceneId) {
    switch (g_gameState->mode) {
    case GS_MODE_FIELD:
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->nextId = sceneId;
        break;
    case GS_MODE_BATTLE:
        g_gameState->gameData->ctx.ovl65->work->returnId = sceneId;
        break;
    case GS_MODE_WORLD:
        g_gameState->gameData->ctx.ovl65->worldObj->sceneId = sceneId;
        break;
    }
}

s32 game_getModel(s32 slot) {
    if (slot < 9) {
        return (s32)&g_gameState->gameData->chrModels[slot];
    }
    return (s32)&g_gameState->gameData->npcModels[slot - 9];
}

void game_clearModelSlot(ModelSlot *slot) {
    slot->flags = 0;
    slot->modelId = -1;
    slot->model = NULL;
    slot->actor = NULL;
    slot->unit = NULL;
}
