#include "ovl_0c.h"

s32 w_memorymanHeapAlloc(s32 arg0) {
    s32 *p = &g_worldHeapStack[g_worldHeapDepth++];

    if (g_worldHeapDepth >= 0x19U) {
        printf("w_memorymanHeapAlloc:(Error) Heap stack overflow\n");
    } else if (g_worldMapDebugFlags & WM_DEBUG_MEMORY) {
        printf("w_memorymanHeapAlloc: [%d]  =  %x - ", g_worldHeapDepth - 1, g_worldHeapTop);
    }
    *p = g_worldHeapTop;
    g_worldHeapLastSize = arg0;
    g_worldHeapTop += arg0;
    g_worldHeapUsed += arg0;
    if (g_worldMapDebugFlags & WM_DEBUG_MEMORY) {
        printf("%x (%x/%x)[%x]\n", g_worldHeapTop, g_worldHeapUsed, 0x24800, arg0);
    }
    return *p;
}

/* pop the heap stack */
s32 w_memorymanHeapFree(void) {
    s32 *stack;
    s32 n;
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
    n = g_worldHeapDepth - 1;
    prev = stack[n];
    size = g_worldHeapTop;
    size -= prev;
    g_worldHeapDepth = n;
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
    g_worldHeapTop = D_80067948;
    D_80067948 += 0x24800;
}

void w_memorymanDestructor(void) {
    if (g_worldHeapDepth != 0) {
        printf("w_memorymanDestructor:(Error) Memory leak can be found.\n");
    }
    if (g_worldMapDebugFlags & WM_DEBUG_MEMORY) {
        printf(D_800A7CCC);
    }
}

void wm_initClutAnims(void) {
    RECT r;
    u16 off;
    s32 src;
    s32 i;
    s32 j;
    WmClutAnim *e;
    s32 a;
    s32 b;
    u16 *dst;

    off = 0;
    src = 0;
    memset(&r, 0, 8);
    r.h = 1;
    g_worldClutAnimFx2 = 0;
    g_worldTexAnimStep[0] = 0;
    g_worldTexAnimStep[1] = 0;
    for (i = 7; i >= 0; i--) {
        g_worldTexScrollAcc[i] = 0;
    }
    for (i = 0; i < 18; i++) {
        e = &g_worldClutAnims[i];
        r.x = e->x + e->start;
        r.y = e->y;
        r.w = e->count;
        r.h = 1;
        if (r.w == 0) {
            r.w = 1;
        }
        if (i < 18) {
            if (i >= 15) {
                dst = &g_worldClutBuf[0][off];
                memcpy((u8 *)dst, (u8 *)w_framePackGetPtr(g_worldDataPack, 2) + src, (u16)e->count * 2);
                src += (u16)e->count * 2;
            } else {
                StoreImage(&r, (u32 *)&g_worldClutBuf[0][off]);
                DrawSync(0);
            }
        } else {
            StoreImage(&r, (u32 *)&g_worldClutBuf[0][off]);
            DrawSync(0);
        }
        for (j = 0; j < r.w; j++) {
            a = (j + r.w) % r.w;
            b = (j + r.w - 1) % r.w;
            g_worldClutBuf[0][off + a + r.w] = g_worldClutBuf[0][off + j];
            g_worldClutBuf[1][off + b + r.w] = g_worldClutBuf[0][off + j];
            g_worldClutBuf[1][off + b] = g_worldClutBuf[0][off + j];
        }
        g_worldClutAnimOffset[i] = off;
        g_worldClutAnimFrame[i] = 0;
        g_worldClutAnimAcc[i] = 0;
        off += (u16)e->count * 2;
    }
}

/* upload TIM arg1 of table arg0, offset by (arg2, arg3) (image) and (arg4, arg5) (clut) */
void wm_loadTim(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s8 arg6) {
    u32 *tim;
    RECT *clut;
    u32 *cdata;
    u32 *img;
    u8 has;
    RECT r;
    RECT r2;
    RECT *q;
    RECT *q2;
    RECT *q3;
    s32 n;
    RECT *rect;
    u32 *data;

    tim = (u32 *)arg0[arg1 + 1];
    if ((arg2 | arg3 | arg4 | arg5) == 0) {
        if (arg6 != 0) {
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
        n = (tim[2] >> 2) - 3;
        img = cdata + n;
    }
    rect = (RECT *)++img;
    data = (u32 *)(rect + 1);
    if (g_worldStartup != 0) {
        r = *rect;
        r.x += arg2;
        r.y += arg3;
        LoadImage(&r, data);
    } else {
        q = (RECT *)g_worldPrimPtr;
        q->x = rect->x + arg2;
        q->y = rect->y + arg3;
        q->w = rect->w;
        q->h = rect->h;
        LoadImage(q, data);
        q++;
        g_worldPrimPtr = (s32)q;
    }
    if (arg6 != 2) {
        if (has) {
            if (g_worldStartup != 0) {
                r2 = *clut;
                r2.x += arg4;
                r2.y += arg5;
                if (r2.w == 0x100) {
                    r2.w = 0xF0;
                }
                LoadImage(&r2, cdata);
            } else {
                q2 = (RECT *)g_worldPrimPtr;
                q2->x = clut->x + arg4;
                q2->y = clut->y + arg5;
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
    s32 t;
    s32 n;
    RECT *r;

    for (i = 0; i < 8; i++) {
        x = g_worldTexScrolls[i].x;
        y = g_worldTexScrolls[i].y;
        w = g_worldTexScrolls[i].w;
        h = g_worldTexScrolls[i].h;
        t = g_worldTexScrollAcc[i] + g_worldTexScrolls[i].speed;
        n = (s16)t >> 8;
        g_worldTexScrollAcc[i] = t & 0xFF;
        if (n != 0) {
            r = (RECT *)g_worldPrimPtr;
            r->x = x;
            r->y = y;
            r->w = w;
            r->h = n;
            MoveImage(r, 0x280, 0xF8);
            r++;
            r->y = y + n;
            r->x = x;
            r->w = w;
            r->h = h - n;
            MoveImage(r, x, y);
            r++;
            r->x = 0x280;
            r->y = 0xF8;
            r->w = w;
            r->h = n;
            MoveImage(r, x, y + h - n);
            r++;
            g_worldPrimPtr = (s32)r;
        }
    }
}

void wm_animTextures(void) {
    s32 t;

    t = D_800CFEF8;
    if (t != 0 && g_worldFrameCount % t == 0) {
        wm_loadTim(g_worldTexAnimPack, g_worldTexAnimSeq[g_worldTexAnimStep[0]] + 8, 0, 0, 0, 0, 0);
        wm_loadTim(g_worldTexAnimPack, g_worldTexAnimSeq[g_worldTexAnimStep[0]], 0, 0, 0, 0, 0);
        g_worldTexAnimStep[0] = (s8)(g_worldTexAnimStep[0] + 1) % 6;
    }
    t = D_800CFF02;
    if (t != 0 && (g_worldFrameCount + 2) % t == 0) {
        wm_loadTim(g_worldTexAnimPack, g_worldTexAnimSeq[g_worldTexAnimStep[1]] + 4, 0, 0, 0, 0, 0);
        wm_loadTim(g_worldTexAnimPack, g_worldTexAnimSeq[g_worldTexAnimStep[1]] + 12, 0, 0, 0, 0, 0);
        g_worldTexAnimStep[1] = (s8)(g_worldTexAnimStep[1] + 1) % 6;
    }
}

void wm_animCluts(void) {
    s8 a = 0;
    s8 b = -1;
    s8 c = 0;
    s8 d = -1;
    u8 on = 0;
    s32 e = 0;
    WmFrmbufSlot *n;

    if (g_worldClutAnimFx2 != 0) {
        if (g_worldFormFlags[3] == 0) {
            a = 0xC;
            b = 0xD;
            c = 0xF;
            d = 0x10;
        } else {
            a = 0xE;
            b = 0xE;
            c = 0x11;
            d = 0x11;
        }
    }
    n = g_worldFrmbufHead.next;
    if ((n->texId & 0x3F) == 4) {
        on = 1;
        if (n->vramArea == 2) {
            e = 8;
        }
    }
    n = n->next;
    if ((n->texId & 0x3F) == 4) {
        on = 1;
        if (n->vramArea == 2) {
            e = 8;
        }
    }
    wm_stepClutAnims(0, 0xB, 0);
    wm_stepClutAnims(a, b, 0);
    if (on) {
        wm_stepClutAnims(c, d, e);
    }
}

/* advance the palette animations arg0..arg1 and upload the new frames (y offset arg2) */
void wm_stepClutAnims(s32 arg0, s32 arg1, s32 arg2) {
    RECT *r;
    s32 i;
    s32 n;

    for (i = arg0; i <= arg1; i++) {
        g_worldClutAnimAcc[i] += g_worldClutAnims[i].speed;
        n = g_worldClutAnimAcc[i] / 256;
        g_worldClutAnimAcc[i] &= 0xFF;
        if (n != 0) {
            r = (RECT *)g_worldPrimPtr;
            r->x = g_worldClutAnims[i].x + g_worldClutAnims[i].start;
            r->y = g_worldClutAnims[i].y + arg2;
            r->w = g_worldClutAnims[i].count;
            r->h = 1;
            g_worldClutAnimFrame[i] += n;
            g_worldClutAnimFrame[i] %= r->w;
            LoadImage(r++, (u32 *)&g_worldClutBuf[g_worldClutAnimFrame[i] & 1][g_worldClutAnimOffset[i] + (g_worldClutAnimFrame[i] & 0xFFFE)]);
            g_worldPrimPtr = (s32)r;
        }
    }
}

void wm_updateVramAnims(void) {
    wm_scrollTextures();
    wm_animTextures();
    wm_animCluts();
}

void wm_loadTimPack(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 n = *arg0 & 0x7FFFFFFF;
    s32 i;

    for (i = 0; i < n; i++) {
        wm_loadTim(arg0, i, arg1, arg2, arg3, arg4, 0);
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

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_memoryman", D_800A7CCC);

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
    s32 p;
    u16 count;
    u16 *cpoly;
    u16 *ccell;
    u8 *slot;
    s32 nfall;
    s32 fidx;
    s32 fy;
    SVECTOR *verts;
    SVECTOR *a;
    SVECTOR *b;
    SVECTOR *c;
    s32 px;
    s32 pz;
    u16 n;
    s32 i;
    s32 k;
    s32 y;
    s32 y0;
    s16 d;
    s16 best;
    s16 bi;
    WmPoly *pp;
    s32 m;

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
    n = 0;
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
            p = cpoly[k];
            if (p >= count) {
                continue;
            }
            pp = (WmPoly *)((p << 4) + (s32)polys);
            a = &verts[pp->v0];
            c = &verts[pp->v2];
            b = &verts[pp->v1];
            v1.vx = c->vx - a->vx;
            v1.vz = c->vz - a->vz;
            w1.vx = px - a->vx;
            w1.vz = pz - a->vz;
            OuterProduct0(&v1, &w1, &o1);
            if (o1.vy > 0) {
                continue;
            }
            v2.vx = a->vx - b->vx;
            v2.vz = a->vz - b->vz;
            w2.vx = px - b->vx;
            w2.vz = pz - b->vz;
            OuterProduct0(&v2, &w2, &o2);
            if (o2.vy > 0) {
                continue;
            }
            v3.vx = b->vx - c->vx;
            v3.vz = b->vz - c->vz;
            w3.vx = px - c->vx;
            w3.vz = pz - c->vz;
            OuterProduct0(&v3, &w3, &o3);
            if (o3.vy > 0) {
                continue;
            }
            if (o1.vy == 0 || o3.vy == 0 || o2.vy == 0) {
                continue;
            }
            v4.vx = b->vx - a->vx;
            v4.vy = b->vy - a->vy;
            v4.vz = b->vz - a->vz;
            v1.vy = c->vy - a->vy;
            OuterProduct0(&v4, &v1, &o1);
            if (abs(o1.vy) >= 0x20) {
                y = ((o1.vx >> 5) * (a->vx - px) + (o1.vy >> 5) * a->vy + (o1.vz >> 5) * (a->vz - pz)) / (o1.vy >> 5);
            } else {
                y = (a->vy + b->vy + c->vy) / 3;
            }
            n = 1;
            g_worldHitHeights[0] = y;
            found = n;
            g_worldHitPolys[0] = p;
            break;
        }
    }
    if (!found) {
        for (i = 0; i < count; i++, poly++) {
            a = &verts[poly->v0];
            c = &verts[poly->v2];
            b = &verts[poly->v1];
            v1.vx = c->vx - a->vx;
            v1.vz = c->vz - a->vz;
            w1.vx = px - a->vx;
            w1.vz = pz - a->vz;
            OuterProduct0(&v1, &w1, &o1);
            if (o1.vy > 0) {
                continue;
            }
            v2.vx = a->vx - b->vx;
            v2.vz = a->vz - b->vz;
            w2.vx = px - b->vx;
            w2.vz = pz - b->vz;
            OuterProduct0(&v2, &w2, &o2);
            if (o2.vy > 0) {
                continue;
            }
            v3.vx = b->vx - c->vx;
            v3.vz = b->vz - c->vz;
            w3.vx = px - c->vx;
            w3.vz = pz - c->vz;
            OuterProduct0(&v3, &w3, &o3);
            if (o3.vy > 0) {
                continue;
            }
            if (o1.vy == 0 && o3.vy == 0 && o2.vy == 0) {
                continue;
            }
            v4.vx = b->vx - a->vx;
            v4.vy = b->vy - a->vy;
            v4.vz = b->vz - a->vz;
            v1.vy = c->vy - a->vy;
            OuterProduct0(&v4, &v1, &o1);
            if (abs(o1.vy) >= 0x20) {
                y = ((o1.vx >> 5) * (a->vx - px) + (o1.vy >> 5) * a->vy + (o1.vz >> 5) * (a->vz - pz)) / (o1.vy >> 5);
            } else {
                y = (a->vy + b->vy + c->vy) / 3;
            }
            if ((o1.vy == 0 || o3.vy == 0 || o2.vy == 0) && (u8)poly->attr >> 2 == 0x3A) {
                nfall++;
                fidx = i;
                fy = y;
            } else {
                m = n;
                n++;
                g_worldHitHeights[m] = y;
                g_worldHitPolys[m] = i;
            }
        }
    }
    if (n != 0) {
        y0 = pos->vy;
        best = abs(g_worldHitHeights[0] - y0);
        bi = 0;
        for (i = 1; i < n; i++) {
            d = abs(g_worldHitHeights[i] - y0);
            if (d < best) {
                best = d;
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
            printf(D_800A7D20, g_worldMapErrorMsg);
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
    D_800D30C4 = 0;
}

void func_800BA2D4(void) {
}

void wm_drawBlock(u8 *arg0, s32 arg1, s32 arg2) {
    s32 t;
    s32 unused[2];

    t = g_worldOt[*g_worldBufIndex];
    g_worldCurTexArea = &g_worldTexAreas[arg0[1]];
    if (g_worldFog != 0) {
        wm_blockVertsFog(arg0);
    } else if (arg2 == 0) {
        wm_blockVertsNear(arg0);
    } else {
        wm_blockVertsFar(arg0);
    }
    if (g_worldFog != 0) {
        g_worldPrimPtr = wm_blockFacesFog(arg0, g_worldPrimPtr, t);
    } else if (arg2 == 0) {
        g_worldPrimPtr = wm_blockFacesNear(arg0, g_worldPrimPtr, t);
    } else {
        g_worldPrimPtr = wm_blockFacesFar(arg0, g_worldPrimPtr, t);
    }
}

void wm_drawBackdrop(s32 arg0, s32 arg1) {
    s32 ot = g_worldOt[*g_worldBufIndex];

    wm_backdropVerts(arg0);
    g_worldPrimPtr = wm_backdropFaces(arg0, g_worldPrimPtr, ot);
}

void wm_drawModel(s32 arg0, s32 arg1, u32 arg2) {
    s32 p = g_worldOt[*g_worldBufIndex] + arg1 * 4;

    if (arg2 < 2) {
        wm_modelVerts(arg0);
    }
    if (arg2 == 2) {
        wm_modelVertsFlat(arg0);
    }
    if (arg2 == 0 || arg2 == 2) {
        g_worldPrimPtr = wm_modelFaces(arg0, g_worldPrimPtr, p);
    }
    if (arg2 == 1) {
        g_worldPrimPtr = wm_modelFacesFog(arg0, g_worldPrimPtr, p);
    }
}

INCLUDE_RODATA("asm/us/ovl_0c/nonmatchings/w_memoryman", D_800A7D20);
