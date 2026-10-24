#include "ff9.h"

void file_loadTim(s32 addr) {
    TimHeader *tim = (TimHeader *)addr;
    TimBlock *b = &tim->clut;

    if (tim->flag & 8) {
        LoadImage((RECT *)&tim->clut.x, (u32 *)(&tim->clut + 1));
        b = (TimBlock *)((u32 *)b + (b->bnum >> 2));
    }
    LoadImage((RECT *)&b->x, (u32 *)(b + 1));
}

void file_getTimRects(TimHeader *tim, RECT **clut, RECT **image) {
    TimBlock *b;

    if (tim != NULL) {
        b = &tim->clut;
        if (tim->flag & 8) {
            if (clut != NULL) {
                *clut = (RECT *)&tim->clut.x;
            }
            b = (TimBlock *)((u32 *)b + (b->bnum >> 2));
        }
        if (image != NULL) {
            *image = (RECT *)&b->x;
        }
    }
}

void file_getTimPos(TimHeader *tim, s32 *cx, s32 *cy, s32 *px, s32 *py) {
    TimBlock *b;

    if (tim != NULL) {
        b = &tim->clut;
        if (tim->flag & 8) {
            *cx = tim->clut.x;
            *cy = tim->clut.y;
            b = (TimBlock *)((u32 *)b + (b->bnum >> 2));
        }
        *px = b->x;
        *py = b->y;
    }
}

void file_setTimPos(s32 addr, s32 cx, s32 cy, s32 px, s32 py) {
    TimHeader *tim = (TimHeader *)addr;
    TimBlock *b;

    if (tim != NULL) {
        b = &tim->clut;
        if (tim->flag & 8) {
            tim->clut.x = cx;
            tim->clut.y = cy;
            b = (TimBlock *)((u32 *)b + (b->bnum >> 2));
        }
        b->x = px;
        b->y = py;
    }
}

u32 tpage_getTp(u32 a) {
    return (a >> 7) & 3;
}

u32 tpage_getAbr(u32 a) {
    return (a >> 5) & 3;
}

s32 tpage_getX(s32 a) {
    return (a << 6) & 0x3C0;
}

s32 tpage_getY(s16 arg0) {
    return ((arg0 << 4) & 0x100) | (s16)((arg0 >> 2) & 0x200);
}

s32 clut_getX(s32 a) {
    return (a & 0x3F) << 4;
}

s32 clut_getY(s32 a) {
    return (s16)a >> 6;
}

void func_8001DA78(u8 **arg0, s32 arg1) {
    *arg0 += arg1 + 0xC;
    if (arg1 & 3) {
        *arg0 += 4;
        *arg0 -= arg1 & 3;
    }
}

s32 file_copyViaVram(s32 dst, s32 src, s32 size, RECT *rc) {
    s32 done = 0;
    RECT *r;
    u32 left;
    u32 h;
    u32 n;
    u32 i;

    if (size != 0) {
        while (done < size) {
            left = size - done;
            i = D_80068234;
            D_80068234 = i + 1;
            r = &D_80073CE8[i];
            if (i >= 5) {
                D_80068234 = 0;
            }
            if (rc != NULL) {
                r->x = rc->x;
                r->y = rc->y;
                r->w = rc->w;
                r->h = rc->h;
            } else {
                r->x = g_gameState->drawEnv[g_gameState->unk1C->unk8].clip.x;
                r->y = g_gameState->drawEnv[g_gameState->unk1C->unk8].clip.y;
                r->w = 320;
                r->h = 218;
            }
            h = left >> 1;
            n = h / r->h;
            if (n == 0 && left != 0) {
                n = 1;
                r->h = h;
                if ((s16)h == 0) {
                    r->h = h + 1;
                }
            } else if (r->w < (s32)n) {
                n = r->w;
            }
            r->w = n;
            LoadImage(r, (u32 *)(src + done));
            while (cd_getState() != 0) {
            }
            StoreImage(r, (u32 *)(dst + done));
            done += r->w * r->h * 2;
        }
    }
    return done;
}

void file_initImg(void) {
    CdlFILE fp;
    char *names[4] = { "\\SLUS_012.51;1", "\\SLUS_012.95;1", "\\SLUS_012.96;1", "\\SLUS_012.97;1" };
    s32 i;
    Ff9ImgHeader *hdr;

    g_gameState->disc = 0xFF;
    for (i = 0; i < ARRAY_COUNT(names); i++) {
        if (CdSearchFile(&fp, names[i])) {
            g_gameState->disc = 1 << (i % 4);
            break;
        }
    }
    if (g_gameState->disc != 0xFF) {
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
        g_gameState->dirCount = hdr->unk8;
        memmove(g_gameState->dirs, hdr->unk10, g_gameState->dirCount * sizeof(DirInfo));
    }
}

void file_loadDirTables(void) {
    GameState *gs;
    u8 *buf;
    DirInfo *dir;
    s32 base;
    s32 size;
    s32 cnt;
    s32 t;
    u8 *top;
    u8 *data;
    u8 *list8;
    u8 *list11;
    Unk8001DE50 *s;
    Unk8001DE50 *e;
    Unk8001DE50b *dst;
    u16 key;
    s32 key2; /* s32, unlike key: no zero_extend in the second inner loop, so loop.c can still hoist the constant 2 */

    gs = g_gameState;
    if (!(gs->unk0 & GS_RELOAD_FILE_TABLES)) {
        return;
    }
    cnt = 0;
    buf = D_80067944 + 0x14C700;
    size = (((DirInfo *)gs->dirs)[13].unk8 << 11) + ((DirInfo *)gs->dirs)[13].unk4 * 2;
    base = ((DirInfo *)gs->dirs)[0].unk8;
    t = size & 0x800;
    if (t != 0) {
        size += 0x800 - t;
    }

    cd_readAsync(gs->imgSector, size, buf, NULL);
    while (cd_getState() != 0) {
    }
    dir = (DirInfo *)(buf + 0x10);
    buf += base << 11;
    do { } while (0); /* empty macro residue: ends cse1's block so the data copy below is kept */

    size = (dir[0].unk4 + 1) * 8;
    /* data == buf; the redundant +/- keeps CSE from folding the copy (the
       original passes a copy of buf in s0 to the first two memmoves) */
    data = buf + (base << 11) - (base << 11);
    top = (u8 *)g_gameState - size;
    gs->dirTables[0] = top;
    memmove(gs->dirTables[0], data, size);

    size = (dir[1].unk4 + 1) * 8;
    top -= size;
    gs->dirTables[1] = top;
    memmove(gs->dirTables[1], data + ((dir[1].unk8 - base) << 11), size);

    size = (dir[10].unk4 + 1) * 8;
    top -= size;
    gs->dirTables[10] = top;
    memmove(gs->dirTables[10], buf + ((dir[10].unk8 - base) << 11), size);

    size = (dir[8].unk4 + 1) * 8;
    top -= size;
    gs->dirTables[8] = top;
    memmove(gs->dirTables[8], buf + ((((DirInfo *)gs->dirs)[8].unk8 - base) << 11), size);

    list8 = buf + ((dir[8].unk8 - base) << 11);
    s = (Unk8001DE50 *)(list8 + dir[8].unk4 * 8) - 1;
    list11 = buf + ((dir[11].unk8 - base) << 11);
    do {
        key = s->unk2;
        e = (Unk8001DE50 *)(list11 + dir[11].unk4 * 8) - 1;
        do {
            if (e->unk0 == key) {
                if (e->unk2 == 0) {
                    e->unk2 = 1;
                    cnt++;
                }
                break;
            }
            e--;
        } while ((u8 *)e >= list11);
        s--;
    } while ((u8 *)s >= list8);
    size = cnt * 0x10;
    top -= size;
    gs->dirTables[11] = top;
    s = (Unk8001DE50 *)(list8 + dir[8].unk4 * 8) - 1;
    dst = gs->dirTables[11];
    do {
        key2 = s->unk2;
        e = (Unk8001DE50 *)(list11 + dir[11].unk4 * 8) - 1;
        do {
            if (e->unk0 == key2) {
                if (e->unk2 == 1) {
                    memmove(dst, e, 0x10);
                    dst->unk8 = 0xFFFF;
                    dst++;
                    e->unk2 = 2;
                }
                break;
            }
            e--;
        } while ((u8 *)e >= list11);
        s--;
    } while ((u8 *)s >= list8);
    ((DirInfo *)gs->dirs)[11].unk4 = cnt * 2;

    size = (dir[12].unk4 + 1) * 8;
    top -= size;
    gs->dirTables[12] = top;
    memmove(gs->dirTables[12], buf + ((dir[12].unk8 - base) << 11), size);

    size = dir[13].unk4 * 2;
    top -= size;
    gs->dirTables[13] = top;
    memmove(gs->dirTables[13], buf + ((dir[13].unk8 - base) << 11), size);
    gs->unk0 &= ~GS_RELOAD_FILE_TABLES;
}

FileEntry *file_find(s32 a, s32 id) {
    FileDirEntry *d = (FileDirEntry *)g_gameState->dirs + a;
    FileIdEntry *base;
    FileIdEntry *e;

    if (d->unk0 == 3) {
        return (FileEntry *)((u16 *)g_gameState->dirTables[13] + id);
    }
    id &= 0xFFFF;
    base = g_gameState->dirTables[a];
    e = base + d->unk4 - 1;
loop:
    if (e->unk0 == id) {
        return (FileEntry *)e;
    }
    e--;
    if (e >= base) {
        goto loop;
    }
    return NULL;
}

void game_setNextScene(u16 arg0) {
    switch (g_gameState->mode) {
    case 1:
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unkA = arg0;
        break;
    case 2:
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk4 = arg0;
        break;
    case 3:
        g_gameState->unk1C->unk8E4.ovl65->unk8->unk24 = arg0;
        break;
    }
}

s32 game_getModel(s32 arg0) {
    if (arg0 < 9) {
        return (s32)&g_gameState->unk1C->unk8D8[arg0];
    }
    return (s32)&g_gameState->unk1C->unk8E0[arg0 - 9];
}

void func_8001E3A4(Unk8001E3A4 *a) {
    a->unk0 = 0;
    a->unk4 = -1;
    a->unk8 = 0;
    a->unkC = 0;
    a->unk10 = 0;
}
