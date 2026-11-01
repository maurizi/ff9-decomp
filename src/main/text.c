#include "ff9.h"

s32 text_renderNameGlyph(s32 x, s32 hi, s32 c) {
    u8 rowPixels[16];
    u8 w;
    u8 start;
    u8 width;
    u8 *dst;
    u8 *src;
    s32 row;
    s32 i;
    s32 j;
    u8 b;
    u8 *p;
    s32 px;
    u8 v, o;
    u8 t;

    dst = (u8 *)g_nameImage;
    w = g_nameGlyphSpans[c];
    start = w >> 4;
    width = (w & 0xF) + 1;
    src = (u8 *)g_nameGlyphs + (c << 6);
    memset(rowPixels, 0, 0x10);
    for (row = 0; row < 12; row++) {
        for (i = 0; i < 4; i++) {
            b = src[row * 4 + i];
            for (j = 0; j < 4; j++) {
                rowPixels[i * 4 + j] = b & 3;
                b >>= 2;
            }
        }
        for (i = 0; i < width; i++) {
            px = x + i;
            if (!(px & 1)) {
                p = dst + (px >> 1);
                v = rowPixels[start + i];
                o = *p;
                if (hi) { *p = o | (v << 2); } else { *p = o | v; }
            } else {
                v = rowPixels[start + i];
                px >>= 1;
                p = dst + px;
                o = *p;
                t = v << 4; if (hi) { *p = o | (v << 6); } else { *p = o | t; }
            }
        }
        dst += 0x54;
    }
    return width + x;
}

s32 text_drawChar(s32 x, s32 y, s32 otz, u8 c, s32 mode) {
    u32 *tbl = g_gameState->gameData->font->glyphs;
    u32 g;
    s32 w;
    SprtWords *p;

    if (c == 0xFF) {
        return 0;
    }
    g = tbl[c];
    setlen(g_primPtr, PRIM_LEN(SPRT));
    w = (g >> 16) & 0xF;
    p = (SprtWords *)g_primPtr;
    p->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    p->xy = (x + ((10 - w) >> 1)) | (y << 16);
    p->uvclut = (g & 0x40FFFF) | (g_textPalettes[mode] << 16);
    p->wh = w | 0xD0000;
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
    return g >> 28;
}

void text_draw(s32 x, s32 y, s32 otz, u8 *s, s32 pal) {
    Font *f = g_gameState->gameData->font;
    s32 th = f->extCode >> 8;
    SprtWords *p = (SprtWords *)g_primPtr;
    u32 *ot = &g_ot[otz];
    s32 spacing;
    s32 c;
    u32 g;
    u32 w;
    s32 gx;
    s32 k;

    while ((c = *s) != TEXT_END) {
        if (c >= th) {
            c = (c << 8) + s[1] - f->extCode + f->extBase;
            s++;
        }
        g = *(f->glyphs + c);
        setlen(p, PRIM_LEN(SPRT));
        k = (g >> 20) & 3;
        p->rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
        w = g >> 28;
        spacing = 1;
        gx = x + k;
        x = x + spacing + w;
        p->xy = (gx & 0xFFFF) | (y << 16);
        p->uvclut = (g & 0x40FFFF) + (g_textPalettes[pal] << 16);
        p->wh = ((g >> 16) & 0xF) | 0xD0000;
        addPrim(ot, p);
        p++;
        s++;
    }
    setlen(p, PRIM_LEN(DR_TPAGE));
    ((DrTpageWords *)p)->code = get_mode(1, 1, 0x1E);
    addPrim(ot, p);
    g_primPtr = (u8 *)p + 8;
}

u8 *text_buildBlock(s32 x, s32 y, u32 *ot, u8 *s, s32 mode, u8 *pkt) {
    s32 w;
    s32 x0;
    u8 th;
    Font *font;
    s32 c;
    s32 k;

    font = g_gameState->gameData->font;
    g_textColor = mode;
    x0 = x;
    th = font->extCode >> 8;
    while ((c = *s) != TEXT_END) {
        if (c == TEXT_ESC) {
            s++;
            if (*s == TEXT_X_ADD) {
                s++;
                x += (s8)*s;
            } else if (*s == TEXT_X_SET) {
                s++;
                x = x0 + *s;
            } else if (*s == TEXT_Y_DOWN) {
                s++;
                y += *s;
            } else if (*s == TEXT_Y_UP) {
                s++;
                y -= *s;
            } else if (*s == TEXT_MSG_NUMBER) {
                s++;
                text_formatInt(g_msgNumbers[*s & 7], g_textNumberBuf, TEXT_NUM_CODES);
                pkt = text_buildPlain(x, y, ot, (Character *)g_textNumberBuf, &w, pkt);
                x += w;
            } else if (*s == TEXT_ARG_NUMBER) {
                s++;
                text_formatInt(g_textNumberArgs[*s], g_textNumberBuf, TEXT_NUM_CODES);
                pkt = text_buildPlain(x, y, ot, (Character *)g_textNumberBuf, &w, pkt);
                x += w;
            } else if (*s == TEXT_ARG_STRING) {
                s++;
                pkt = text_buildPlain(x, y, ot, (Character *)g_textStringArgs[*s], &w, pkt);
                x += w;
            } else if (*s == TEXT_ARG_NUMBER_SMALL) {
                s++;
                text_formatInt(g_textNumberArgs[*s], g_textNumberBuf, TEXT_NUM_ASCII);
                pkt = text_buildSmall(x, y, ot, g_textNumberBuf, g_textColor, pkt);
                x += text_measureSmall(g_textNumberBuf);
            } else if (*s == TEXT_ARG_STRING_SMALL) {
                s++;
                g_textArgPtr = g_textStringArgs[*s];
                pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                pkt = text_buildSmall(x, y, ot, g_textArgPtr, g_textColor, pkt);
                x += text_measureSmall(g_textArgPtr);
            } else if (*s == TEXT_ARG_NUMBER_LABEL) {
                s++;
                text_formatInt(g_textNumberArgs[*s], g_textNumberBuf, TEXT_NUM_ASCII);
                pkt = ui_buildLabel(x, y, ot, (s32)g_textNumberBuf, g_textColor, pkt);
                x += ui_measureLabel(g_textNumberBuf);
            } else if (*s == TEXT_ARG_STRING_LABEL) {
                s++;
                g_textArgPtr = g_textStringArgs[*s];
                pkt = ui_buildLabel(x, y, ot, (s32)g_textArgPtr, g_textColor, pkt);
                x += ui_measureLabel(g_textArgPtr);
            } else if (*s == TEXT_ARG_NUMBER_DIGITS) {
                s++;
                text_formatInt(g_textNumberArgs[*s], g_textNumberBuf, TEXT_NUM_ASCII);
                pkt = text_buildDigits(x, y, ot, g_textNumberBuf, g_textColor, pkt);
                x += text_measureDigits(g_textNumberBuf);
            } else if (*s == TEXT_ARG_STRING_DIGITS) {
                s++;
                g_textArgPtr = g_textStringArgs[*s];
                pkt = text_buildDigits(x, y, ot, g_textArgPtr, g_textColor, pkt);
                x += text_measureDigits(g_textArgPtr);
            } else if (*s == TEXT_SPRITE) {
                pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                pkt = ui_buildSprite(x, y, ot, *++s, 0, &w, pkt);
                x += w;
            } else if (*s == TEXT_ITEM_ICON) {
                s++;
                k = *s;
                if (k != 0xFF) {
                    pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                    pkt = ui_buildItemIcon(x, y, ot, k, 1, pkt);
                    x += g_uiSprites[g_itemTable[k].icon + 0x5C].w;
                }
            } else if (*s == TEXT_ARG_SPRITE) {
                pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                pkt = ui_buildSprite(x, y, ot, g_textIconArgs[*++s], 0, &w, pkt);
                x += w;
            } else if (*s == TEXT_ARG_ITEM_ICON) {
                s++;
                k = g_textIconArgs[*s];
                if (k != 0xFF) {
                    pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                    pkt = ui_buildItemIcon(x, y, ot, k, 1, pkt);
                    x += g_uiSprites[g_itemTable[k].icon + 0x5C].w;
                }
            } else {
                pkt = text_buildCode(x, y, ot, *s, &w, pkt);
                x += w;
            }
        } else if (c == TEXT_NEWLINE) {
            x = x0;
            y += 15;
        } else {
            if (c >= th) {
                c = (c << 8) + s[1] - font->extCode + font->extBase;
                s++;
                pkt = text_buildGlyph(x, y, ot, *(font->glyphs + c), g_textColor, &w, (SprtWords *)pkt);
                x += w;
            } else {
                pkt = text_buildGlyph(x, y, ot, *(font->glyphs + c), g_textColor, &w, (SprtWords *)pkt);
                x += w;
            }
        }
        s++;
    }
    setlen(pkt, PRIM_LEN(DR_TPAGE));
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, 0x1E);
    addPrim(ot, pkt);
    pkt += sizeof(DrTpageWords);
    return pkt;
}

u8 *text_buildPlain(s32 x, s32 y, u32 *ot, Character *ch, s32 *w, u8 *pkt) {
    s32 adv;
    s32 x0;
    u8 th;
    Font *font;
    Font *kanji;
    s32 th2;
    u8 *s;
    s32 c;
    s32 code;

    font = g_gameState->gameData->font;
    s = ch->name;
    x0 = x;
    th = font->extCode >> 8;
    kanji = g_gameState->gameData->kanjiFont;
    th2 = 0;
    if (kanji != NULL) {
        th2 = kanji->codeBase >> 8;
    }
    while ((c = *s) != TEXT_END) {
        if (kanji != NULL && c >= th2) {
            code = (c << 8) + s[1];
            s++;
            c = code - kanji->codeBase;
            pkt = text_buildGlyph(x, y, ot, *(kanji->glyphs + c), g_textColor, &adv, (SprtWords *)pkt);
            x += adv;
        } else if (c >= th) {
            code = (c << 8) + s[1];
            s++;
            c = code - font->extCode + font->extBase;
            pkt = text_buildGlyph(x, y, ot, *(font->glyphs + c), g_textColor, &adv, (SprtWords *)pkt);
            x += adv;
        } else {
            pkt = text_buildGlyph(x, y, ot, *(font->glyphs + c), g_textColor, &adv, (SprtWords *)pkt);
            x += adv;
        }
        s++;
    }
    setlen(pkt, PRIM_LEN(DR_TPAGE));
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, 0x1E);
    addPrim(ot, pkt);
    *w = x - x0;
    return pkt + 8;
}

u8 *ui_buildLabel(s32 x, s32 y, u32 *ot, s32 str, s32 mode, u8 *pkt) {
    u8 *s = (u8 *)str;
    SprtWords *p = (SprtWords *)pkt;
    SpriteDesc16 *g = NULL;
    SpriteDesc16 *tbl;
    u32 rgbc;
    u16 clut;
    s32 c;
    s32 w;

    rgbc = GPU_RGBC(GPU_SPRT, 0x404040);
    if (mode == UI_LABEL_NORMAL) {
        rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    }
    tbl = (SpriteDesc16 *)g_uiSprites;
    clut = tbl[46].clut;
    if (mode == UI_LABEL_MASK) {
        clut += 0x40;
        rgbc = GPU_CODE(GPU_SPRT | GPU_ABE);
    }
    if (s == NULL) {
        return (u8 *)p;
    }
    while ((c = *s) != 0) {
        if ((u32)(c - 'A') < 26) {
            g = &((SpriteDesc16 *)g_uiSprites)[*s - 8];
        } else if ((u32)(c - 'a') < 26) {
            g = &((SpriteDesc16 *)g_uiSprites)[*s - 0x28];
        } else if ((u32)(c - '0') < 10) {
            g = &((SpriteDesc16 *)g_uiSprites)[*s - 2];
        } else if ((u8)c == '&') {
            g = (SpriteDesc16 *)&g_uiSprites[UI_SPRITE_AMPERSAND];
        } else if ((u8)c == '.') {
            g = (SpriteDesc16 *)&g_uiSprites[UI_SPRITE_PERIOD];
        } else if ((u8)c == '_') {
            g = (SpriteDesc16 *)&g_uiSprites[UI_SPRITE_UNDERSCORE];
        }
        if (*s != ' ') {
            setlen(p, PRIM_LEN(SPRT));
            p->rgbc = rgbc;
            p->xy = (x & 0xFFFF) | (y << 16);
            w = g->w;
            p->uvclut = g->uv | (clut << 16);
            p->wh = g->w | (g->h << 16);
            addPrim(ot, p);
            p++;
        } else {
            w = 5;
        }
        s++;
        x += w - 1;
    }
    setlen(p, PRIM_LEN(DR_TPAGE));
    ((DrTpageWords *)p)->code = get_mode(1, 1, 0x1F);
    addPrim(ot, p);
    return (u8 *)p + 8;
}

u8 *text_buildSmall(s32 x, s32 y, u32 *ot, u8 *s, s32 pal, u8 *pkt) {
    Sprt8Fields *p = (Sprt8Fields *)pkt;
    u8 kana = 0;
    s32 hi = 0;
    s32 c;
    s32 code;
    s32 row;
    s32 u;
    s32 v;
    s32 k;
    Sprt8Fields *q;

    if (*s == 0x40) {
        kana = 1;
        s++;
    }
    while ((c = *s) != 0) {
        if ((u8)c == 0x40) {
            kana ^= 1;
            s++;
        } else if ((u8)c == 0x20) {
            x += 7;
            s++;
        } else {
            if ((u32)(c - 0x21) < 0x3F) {
                code = *s - 0x20;
                if (*s == 0x5C) {
                    code = *s - 0x4D;
                }
                goto calc;
            } else if ((u32)(c - 0x61) < 0x1A) {
                code = *s - 0x40;
            calc:
                row = code >> 5;
                hi = (row >= 3) << 6;
                u = (code & 0x1F) * 8;
                v = (row % 3) * 8;
            } else if ((u8)c >= 0xA0) {
                code = *s - 0xA0;
                k = (code >> 5) + 2;
                if (!kana) {
                    k = (code >> 5) + 4;
                }
                hi = (k >= 3) << 6;
                u = (code & 0x1F) * 8;
                v = (k % 3) * 8;
            } else {
                v = 0;
                u = 0;
            }
            setlen(p, PRIM_LEN(SPRT_8));
            p->rgbc = GPU_RGBC(GPU_SPRT_8, GPU_RGB_NEUTRAL);
            q = p;
            if ((u8)(*s + 0x22) < 2) {
                p->x = x - 7;
                p->y = y - 7;
            } else {
                p->x = x;
                x += 7;
                p->y = y;
            }
            q->u = u;
            q->v = v - 0x80;
            q->clut = g_textPalettes[pal] | hi;
            addPrim(ot, p);
            p++;
            s++;
        }
    }
    setlen(p, PRIM_LEN(DR_TPAGE));
    ((DrTpageWords *)p)->code = get_mode(1, 1, 0x1F);
    hi = getaddr(ot); /* reuses hi as the tag temp */
    setaddr(p, hi), setaddr(ot, p);
    return (u8 *)p + 8;
}

u8 *text_buildCode(s32 x, s32 y, u32 *ot, s32 code, s32 *w, u8 *pkt) {
    pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
    switch (code & 0xFF) {
    case TEXT_COLOR + TEXT_PAL_PLAIN:
        g_textColor = TEXT_PAL_PLAIN;
        *w = 0;
        break;
    case TEXT_COLOR + TEXT_PAL_RED:
        g_textColor = TEXT_PAL_RED;
        *w = 0;
        break;
    case TEXT_COLOR + TEXT_PAL_PURPLE:
        g_textColor = TEXT_PAL_PURPLE;
        *w = 0;
        break;
    case TEXT_COLOR + TEXT_PAL_GREEN:
        g_textColor = TEXT_PAL_GREEN;
        *w = 0;
        break;
    case TEXT_COLOR + TEXT_PAL_CYAN:
        g_textColor = TEXT_PAL_CYAN;
        *w = 0;
        break;
    case TEXT_COLOR + TEXT_PAL_YELLOW:
        g_textColor = TEXT_PAL_YELLOW;
        *w = 0;
        break;
    case TEXT_COLOR + TEXT_PAL_WHITE:
        g_textColor = TEXT_PAL_WHITE;
        *w = 0;
        break;
    case TEXT_COLOR + TEXT_PAL_GREY:
        g_textColor = TEXT_PAL_GREY;
        *w = 0;
        break;
    case TEXT_COLOR + TEXT_PAL_HELP:
        g_textColor = TEXT_PAL_HELP;
        *w = 0;
        break;
    case TEXT_COLOR + TEXT_PAL_HELP_RED:
        g_textColor = TEXT_PAL_HELP_RED;
        *w = 0;
        break;
    case TEXT_BTN_CIRCLE:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_CIRCLE, 0, w, pkt);
        break;
    case TEXT_BTN_CROSS:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_CROSS, 0, w, pkt);
        break;
    case TEXT_BTN_TRIANGLE:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_TRIANGLE, 0, w, pkt);
        break;
    case TEXT_BTN_SQUARE:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_SQUARE, 0, w, pkt);
        break;
    case TEXT_BTN_L1:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_L1, 0, w, pkt);
        break;
    case TEXT_BTN_R1:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_R1, 0, w, pkt);
        break;
    case TEXT_BTN_L2:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_L2, 0, w, pkt);
        break;
    case TEXT_BTN_R2:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_R2, 0, w, pkt);
        break;
    case TEXT_PAD_BTN + 0:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(1), 0, w, pkt);
        break;
    case TEXT_PAD_BTN + 1:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(0), 0, w, pkt);
        break;
    case TEXT_PAD_BTN + 2:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(2), 0, w, pkt);
        break;
    case TEXT_PAD_BTN + 3:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(3), 0, w, pkt);
        break;
    case TEXT_PAD_BTN + 4:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(4), 0, w, pkt);
        break;
    case TEXT_PAD_BTN + 5:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(5), 0, w, pkt);
        break;
    case TEXT_PAD_BTN + 6:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(6), 0, w, pkt);
        break;
    case TEXT_PAD_BTN + 7:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(7), 0, w, pkt);
        break;
    case TEXT_BTN_START:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_START, 0, w, pkt);
        break;
    case TEXT_BTN_SELECT:
        pkt = ui_buildSprite(x, y + 2, ot, UI_SPRITE_SELECT, 0, w, pkt);
        break;
    case TEXT_DPAD_UP:
        pkt = text_buildDpad(x, y + 2, ot, DPAD_ARROW_UP, 0, w, pkt);
        break;
    case TEXT_DPAD_DOWN:
        pkt = text_buildDpad(x, y + 2, ot, DPAD_ARROW_DOWN, 0, w, pkt);
        break;
    case TEXT_DPAD_LEFT:
        pkt = text_buildDpad(x, y + 2, ot, DPAD_ARROW_LEFT, 0, w, pkt);
        break;
    case TEXT_DPAD_RIGHT:
        pkt = text_buildDpad(x, y + 2, ot, DPAD_ARROW_RIGHT, 0, w, pkt);
        break;
    case TEXT_DPAD_UP_DOWN:
        pkt = text_buildDpad(x, y + 2, ot, DPAD_ARROW_UP | DPAD_ARROW_DOWN, 0, w, pkt);
        break;
    case TEXT_DPAD_LEFT_RIGHT:
        pkt = text_buildDpad(x, y + 2, ot, DPAD_ARROW_LEFT | DPAD_ARROW_RIGHT, 0, w, pkt);
        break;
    case TEXT_CHAR_NAME + 0:
        pkt = text_buildPlain(x, y, ot, &g_gameState->gameData->characters[0], w, pkt);
        break;
    case TEXT_CHAR_NAME + 1:
        pkt = text_buildPlain(x, y, ot, &g_gameState->gameData->characters[1], w, pkt);
        break;
    case TEXT_CHAR_NAME + 2:
        pkt = text_buildPlain(x, y, ot, &g_gameState->gameData->characters[2], w, pkt);
        break;
    case TEXT_CHAR_NAME + 3:
        pkt = text_buildPlain(x, y, ot, &g_gameState->gameData->characters[3], w, pkt);
        break;
    case TEXT_CHAR_NAME + 4:
        pkt = text_buildPlain(x, y, ot, &g_gameState->gameData->characters[4], w, pkt);
        break;
    case TEXT_CHAR_NAME + 5:
        pkt = text_buildPlain(x, y, ot, &g_gameState->gameData->characters[5], w, pkt);
        break;
    case TEXT_CHAR_NAME + 6:
        pkt = text_buildPlain(x, y, ot, &g_gameState->gameData->characters[6], w, pkt);
        break;
    case TEXT_CHAR_NAME + 7:
        pkt = text_buildPlain(x, y, ot, &g_gameState->gameData->characters[7], w, pkt);
        break;
    case TEXT_PARTY_NAME + 0:
        if (g_gameState->gameData->party[0] != NULL) {
            pkt = text_buildPlain(x, y, ot, g_gameState->gameData->party[0], w, pkt);
        }
        break;
    case TEXT_PARTY_NAME + 1:
        if (g_gameState->gameData->party[1] != NULL) {
            pkt = text_buildPlain(x, y, ot, g_gameState->gameData->party[1], w, pkt);
        }
        break;
    case TEXT_PARTY_NAME + 2:
        if (g_gameState->gameData->party[2] != NULL) {
            pkt = text_buildPlain(x, y, ot, g_gameState->gameData->party[2], w, pkt);
        }
        break;
    case TEXT_PARTY_NAME + 3:
        if (g_gameState->gameData->party[3] != NULL) {
            pkt = text_buildPlain(x, y, ot, g_gameState->gameData->party[3], w, pkt);
        }
        break;
    }
    return pkt;
}

s32 text_getCodeWidth(s32 c) {
    s32 r = 0;

    switch (c & 0xFF) {
    case TEXT_BTN_CIRCLE:
        r = g_uiSprites[UI_SPRITE_CIRCLE].w;
        break;
    case TEXT_BTN_CROSS:
        r = g_uiSprites[UI_SPRITE_CROSS].w;
        break;
    case TEXT_BTN_TRIANGLE:
        r = g_uiSprites[UI_SPRITE_TRIANGLE].w;
        break;
    case TEXT_BTN_SQUARE:
        r = g_uiSprites[UI_SPRITE_SQUARE].w;
        break;
    case TEXT_BTN_L1:
        r = g_uiSprites[UI_SPRITE_L1].w;
        break;
    case TEXT_BTN_R1:
        r = g_uiSprites[UI_SPRITE_R1].w;
        break;
    case TEXT_BTN_L2:
        r = g_uiSprites[UI_SPRITE_L2].w;
        break;
    case TEXT_BTN_R2:
        r = g_uiSprites[UI_SPRITE_R2].w;
        break;
    case TEXT_PAD_BTN + 0:
        r = g_uiSprites[text_getButtonSprite(0)].w;
        break;
    case TEXT_PAD_BTN + 1:
        r = g_uiSprites[text_getButtonSprite(1)].w;
        break;
    case TEXT_PAD_BTN + 2:
        r = g_uiSprites[text_getButtonSprite(2)].w;
        break;
    case TEXT_PAD_BTN + 3:
        r = g_uiSprites[text_getButtonSprite(3)].w;
        break;
    case TEXT_PAD_BTN + 4:
        r = g_uiSprites[text_getButtonSprite(4)].w;
        break;
    case TEXT_PAD_BTN + 5:
        r = g_uiSprites[text_getButtonSprite(5)].w;
        break;
    case TEXT_PAD_BTN + 6:
        r = g_uiSprites[text_getButtonSprite(6)].w;
        break;
    case TEXT_PAD_BTN + 7:
        r = g_uiSprites[text_getButtonSprite(7)].w;
        break;
    case TEXT_BTN_START:
        r = g_uiSprites[UI_SPRITE_START].w;
        break;
    case TEXT_BTN_SELECT:
        r = g_uiSprites[UI_SPRITE_SELECT].w;
        break;
    case TEXT_DPAD_UP:
    case TEXT_DPAD_DOWN:
    case TEXT_DPAD_LEFT:
    case TEXT_DPAD_RIGHT:
    case TEXT_DPAD_UP_DOWN:
    case TEXT_DPAD_LEFT_RIGHT:
        r = g_uiSprites[UI_SPRITE_DPAD].w;
        break;
    case TEXT_CHAR_NAME + 0:
        r = text_measureWidth((s32)&g_gameState->gameData->characters[0]);
        break;
    case TEXT_CHAR_NAME + 1:
        r = text_measureWidth((s32)&g_gameState->gameData->characters[1]);
        break;
    case TEXT_CHAR_NAME + 2:
        r = text_measureWidth((s32)&g_gameState->gameData->characters[2]);
        break;
    case TEXT_CHAR_NAME + 3:
        r = text_measureWidth((s32)&g_gameState->gameData->characters[3]);
        break;
    case TEXT_CHAR_NAME + 4:
        r = text_measureWidth((s32)&g_gameState->gameData->characters[4]);
        break;
    case TEXT_CHAR_NAME + 5:
        r = text_measureWidth((s32)&g_gameState->gameData->characters[5]);
        break;
    case TEXT_CHAR_NAME + 6:
        r = text_measureWidth((s32)&g_gameState->gameData->characters[6]);
        break;
    case TEXT_CHAR_NAME + 7:
        r = text_measureWidth((s32)&g_gameState->gameData->characters[7]);
        break;
    case TEXT_PARTY_NAME + 0:
        if (g_gameState->gameData->party[0] != NULL) {
            r = text_measureWidth((s32)g_gameState->gameData->party[0]);
        } else {
            r = 0;
        }
        break;
    case TEXT_PARTY_NAME + 1:
        if (g_gameState->gameData->party[1] != NULL) {
            r = text_measureWidth((s32)g_gameState->gameData->party[1]);
        } else {
            r = 0;
        }
        break;
    case TEXT_PARTY_NAME + 2:
        if (g_gameState->gameData->party[2] != NULL) {
            r = text_measureWidth((s32)g_gameState->gameData->party[2]);
        } else {
            r = 0;
        }
        break;
    case TEXT_PARTY_NAME + 3:
        if (g_gameState->gameData->party[3] != NULL) {
            r = text_measureWidth((s32)g_gameState->gameData->party[3]);
        } else {
            r = 0;
        }
        break;
    }
    return r;
}

s32 text_formatTime(s32 frames, u8 *out, s32 blink) {
    s32 r = 0;
    s32 sec;
    s32 min;
    s32 hr;
    u8 sub;

    sec = (u32)frames / 60;
    min = sec / 60;
    sec %= 60;
    hr = min / 60;
    min %= 60;
    if (hr >= 601) {
        hr = 599;
        sec = 59;
        min = 59;
    }
    if (hr >= 100) {
        r = g_hourHundredsGlyphs[hr / 100];
        hr %= 100;
    }
    if ((u8)blink) {
        sub = (u32)frames % 60;
    } else {
        sub = 0;
    }
    text_formatNumber_inline(hr, out, 2, TEXT_NUM_ASCII);
    text_formatNumber_inline(min, out + 3, 2, TEXT_NUM_ASCII);
    if (out[3] == ' ') {
        out[3] = '0';
    }
    text_formatNumber_inline(sec, out + 6, 2, TEXT_NUM_ASCII);
    if (out[6] == ' ') {
        out[6] = '0';
    }
    if ((u8)sub < 30) {
        out[2] = ':';
        out[5] = ':';
    } else {
        out[5] = ' ';
        out[2] = ' ';
    }
    if (hr < 10) {
        out[0] = '0';
    }
    return r;
}

u8 *text_append(u8 *dst, u8 *src) {
    s32 c;
    u8 th;
    u8 *p;
    u8 *start;
    u8 *s;
    s32 c2;
    u8 th2;

    start = dst;
    s = start;
    p = s;
    th = g_gameState->gameData->font->extCode >> 8;
    while ((c = *p) != TEXT_END) {
        p++;
        if (c == TEXT_ESC) {
            p++;
            if ((u32)(p[-2] - TEXT_X_ADD) < 2) {
                p++;
            }
        } else if (c >= th) {
            p++;
        }
    }
    dst += p - s;
    th2 = g_gameState->gameData->font->extCode >> 8;
    while ((c2 = *src) != TEXT_END) {
        *dst++ = *src++;
        if (c2 == TEXT_ESC) {
            *dst++ = *src++;
            if ((u32)(src[-2] - TEXT_X_ADD) < 2) {
                *dst++ = *src++;
            }
        } else if (c2 >= th2) {
            *dst++ = *src++;
        }
    }
    *dst = TEXT_END;
    return start;
}

s32 ui_measureLabel(u8 *s) {
    s32 w;
    Glyph8 *g;
    u8 c;
    s32 adv;

    w = 0;
    g = NULL;
    while ((c = *s) != 0) {
        if (c >= 'A' && c <= 'Z') {
            g = (Glyph8 *)&g_uiSprites[*s - 8];
        } else if (c >= 'a' && c <= 'z') {
            g = (Glyph8 *)&g_uiSprites[*s - 0x28];
        } else if (c >= '0' && c <= '9') {
            g = (Glyph8 *)&g_uiSprites[*s - 2];
        } else if (c == '&') {
            g = (Glyph8 *)&g_uiSprites[UI_SPRITE_AMPERSAND];
        } else if (c == '.') {
            g = (Glyph8 *)&g_uiSprites[UI_SPRITE_PERIOD];
        } else if (c == '_') {
            g = (Glyph8 *)&g_uiSprites[UI_SPRITE_UNDERSCORE];
        }
        if (*s == ' ') {
            adv = 5;
        } else {
            adv = g->advance;
        }
        s++;
        w += adv - 1;
    }
    return w;
}

TextSize text_measure(u8 *s) {
    TextSize r;
    s32 w;
    s32 maxw;
    s32 h;
    s32 c;
    s32 code;
    Font *font;
    Font *kanji;
    u8 th;
    u8 th2;
    s32 idx;
    Font *tbl;

    memset(&r, 0, 4);
    w = 0;
    maxw = 0;
    font = g_gameState->gameData->font;
    kanji = g_gameState->gameData->kanjiFont;
    h = 15;
    th2 = 0;
    if (s == NULL) {
        return r;
    }
    th = font->extCode >> 8;
    if (kanji != NULL) {
        th2 = kanji->codeBase >> 8;
    }
    while ((c = *s) != TEXT_END) {
        if (c == TEXT_ESC) {
            s++;
            if (*s == TEXT_X_ADD) {
                s++;
                w += (s8)*s;
            } else if (*s == TEXT_X_SET) {
                s++;
                w = *s;
            } else if (*s == TEXT_MSG_NUMBER) {
                s++;
                w += text_measureNumber(g_msgNumbers[*s], 0);
            } else if (*s == TEXT_ARG_NUMBER) {
                s++;
                w += text_measureNumber(g_textNumberArgs[*s], 0);
            } else if (*s == TEXT_ARG_STRING) {
                s++;
                w += g_textMeasureFuncs[0](g_textStringArgs[*s]);
            } else if (*s == TEXT_ARG_NUMBER_SMALL) {
                s++;
                w += text_measureNumber(g_textNumberArgs[*s], 1);
            } else if (*s == TEXT_ARG_STRING_SMALL) {
                s++;
                w += g_textMeasureFuncs[1](g_textStringArgs[*s]);
            } else if (*s == TEXT_ARG_NUMBER_LABEL) {
                s++;
                w += text_measureNumber(g_textNumberArgs[*s], 2);
            } else if (*s == TEXT_ARG_STRING_LABEL) {
                s++;
                w += g_textMeasureFuncs[2](g_textStringArgs[*s]);
            } else if (*s == TEXT_ARG_NUMBER_DIGITS) {
                s++;
                w += text_measureNumber(g_textNumberArgs[*s], 3);
            } else if (*s == TEXT_ARG_STRING_DIGITS) {
                s++;
                w += g_textMeasureFuncs[3](g_textStringArgs[*s]);
            } else if (*s == TEXT_SPRITE) {
                s++;
                w += g_uiSprites[*s].w;
            } else if (*s == TEXT_ITEM_ICON) {
                s++;
                if (*s == 0xFF) {
                    w += g_uiSprites[g_itemTable[0xFF].icon + 0x5C].w;
                }
            } else if (*s == TEXT_ARG_SPRITE) {
                s++;
                w += g_uiSprites[g_textIconArgs[*s]].w;
            } else if (*s == TEXT_ARG_ITEM_ICON) {
                s++;
                if (g_textIconArgs[*s] == 0xFF) {
                    w += g_uiSprites[g_itemTable[0xFF].icon + 0x5C].w;
                }
            } else if (*s == TEXT_Y_DOWN) {
                s++;
                h += *s;
            } else if (*s == TEXT_Y_UP) {
                s++;
                h -= *s;
            } else {
                w += text_getCodeWidth(*s);
            }
        } else if (c == TEXT_NEWLINE) {
            h += 15;
            if (maxw < w) {
                maxw = w;
            }
            w = 0;
        } else if (kanji != NULL && c >= th2) {
            code = (c << 8) + s[1];
            c = code - kanji->codeBase;
            s++;
            w += 1 + (*(kanji->glyphs + c) >> 28);
        } else if (c < th) {
            w += 1 + (*(font->glyphs + c) >> 28);
        } else {
            code = (c << 8) + s[1];
            c = code - font->extCode + font->extBase;
            s++;
            w += 1 + (*(font->glyphs + c) >> 28);
        }
        s++;
    }
    r.w = maxw > w ? maxw : w;
    r.h = h;
    return r;
}

s32 text_initNameImage(s32 work) {
    g_nameGlyphs = work;
    g_nameImage = work + 0x4000;
    memset(g_nameImage, 0, 0x3F0);
    return g_nameImage;
}

void text_renderPartyName(s32 slot) {
    s32 x = (slot & 1) * 0x54;
    s32 y = slot >> 1;
    s32 i;
    u8 *name = g_gameState->gameData->party[slot]->name;

    for (i = 0; i < 7; i++) {
        if (*name == 0xFF) {
            break;
        }
        x = text_renderNameGlyph(x, y, *name++);
    }
}

void ui_drawLabel(s32 x, s32 y, s32 otz, s32 str, s32 mode) {
    g_primPtr = ui_buildLabel(x, y, &g_ot[otz], str, mode, g_primPtr);
}

void ui_drawSmallDigits(s32 x, s32 y, s32 otz, u8 *s, s32 pal) {
    g_primPtr = text_buildSmall(x, y, &g_ot[otz], s, pal, g_primPtr);
}

void ui_drawDigits(s32 x, s32 y, s32 otz, u8 *s, s32 style) {
    g_primPtr = text_buildDigits(x, y, &g_ot[otz], s, style, g_primPtr);
}

void text_drawBlock(s32 x, s32 y, s32 otz, u8 *s, s32 mode) {
    g_primPtr = text_buildBlock(x, y, &g_ot[otz], s, mode, g_primPtr);
}

u8 *text_buildDigits(s32 x, s32 y, u32 *ot, u8 *s, s32 style, void *pkt) {
    SprtWords *p = pkt;
    DrTpageWords *t;
    u32 rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    s32 c;
    s32 u;

    if (style == DIGITS_DIM) {
        rgbc = GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060);
    }
    while ((c = *s) != 0) {
        if ((u8)c != ' ') {
            do { } while (0);
            if ((u8)c != '/') {
                u = 0x1E8;
                u = c * 8 - u;
            } else {
                u = 0xE8;
            }
            setlen(p, PRIM_LEN(SPRT));
            p->rgbc = rgbc;
            p->xy = (x & 0xFFFF) | (y << 16);
            p->wh = 0xA0008;
            p->uvclut = ((u & 0xFF) | 0x4000) | ((g_numberClutRows[style] + 0x3EE4) << 16);
            addPrim(ot, p);
            p++;
        }
        s++;
        x += 7;
    }
    t = (DrTpageWords *)p;
    setDrawTPage(t, 1, 1, 0x1F);
    addPrim(ot, t);
    return (u8 *)(t + 1);
}

u8 *text_buildGlyph(s32 x, s32 y, u32 *ot, u32 glyph, s32 mode, s32 *out, SprtWords *p) {
    u32 rgbc = GPU_CODE(GPU_SPRT | GPU_ABE);
    s32 dx = (glyph >> 20) & 3;
    s32 h = (glyph >> 16) & 0xF;
    s32 n = glyph >> 28;
    if (mode != TEXT_PAL_PLAIN) {
        rgbc = GPU_RGBC(GPU_SPRT, GPU_RGB_NEUTRAL);
    }
    setlen(p, PRIM_LEN(SPRT));
    p->xy = ((x + dx) & 0xFFFF) | (y << 16);
    p->rgbc = rgbc;
    p->uvclut = (glyph & 0x40FFFF) + (g_textPalettes[mode] << 16);
    p->wh = h | 0xD0000;
    addPrim(ot, p);
    if (out != NULL) {
        *out = n + 1;
    }
    return (u8 *)(p + 1);
}

u8 *text_buildDpad(s32 x, s32 y, u32 *ot, u32 dirMask, u8 flag, s32 *outWidth, u8 *pkt) {
    s32 arrowW[2];
    s32 i;
    for (i = 0; i < 4; i++) {
        if (dirMask & (1 << i)) {
            pkt = ui_buildSprite(x + g_dpadArrowSprites[i].x, y + g_dpadArrowSprites[i].y, ot, g_dpadArrowSprites[i].sprite, flag, arrowW, pkt);
        }
    }
    return ui_buildSprite(x, y, ot, UI_SPRITE_DPAD, flag, outWidth, pkt);
}

s32 text_measureStyle(u8 *s, s32 idx) {
    return g_textMeasureFuncs[idx](s);
}

s32 text_measureSmall(u8 *s) {
    s32 w = 0;
    u8 c;

    while ((c = *s) != 0) {
        if (c != 0x40 && c != 0xDE && c != 0xDF) {
            w += 7;
        }
        s++;
    }
    return w;
}

s32 text_measureDigits(u8 *s) {
    s32 w = 0;
    u8 c;

    while ((c = *s) != 0) {
        if (c != 0x40 && c != 0xDE && c != 0xDF) {
            w += 7;
        }
        s++;
    }
    return w;
}

s32 text_measureWidth(s32 s) {
    return text_measure((u8 *)s).w;
}

u8 *text_buildFontTpage(u32 *ot, DrTpageWords *p) {
    setDrawTPage(p, 1, 1, 0x1E);
    addPrim(ot, p);
    return (u8 *)(p + 1);
}

void text_formatNumber(s32 n, u8 *out, s32 width, u8 ascii) {
    s32 base = 0;
    s32 fill;
    s32 i;
    s32 q;
    s32 d;
    u8 *end;

    if (ascii) {
        base = '0';
    }
    fill = ' ';
    if (!ascii) {
        fill = 0xF;
    }
    if (width < 0) {
        text_formatInt(n, out, ascii);
        return;
    }
    memset(out, fill, width);
    for (i = 0; i < width; i++) {
        d = n % 10;
        q = n / 10;
        out[width - i - 1] = d + base;
        if (q == 0) {
            break;
        }
        n = q;
    }
    end = out + width;
    if (!ascii) {
        *end = 0xFF;
    } else {
        *end = 0;
    }
}

void text_formatInt(s32 v, u8 *out, s32 ascii) {
    u8 digits[16];
    s32 n = 0;
    s32 i;
    s32 base;
    s32 minus;
    s32 term;
    if (ascii == 0) {
        base = 0;
        minus = 0xB;
        term = 0xFF;
    } else {
        base = '0';
        minus = '-';
        term = 0;
    }
    if (v == 0) {
        *out++ = base;
    } else {
        if (v < 0) {
            *out++ = minus;
            v = -v;
        }
        while (v != 0) {
            digits[n++] = base + v % 10;
            v /= 10;
        }
        for (i = 0; i < n;) {
            *out++ = digits[n - ++i];
        }
    }
    *out = term;
}

s32 text_getLength(u8 *s) {
    u8 *start = s;
    u8 lim = g_gameState->gameData->font->extCode >> 8;
    s32 c;

    while ((c = *s) != TEXT_END) {
        s++;
        if (c == TEXT_ESC) {
            s++;
            if ((u32)(s[-2] - TEXT_X_ADD) < 2) {
                s++;
            }
        } else if (c >= lim) {
            s++;
        }
    }
    return s - start;
}

u8 *text_copy(u8 *dst, u8 *src) {
    u8 *start = dst;
    u8 lim = g_gameState->gameData->font->extCode >> 8;
    s32 c;

    while ((c = *src) != TEXT_END) {
        *dst++ = *src++;
        if (c == TEXT_ESC) {
            *dst++ = *src++;
            if ((u32)(src[-2] - TEXT_X_ADD) < 2) {
                *dst++ = *src++;
            }
        } else if (c >= lim) {
            *dst++ = *src++;
        }
    }
    *dst = TEXT_END;
    return start;
}

s32 text_getButtonSprite(s32 button) {
    return g_buttonSprites[config_remapButton(button)];
}

void text_setStringArg(s32 i, u8 *v) {
    g_textStringArgs[i] = v;
}

void text_setNumberArg(s32 i, s32 v) {
    g_textNumberArgs[i] = v;
}

void text_setIconArg(s32 i, s32 v) {
    g_textIconArgs[i] = v;
}

s32 text_getColor(void) {
    return g_textColor;
}

void text_setColor(s32 v) {
    g_textColor = v;
}

s32 text_measureNumber(s32 n, s32 idx) {
    s32 r = 0;
    s32 step = g_numberDigitWidths[idx];

    while (n != 0) {
        n /= 10;
        r += step;
    }
    return r - 1;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/text", g_nameGlyphSpans);
