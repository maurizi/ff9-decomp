#include "ff9.h"

void ui_drawCharStatus(s32 x, s32 y, s32 otz, Character *chr) {
    u8 numText[16];
    s32 col;

    if (chr == NULL) {
        return;
    }
    text_formatNumber(chr->level, numText, 2, TEXT_NUM_ASCII);
    text_drawBlock(x, y, otz, chr->name, TEXT_PAL_WHITE);
    ui_drawSmallDigits(x + 0x24, y + 0xE, otz, numText, TEXT_PAL_WHITE);
    ui_drawSprite(x, y + 0xE, otz, 0xB4, 0);
    ui_drawSprite(x, y + 0x17, otz, 0xAF, 0);
    ui_drawSprite(x, y + 0x20, otz, 0xB0, 0);
    if (chr->hp != 0) {
        col = ((u16)(chr->maxHp / 6) >= chr->hp) * TEXT_PAL_YELLOW;
    } else {
        col = TEXT_PAL_RED;
    }
    text_formatNumber(chr->hp, numText, 4, TEXT_NUM_ASCII);
    numText[4] = '/';
    text_formatNumber(chr->maxHp, numText + 5, TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
    ui_drawSmallDigits(x + 0x16, y + 0x17, otz, numText, col);
    col = ((s16)((s16)chr->maxMp / 6) >= (s16)chr->mp) * TEXT_PAL_YELLOW;
    text_formatNumber((s16)chr->mp, numText, 3, TEXT_NUM_ASCII);
    numText[3] = '/';
    text_formatNumber((s16)chr->maxMp, numText + 4, TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
    ui_drawSmallDigits(x + 0x1D, y + 0x20, otz, numText, col);
}

void ui_drawMemberHeader(s32 x, s32 y, s32 otz, MemberSwitch *memberSw) {
    Character *curChr = g_gameState->gameData->party[memberSw->member];
    Character *nextChr = g_gameState->gameData->party[memberSw->nextMember];
    RECT r1;
    RECT r2;
    s32 framesLeft;
    s32 px;
    s32 slideX;

    if (memberSw->active != 0) {
        r1.w = 0x20;
        r1.x = x;
        r1.y = y;
        r1.h = 0x2E;
        win_setClip(NULL, otz, WIN_STYLE_FRAMELESS);
        framesLeft = 11 - memberSw->frame;
        if (memberSw->dir == 1) {
            slideX = -(framesLeft * 3);
        } else {
            slideX = framesLeft * 3;
        }
        px = x + slideX;
        r2.x = px - 2;
        r2.y = y - 2;
        r2.w = 0x24;
        r2.h = 0x32;
        win_drawLineBorder(&r2, otz, 0, CONFIG_WINDOW_COLOR());
        ui_drawPartyPortrait(px, y, otz, memberSw->nextMember, nextChr->hp == 0);
        win_setClip(&r1, otz, WIN_STYLE_FRAMELESS);
        ui_drawCharStatus(x + 0x26, y + 3, otz, g_gameState->gameData->party[memberSw->nextMember]);
    } else {
        ui_drawCharStatus(x + 0x26, y + 3, otz, curChr);
    }
    ui_drawPartyPortrait(x, y, otz, memberSw->member, curChr->hp == 0);
}

void win_drawShadowRect(RECT *rect, s32 otz) {
    TileWords *tile;
    DrTpageWords *drMode;

    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = GPU_CODE(GPU_TILE | GPU_ABE);
    tile = (TileWords *)g_primPtr;
    tile->xy = (rect->x + 3) | ((rect->y + rect->h) << 16);
    tile->wh = rect->w | 0x30000;
    addPrim(&g_ot[otz], tile);
    g_primPtr = (u8 *)(tile + 1);

    setlen(g_primPtr, 3);
    ((TileWords *)g_primPtr)->rgbc = GPU_CODE(GPU_TILE | GPU_ABE);
    tile = (TileWords *)g_primPtr;
    tile->xy = (rect->x + rect->w) | ((rect->y + 3) << 16);
    tile->wh = 3 | ((rect->h - 3) << 16);
    addPrim(&g_ot[otz], tile);
    g_primPtr = (u8 *)(tile + 1);

    setlen(g_primPtr, 1);
    drMode = (DrTpageWords *)g_primPtr;
    drMode->code = get_mode(1, 1, 0);
    addPrim(&g_ot[otz], drMode);
    g_primPtr = (u8 *)(drMode + 1);
}

void win_drawShadowSlant(RECT *rect, s32 otz) {
    PolyF4Half *poly;
    DrTpageWords *drMode;

    poly = (PolyF4Half *)g_primPtr;
    setlen(poly, 5);
    poly->rgbc = GPU_CODE(GPU_POLY_F4 | GPU_ABE);
    poly->x0 = rect->x + 4;
    poly->y0 = rect->y + rect->h - 1;
    poly->x1 = rect->x + rect->w + 2;
    poly->y1 = rect->y + rect->h - 1;
    poly->x2 = rect->x + 7;
    poly->y2 = rect->y + rect->h + 2;
    poly->x3 = rect->x + rect->w - 3;
    poly->y3 = rect->y + rect->h + 2;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], poly);

    poly = (PolyF4Half *)g_primPtr;
    setlen(poly, 5);
    poly->rgbc = GPU_CODE(GPU_POLY_F4 | GPU_ABE);
    poly->x0 = rect->x + rect->w - 3;
    poly->y0 = rect->y + 2;
    poly->x1 = rect->x + rect->w + 2;
    poly->y1 = rect->y + 7;
    poly->x2 = rect->x + rect->w - 3;
    poly->y2 = rect->y + rect->h - 1;
    poly->x3 = rect->x + rect->w + 2;
    poly->y3 = rect->y + rect->h - 1;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], poly);

    setlen(g_primPtr, 1);
    drMode = (DrTpageWords *)g_primPtr;
    drMode->code = get_mode(1, 1, 0);
    addPrim(&g_ot[otz], drMode);
    g_primPtr = (u8 *)(drMode + 1);
}

void win_drawShadowSlant2(RECT *rect, s32 otz) {
    PolyF4Half *poly;
    DrTpageWords *drMode;

    poly = (PolyF4Half *)g_primPtr;
    setlen(poly, 5);
    poly->rgbc = GPU_CODE(GPU_POLY_F4 | GPU_ABE);
    poly->x0 = rect->x + 2;
    poly->y0 = rect->y + rect->h;
    poly->x1 = rect->x + rect->w + 1;
    poly->y1 = rect->y + rect->h;
    poly->x2 = rect->x + 4;
    poly->y2 = rect->y + rect->h + 2;
    poly->x3 = rect->x + rect->w - 1;
    poly->y3 = rect->y + rect->h + 2;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], poly);

    poly = (PolyF4Half *)g_primPtr;
    setlen(poly, 5);
    poly->rgbc = GPU_CODE(GPU_POLY_F4 | GPU_ABE);
    poly->x0 = rect->x + rect->w - 3;
    poly->y0 = rect->y;
    poly->x1 = rect->x + rect->w + 2;
    poly->y1 = rect->y + 5;
    poly->x2 = rect->x + rect->w - 3;
    poly->y2 = rect->y + rect->h;
    poly->x3 = rect->x + rect->w + 2;
    poly->y3 = rect->y + rect->h;
    g_primPtr += sizeof(PolyF4Half);
    addPrim(&g_ot[otz], poly);

    setlen(g_primPtr, 1);
    drMode = (DrTpageWords *)g_primPtr;
    drMode->code = get_mode(1, 1, 0);
    addPrim(&g_ot[otz], drMode);
    g_primPtr = (u8 *)(drMode + 1);
}

void ui_drawBackground(void) {
    s32 w;
    RECT tw;
    PolyG4Words *poly;

    w = g_gameState->dispEnv->disp.w;
    setlen(g_primPtr, 8);
    poly = (PolyG4Words *)g_primPtr;
    poly->rgbc0 = GPU_CODE(GPU_POLY_G4 | GPU_ABE);
    poly->rgb2 = 0x505050;
    poly->rgb3 = 0x505050;
    poly->xy1 = w;
    poly->rgb1 = 0;
    poly->xy0 = 0;
    poly->xy2 = 0x800000;
    poly->xy3 = w | 0x800000;
    addPrim(&g_ot[0xFFF], poly);
    g_primPtr += sizeof(PolyG4Words);

    setlen(g_primPtr, 1);
    ((DrTpageWords *)g_primPtr)->code = get_mode(1, 1, 0x40);
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);

    tw.x = g_uiBackgroundSprite.u;
    tw.y = g_uiBackgroundSprite.v;
    tw.w = g_uiBackgroundSprite.w;
    tw.h = g_uiBackgroundSprite.h;

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&g_noTexWindow);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    ((SprtWords *)g_primPtr)->xy = 0;
    ((SprtWords *)g_primPtr)->uvclut = g_uiBackgroundSprite.clut << 16;
    ((SprtWords *)g_primPtr)->wh = w | 0x800000;
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(SprtWords);

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, g_uiBackgroundSprite.tpage);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(&tw);
    addPrim(&g_ot[0xFFF], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
}

u8 *win_buildFill(RECT *rect, u32 *ot, u8 u, u8 v, s32 pal, u8 *pkt) {
    s32 clut = win_getClut(g_winFillSprite.clut, pal);
    RECT tw;

    tw.x = g_winFillSprite.u;
    tw.y = g_winFillSprite.v;
    tw.w = g_winFillSprite.w;
    tw.h = g_winFillSprite.h;

    setlen(pkt, 2);
    ((DrMode2Words *)pkt)->code[0] = get_tw(&g_noTexWindow);
    ((DrMode2Words *)pkt)->code[1] = 0;
    addPrim(ot, pkt);
    pkt += sizeof(DrMode2Words);

    setlen(pkt, 4);
    ((SprtWords *)pkt)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    ((SprtWords *)pkt)->xy = (u16)rect->x | (rect->y << 16);
    ((SprtWords *)pkt)->uvclut = u | (v << 8) | (clut << 16);
    ((SprtWords *)pkt)->wh = rect->w | (rect->h << 16);
    addPrim(ot, pkt);
    pkt += sizeof(SprtWords);

    setlen(pkt, 2);
    ((DrMode2Words *)pkt)->code[0] = get_mode(1, 1, g_winFillSprite.tpage);
    ((DrMode2Words *)pkt)->code[1] = get_tw(&tw);
    addPrim(ot, pkt);
    return pkt + sizeof(DrMode2Words);
}

void win_drawLineBorder(RECT *rect, s32 otz, u8 skip, s32 abr) {
    LineXY lines[4];
    s32 i;
    u32 *col;
    LineF2W *line;

    lines[0].x0 = rect->x + 1;
    lines[0].y0 = rect->y + 1;
    lines[0].x1 = rect->x + rect->w - 3;
    lines[0].y1 = rect->y + 1;
    lines[1].x0 = rect->x + 1;
    lines[1].y0 = rect->y + 2;
    lines[1].x1 = rect->x + 1;
    lines[1].y1 = rect->y + rect->h - 3;
    lines[2].x0 = rect->x + 1;
    lines[2].y0 = rect->y + rect->h - 2;
    lines[2].x1 = rect->x + rect->w - 2;
    lines[2].y1 = rect->y + rect->h - 2;
    lines[3].x0 = rect->x + rect->w - 2;
    lines[3].y0 = rect->y;
    lines[3].x1 = rect->x + rect->w - 2;
    lines[3].y1 = rect->y + rect->h - 3;
    col = g_winLineColors[abr];
    for (i = skip != 0; i < 4; i++) {
        line = (LineF2W *)g_primPtr;
        setlen(line, 3);
        line->rgbc = col[i >> 1];
        line->xy0 = (u16)lines[i].x0 | (lines[i].y0 << 16);
        line->xy1 = (u16)lines[i].x1 | (lines[i].y1 << 16);
        addPrim(&g_ot[otz], line);
        g_primPtr += sizeof(LineF2W);
        line = (LineF2W *)g_primPtr;
        setlen(line, 3);
        line->rgbc = col[(i >> 1) + 2];
        line->xy0 = ((lines[i].x0 + g_winShadowLineOffsets[i][0]) & 0xFFFF) | ((lines[i].y0 + g_winShadowLineOffsets[i][1]) << 16);
        line->xy1 = ((lines[i].x1 + g_winShadowLineOffsets[i][2]) & 0xFFFF) | ((lines[i].y1 + g_winShadowLineOffsets[i][3]) << 16);
        addPrim(&g_ot[otz], line);
        g_primPtr += sizeof(LineF2W);
    }
}

u8 *win_buildBorder(RECT *rect, u32 *ot, u8 flag, u8 abe, s32 style, s32 pal, u8 *pkt) {
    s32 edgeCount = 1;
    s32 semi = (abe != 0) << 25;
    UiSprite *sprite;
    s32 clut;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    RECT tw;

    if (flag) {
        edgeCount = 2;
    }
    sprite = NULL;
    if (style != 2) {
        clut = win_getClut(g_uiSprites[g_winCornerSprites[style][0]].uvclut >> 16, pal) << 16;
    } else {
        clut = 0x3EA40000;
    }
    for (i = 0, y = rect->y; i < 2; i++) {
        for (j = 0, x = rect->x; j < 2; j++) {
            if (flag || i || !j) {
                sprite = (UiSprite *)&g_uiSprites[g_winCornerSprites[style][i * 2 + j]];
                setlen(pkt, 3);
                ((Sprt8Words *)pkt)->rgbc = g_winCornerRgbc[flag] | semi;
                ((Sprt8Words *)pkt)->xy = (u16)x | (y << 16);
                ((Sprt8Words *)pkt)->uvclut = sprite->u | (sprite->v << 8) | clut;
                addPrim(ot, pkt);
                pkt += sizeof(Sprt8Words);
                x += rect->w - 8;
            }
        }
        y += rect->h - 8;
    }

    setlen(pkt, 2);
    ((DrMode2Words *)pkt)->code[0] = get_tw(&g_noTexWindow);
    ((DrMode2Words *)pkt)->code[1] = 0;
    addPrim(ot, pkt);
    pkt += sizeof(DrMode2Words);

    if (rect->w - 16 > 0) {
        y = rect->y + rect->h - 8;
        for (i = 0; i < edgeCount; i++) {
            sprite = (UiSprite *)&g_uiSprites[g_winHEdgeSprites[style][i]];
            setlen(pkt, 4);
            ((SprtWords *)pkt)->rgbc = g_winEdgeRgbc[flag] | semi;
            ((SprtWords *)pkt)->xy = ((rect->x + 8) & 0xFFFF) | (y << 16);
            ((SprtWords *)pkt)->uvclut = clut;
            ((SprtWords *)pkt)->wh = (rect->w - 16) | 0x80000;
            addPrim(ot, pkt);
            y -= rect->h - 8;
            tw.x = sprite->u;
            tw.y = sprite->v;
            tw.w = sprite->w;
            tw.h = sprite->h;
            pkt += sizeof(SprtWords);
            setlen(pkt, 2);
            ((DrMode2Words *)pkt)->code[0] = get_tw(&tw);
            ((DrMode2Words *)pkt)->code[1] = 0;
            addPrim(ot, pkt);
            pkt += sizeof(DrMode2Words);
        }
    }
    if (rect->h - 16 > 0) {
        x = rect->x;
        for (i = 0; i < edgeCount; i++) {
            sprite = (UiSprite *)&g_uiSprites[g_winVEdgeSprites[style][i]];
            setlen(pkt, 4);
            ((SprtWords *)pkt)->rgbc = g_winEdgeRgbc[flag] | semi;
            ((SprtWords *)pkt)->xy = (x & 0xFFFF) | ((rect->y + 8) << 16);
            ((SprtWords *)pkt)->uvclut = clut;
            ((SprtWords *)pkt)->wh = ((rect->h - 16) << 16) | 8;
            addPrim(ot, pkt);
            x += rect->w - 8;
            tw.x = sprite->u;
            tw.y = sprite->v;
            tw.w = sprite->w;
            tw.h = sprite->h;
            pkt += sizeof(SprtWords);
            setlen(pkt, 2);
            ((DrMode2Words *)pkt)->code[0] = get_tw(&tw);
            ((DrMode2Words *)pkt)->code[1] = 0;
            addPrim(ot, pkt);
            pkt += sizeof(DrMode2Words);
        }
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, sprite->tpage);
    addPrim(ot, pkt);
    return pkt + sizeof(DrTpageWords);
}

void ui_drawRule(s32 x, s32 y, s32 otz, s32 style, s32 w, s32 abr) {
    UiSprite *sprite;
    u16 clut;
    s32 i;
    RECT tw;
    UiSprite *tbl;
    UiSprite *tbl2;
    s32 yy;

    if (style == 1) {
        tbl = (UiSprite *)g_uiSprites;
        clut = tbl[g_winRuleSprites[1][0]].clut;
    } else {
        tbl2 = (UiSprite *)g_uiSprites;
        clut = win_getClut(tbl2[g_winRuleSprites[style][0]].clut, abr);
    }
    for (i = 0; i < 2; i++) {
        sprite = &((UiSprite *)g_uiSprites)[g_winRuleSprites[style][i]];
        setlen(g_primPtr, 4);
        yy = y << 16;
        ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
        if (i != 0) {
            ((SprtWords *)g_primPtr)->xy = ((x - (8 - w)) & 0xFFFF) | yy;
        } else {
            ((SprtWords *)g_primPtr)->xy = (x & 0xFFFF) | yy;
        }
        ((SprtWords *)g_primPtr)->uvclut = sprite->u | (sprite->v << 8) | (clut << 16);
        ((SprtWords *)g_primPtr)->wh = sprite->w | (sprite->h << 16);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(SprtWords);
    }
    if (w > 0x10) {
        setlen(g_primPtr, 2);
        ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&g_noTexWindow);
        ((DrMode2Words *)g_primPtr)->code[1] = 0;
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DrMode2Words);
        sprite = &((UiSprite *)g_uiSprites)[g_winRuleSprites[style][2]];
        tw.x = sprite->u;
        tw.y = sprite->v;
        tw.w = sprite->w;
        tw.h = sprite->h;
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
        ((SprtWords *)g_primPtr)->xy = ((x + 8) & 0xFFFF) | (y << 16);
        ((SprtWords *)g_primPtr)->uvclut = (style << 10) | (clut << 16);
        ((SprtWords *)g_primPtr)->wh = (w - 0x10) | (sprite->h << 16);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(SprtWords);
    }
    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_mode(1, 1, sprite->tpage);
    ((DrMode2Words *)g_primPtr)->code[1] = get_tw(w > 0x10 ? &tw : &g_noTexWindow);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
}

void ui_drawGauge(s32 x, s32 y, s32 otz, s32 cur, s32 max, s32 style, u8 blink) {
    CVECTOR c1;
    CVECTOR c2;
    GaugeColors *colors;
    CVECTOR *f1, *f2, *t1, *t2;
    s32 w;
    s32 k;
    s32 right;
    PolyG4Words *poly;
    TileWords *tile;

    w = cur * 36 / max;
    colors = &g_gaugeColors[style];
    if (cur != 0) {
        if (blink && cur >= max) {
            k = g_winLastVsync & 0x1F;
            if (k < 16) {
                f1 = &colors->c[2];
                if (k >= 8) {
                    k = 15 - k;
                }
                f2 = &colors->c[3];
                t1 = &colors->c[0];
                t2 = &colors->c[0];
            } else {
                k -= 16;
                f1 = &colors->c[2];
                if (k >= 8) {
                    k = 15 - k;
                }
                f2 = &colors->c[3];
                t1 = &colors->c[1];
                t2 = t1;
            }
            CVECTOR_LERP(c1, f1, t1, k, 7);
            CVECTOR_LERP(c2, f2, t2, k, 7);
        } else {
            t1 = &colors->c[3];
            c1 = colors->c[2];
            CVECTOR_LERP(c2, &c1, t1, cur, max);
        }
        setlen(g_primPtr, 8);
        poly = (PolyG4Words *)g_primPtr;
        poly->rgbc0 = poly->rgb2 = c1.r | (c1.g << 8) | (c1.b << 16) | GPU_CODE(GPU_POLY_G4);
        poly->rgb1 = poly->rgb3 = c2.r | (c2.g << 8) | (c2.b << 16);
        right = w + 2;
        poly->xy0 = (x + 2) | ((y + 2) << 16);
        poly->xy2 = (x + 2) | ((y + 4) << 16);
        poly->xy1 = (x + right) | ((y + 2) << 16);
        poly->xy3 = (x + right) | ((y + 4) << 16);
        addPrim(&g_ot[otz], poly);
        g_primPtr = (u8 *)(poly + 1);
    }
    ui_drawSprite(x, y, otz, 2, 0);
    setlen(g_primPtr, 3);
    tile = (TileWords *)g_primPtr;
    tile->rgbc = *(u32 *)&colors->c[0];
    tile->xy = (x + 1) | ((y + 1) << 16);
    tile->wh = (w < 36 ? 0x30000 : 0x10000) | 0x26;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(TileWords);
}

void win_drawTailEdge(s16 x, s16 y, s32 otz, s32 idx, u8 flag, u8 mode) {
    UiSprite *sprite = &g_winTailSprites[idx];
    u32 clut;
    SprtWords *sprt;
    DrTpageWords *drMode;

    if (!mode) {
        clut = (sprite->clut - CONFIG_WINDOW_COLOR() * 0xC0) << 16;
    } else {
        clut = 0x3EA40000;
    }
    setlen(g_primPtr, 4);
    ((SprtWords *)g_primPtr)->rgbc = flag ? GPU_RGBC(GPU_SPRT | GPU_ABE, GPU_RGB_NEUTRAL) : GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    sprt = (SprtWords *)g_primPtr;
    sprt->xy = x | (y << 16);
    sprt->uvclut = sprite->u | (sprite->v << 8) | clut;
    sprt->wh = sprite->w | (sprite->h << 16);
    addPrim(&g_ot[otz], sprt);
    g_primPtr = (u8 *)(sprt + 1);
    setlen(g_primPtr, 1);
    drMode = (DrTpageWords *)g_primPtr;
    drMode->code = get_mode(1, 1, sprite->tpage);
    addPrim(&g_ot[otz], drMode);
    g_primPtr = (u8 *)(drMode + 1);
}

void win_drawTailFill(s32 x, s32 y, s32 otz, s32 shape, s32 u, s32 v) {
    RECT tw;
    Unk80068F48 *tailShape;
    u16 clut;

    clut = g_winFillSprite.clut - CONFIG_WINDOW_COLOR() * 0xC0;
    tw.x = g_winFillSprite.u;
    tw.y = g_winFillSprite.v;
    tw.w = g_winFillSprite.w;
    tw.h = g_winFillSprite.h;

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&g_noTexWindow);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);

    setlen(g_primPtr, 9);
    tailShape = &g_winTailShapes[shape];
    ((PolyFT4Words *)g_primPtr)->rgbc = GPU_RGBC(GPU_POLY_FT4, GPU_RGB_NEUTRAL);
    ((PolyFT4Words *)g_primPtr)->xy0 = (x + tailShape->x0) | ((y + tailShape->y0) << 16);
    ((PolyFT4Words *)g_primPtr)->uv0clut = (u + tailShape->x0) | ((v + tailShape->y0) << 8) | (clut << 16);
    ((PolyFT4Words *)g_primPtr)->xy1 = (x + tailShape->x1) | ((y + tailShape->y1) << 16);
    ((PolyFT4Words *)g_primPtr)->uv1tpage = (u + tailShape->x1) | ((v + tailShape->y1) << 8) | (g_winFillSprite.tpage << 16);
    ((PolyFT4Words *)g_primPtr)->xy2 = (x + tailShape->x2) | ((y + tailShape->y2) << 16);
    ((PolyFT4Words *)g_primPtr)->uv2 = (u + tailShape->x2) | ((v + tailShape->y2) << 8);
    ((PolyFT4Words *)g_primPtr)->xy3 = (x + tailShape->x3) | ((y + tailShape->y3) << 16);
    ((PolyFT4Words *)g_primPtr)->uv3 = (u + tailShape->x3) | ((v + tailShape->y3) << 8);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(PolyFT4Words);

    setlen(g_primPtr, 2);
    ((DrMode2Words *)g_primPtr)->code[0] = get_tw(&tw);
    ((DrMode2Words *)g_primPtr)->code[1] = 0;
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrMode2Words);
}

void win_drawHelpTailFill(s32 x, s32 y, s32 otz, s32 idx) {
    PolyF4Words *poly;
    Unk80068F48 *tailShape;
    setlen(g_primPtr, 5);
    poly = (PolyF4Words *)g_primPtr;
    poly->rgbc = GPU_RGBC(GPU_POLY_F4, 0xA8B078);
    tailShape = &g_winTailShapes[idx];
    poly->xy0 = (x + tailShape->x0) | ((y + tailShape->y0) << 16);
    poly->xy1 = (x + tailShape->x1) | ((y + tailShape->y1) << 16);
    poly->xy2 = (x + tailShape->x2) | ((y + tailShape->y2) << 16);
    poly->xy3 = (x + tailShape->x3) | ((y + tailShape->y3) << 16);
    addPrim(&g_ot[otz], poly);
    g_primPtr = (u8 *)(poly + 1);
}

void ui_drawEtchedLine(s32 x, s32 y, s32 otz, s32 len) {
    u32 col[3];
    s32 i;
    LineF2W *line;

    col[0] = g_winLineColors[CONFIG_WINDOW_COLOR()][1];
    col[1] = GPU_RGBC(GPU_LINE_F2, 0x403838);
    col[2] = g_winLineColors[CONFIG_WINDOW_COLOR()][0];

    for (i = 0; i < 3; i++) {
        line = (LineF2W *)g_primPtr;
        setlen(line, 3);
        line->rgbc = col[i];
        if (len < 0) {
            line->xy0 = (x + i) | (y << 16);
            line->xy1 = (x + i) | ((y - len) << 16);
        } else {
            line->xy0 = x | ((y + i) << 16);
            line->xy1 = (x + len - 1) | ((y + i) << 16);
        }
        addPrim(&g_ot[otz], line);
        g_primPtr += sizeof(LineF2W);
    }
}

void win_drawBorder(RECT *rect, s32 otz, u8 flag, u8 abe, s32 pal) {
    g_primPtr = win_buildBorder(rect, &g_ot[otz], flag, abe, 0, pal, g_primPtr);
}

void win_drawBalloonBorder(RECT *rect, s32 otz, u8 flag, u8 abe, s32 pal) {
    g_primPtr = win_buildBorder(rect, &g_ot[otz], flag, abe, 1, pal, g_primPtr);
}

void win_drawHelpBorder(RECT *rect, s32 otz, u8 flag, u8 abe, s32 pal) {
    g_primPtr = win_buildBorder(rect, &g_ot[otz], flag, abe, 2, pal, g_primPtr);
}

void ui_drawItemIcon(s32 x, s32 y, s32 otz, s32 item, u8 flag) {
    g_primPtr = ui_buildItemIcon(x, y, &g_ot[otz], item, flag, g_primPtr);
}

u8 *ui_buildItemIcon(s32 x, s32 y, u32 *ot, s32 item, u8 flag, u8 *pkt) {
    ItemInfo *info = &g_itemTable[item];
    UiSprite *sprite = &g_itemIconSprites[info->icon];
    s32 clut;

    clut = sprite->clut + (flag ? info->iconPalette << 6 : 0x3C0);
    setlen(pkt, 4);
    ((SprtWords *)pkt)->rgbc = (flag == 0 ? GPU_RGBC(GPU_SPRT | GPU_ABE, GPU_RGB_NEUTRAL) : GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL));
    ((SprtWords *)pkt)->xy = (x & 0xFFFF) | (y << 16);
    ((SprtWords *)pkt)->uvclut = sprite->u | (sprite->v << 8) | (clut << 16);
    ((SprtWords *)pkt)->wh = sprite->w | (sprite->h << 16);
    addPrim(ot, pkt);
    pkt += sizeof(SprtWords);
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, sprite->tpage);
    addPrim(ot, pkt);
    return pkt + sizeof(DrTpageWords);
}

void ui_drawItemIcon2(s32 x, s32 y, s32 otz, s32 item, u8 flag) {
    g_primPtr = ui_buildItemIcon(x, y, &g_ot[otz], item, flag, g_primPtr);
}

void ui_drawMember(s32 x, s32 y, s32 otz, s32 slot) {
    Character *chr = g_gameState->gameData->party[slot];

    if (chr != NULL) {
        ui_drawPartyPortrait(x, y, otz, slot, chr->hp == 0);
        ui_drawMemberStats(x + 0x26, y + 3, otz, slot);
    }
}

void ui_drawMemberSummary(s32 x, s32 y, s32 otz, s32 slot, s32 mode) {
    Character *chr = g_gameState->gameData->party[slot];
    if (chr != NULL && mode >= 0) {
        if (mode < 3) {
            ui_drawPortrait(x, y, otz, slot, 0x29, chr->hp == 0);
            ui_drawMemberStats(x + 0x26, y, otz, slot);
        } else if (mode < 5) {
            ui_drawPortrait(x, y, otz, slot, 0x29, chr->hp == 0);
            ui_drawCharStatuses(x + 0x26, y, otz, slot);
        }
    }
}

void ui_drawMemberStats(s32 x, s32 y, s32 otz, s32 slot) {
    ui_drawCharStatus(x, y, otz, g_gameState->gameData->party[slot]);
}

void ui_drawStatusIcons(s32 x, s32 y, s32 otz, u8 statusBits, s32 unused) {
    s32 i;

    for (i = 0; i < 7; i++) {
        if ((statusBits >> i) & 1) {
            ui_drawSprite(x, y, otz, g_statusIconSprites[i], 0);
            x += 12;
        }
    }
}

void win_drawShadow(RECT *rect, s32 otz, s32 style) {
    void (*fn)(RECT *, s32) = g_winShadowFuncs[style];

    if (fn != 0) {
        fn(rect, otz);
    }
}

void win_drawFill(RECT *rect, s32 otz, u8 u, u8 v, s32 pal) {
    g_primPtr = win_buildFill(rect, &g_ot[otz], u, v, pal, g_primPtr);
}

void ui_drawHand(s32 x, s32 y, s32 otz) {
    ui_drawSprite(x, y, otz, 1, 0);
}

void ui_drawApGauge(s32 x, s32 y, s32 otz, s32 cur, s32 max, u8 show, u8 flag) {
    g_primPtr = ui_buildApGauge(x, y, &g_ot[otz], cur, max, show, flag, g_primPtr);
}

u8 *ui_buildApGauge(s32 x, s32 y, u32 *ot, s32 cur, s32 max, u8 show, u8 flag, u8 *pkt) {
    u8 numText[16];
    s32 spriteW[2];
    s32 w;
    s32 barW;
    u32 rgbc;
    SpriteDef *def = &g_uiSprites[UI_SPRITE_AP_GAUGE];

    if (max < cur) {
        cur = max;
    }
    if (cur != 0 && cur >= max) {
        pkt = ui_buildSprite(x + 4, y, ot, 0xC0, flag, spriteW, pkt);
    } else if (show) {
        text_formatNumber(cur, numText, 3, TEXT_NUM_ASCII);
        text_formatNumber(max, numText + 4, TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
        numText[3] = 0x2F;
        pkt = ui_buildDigits(x + 3, y + 2, ot, numText, flag ? GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060) : GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL), (SprtWords *)pkt);
    }
    if (cur != 0) {
        barW = def->w - 6;
        if (max != 0) {
            w = cur * barW / max;
        } else {
            w = barW;
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
    return ui_buildSpriteDef(x, y, ot, def, flag, spriteW, (SprtWords *)pkt);
}

void ui_drawStones(s32 x, s32 y, s32 otz, s32 cur, s32 max) {
    u8 numText[8];

    text_formatNumber(cur, &numText[0], 2, TEXT_NUM_ASCII);
    text_formatNumber(max, &numText[3], TEXT_NUM_NO_PAD, TEXT_NUM_ASCII);
    numText[2] = '/';
    ui_drawSprite(x, y, otz, 0xC1, 0);
    ui_drawDigits(x + 15, y + 1, otz, numText, cur == 0 ? DIGITS_YELLOW : DIGITS_WHITE);
}

u16 win_getPalClut(s32 baseClut) {
    return baseClut - CONFIG_WINDOW_COLOR() * 0xC0;
}

u16 win_getClut(s32 baseClut, s32 pal) {
    return baseClut - pal * 0xC0;
}

void win_drawHelpFill(RECT *rect, s32 otz) {
    TileWords *tile;

    setlen(g_primPtr, 3);
    tile = (TileWords *)g_primPtr;
    tile->rgbc = GPU_RGBC(GPU_TILE, 0xA8B078);
    tile->xy = (u16)rect->x | (rect->y << 16);
    tile->wh = rect->w | (rect->h << 16);
    addPrim(&g_ot[otz], tile);
    g_primPtr = (u8 *)(tile + 1);
}

/* K&R definition: callers pass plain ints */
void win_drawHelpTailEdge(x, y, otz, idx, flag)
    s16 x;
    s16 y;
    s32 otz;
    s32 idx;
    u8 flag;
{
    win_drawTailEdge(x, y, otz, idx, flag, 1);
}

/* K&R definition: callers pass plain ints */
void win_drawMsgTailEdge(x, y, otz, idx, flag)
    s16 x;
    s16 y;
    s32 otz;
    s32 idx;
    u8 flag;
{
    win_drawTailEdge(x, y, otz, idx, flag, 0);
}

s32 ui_drawSprite(s32 x, s32 y, s32 otz, s32 id, u8 flag) {
    s32 w;
    SpriteDef *def = &g_uiSprites[id];

    g_primPtr = ui_buildSpriteDef_inline(x, y, &g_ot[otz], def, flag, &w, (SprtWords *)g_primPtr);
    return w;
}

s32 ui_drawSpriteDef(s32 x, s32 y, s32 otz, void *def, u8 flag) {
    s32 w;

    g_primPtr = ui_buildSpriteDef_inline(x, y, &g_ot[otz], def, flag, &w, (SprtWords *)g_primPtr);
    return w;
}

u8 *ui_buildSprite(s32 x, s32 y, u32 *ot, s32 i, u8 flag, s32 *outWidth, void *pkt) {
    SprtWords *sprt = pkt;
    DrTpageWords *drMode;
    SpriteDef *def;
    setlen(sprt, 4);
    def = &g_uiSprites[i];
    if (flag) {
        sprt->rgbc = GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060);
    } else {
        sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    }
    sprt->xy = (x & 0xFFFF) | (y << 16);
    sprt->uvclut = def->uvclut;
    sprt->wh = def->w | (def->h << 16);
    addPrim(ot, sprt);
    sprt++;
    drMode = (DrTpageWords *)sprt;
    setlen(drMode, 1);
    drMode->code = get_mode(1, 1, def->tpage);
    addPrim(ot, drMode);
    *outWidth = def->w;
    return (u8 *)(drMode + 1);
}

u8 *ui_buildSpriteDef(s32 x, s32 y, u32 *ot, SpriteDef *def, u8 flag, s32 *outWidth, SprtWords *sprt) {
    return ui_buildSpriteDef_inline(x, y, ot, def, flag, outWidth, sprt);
}

void ui_drawCharStatuses(s32 x, s32 y, s32 otz, s32 slot) {
    Character *chr = g_gameState->gameData->party[slot];
    u8 numText[16];
    RECT rect;

    if (chr == NULL) {
        return;
    }
    text_formatNumber(chr->level, numText, 2, TEXT_NUM_ASCII);
    text_drawBlock(x, y, otz, chr->name, TEXT_PAL_WHITE);
    ui_drawSmallDigits(x + 0x26, y + 0xE, otz, numText, TEXT_PAL_WHITE);
    ui_drawSprite(x, y + 0xE, otz, 0xB4, 0);
    ui_drawStatusIcons(x, y + 0x1C, otz, chr->status, 0);
    rect.x = x - 4;
    rect.y = y + 0x18;
    rect.w = 0x60;
    rect.h = 0x12;
    win_drawBox(&rect, otz, WIN_STYLE_LINE);
}

u8 *ui_buildDigits(s32 x, s32 y, u32 *ot, u8 *str, s32 unused, SprtWords *sprt) {
    s32 idx;
    s32 adv;
    SpriteDef *def;
    DrTpageWords *drMode;

    while (*str != 0) {
        if (*str != ' ') {
            if (*str == '/') {
                idx = 11;
                adv = 4;
            } else {
                adv = 5;
                if (*str != '%') {
                    idx = *str - '0';
                } else {
                    idx = 10;
                }
            }
            setlen(sprt, 4);
            sprt->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
            sprt->xy = (x & 0xFFFF) | (y << 16);
            def = &g_uiSprites[UI_SPRITE_SMALL_DIGITS + idx];
            sprt->wh = def->w | (def->h << 16);
            sprt->uvclut = def->uvclut;
            addPrim(ot, sprt);
            sprt++;
        } else {
            adv = 5;
        }
        str++;
        x += adv;
    }
    drMode = (DrTpageWords *)sprt;
    setlen(drMode, 1);
    drMode->code = get_mode(1, 1, 0x1F);
    addPrim(ot, drMode);
    return (u8 *)(drMode + 1);
}

void item_initNew(void) {
    u8 *p = g_newGameItems;

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
        g_gameState->gameData->items.slots[i].count = 0;
    }
}

void keyItem_clearAll(void) {
    s32 i;

    for (i = 0; i < 0x40; i++) {
        g_gameState->gameData->keyItems.bits[i] = 0;
    }
}

s32 item_add(s32 id, s32 amount) {
    ItemSlot *slot;
    s32 i;
    if (id == 0xFF) {
        return 0;
    }
    slot = g_gameState->gameData->items.slots;
    for (i = 0; i < 0x100; slot++, i++) {
        if (slot->count != 0 && slot->id == id) {
            if (slot->count + amount >= 100) {
                amount = 99 - slot->count;
            }
            slot->count += amount;
            return amount;
        }
    }
    slot = g_gameState->gameData->items.slots;
    for (i = 0; i < 0x100; i++, slot++) {
        if (slot->count == 0) {
            slot->id = id;
            slot->count = amount;
            return amount;
        }
    }
    return 0;
}

s32 item_remove(s32 id, s32 count) {
    s32 i;
    ItemSlot *slot = g_gameState->gameData->items.slots;

    for (i = 0; i < 0x100; slot++, i++) {
        if (slot->count != 0 && slot->id == id) {
            if (slot->count < count) {
                count = slot->count;
            }
            slot->count -= count;
            return count;
        }
    }
    return 0;
}

/* Same logic as item_findSlot_inline (ff9.h). Written out because a call of the inline compiles the loop exit
   differently. */
ItemSlot *item_findSlot(s32 id) {
    s32 i;
    ItemSlot *slot = g_gameState->gameData->items.slots;

    for (i = 0; i < 0x100; i++, slot++) {
        if (slot->count != 0 && slot->id == id) {
            return slot;
        }
    }
    return NULL;
}

s32 item_getCount(s32 id) {
    ItemSlot *slot = item_findSlot_inline(id);

    return slot != NULL ? slot->count : 0;
}

s32 item_countEquipped(s32 id) {
    s32 i, j;
    s32 count = 0;

    for (i = 0; i < 9; i++) {
        if (g_gameState->gameData->characters[i].info.b.recruited) {
            for (j = 0; j < 5; j++) {
                if (id == g_gameState->gameData->characters[i].equip[j]) {
                    count++;
                }
            }
        }
    }
    return count;
}

s32 item_getEquipSlot(s32 itemId) {
    s32 i;
    ItemInfo *info = &g_itemTable[itemId];

    for (i = 0; i < 5; i++) {
        if (info->slots & g_equipSlotMasks[i]) {
            return i;
        }
    }
    return -1;
}

void keyItem_add(s32 id) {
    s32 idx = id >> 2;

    g_gameState->gameData->keyItems.bits[idx] |= 1 << ((id & 3) * 2);
    g_gameState->gameData->keyItems.bits[idx] &= ~(1 << ((id & 3) * 2 + 1));
}

void keyItem_remove(s32 id) {
    s32 idx = id >> 2;

    g_gameState->gameData->keyItems.bits[idx] &= ~(1 << ((id & 3) * 2));
    g_gameState->gameData->keyItems.bits[idx] &= ~(1 << ((id & 3) * 2 + 1));
}

void keyItem_setSeen(s32 id) {
    g_gameState->gameData->keyItems.bits[id >> 2] |= 1 << ((id & 3) * 2 + 1);
}

void keyItem_clearSeen(s32 id) {
    g_gameState->gameData->keyItems.bits[id >> 2] &= ~(1 << ((id & 3) * 2 + 1));
}

s32 keyItem_has(s32 keyItemId) {
    return (g_gameState->gameData->keyItems.bits[keyItemId >> 2] >> ((keyItemId & 3) * 2)) & 1;
}

s32 keyItem_isSeen(s32 id) {
    return (g_gameState->gameData->keyItems.bits[id >> 2] >> ((id & 3) * 2 + 1)) & 1;
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
        ((SprtWords *)g_primPtr)->rgbc = flag == 0 ? GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL) : GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060);
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
    SprtWords *topSprt;
    SprtWords *lowSprt;

    h0 = 0x20;
    if (h < 0x20) {
        h0 = h;
    }
    if (g_gameState->mode == GS_MODE_WORLD) {
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
    ((SprtWords *)g_primPtr)->rgbc = flag == 0 ? GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL) : GPU_RGBC(GPU_SPRT | GPU_ABE, GPU_RGB_NEUTRAL);
    topSprt = (SprtWords *)g_primPtr;
    g_primPtr += sizeof(SprtWords);
    topSprt->xy = (x & 0xFFFF) | (y << 16);
    topSprt->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (clut << 16);
    topSprt->wh = (h0 << 16) | 0x20;
    addPrim(&g_ot[otz], topSprt);
    SetDrawTPage(g_primPtr, 1, 1, 0x88);
    addPrim(&g_ot[otz], g_primPtr);
    g_primPtr += sizeof(DrTpageWords);
    if (h > 0x20) {
        u = ((slot & 1) << 5) + 0x20;
        v = (slot >> 1) * 14 - 0x20;
        h -= h0;
        setlen(g_primPtr, 4);
        ((SprtWords *)g_primPtr)->rgbc = flag == 0 ? GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL) : GPU_RGBC(GPU_SPRT | GPU_ABE, GPU_RGB_NEUTRAL);
        lowSprt = (SprtWords *)g_primPtr;
        g_primPtr += sizeof(SprtWords);
        lowSprt->xy = (x & 0xFFFF) | ((y + 0x20) << 16);
        lowSprt->uvclut = (u & 0xFF) | ((v & 0xFF) << 8) | (clut << 16);
        lowSprt->wh = (h << 16) | 0x20;
        addPrim(&g_ot[otz], lowSprt);
        SetDrawTPage(g_primPtr, 1, 1, 0x89);
        addPrim(&g_ot[otz], g_primPtr);
        g_primPtr += sizeof(DrTpageWords);
    }
}

void ui_loadPortraits(void *data, s32 count) {
    RECT r0;
    RECT r1;
    s32 i;
    u32 *prev = data;
    u32 *imgSave = prev;
    u32 *cur;

    g_portraits.count = count;
    g_portraits.images = prev;
    ui_getPortraitRects(0, &r0, &r1);
    StoreImage(&r0, g_portraitSavedVram);
    DrawSync(0);
    LoadImage(&r0, prev);
    DrawSync(0);
    LoadImage(&r1, prev + 0x40);
    DrawSync(0);
    cur = prev + 0x1B0;
    for (i = 1; i < g_portraits.count; i++) {
        ui_getPortraitRects(i, &r0, &r1);
        StoreImage(&r0, prev);
        DrawSync(0);
        if (i >= 4) {
            StoreImage(&r1, imgSave + 0x40);
            DrawSync(0);
            imgSave += 0x1B0;
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
    u32 *clutSrc = g_portraits.images;
    u32 *imgSrc;
    s32 i;
    ui_getPortraitRects(0, &clut, &img);
    LoadImage(&clut, g_portraitSavedVram);
    DrawSync(0);
    imgSrc = clutSrc;
    for (i = 1; i < g_portraits.count; i++) {
        ui_getPortraitRects(i, &clut, &img);
        LoadImage(&clut, clutSrc);
        DrawSync(0);
        if (i >= 4) {
            LoadImage(&img, imgSrc + 0x40);
            DrawSync(0);
            imgSrc += 0x1B0;
        }
        clutSrc += 0x1B0;
    }
}

s32 chr_getPortrait(s32 charId) {
    return g_portraitByKind[g_gameState->gameData->characters[charId].info.b.kind];
}

void ui_getPortraitRects(s32 portrait, RECT *clut, RECT *img) {
    clut->y = portrait + 0x1F3;
    clut->w = 0x80;
    clut->h = 1;
    clut->x = 0;
    if (portrait < 4) {
        img->x = portrait * 16 + 0x3C0;
        img->y = 0x1B0;
    } else {
        s32 idx = portrait - 4;

        img->x = (idx & 3) * 16;
        img->y = (idx >> 2) * 0x2E + 0x100;
    }
    img->w = 0x10;
    img->h = 0x2E;
}

void ui_drawPortraitIdFull(s32 x, s32 y, s32 otz, s32 id, u8 flag) {
    ui_drawPortraitId(x, y, otz, id, 0x2E, flag);
}

void ui_getPartyPortraitRects(s32 slot, RECT *clutRect, RECT *topRect, RECT *lowRect) {
    s32 x, y;

    if (g_gameState->mode == GS_MODE_WORLD) {
        x = 0x280;
        y = 0xDC;
    } else {
        x = 0x100;
        y = 0xFD;
    }
    clutRect->x = x + ((slot & 1) << 7);
    clutRect->y = y + (slot >> 1);
    clutRect->w = 0x80;
    clutRect->h = 1;
    topRect->x = slot * 16 + 0x200;
    topRect->y = 0xE0;
    topRect->w = 0x10;
    topRect->h = 0x20;
    lowRect->x = (slot & 1) * 16 + 0x250;
    lowRect->y = (slot >> 1) * 14 + 0xE0;
    lowRect->w = 0x10;
    lowRect->h = 0xE;
}

void ui_drawPartyPortrait(s32 x, s32 y, s32 otz, s32 slot, u8 flag) {
    ui_drawPortrait(x, y, otz, slot, 0x2E, flag);
}

void ui_loadPartyPortraits(u8 *portraitData) {
    s32 i;

    for (i = 0; i < 4; i++) {
        GameData *gd = g_gameState->gameData;
        Character *chr = gd->party[i];

        if (chr != NULL) {
            ui_loadPartyPortrait(portraitData + g_portraitByKind[gd->characters[chr->info.b.id].info.b.kind] * 0x6C0, i);
        }
    }
}

void ui_loadPartyPortrait(u8 *portraitData, s32 slot) {
    RECT clutRect, topRect, lowRect;

    ui_getPartyPortraitRects(slot, &clutRect, &topRect, &lowRect);
    LoadImage(&clutRect, (u32 *)portraitData);
    DrawSync(0);
    LoadImage(&topRect, (u32 *)(portraitData + 0x100));
    DrawSync(0);
    LoadImage(&lowRect, (u32 *)(portraitData + 0x500));
    DrawSync(0);
}

void ui_savePartyPortrait(u8 *portraitData, s32 slot) {
    RECT clutRect, topRect, lowRect;

    ui_getPartyPortraitRects(slot, &clutRect, &topRect, &lowRect);
    StoreImage(&clutRect, (u32 *)portraitData);
    DrawSync(0);
    StoreImage(&topRect, (u32 *)(portraitData + 0x100));
    DrawSync(0);
    StoreImage(&lowRect, (u32 *)(portraitData + 0x500));
    DrawSync(0);
}

void ui_initMemberSwitch(MemberSwitch *memberSw, s32 member, void (*cb)(void)) {
    memberSw->active = 0;
    memberSw->member = member;
    memberSw->onDone = cb;
}

void ui_startMemberSwitch(MemberSwitch *memberSw, s32 member, s32 nextMember, s32 dir) {
    memberSw->dir = dir;
    memberSw->frame = 0;
    if (dir != 0) {
        memberSw->member = member;
        memberSw->nextMember = nextMember;
        memberSw->active = 1;
    }
}

void ui_updateMemberSwitch(MemberSwitch *memberSw) {
    void (*cb)(void);

    if (memberSw->active != 0) {
        if (++memberSw->frame >= 12) {
            memberSw->member = memberSw->nextMember;
            cb = memberSw->onDone;
            memberSw->frame = 0;
            memberSw->active = 0;
            if (cb != NULL) {
                cb();
            }
        }
    }
}
