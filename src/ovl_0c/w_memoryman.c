#include "ovl_0c.h"

s32 w_memorymanHeapAlloc(s32 size) {
    s32 *slot = &g_worldHeapStack[g_worldHeapDepth++];

    if (g_worldHeapDepth >= 0x19U) {
        printf("w_memorymanHeapAlloc:(Error) Heap stack overflow\n");
    } else if (g_worldMapDebugFlags & WM_DEBUG_MEMORY) {
        printf("w_memorymanHeapAlloc: [%d]  =  %x - ", g_worldHeapDepth - 1, g_worldHeapTop);
    }
    *slot = g_worldHeapTop;
    g_worldHeapLastSize = size;
    g_worldHeapTop += size;
    g_worldHeapUsed += size;
    if (g_worldMapDebugFlags & WM_DEBUG_MEMORY) {
        printf("%x (%x/%x)[%x]\n", g_worldHeapTop, g_worldHeapUsed, 0x24800, size);
    }
    return *slot;
}

/* pop the heap stack */
s32 w_memorymanHeapFree(void) {
    s32 *stack;
    s32 depth;
    s32 prev;
    s32 size;

    if (g_worldHeapDepth == 0) {
        printf("w_memorymanHeapFree:no allocated heap.\n");
        return 0;
    }
    if (g_worldMapDebugFlags & WM_DEBUG_MEMORY) {
        printf("w_memorymanHeapFree : [%d]  =  %x - ", g_worldHeapDepth - 1, g_worldHeapTop);
    }
    stack = g_worldHeapStack;
    depth = g_worldHeapDepth - 1;
    prev = stack[depth];
    size = g_worldHeapTop;
    size -= prev;
    g_worldHeapDepth = depth;
    g_worldHeapTop = prev;
    g_worldHeapUsed -= size;
    if (g_worldMapDebugFlags & WM_DEBUG_MEMORY) {
        printf("%x\n", g_worldHeapTop);
    }
    return 1;
}

void wm_heapInit(void) {
    g_worldHeapUsed = 0;
    g_worldHeapDepth = 0;
    g_worldHeapLastSize = 0;
    g_worldHeapTop = g_modeHeap;
    g_modeHeap += 0x24800;
}

void w_memorymanDestructor(void) {
    if (g_worldHeapDepth != 0) {
        printf("w_memorymanDestructor:(Error) Memory leak can be found.\n");
    }
    if (g_worldMapDebugFlags & WM_DEBUG_MEMORY) {
        printf(s_wmStrMemorymanDone);
    }
}

void wm_initClutAnims(void) {
    RECT rect;
    u16 off;
    s32 src;
    s32 i;
    s32 j;
    WmClutAnim *anim;
    s32 dstIdx;
    s32 prevIdx;
    u16 *dst;

    off = 0;
    src = 0;
    memset(&rect, 0, sizeof(rect));
    rect.h = 1;
    g_worldClutAnimFx2 = 0;
    g_worldTexAnimStep[0] = 0;
    g_worldTexAnimStep[1] = 0;
    for (i = 7; i >= 0; i--) {
        g_worldTexScrollAcc[i] = 0;
    }
    for (i = 0; i < 18; i++) {
        anim = &g_worldClutAnims[i];
        rect.x = anim->x + anim->start;
        rect.y = anim->y;
        rect.w = anim->count;
        rect.h = 1;
        if (rect.w == 0) {
            rect.w = 1;
        }
        if (i < 18) {
            if (i >= 15) {
                dst = &g_worldClutBuf[0][off];
                memcpy((u8 *)dst, (u8 *)w_framePackGetPtr(g_worldDataPack, WM_PACK_CLUT_ANIM) + src, (u16)anim->count * 2);
                src += (u16)anim->count * 2;
            } else {
                StoreImage(&rect, (u32 *)&g_worldClutBuf[0][off]);
                DrawSync(0);
            }
        } else {
            StoreImage(&rect, (u32 *)&g_worldClutBuf[0][off]);
            DrawSync(0);
        }
        for (j = 0; j < rect.w; j++) {
            dstIdx = (j + rect.w) % rect.w;
            prevIdx = (j + rect.w - 1) % rect.w;
            g_worldClutBuf[0][off + dstIdx + rect.w] = g_worldClutBuf[0][off + j];
            g_worldClutBuf[1][off + prevIdx + rect.w] = g_worldClutBuf[0][off + j];
            g_worldClutBuf[1][off + prevIdx] = g_worldClutBuf[0][off + j];
        }
        g_worldClutAnimOffset[i] = off;
        g_worldClutAnimFrame[i] = 0;
        g_worldClutAnimAcc[i] = 0;
        off += (u16)anim->count * 2;
    }
}

/* upload TIM idx of table table, offset by (imgDx, imgDy) (image) and (clutDx, clutDy) (clut) */
void wm_loadTim(u32 *table, s32 idx, s32 imgDx, s32 imgDy, s32 clutDx, s32 clutDy, s8 mode) {
    u32 *tim;
    RECT *clut;
    u32 *cdata;
    u32 *img;
    u8 has;
    RECT imgRect;
    RECT r2;
    RECT *imgRectBuf;
    RECT *q2;
    RECT *q3;
    s32 clutLen;
    RECT *rect;
    u32 *data;

    tim = (u32 *)table[idx + 1];
    if ((imgDx | imgDy | clutDx | clutDy) == 0) {
        if (mode != 0) {
            file_loadTim((s32)tim);
            return;
        }
    }
    clut = NULL;
    cdata = NULL;
    has = tim[1] >> 3;
    img = tim + 2;
    if (has) {
        clut = (RECT *)(tim + 3);
        cdata = tim + 5;
        clutLen = (tim[2] >> 2) - 3;
        img = cdata + clutLen;
    }
    rect = (RECT *)++img;
    data = (u32 *)(rect + 1);
    if (g_worldStartup != 0) {
        imgRect = *rect;
        imgRect.x += imgDx;
        imgRect.y += imgDy;
        LoadImage(&imgRect, data);
    } else {
        imgRectBuf = (RECT *)g_worldPrimPtr;
        imgRectBuf->x = rect->x + imgDx;
        imgRectBuf->y = rect->y + imgDy;
        imgRectBuf->w = rect->w;
        imgRectBuf->h = rect->h;
        LoadImage(imgRectBuf, data);
        imgRectBuf++;
        g_worldPrimPtr = (s32)imgRectBuf;
    }
    if (mode != 2) {
        if (has) {
            if (g_worldStartup != 0) {
                r2 = *clut;
                r2.x += clutDx;
                r2.y += clutDy;
                if (r2.w == 0x100) {
                    r2.w = 0xF0;
                }
                LoadImage(&r2, cdata);
            } else {
                q2 = (RECT *)g_worldPrimPtr;
                q2->x = clut->x + clutDx;
                q2->y = clut->y + clutDy;
                q2->w = clut->w;
                q2->h = clut->h;
                if (q2->w == 0x100) {
                    q2->w = 0xF0;
                }
                LoadImage(q2, cdata);
                q2++;
                g_worldPrimPtr = (s32)q2;
            }
        }
    } else {
        q3 = (RECT *)g_worldPrimPtr;
        q3->x = 0x280;
        q3->y = 0xF6;
        q3->w = 0x80;
        q3->h = 1;
        LoadImage(q3, cdata);
        q3++;
        g_worldPrimPtr = (s32)q3;
    }
}

void wm_scrollTextures(void) {
    s32 i;
    s32 x, y, w, h;
    s32 acc;
    s32 rows;
    RECT *rect;

    for (i = 0; i < (s32)ARRAY_COUNT(g_worldTexScrolls); i++) {
        x = g_worldTexScrolls[i].x;
        y = g_worldTexScrolls[i].y;
        w = g_worldTexScrolls[i].w;
        h = g_worldTexScrolls[i].h;
        acc = g_worldTexScrollAcc[i] + g_worldTexScrolls[i].speed;
        rows = (s16)acc >> 8;
        g_worldTexScrollAcc[i] = acc & 0xFF;
        if (rows != 0) {
            rect = (RECT *)g_worldPrimPtr;
            rect->x = x;
            rect->y = y;
            rect->w = w;
            rect->h = rows;
            MoveImage(rect, 0x280, 0xF8);
            rect++;
            rect->y = y + rows;
            rect->x = x;
            rect->w = w;
            rect->h = h - rows;
            MoveImage(rect, x, y);
            rect++;
            rect->x = 0x280;
            rect->y = 0xF8;
            rect->w = w;
            rect->h = rows;
            MoveImage(rect, x, y + h - rows);
            rect++;
            g_worldPrimPtr = (s32)rect;
        }
    }
}

void wm_animTextures(void) {
    s32 interval;

    interval = g_worldTexAnim0Interval;
    if (interval != 0 && g_worldFrameCount % interval == 0) {
        wm_loadTim(g_worldTexAnimPack, g_worldTexAnimSeq[g_worldTexAnimStep[0]] + 8, 0, 0, 0, 0, 0);
        wm_loadTim(g_worldTexAnimPack, g_worldTexAnimSeq[g_worldTexAnimStep[0]], 0, 0, 0, 0, 0);
        g_worldTexAnimStep[0] = (s8)(g_worldTexAnimStep[0] + 1) % 6;
    }
    interval = g_worldTexAnim1Interval;
    if (interval != 0 && (g_worldFrameCount + 2) % interval == 0) {
        wm_loadTim(g_worldTexAnimPack, g_worldTexAnimSeq[g_worldTexAnimStep[1]] + 4, 0, 0, 0, 0, 0);
        wm_loadTim(g_worldTexAnimPack, g_worldTexAnimSeq[g_worldTexAnimStep[1]] + 12, 0, 0, 0, 0, 0);
        g_worldTexAnimStep[1] = (s8)(g_worldTexAnimStep[1] + 1) % 6;
    }
}

void wm_animCluts(void) {
    s8 extraFirst = 0;
    s8 extraLast = -1;
    s8 frmFirst = 0;
    s8 frmLast = -1;
    u8 on = 0;
    s32 frmDy = 0;
    WmFrmbufSlot *slot;

    if (g_worldClutAnimFx2 != 0) {
        if (g_worldFormFlags[WM_FORM_HOKORA_HI] == 0) {
            extraFirst = 0xC;
            extraLast = 0xD;
            frmFirst = 0xF;
            frmLast = 0x10;
        } else {
            extraFirst = 0xE;
            extraLast = 0xE;
            frmFirst = 0x11;
            frmLast = 0x11;
        }
    }
    slot = g_worldFrmbufHead.next;
    if ((slot->texId & WM_TEXID_MASK) == 4) {
        on = 1;
        if (slot->vramArea == 2) {
            frmDy = 8;
        }
    }
    slot = slot->next;
    if ((slot->texId & WM_TEXID_MASK) == 4) {
        on = 1;
        if (slot->vramArea == 2) {
            frmDy = 8;
        }
    }
    wm_stepClutAnims(0, 0xB, 0);
    wm_stepClutAnims(extraFirst, extraLast, 0);
    if (on) {
        wm_stepClutAnims(frmFirst, frmLast, frmDy);
    }
}

/* advance the palette animations first..last and upload the new frames (y offset dy) */
void wm_stepClutAnims(s32 first, s32 last, s32 dy) {
    RECT *rect;
    s32 i;
    s32 frames;

    for (i = first; i <= last; i++) {
        g_worldClutAnimAcc[i] += g_worldClutAnims[i].speed;
        frames = g_worldClutAnimAcc[i] / 256;
        g_worldClutAnimAcc[i] &= 0xFF;
        if (frames != 0) {
            rect = (RECT *)g_worldPrimPtr;
            rect->x = g_worldClutAnims[i].x + g_worldClutAnims[i].start;
            rect->y = g_worldClutAnims[i].y + dy;
            rect->w = g_worldClutAnims[i].count;
            rect->h = 1;
            g_worldClutAnimFrame[i] += frames;
            g_worldClutAnimFrame[i] %= rect->w;
            LoadImage(rect++, (u32 *)&g_worldClutBuf[g_worldClutAnimFrame[i] & 1][g_worldClutAnimOffset[i] + (g_worldClutAnimFrame[i] & 0xFFFE)]);
            g_worldPrimPtr = (s32)rect;
        }
    }
}

void wm_updateVramAnims(void) {
    wm_scrollTextures();
    wm_animTextures();
    wm_animCluts();
}

void wm_loadTimPack(s32 *pack, s32 imgDx, s32 imgDy, s32 clutDx, s32 clutDy) {
    s32 count = *pack & WM_PACK_COUNT_MASK;
    s32 i;

    for (i = 0; i < count; i++) {
        wm_loadTim(pack, i, imgDx, imgDy, clutDx, clutDy, 0);
    }
}

void wm_updateFogParams(void) {
    g_worldFogParams[0].color.r = g_worldWeatherData.work[0].fog[1].color[0];
    g_worldFogParams[0].color.g = g_worldWeatherData.work[0].fog[1].color[1];
    g_worldFogParams[0].color.b = g_worldWeatherData.work[0].fog[1].color[2];
    g_worldFogParams[0].fogOffset = g_worldWeatherData.work[0].fog[1].fogOffset;
    g_worldFogParams[0].fogOffset2 = g_worldWeatherData.work[0].fog[1].fogOffset2;
    g_worldFogParams[1].color.r = g_worldWeatherData.work[0].fog[0].color[0];
    g_worldFogParams[1].color.g = g_worldWeatherData.work[0].fog[0].color[1];
    g_worldFogParams[1].color.b = g_worldWeatherData.work[0].fog[0].color[2];
    g_worldFogParams[1].fogOffset = g_worldWeatherData.work[0].fog[0].fogOffset;
    g_worldFogParams[1].fogOffset2 = g_worldWeatherData.work[0].fog[0].fogOffset2;
    g_worldBackdropFog.color.r = g_worldWeatherData.work[0].fog[2].color[0];
    g_worldBackdropFog.color.g = g_worldWeatherData.work[0].fog[2].color[1];
    g_worldBackdropFog.color.b = g_worldWeatherData.work[0].fog[2].color[2];
    g_worldBackdropFog.fogOffset = g_worldWeatherData.work[0].fog[2].fogOffset;
    g_worldBackdropFog.fogOffset2 = g_worldWeatherData.work[0].fog[2].fogOffset2;
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_memoryman", s_wmStrMemorymanDone);

s32 wm_getBlockHeight(s32 cell, WmBlock *tex, VECTOR *pos, u32 *attr, s32 *idx, WmPolyCache *cache) {
    VECTOR v1;
    VECTOR v3;
    VECTOR v2;
    VECTOR v4;
    VECTOR w1;
    VECTOR w3;
    VECTOR w2;
    VECTOR o1;
    VECTOR o3;
    VECTOR o2;
    WmPoly *polys;
    WmPoly *poly;
    u8 found;
    s32 cachedPoly;
    u16 count;
    u16 *cpoly;
    u16 *ccell;
    u8 *slot;
    s32 nfall;
    s32 fidx;
    s32 fy;
    SVECTOR *verts;
    SVECTOR *va;
    SVECTOR *vb;
    SVECTOR *vc;
    s32 px;
    s32 pz;
    u16 hitCount;
    s32 i;
    s32 k;
    s32 y;
    s32 y0;
    s16 dist;
    s16 best;
    s16 bi;
    WmPoly *pp;
    s32 hitIdx;

    found = 0;
    nfall = 0;
    fidx = -1;
    poly = polys = tex->polys;
    verts = tex->verts;
    count = tex->polyCount;
    px = pos->vx;
    pz = pos->vz;
    cpoly = cache->polys;
    ccell = cache->cells;
    slot = &cache->lastSlot;
    v2.vy = 0;
    v3.vy = 0;
    v1.vy = 0;
    w3.vy = 0;
    w2.vy = 0;
    w1.vy = 0;
    hitCount = 0;
    fy = 0;
    if (g_worldUsePolyCache == 0) {
        cache = NULL;
    }
    if (cache != NULL) {
        for (i = 0; i < 6; i++) {
            k = (*slot + i) % 6;
            if (ccell[k] != cell) {
                continue;
            }
            cachedPoly = cpoly[k];
            if (cachedPoly >= count) {
                continue;
            }
            pp = (WmPoly *)((cachedPoly << 4) + (s32)polys);
            va = &verts[pp->v0];
            vc = &verts[pp->v2];
            vb = &verts[pp->v1];
            v1.vx = vc->vx - va->vx;
            v1.vz = vc->vz - va->vz;
            w1.vx = px - va->vx;
            w1.vz = pz - va->vz;
            OuterProduct0(&v1, &w1, &o1);
            if (o1.vy > 0) {
                continue;
            }
            v2.vx = va->vx - vb->vx;
            v2.vz = va->vz - vb->vz;
            w2.vx = px - vb->vx;
            w2.vz = pz - vb->vz;
            OuterProduct0(&v2, &w2, &o2);
            if (o2.vy > 0) {
                continue;
            }
            v3.vx = vb->vx - vc->vx;
            v3.vz = vb->vz - vc->vz;
            w3.vx = px - vc->vx;
            w3.vz = pz - vc->vz;
            OuterProduct0(&v3, &w3, &o3);
            if (o3.vy > 0) {
                continue;
            }
            if (o1.vy == 0 || o3.vy == 0 || o2.vy == 0) {
                continue;
            }
            v4.vx = vb->vx - va->vx;
            v4.vy = vb->vy - va->vy;
            v4.vz = vb->vz - va->vz;
            v1.vy = vc->vy - va->vy;
            OuterProduct0(&v4, &v1, &o1);
            if (abs(o1.vy) >= 0x20) {
                y = ((o1.vx >> 5) * (va->vx - px) + (o1.vy >> 5) * va->vy + (o1.vz >> 5) * (va->vz - pz)) / (o1.vy >> 5);
            } else {
                y = (va->vy + vb->vy + vc->vy) / 3;
            }
            hitCount = 1;
            g_worldHitHeights[0] = y;
            found = hitCount;
            g_worldHitPolys[0] = cachedPoly;
            break;
        }
    }
    if (!found) {
        for (i = 0; i < count; i++, poly++) {
            va = &verts[poly->v0];
            vc = &verts[poly->v2];
            vb = &verts[poly->v1];
            v1.vx = vc->vx - va->vx;
            v1.vz = vc->vz - va->vz;
            w1.vx = px - va->vx;
            w1.vz = pz - va->vz;
            OuterProduct0(&v1, &w1, &o1);
            if (o1.vy > 0) {
                continue;
            }
            v2.vx = va->vx - vb->vx;
            v2.vz = va->vz - vb->vz;
            w2.vx = px - vb->vx;
            w2.vz = pz - vb->vz;
            OuterProduct0(&v2, &w2, &o2);
            if (o2.vy > 0) {
                continue;
            }
            v3.vx = vb->vx - vc->vx;
            v3.vz = vb->vz - vc->vz;
            w3.vx = px - vc->vx;
            w3.vz = pz - vc->vz;
            OuterProduct0(&v3, &w3, &o3);
            if (o3.vy > 0) {
                continue;
            }
            if (o1.vy == 0 && o3.vy == 0 && o2.vy == 0) {
                continue;
            }
            v4.vx = vb->vx - va->vx;
            v4.vy = vb->vy - va->vy;
            v4.vz = vb->vz - va->vz;
            v1.vy = vc->vy - va->vy;
            OuterProduct0(&v4, &v1, &o1);
            if (abs(o1.vy) >= 0x20) {
                y = ((o1.vx >> 5) * (va->vx - px) + (o1.vy >> 5) * va->vy + (o1.vz >> 5) * (va->vz - pz)) / (o1.vy >> 5);
            } else {
                y = (va->vy + vb->vy + vc->vy) / 3;
            }
            if ((o1.vy == 0 || o3.vy == 0 || o2.vy == 0) && (u8)poly->attr >> 2 == 0x3A) {
                nfall++;
                fidx = i;
                fy = y;
            } else {
                hitIdx = hitCount;
                hitCount++;
                g_worldHitHeights[hitIdx] = y;
                g_worldHitPolys[hitIdx] = i;
            }
        }
    }
    if (hitCount != 0) {
        y0 = pos->vy;
        best = abs(g_worldHitHeights[0] - y0);
        bi = 0;
        for (i = 1; i < hitCount; i++) {
            dist = abs(g_worldHitHeights[i] - y0);
            if (dist < best) {
                best = dist;
                bi = i;
            }
        }
        *idx = g_worldHitPolys[bi];
        *attr = polys[*idx].attr;
        y = g_worldHitHeights[bi];
        if (cache != NULL && !found) {
            (*slot)++;
            *slot %= 6;
            ccell[*slot] = cell;
            cpoly[*slot] = *idx;
        }
    } else if (nfall != 0) {
        *idx = fidx;
        *attr = polys[fidx].attr;
        y = fy;
    } else {
        *idx = -1;
        *attr = 0;
        y = pos->vy;
        printf("Not hit polygon\n");
        if (g_worldLoadedFrames >= 2) {
            sprintf(g_worldMapErrorMsg, "no polygon [ %d,%d,%d:%d ] \n", g_worldPlayer->pos.vx, g_worldPlayer->pos.vy,
                    g_worldPlayer->pos.vz, found);
            printf(s_wmStrMemorymanError, g_worldMapErrorMsg);
            if (g_worldMapError == 0) {
                g_worldMapError = 1;
            }
        }
    }
    return y;
}

void wm_initTexPages(void) {
    s32 i;
    s32 x;
    s32 y;
    s32 v;
    s32 u;

    i = 0;
    g_worldTexAreas[0].tpage = 0;
    g_worldTexAreas[0].clut = 0;
    g_worldTexAreas[0].u = 0;
    g_worldTexAreas[0].v = 0;
    g_worldTexAreas[0].frmTpage = 0;
    do {
        v = i / 8;
        x = (i % 8) * 16 + 0x300;
        y = i + 0xF0;
        u = (i % 4) << 6;
        g_worldTexAreas[i + 1].tpage = getTPage(0, 1, x, 0x100);
        g_worldTexAreas[i + 1].clut = getClut(0x300, y);
        g_worldTexAreas[i + 1].u = u;
        g_worldTexAreas[i + 1].v = v << 7;
        g_worldTexAreas[i + 1].frmTpage = 0;
        i++;
    } while (i < 16);
}

void wm_initMapState(void) {
    wm_initTexPages();
    g_worldUsePolyCache = 1;
    g_worldLoadedFrames = 0;
    g_worldGetoffSearching = 0;
}

void wm_termMapState(void) {
}

void wm_drawBlock(u8 *block, s32 parity, s32 far) {
    s32 ot;
    s32 unused[2];

    ot = g_worldOt[*g_worldBufIndex];
    g_worldCurTexArea = &g_worldTexAreas[block[1]];
    if (g_worldFog != 0) {
        wm_blockVertsFog(block);
    } else if (far == 0) {
        wm_blockVertsNear(block);
    } else {
        wm_blockVertsFar(block);
    }
    if (g_worldFog != 0) {
        g_worldPrimPtr = wm_blockFacesFog(block, g_worldPrimPtr, ot);
    } else if (far == 0) {
        g_worldPrimPtr = wm_blockFacesNear(block, g_worldPrimPtr, ot);
    } else {
        g_worldPrimPtr = wm_blockFacesFar(block, g_worldPrimPtr, ot);
    }
}

void wm_drawBackdrop(s32 model, s32 unused) {
    s32 ot = g_worldOt[*g_worldBufIndex];

    wm_backdropVerts(model);
    g_worldPrimPtr = wm_backdropFaces(model, g_worldPrimPtr, ot);
}

void wm_drawModel(s32 model, s32 otz, u32 mode) {
    s32 otEntry = g_worldOt[*g_worldBufIndex] + otz * 4;

    if (mode < 2) {
        wm_modelVerts(model);
    }
    if (mode == 2) {
        wm_modelVertsFlat(model);
    }
    if (mode == 0 || mode == 2) {
        g_worldPrimPtr = wm_modelFaces(model, g_worldPrimPtr, otEntry);
    }
    if (mode == 1) {
        g_worldPrimPtr = wm_modelFacesFog(model, g_worldPrimPtr, otEntry);
    }
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_memoryman", s_wmStrMemorymanError);
