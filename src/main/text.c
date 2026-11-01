#include "ff9.h"

s32 text_renderNameGlyph(s32 x, s32 hi, s32 ch) {
    u8 rowPixels[16];
    u8 w;
    u8 start;
    u8 width;
    u8 *dst;
    u8 *src;
    s32 row;
    s32 i;
    s32 j;
    u8 bits;
    u8 *pix;
    s32 px;
    u8 pixVal, old;
    u8 shifted;

    dst = (u8 *)g_nameImage;
    w = g_nameGlyphSpans[ch];
    start = w >> 4;
    width = (w & 0xF) + 1;
    src = (u8 *)g_nameGlyphs + (ch << 6);
    memset(rowPixels, 0, 0x10);
    for (row = 0; row < 12; row++) {
        for (i = 0; i < 4; i++) {
            bits = src[row * 4 + i];
            for (j = 0; j < 4; j++) {
                rowPixels[i * 4 + j] = bits & 3;
                bits >>= 2;
            }
        }
        for (i = 0; i < width; i++) {
            px = x + i;
            if (!(px & 1)) {
                pix = dst + (px >> 1);
                pixVal = rowPixels[start + i];
                old = *pix;
                if (hi) { *pix = old | (pixVal << 2); } else { *pix = old | pixVal; }
            } else {
                pixVal = rowPixels[start + i];
                px >>= 1;
                pix = dst + px;
                old = *pix;
                shifted = pixVal << 4; if (hi) { *pix = old | (pixVal << 6); } else { *pix = old | shifted; }
            }
        }
        dst += 0x54;
    }
    return width + x;
}

s32 text_drawChar(s32 x, s32 y, s32 otz, u8 ch, s32 mode) {
    u32 *tbl = g_gameState->gameData->font->glyphs;
    u32 glyph;
    s32 w;
    SprtWords *sprt;

    if (ch == 0xFF) {
        return 0;
    }
    glyph = tbl[ch];
    setlen(g_primPtr, 4);
    w = (glyph >> 16) & 0xF;
    sprt = (SprtWords *)g_primPtr;
    sprt->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    sprt->xy = (x + ((10 - w) >> 1)) | (y << 16);
    sprt->uvclut = (glyph & 0x40FFFF) | (g_textPalettes[mode] << 16);
    sprt->wh = w | 0xD0000;
    addPrim(&g_ot[otz], sprt);
    g_primPtr = (u8 *)(sprt + 1);
    return glyph >> 28;
}

void text_draw(s32 x, s32 y, s32 otz, u8 *str, s32 pal) {
    Font *font = g_gameState->gameData->font;
    s32 th = font->extCode >> 8;
    SprtWords *sprt = (SprtWords *)g_primPtr;
    u32 *ot = &g_ot[otz];
    s32 spacing;
    s32 ch;
    u32 glyph;
    u32 w;
    s32 gx;
    s32 k;

    while ((ch = *str) != TEXT_END) {
        if (ch >= th) {
            ch = (ch << 8) + str[1] - font->extCode + font->extBase;
            str++;
        }
        glyph = *(font->glyphs + ch);
        setlen(sprt, 4);
        k = (glyph >> 20) & 3;
        sprt->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
        w = glyph >> 28;
        spacing = 1;
        gx = x + k;
        x = x + spacing + w;
        sprt->xy = (gx & 0xFFFF) | (y << 16);
        sprt->uvclut = (glyph & 0x40FFFF) + (g_textPalettes[pal] << 16);
        sprt->wh = ((glyph >> 16) & 0xF) | 0xD0000;
        addPrim(ot, sprt);
        sprt++;
        str++;
    }
    setlen(sprt, 1);
    ((DrTpageWords *)sprt)->code = get_mode(1, 1, 0x1E);
    addPrim(ot, sprt);
    g_primPtr = (u8 *)sprt + 8;
}

u8 *text_buildBlock(s32 x, s32 y, u32 *ot, u8 *str, s32 mode, u8 *pkt) {
    s32 w;
    s32 x0;
    u8 th;
    Font *font;
    s32 ch;
    s32 k;

    font = g_gameState->gameData->font;
    g_textColor = mode;
    x0 = x;
    th = font->extCode >> 8;
    while ((ch = *str) != TEXT_END) {
        if (ch == TEXT_ESC) {
            str++;
            if (*str == TEXT_X_ADD) {
                str++;
                x += (s8)*str;
            } else if (*str == TEXT_X_SET) {
                str++;
                x = x0 + *str;
            } else if (*str == TEXT_Y_DOWN) {
                str++;
                y += *str;
            } else if (*str == TEXT_Y_UP) {
                str++;
                y -= *str;
            } else if (*str == TEXT_MSG_NUMBER) {
                str++;
                text_formatInt(g_msgNumbers[*str & 7], g_textNumberBuf, TEXT_NUM_CODES);
                pkt = text_buildPlain(x, y, ot, (Character *)g_textNumberBuf, &w, pkt);
                x += w;
            } else if (*str == TEXT_ARG_NUMBER) {
                str++;
                text_formatInt(g_textNumberArgs[*str], g_textNumberBuf, TEXT_NUM_CODES);
                pkt = text_buildPlain(x, y, ot, (Character *)g_textNumberBuf, &w, pkt);
                x += w;
            } else if (*str == TEXT_ARG_STRING) {
                str++;
                pkt = text_buildPlain(x, y, ot, (Character *)g_textStringArgs[*str], &w, pkt);
                x += w;
            } else if (*str == TEXT_ARG_NUMBER_SMALL) {
                str++;
                text_formatInt(g_textNumberArgs[*str], g_textNumberBuf, TEXT_NUM_ASCII);
                pkt = text_buildSmall(x, y, ot, g_textNumberBuf, g_textColor, pkt);
                x += text_measureSmall(g_textNumberBuf);
            } else if (*str == TEXT_ARG_STRING_SMALL) {
                str++;
                g_textArgPtr = g_textStringArgs[*str];
                pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                pkt = text_buildSmall(x, y, ot, g_textArgPtr, g_textColor, pkt);
                x += text_measureSmall(g_textArgPtr);
            } else if (*str == TEXT_ARG_NUMBER_LABEL) {
                str++;
                text_formatInt(g_textNumberArgs[*str], g_textNumberBuf, TEXT_NUM_ASCII);
                pkt = ui_buildLabel(x, y, ot, (s32)g_textNumberBuf, g_textColor, pkt);
                x += ui_measureLabel(g_textNumberBuf);
            } else if (*str == TEXT_ARG_STRING_LABEL) {
                str++;
                g_textArgPtr = g_textStringArgs[*str];
                pkt = ui_buildLabel(x, y, ot, (s32)g_textArgPtr, g_textColor, pkt);
                x += ui_measureLabel(g_textArgPtr);
            } else if (*str == TEXT_ARG_NUMBER_DIGITS) {
                str++;
                text_formatInt(g_textNumberArgs[*str], g_textNumberBuf, TEXT_NUM_ASCII);
                pkt = text_buildDigits(x, y, ot, g_textNumberBuf, g_textColor, pkt);
                x += text_measureDigits(g_textNumberBuf);
            } else if (*str == TEXT_ARG_STRING_DIGITS) {
                str++;
                g_textArgPtr = g_textStringArgs[*str];
                pkt = text_buildDigits(x, y, ot, g_textArgPtr, g_textColor, pkt);
                x += text_measureDigits(g_textArgPtr);
            } else if (*str == TEXT_SPRITE) {
                pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                pkt = ui_buildSprite(x, y, ot, *++str, 0, &w, pkt);
                x += w;
            } else if (*str == TEXT_ITEM_ICON) {
                str++;
                k = *str;
                if (k != 0xFF) {
                    pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                    pkt = ui_buildItemIcon(x, y, ot, k, 1, pkt);
                    x += g_uiSprites[g_itemTable[k].icon + 0x5C].w;
                }
            } else if (*str == TEXT_ARG_SPRITE) {
                pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                pkt = ui_buildSprite(x, y, ot, g_textIconArgs[*++str], 0, &w, pkt);
                x += w;
            } else if (*str == TEXT_ARG_ITEM_ICON) {
                str++;
                k = g_textIconArgs[*str];
                if (k != 0xFF) {
                    pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
                    pkt = ui_buildItemIcon(x, y, ot, k, 1, pkt);
                    x += g_uiSprites[g_itemTable[k].icon + 0x5C].w;
                }
            } else {
                pkt = text_buildCode(x, y, ot, *str, &w, pkt);
                x += w;
            }
        } else if (ch == TEXT_NEWLINE) {
            x = x0;
            y += 15;
        } else {
            if (ch >= th) {
                ch = (ch << 8) + str[1] - font->extCode + font->extBase;
                str++;
                pkt = text_buildGlyph(x, y, ot, *(font->glyphs + ch), g_textColor, &w, (SprtWords *)pkt);
                x += w;
            } else {
                pkt = text_buildGlyph(x, y, ot, *(font->glyphs + ch), g_textColor, &w, (SprtWords *)pkt);
                x += w;
            }
        }
        str++;
    }
    setlen(pkt, 1);
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
    u8 *str;
    s32 charCode;
    s32 code;

    font = g_gameState->gameData->font;
    str = ch->name;
    x0 = x;
    th = font->extCode >> 8;
    kanji = g_gameState->gameData->kanjiFont;
    th2 = 0;
    if (kanji != NULL) {
        th2 = kanji->codeBase >> 8;
    }
    while ((charCode = *str) != TEXT_END) {
        if (kanji != NULL && charCode >= th2) {
            code = (charCode << 8) + str[1];
            str++;
            charCode = code - kanji->codeBase;
            pkt = text_buildGlyph(x, y, ot, *(kanji->glyphs + charCode), g_textColor, &adv, (SprtWords *)pkt);
            x += adv;
        } else if (charCode >= th) {
            code = (charCode << 8) + str[1];
            str++;
            charCode = code - font->extCode + font->extBase;
            pkt = text_buildGlyph(x, y, ot, *(font->glyphs + charCode), g_textColor, &adv, (SprtWords *)pkt);
            x += adv;
        } else {
            pkt = text_buildGlyph(x, y, ot, *(font->glyphs + charCode), g_textColor, &adv, (SprtWords *)pkt);
            x += adv;
        }
        str++;
    }
    setlen(pkt, 1);
    ((DrTpageWords *)pkt)->code = get_mode(1, 1, 0x1E);
    addPrim(ot, pkt);
    *w = x - x0;
    return pkt + 8;
}

u8 *ui_buildLabel(s32 x, s32 y, u32 *ot, s32 str, s32 mode, u8 *pkt) {
    u8 *cur = (u8 *)str;
    SprtWords *sprt = (SprtWords *)pkt;
    SpriteDesc16 *glyph = NULL;
    SpriteDesc16 *tbl;
    u32 rgbc;
    u16 clut;
    s32 ch;
    s32 w;

    rgbc = GPU_RGBC(GPU_SPRT, 0x404040);
    if (mode == UI_LABEL_NORMAL) {
        rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    }
    tbl = (SpriteDesc16 *)g_uiSprites;
    clut = tbl[46].clut;
    if (mode == UI_LABEL_MASK) {
        clut += 0x40;
        rgbc = GPU_CODE(GPU_SPRT | GPU_ABE);
    }
    if (cur == NULL) {
        return (u8 *)sprt;
    }
    while ((ch = *cur) != 0) {
        if ((u32)(ch - 'A') < 26) {
            glyph = &((SpriteDesc16 *)g_uiSprites)[*cur - 8];
        } else if ((u32)(ch - 'a') < 26) {
            glyph = &((SpriteDesc16 *)g_uiSprites)[*cur - 0x28];
        } else if ((u32)(ch - '0') < 10) {
            glyph = &((SpriteDesc16 *)g_uiSprites)[*cur - 2];
        } else if ((u8)ch == '&') {
            glyph = (SpriteDesc16 *)&g_uiSprites[UI_SPRITE_AMPERSAND];
        } else if ((u8)ch == '.') {
            glyph = (SpriteDesc16 *)&g_uiSprites[UI_SPRITE_PERIOD];
        } else if ((u8)ch == '_') {
            glyph = (SpriteDesc16 *)&g_uiSprites[UI_SPRITE_UNDERSCORE];
        }
        if (*cur != ' ') {
            setlen(sprt, 4);
            sprt->rgbc = rgbc;
            sprt->xy = (x & 0xFFFF) | (y << 16);
            w = glyph->w;
            sprt->uvclut = glyph->uv | (clut << 16);
            sprt->wh = glyph->w | (glyph->h << 16);
            addPrim(ot, sprt);
            sprt++;
        } else {
            w = 5;
        }
        cur++;
        x += w - 1;
    }
    setlen(sprt, 1);
    ((DrTpageWords *)sprt)->code = get_mode(1, 1, 0x1F);
    addPrim(ot, sprt);
    return (u8 *)sprt + 8;
}

u8 *text_buildSmall(s32 x, s32 y, u32 *ot, u8 *str, s32 pal, u8 *pkt) {
    Sprt8Fields *sprt = (Sprt8Fields *)pkt;
    u8 kana = 0;
    s32 hi = 0;
    s32 ch;
    s32 code;
    s32 row;
    s32 u;
    s32 v;
    s32 k;
    Sprt8Fields *sprtAlias;

    if (*str == 0x40) {
        kana = 1;
        str++;
    }
    while ((ch = *str) != 0) {
        if ((u8)ch == 0x40) {
            kana ^= 1;
            str++;
        } else if ((u8)ch == 0x20) {
            x += 7;
            str++;
        } else {
            if ((u32)(ch - 0x21) < 0x3F) {
                code = *str - 0x20;
                if (*str == 0x5C) {
                    code = *str - 0x4D;
                }
                goto calc;
            } else if ((u32)(ch - 0x61) < 0x1A) {
                code = *str - 0x40;
            calc:
                row = code >> 5;
                hi = (row >= 3) << 6;
                u = (code & 0x1F) * 8;
                v = (row % 3) * 8;
            } else if ((u8)ch >= 0xA0) {
                code = *str - 0xA0;
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
            setlen(sprt, 3);
            sprt->rgbc = GPU_RGBC(GPU_SPRT_8, 0x808080);
            sprtAlias = sprt;
            if ((u8)(*str + 0x22) < 2) {
                sprt->x = x - 7;
                sprt->y = y - 7;
            } else {
                sprt->x = x;
                x += 7;
                sprt->y = y;
            }
            sprtAlias->u = u;
            sprtAlias->v = v - 0x80;
            sprtAlias->clut = g_textPalettes[pal] | hi;
            addPrim(ot, sprt);
            sprt++;
            str++;
        }
    }
    setlen(sprt, 1);
    ((DrTpageWords *)sprt)->code = get_mode(1, 1, 0x1F);
    hi = getaddr(ot); /* reuses hi as the tag temp */
    setaddr(sprt, hi), setaddr(ot, sprt);
    return (u8 *)sprt + 8;
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

s32 text_getCodeWidth(s32 code) {
    s32 width = 0;

    switch (code & 0xFF) {
    case TEXT_BTN_CIRCLE:
        width = g_uiSprites[UI_SPRITE_CIRCLE].w;
        break;
    case TEXT_BTN_CROSS:
        width = g_uiSprites[UI_SPRITE_CROSS].w;
        break;
    case TEXT_BTN_TRIANGLE:
        width = g_uiSprites[UI_SPRITE_TRIANGLE].w;
        break;
    case TEXT_BTN_SQUARE:
        width = g_uiSprites[UI_SPRITE_SQUARE].w;
        break;
    case TEXT_BTN_L1:
        width = g_uiSprites[UI_SPRITE_L1].w;
        break;
    case TEXT_BTN_R1:
        width = g_uiSprites[UI_SPRITE_R1].w;
        break;
    case TEXT_BTN_L2:
        width = g_uiSprites[UI_SPRITE_L2].w;
        break;
    case TEXT_BTN_R2:
        width = g_uiSprites[UI_SPRITE_R2].w;
        break;
    case TEXT_PAD_BTN + 0:
        width = g_uiSprites[text_getButtonSprite(0)].w;
        break;
    case TEXT_PAD_BTN + 1:
        width = g_uiSprites[text_getButtonSprite(1)].w;
        break;
    case TEXT_PAD_BTN + 2:
        width = g_uiSprites[text_getButtonSprite(2)].w;
        break;
    case TEXT_PAD_BTN + 3:
        width = g_uiSprites[text_getButtonSprite(3)].w;
        break;
    case TEXT_PAD_BTN + 4:
        width = g_uiSprites[text_getButtonSprite(4)].w;
        break;
    case TEXT_PAD_BTN + 5:
        width = g_uiSprites[text_getButtonSprite(5)].w;
        break;
    case TEXT_PAD_BTN + 6:
        width = g_uiSprites[text_getButtonSprite(6)].w;
        break;
    case TEXT_PAD_BTN + 7:
        width = g_uiSprites[text_getButtonSprite(7)].w;
        break;
    case TEXT_BTN_START:
        width = g_uiSprites[UI_SPRITE_START].w;
        break;
    case TEXT_BTN_SELECT:
        width = g_uiSprites[UI_SPRITE_SELECT].w;
        break;
    case TEXT_DPAD_UP:
    case TEXT_DPAD_DOWN:
    case TEXT_DPAD_LEFT:
    case TEXT_DPAD_RIGHT:
    case TEXT_DPAD_UP_DOWN:
    case TEXT_DPAD_LEFT_RIGHT:
        width = g_uiSprites[UI_SPRITE_DPAD].w;
        break;
    case TEXT_CHAR_NAME + 0:
        width = text_measureWidth((s32)&g_gameState->gameData->characters[0]);
        break;
    case TEXT_CHAR_NAME + 1:
        width = text_measureWidth((s32)&g_gameState->gameData->characters[1]);
        break;
    case TEXT_CHAR_NAME + 2:
        width = text_measureWidth((s32)&g_gameState->gameData->characters[2]);
        break;
    case TEXT_CHAR_NAME + 3:
        width = text_measureWidth((s32)&g_gameState->gameData->characters[3]);
        break;
    case TEXT_CHAR_NAME + 4:
        width = text_measureWidth((s32)&g_gameState->gameData->characters[4]);
        break;
    case TEXT_CHAR_NAME + 5:
        width = text_measureWidth((s32)&g_gameState->gameData->characters[5]);
        break;
    case TEXT_CHAR_NAME + 6:
        width = text_measureWidth((s32)&g_gameState->gameData->characters[6]);
        break;
    case TEXT_CHAR_NAME + 7:
        width = text_measureWidth((s32)&g_gameState->gameData->characters[7]);
        break;
    case TEXT_PARTY_NAME + 0:
        if (g_gameState->gameData->party[0] != NULL) {
            width = text_measureWidth((s32)g_gameState->gameData->party[0]);
        } else {
            width = 0;
        }
        break;
    case TEXT_PARTY_NAME + 1:
        if (g_gameState->gameData->party[1] != NULL) {
            width = text_measureWidth((s32)g_gameState->gameData->party[1]);
        } else {
            width = 0;
        }
        break;
    case TEXT_PARTY_NAME + 2:
        if (g_gameState->gameData->party[2] != NULL) {
            width = text_measureWidth((s32)g_gameState->gameData->party[2]);
        } else {
            width = 0;
        }
        break;
    case TEXT_PARTY_NAME + 3:
        if (g_gameState->gameData->party[3] != NULL) {
            width = text_measureWidth((s32)g_gameState->gameData->party[3]);
        } else {
            width = 0;
        }
        break;
    }
    return width;
}

s32 text_formatTime(s32 frames, u8 *out, s32 blink) {
    s32 hundredsGlyph = 0;
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
        hundredsGlyph = g_hourHundredsGlyphs[hr / 100];
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
    return hundredsGlyph;
}

u8 *text_append(u8 *dst, u8 *src) {
    s32 ch;
    u8 th;
    u8 *p;
    u8 *start;
    u8 *scanStart;
    s32 c2;
    u8 th2;

    start = dst;
    scanStart = start;
    p = scanStart;
    th = g_gameState->gameData->font->extCode >> 8;
    while ((ch = *p) != TEXT_END) {
        p++;
        if (ch == TEXT_ESC) {
            p++;
            if ((u32)(p[-2] - TEXT_X_ADD) < 2) {
                p++;
            }
        } else if (ch >= th) {
            p++;
        }
    }
    dst += p - scanStart;
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

s32 ui_measureLabel(u8 *str) {
    s32 w;
    Glyph8 *glyph;
    u8 ch;
    s32 adv;

    w = 0;
    glyph = NULL;
    while ((ch = *str) != 0) {
        if (ch >= 'A' && ch <= 'Z') {
            glyph = (Glyph8 *)&g_uiSprites[*str - 8];
        } else if (ch >= 'a' && ch <= 'z') {
            glyph = (Glyph8 *)&g_uiSprites[*str - 0x28];
        } else if (ch >= '0' && ch <= '9') {
            glyph = (Glyph8 *)&g_uiSprites[*str - 2];
        } else if (ch == '&') {
            glyph = (Glyph8 *)&g_uiSprites[UI_SPRITE_AMPERSAND];
        } else if (ch == '.') {
            glyph = (Glyph8 *)&g_uiSprites[UI_SPRITE_PERIOD];
        } else if (ch == '_') {
            glyph = (Glyph8 *)&g_uiSprites[UI_SPRITE_UNDERSCORE];
        }
        if (*str == ' ') {
            adv = 5;
        } else {
            adv = glyph->advance;
        }
        str++;
        w += adv - 1;
    }
    return w;
}

TextSize text_measure(u8 *str) {
    TextSize size;
    s32 w;
    s32 maxw;
    s32 h;
    s32 ch;
    s32 code;
    Font *font;
    Font *kanji;
    u8 th;
    u8 th2;
    s32 idx;
    Font *tbl;

    memset(&size, 0, 4);
    w = 0;
    maxw = 0;
    font = g_gameState->gameData->font;
    kanji = g_gameState->gameData->kanjiFont;
    h = 15;
    th2 = 0;
    if (str == NULL) {
        return size;
    }
    th = font->extCode >> 8;
    if (kanji != NULL) {
        th2 = kanji->codeBase >> 8;
    }
    while ((ch = *str) != TEXT_END) {
        if (ch == TEXT_ESC) {
            str++;
            if (*str == TEXT_X_ADD) {
                str++;
                w += (s8)*str;
            } else if (*str == TEXT_X_SET) {
                str++;
                w = *str;
            } else if (*str == TEXT_MSG_NUMBER) {
                str++;
                w += text_measureNumber(g_msgNumbers[*str], 0);
            } else if (*str == TEXT_ARG_NUMBER) {
                str++;
                w += text_measureNumber(g_textNumberArgs[*str], 0);
            } else if (*str == TEXT_ARG_STRING) {
                str++;
                w += g_textMeasureFuncs[0](g_textStringArgs[*str]);
            } else if (*str == TEXT_ARG_NUMBER_SMALL) {
                str++;
                w += text_measureNumber(g_textNumberArgs[*str], 1);
            } else if (*str == TEXT_ARG_STRING_SMALL) {
                str++;
                w += g_textMeasureFuncs[1](g_textStringArgs[*str]);
            } else if (*str == TEXT_ARG_NUMBER_LABEL) {
                str++;
                w += text_measureNumber(g_textNumberArgs[*str], 2);
            } else if (*str == TEXT_ARG_STRING_LABEL) {
                str++;
                w += g_textMeasureFuncs[2](g_textStringArgs[*str]);
            } else if (*str == TEXT_ARG_NUMBER_DIGITS) {
                str++;
                w += text_measureNumber(g_textNumberArgs[*str], 3);
            } else if (*str == TEXT_ARG_STRING_DIGITS) {
                str++;
                w += g_textMeasureFuncs[3](g_textStringArgs[*str]);
            } else if (*str == TEXT_SPRITE) {
                str++;
                w += g_uiSprites[*str].w;
            } else if (*str == TEXT_ITEM_ICON) {
                str++;
                if (*str == 0xFF) {
                    w += g_uiSprites[g_itemTable[0xFF].icon + 0x5C].w;
                }
            } else if (*str == TEXT_ARG_SPRITE) {
                str++;
                w += g_uiSprites[g_textIconArgs[*str]].w;
            } else if (*str == TEXT_ARG_ITEM_ICON) {
                str++;
                if (g_textIconArgs[*str] == 0xFF) {
                    w += g_uiSprites[g_itemTable[0xFF].icon + 0x5C].w;
                }
            } else if (*str == TEXT_Y_DOWN) {
                str++;
                h += *str;
            } else if (*str == TEXT_Y_UP) {
                str++;
                h -= *str;
            } else {
                w += text_getCodeWidth(*str);
            }
        } else if (ch == TEXT_NEWLINE) {
            h += 15;
            if (maxw < w) {
                maxw = w;
            }
            w = 0;
        } else if (kanji != NULL && ch >= th2) {
            code = (ch << 8) + str[1];
            ch = code - kanji->codeBase;
            str++;
            w += 1 + (*(kanji->glyphs + ch) >> 28);
        } else if (ch < th) {
            w += 1 + (*(font->glyphs + ch) >> 28);
        } else {
            code = (ch << 8) + str[1];
            ch = code - font->extCode + font->extBase;
            str++;
            w += 1 + (*(font->glyphs + ch) >> 28);
        }
        str++;
    }
    size.w = maxw > w ? maxw : w;
    size.h = h;
    return size;
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

void ui_drawSmallDigits(s32 x, s32 y, s32 otz, u8 *str, s32 pal) {
    g_primPtr = text_buildSmall(x, y, &g_ot[otz], str, pal, g_primPtr);
}

void ui_drawDigits(s32 x, s32 y, s32 otz, u8 *str, s32 style) {
    g_primPtr = text_buildDigits(x, y, &g_ot[otz], str, style, g_primPtr);
}

void text_drawBlock(s32 x, s32 y, s32 otz, u8 *str, s32 mode) {
    g_primPtr = text_buildBlock(x, y, &g_ot[otz], str, mode, g_primPtr);
}

u8 *text_buildDigits(s32 x, s32 y, u32 *ot, u8 *str, s32 style, void *pkt) {
    SprtWords *sprt = pkt;
    DrTpageWords *tpage;
    u32 rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    s32 ch;
    s32 u;

    if (style == DIGITS_DIM) {
        rgbc = GPU_RGBC(GPU_SPRT | GPU_ABE, 0x606060);
    }
    while ((ch = *str) != 0) {
        if ((u8)ch != ' ') {
            do { } while (0);
            if ((u8)ch != '/') {
                u = 0x1E8;
                u = ch * 8 - u;
            } else {
                u = 0xE8;
            }
            setlen(sprt, 4);
            sprt->rgbc = rgbc;
            sprt->xy = (x & 0xFFFF) | (y << 16);
            sprt->wh = 0xA0008;
            sprt->uvclut = ((u & 0xFF) | 0x4000) | ((g_numberClutRows[style] + 0x3EE4) << 16);
            addPrim(ot, sprt);
            sprt++;
        }
        str++;
        x += 7;
    }
    tpage = (DrTpageWords *)sprt;
    setlen(tpage, 1);
    tpage->code = get_mode(1, 1, 0x1F);
    addPrim(ot, tpage);
    return (u8 *)(tpage + 1);
}

u8 *text_buildGlyph(s32 x, s32 y, u32 *ot, u32 glyph, s32 mode, s32 *out, SprtWords *sprt) {
    u32 rgbc = GPU_CODE(GPU_SPRT | GPU_ABE);
    s32 dx = (glyph >> 20) & 3;
    s32 h = (glyph >> 16) & 0xF;
    s32 advance = glyph >> 28;
    if (mode != TEXT_PAL_PLAIN) {
        rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    }
    setlen(sprt, 4);
    sprt->xy = ((x + dx) & 0xFFFF) | (y << 16);
    sprt->rgbc = rgbc;
    sprt->uvclut = (glyph & 0x40FFFF) + (g_textPalettes[mode] << 16);
    sprt->wh = h | 0xD0000;
    addPrim(ot, sprt);
    if (out != NULL) {
        *out = advance + 1;
    }
    return (u8 *)(sprt + 1);
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

s32 text_measureStyle(u8 *str, s32 idx) {
    return g_textMeasureFuncs[idx](str);
}

s32 text_measureSmall(u8 *str) {
    s32 w = 0;
    u8 ch;

    while ((ch = *str) != 0) {
        if (ch != 0x40 && ch != 0xDE && ch != 0xDF) {
            w += 7;
        }
        str++;
    }
    return w;
}

s32 text_measureDigits(u8 *str) {
    s32 w = 0;
    u8 ch;

    while ((ch = *str) != 0) {
        if (ch != 0x40 && ch != 0xDE && ch != 0xDF) {
            w += 7;
        }
        str++;
    }
    return w;
}

s32 text_measureWidth(s32 str) {
    return text_measure((u8 *)str).w;
}

u8 *text_buildFontTpage(u32 *ot, DrTpageWords *tpage) {
    setlen(tpage, 1);
    tpage->code = get_mode(1, 1, 0x1E);
    addPrim(ot, tpage);
    return (u8 *)(tpage + 1);
}

/* Same logic as text_formatNumber_inline (ff9.h). Written out because a call of the inline saves s0 later in the
   prologue. */
void text_formatNumber(s32 value, u8 *out, s32 width, u8 ascii) {
    s32 base = 0;
    s32 fill;
    s32 i;
    s32 quot;
    s32 digit;
    u8 *end;

    if (ascii) {
        base = '0';
    }
    fill = ' ';
    if (!ascii) {
        fill = 0xF;
    }
    if (width < 0) {
        text_formatInt(value, out, ascii);
        return;
    }
    memset(out, fill, width);
    for (i = 0; i < width; i++) {
        digit = value % 10;
        quot = value / 10;
        out[width - i - 1] = digit + base;
        if (quot == 0) {
            break;
        }
        value = quot;
    }
    end = out + width;
    if (!ascii) {
        *end = 0xFF;
    } else {
        *end = 0;
    }
}

void text_formatInt(s32 value, u8 *out, s32 ascii) {
    u8 digits[16];
    s32 digitCount = 0;
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
    if (value == 0) {
        *out++ = base;
    } else {
        if (value < 0) {
            *out++ = minus;
            value = -value;
        }
        while (value != 0) {
            digits[digitCount++] = base + value % 10;
            value /= 10;
        }
        for (i = 0; i < digitCount;) {
            *out++ = digits[digitCount - ++i];
        }
    }
    *out = term;
}

s32 text_getLength(u8 *str) {
    u8 *start = str;
    u8 lim = g_gameState->gameData->font->extCode >> 8;
    s32 ch;

    while ((ch = *str) != TEXT_END) {
        str++;
        if (ch == TEXT_ESC) {
            str++;
            if ((u32)(str[-2] - TEXT_X_ADD) < 2) {
                str++;
            }
        } else if (ch >= lim) {
            str++;
        }
    }
    return str - start;
}

u8 *text_copy(u8 *dst, u8 *src) {
    u8 *start = dst;
    u8 lim = g_gameState->gameData->font->extCode >> 8;
    s32 ch;

    while ((ch = *src) != TEXT_END) {
        *dst++ = *src++;
        if (ch == TEXT_ESC) {
            *dst++ = *src++;
            if ((u32)(src[-2] - TEXT_X_ADD) < 2) {
                *dst++ = *src++;
            }
        } else if (ch >= lim) {
            *dst++ = *src++;
        }
    }
    *dst = TEXT_END;
    return start;
}

s32 text_getButtonSprite(s32 button) {
    return g_buttonSprites[config_remapButton(button)];
}

void text_setStringArg(s32 i, u8 *str) {
    g_textStringArgs[i] = str;
}

void text_setNumberArg(s32 i, s32 value) {
    g_textNumberArgs[i] = value;
}

void text_setIconArg(s32 i, s32 icon) {
    g_textIconArgs[i] = icon;
}

s32 text_getColor(void) {
    return g_textColor;
}

void text_setColor(s32 color) {
    g_textColor = color;
}

s32 text_measureNumber(s32 value, s32 idx) {
    s32 width = 0;
    s32 step = g_numberDigitWidths[idx];

    while (value != 0) {
        value /= 10;
        width += step;
    }
    return width - 1;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/text", g_nameGlyphSpans);
