#include "ff9.h"

void msg_init(void) {
    GameState *s;
    Font *f;

    if (g_msgKeepWindows != 0) {
        g_msgKeepWindows = 0;
    } else {
        msg_resetWindows();
        win_closeAll();
    }
    s = g_gameState;
    g_msgReady = 0;
    f = s->gameData->font;
    g_penGlyphDecode.lead = 0;
    g_penGlyphs = f->glyphs;
    g_penGlyphDecode.extRange = *(s32 *)&f->extCode;
    D_8007B94C = f->unk8;
    g_penGlyphDecode.kanjiCode = -1;
    switch (s->mode) {
    case GS_MODE_FIELD:
        g_msgTable = s->gameData->ctx.ovl65->fieldArea->msgTable;
        break;
    case GS_MODE_WORLD:
        g_msgTable = (Unk8007B954 *)s->gameData->ctx.ovl65->worldObj->areaData;
        break;
    case GS_MODE_BATTLE:
        g_msgTable = (Unk8007B954 *)s->gameData->ctx.ovl65->work->texts;
        break;
    }
    g_msgReady = 1;
    msg_resetChoices();
}

static inline void msg_readPad_set(s32 type, u32 bits) {
    g_msgPadEvent.type = type;
    g_msgPadEvent.buttons = bits;
}

void msg_readPad(void) {
    s32 f = 0;
    u32 cur = g_padPortHeld[0] & 0x3FFFFFF;
    u32 x;

    x = ~cur & g_msgPadPrev;
    g_msgPadPressed = cur & ~g_msgPadPrev;
    g_msgPadReleased = x;
    if (x != 0) {
        msg_readPad_set(4, x);
    }
    if (g_msgPadPrev != cur) {
        g_msgPadRepeatTimer = 16;
    }
    g_msgPadRepeatTimer = g_gameState->vsyncMode != 0 ? g_msgPadRepeatTimer - 2 : g_msgPadRepeatTimer - 1;
    if (g_msgPadRepeatTimer < 0 && !(g_msgPadRepeatTimer & 3)) {
        f = 1;
        g_msgPadPrev &= ~(PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT);
    }
    x = cur & ~g_msgPadPrev;
    if (x != 0) {
        s32 v = 1;
        if (f) {
            v = 2;
        }
        msg_readPad_set(v, x);
    }
    g_msgPadPrev = cur;
}

void msg_beginDraw(void) {
    GameState *s = g_gameState;
    GameData *g;
    u8 **pp;
    u32 *tbl;
    u8 *cur;

    if (s->mode == GS_MODE_BATTLE || s->mode == 6) {
        D_8007B9A0 = 1;
        pp = &D_8007B9A8;
        tbl = &D_8007B9B0[s->gameData->drawBuf];
        g_penPktPtr = pp;
        D_8007B9A4 = D_8007B9A8 = (u8 *)tbl;
    } else {
        D_8007B9A0 = 0x4000;
        cur = g_primPtr;
        g_penPktPtr = &g_primPtr;
        D_8007B9A4 = cur;
    }
    g = g_gameState->gameData;
    g_penOt = g->ot[g->drawBuf];
}

/* width of an embedded string, up to TEXT_NEWLINE / TEXT_END */
static inline s32 msg_measureLine_width(u8 *s) {
    s32 w = 0;
    s32 multi = 0;
    s32 c;
    u32 g;

    g_penGlyphDecode.lead = 0;
    c = *s++;
    while (c != TEXT_NEWLINE && c != TEXT_END) {
        do {
            g = pen_getGlyph(c);
            if (g != 0) {
                w += 1 + (g >> 28);
            }
            multi = multi ? 0 : (c >= TEXT_WIDE_FIRST ? c <= TEXT_WIDE_LAST : 0);
            c = *s++;
        } while (multi);
    }
    return w;
}

/* copy character ch's name into g_msgNameBuf, converting the case */
static inline void msg_measureLine_name(s32 ch) {
    u8 *name = g_gameState->gameData->characters[ch].name;
    s32 k;
    u32 c;

    for (k = 0; k < 8; k++) {
        c = name[k];
        if (c - 0x30 < 0x1A) {
            c -= 0x20;
        } else if (c - 0x64 < 0x14) {
            c -= 0x14;
        } else if (c - 0x7A < 2) {
            c -= 2;
        }
        g_msgNameBuf[k] = c;
    }
}

void msg_measureLine(u8 **pp, s32 *pw) {
    u8 *s;
    u8 *t;
    u8 *tbl;
    u8 *str;
    u8 *p;
    s32 w;
    s32 c;
    s32 n, k;
    u32 g, i;

    if (pp != NULL && pw != NULL) {
        s = *pp;
        w = *pw;
        c = *s++;
        while (c != TEXT_END) {
            switch (c) {
            case TEXT_CHAR_NAME + 0:
            case TEXT_CHAR_NAME + 1:
            case TEXT_CHAR_NAME + 2:
            case TEXT_CHAR_NAME + 3:
            case TEXT_CHAR_NAME + 4:
            case TEXT_CHAR_NAME + 5:
            case TEXT_CHAR_NAME + 6:
            case TEXT_CHAR_NAME + 7:
                w += msg_measureLine_width(g_gameState->gameData->characters[c - TEXT_CHAR_NAME].name);
                break;
            case TEXT_PARTY_NAME + 0:
            case TEXT_PARTY_NAME + 1:
            case TEXT_PARTY_NAME + 2:
            case TEXT_PARTY_NAME + 3:
                w += msg_measureLine_width(g_gameState->gameData->characters[evt_getPartyChrIndex(c - TEXT_PARTY_NAME)].name);
                break;
            case TEXT_CHAR_NAME_CAPS + 0:
            case TEXT_CHAR_NAME_CAPS + 1:
            case TEXT_CHAR_NAME_CAPS + 2:
            case TEXT_CHAR_NAME_CAPS + 3:
            case TEXT_CHAR_NAME_CAPS + 4:
            case TEXT_CHAR_NAME_CAPS + 5:
            case TEXT_CHAR_NAME_CAPS + 6:
            case TEXT_CHAR_NAME_CAPS + 7:
                msg_measureLine_name(c - TEXT_CHAR_NAME_CAPS);
                w += msg_measureLine_width(g_msgNameBuf);
                break;
            case TEXT_MSG_ITEM_NAME:
                n = g_msgNumbers[*s++];
                if (n < ITEM_ID_KEY_FIRST) {
                    str = g_itemNames + g_itemTable[n].name;
                } else if (n < ITEM_ID_CARD_FIRST) {
                    str = g_keyItemNames + g_keyItemTable[n - ITEM_ID_KEY_FIRST].name;
                } else {
                    str = (u8 *)card_getName(n - ITEM_ID_CARD_FIRST);
                }
                w += msg_measureLine_width(str);
                break;
            case TEXT_MSG_NUMBER:
                n = g_msgNumbers[*s++];
                if (n < 0) {
                    n = -n;
                    w += 9;
                }
                do {
                    n /= 10;
                    w += 9;
                } while (n != 0);
                break;
            case TEXT_BANK_TEXT:
                c = *s++;
                g = (c >> 4) & 3;
                i = c & 7;
                t = NULL;
                if (g < 4 && i < 8 && (tbl = g_msgTextBanks[g]) != NULL) {
                    k = g_msgNumbers[i];
                    if (k < *tbl) {
                        tbl++;
                        k <<= 1;
                        p = tbl + k;
                        t = tbl + (p[0] | (p[1] << 8));
                    }
                }
                w += msg_measureLine_width(t);
                break;
            case TEXT_FLAG_ICON:
                if ((u32)g_msgNumbers[0] & (1 << *s++)) {
                    w += 0x19;
                }
                break;
            }
            c = *s++;
        }
        *pp = s;
        *pw = w + 9;
    }
}

INCLUDE_RODATA("asm/us/main/nonmatchings/msg", g_msgChoiceAnalogThresholds);

/* skip the choices hidden by g_msgChoiceDrawMask once the window reaches the choice line */
static inline void msg_renderText_skip(s32 id, u8 **pp) {
    s32 c;

    if (g_msgChoiceLine >= 0 && g_msgChoiceWindow == id && g_msgLine >= g_msgChoiceLine) {
        if (!(g_msgChoiceDrawMask & 1)) {
            do {
                while ((c = *(*pp)++) != TEXT_NEWLINE && c != TEXT_END) {
                    if (c < TEXT_PAGE) {
                        if (c >= TEXT_ESC) {
                            (*pp)++;
                        }
                    }
                }
                if (c == TEXT_END) {
                    (*pp)--;
                    break;
                }
                g_msgChoiceDrawMask >>= 1;
            } while (!(g_msgChoiceDrawMask & 1));
        }
        g_msgChoiceDrawMask >>= 1;
    }
}

/* typewriter: advance the shown character count of an embedded value of n characters */
static inline void msg_renderText_type(MsgWindow *w, s32 n) {
    w->typeAccum += w->speed;
    w->pendingChars += w->typeAccum >> 3;
    w->typeAccum &= 7;
    if (w->pendingChars >= n) {
        w->pendingChars = 0;
    }
}

/* text b of message bank a (g_msgTextBanks) */
static inline u8 *msg_renderText_text(u32 a, u32 b) {
    u8 *t = NULL;
    u8 *tbl;
    u8 *q;
    s32 k;

    if (a < 4 && b < 8 && (tbl = g_msgTextBanks[a]) != NULL) {
        k = g_msgNumbers[b];
        if (k < *tbl) {
            tbl++;
            k <<= 1;
            q = tbl + k;
            t = tbl + (q[0] | (q[1] << 8));
        }
    }
    return t;
}

/* print an embedded value, typewriter-style when it ends the text */
static inline void msg_renderText_value(MsgWindow *w, s32 arg, s32 (*fn)(), s32 last) {
    if (last && w->pendingChars != 0) {
        msg_renderText_type(w, fn(arg, w->pendingChars));
    } else {
        fn(arg, 0xFF);
    }
}

void msg_renderText(MsgWindow *w, u8 *s) {
    u8 *p;
    u8 *end;
    s32 c;
    s32 last;
    s32 v;
    u8 *t;
    u8 *tbl;
    u32 b;
    s32 k;
    u8 *q;

    g_msgLine = 0;
    end = s + w->textPos;
    p = s + w->pageStart;
    msg_renderText_skip(w->id, &p);
    while (p < end) {
        c = *p++;
        switch (c) {
        case TEXT_SPACE_1PX:
            pen_move(1, 0);
            break;
        case TEXT_NEWLINE:
            pen_popPos();
            pen_move(0, 14);
            pen_pushPos();
            g_msgLine++;
            msg_renderText_skip(w->id, &p);
            if (p >= end) {
                w->textPos = p - s;
            }
            break;
        case TEXT_ESC:
            /* padding between the two jump tables in the original rodata */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            c = *p++;
            switch (c) {
            case TEXT_TAB:
                pen_move(0x12, 0);
                break;
            case TEXT_X_SET:
                pen_setX(g_penStackPtr[-1].vx);
            case TEXT_X_ADD:
                pen_move(*p++, 0);
                break;
            case TEXT_Y_DOWN:
                pen_move(0, *p++);
                break;
            case TEXT_Y_UP:
                pen_move(0, -*p++);
                break;
            case TEXT_CENTER:
                c = *p++;
                c |= *p++ << 8;
                msg_measureLine(&p, &c);
                pen_move((w->w - c) >> 1, 0);
                break;
            case TEXT_COLOR + 0:
            case TEXT_COLOR + 1:
            case TEXT_COLOR + 2:
            case TEXT_COLOR + 3:
            case TEXT_COLOR + 4:
            case TEXT_COLOR + 5:
            case TEXT_COLOR + 6:
            case TEXT_COLOR + 7:
            case TEXT_COLOR + 8:
                pen_setPalette(c - TEXT_COLOR);
                break;
            case TEXT_MSG_NUMBER:
                last = end - p == 1;
                v = g_msgNumbers[*p++];
                msg_renderText_value(w, v, msg_emitNumber, last);
                break;
            case TEXT_BANK_TEXT:
                c = *p++;
                last = end - p == 1;
                t = msg_renderText_text(c, *p++);
                msg_renderText_value(w, (s32)t, msg_emitString, last);
                break;
            case TEXT_MSG_ITEM_NAME:
                last = end - p == 1;
                pen_setPalette(4);
                msg_renderText_value(w, g_msgNumbers[*p++], msg_emitItemName, last);
                pen_setPalette(0);
                break;
            case TEXT_CHAR_NAME + 0:
            case TEXT_CHAR_NAME + 1:
            case TEXT_CHAR_NAME + 2:
            case TEXT_CHAR_NAME + 3:
            case TEXT_CHAR_NAME + 4:
            case TEXT_CHAR_NAME + 5:
            case TEXT_CHAR_NAME + 6:
            case TEXT_CHAR_NAME + 7:
                msg_renderText_value(w, c - TEXT_CHAR_NAME, msg_emitCharName, end == p);
                break;
            case TEXT_PARTY_NAME + 0:
            case TEXT_PARTY_NAME + 1:
            case TEXT_PARTY_NAME + 2:
            case TEXT_PARTY_NAME + 3:
                last = end - p == 0;
                msg_renderText_value(w, evt_getPartyChrIndex(c - TEXT_PARTY_NAME), msg_emitCharName, last);
                break;
            case TEXT_CHAR_NAME_CAPS + 0:
            case TEXT_CHAR_NAME_CAPS + 1:
            case TEXT_CHAR_NAME_CAPS + 2:
            case TEXT_CHAR_NAME_CAPS + 3:
            case TEXT_CHAR_NAME_CAPS + 4:
            case TEXT_CHAR_NAME_CAPS + 5:
            case TEXT_CHAR_NAME_CAPS + 6:
            case TEXT_CHAR_NAME_CAPS + 7:
                msg_renderText_value(w, c - TEXT_CHAR_NAME_CAPS, msg_emitCharNameCaps, end == p);
                break;
            case TEXT_BTN_CIRCLE:
            case TEXT_BTN_CROSS:
            case TEXT_BTN_TRIANGLE:
            case TEXT_BTN_SQUARE:
            case TEXT_BTN_L1:
            case TEXT_BTN_R1:
            case TEXT_BTN_L2:
            case TEXT_BTN_R2:
            case TEXT_BTN_START:
            case TEXT_BTN_SELECT:
                pen_move(0, 1);
                pen_drawSprite(g_msgIconSprites[c - TEXT_BTN_CIRCLE]);
                pen_move(1, -1);
                break;
            case TEXT_PAD_BTN + 0:
            case TEXT_PAD_BTN + 1:
            case TEXT_PAD_BTN + 2:
            case TEXT_PAD_BTN + 3:
            case TEXT_PAD_BTN + 4:
            case TEXT_PAD_BTN + 5:
            case TEXT_PAD_BTN + 6:
            case TEXT_PAD_BTN + 7:
                pen_move(0, 1);
                if (c < TEXT_PAD_BTN + 2) {
                    c ^= 1;
                }
                c = (g_gameState->config >> CFG_BIT_CONTROLLER) & 1 ? (g_gameState->padMap >> (c * 4)) & 0xF : c & 0xF;
                if (c < 2) {
                    c ^= 1;
                }
                pen_drawSprite(g_msgIconSprites[c]);
                pen_move(1, -1);
                break;
            case TEXT_DPAD_UP:
                pen_move(4, 2);
                pen_drawSprite(13);
                pen_move(-8, -1);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case TEXT_DPAD_DOWN:
                pen_move(4, 7);
                pen_drawSprite(14);
                pen_move(-8, -6);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case TEXT_DPAD_LEFT:
                pen_move(1, 5);
                pen_drawSprite(15);
                pen_move(-5, -4);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case TEXT_DPAD_RIGHT:
                pen_move(6, 5);
                pen_drawSprite(16);
                pen_move(-10, -4);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case TEXT_DPAD:
                pen_move(0, 1);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case TEXT_SPRITE:
                pen_drawSprite(*p++);
                break;
            case TEXT_FLAG_ICON:
                if ((u32)g_msgNumbers[0] & (1 << *p++)) {
                    pen_move(6, 3);
                    pen_drawSprite(0xB6);
                    if (VSync(-1) % 48 >= 16) {
                        pen_setLastSemiTrans();
                    }
                    pen_move(-24, -3);
                    pen_drawSprite(0xB5);
                }
                break;
            case TEXT_SPEED:
            case TEXT_SYNC_TAG:
            case TEXT_WAIT:
                p++;
                break;
            }
            break;
        case TEXT_PAGE:
        case TEXT_END:
            break;
        case TEXT_WIDE_FIRST:
        case TEXT_WIDE_FIRST + 1:
        case TEXT_WIDE_FIRST + 2:
        case TEXT_WIDE_LAST:
            pen_putGlyph(c);
            c = *p++;
        default:
            pen_putGlyph(c);
            break;
        }
    }
}

void msg_advanceText(MsgWindow *w, u8 *e) {
    u8 *p;
    s32 c;
    u8 *start;
    Unk4 tmp;
    s32 n;

    p = e + w->textPos;
    start = p;
    while (w->waitFrames == 0 && w->pendingChars == 0 && w->typeAccum >= 8) {
        start = p;
        c = *p++;
        switch (c) {
        case TEXT_ESC:
            /* padding between the two jump tables in the original rodata */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            c = *p++;
            switch (c) {
            case TEXT_CENTER:
                p += 2;
                msg_measureLine(&p, &c);
                break;
            case TEXT_WAIT:
                if (!(w->flags & MSGW_SKIP)) {
                    w->waitFrames = *p++;
                } else {
                    p++;
                }
                break;
            case TEXT_WAIT_PAGE:
            case TEXT_WAIT_CLOSE:
                w->flags = (w->flags & ~MSGW_SKIP) | MSGW_NO_SKIP;
            set26:
                w->waitFrames = *p++;
                break;
            case TEXT_SPEED:
                w->speed = *p++;
                break;
            case TEXT_SPEED_DEFAULT:
                w->speed = g_msgTextSpeeds[(g_gameState->config >> CFG_SHIFT_MSG_SPEED) & 7];
                break;
            case TEXT_SKIP:
                w->flags |= MSGW_SKIP;
                break;
            case TEXT_CHOICES:
                p += 2;
                break;
            case TEXT_CHOICE_START:
                tmp = g_msgChoiceAnalogThresholds;
                n = g_msgLine;
                pad_getAnalogThresholds(&g_msgSavedAnalogThresholds);
                pad_setAnalogThresholds(&tmp);
                g_msgChoiceLine = n;
                w->flags |= MSGW_CHOICE;
                break;
            case TEXT_HOLD:
                w->waitFrames = MSGW_WAIT_INPUT;
            case TEXT_NO_INPUT:
                w->flags |= MSGW_NO_INPUT;
                break;
            case TEXT_NO_SKIP:
                w->flags |= MSGW_NO_SKIP;
                break;
            case TEXT_SYNC_TAG:
                g_msgSyncTag = *p++;
                break;
            case TEXT_SYNC_TAG_NEXT:
                g_msgSyncTag++;
                break;
            case TEXT_BANK_TEXT:
                p++;
            case TEXT_MSG_ITEM_NAME:
            case TEXT_MSG_NUMBER:
                p++;
            case TEXT_CHAR_NAME + 0:
            case TEXT_CHAR_NAME + 1:
            case TEXT_CHAR_NAME + 2:
            case TEXT_CHAR_NAME + 3:
            case TEXT_CHAR_NAME + 4:
            case TEXT_CHAR_NAME + 5:
            case TEXT_CHAR_NAME + 6:
            case TEXT_CHAR_NAME + 7:
            case TEXT_PARTY_NAME + 0:
            case TEXT_PARTY_NAME + 1:
            case TEXT_PARTY_NAME + 2:
            case TEXT_PARTY_NAME + 3:
            case TEXT_CHAR_NAME_CAPS + 0:
            case TEXT_CHAR_NAME_CAPS + 1:
            case TEXT_CHAR_NAME_CAPS + 2:
            case TEXT_CHAR_NAME_CAPS + 3:
            case TEXT_CHAR_NAME_CAPS + 4:
            case TEXT_CHAR_NAME_CAPS + 5:
            case TEXT_CHAR_NAME_CAPS + 6:
            case TEXT_CHAR_NAME_CAPS + 7:
                if (!(w->flags & MSGW_SKIP)) {
                    w->pendingChars = 1;
                    w->typeAccum = 0;
                }
                break;
            case TEXT_X_ADD:
            case TEXT_X_SET:
            case TEXT_Y_DOWN:
            case TEXT_Y_UP:
            case TEXT_SPRITE:
            case TEXT_FLAG_ICON:
            skip1:
                p++;
                break;
            }
            break;
        case TEXT_PAGE:
        case TEXT_END:
            w->waitFrames = MSGW_WAIT_INPUT;
            break;
        case TEXT_NEWLINE:
            g_msgLine++;
            break;
        case TEXT_WIDE_FIRST:
        case TEXT_WIDE_FIRST + 1:
        case TEXT_WIDE_FIRST + 2:
        case TEXT_WIDE_LAST:
            p++;
        default:
            if (!(w->flags & MSGW_SKIP)) {
                w->typeAccum -= 8;
            }
            break;
        }
    }
    w->textPos = (w->waitFrames != 0 ? start : p) - e;
    if (w->waitFrames == MSGW_WAIT_INPUT) {
        w->flags &= ~MSGW_SKIP;
    }
    w->typeAccum += w->speed;
}

void msg_stepControl(MsgWindow *w, u8 *e) {
    u8 *p = e + w->textPos;
    s32 c;

    c = *p++;
    switch (c) {
    case TEXT_ESC:
        c = *p++;
        switch (c) {
        case TEXT_WAIT:
            w->textPos = p + 1 - e;
            break;
        case TEXT_WAIT_CLOSE:
            msg_closeWindow_inline(w);
            break;
        case TEXT_WAIT_PAGE:
            w->pageStart = p + 1 - e;
            w->flags &= ~MSGW_NO_SKIP;
            break;
        }
        break;
    case TEXT_END:
        msg_closeWindow_inline(w);
        break;
    case TEXT_PAGE:
        w->pageStart = p - e;
        break;
    }
}

void msg_drawText(MsgWindow *w) {
    u8 *e = NULL;
    s32 i = w->msgId;

    if (i >= 0 && i < g_msgTable->count) {
        e = (u8 *)g_msgTable->offsets;
        e += *(u16 *)&((u32 *)e)[(g_evtMode == EVT_MODE_BATTLE || g_evtMode == EVT_MODE_BATTLE_END) ? i : i * 2 + 1];
    }
    if (e != NULL) {
        pen_move(((w->w - w->textWidth) >> 1) + 4, 2);
        pen_pushPos();
        if (w->textPos < w->pageStart) {
            w->textPos = w->pageStart;
        }
        g_penGlyphDecode.lead = 0;
        g_msgChoiceDrawMask = g_msgChoiceEnabled;
        msg_renderText(w, e);
        if (w->waitFrames == 0) {
            if (w->pendingChars == 0) {
                msg_advanceText(w, e);
            } else if (w->flags & MSGW_SKIP) {
                w->pendingChars = 0;
                w->typeAccum = 8;
            }
        } else {
            w->waitFrames -= (w->waitFrames != MSGW_WAIT_INPUT);
            if (w->flags & MSGW_SKIP) {
                w->waitFrames = 0;
            }
            if (w->waitFrames == 0) {
                msg_stepControl(w, e);
            }
        }
        pen_popPos();
        pen_setPalette(0);
    }
}

/* The sound callback returns s32 (GameData.soundHook), so each call is a call_value insn even with the
   result ignored. That gives the function pointer a v0 preference in local-alloc, which the
   0x10000 path needs (pointer in v0, %hi(g_msgChoice) in v1). The 0x10000 path must come
   last in the chain so its call falls into the shared `return 1` and jump2 cross-jumps the
   0x10/0x40 calls onto its jalr. */
s32 msg_input(MsgWindow *w, s32 pad) {
    u8 f;

    if ((g_evtMode == EVT_MODE_FIELD && (pad & (BTN_CROSS | BTN_SQUARE))) || (g_evtMode != EVT_MODE_FIELD && (pad & BTN_CROSS))) {
        f = w->flags;
        if (!(f & MSGW_NO_INPUT)) {
            if (w->waitFrames == MSGW_WAIT_INPUT) {
                w->waitFrames = 1;
                if (g_msgChoiceLine >= 0) {
                    g_gameState->gameData->soundHook(SND_SE_PLAY, SE_CURSOR, 0x800000, SND_PAN_CENTER, SND_VOL_MAX);
                }
                return 1;
            }
            if (!(f & MSGW_NO_SKIP)) {
                w->flags = f | MSGW_SKIP;
                return 1;
            }
        }
    }
    if (g_msgChoiceLine >= 0) {
        if (g_msgChoiceWindow != w->id) {
            return 0;
        }
        if (pad & PAD_UP) {
            if (g_msgChoice > 0) {
                g_msgChoice--;
            } else {
                g_msgChoice = g_msgChoiceCount - 1;
            }
            g_gameState->gameData->soundHook(SND_SE_PLAY, SE_CURSOR, 0x800000, SND_PAN_CENTER, SND_VOL_MAX);
        } else if (pad & PAD_DOWN) {
            if (g_msgChoice < g_msgChoiceCount - 1) {
                g_msgChoice++;
            } else {
                g_msgChoice = 0;
            }
            g_gameState->gameData->soundHook(SND_SE_PLAY, SE_CURSOR, 0x800000, SND_PAN_CENTER, SND_VOL_MAX);
        } else if (!(pad & BTN_CIRCLE) || g_msgChoiceCancel < 0) {
            return 0;
        } else {
            g_msgChoice = g_msgChoiceCancel;
            g_gameState->gameData->soundHook(SND_SE_PLAY, SE_CANCEL, 0x800000, SND_PAN_CENTER, SND_VOL_MAX);
        }
        return 1;
    }
    return 0;
}

static inline MsgWindow *msg_open_init(s32 id, WinLayout *l) {
    MsgWindow *w;
    MsgWindow *parent;

    msg_closeWindow_inline(&g_msgWindows[id]);
    parent = g_msgFocus;
    w = &g_msgWindows[id];
    w->open = 1;
    w->onInput = NULL;
    w->unk10 = 0;
    w->y = 0;
    w->x = 0;
    w->id = 0;
    w->parent = parent;
    if (l->x != 0) {
        w->x = l->x;
        w->y = (u8)l->y;
    } else {
        w->x = 0xA0 - (l->w >> 1);
        w->y = 0x78 - (l->h >> 1);
    }
    w->w = l->w;
    w->h = l->h;
    w->flags = 0;
    w->msgId = 0;
    w->textPos = 0;
    w->tailOffset = 0;
    w->tailMode = 0;
    w->window = NULL;
    return w;
}

/* window titles */
INCLUDE_RODATA("asm/us/main/nonmatchings/msg", g_msgAteTitle);

INCLUDE_RODATA("asm/us/main/nonmatchings/msg", g_msgMognetTitle);

void msg_open(s32 msg, s32 id, WinLayout *l, s32 flags) {
    MsgWindow *w;
    RECT r;
    s32 style;
    s32 n;
    u8 *e;

    e = NULL;
    if (msg >= 0 && msg < g_msgTable->count) {
        e = (u8 *)g_msgTable->offsets;
        e += *(u16 *)&((u32 *)e)[(g_evtMode == EVT_MODE_BATTLE || g_evtMode == EVT_MODE_BATTLE_END) ? msg : msg * 2 + 1];
    }
    if (e == NULL) {
        return;
    }
    w = msg_open_init(id, l);
    w->onInput = msg_input;
    w->waitFrames = 0;
    w->id = id;
    w->msgId = msg;
    w->speed = g_msgTextSpeeds[(g_gameState->config >> CFG_SHIFT_MSG_SPEED) & 7];
    w->pageStart = 0;
    w->typeAccum = 8;
    w->pendingChars = 0;
    w->tailOffset = l->tailOffset;
    w->tailMode = l->tailMode;
    w->textWidth = l->w;
    w->flags |= MSGW_MESSAGE;
    if (!(l->flags & WINLAYOUT_NO_FOCUS)) {
        g_msgFocus = w;
    }
    r.x = w->x;
    r.y = w->y;
    r.w = w->w + 3;
    r.h = w->h + 3;
    style = WIN_STYLE_FRAMELESS;
    if (!(l->tailMode & MSG_TAIL_FRAMELESS)) {
        style = WIN_STYLE_BALLOON;
        if (l->tailOffset == 0 && !(flags & MSG_FLAG_NO_TAIL)) {
            style = WIN_STYLE_BEVEL;
        }
    }
    w->window = win_open(&r, 2, style, id);
    if (w->window != NULL) {
        w->window->userData = (s32)w;
        win_setDrawContent(w->window, msg_drawWindowContent);
        win_setDrawOverlay(w->window, msg_drawChoiceCursor);
        if (l->tailOffset != 0 && !(flags & MSG_FLAG_NO_TAIL)) {
            win_setTail(w->window, l->tailMode & MSG_TAIL_DIR, l->tailOffset);
        }
        if (g_msgLayoutAltOpen != 0) {
            win_setOpenAnim(w->window, WIN_ANIM_NONE);
        }
        if (flags & MSG_FLAG_ATE) {
            win_setTitle(w->window, (s32)g_msgAteTitle);
        } else if (flags & MSG_FLAG_MOGNET) {
            win_setTitle(w->window, (s32)g_msgMognetTitle);
        }
    }
    if (!(l->flags & WINLAYOUT_NO_FOCUS)) {
        g_msgChoiceLine = -1;
        n = g_msgChoiceDefault;
        g_msgChoiceDefault = 0;
        g_msgChoiceWindow = id;
        g_msgChoice = n;
    }
    g_evtMsgCount++;
    msg_startHereIcon(0);
}

void msg_resetChoices(void) {
    s32 i;
    u8 *e;

    g_msgChoice = 0;
    g_msgChoiceMaskReq = -1;
    g_msgChoiceLine = -1;
    for (i = 0; i < 4; i++) {
        e = msg_drawText_msg(i);
        if (e == NULL || e[0] != TEXT_ESC || e[1] != TEXT_BANK) {
            break;
        }
        g_msgTextBanks[i] = e + 2;
    }
    g_msgChoiceDefault = 0;
    g_evtMsgCount = 0;
}

/* Inlined copy of msg_getEntry (message i, its size into out). */
static inline u8 *msg_getEntry_inline(s32 i, WinLayout *out) {
    u8 *e = NULL;
    s32 v;

    if (i >= 0 && i < g_msgTable->count) {
        e = (u8 *)g_msgTable->offsets;
        v = ((u32 *)e)[g_evtMode == EVT_MODE_BATTLE_END ? i : i * 2];
        if (out != NULL) {
            out->w = ((v >> 16) & 0xFFF) + 9;
            out->h = (g_evtMode == EVT_MODE_BATTLE_END ? (u32)v >> 28 : ((MsgEntry8 *)e)[i].lines) * 14 + 6;
        }
        e += v & 0xFFFF;
    }
    return e;
}

WinLayout *msg_getLayout(s32 id, s32 *x, s32 *y, s32 *z) {
    u8 *p;
    s32 v;
    s32 done;
    s32 t;
    s32 n, i;
    s32 mask;
    s32 cnt;

    p = msg_getEntry_inline(id, &g_msgLayout);
    if (p != NULL) {
        done = 0;
        g_msgLayoutAltOpen = 0;
        g_msgLayout.anchor = WINLAYOUT_ANCHOR_NONE;
        g_msgLayout.flags = 0;
        *x = 0;
        *y = 0;
        *z = 0;
        while (!done) {
            if (*p++ != TEXT_ESC) {
                break;
            }
            switch (*p++) {
            case TEXT_CHOICES:
                g_msgChoiceEnabled = -1;
                g_msgChoiceCount = *p++;
                g_msgChoiceCancel = *p++ - 1;
                break;
            case TEXT_CHOICES_MASKED:
                g_msgChoiceEnabled = g_msgChoiceMaskReq;
                n = i = *p;
                mask = g_msgChoiceMaskReq;
                while (--i != -1) {
                    n -= (mask ^ 1) & 1;
                    mask >>= 1;
                }
                g_msgChoiceCount = n;
                g_msgLayout.h -= (*p++ - n) * 14;
                n = -1;
                mask = g_msgChoiceEnabled;
                cnt = *p++;
                while (--cnt != -1) {
                    n += mask & 1;
                    mask >>= 1;
                }
                if (n >= g_msgChoiceCount) {
                    n = g_msgChoiceCount - 1;
                }
                g_msgChoiceCancel = n;
                break;
            case TEXT_WIN_POS:
                g_msgLayout.x = *p++;
                g_msgLayout.x |= *p++ << 8;
                g_msgLayout.y = *p++;
                g_msgLayout.anchor = WINLAYOUT_ANCHOR_POS;
                break;
            case TEXT_WIN_BOTTOM_RIGHT:
            case TEXT_WIN_BOTTOM_LEFT:
            case TEXT_WIN_TOP_RIGHT:
            case TEXT_WIN_TOP_LEFT:
            case TEXT_WIN_BOTTOM:
            case TEXT_WIN_TOP:
            case TEXT_WIN_FIXED_BOTTOM_RIGHT:
            case TEXT_WIN_FIXED_BOTTOM_LEFT:
            case TEXT_WIN_FIXED_TOP_RIGHT:
            case TEXT_WIN_FIXED_TOP_LEFT:
            case TEXT_WIN_CENTER:
                g_msgLayout.anchor = p[-1];
                break;
            case TEXT_BALLOON_OFS:
                *x = *p;
                *x |= *(s8 *)++p << 8;
                *y = *++p;
                *y |= *(s8 *)++p << 8;
                *z = *++p;
                *z |= *(s8 *)++p << 8;
                p++;
                break;
            case TEXT_NO_OPEN_ANIM:
                g_msgLayoutAltOpen = 1;
                break;
            case TEXT_NO_INPUT:
                g_msgLayout.flags = WINLAYOUT_NO_FOCUS;
                break;
            case TEXT_LINE_WIDTHS:
                for (;;) {
                    v = *p++;
                    v |= *p++ << 8;
                    if (v == 0xFFFF) break;
                    t = (v >> 12) & 0xF;
                    v &= 0xFFF;
                    if (t == 0 || ((g_msgChoiceEnabled >> (t - 1)) & 1)) {
                        msg_measureLine(&p, &v);
                        if (g_msgLayout.w < v) {
                            g_msgLayout.w = v;
                        }
                    } else {
                        while (*p++ != TEXT_END) {
                        }
                    }
                }
                break;
            default:
                done = 1;
                break;
            }
        }
    }
    return &g_msgLayout;
}

void msg_setOtz(s32 a) {
    GameData *d = g_gameState->gameData;
    g_penOt = d->ot[d->drawBuf] + a;
}

s32 msg_getPartyMemberId(s32 i) {
    Character *p = g_gameState->gameData->party[i];
    if (p == NULL) {
        return -1;
    }
    return p->info.b.id;
}

void msg_resetPad(void) {
    g_msgPadPrev = 0;
    g_msgPadEvent.type = 0;
    g_msgPadEvent.buttons = 0;
}

void msg_update(void) {
    MsgWindow *p = msg_getFocus();
    s32 a;
    msg_readPad();
    if (g_msgPadEvent.type < 3) {
        if (g_msgPadEvent.type > 0) {
            while (p != NULL) {
                a = g_msgPadEvent.buttons;
                if (g_msgPadEvent.type == 2) {
                    a |= 0x80000000;
                }
                if (p->onInput(p, a) != 0) {
                    break;
                }
                p = p->parent;
            }
        }
    }
    g_msgPadEvent.type = 0;
    g_msgPadEvent.buttons = 0;
    D_8007B998 = D_8007B994;
}

u32 msg_getPadHeld(void) {
    return g_padPortHeld[0] & 0x3FFFFFF;
}

u32 msg_getPadPressed(void) {
    return g_msgPadPressed & 0x3FFFFFF;
}

u32 msg_getPadReleased(void) {
    return g_msgPadReleased & 0x3FFFFFF;
}

void func_8004E91C(void) {
}

void msg_resetWindows(void) {
    MsgWindow *p;
    for (p = g_msgWindows; p < g_msgWindows + 10; p++) {
        p->open = 0;
    }
    g_msgFocus = NULL;
}

void msg_closeAll(void) {
    MsgWindow *p;
    MsgWindow *q;
    for (p = g_msgWindows; p < g_msgWindows + 8; p++) {
        if (p != NULL && p->open != 0) {
            p->open = 0;
            if (msg_getFocus() == p) {
                msg_setFocus(p->parent);
            }
            for (q = g_msgWindows; q < g_msgWindows + 10; q++) {
                if (q->open != 0 && q->parent == p) {
                    q->parent = NULL;
                }
            }
        }
    }
}

void msg_setFocus(MsgWindow *a) {
    g_msgFocus = a;
}

MsgWindow *msg_getFocus(void) {
    return g_msgFocus;
}

void msg_close(s32 i) {
    msg_closeWindow_inline(&g_msgWindows[i]);
}

MsgWindow *msg_getLastModal(void) {
    MsgWindow *p;
    MsgWindow *r = NULL;
    for (p = g_msgWindows; p < g_msgWindows + 10; p++) {
        if (p->open != 0 && (p->flags & MSGW_CHOICE)) {
            r = p;
        }
    }
    return r;
}

s32 msg_overlapsRect(WinLayout *r) {
    MsgWindow *p;
    for (p = g_msgWindows; p < g_msgWindows + 8; p++) {
        if (p->open != 0 && r->x < p->x + p->w && p->x < r->x + r->w &&
            (u8)r->y < p->y + p->h && p->y < (u8)r->y + r->h) {
            return 1;
        }
    }
    return 0;
}

void msg_setNumber(u32 i, s32 v) {
    if (i < 8) {
        g_msgNumbers[i] = v;
    }
}

s16 msg_isOpen(s32 i) {
    return g_msgWindows[i].open;
}

s32 msg_getChoiceIndex(void) {
    s32 i = -1;
    s32 b = g_msgChoiceEnabled;
    s32 a = g_msgChoice;
    for (; i < 15;) {
        if (a < 0) {
            break;
        }
        a -= b & 1;
        i++;
        b >>= 1;
    }
    return i;
}

void msg_setChoiceMask(s32 a, s32 b) {
    s32 r;
    g_msgChoiceMaskReq = a;
    r = -1;
    for (; b >= 0; b--) {
        if (a == 0) {
            break;
        }
        r += a & 1;
        a >>= 1;
    }
    if (r < 0) {
        r = 0;
    }
    g_msgChoiceDefault = r;
}

void msg_clearWindowOtz(void) {
    MsgWindow *p;
    for (p = g_msgWindows; p < g_msgWindows + 10; p++) {
        if (p->open != 0 && p->window != NULL) {
            p->window->otz = 0;
        }
    }
}

Window *msg_getWindow(s32 i) {
    MsgWindow *e = &g_msgWindows[i];
    Window *r = NULL;
    if (e->open != 0) {
        r = e->window;
    }
    return r;
}

MsgWindow *msg_allocWindow(WinLayout *r, MsgWindow *a, s32 idx, s32 flag) {
    MsgWindow *e = &g_msgWindows[idx];
    e->open = 1;
    e->parent = a;
    e->onInput = 0;
    e->unk10 = 0;
    e->y = 0;
    e->x = 0;
    e->id = 0;
    if (r->x != 0) {
        e->x = r->x;
        e->y = (u8)r->y;
    } else {
        e->x = 160 - (r->w >> 1);
        e->y = 120 - (r->h >> 1);
    }
    e->w = r->w;
    e->h = r->h;
    if (flag != 0) {
        e->flags = MSGW_CHOICE;
    } else {
        e->flags = 0;
    }
    e->msgId = 0;
    e->textPos = 0;
    e->tailOffset = 0;
    e->tailMode = 0;
    e->window = NULL;
    return e;
}

void msg_closeWindow(MsgWindow *p) {
    msg_closeWindow_inline(p);
}

void msg_keepWindowsOnInit(void) {
    g_msgKeepWindows = 1;
}

u8 *msg_getEntry(s32 i, WinLayout *out) {
    u8 *e = NULL;
    u16 *t;
    s32 v;
    u32 k;

    if (i >= 0 && i < g_msgTable->count) {
        e = (u8 *)g_msgTable->offsets;
        v = ((u32 *)e)[g_evtMode == EVT_MODE_BATTLE_END ? i : i * 2];
        if (out != NULL) {
            out->w = ((v >> 16) & 0xFFF) + 9;
            out->h = (g_evtMode == EVT_MODE_BATTLE_END ? (u32)v >> 28 : ((MsgEntry8 *)e)[i].lines) * 14 + 6;
        }
        e += v & 0xFFFF;
    }
    return e;
}

void *msg_getText(s32 i) {
    u8 *e = NULL;

    if (i >= 0 && i < g_msgTable->count) {
        e = (u8 *)g_msgTable->offsets;
        e += *(u16 *)&((u32 *)e)[(g_evtMode == EVT_MODE_BATTLE || g_evtMode == EVT_MODE_BATTLE_END) ? i : i * 2 + 1];
    }
    return e;
}

s32 msg_emitNumber(s32 v, s32 n) {
    s32 d;
    s32 digits;
    s32 i;
    s32 q;
    s32 ch;

    if (v < 0) {
        v = 0;
    }
    d = 10;
    digits = 1;
    while (v >= d) {
        d *= 10;
        digits++;
    }
    for (i = 0; i < n; i++) {
        d /= 10;
        if (d == 0) {
            break;
        }
        q = v / d;
        if (d < 2 || q != 0) {
            ch = q % 10;
        } else {
            ch = 0xF;
        }
        pen_putGlyph(ch);
    }
    return digits;
}

s32 msg_emitString(u8 *s, s32 max) {
    s32 n = 0;
    s32 i;
    u8 *p;
    u32 c;
    if (s != NULL) {
        p = s;
        c = *p++;
        while (c != TEXT_NEWLINE) {
            if (c == TEXT_END) {
                break;
            }
            if (c - TEXT_ESC < 5) { /* TEXT_ESC or a wide-glyph lead: one more byte */
                p++;
                n++;
            }
            n++;
            c = *p++;
        }
        p = s;
        if (n < max) {
            max = n;
        }
        for (i = 0; i < max; i++) {
            pen_putGlyph(*p++);
        }
    }
    return n;
}

s32 msg_emitItemName(s32 id, s32 n) {
    s32 esc = 0;
    u8 *s;
    s32 len;
    s32 i;
    u8 *t;

    if (id < ITEM_ID_KEY_FIRST) {
        t = g_itemNames + g_itemTable[id].name;
    } else if (id < ITEM_ID_CARD_FIRST) {
        t = g_keyItemNames + g_keyItemTable[id - ITEM_ID_KEY_FIRST].name;
    } else {
        t = (u8 *)card_getName(id - ITEM_ID_CARD_FIRST);
    }
    s = t;
    for (len = 0; len < 32; len++) {
        if (!esc && s[len] == TEXT_END) {
            break;
        }
        esc = !esc && s[len] == TEXT_WIDE_FIRST;
    }
    if (len < n) {
        n = len;
    }
    for (i = 0; i < n; i++) {
        pen_putGlyph(s[i]);
    }
    return len;
}

s32 msg_emitCharName(s32 arg0, s32 n) {
    u8 *name = g_gameState->gameData->characters[arg0].name;
    s32 len;
    s32 i;

    for (len = 0; len < 8; len++) {
        if (name[len] == TEXT_END) {
            break;
        }
    }
    if (len < n) {
        n = len;
    }
    for (i = 0; i < n; i++) {
        pen_putGlyph(name[i]);
    }
    return len;
}

s32 msg_emitCharNameCaps(s32 ch, s32 n) {
    s32 i;
    s32 j;
    s32 k;
    u8 *name = g_gameState->gameData->characters[ch].name;
    u32 c;
    u8 *buf;

    for (k = 0; k < 8; k++) {
        c = name[k];
        if (c - 0x30 < 0x1A) {
            c -= 0x20;
        } else if (c - 0x64 < 0x14) {
            c -= 0x14;
        } else if (c - 0x7A < 2) {
            c -= 2;
        }
        g_msgNameBuf[k] = c;
    }
    buf = g_msgNameBuf;
    for (i = 0; i < 8; i++) {
        if (buf[i] == TEXT_END) {
            break;
        }
    }
    if (i < n) {
        n = i;
    }
    for (j = 0; j < n; j++) {
        pen_putGlyph(buf[j]);
    }
    return i;
}

void msg_drawWindowContent(Window *p) {
    GameData *g = g_gameState->gameData;

    g_penOt = g->ot[g->drawBuf] + p->otz;
    pen_begin();
    pen_pushPos();
    pen_setPos(p->x + 2, p->y + 2);
    msg_drawText((MsgWindow *)p->userData);
    pen_popPos();
    pen_endFont();
}

void msg_drawChoiceCursor(Window *w) {
    GameData *g;
    s32 ofs;

    if (g_msgChoiceLine >= 0 && w->state == WIN_STATE_OPEN && (((Window *)w->userData)->animMode & 2)) {
        g = g_gameState->gameData;
        g_penOt = g->ot[g->drawBuf] + w->otz;
        pen_begin();
        pen_pushPos();
        ofs = 8;
        pen_setPos(w->x - 3, w->y + ((g_msgChoiceLine + g_msgChoice) * 14 + ofs));
        pen_drawSprite(1);
        pen_popPos();
        pen_endSmall();
    }
}

void msg_drawHereIcon(EvtObject *o) {
    POLY_FT4 *p;
    Unk80068790 *s;
    s32 pos;
    s32 n;
    s32 x;
    s32 nx;

    if (g_evtState->controlEnabled != 0 && (msg_getPadPressed() & PAD_SELECT)) {
        g_hereIconTimer = 60;
        g_hereIconActive = 1;
    }
    if (g_hereIconTimer > 0 && g_evtCameraChanged == 0) {
        n = --g_hereIconTimer;
        if (n == 0) {
            g_hereIconActive = 0;
        }
        if (g_evtMode == EVT_MODE_FIELD && !(g_gameState->gameData->flags & GD_HIDE_ALL) && n < 58) {
            s = &g_hereIconSprite;
            pen_begin();
            pen_pushPos();
            evt_setDrawPosAtObject(o, 0, 0);
            if (g_hereIconTimer == 57) {
                g_hereIconFlipped = g_penPos.s.x < 24;
            }
            pos = g_penPos.w;
            if ((pos >> 16) < 16) {
                pen_setPos(pos & 0xFFFF, 16);
            }
            pen_move(0, -((g_hereIconTimer >> 2) % 3));
            p = pen_addPolyFT4();
            p->x1 = p->x3 = p->x0;
            x = p->x0;
            if (g_hereIconFlipped) {
                nx = x + s->w;
            } else {
                nx = x - s->w;
            }
            p->x2 = p->x0 = nx;
            p->y2 = p->y3 = p->y0;
            p->y1 = p->y0 = p->y0 - s->h;
            p->u0 = p->u2 = s->u;
            p->v0 = p->v1 = s->v;
            p->u1 = p->u3 = s->u + s->w - (u8)g_hereIconFlipped;
            p->v2 = p->v3 = s->v + s->h;
            p->tpage = 0x5F;
            p->clut = s->clut;
            pen_popPos();
            pen_end();
        }
    }
}

void msg_drawPlayerBubble(void) {
    Unk80068788 *s = &g_bubbleSprite;
    s32 c = g_bubbleColor;
    s32 hw = g_bubbleHalfWidth;
    s32 h = g_bubbleHeight;
    s32 n;
    POLY_FT4 *p;
    POLY_FT4 *q;
    s32 y;
    s32 x;
    s32 d;

    if (c == 0x80) {
        n = g_bubbleVariant + 0x19;
        pen_pushPos();
        pen_move(-4, -0x10 - g_bubbleLift);
        pen_drawSprite(n);
        pen_popPos();
    }
    p = pen_addPolyFT4();
    q = pen_addPolyFT4();
    setSemiTrans(p, 1);
    setSemiTrans(q, 1);
    p->r0 = p->g0 = p->b0 = c;
    q->r0 = q->g0 = q->b0 = c;
    y = p->y0 - g_bubbleLift;
    x = p->x0;
    p->y2 = p->y3 = y;
    p->y0 = y - g_bubbleLift;
    p->x1 = p->x3 = x + hw;
    p->x2 = p->x0 = x - hw;
    p->y1 = p->y0 = y - h;
    q->y0 = q->y1 = y;
    q->y2 = q->y3 = y + h / 2;
    d = hw / 3;
    q->x0 = q->x2 = x - d;
    q->x1 = q->x3 = x + d;
    p->u0 = p->u2 = s->u;
    p->v0 = p->v1 = s->v;
    p->u1 = p->u3 = s->u + 0x18;
    p->v2 = p->v3 = s->v + 0x10;
    q->u0 = q->u2 = s->u + 8;
    /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
    if (q) {
        q->u1 = q->u3 = s->u + 0x10;
    } else {
        q->u1 = q->u3 = s->u + 0x10;
    }
    q->v0 = q->v1 = s->v + 0x10;
    q->v2 = q->v3 = s->v + 0x18;
    q->tpage = p->tpage = 0x5F;
    q->clut = p->clut = s->clut;
}

void msg_updatePlayerBubble(void) {
    EvtObject *o;

    if (g_bubbleRequestPrev == 0 && g_bubbleRequest != 0) {
        g_bubbleScript = g_bubbleVariant ? g_bubbleScriptOpenAlt : g_bubbleScriptOpen;
    } else if (g_bubbleRequestPrev != 0 && g_bubbleRequest == 0) {
        g_bubbleScript = g_bubbleScriptClose;
    }
    g_bubbleRequestPrev = g_bubbleRequest;
    g_bubbleRequest = 0;
    if (g_bubbleScript == NULL || g_evtCameraChanged != 0) {
        return;
    }
    if (g_bubbleWait > 0) {
        g_bubbleWait--;
    } else {
        switch (*g_bubbleScript) {
        case 0xFD:
            g_bubbleScript = NULL;
            return;
        case 0xFE:
            g_bubbleWait = g_bubbleScript[1];
            g_bubbleScript += 4;
            goto draw;
        case 0xFF:
            g_bubbleScript -= g_bubbleScript[1] * 4;
            break;
        }
        g_bubbleColor = g_bubbleScript[0];
        g_bubbleHalfWidth = g_bubbleScript[1];
        g_bubbleHeight = g_bubbleScript[2];
        g_bubbleLift = g_bubbleScript[3];
        g_bubbleScript += 4;
    }
    draw:
    if (!(g_gameState->gameData->flags & GD_HIDE_ALL)) {
        o = evt_getObject(g_evtState->playerUid);
        if (o != NULL && o->kind == EVT_KIND_ACTOR) {
            pen_begin();
            evt_setDrawPosAtObject(o, -2, 0);
            msg_drawPlayerBubble();
            pen_end();
        }
    }
    if (g_hereIconActive == 0) {
        g_hereIconTimer = 0;
    }
}

void msg_drawAtePrompt(void) {
    if (g_atePromptMode == ATE_PROMPT_OFF) {
        return;
    }
    if (!(g_atePromptMode & ATE_PROMPT_ALWAYS) && !evt_isControlEnabled()) {
        return;
    }
    g_atePromptTimer++;
    if (g_gameState->gameData->flags & GD_HIDE_ALL) {
        return;
    }
    pen_begin();
    pen_setPos(0x10, 200);
    if ((g_atePromptMode & ATE_PROMPT_STYLE_MASK) != ATE_PROMPT_BLINK) {
        if ((g_atePromptTimer / 44) & 1) {
            pen_drawSprite(0x80);
            pen_move(2, 0);
            pen_drawSprite(0x81);
            pen_move(2, 0);
            pen_drawSprite(0x82);
        } else {
            pen_move(-2, 0);
            pen_drawSprite(0x53);
            pen_move(2, 0);
            pen_drawSprite(0x12);
            pen_drawSprite(0x54);
        }
    } else if ((g_atePromptTimer / 15) & 1) {
        pen_drawSprite(0x1F);
        pen_move(2, 0);
        pen_drawSprite(0x20);
        pen_move(2, 0);
        pen_drawSprite(0x21);
    }
    pen_end();
}

void msg_startHereIcon(s32 arg0) {
    s32 mode;

    if (arg0 == 0) {
        g_hereIconActive = 0;
        g_hereIconTimer = 0;
        return;
    }
    mode = (g_gameState->config >> CFG_BIT_HERE_ICON) & 1;
    mode = mode == 0;
    if (arg0 >= 3 || (g_evtState->controlEnabled != 0 &&
                      (mode >= 2 || (mode == 1 && (arg0 == 2 || g_evtMsgCount >= 3 || g_evtPlayerAnimCount != 0))))) {
        g_hereIconTimer = 60;
    }
}

void msg_resetPlayerBubble(void) {
    g_bubbleScript = 0;
    g_bubbleWait = 0;
    g_bubbleRequestPrev = 0;
    g_bubbleRequest = 0;
}

void msg_showPlayerBubble(s32 a) {
    g_bubbleRequest = 1;
    g_bubbleVariant = a;
}

void msg_setAtePrompt(s32 a) {
    if (g_atePromptMode != a) {
        g_atePromptMode = a;
        g_atePromptTimer = 0x2C;
    }
}

s32 evt_modelOpField(EvtObject *o, s32 op) {
    s32 a;
    s32 b;
    u32 *p;
    GameState *gs;

    switch (op) {
    case EVT_OP_SHADOW_ON:
        *(u32 *)game_getModel(o->modelSlot) &= ~MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_OFF:
        *(u32 *)game_getModel(o->modelSlot) |= MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_SIZE:
        a = evt_getArg8();
        b = evt_getArg8();
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[o->modelSlot].sizeX = a * 14;
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[o->modelSlot].sizeZ = b * 12;
        return 0;
    case EVT_OP_SHADOW_OFFSET:
        a = evt_getArg16();
        b = evt_getArg16();
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[o->modelSlot].ofsX = a;
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[o->modelSlot].ofsZ = b;
        return 0;
    case EVT_OP_SHADOW_FIXED:
        a = evt_getArg8();
        p = (u32 *)game_getModel(o->modelSlot);
        gs = g_gameState;
        *p |= MODEL_SLOT_SHADOW_FIXED;
        gs->gameData->ctx.ovl65->fieldArea->scene->shadows[o->modelSlot].fixedAngle = a * 16;
        return 0;
    case EVT_OP_SHADOW_UNFIX:
        *(u32 *)game_getModel(o->modelSlot) &= ~MODEL_SLOT_SHADOW_FIXED;
        return 0;
    case 0x85:
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[o->modelSlot].unk0_0 = evt_getArg8();
        return 0;
    case EVT_OP_RAIN:
        fld_enableRain(0, 0, 1);
        a = evt_getArg8();
        rain_set(a, evt_getArg8());
        g_gameState->gameData->rainCount = a;
        return 0;
    default:
        return 1;
    }
    return 0;
}

s32 evt_modelOpBattle(EvtObject *o, s32 op) {
    s32 a;
    s32 b;
    u32 *p;
    GameState *gs;

    switch (op) {
    case EVT_OP_SHADOW_ON:
        *(u32 *)game_getModel(o->modelSlot) &= ~MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_OFF:
        *(u32 *)game_getModel(o->modelSlot) |= MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_SIZE:
        a = evt_getArg8();
        b = evt_getArg8();
        g_gameState->gameData->ctx.ovl65->work->shadows[o->modelSlot].sizeX = a * 14;
        g_gameState->gameData->ctx.ovl65->work->shadows[o->modelSlot].sizeZ = b * 12;
        return 0;
    case EVT_OP_SHADOW_OFFSET:
        a = evt_getArg16();
        b = evt_getArg16();
        g_gameState->gameData->ctx.ovl65->work->shadows[o->modelSlot].ofsX = a;
        g_gameState->gameData->ctx.ovl65->work->shadows[o->modelSlot].ofsZ = b;
        return 0;
    case EVT_OP_SHADOW_FIXED:
        a = evt_getArg8();
        p = (u32 *)game_getModel(o->modelSlot);
        gs = g_gameState;
        *p |= MODEL_SLOT_SHADOW_FIXED;
        gs->gameData->ctx.ovl65->work->shadows[o->modelSlot].fixedAngle = a * 16;
        return 0;
    case EVT_OP_SHADOW_UNFIX:
        *(u32 *)game_getModel(o->modelSlot) &= ~MODEL_SLOT_SHADOW_FIXED;
        return 0;
    case 0x85:
        g_gameState->gameData->ctx.ovl65->work->shadows[o->modelSlot].unk0_0 = evt_getArg8();
        return 0;
    case EVT_OP_RAIN:
        a = evt_getArg8();
        evt_getArg8();
        g_gameState->gameData->rainCount = a;
        return 0;
    default:
        return 1;
    }
    return 0;
}

s32 evt_modelOpWorld(EvtObject *o, s32 op) {
    s32 a;
    s32 b;
    u32 *p;
    GameState *gs;

    switch (op) {
    case EVT_OP_SHADOW_ON:
        *(u32 *)game_getModel(o->modelSlot) &= ~MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_OFF:
        *(u32 *)game_getModel(o->modelSlot) |= MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_SIZE:
        a = evt_getArg8();
        b = evt_getArg8();
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[o->modelSlot].sizeX = a * 14;
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[o->modelSlot].sizeZ = b * 12;
        return 0;
    case EVT_OP_SHADOW_OFFSET:
        a = evt_getArg16();
        b = evt_getArg16();
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[o->modelSlot].ofsX = a;
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[o->modelSlot].ofsZ = b;
        return 0;
    case EVT_OP_SHADOW_FIXED:
        a = evt_getArg8();
        p = (u32 *)game_getModel(o->modelSlot);
        gs = g_gameState;
        *p |= MODEL_SLOT_SHADOW_FIXED;
        gs->gameData->ctx.ovl65->worldObj->shadows[o->modelSlot].fixedAngle = a * 16;
        return 0;
    case EVT_OP_SHADOW_UNFIX:
        *(u32 *)game_getModel(o->modelSlot) &= ~MODEL_SLOT_SHADOW_FIXED;
        return 0;
    case 0x85:
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[o->modelSlot].unk0_0 = evt_getArg8();
        return 0;
    case EVT_OP_RAIN:
        a = evt_getArg8();
        wrain_set(a, evt_getArg8());
        g_gameState->gameData->rainCount = a;
        return 0;
    default:
        return 1;
    }
    return 0;
}

void evt_clearGlobalVars(void) {
    bzero(&g_evtGlobalVars, 0x800);
}

s32 rain_draw(void) {
    u8 *r = g_fieldRainPrims;
    GameData *gd;
    s32 n;

    if (g_gameState->gameData->drawBuf != 0) {
        r += 0x22B0;
    }
    if (g_fieldRain.stopping < 2) {
        rain_spawn(g_fieldRain.speed == 0xFF0 ? -0x1770 : -0xFA0);
    }
    if (g_fieldRain.stopping != 0) {
        g_fieldRain.stopping++;
    }
    n = g_fieldRain.active;
    if (n != 0) {
        fldbg_applyGeomOffset();
        gd = g_gameState->gameData;
        gte_SetRotMatrix(&gd->viewMatrix);
        gte_SetTransMatrix(&gd->viewMatrix);
        r = (u8 *)rain_drawDrops(n, gd->drawBuf, gd->ot[gd->drawBuf] + 3, (s32)r);
        fldbg_resetGeomOffset();
    }
    return (s32)r;
}

u8 *rain_init(RainNode *buf) {
    RainNode *p = buf;
    RainNode *end;
    g_fieldRain.freeList = buf;
    if (buf != NULL) {
        end = buf + 222;
        for (; p < end; p++) {
            p->next = p + 1;
        }
        p[-1].next = NULL;
    }
    g_fieldRain.active = 0;
    SetDrawTPage(&g_fieldRain.tpage, 0, 0, GetTPage(0, 3, 0, 0));
    g_fieldRain.tpage2 = g_fieldRain.tpage;
    g_fieldRain.count = 0;
    g_fieldRain.speed = 0;
    g_fieldRain.stopping = 0;
    g_fieldRainPrims = (u8 *)(buf + 222);
    return (u8 *)p + 0x4560;
}

void rain_set(s32 a, u32 b) {
    if (a != 0 && b != 0) {
        RainState *p = &g_fieldRain;

        if (a < 0x83) {
            p->count = a;
        } else {
            p->count = 0x82;
        }
    }
    g_fieldRain.speed = b * 16;
    g_fieldRain.stopping = a == 0 && b != 0;
}
