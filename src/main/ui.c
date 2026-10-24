#include "ff9.h"

void ui_drawCharStatus(s32 x, s32 y, s32 otz, Character *c) {
    u8 buf[16];
    s32 col;

    if (c == NULL) {
        return;
    }
    text_formatNumber(c->level, buf, 2, 1);
    text_drawBlock(x, y, otz, c->unk0, 0);
    ui_drawSmallDigits(x + 0x24, y + 0xE, otz, buf, 0);
    ui_drawSprite(x, y + 0xE, otz, 0xB4, 0);
    ui_drawSprite(x, y + 0x17, otz, 0xAF, 0);
    ui_drawSprite(x, y + 0x20, otz, 0xB0, 0);
    if (c->hp != 0) {
        col = ((u16)(c->maxHp / 6) >= c->hp) << 2;
    } else {
        col = 3;
    }
    text_formatNumber(c->hp, buf, 4, 1);
    buf[4] = '/';
    text_formatNumber(c->maxHp, buf + 5, -1, 1);
    ui_drawSmallDigits(x + 0x16, y + 0x17, otz, buf, col);
    col = ((s16)((s16)c->maxMp / 6) >= (s16)c->mp) << 2;
    text_formatNumber((s16)c->mp, buf, 3, 1);
    buf[3] = '/';
    text_formatNumber((s16)c->maxMp, buf + 4, -1, 1);
    ui_drawSmallDigits(x + 0x1D, y + 0x20, otz, buf, col);
}

void ui_drawMemberHeader(s32 x, s32 y, s32 otz, Unk80024428 *s) {
    Character *a = g_gameState->unk1C->party[s->unk2];
    Character *b = g_gameState->unk1C->party[s->unk3];
    RECT r1;
    RECT r2;
    s32 d;
    s32 px;
    s32 t;

    if (s->unk0 != 0) {
        r1.w = 0x20;
        r1.x = x;
        r1.y = y;
        r1.h = 0x2E;
        win_setClip(NULL, otz, 4);
        d = 11 - s->unk4;
        if (s->unk1 == 1) {
            t = -(d * 3);
        } else {
            t = d * 3;
        }
        px = x + t;
        r2.x = px - 2;
        r2.y = y - 2;
        r2.w = 0x24;
        r2.h = 0x32;
        win_drawLineBorder(&r2, otz, 0, CONFIG_WINDOW_COLOR());
        ui_drawPartyPortrait(px, y, otz, s->unk3, b->hp == 0);
        win_setClip(&r1, otz, 4);
        ui_drawCharStatus(x + 0x26, y + 3, otz, g_gameState->unk1C->party[s->unk3]);
    } else {
        ui_drawCharStatus(x + 0x26, y + 3, otz, a);
    }
    ui_drawPartyPortrait(x, y, otz, s->unk2, a->hp == 0);
}

void win_drawShadowRect(RECT *r, s32 otz) {
    TileWords *p;
    DrTpageWords *t;

    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = GPU_CODE(GPU_TILE | GPU_ABE);
    p = (TileWords *)g_primPtr;
    p->xy = (r->x + 3) | ((r->y + r->h) << 16);
    p->wh = r->w | 0x30000;
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);

    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = GPU_CODE(GPU_TILE | GPU_ABE);
    p = (TileWords *)g_primPtr;
    p->xy = (r->x + r->w) | ((r->y + 3) << 16);
    p->wh = 3 | ((r->h - 3) << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);

    setlen(g_primPtr, 1);
    t = (DrTpageWords *)g_primPtr;
    t->code = get_mode(1, 1, 0);
    addPrim(&g_ot[otz], t);
    g_primPtr = (u8 *)(t + 1);
}

void win_drawShadowSlant(RECT *r, s32 otz) {
    PolyF4Half *p;
    DrTpageWords *t;

    p = (PolyF4Half *)g_primPtr;
    setlen(p, 5);
    p->rgbc = GPU_CODE(GPU_POLY_F4 | GPU_ABE);
    p->x0 = r->x + 4;
    p->y0 = r->y + r->h - 1;
    p->x1 = r->x + r->w + 2;
    p->y1 = r->y + r->h - 1;
    p->x2 = r->x + 7;
    p->y2 = r->y + r->h + 2;
    p->x3 = r->x + r->w - 3;
    p->y3 = r->y + r->h + 2;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], p);

    p = (PolyF4Half *)g_primPtr;
    setlen(p, 5);
    p->rgbc = GPU_CODE(GPU_POLY_F4 | GPU_ABE);
    p->x0 = r->x + r->w - 3;
    p->y0 = r->y + 2;
    p->x1 = r->x + r->w + 2;
    p->y1 = r->y + 7;
    p->x2 = r->x + r->w - 3;
    p->y2 = r->y + r->h - 1;
    p->x3 = r->x + r->w + 2;
    p->y3 = r->y + r->h - 1;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], p);

    setlen(g_primPtr, 1);
    t = (DrTpageWords *)g_primPtr;
    t->code = get_mode(1, 1, 0);
    addPrim(&g_ot[otz], t);
    g_primPtr = (u8 *)(t + 1);
}

void win_drawShadowSlant2(RECT *r, s32 otz) {
    PolyF4Half *p;
    DrTpageWords *t;

    p = (PolyF4Half *)g_primPtr;
    setlen(p, 5);
    p->rgbc = GPU_CODE(GPU_POLY_F4 | GPU_ABE);
    p->x0 = r->x + 2;
    p->y0 = r->y + r->h;
    p->x1 = r->x + r->w + 1;
    p->y1 = r->y + r->h;
    p->x2 = r->x + 4;
    p->y2 = r->y + r->h + 2;
    p->x3 = r->x + r->w - 1;
    p->y3 = r->y + r->h + 2;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], p);

    p = (PolyF4Half *)g_primPtr;
    setlen(p, 5);
    p->rgbc = GPU_CODE(GPU_POLY_F4 | GPU_ABE);
    p->x0 = r->x + r->w - 3;
    p->y0 = r->y;
    p->x1 = r->x + r->w + 2;
    p->y1 = r->y + 5;
    p->x2 = r->x + r->w - 3;
    p->y2 = r->y + r->h;
    p->x3 = r->x + r->w + 2;
    p->y3 = r->y + r->h;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], p);

    setlen(g_primPtr, 1);
    t = (DrTpageWords *)g_primPtr;
    t->code = get_mode(1, 1, 0);
    addPrim(&g_ot[otz], t);
    g_primPtr = (u8 *)(t + 1);
}

void ui_drawBackground(void) {
    s32 w;
    RECT tw;
    PolyG4Words *p;

    w = g_gameState->unkC->disp.w;
    setlen(g_primPtr, 8);
    p = (PolyG4Words *)g_primPtr;
    p->rgbc0 = GPU_CODE(GPU_POLY_G4 | GPU_ABE);
    p->rgb2 = 0x505050;
    p->rgb3 = 0x505050;
    p->xy1 = w;
    p->rgb1 = 0;
    p->xy0 = 0;
    p->xy2 = 0x800000;
    p->xy3 = w | 0x800000;
    addPrim(&g_ot[0xFFF], p);
    g_primPtr += sizeof(PolyG4Words);

    setlen(g_primPtr, 1);
    ((DrTpageWords *)g_primPtr)->code = get_mode(1, 1, 0x40);
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);

    tw.x = D_80068DC0.unk0;
    tw.y = D_80068DC0.unk1;
    tw.w = D_80068DC0.unk4;
    tw.h = D_80068DC0.unk5;

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&D_80068F40);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    ((SprtWords *)g_primPtr)->xy = 0;
    ((SprtWords *)g_primPtr)->uvclut = D_80068DC0.unk2 << 16;
    ((SprtWords *)g_primPtr)->wh = w | 0x800000;
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, D_80068DC0.unk6);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(&tw);
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
}

u8 *win_buildFill(RECT *r, u32 *ot, u8 u, u8 v, s32 pal, u8 *pkt) {
    s32 clut = win_getClut(D_80068CE8.unk2, pal);
    RECT tw;

    tw.x = D_80068CE8.unk0;
    tw.y = D_80068CE8.unk1;
    tw.w = D_80068CE8.unk4;
    tw.h = D_80068CE8.unk5;

    setlen(pkt, 2);
    ((DrMode2Words *)pkt)->code[0] = get_tw(&D_80068F40);
    ((DrMode2Words *)pkt)->code[1] = 0;
    addPrim(ot, pkt);
    pkt += sizeof(DrMode2Words);

    setlen(pkt, 4);
    ((SprtWords *)pkt)->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    ((SprtWords *)pkt)->xy = (u16)r->x | (r->y << 16);
    ((SprtWords *)pkt)->uvclut = u | (v << 8) | (clut << 16);
    ((SprtWords *)pkt)->wh = r->w | (r->h << 16);
    addPrim(ot, pkt);
    pkt += sizeof(SprtWords);

    setlen(pkt, 2);
    ((DrMode2Words *)pkt)->code[0] = get_mode(1, 1, D_80068CE8.unk6);
    ((DrMode2Words *)pkt)->code[1] = get_tw(&tw);
    addPrim(ot, pkt);
    return pkt + sizeof(DrMode2Words);
}

void win_drawLineBorder(RECT *r, s32 otz, u8 skip, s32 abr) {
    LineXY l[4];
    s32 i;
    u32 *col;
    LineF2W *p;

    l[0].x0 = r->x + 1;
    l[0].y0 = r->y + 1;
    l[0].x1 = r->x + r->w - 3;
    l[0].y1 = r->y + 1;
    l[1].x0 = r->x + 1;
    l[1].y0 = r->y + 2;
    l[1].x1 = r->x + 1;
    l[1].y1 = r->y + r->h - 3;
    l[2].x0 = r->x + 1;
    l[2].y0 = r->y + r->h - 2;
    l[2].x1 = r->x + r->w - 2;
    l[2].y1 = r->y + r->h - 2;
    l[3].x0 = r->x + r->w - 2;
    l[3].y0 = r->y;
    l[3].x1 = r->x + r->w - 2;
    l[3].y1 = r->y + r->h - 3;
    col = D_80068F20[abr];
    for (i = skip != 0; i < 4; i++) {
        p = (LineF2W *)g_primPtr;
        setlen(p, 3);
        p->rgbc = col[i >> 1];
        p->xy0 = (u16)l[i].x0 | (l[i].y0 << 16);
        p->xy1 = (u16)l[i].x1 | (l[i].y1 << 16);
        addPrim(&g_ot[otz], p);
        g_primPtr += sizeof(LineF2W);
        p = (LineF2W *)g_primPtr;
        setlen(p, 3);
        p->rgbc = col[(i >> 1) + 2];
        p->xy0 = ((l[i].x0 + D_80068F94[i][0]) & 0xFFFF) | ((l[i].y0 + D_80068F94[i][1]) << 16);
        p->xy1 = ((l[i].x1 + D_80068F94[i][2]) & 0xFFFF) | ((l[i].y1 + D_80068F94[i][3]) << 16);
        addPrim(&g_ot[otz], p);
        g_primPtr += sizeof(LineF2W);
    }
}

u8 *win_buildBorder(RECT *r, u32 *ot, u8 flag, u8 abe, s32 style, s32 pal, u8 *pkt) {
    s32 n = 1;
    s32 semi = (abe != 0) << 25;
    Unk800689B0 *d;
    s32 clut;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    RECT tw;

    if (flag) {
        n = 2;
    }
    d = NULL;
    if (style != 2) {
        clut = win_getClut(D_800686D0[D_80068FB4[style][0]].unk0 >> 16, pal) << 16;
    } else {
        clut = 0x3EA40000;
    }
    for (i = 0, y = r->y; i < 2; i++) {
        for (j = 0, x = r->x; j < 2; j++) {
            if (flag || i || !j) {
                d = (Unk800689B0 *)&D_800686D0[D_80068FB4[style][i * 2 + j]];
                setlen(pkt, 3);
                ((Sprt8Words *)pkt)->rgbc = D_80068FAC[flag] | semi;
                ((Sprt8Words *)pkt)->xy = (u16)x | (y << 16);
                ((Sprt8Words *)pkt)->uvclut = d->unk0 | (d->unk1 << 8) | clut;
                addPrim(ot, pkt);
                pkt += sizeof(Sprt8Words);
                x += r->w - 8;
            }
        }
        y += r->h - 8;
    }

    setlen(pkt, 2);
    ((DrMode2Words *)pkt)->code[0] = get_tw(&D_80068F40);
    ((DrMode2Words *)pkt)->code[1] = 0;
    addPrim(ot, pkt);
    pkt += sizeof(DrMode2Words);

    if (r->w - 16 > 0) {
        y = r->y + r->h - 8;
        for (i = 0; i < n; i++) {
            d = (Unk800689B0 *)&D_800686D0[D_80068FC0[style][i]];
            setlen(pkt, 4);
            ((SprtWords *)pkt)->rgbc = D_80068FA4[flag] | semi;
            ((SprtWords *)pkt)->xy = ((r->x + 8) & 0xFFFF) | (y << 16);
            ((SprtWords *)pkt)->uvclut = clut;
            ((SprtWords *)pkt)->wh = (r->w - 16) | 0x80000;
            addPrim(ot, pkt);
            y -= r->h - 8;
            tw.x = d->unk0;
            tw.y = d->unk1;
            tw.w = d->unk4;
            tw.h = d->unk5;
            pkt += sizeof(SprtWords);
            setlen(pkt, 2);
            ((DrMode2Words *)pkt)->code[0] = get_tw(&tw);
            ((DrMode2Words *)pkt)->code[1] = 0;
            addPrim(ot, pkt);
            pkt += sizeof(DrMode2Words);
        }
    }
    if (r->h - 16 > 0) {
        x = r->x;
        for (i = 0; i < n; i++) {
            d = (Unk800689B0 *)&D_800686D0[D_80068FC8[style][i]];
            setlen(pkt, 4);
            ((SprtWords *)pkt)->rgbc = D_80068FA4[flag] | semi;
            ((SprtWords *)pkt)->xy = (x & 0xFFFF) | ((r->y + 8) << 16);
            ((SprtWords *)pkt)->uvclut = clut;
            ((SprtWords *)pkt)->wh = ((r->h - 16) << 16) | 8;
            addPrim(ot, pkt);
            x += r->w - 8;
            tw.x = d->unk0;
            tw.y = d->unk1;
            tw.w = d->unk4;
            tw.h = d->unk5;
            pkt += sizeof(SprtWords);
            setlen(pkt, 2);
            ((DrMode2Words *)pkt)->code[0] = get_tw(&tw);
            ((DrMode2Words *)pkt)->code[1] = 0;
            addPrim(ot, pkt);
            pkt += sizeof(DrMode2Words);
        }
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, d->unk6);
    addPrim(ot, pkt);
    return pkt + sizeof(DrTpageWords);
}

void ui_drawRule(s32 x, s32 y, s32 otz, s32 style, s32 w, s32 abr) {
    Unk800689B0 *d;
    u16 clut;
    s32 i;
    RECT tw;
    Unk800689B0 *tbl;
    Unk800689B0 *tbl2;
    s32 yy;

    if (style == 1) {
        tbl = (Unk800689B0 *)D_800686D0;
        clut = tbl[D_80068FD0[1][0]].unk2;
    } else {
        tbl2 = (Unk800689B0 *)D_800686D0;
        clut = win_getClut(tbl2[D_80068FD0[style][0]].unk2, abr);
    }
    for (i = 0; i < 2; i++) {
        d = &((Unk800689B0 *)D_800686D0)[D_80068FD0[style][i]];
        setlen(g_primPtr, 4);
        yy = y << 16;
        ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
        if (i != 0) {
            ((SprtWords *)g_primPtr)->xy = ((x - (8 - w)) & 0xFFFF) | yy;
        } else {
            ((SprtWords *)g_primPtr)->xy = (x & 0xFFFF) | yy;
        }
        ((SprtWords *)g_primPtr)->uvclut = d->unk0 | (d->unk1 << 8) | (clut << 16);
        ((SprtWords *)g_primPtr)->wh = d->unk4 | (d->unk5 << 16);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(SprtWords);
    }
    if (w > 0x10) {
        setlen(g_primPtr, 2);
        ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&D_80068F40);
        ((DrMode2Words *)g_primPtr)->code[1] = 0;
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DrMode2Words);
        d = &((Unk800689B0 *)D_800686D0)[D_80068FD0[style][2]];
        tw.x = d->unk0;
        tw.y = d->unk1;
        tw.w = d->unk4;
        tw.h = d->unk5;
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
        ((SprtWords *)g_primPtr)->xy = ((x + 8) & 0xFFFF) | (y << 16);
        ((SprtWords *)g_primPtr)->uvclut = (style << 10) | (clut << 16);
        ((SprtWords *)g_primPtr)->wh = (w - 0x10) | (d->unk5 << 16);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(SprtWords);
    }
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, d->unk6);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(w > 0x10 ? &tw : &D_80068F40);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
}

void ui_drawGauge(s32 x, s32 y, s32 otz, s32 cur, s32 max, s32 style, u8 blink) {
    CVECTOR c1;
    CVECTOR c2;
    GaugeColors *e;
    CVECTOR *f1, *f2, *t1, *t2;
    s32 w;
    s32 k;
    s32 right;
    PolyG4Words *p;
    TileWords *t;

    w = cur * 36 / max;
    e = &D_80068FD8[style];
    if (cur != 0) {
        if (blink && cur >= max) {
            k = g_winLastVsync & 0x1F;
            if (k < 16) {
                f1 = &e->c[2];
                if (k >= 8) {
                    k = 15 - k;
                }
                f2 = &e->c[3];
                t1 = &e->c[0];
                t2 = &e->c[0];
            } else {
                k -= 16;
                f1 = &e->c[2];
                if (k >= 8) {
                    k = 15 - k;
                }
                f2 = &e->c[3];
                t1 = &e->c[1];
                t2 = t1;
            }
            CVECTOR_LERP(c1, f1, t1, k, 7);
            CVECTOR_LERP(c2, f2, t2, k, 7);
        } else {
            t1 = &e->c[3];
            c1 = e->c[2];
            CVECTOR_LERP(c2, &c1, t1, cur, max);
        }
        setlen(g_primPtr, 8);
        p = (PolyG4Words *)g_primPtr;
        p->rgbc0 = p->rgb2 = c1.r | (c1.g << 8) | (c1.b << 16) | GPU_CODE(GPU_POLY_G4);
        p->rgb1 = p->rgb3 = c2.r | (c2.g << 8) | (c2.b << 16);
        right = w + 2;
        p->xy0 = (x + 2) | ((y + 2) << 16);
        p->xy2 = (x + 2) | ((y + 4) << 16);
        p->xy1 = (x + right) | ((y + 2) << 16);
        p->xy3 = (x + right) | ((y + 4) << 16);
        addPrim(&g_ot[otz], p);
        g_primPtr = (u8 *)(p + 1);
    }
    ui_drawSprite(x, y, otz, 2, 0);
    setlen(g_primPtr, 3);
    t = (TileWords *)g_primPtr;
    t->rgbc = *(u32 *)&e->c[0];
    t->xy = (x + 1) | ((y + 1) << 16);
    t->wh = (w < 36 ? 0x30000 : 0x10000) | 0x26;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
}

void win_drawTailEdge(s16 x, s16 y, s32 otz, s32 idx, u8 flag, u8 mode) {
    Unk800689B0 *g = &g_winTailSprites[idx];
    u32 clut;
    SprtWords *p;
    DrTpageWords *t;

    if (!mode) {
        clut = (g->unk2 - CONFIG_WINDOW_COLOR() * 0xC0) << 16;
    } else {
        clut = 0x3EA40000;
    }
    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = flag ? GPU_RGBC(GPU_SPRT | GPU_ABE, 0x808080) : GPU_RGBC(GPU_SPRT, 0x808080);
    p = (SprtWords *)g_primPtr;
    p->xy = x | (y << 16);
    p->uvclut = g->unk0 | (g->unk1 << 8) | clut;
    p->wh = g->unk4 | (g->unk5 << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
    setlen(g_primPtr, 1);
    t = (DrTpageWords *)g_primPtr;
    t->code = get_mode(1, 1, g->unk6);
    addPrim(&g_ot[otz], t);
    g_primPtr = (u8 *)(t + 1);
}

void win_drawTailFill(s32 x, s32 y, s32 otz, s32 shape, s32 u, s32 v) {
    RECT tw;
    Unk80068F48 *o;
    u16 clut;

    clut = D_80068CE8.unk2 - CONFIG_WINDOW_COLOR() * 0xC0;
    tw.x = D_80068CE8.unk0;
    tw.y = D_80068CE8.unk1;
    tw.w = D_80068CE8.unk4;
    tw.h = D_80068CE8.unk5;

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&D_80068F40);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    setlen(g_primPtr, 9);
    o = &g_winTailShapes[shape];
    ((PolyFT4Words *)g_primPtr)->rgbc = GPU_RGBC(GPU_POLY_FT4, 0x808080);
    ((PolyFT4Words *)g_primPtr)->xy0 = (x + o->x0) | ((y + o->y0) << 16);
    ((PolyFT4Words *)g_primPtr)->uv0clut = (u + o->x0) | ((v + o->y0) << 8) | (clut << 16);
    ((PolyFT4Words *)g_primPtr)->xy1 = (x + o->x1) | ((y + o->y1) << 16);
    ((PolyFT4Words *)g_primPtr)->uv1tpage = (u + o->x1) | ((v + o->y1) << 8) | (D_80068CE8.unk6 << 16);
    ((PolyFT4Words *)g_primPtr)->xy2 = (x + o->x2) | ((y + o->y2) << 16);
    ((PolyFT4Words *)g_primPtr)->uv2 = (u + o->x2) | ((v + o->y2) << 8);
    ((PolyFT4Words *)g_primPtr)->xy3 = (x + o->x3) | ((y + o->y3) << 16);
    ((PolyFT4Words *)g_primPtr)->uv3 = (u + o->x3) | ((v + o->y3) << 8);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(PolyFT4Words);

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&tw);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
}

void win_drawHelpTailFill(s32 x, s32 y, s32 otz, s32 idx) {
    PolyF4Words *p;
    Unk80068F48 *t;
    setlen(g_primPtr, 5);
    p = (PolyF4Words *)g_primPtr;
    p->rgbc = GPU_RGBC(GPU_POLY_F4, 0xA8B078);
    t = &g_winTailShapes[idx];
    p->xy0 = (x + t->x0) | ((y + t->y0) << 16);
    p->xy1 = (x + t->x1) | ((y + t->y1) << 16);
    p->xy2 = (x + t->x2) | ((y + t->y2) << 16);
    p->xy3 = (x + t->x3) | ((y + t->y3) << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
}

void ui_drawEtchedLine(s32 x, s32 y, s32 otz, s32 len) {
    u32 col[3];
    s32 i;
    LineF2W *p;

    col[0] = D_80068F20[CONFIG_WINDOW_COLOR()][1];
    col[1] = GPU_RGBC(GPU_LINE_F2, 0x403838);
    col[2] = D_80068F20[CONFIG_WINDOW_COLOR()][0];

    for (i = 0; i < 3; i++) {
        p = (LineF2W *)g_primPtr;
        setlen(p, 3);
        p->rgbc = col[i];
        if (len < 0) {
            p->xy0 = (x + i) | (y << 16);
            p->xy1 = (x + i) | ((y - len) << 16);
        } else {
            p->xy0 = x | ((y + i) << 16);
            p->xy1 = (x + len - 1) | ((y + i) << 16);
        }
        addPrim(&g_ot[otz], p);
        g_primPtr += sizeof(LineF2W);
    }
}

void win_drawBorder(RECT *arg0, s32 otz, u8 arg2, u8 arg3, s32 arg4) {
    g_primPtr = win_buildBorder(arg0, &g_ot[otz], arg2, arg3, 0, arg4, g_primPtr);
}

void win_drawBalloonBorder(RECT *arg0, s32 otz, u8 arg2, u8 arg3, s32 arg4) {
    g_primPtr = win_buildBorder(arg0, &g_ot[otz], arg2, arg3, 1, arg4, g_primPtr);
}

void win_drawHelpBorder(RECT *arg0, s32 otz, u8 arg2, u8 arg3, s32 arg4) {
    g_primPtr = win_buildBorder(arg0, &g_ot[otz], arg2, arg3, 2, arg4, g_primPtr);
}

void ui_drawItemIcon(s32 arg0, s32 arg1, s32 otz, s32 arg3, u8 arg4) {
    g_primPtr = ui_buildItemIcon(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

u8 *ui_buildItemIcon(s32 x, s32 y, u32 *ot, s32 item, u8 flag, u8 *pkt) {
    Unk80069018 *e = &D_80069018[item];
    Unk800689B0 *d = &D_800689B0[e->unk8];
    s32 clut;

    clut = d->unk2 + (flag ? e->unk9 << 6 : 0x3C0);
    setlen(pkt, 4);
    ((SprtWords *)pkt)->rgbc = (flag == 0 ? GPU_RGBC(GPU_SPRT | GPU_ABE, 0x808080) : GPU_RGBC(GPU_SPRT, 0x808080));
    ((SprtWords *)pkt)->xy = (x & 0xFFFF) | (y << 16);
    ((SprtWords *)pkt)->uvclut = d->unk0 | (d->unk1 << 8) | (clut << 16);
    ((SprtWords *)pkt)->wh = d->unk4 | (d->unk5 << 16);
    addPrim(ot, pkt);
    pkt += sizeof(SprtWords);
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, d->unk6);
    addPrim(ot, pkt);
    return pkt + sizeof(DrTpageWords);
}

void func_80026DCC(s32 arg0, s32 arg1, s32 otz, s32 arg3, u8 arg4) {
    g_primPtr = ui_buildItemIcon(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void ui_drawMember(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    Character *p = g_gameState->unk1C->party[arg3];

    if (p != NULL) {
        ui_drawPartyPortrait(arg0, arg1, arg2, arg3, p->hp == 0);
        ui_drawMemberStats(arg0 + 0x26, arg1 + 3, arg2, arg3);
    }
}

void ui_drawMemberSummary(s32 x, s32 y, s32 otz, s32 slot, s32 mode) {
    Character *p = g_gameState->unk1C->party[slot];
    if (p != NULL && mode >= 0) {
        if (mode < 3) {
            ui_drawPortrait(x, y, otz, slot, 0x29, p->hp == 0);
            ui_drawMemberStats(x + 0x26, y, otz, slot);
        } else if (mode < 5) {
            ui_drawPortrait(x, y, otz, slot, 0x29, p->hp == 0);
            ui_drawCharStatuses(x + 0x26, y, otz, slot);
        }
    }
}

void ui_drawMemberStats(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    ui_drawCharStatus(arg0, arg1, arg2, g_gameState->unk1C->party[arg3]);
}

void ui_drawStatusIcons(s32 arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4) {
    s32 i;

    for (i = 0; i < 7; i++) {
        if ((arg3 >> i) & 1) {
            ui_drawSprite(arg0, arg1, arg2, D_80068F70[i], 0);
            arg0 += 12;
        }
    }
}

void win_drawShadow(RECT *arg0, s32 arg1, s32 arg2) {
    void (*fn)(RECT *, s32) = g_winShadowFuncs[arg2];

    if (fn != 0) {
        fn(arg0, arg1);
    }
}

void win_drawFill(RECT *arg0, s32 otz, u8 arg2, u8 arg3, s32 arg4) {
    g_primPtr = win_buildFill(arg0, &g_ot[otz], arg2, arg3, arg4, g_primPtr);
}

void ui_drawHand(s32 arg0, s32 arg1, s32 arg2) {
    ui_drawSprite(arg0, arg1, arg2, 1, 0);
}

void ui_drawApGauge(s32 arg0, s32 arg1, s32 otz, s32 arg3, s32 arg4, u8 arg5, u8 arg6) {
    g_primPtr = ui_buildApGauge(arg0, arg1, &g_ot[otz], arg3, arg4, arg5, arg6, g_primPtr);
}

u8 *ui_buildApGauge(s32 x, s32 y, u32 *ot, s32 cur, s32 max, u8 show, u8 flag, u8 *pkt) {
    u8 buf[16];
    s32 tmp[2];
    s32 w;
    s32 t;
    u32 rgbc;
    Unk800686E8 *d = &D_800686E8;

    if (max < cur) {
        cur = max;
    }
    if (cur != 0 && cur >= max) {
        pkt = ui_buildSprite(x + 4, y, ot, 0xC0, flag, tmp, pkt);
    } else if (show) {
        text_formatNumber(cur, buf, 3, 1);
        text_formatNumber(max, buf + 4, -1, 1);
        buf[3] = 0x2F;
        pkt = ui_buildDigits(x + 3, y + 2, ot, buf, flag ? GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060) : GPU_RGBC(GPU_SPRT, 0x808080), (SprtWords *)pkt);
    }
    if (cur != 0) {
        t = d->unk4 - 6;
        if (max != 0) {
            w = cur * t / max;
        } else {
            w = t;
        }
        setlen(pkt, 3);
        rgbc = GPU_RGBC(GPU_TILE, 0x70A838);
        if (cur >= max) {
            rgbc = GPU_RGBC(GPU_TILE, 0x2838A8);
        }
        if (flag) {
            rgbc |= GPU_CODE(GPU_ABE);
        }
        ((TileWords *)pkt)->rgbc = rgbc;
        ((TileWords *)pkt)->xy = ((x + 3) & 0xFFFF) | ((y + 1) << 16);
        ((TileWords *)pkt)->wh = w | 0x30000;
        addPrim(ot, pkt);
        pkt += sizeof(TileWords);
    }
    return ui_buildSpriteDef(x, y, ot, d, flag, tmp, (SprtWords *)pkt);
}

void ui_drawStones(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 buf[8];

    text_formatNumber(arg3, &buf[0], 2, 1);
    text_formatNumber(arg4, &buf[3], -1, 1);
    buf[2] = '/';
    ui_drawSprite(arg0, arg1, arg2, 0xC1, 0);
    ui_drawDigits(arg0 + 15, arg1 + 1, arg2, buf, arg3 == 0 ? 4 : 0);
}

u16 win_getPalClut(s32 arg0) {
    return arg0 - CONFIG_WINDOW_COLOR() * 0xC0;
}

u16 win_getClut(s32 arg0, s32 arg1) {
    return arg0 - arg1 * 0xC0;
}

void win_drawHelpFill(RECT *rect, s32 otz) {
    TileWords *p;

    setlen(g_primPtr, 3);
    p = (TileWords *)g_primPtr;
    p->rgbc = GPU_RGBC(GPU_TILE, 0xA8B078);
    p->xy = (u16)rect->x | (rect->y << 16);
    p->wh = rect->w | (rect->h << 16);
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
}

/* K&R definition: callers pass plain ints */
void win_drawHelpTailEdge(arg0, arg1, arg2, arg3, arg4)
    s16 arg0;
    s16 arg1;
    s32 arg2;
    s32 arg3;
    u8 arg4;
{
    win_drawTailEdge(arg0, arg1, arg2, arg3, arg4, 1);
}

/* K&R definition: callers pass plain ints */
void win_drawMsgTailEdge(arg0, arg1, arg2, arg3, arg4)
    s16 arg0;
    s16 arg1;
    s32 arg2;
    s32 arg3;
    u8 arg4;
{
    win_drawTailEdge(arg0, arg1, arg2, arg3, arg4, 0);
}

s32 ui_drawSprite(s32 x, s32 y, s32 otz, s32 id, u8 flag) {
    s32 w;
    Unk800686E8 *d = &D_800686D0[id];

    g_primPtr = ui_buildSpriteDef_inline(x, y, &g_ot[otz], d, flag, &w, (SprtWords *)g_primPtr);
    return w;
}

s32 ui_drawSpriteDef(s32 x, s32 y, s32 otz, void *d, u8 flag) {
    s32 w;

    g_primPtr = ui_buildSpriteDef_inline(x, y, &g_ot[otz], d, flag, &w, (SprtWords *)g_primPtr);
    return w;
}

u8 *ui_buildSprite(s32 x, s32 y, u32 *ot, s32 i, u8 flag, s32 *out, void *pkt) {
    SprtWords *s = pkt;
    DrTpageWords *t;
    Unk800686E8 *d;
    setlen(s, 4);
    d = &D_800686D0[i];
    if (flag) {
        s->rgbc = GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060);
    } else {
        s->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    }
    s->xy = (x & 0xFFFF) | (y << 16);
    s->uvclut = d->unk0;
    s->wh = d->unk4 | (d->unk5 << 16);
    addPrim(ot, s);
    s++;
    t = (DrTpageWords *)s;
    setlen(t, 1);
    t->code = get_mode(1, 1, d->unk6);
    addPrim(ot, t);
    *out = d->unk4;
    return (u8 *)(t + 1);
}

u8 *ui_buildSpriteDef(s32 x, s32 y, u32 *ot, Unk800686E8 *d, u8 flag, s32 *out, SprtWords *s) {
    DrTpageWords *t;
    setlen(s, 4);
    if (flag) {
        s->rgbc = GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060);
    } else {
        s->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    }
    s->xy = (x & 0xFFFF) | (y << 16);
    s->uvclut = d->unk0;
    s->wh = d->unk4 | (d->unk5 << 16);
    addPrim(ot, s);
    s++;
    t = (DrTpageWords *)s;
    setlen(t, 1);
    t->code = get_mode(1, 1, d->unk6);
    addPrim(ot, t);
    *out = d->unk4;
    return (u8 *)(t + 1);
}

void ui_drawCharStatuses(s32 x, s32 y, s32 otz, s32 slot) {
    Character *c = g_gameState->unk1C->party[slot];
    u8 buf[16];
    RECT r;

    if (c == NULL) {
        return;
    }
    text_formatNumber(c->level, buf, 2, 1);
    text_drawBlock(x, y, otz, c->unk0, 0);
    ui_drawSmallDigits(x + 0x26, y + 0xE, otz, buf, 0);
    ui_drawSprite(x, y + 0xE, otz, 0xB4, 0);
    ui_drawStatusIcons(x, y + 0x1C, otz, c->status, 0);
    r.x = x - 4;
    r.y = y + 0x18;
    r.w = 0x60;
    r.h = 0x12;
    win_drawBox(&r, otz, 0);
}

u8 *ui_buildDigits(s32 x, s32 y, u32 *ot, u8 *s, s32 unused, SprtWords *p) {
    s32 idx;
    s32 adv;
    Unk800686E8 *g;
    DrTpageWords *t;

    while (*s != 0) {
        if (*s != ' ') {
            if (*s == '/') {
                idx = 11;
                adv = 4;
            } else {
                adv = 5;
                if (*s != '%') {
                    idx = *s - '0';
                } else {
                    idx = 10;
                }
            }
            setlen(p, 4);
            p->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
            p->xy = (x & 0xFFFF) | (y << 16);
            g = &D_800687E0[idx];
            p->wh = g->unk4 | (g->unk5 << 16);
            p->uvclut = g->unk0;
            addPrim(ot, p);
            p++;
        } else {
            adv = 5;
        }
        s++;
        x += adv;
    }
    t = (DrTpageWords *)p;
    setlen(t, 1);
    t->code = get_mode(1, 1, 0x1F);
    addPrim(ot, t);
    return (u8 *)(t + 1);
}

void item_initNew(void) {
    u8 *p = D_8006BB74;

    item_clearAll();
    keyItem_clearAll();
    while (*p != 0xFF) {
        item_add(p[0], p[1]);
        p += 2;
    }
}

void item_clearAll(void) {
    s32 i;

    for (i = 0; i < 0x100; i++) {
        g_gameState->unk1C->items.slots[i].count = 0;
    }
}

void keyItem_clearAll(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        g_gameState->unk1C->unk774.unk0[i] = 0;
    }
}

s32 item_add(s32 id, s32 n) {
    ItemSlot *e;
    s32 i;
    if (id == 0xFF) {
        return 0;
    }
    e = g_gameState->unk1C->items.slots;
    for (i = 0; i < 0x100; e++, i++) {
        if (e->count != 0 && e->id == id) {
            if (e->count + n >= 100) {
                n = 99 - e->count;
            }
            e->count += n;
            return n;
        }
    }
    e = g_gameState->unk1C->items.slots;
    for (i = 0; i < 0x100; i++, e++) {
        if (e->count == 0) {
            e->id = id;
            e->count = n;
            return n;
        }
    }
    return 0;
}

s32 item_remove(s32 id, s32 count) {
    s32 i;
    ItemSlot *e = g_gameState->unk1C->items.slots;

    for (i = 0; i < 0x100; e++, i++) {
        if (e->count != 0 && e->id == id) {
            if (e->count < count) {
                count = e->count;
            }
            e->count -= count;
            return count;
        }
    }
    return 0;
}

ItemSlot *item_findSlot(s32 id) {
    s32 i;
    ItemSlot *e = g_gameState->unk1C->items.slots;

    for (i = 0; i < 0x100; i++, e++) {
        if (e->count != 0 && e->id == id) {
            return e;
        }
    }
    return NULL;
}

s32 item_getCount(s32 id) {
    ItemSlot *e = item_findSlot_inline(id);

    return e != NULL ? e->count : 0;
}

s32 item_countEquipped(s32 id) {
    s32 i, j;
    s32 n = 0;

    for (i = 0; i < 9; i++) {
        if (g_gameState->unk1C->characters[i].unk34.b.unkB) {
            for (j = 0; j < 5; j++) {
                if (id == g_gameState->unk1C->characters[i].equip[j]) {
                    n++;
                }
            }
        }
    }
    return n;
}

s32 item_getEquipSlot(s32 arg0) {
    s32 i;
    Unk80069018 *e = &D_80069018[arg0];

    for (i = 0; i < 5; i++) {
        if (e->unkF & D_8006BB84[i]) {
            return i;
        }
    }
    return -1;
}

void keyItem_add(s32 id) {
    s32 idx = id >> 2;

    g_gameState->unk1C->unk774.unk0[idx] |= 1 << ((id & 3) * 2);
    g_gameState->unk1C->unk774.unk0[idx] &= ~(1 << ((id & 3) * 2 + 1));
}

void keyItem_remove(s32 id) {
    s32 idx = id >> 2;

    g_gameState->unk1C->unk774.unk0[idx] &= ~(1 << ((id & 3) * 2));
    g_gameState->unk1C->unk774.unk0[idx] &= ~(1 << ((id & 3) * 2 + 1));
}

void keyItem_setSeen(s32 id) {
    g_gameState->unk1C->unk774.unk0[id >> 2] |= 1 << ((id & 3) * 2 + 1);
}

void keyItem_clearSeen(s32 id) {
    g_gameState->unk1C->unk774.unk0[id >> 2] &= ~(1 << ((id & 3) * 2 + 1));
}

s32 keyItem_has(s32 arg0) {
    return (g_gameState->unk1C->unk774.unk0[arg0 >> 2] >> ((arg0 & 3) * 2)) & 1;
}

s32 keyItem_isSeen(s32 id) {
    return (g_gameState->unk1C->unk774.unk0[id >> 2] >> ((id & 3) * 2 + 1)) & 1;
}

void ui_drawPortraitId(s32 x, s32 y, s32 otz, s32 id, s32 h, u8 flag) {
    u16 clut = GetClut(0, id + 0x1F3);
    s32 u;
    s32 v;
    s32 tpage;

    if (id >= 0 && id < g_portraitCount) {
        u = id << 5;
        if (id < 4) {
            v = 0xB0;
            tpage = 0x9F;
        } else {
            u = ((id - 4) & 3) << 5;
            v = ((id - 4) >> 2) * 46;
            tpage = 0x90;
        }
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = flag == 0 ? GPU_RGBC(GPU_SPRT, 0x808080) : GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060);
        ((SprtWords *)g_primPtr)->xy = x | (y << 16);
        ((SprtWords *)g_primPtr)->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (clut << 16);
        ((SprtWords *)g_primPtr)->wh = (h << 16) | 0x20;
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(SprtWords);
        setlen(g_primPtr, 2);
        SetDrawTPage(g_primPtr, 1, 1, tpage);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DrTpageWords);
    }
}

void ui_drawPortrait(s32 x, s32 y, s32 otz, s32 slot, s32 h, u8 flag) {
    s16 h0;
    s32 cx;
    s32 cy;
    u16 clut;
    s32 u, v;
    SprtWords *p;
    SprtWords *q;

    h0 = 0x20;
    if (h < 0x20) {
        h0 = h;
    }
    if (g_gameState->mode == 3) {
        cx = 0x280;
        cy = 0xDC;
    } else {
        cx = 0x100;
        cy = 0xFD;
    }
    clut = GetClut(cx + ((slot & 1) << 7), cy + (slot >> 1));
    u = slot << 5;
    v = 0xE0;
    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = flag == 0 ? GPU_RGBC(GPU_SPRT, 0x808080) : GPU_RGBC(GPU_SPRT | GPU_ABE, 0x808080);
    p = (SprtWords *)g_primPtr;
    g_primPtr += sizeof(SprtWords);
    p->xy = (x & 0xFFFF) | (y << 16);
    p->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (clut << 16);
    p->wh = (h0 << 16) | 0x20;
    addPrim(&g_ot[otz], p);
    SetDrawTPage(g_primPtr, 1, 1, 0x88);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    if (h > 0x20) {
        u = ((slot & 1) << 5) + 0x20;
        v = (slot >> 1) * 14 - 0x20;
        h -= h0;
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = flag == 0 ? GPU_RGBC(GPU_SPRT, 0x808080) : GPU_RGBC(GPU_SPRT | GPU_ABE, 0x808080);
        q = (SprtWords *)g_primPtr;
        g_primPtr += sizeof(SprtWords);
        q->xy = (x & 0xFFFF) | ((y + 0x20) << 16);
        q->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (clut << 16);
        q->wh = (h << 16) | 0x20;
        addPrim(&g_ot[otz], q);
        SetDrawTPage(g_primPtr, 1, 1, 0x89);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DrTpageWords);
    }
}

void ui_loadPortraits(void *data, s32 n) {
    RECT r0;
    RECT r1;
    s32 i;
    u32 *prev = data;
    u32 *q = prev;
    u32 *cur;

    g_portraits.unk4 = n;
    g_portraits.unk0 = prev;
    ui_getPortraitRects(0, &r0, &r1);
    StoreImage(&r0, g_portraitSavedVram);
    DrawSync(0);
    LoadImage(&r0, prev);
    DrawSync(0);
    LoadImage(&r1, prev + 0x40);
    DrawSync(0);
    cur = prev + 0x1B0;
    for (i = 1; i < g_portraits.unk4; i++) {
        ui_getPortraitRects(i, &r0, &r1);
        StoreImage(&r0, prev);
        DrawSync(0);
        if (i >= 4) {
            StoreImage(&r1, q + 0x40);
            DrawSync(0);
            q += 0x1B0;
        }
        LoadImage(&r0, cur);
        DrawSync(0);
        LoadImage(&r1, cur + 0x40);
        DrawSync(0);
        cur += 0x1B0;
        prev += 0x1B0;
    }
}

void ui_restorePortraits(void) {
    RECT clut;
    RECT img;
    u32 *p = g_portraits.unk0;
    u32 *q;
    s32 i;
    ui_getPortraitRects(0, &clut, &img);
    LoadImage(&clut, g_portraitSavedVram);
    DrawSync(0);
    q = p;
    for (i = 1; i < g_portraits.unk4; i++) {
        ui_getPortraitRects(i, &clut, &img);
        LoadImage(&clut, p);
        DrawSync(0);
        if (i >= 4) {
            LoadImage(&img, q + 0x40);
            DrawSync(0);
            q += 0x1B0;
        }
        p += 0x1B0;
    }
}

u8 chr_getPortrait(s32 arg0) {
    return D_8006BB8C[g_gameState->unk1C->characters[arg0].unk34.b.unk4];
}

void ui_getPortraitRects(s32 arg0, RECT *clut, RECT *img) {
    clut->y = arg0 + 0x1F3;
    clut->w = 0x80;
    clut->h = 1;
    clut->x = 0;
    if (arg0 < 4) {
        img->x = arg0 * 16 + 0x3C0;
        img->y = 0x1B0;
    } else {
        s32 n = arg0 - 4;

        img->x = (n & 3) * 16;
        img->y = (n >> 2) * 0x2E + 0x100;
    }
    img->w = 0x10;
    img->h = 0x2E;
}

void ui_drawPortraitIdFull(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4) {
    ui_drawPortraitId(arg0, arg1, arg2, arg3, 0x2E, arg4);
}

void ui_getPartyPortraitRects(s32 arg0, RECT *a, RECT *b, RECT *c) {
    s32 x, y;

    if (g_gameState->mode == 3) {
        x = 0x280;
        y = 0xDC;
    } else {
        x = 0x100;
        y = 0xFD;
    }
    a->x = x + ((arg0 & 1) << 7);
    a->y = y + (arg0 >> 1);
    a->w = 0x80;
    a->h = 1;
    b->x = arg0 * 16 + 0x200;
    b->y = 0xE0;
    b->w = 0x10;
    b->h = 0x20;
    c->x = (arg0 & 1) * 16 + 0x250;
    c->y = (arg0 >> 1) * 14 + 0xE0;
    c->w = 0x10;
    c->h = 0xE;
}

void ui_drawPartyPortrait(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4) {
    ui_drawPortrait(arg0, arg1, arg2, arg3, 0x2E, arg4);
}

void ui_loadPartyPortraits(u8 *buf) {
    s32 i;

    for (i = 0; i < 4; i++) {
        GameData *d = g_gameState->unk1C;
        Character *p = d->party[i];

        if (p != NULL) {
            ui_loadPartyPortrait(buf + D_8006BB8C[d->characters[p->unk34.b.id].unk34.b.unk4] * 0x6C0, i);
        }
    }
}

void ui_loadPartyPortrait(u8 *buf, s32 arg1) {
    RECT a, b, c;

    ui_getPartyPortraitRects(arg1, &a, &b, &c);
    LoadImage(&a, (u32 *)buf);
    DrawSync(0);
    LoadImage(&b, (u32 *)(buf + 0x100));
    DrawSync(0);
    LoadImage(&c, (u32 *)(buf + 0x500));
    DrawSync(0);
}

void ui_savePartyPortrait(u8 *buf, s32 arg1) {
    RECT a, b, c;

    ui_getPartyPortraitRects(arg1, &a, &b, &c);
    StoreImage(&a, (u32 *)buf);
    DrawSync(0);
    StoreImage(&b, (u32 *)(buf + 0x100));
    DrawSync(0);
    StoreImage(&c, (u32 *)(buf + 0x500));
    DrawSync(0);
}

void ui_initMemberSwitch(UnkBlink *b, s32 arg1, void (*cb)(void)) {
    b->unk0 = 0;
    b->unk2 = arg1;
    b->unk8 = cb;
}

void ui_startMemberSwitch(UnkBlink *b, s32 arg1, s32 arg2, s32 arg3) {
    b->unk1 = arg3;
    b->unk4 = 0;
    if (arg3 != 0) {
        b->unk2 = arg1;
        b->unk3 = arg2;
        b->unk0 = 1;
    }
}

void ui_updateMemberSwitch(UnkBlink *b) {
    void (*cb)(void);

    if (b->unk0 != 0) {
        if (++b->unk4 >= 12) {
            b->unk2 = b->unk3;
            cb = b->unk8;
            b->unk4 = 0;
            b->unk0 = 0;
            if (cb != NULL) {
                cb();
            }
        }
    }
}
