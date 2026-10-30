#include "ff9.h"

s32 text_renderNameGlyph(s32 x, s32 hi, s32 c) {
    u8 buf[16];
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
    w = D_80010BD0[c];
    start = w >> 4;
    width = (w & 0xF) + 1;
    src = (u8 *)g_nameGlyphs + (c << 6);
    memset(buf, 0, 0x10);
    for (row = 0; row < 12; row++) {
        for (i = 0; i < 4; i++) {
            b = src[row * 4 + i];
            for (j = 0; j < 4; j++) {
                buf[i * 4 + j] = b & 3;
                b >>= 2;
            }
        }
        for (i = 0; i < width; i++) {
            px = x + i;
            if (!(px & 1)) {
                p = dst + (px >> 1);
                v = buf[start + i];
                o = *p;
                if (hi) { *p = o | (v << 2); } else { *p = o | v; }
            } else {
                v = buf[start + i];
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
    u32 *tbl = g_gameState->gameData->unk7D4->unkC;
    u32 g;
    s32 w;
    SprtWords *p;

    if (c == 0xFF) {
        return 0;
    }
    g = tbl[c];
    setlen(g_primPtr, 4);
    w = (g >> 16) & 0xF;
    p = (SprtWords *)g_primPtr;
    p->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    p->xy = (x + ((10 - w) >> 1)) | (y << 16);
    p->uvclut = (g & 0x40FFFF) | (g_textPalettes[mode] << 16);
    p->wh = w | 0xD0000;
    addPrim(&g_ot[otz], p);
    g_primPtr = (u8 *)(p + 1);
    return g >> 28;
}

void text_draw(s32 x, s32 y, s32 otz, u8 *s, s32 pal) {
    Unk7D4 *f = g_gameState->gameData->unk7D4;
    s32 th = f->unk4 >> 8;
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
            c = (c << 8) + s[1] - f->unk4 + f->unk6;
            s++;
        }
        g = *(f->unkC + c);
        setlen(p, 4);
        k = (g >> 20) & 3;
        p->rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
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
    setlen(p, 1);
    ((DrTpageWords *)p)->code = get_mode(1, 1, 0x1E);
    addPrim(ot, p);
    g_primPtr = (u8 *)p + 8;
}

u8 *text_buildBlock(s32 x, s32 y, u32 *ot, u8 *s, s32 mode, u8 *pkt) {
    s32 w;
    s32 x0;
    u8 th;
    Unk7D4 *font;
    s32 c;
    s32 k;

    font = g_gameState->gameData->unk7D4;
    g_textColor = mode;
    x0 = x;
    th = font->unk4 >> 8;
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
                text_formatInt(g_msgNumbers[*s & 7], g_textNumberBuf, 0);
                pkt = text_buildPlain(x, y, ot, (Character *)g_textNumberBuf, &w, pkt);
                x += w;
            } else if (*s == TEXT_ARG_NUMBER) {
                s++;
                text_formatInt(g_textNumberArgs[*s], g_textNumberBuf, 0);
                pkt = text_buildPlain(x, y, ot, (Character *)g_textNumberBuf, &w, pkt);
                x += w;
            } else if (*s == TEXT_ARG_STRING) {
                s++;
                pkt = text_buildPlain(x, y, ot, (Character *)g_textStringArgs[*s], &w, pkt);
                x += w;
            } else if (*s == TEXT_ARG_NUMBER_SMALL) {
                s++;
                text_formatInt(g_textNumberArgs[*s], g_textNumberBuf, 1);
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
                text_formatInt(g_textNumberArgs[*s], g_textNumberBuf, 1);
                pkt = ui_buildLabel(x, y, ot, (s32)g_textNumberBuf, g_textColor, pkt);
                x += ui_measureLabel(g_textNumberBuf);
            } else if (*s == TEXT_ARG_STRING_LABEL) {
                s++;
                g_textArgPtr = g_textStringArgs[*s];
                pkt = ui_buildLabel(x, y, ot, (s32)g_textArgPtr, g_textColor, pkt);
                x += ui_measureLabel(g_textArgPtr);
            } else if (*s == TEXT_ARG_NUMBER_DIGITS) {
                s++;
                text_formatInt(g_textNumberArgs[*s], g_textNumberBuf, 1);
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
                    x += g_uiSprites[g_itemTable[k].unk8 + 0x5C].unk4;
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
                    x += g_uiSprites[g_itemTable[k].unk8 + 0x5C].unk4;
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
                c = (c << 8) + s[1] - font->unk4 + font->unk6;
                s++;
                pkt = text_buildGlyph(x, y, ot, *(font->unkC + c), g_textColor, &w, (SprtWords *)pkt);
                x += w;
            } else {
                pkt = text_buildGlyph(x, y, ot, *(font->unkC + c), g_textColor, &w, (SprtWords *)pkt);
                x += w;
            }
        }
        s++;
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
    Unk7D4 *font;
    Unk7D4 *kanji;
    s32 th2;
    u8 *s;
    s32 c;
    s32 code;

    font = g_gameState->gameData->unk7D4;
    s = ch->name;
    x0 = x;
    th = font->unk4 >> 8;
    kanji = g_gameState->gameData->unk84C;
    th2 = 0;
    if (kanji != NULL) {
        th2 = kanji->unk0 >> 8;
    }
    while ((c = *s) != TEXT_END) {
        if (kanji != NULL && c >= th2) {
            code = (c << 8) + s[1];
            s++;
            c = code - kanji->unk0;
            pkt = text_buildGlyph(x, y, ot, *(kanji->unkC + c), g_textColor, &adv, (SprtWords *)pkt);
            x += adv;
        } else if (c >= th) {
            code = (c << 8) + s[1];
            s++;
            c = code - font->unk4 + font->unk6;
            pkt = text_buildGlyph(x, y, ot, *(font->unkC + c), g_textColor, &adv, (SprtWords *)pkt);
            x += adv;
        } else {
            pkt = text_buildGlyph(x, y, ot, *(font->unkC + c), g_textColor, &adv, (SprtWords *)pkt);
            x += adv;
        }
        s++;
    }
    setlen(pkt, 1);
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
    if (mode == 0) {
        rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    }
    tbl = (SpriteDesc16 *)g_uiSprites;
    clut = tbl[46].clut;
    if (mode == 1) {
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
            g = (SpriteDesc16 *)&D_80068890;
        } else if ((u8)c == '.') {
            g = (SpriteDesc16 *)&D_80068CE0;
        } else if ((u8)c == '_') {
            g = (SpriteDesc16 *)&D_80068DC8;
        }
        if (*s != ' ') {
            setlen(p, 4);
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
    setlen(p, 1);
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
            setlen(p, 3);
            p->rgbc = GPU_RGBC(GPU_SPRT_8, 0x808080);
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
    setlen(p, 1);
    ((DrTpageWords *)p)->code = get_mode(1, 1, 0x1F);
    hi = getaddr(ot); /* reuses hi as the tag temp */
    setaddr(p, hi), setaddr(ot, p);
    return (u8 *)p + 8;
}

u8 *text_buildCode(s32 x, s32 y, u32 *ot, s32 code, s32 *w, u8 *pkt) {
    pkt = text_buildFontTpage(ot, (DrTpageWords *)pkt);
    switch (code & 0xFF) {
    case TEXT_COLOR + 1:
        g_textColor = 1;
        *w = 0;
        break;
    case TEXT_COLOR + 3:
        g_textColor = 3;
        *w = 0;
        break;
    case TEXT_COLOR + 6:
        g_textColor = 6;
        *w = 0;
        break;
    case TEXT_COLOR + 7:
        g_textColor = 7;
        *w = 0;
        break;
    case TEXT_COLOR + 5:
        g_textColor = 5;
        *w = 0;
        break;
    case TEXT_COLOR + 4:
        g_textColor = 4;
        *w = 0;
        break;
    case TEXT_COLOR + 0:
        g_textColor = 0;
        *w = 0;
        break;
    case TEXT_COLOR + 2:
        g_textColor = 2;
        *w = 0;
        break;
    case TEXT_COLOR + 8:
        g_textColor = 8;
        *w = 0;
        break;
    case TEXT_COLOR + 9:
        g_textColor = 9;
        *w = 0;
        break;
    case 0x30:
        pkt = ui_buildSprite(x, y + 2, ot, 5, 0, w, pkt);
        break;
    case 0x31:
        pkt = ui_buildSprite(x, y + 2, ot, 4, 0, w, pkt);
        break;
    case 0x32:
        pkt = ui_buildSprite(x, y + 2, ot, 7, 0, w, pkt);
        break;
    case 0x33:
        pkt = ui_buildSprite(x, y + 2, ot, 6, 0, w, pkt);
        break;
    case 0x34:
        pkt = ui_buildSprite(x, y + 2, ot, 8, 0, w, pkt);
        break;
    case 0x35:
        pkt = ui_buildSprite(x, y + 2, ot, 10, 0, w, pkt);
        break;
    case 0x36:
        pkt = ui_buildSprite(x, y + 2, ot, 9, 0, w, pkt);
        break;
    case 0x37:
        pkt = ui_buildSprite(x, y + 2, ot, 11, 0, w, pkt);
        break;
    case 0x60:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(1), 0, w, pkt);
        break;
    case 0x61:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(0), 0, w, pkt);
        break;
    case 0x62:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(2), 0, w, pkt);
        break;
    case 0x63:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(3), 0, w, pkt);
        break;
    case 0x64:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(4), 0, w, pkt);
        break;
    case 0x65:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(5), 0, w, pkt);
        break;
    case 0x66:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(6), 0, w, pkt);
        break;
    case 0x67:
        pkt = ui_buildSprite(x, y + 2, ot, text_getButtonSprite(7), 0, w, pkt);
        break;
    case 0x38:
        pkt = ui_buildSprite(x, y + 2, ot, 0x11, 0, w, pkt);
        break;
    case 0x39:
        pkt = ui_buildSprite(x, y + 2, ot, 0x12, 0, w, pkt);
        break;
    case 0x3A:
        pkt = text_buildDpad(x, y + 2, ot, 1, 0, w, pkt);
        break;
    case 0x3B:
        pkt = text_buildDpad(x, y + 2, ot, 2, 0, w, pkt);
        break;
    case 0x3C:
        pkt = text_buildDpad(x, y + 2, ot, 4, 0, w, pkt);
        break;
    case 0x3D:
        pkt = text_buildDpad(x, y + 2, ot, 8, 0, w, pkt);
        break;
    case 0x3E:
        pkt = text_buildDpad(x, y + 2, ot, 3, 0, w, pkt);
        break;
    case 0x3F:
        pkt = text_buildDpad(x, y + 2, ot, 12, 0, w, pkt);
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
    case 0x30:
        r = g_uiSprites[5].unk4;
        break;
    case 0x31:
        r = g_uiSprites[4].unk4;
        break;
    case 0x32:
        r = g_uiSprites[7].unk4;
        break;
    case 0x33:
        r = g_uiSprites[6].unk4;
        break;
    case 0x34:
        r = g_uiSprites[8].unk4;
        break;
    case 0x35:
        r = g_uiSprites[10].unk4;
        break;
    case 0x36:
        r = g_uiSprites[9].unk4;
        break;
    case 0x37:
        r = g_uiSprites[11].unk4;
        break;
    case 0x60:
        r = g_uiSprites[text_getButtonSprite(0)].unk4;
        break;
    case 0x61:
        r = g_uiSprites[text_getButtonSprite(1)].unk4;
        break;
    case 0x62:
        r = g_uiSprites[text_getButtonSprite(2)].unk4;
        break;
    case 0x63:
        r = g_uiSprites[text_getButtonSprite(3)].unk4;
        break;
    case 0x64:
        r = g_uiSprites[text_getButtonSprite(4)].unk4;
        break;
    case 0x65:
        r = g_uiSprites[text_getButtonSprite(5)].unk4;
        break;
    case 0x66:
        r = g_uiSprites[text_getButtonSprite(6)].unk4;
        break;
    case 0x67:
        r = g_uiSprites[text_getButtonSprite(7)].unk4;
        break;
    case 0x38:
        r = g_uiSprites[17].unk4;
        break;
    case 0x39:
        r = g_uiSprites[18].unk4;
        break;
    case 0x3A:
    case 0x3B:
    case 0x3C:
    case 0x3D:
    case 0x3E:
    case 0x3F:
        r = g_uiSprites[12].unk4;
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

s32 text_formatTime(s32 frames, u8 *buf, s32 blink) {
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
        r = D_8006BBD4[hr / 100];
        hr %= 100;
    }
    if ((u8)blink) {
        sub = (u32)frames % 60;
    } else {
        sub = 0;
    }
    text_formatNumber_inline(hr, buf, 2, 1);
    text_formatNumber_inline(min, buf + 3, 2, 1);
    if (buf[3] == ' ') {
        buf[3] = '0';
    }
    text_formatNumber_inline(sec, buf + 6, 2, 1);
    if (buf[6] == ' ') {
        buf[6] = '0';
    }
    if ((u8)sub < 30) {
        buf[2] = ':';
        buf[5] = ':';
    } else {
        buf[5] = ' ';
        buf[2] = ' ';
    }
    if (hr < 10) {
        buf[0] = '0';
    }
    return r;
}

u8 *text_append(u8 *dst, u8 *src) {
    s32 c;
    u8 th;
    u8 *p;
    u8 *ret;
    u8 *s;
    s32 c2;
    u8 th2;

    ret = dst;
    s = ret;
    p = s;
    th = g_gameState->gameData->unk7D4->unk4 >> 8;
    while ((c = *p) != 0xFF) {
        p++;
        if (c == 0xF9) {
            p++;
            if ((u32)(p[-2] - 0x43) < 2) {
                p++;
            }
        } else if (c >= th) {
            p++;
        }
    }
    dst += p - s;
    th2 = g_gameState->gameData->unk7D4->unk4 >> 8;
    while ((c2 = *src) != 0xFF) {
        *dst++ = *src++;
        if (c2 == 0xF9) {
            *dst++ = *src++;
            if ((u32)(src[-2] - 0x43) < 2) {
                *dst++ = *src++;
            }
        } else if (c2 >= th2) {
            *dst++ = *src++;
        }
    }
    *dst = 0xFF;
    return ret;
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
            g = &D_80068690[*s];
        } else if (c >= 'a' && c <= 'z') {
            g = &D_80068690[*s - 32];
        } else if (c >= '0' && c <= '9') {
            g = &D_80068690[*s + 6];
        } else if (c == '&') {
            g = &D_80068890;
        } else if (c == '.') {
            g = &D_80068CE0;
        } else if (c == '_') {
            g = &D_80068DC8;
        }
        if (*s == ' ') {
            adv = 5;
        } else {
            adv = g->unk4;
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
    Unk7D4 *font;
    Unk7D4 *kanji;
    u8 th;
    u8 th2;
    s32 idx;
    Unk7D4 *tbl;

    memset(&r, 0, 4);
    w = 0;
    maxw = 0;
    font = g_gameState->gameData->unk7D4;
    kanji = g_gameState->gameData->unk84C;
    h = 15;
    th2 = 0;
    if (s == NULL) {
        return r;
    }
    th = font->unk4 >> 8;
    if (kanji != NULL) {
        th2 = kanji->unk0 >> 8;
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
                w += g_uiSprites[*s].unk4;
            } else if (*s == TEXT_ITEM_ICON) {
                s++;
                if (*s == 0xFF) {
                    w += g_uiSprites[D_8006A20E + 0x5C].unk4;
                }
            } else if (*s == TEXT_ARG_SPRITE) {
                s++;
                w += g_uiSprites[g_textIconArgs[*s]].unk4;
            } else if (*s == TEXT_ARG_ITEM_ICON) {
                s++;
                if (g_textIconArgs[*s] == 0xFF) {
                    w += g_uiSprites[D_8006A20E + 0x5C].unk4;
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
            c = code - kanji->unk0;
            s++;
            w += 1 + (*(kanji->unkC + c) >> 28);
        } else if (c < th) {
            w += 1 + (*(font->unkC + c) >> 28);
        } else {
            code = (c << 8) + s[1];
            c = code - font->unk4 + font->unk6;
            s++;
            w += 1 + (*(font->unkC + c) >> 28);
        }
        s++;
    }
    r.w = maxw > w ? maxw : w;
    r.h = h;
    return r;
}

s32 text_initNameImage(s32 arg0) {
    g_nameGlyphs = arg0;
    g_nameImage = arg0 + 0x4000;
    memset(g_nameImage, 0, 0x3F0);
    return g_nameImage;
}

void text_renderPartyName(s32 arg0) {
    s32 x = (arg0 & 1) * 0x54;
    s32 y = arg0 >> 1;
    s32 i;
    u8 *name = g_gameState->gameData->party[arg0]->name;

    for (i = 0; i < 7; i++) {
        if (*name == 0xFF) {
            break;
        }
        x = text_renderNameGlyph(x, y, *name++);
    }
}

void ui_drawLabel(s32 arg0, s32 arg1, s32 otz, s32 arg3, s32 arg4) {
    g_primPtr = ui_buildLabel(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void ui_drawSmallDigits(s32 arg0, s32 arg1, s32 otz, u8 *arg3, s32 arg4) {
    g_primPtr = text_buildSmall(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void ui_drawDigits(s32 arg0, s32 arg1, s32 otz, u8 *arg3, s32 arg4) {
    g_primPtr = text_buildDigits(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

void text_drawBlock(s32 arg0, s32 arg1, s32 otz, u8 *arg3, s32 arg4) {
    g_primPtr = text_buildBlock(arg0, arg1, &g_ot[otz], arg3, arg4, g_primPtr);
}

u8 *text_buildDigits(s32 x, s32 y, u32 *ot, u8 *s, s32 style, void *pkt) {
    SprtWords *p = pkt;
    DrTpageWords *t;
    u32 rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    s32 c;
    s32 u;

    if (style == 2) {
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
            setlen(p, 4);
            p->rgbc = rgbc;
            p->xy = (x & 0xFFFF) | (y << 16);
            p->wh = 0xA0008;
            p->uvclut = ((u & 0xFF) | 0x4000) | ((D_8006BBB4[style] + 0x3EE4) << 16);
            addPrim(ot, p);
            p++;
        }
        s++;
        x += 7;
    }
    t = (DrTpageWords *)p;
    setlen(t, 1);
    t->code = get_mode(1, 1, 0x1F);
    addPrim(ot, t);
    return (u8 *)(t + 1);
}

u8 *text_buildGlyph(s32 x, s32 y, u32 *ot, u32 a, s32 mode, s32 *out, SprtWords *p) {
    u32 rgbc = GPU_CODE(GPU_SPRT | GPU_ABE);
    s32 dx = (a >> 20) & 3;
    s32 h = (a >> 16) & 0xF;
    s32 n = a >> 28;
    if (mode != 1) {
        rgbc = GPU_RGBC(GPU_SPRT, 0x808080);
    }
    setlen(p, 4);
    p->xy = ((x + dx) & 0xFFFF) | (y << 16);
    p->rgbc = rgbc;
    p->uvclut = (a & 0x40FFFF) + (g_textPalettes[mode] << 16);
    p->wh = h | 0xD0000;
    addPrim(ot, p);
    if (out != NULL) {
        *out = n + 1;
    }
    return (u8 *)(p + 1);
}

u8 *text_buildDpad(s32 x, s32 y, u32 *ot, u32 mask, u8 b, s32 *c, u8 *pkt) {
    s32 tmp[2];
    s32 i;
    for (i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            pkt = ui_buildSprite(x + D_8006BC0C[i].unk0, y + D_8006BC0C[i].unk1, ot, D_8006BC0C[i].unk2, b, tmp, pkt);
        }
    }
    return ui_buildSprite(x, y, ot, 0xC, b, c, pkt);
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

s32 text_measureWidth(s32 arg0) {
    return text_measure((u8 *)arg0).w;
}

u8 *text_buildFontTpage(u32 *ot, DrTpageWords *p) {
    setlen(p, 1);
    p->code = get_mode(1, 1, 0x1E);
    addPrim(ot, p);
    return (u8 *)(p + 1);
}

void text_formatNumber(s32 n, u8 *buf, s32 width, u8 ascii) {
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
        text_formatInt(n, buf, ascii);
        return;
    }
    memset(buf, fill, width);
    for (i = 0; i < width; i++) {
        d = n % 10;
        q = n / 10;
        buf[width - i - 1] = d + base;
        if (q == 0) {
            break;
        }
        n = q;
    }
    end = buf + width;
    if (!ascii) {
        *end = 0xFF;
    } else {
        *end = 0;
    }
}

void text_formatInt(s32 v, u8 *buf, s32 ascii) {
    u8 tmp[16];
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
        *buf++ = base;
    } else {
        if (v < 0) {
            *buf++ = minus;
            v = -v;
        }
        while (v != 0) {
            tmp[n++] = base + v % 10;
            v /= 10;
        }
        for (i = 0; i < n;) {
            *buf++ = tmp[n - ++i];
        }
    }
    *buf = term;
}

s32 text_getLength(u8 *s) {
    u8 *start = s;
    u8 lim = g_gameState->gameData->unk7D4->unk4 >> 8;
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
    u8 lim = g_gameState->gameData->unk7D4->unk4 >> 8;
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

s32 text_getButtonSprite(s32 arg0) {
    return g_buttonSprites[config_remapButton(arg0)];
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
    s32 step = D_8006BBFC[idx];

    while (n != 0) {
        n /= 10;
        r += step;
    }
    return r - 1;
}

INCLUDE_RODATA("asm/us/main/nonmatchings/text", D_80010BD0);
