#include "ff9.h"

void msg_init(void) {
    GameState *gs;
    Font *curFont;

    if (g_msgKeepWindows != 0) {
        g_msgKeepWindows = 0;
    } else {
        msg_resetWindows();
        win_closeAll();
    }
    gs = g_gameState;
    g_msgReady = 0;
    curFont = gs->gameData->font;
    g_penGlyphDecode.lead = 0;
    g_penGlyphs = curFont->glyphs;
    g_penGlyphDecode.extRange = *(s32 *)&curFont->extCode;
    g_msgFontGlyphCount = curFont->glyphCount;
    g_penGlyphDecode.kanjiCode = -1;
    switch (gs->mode) {
    case GS_MODE_FIELD:
        g_msgTable = gs->gameData->ctx.ovl65->fieldArea->msgTable;
        break;
    case GS_MODE_WORLD:
        g_msgTable = (Unk8007B954 *)gs->gameData->ctx.ovl65->worldObj->areaData;
        break;
    case GS_MODE_BATTLE:
        g_msgTable = (Unk8007B954 *)gs->gameData->ctx.ovl65->work->texts;
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
    s32 isRepeat = 0;
    u32 cur = g_padPortHeld[0] & PAD_GAME_BITS;
    u32 x;

    x = ~cur & g_msgPadPrev;
    g_msgPadPressed = cur & ~g_msgPadPrev;
    g_msgPadReleased = x;
    if (x != 0) {
        msg_readPad_set(MSG_PAD_RELEASE, x);
    }
    if (g_msgPadPrev != cur) {
        g_msgPadRepeatTimer = 16;
    }
    g_msgPadRepeatTimer = g_gameState->vsyncMode != 0 ? g_msgPadRepeatTimer - 2 : g_msgPadRepeatTimer - 1;
    if (g_msgPadRepeatTimer < 0 && !(g_msgPadRepeatTimer & 3)) {
        isRepeat = 1;
        g_msgPadPrev &= ~(PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT);
    }
    x = cur & ~g_msgPadPrev;
    if (x != 0) {
        s32 evType = MSG_PAD_PRESS;
        if (isRepeat) {
            evType = MSG_PAD_REPEAT;
        }
        msg_readPad_set(evType, x);
    }
    g_msgPadPrev = cur;
}

void msg_beginDraw(void) {
    GameState *gs = g_gameState;
    GameData *gd;
    u8 **pp;
    u32 *tbl;
    u8 *cur;

    if (gs->mode == GS_MODE_BATTLE || gs->mode == 6) {
        g_msgDrawBufMode = 1;
        pp = &g_msgBattlePktPtr;
        tbl = &g_msgBattlePktBufs[gs->gameData->drawBuf];
        g_penPktPtr = pp;
        g_msgDrawPktStart = g_msgBattlePktPtr = (u8 *)tbl;
    } else {
        g_msgDrawBufMode = 0x4000;
        cur = g_primPtr;
        g_penPktPtr = &g_primPtr;
        g_msgDrawPktStart = cur;
    }
    gd = g_gameState->gameData;
    g_penOt = gd->ot[gd->drawBuf];
}

/* width of an embedded string, up to TEXT_NEWLINE / TEXT_END */
static inline s32 msg_measureLine_width(u8 *str) {
    s32 w = 0;
    s32 multi = 0;
    s32 code;
    u32 glyph;

    g_penGlyphDecode.lead = 0;
    code = *str++;
    while (code != TEXT_NEWLINE && code != TEXT_END) {
        do {
            glyph = pen_getGlyph(code);
            if (glyph != 0) {
                w += 1 + GLYPH_ADVANCE(glyph);
            }
            multi = multi ? 0 : (code >= TEXT_WIDE_FIRST ? code <= TEXT_WIDE_LAST : 0);
            code = *str++;
        } while (multi);
    }
    return w;
}

/* copy character ch's name into g_msgNameBuf, converting the case */
static inline void msg_measureLine_name(s32 ch) {
    u8 *name = g_gameState->gameData->characters[ch].name;
    s32 k;
    u32 code;

    for (k = 0; k < 8; k++) {
        code = name[k];
        if (code - TEXT_CODE_LOWER_A < 26) {
            code -= TEXT_CODE_LOWER_A - TEXT_CODE_UPPER_A;
        } else if (code - TEXT_CODE_ACCENT_LOWER < 20) {
            code -= TEXT_CODE_ACCENT_LOWER - TEXT_CODE_ACCENT_UPPER;
        } else if (code - (TEXT_CODE_C_CEDILLA + 2) < 2) {
            code -= 2;
        }
        g_msgNameBuf[k] = code;
    }
}

void msg_measureLine(u8 **pp, s32 *pw) {
    u8 *text;
    u8 *bankText;
    u8 *tbl;
    u8 *str;
    u8 *ofsEntry;
    s32 w;
    s32 code;
    s32 num, k;
    u32 bank, i;

    if (pp != NULL && pw != NULL) {
        text = *pp;
        w = *pw;
        code = *text++;
        while (code != TEXT_END) {
            switch (code) {
            case TEXT_CHAR_NAME + 0:
            case TEXT_CHAR_NAME + 1:
            case TEXT_CHAR_NAME + 2:
            case TEXT_CHAR_NAME + 3:
            case TEXT_CHAR_NAME + 4:
            case TEXT_CHAR_NAME + 5:
            case TEXT_CHAR_NAME + 6:
            case TEXT_CHAR_NAME + 7:
                w += msg_measureLine_width(g_gameState->gameData->characters[code - TEXT_CHAR_NAME].name);
                break;
            case TEXT_PARTY_NAME + 0:
            case TEXT_PARTY_NAME + 1:
            case TEXT_PARTY_NAME + 2:
            case TEXT_PARTY_NAME + 3:
                w += msg_measureLine_width(g_gameState->gameData->characters[evt_getPartyChrIndex(code - TEXT_PARTY_NAME)].name);
                break;
            case TEXT_CHAR_NAME_CAPS + 0:
            case TEXT_CHAR_NAME_CAPS + 1:
            case TEXT_CHAR_NAME_CAPS + 2:
            case TEXT_CHAR_NAME_CAPS + 3:
            case TEXT_CHAR_NAME_CAPS + 4:
            case TEXT_CHAR_NAME_CAPS + 5:
            case TEXT_CHAR_NAME_CAPS + 6:
            case TEXT_CHAR_NAME_CAPS + 7:
                msg_measureLine_name(code - TEXT_CHAR_NAME_CAPS);
                w += msg_measureLine_width(g_msgNameBuf);
                break;
            case TEXT_MSG_ITEM_NAME:
                num = g_msgNumbers[*text++];
                if (num < ITEM_ID_KEY_FIRST) {
                    str = g_itemNames + g_itemTable[num].name;
                } else if (num < ITEM_ID_CARD_FIRST) {
                    str = g_keyItemNames + g_keyItemTable[num - ITEM_ID_KEY_FIRST].name;
                } else {
                    str = (u8 *)card_getName(num - ITEM_ID_CARD_FIRST);
                }
                w += msg_measureLine_width(str);
                break;
            case TEXT_MSG_NUMBER:
                num = g_msgNumbers[*text++];
                if (num < 0) {
                    num = -num;
                    w += 9;
                }
                do {
                    num /= 10;
                    w += 9;
                } while (num != 0);
                break;
            case TEXT_BANK_TEXT:
                code = *text++;
                bank = (code >> 4) & 3;
                i = code & 7;
                bankText = NULL;
                if (bank < 4 && i < 8 && (tbl = g_msgTextBanks[bank]) != NULL) {
                    k = g_msgNumbers[i];
                    if (k < *tbl) {
                        tbl++;
                        k <<= 1;
                        ofsEntry = tbl + k;
                        bankText = tbl + (ofsEntry[0] | (ofsEntry[1] << 8));
                    }
                }
                w += msg_measureLine_width(bankText);
                break;
            case TEXT_FLAG_ICON:
                if ((u32)g_msgNumbers[0] & (1 << *text++)) {
                    w += 0x19;
                }
                break;
            }
            code = *text++;
        }
        *pp = text;
        *pw = w + 9;
    }
}

INCLUDE_RODATA("asm/us/main/nonmatchings/msg", g_msgChoiceAnalogThresholds);

/* skip the choices hidden by g_msgChoiceDrawMask once the window reaches the choice line */
static inline void msg_renderText_skip(s32 id, u8 **pp) {
    s32 code;

    if (g_msgChoiceLine >= 0 && g_msgChoiceWindow == id && g_msgLine >= g_msgChoiceLine) {
        if (!(g_msgChoiceDrawMask & 1)) {
            do {
                while ((code = *(*pp)++) != TEXT_NEWLINE && code != TEXT_END) {
                    if (code < TEXT_PAGE) {
                        if (code >= TEXT_ESC) {
                            (*pp)++;
                        }
                    }
                }
                if (code == TEXT_END) {
                    (*pp)--;
                    break;
                }
                g_msgChoiceDrawMask >>= 1;
            } while (!(g_msgChoiceDrawMask & 1));
        }
        g_msgChoiceDrawMask >>= 1;
    }
}

/* typewriter: advance the shown character count of an embedded value of charCount characters */
static inline void msg_renderText_type(MsgWindow *w, s32 charCount) {
    w->typeAccum += w->speed;
    w->pendingChars += w->typeAccum >> 3;
    w->typeAccum &= 7;
    if (w->pendingChars >= charCount) {
        w->pendingChars = 0;
    }
}

/* text slot of message bank bank (g_msgTextBanks) */
static inline u8 *msg_renderText_text(u32 bank, u32 slot) {
    u8 *text = NULL;
    u8 *tbl;
    u8 *ofsEntry;
    s32 k;

    if (bank < 4 && slot < 8 && (tbl = g_msgTextBanks[bank]) != NULL) {
        k = g_msgNumbers[slot];
        if (k < *tbl) {
            tbl++;
            k <<= 1;
            ofsEntry = tbl + k;
            text = tbl + (ofsEntry[0] | (ofsEntry[1] << 8));
        }
    }
    return text;
}

/* print an embedded value, typewriter-style when it ends the text */
static inline void msg_renderText_value(MsgWindow *w, s32 arg, s32 (*fn)(), s32 last) {
    if (last && w->pendingChars != 0) {
        msg_renderText_type(w, fn(arg, w->pendingChars));
    } else {
        fn(arg, 0xFF);
    }
}

void msg_renderText(MsgWindow *w, u8 *msgText) {
    u8 *cur;
    u8 *end;
    s32 code;
    s32 last;
    s32 num;
    u8 *bankText;
    u8 *tbl;
    u32 slot;
    s32 k;
    u8 *ofsEntry;

    g_msgLine = 0;
    end = msgText + w->textPos;
    cur = msgText + w->pageStart;
    msg_renderText_skip(w->id, &cur);
    while (cur < end) {
        code = *cur++;
        switch (code) {
        case TEXT_SPACE_1PX:
            pen_move(1, 0);
            break;
        case TEXT_NEWLINE:
            pen_popPos();
            pen_move(0, 14);
            pen_pushPos();
            g_msgLine++;
            msg_renderText_skip(w->id, &cur);
            if (cur >= end) {
                w->textPos = cur - msgText;
            }
            break;
        case TEXT_ESC:
            /* padding between the two jump tables in the original rodata */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            code = *cur++;
            switch (code) {
            case TEXT_TAB:
                pen_move(0x12, 0);
                break;
            case TEXT_X_SET:
                pen_setX(g_penStackPtr[-1].vx);
            case TEXT_X_ADD:
                pen_move(*cur++, 0);
                break;
            case TEXT_Y_DOWN:
                pen_move(0, *cur++);
                break;
            case TEXT_Y_UP:
                pen_move(0, -*cur++);
                break;
            case TEXT_CENTER:
                code = *cur++;
                code |= *cur++ << 8;
                msg_measureLine(&cur, &code);
                pen_move((w->w - code) >> 1, 0);
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
                pen_setPalette(code - TEXT_COLOR);
                break;
            case TEXT_MSG_NUMBER:
                last = end - cur == 1;
                num = g_msgNumbers[*cur++];
                msg_renderText_value(w, num, msg_emitNumber, last);
                break;
            case TEXT_BANK_TEXT:
                code = *cur++;
                last = end - cur == 1;
                bankText = msg_renderText_text(code, *cur++);
                msg_renderText_value(w, (s32)bankText, msg_emitString, last);
                break;
            case TEXT_MSG_ITEM_NAME:
                last = end - cur == 1;
                pen_setPalette(4);
                msg_renderText_value(w, g_msgNumbers[*cur++], msg_emitItemName, last);
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
                msg_renderText_value(w, code - TEXT_CHAR_NAME, msg_emitCharName, end == cur);
                break;
            case TEXT_PARTY_NAME + 0:
            case TEXT_PARTY_NAME + 1:
            case TEXT_PARTY_NAME + 2:
            case TEXT_PARTY_NAME + 3:
                last = end - cur == 0;
                msg_renderText_value(w, evt_getPartyChrIndex(code - TEXT_PARTY_NAME), msg_emitCharName, last);
                break;
            case TEXT_CHAR_NAME_CAPS + 0:
            case TEXT_CHAR_NAME_CAPS + 1:
            case TEXT_CHAR_NAME_CAPS + 2:
            case TEXT_CHAR_NAME_CAPS + 3:
            case TEXT_CHAR_NAME_CAPS + 4:
            case TEXT_CHAR_NAME_CAPS + 5:
            case TEXT_CHAR_NAME_CAPS + 6:
            case TEXT_CHAR_NAME_CAPS + 7:
                msg_renderText_value(w, code - TEXT_CHAR_NAME_CAPS, msg_emitCharNameCaps, end == cur);
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
                pen_drawSprite(g_msgIconSprites[code - TEXT_BTN_CIRCLE]);
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
                if (code < TEXT_PAD_BTN + 2) {
                    code ^= 1;
                }
                code = (g_gameState->config >> CFG_BIT_CONTROLLER) & 1 ? (g_gameState->padMap >> (code * 4)) & 0xF : code & 0xF;
                if (code < 2) {
                    code ^= 1;
                }
                pen_drawSprite(g_msgIconSprites[code]);
                pen_move(1, -1);
                break;
            case TEXT_DPAD_UP:
                pen_move(4, 2);
                pen_drawSprite(UI_SPRITE_DPAD_UP);
                pen_move(-8, -1);
                pen_drawSprite(UI_SPRITE_DPAD);
                pen_move(1, -1);
                break;
            case TEXT_DPAD_DOWN:
                pen_move(4, 7);
                pen_drawSprite(UI_SPRITE_DPAD_DOWN);
                pen_move(-8, -6);
                pen_drawSprite(UI_SPRITE_DPAD);
                pen_move(1, -1);
                break;
            case TEXT_DPAD_LEFT:
                pen_move(1, 5);
                pen_drawSprite(UI_SPRITE_DPAD_LEFT);
                pen_move(-5, -4);
                pen_drawSprite(UI_SPRITE_DPAD);
                pen_move(1, -1);
                break;
            case TEXT_DPAD_RIGHT:
                pen_move(6, 5);
                pen_drawSprite(UI_SPRITE_DPAD_RIGHT);
                pen_move(-10, -4);
                pen_drawSprite(UI_SPRITE_DPAD);
                pen_move(1, -1);
                break;
            case TEXT_DPAD:
                pen_move(0, 1);
                pen_drawSprite(UI_SPRITE_DPAD);
                pen_move(1, -1);
                break;
            case TEXT_SPRITE:
                pen_drawSprite(*cur++);
                break;
            case TEXT_FLAG_ICON:
                if ((u32)g_msgNumbers[0] & (1 << *cur++)) {
                    pen_move(6, 3);
                    pen_drawSprite(UI_SPRITE_NEW_BLINK);
                    if (VSync(-1) % 48 >= 16) {
                        pen_setLastSemiTrans();
                    }
                    pen_move(-24, -3);
                    pen_drawSprite(UI_SPRITE_NEW);
                }
                break;
            case TEXT_SPEED:
            case TEXT_SYNC_TAG:
            case TEXT_WAIT:
                cur++;
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
            pen_putGlyph(code);
            code = *cur++;
        default:
            pen_putGlyph(code);
            break;
        }
    }
}

void msg_advanceText(MsgWindow *w, u8 *msgText) {
    u8 *cur;
    s32 code;
    u8 *start;
    Unk4 thresholds;
    s32 choiceLine;

    cur = msgText + w->textPos;
    start = cur;
    while (w->waitFrames == 0 && w->pendingChars == 0 && w->typeAccum >= 8) {
        start = cur;
        code = *cur++;
        switch (code) {
        case TEXT_ESC:
            /* padding between the two jump tables in the original rodata */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            code = *cur++;
            switch (code) {
            case TEXT_CENTER:
                cur += 2;
                msg_measureLine(&cur, &code);
                break;
            case TEXT_WAIT:
                if (!(w->flags & MSGW_SKIP)) {
                    w->waitFrames = *cur++;
                } else {
                    cur++;
                }
                break;
            case TEXT_WAIT_PAGE:
            case TEXT_WAIT_CLOSE:
                w->flags = (w->flags & ~MSGW_SKIP) | MSGW_NO_SKIP;
            set26:
                w->waitFrames = *cur++;
                break;
            case TEXT_SPEED:
                w->speed = *cur++;
                break;
            case TEXT_SPEED_DEFAULT:
                w->speed = g_msgTextSpeeds[(g_gameState->config >> CFG_SHIFT_MSG_SPEED) & 7];
                break;
            case TEXT_SKIP:
                w->flags |= MSGW_SKIP;
                break;
            case TEXT_CHOICES:
                cur += 2;
                break;
            case TEXT_CHOICE_START:
                thresholds = g_msgChoiceAnalogThresholds;
                choiceLine = g_msgLine;
                pad_getAnalogThresholds(&g_msgSavedAnalogThresholds);
                pad_setAnalogThresholds(&thresholds);
                g_msgChoiceLine = choiceLine;
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
                g_msgSyncTag = *cur++;
                break;
            case TEXT_SYNC_TAG_NEXT:
                g_msgSyncTag++;
                break;
            case TEXT_BANK_TEXT:
                cur++;
            case TEXT_MSG_ITEM_NAME:
            case TEXT_MSG_NUMBER:
                cur++;
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
                cur++;
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
            cur++;
        default:
            if (!(w->flags & MSGW_SKIP)) {
                w->typeAccum -= 8;
            }
            break;
        }
    }
    w->textPos = (w->waitFrames != 0 ? start : cur) - msgText;
    if (w->waitFrames == MSGW_WAIT_INPUT) {
        w->flags &= ~MSGW_SKIP;
    }
    w->typeAccum += w->speed;
}

void msg_stepControl(MsgWindow *w, u8 *msgText) {
    u8 *cur = msgText + w->textPos;
    s32 code;

    code = *cur++;
    switch (code) {
    case TEXT_ESC:
        code = *cur++;
        switch (code) {
        case TEXT_WAIT:
            w->textPos = cur + 1 - msgText;
            break;
        case TEXT_WAIT_CLOSE:
            msg_closeWindow_inline(w);
            break;
        case TEXT_WAIT_PAGE:
            w->pageStart = cur + 1 - msgText;
            w->flags &= ~MSGW_NO_SKIP;
            break;
        }
        break;
    case TEXT_END:
        msg_closeWindow_inline(w);
        break;
    case TEXT_PAGE:
        w->pageStart = cur - msgText;
        break;
    }
}

void msg_drawText(MsgWindow *w) {
    u8 *msgText = NULL;
    s32 i = w->msgId;

    if (i >= 0 && i < g_msgTable->count) {
        msgText = (u8 *)g_msgTable->offsets;
        msgText += *(u16 *)&((u32 *)msgText)[(g_evtMode == EVT_MODE_BATTLE || g_evtMode == EVT_MODE_BATTLE_END) ? i : i * 2 + 1];
    }
    if (msgText != NULL) {
        pen_move(((w->w - w->textWidth) >> 1) + 4, 2);
        pen_pushPos();
        if (w->textPos < w->pageStart) {
            w->textPos = w->pageStart;
        }
        g_penGlyphDecode.lead = 0;
        g_msgChoiceDrawMask = g_msgChoiceEnabled;
        msg_renderText(w, msgText);
        if (w->waitFrames == 0) {
            if (w->pendingChars == 0) {
                msg_advanceText(w, msgText);
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
                msg_stepControl(w, msgText);
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
    u8 winFlags;

    if ((g_evtMode == EVT_MODE_FIELD && (pad & (BTN_CROSS | BTN_SQUARE))) || (g_evtMode != EVT_MODE_FIELD && (pad & BTN_CROSS))) {
        winFlags = w->flags;
        if (!(winFlags & MSGW_NO_INPUT)) {
            if (w->waitFrames == MSGW_WAIT_INPUT) {
                w->waitFrames = 1;
                if (g_msgChoiceLine >= 0) {
                    g_gameState->gameData->soundHook(SND_SE_PLAY, SE_CURSOR, SND_SE_GROUP_MENU, SND_PAN_CENTER, SND_VOL_MAX);
                }
                return 1;
            }
            if (!(winFlags & MSGW_NO_SKIP)) {
                w->flags = winFlags | MSGW_SKIP;
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
            g_gameState->gameData->soundHook(SND_SE_PLAY, SE_CURSOR, SND_SE_GROUP_MENU, SND_PAN_CENTER, SND_VOL_MAX);
        } else if (pad & PAD_DOWN) {
            if (g_msgChoice < g_msgChoiceCount - 1) {
                g_msgChoice++;
            } else {
                g_msgChoice = 0;
            }
            g_gameState->gameData->soundHook(SND_SE_PLAY, SE_CURSOR, SND_SE_GROUP_MENU, SND_PAN_CENTER, SND_VOL_MAX);
        } else if (!(pad & BTN_CIRCLE) || g_msgChoiceCancel < 0) {
            return 0;
        } else {
            g_msgChoice = g_msgChoiceCancel;
            g_gameState->gameData->soundHook(SND_SE_PLAY, SE_CANCEL, SND_SE_GROUP_MENU, SND_PAN_CENTER, SND_VOL_MAX);
        }
        return 1;
    }
    return 0;
}

static inline MsgWindow *msg_open_init(s32 id, WinLayout *layout) {
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
    if (layout->x != 0) {
        w->x = layout->x;
        w->y = (u8)layout->y;
    } else {
        w->x = 0xA0 - (layout->w >> 1);
        w->y = 0x78 - (layout->h >> 1);
    }
    w->w = layout->w;
    w->h = layout->h;
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

void msg_open(s32 msg, s32 id, WinLayout *layout, s32 flags) {
    MsgWindow *w;
    RECT rect;
    s32 style;
    s32 defChoice;
    u8 *msgText;

    msgText = NULL;
    if (msg >= 0 && msg < g_msgTable->count) {
        msgText = (u8 *)g_msgTable->offsets;
        msgText += *(u16 *)&((u32 *)msgText)[(g_evtMode == EVT_MODE_BATTLE || g_evtMode == EVT_MODE_BATTLE_END) ? msg : msg * 2 + 1];
    }
    if (msgText == NULL) {
        return;
    }
    w = msg_open_init(id, layout);
    w->onInput = msg_input;
    w->waitFrames = 0;
    w->id = id;
    w->msgId = msg;
    w->speed = g_msgTextSpeeds[(g_gameState->config >> CFG_SHIFT_MSG_SPEED) & 7];
    w->pageStart = 0;
    w->typeAccum = 8;
    w->pendingChars = 0;
    w->tailOffset = layout->tailOffset;
    w->tailMode = layout->tailMode;
    w->textWidth = layout->w;
    w->flags |= MSGW_MESSAGE;
    if (!(layout->flags & WINLAYOUT_NO_FOCUS)) {
        g_msgFocus = w;
    }
    rect.x = w->x;
    rect.y = w->y;
    rect.w = w->w + 3;
    rect.h = w->h + 3;
    style = WIN_STYLE_FRAMELESS;
    if (!(layout->tailMode & MSG_TAIL_FRAMELESS)) {
        style = WIN_STYLE_BALLOON;
        if (layout->tailOffset == 0 && !(flags & MSG_FLAG_NO_TAIL)) {
            style = WIN_STYLE_BEVEL;
        }
    }
    w->window = win_open(&rect, 2, style, id);
    if (w->window != NULL) {
        w->window->userData = (s32)w;
        win_setDrawContent(w->window, msg_drawWindowContent);
        win_setDrawOverlay(w->window, msg_drawChoiceCursor);
        if (layout->tailOffset != 0 && !(flags & MSG_FLAG_NO_TAIL)) {
            win_setTail(w->window, layout->tailMode & MSG_TAIL_DIR, layout->tailOffset);
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
    if (!(layout->flags & WINLAYOUT_NO_FOCUS)) {
        g_msgChoiceLine = -1;
        defChoice = g_msgChoiceDefault;
        g_msgChoiceDefault = 0;
        g_msgChoiceWindow = id;
        g_msgChoice = defChoice;
    }
    g_evtMsgCount++;
    msg_startHereIcon(0);
}

void msg_resetChoices(void) {
    s32 i;
    u8 *msgText;

    g_msgChoice = 0;
    g_msgChoiceMaskReq = -1;
    g_msgChoiceLine = -1;
    for (i = 0; i < 4; i++) {
        msgText = msg_drawText_msg(i);
        if (msgText == NULL || msgText[0] != TEXT_ESC || msgText[1] != TEXT_BANK) {
            break;
        }
        g_msgTextBanks[i] = msgText + 2;
    }
    g_msgChoiceDefault = 0;
    g_evtMsgCount = 0;
}

/* Inlined copy of msg_getEntry (message i, its size into out). */
static inline u8 *msg_getEntry_inline(s32 i, WinLayout *out) {
    u8 *msgText = NULL;
    s32 packed;

    if (i >= 0 && i < g_msgTable->count) {
        msgText = (u8 *)g_msgTable->offsets;
        packed = ((u32 *)msgText)[g_evtMode == EVT_MODE_BATTLE_END ? i : i * 2];
        if (out != NULL) {
            out->w = MSG_ENTRY_W(packed) + 9;
            out->h = (g_evtMode == EVT_MODE_BATTLE_END ? MSG_ENTRY_LINES(packed) : ((MsgEntry8 *)msgText)[i].lines) * 14 + 6;
        }
        msgText += MSG_ENTRY_OFS(packed);
    }
    return msgText;
}

WinLayout *msg_getLayout(s32 id, s32 *x, s32 *y, s32 *z) {
    u8 *cur;
    s32 lineWidth;
    s32 done;
    s32 choiceNo;
    s32 choiceVal, i; /* visible choice count, then the cancel choice index (separate locals don't match) */
    s32 choiceBits;
    s32 cnt;

    cur = msg_getEntry_inline(id, &g_msgLayout);
    if (cur != NULL) {
        done = 0;
        g_msgLayoutAltOpen = 0;
        g_msgLayout.anchor = WINLAYOUT_ANCHOR_NONE;
        g_msgLayout.flags = 0;
        *x = 0;
        *y = 0;
        *z = 0;
        while (!done) {
            if (*cur++ != TEXT_ESC) {
                break;
            }
            switch (*cur++) {
            case TEXT_CHOICES:
                g_msgChoiceEnabled = -1;
                g_msgChoiceCount = *cur++;
                g_msgChoiceCancel = *cur++ - 1;
                break;
            case TEXT_CHOICES_MASKED:
                g_msgChoiceEnabled = g_msgChoiceMaskReq;
                choiceVal = i = *cur;
                choiceBits = g_msgChoiceMaskReq;
                while (--i != -1) {
                    choiceVal -= (choiceBits ^ 1) & 1;
                    choiceBits >>= 1;
                }
                g_msgChoiceCount = choiceVal;
                g_msgLayout.h -= (*cur++ - choiceVal) * 14;
                choiceVal = -1;
                choiceBits = g_msgChoiceEnabled;
                cnt = *cur++;
                while (--cnt != -1) {
                    choiceVal += choiceBits & 1;
                    choiceBits >>= 1;
                }
                if (choiceVal >= g_msgChoiceCount) {
                    choiceVal = g_msgChoiceCount - 1;
                }
                g_msgChoiceCancel = choiceVal;
                break;
            case TEXT_WIN_POS:
                g_msgLayout.x = *cur++;
                g_msgLayout.x |= *cur++ << 8;
                g_msgLayout.y = *cur++;
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
                g_msgLayout.anchor = cur[-1];
                break;
            case TEXT_BALLOON_OFS:
                *x = *cur;
                *x |= *(s8 *)++cur << 8;
                *y = *++cur;
                *y |= *(s8 *)++cur << 8;
                *z = *++cur;
                *z |= *(s8 *)++cur << 8;
                cur++;
                break;
            case TEXT_NO_OPEN_ANIM:
                g_msgLayoutAltOpen = 1;
                break;
            case TEXT_NO_INPUT:
                g_msgLayout.flags = WINLAYOUT_NO_FOCUS;
                break;
            case TEXT_LINE_WIDTHS:
                for (;;) {
                    lineWidth = *cur++;
                    lineWidth |= *cur++ << 8;
                    if (lineWidth == 0xFFFF) break;
                    choiceNo = (lineWidth >> 12) & 0xF;
                    lineWidth &= 0xFFF;
                    if (choiceNo == 0 || ((g_msgChoiceEnabled >> (choiceNo - 1)) & 1)) {
                        msg_measureLine(&cur, &lineWidth);
                        if (g_msgLayout.w < lineWidth) {
                            g_msgLayout.w = lineWidth;
                        }
                    } else {
                        while (*cur++ != TEXT_END) {
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

void msg_setOtz(s32 otz) {
    GameData *gd = g_gameState->gameData;
    g_penOt = gd->ot[gd->drawBuf] + otz;
}

s32 msg_getPartyMemberId(s32 i) {
    Character *chr = g_gameState->gameData->party[i];
    if (chr == NULL) {
        return -1;
    }
    return chr->info.b.id;
}

void msg_resetPad(void) {
    g_msgPadPrev = 0;
    g_msgPadEvent.type = 0;
    g_msgPadEvent.buttons = 0;
}

void msg_update(void) {
    MsgWindow *win = msg_getFocus();
    s32 pad;
    msg_readPad();
    if (g_msgPadEvent.type <= MSG_PAD_REPEAT) {
        if (g_msgPadEvent.type > 0) {
            while (win != NULL) {
                pad = g_msgPadEvent.buttons;
                if (g_msgPadEvent.type == MSG_PAD_REPEAT) {
                    pad |= MSG_INPUT_REPEAT;
                }
                if (win->onInput(win, pad) != 0) {
                    break;
                }
                win = win->parent;
            }
        }
    }
    g_msgPadEvent.type = 0;
    g_msgPadEvent.buttons = 0;
    g_msgPadCopyDst = g_msgPadCopySrc;
}

u32 msg_getPadHeld(void) {
    return g_padPortHeld[0] & PAD_GAME_BITS;
}

u32 msg_getPadPressed(void) {
    return g_msgPadPressed & PAD_GAME_BITS;
}

u32 msg_getPadReleased(void) {
    return g_msgPadReleased & PAD_GAME_BITS;
}

void msg_onTextEnd(void) {
}

void msg_resetWindows(void) {
    MsgWindow *win;
    for (win = g_msgWindows; win < g_msgWindows + 10; win++) {
        win->open = 0;
    }
    g_msgFocus = NULL;
}

void msg_closeAll(void) {
    MsgWindow *win;
    MsgWindow *child;
    for (win = g_msgWindows; win < g_msgWindows + 8; win++) {
        if (win != NULL && win->open != 0) {
            win->open = 0;
            if (msg_getFocus() == win) {
                msg_setFocus(win->parent);
            }
            for (child = g_msgWindows; child < g_msgWindows + 10; child++) {
                if (child->open != 0 && child->parent == win) {
                    child->parent = NULL;
                }
            }
        }
    }
}

void msg_setFocus(MsgWindow *win) {
    g_msgFocus = win;
}

MsgWindow *msg_getFocus(void) {
    return g_msgFocus;
}

void msg_close(s32 i) {
    msg_closeWindow_inline(&g_msgWindows[i]);
}

MsgWindow *msg_getLastModal(void) {
    MsgWindow *win;
    MsgWindow *modal = NULL;
    for (win = g_msgWindows; win < g_msgWindows + 10; win++) {
        if (win->open != 0 && (win->flags & MSGW_CHOICE)) {
            modal = win;
        }
    }
    return modal;
}

s32 msg_overlapsRect(WinLayout *layout) {
    MsgWindow *win;
    for (win = g_msgWindows; win < g_msgWindows + 8; win++) {
        if (win->open != 0 && layout->x < win->x + win->w && win->x < layout->x + layout->w &&
            (u8)layout->y < win->y + win->h && win->y < (u8)layout->y + layout->h) {
            return 1;
        }
    }
    return 0;
}

void msg_setNumber(u32 i, s32 value) {
    if (i < 8) {
        g_msgNumbers[i] = value;
    }
}

s16 msg_isOpen(s32 i) {
    return g_msgWindows[i].open;
}

s32 msg_getChoiceIndex(void) {
    s32 i = -1;
    s32 enabledMask = g_msgChoiceEnabled;
    s32 choiceLeft = g_msgChoice;
    for (; i < 15;) {
        if (choiceLeft < 0) {
            break;
        }
        choiceLeft -= enabledMask & 1;
        i++;
        enabledMask >>= 1;
    }
    return i;
}

void msg_setChoiceMask(s32 choiceMask, s32 defaultIdx) {
    s32 choice;
    g_msgChoiceMaskReq = choiceMask;
    choice = -1;
    for (; defaultIdx >= 0; defaultIdx--) {
        if (choiceMask == 0) {
            break;
        }
        choice += choiceMask & 1;
        choiceMask >>= 1;
    }
    if (choice < 0) {
        choice = 0;
    }
    g_msgChoiceDefault = choice;
}

void msg_clearWindowOtz(void) {
    MsgWindow *win;
    for (win = g_msgWindows; win < g_msgWindows + 10; win++) {
        if (win->open != 0 && win->window != NULL) {
            win->window->otz = 0;
        }
    }
}

Window *msg_getWindow(s32 i) {
    MsgWindow *win = &g_msgWindows[i];
    Window *box = NULL;
    if (win->open != 0) {
        box = win->window;
    }
    return box;
}

MsgWindow *msg_allocWindow(WinLayout *layout, MsgWindow *parent, s32 idx, s32 flag) {
    MsgWindow *win = &g_msgWindows[idx];
    win->open = 1;
    win->parent = parent;
    win->onInput = 0;
    win->unk10 = 0;
    win->y = 0;
    win->x = 0;
    win->id = 0;
    if (layout->x != 0) {
        win->x = layout->x;
        win->y = (u8)layout->y;
    } else {
        win->x = 160 - (layout->w >> 1);
        win->y = 120 - (layout->h >> 1);
    }
    win->w = layout->w;
    win->h = layout->h;
    if (flag != 0) {
        win->flags = MSGW_CHOICE;
    } else {
        win->flags = 0;
    }
    win->msgId = 0;
    win->textPos = 0;
    win->tailOffset = 0;
    win->tailMode = 0;
    win->window = NULL;
    return win;
}

void msg_closeWindow(MsgWindow *win) {
    msg_closeWindow_inline(win);
}

void msg_keepWindowsOnInit(void) {
    g_msgKeepWindows = 1;
}

u8 *msg_getEntry(s32 i, WinLayout *out) {
    return msg_getEntry_inline(i, out);
}

void *msg_getText(s32 i) {
    u8 *msgText = NULL;

    if (i >= 0 && i < g_msgTable->count) {
        msgText = (u8 *)g_msgTable->offsets;
        msgText += *(u16 *)&((u32 *)msgText)[(g_evtMode == EVT_MODE_BATTLE || g_evtMode == EVT_MODE_BATTLE_END) ? i : i * 2 + 1];
    }
    return msgText;
}

s32 msg_emitNumber(s32 value, s32 maxChars) {
    s32 place;
    s32 digits;
    s32 i;
    s32 quot;
    s32 ch;

    if (value < 0) {
        value = 0;
    }
    place = 10;
    digits = 1;
    while (value >= place) {
        place *= 10;
        digits++;
    }
    for (i = 0; i < maxChars; i++) {
        place /= 10;
        if (place == 0) {
            break;
        }
        quot = value / place;
        if (place < 2 || quot != 0) {
            ch = quot % 10;
        } else {
            ch = 0xF;
        }
        pen_putGlyph(ch);
    }
    return digits;
}

s32 msg_emitString(u8 *str, s32 max) {
    s32 len = 0;
    s32 i;
    u8 *p;
    u32 code;
    if (str != NULL) {
        p = str;
        code = *p++;
        while (code != TEXT_NEWLINE) {
            if (code == TEXT_END) {
                break;
            }
            if (code - TEXT_ESC < 5) { /* TEXT_ESC or a wide-glyph lead: one more byte */
                p++;
                len++;
            }
            len++;
            code = *p++;
        }
        p = str;
        if (len < max) {
            max = len;
        }
        for (i = 0; i < max; i++) {
            pen_putGlyph(*p++);
        }
    }
    return len;
}

s32 msg_emitItemName(s32 id, s32 maxChars) {
    s32 esc = 0;
    u8 *itemName;
    s32 len;
    s32 i;
    u8 *str;

    if (id < ITEM_ID_KEY_FIRST) {
        str = g_itemNames + g_itemTable[id].name;
    } else if (id < ITEM_ID_CARD_FIRST) {
        str = g_keyItemNames + g_keyItemTable[id - ITEM_ID_KEY_FIRST].name;
    } else {
        str = (u8 *)card_getName(id - ITEM_ID_CARD_FIRST);
    }
    itemName = str;
    for (len = 0; len < 32; len++) {
        if (!esc && itemName[len] == TEXT_END) {
            break;
        }
        esc = !esc && itemName[len] == TEXT_WIDE_FIRST;
    }
    if (len < maxChars) {
        maxChars = len;
    }
    for (i = 0; i < maxChars; i++) {
        pen_putGlyph(itemName[i]);
    }
    return len;
}

s32 msg_emitCharName(s32 chr, s32 maxChars) {
    u8 *name = g_gameState->gameData->characters[chr].name;
    s32 len;
    s32 i;

    for (len = 0; len < 8; len++) {
        if (name[len] == TEXT_END) {
            break;
        }
    }
    if (len < maxChars) {
        maxChars = len;
    }
    for (i = 0; i < maxChars; i++) {
        pen_putGlyph(name[i]);
    }
    return len;
}

s32 msg_emitCharNameCaps(s32 ch, s32 maxChars) {
    s32 i;
    s32 j;
    s32 k;
    u8 *name = g_gameState->gameData->characters[ch].name;
    u32 code;
    u8 *capsName;

    for (k = 0; k < 8; k++) {
        code = name[k];
        if (code - TEXT_CODE_LOWER_A < 26) {
            code -= TEXT_CODE_LOWER_A - TEXT_CODE_UPPER_A;
        } else if (code - TEXT_CODE_ACCENT_LOWER < 20) {
            code -= TEXT_CODE_ACCENT_LOWER - TEXT_CODE_ACCENT_UPPER;
        } else if (code - (TEXT_CODE_C_CEDILLA + 2) < 2) {
            code -= 2;
        }
        g_msgNameBuf[k] = code;
    }
    capsName = g_msgNameBuf;
    for (i = 0; i < 8; i++) {
        if (capsName[i] == TEXT_END) {
            break;
        }
    }
    if (i < maxChars) {
        maxChars = i;
    }
    for (j = 0; j < maxChars; j++) {
        pen_putGlyph(capsName[j]);
    }
    return i;
}

void msg_drawWindowContent(Window *win) {
    GameData *gd = g_gameState->gameData;

    g_penOt = gd->ot[gd->drawBuf] + win->otz;
    pen_begin();
    pen_pushPos();
    pen_setPos(win->x + 2, win->y + 2);
    msg_drawText((MsgWindow *)win->userData);
    pen_popPos();
    pen_endFont();
}

void msg_drawChoiceCursor(Window *w) {
    GameData *gd;
    s32 ofs;

    if (g_msgChoiceLine >= 0 && w->state == WIN_STATE_OPEN && (((Window *)w->userData)->animMode & 2)) {
        gd = g_gameState->gameData;
        g_penOt = gd->ot[gd->drawBuf] + w->otz;
        pen_begin();
        pen_pushPos();
        ofs = 8;
        pen_setPos(w->x - 3, w->y + ((g_msgChoiceLine + g_msgChoice) * 14 + ofs));
        pen_drawSprite(UI_SPRITE_HAND);
        pen_popPos();
        pen_endSmall();
    }
}

void msg_drawHereIcon(EvtObject *obj) {
    POLY_FT4 *poly;
    Unk80068790 *sprite;
    s32 pos;
    s32 timeLeft;
    s32 x;
    s32 nx;

    if (g_evtState->controlEnabled != 0 && (msg_getPadPressed() & PAD_SELECT)) {
        g_hereIconTimer = 60;
        g_hereIconActive = 1;
    }
    if (g_hereIconTimer > 0 && g_evtCameraChanged == 0) {
        timeLeft = --g_hereIconTimer;
        if (timeLeft == 0) {
            g_hereIconActive = 0;
        }
        if (g_evtMode == EVT_MODE_FIELD && !(g_gameState->gameData->flags & GD_HIDE_ALL) && timeLeft < 58) {
            sprite = &g_hereIconSprite;
            pen_begin();
            pen_pushPos();
            evt_setDrawPosAtObject(obj, 0, 0);
            if (g_hereIconTimer == 57) {
                g_hereIconFlipped = g_penPos.s.x < 24;
            }
            pos = g_penPos.w;
            if ((pos >> 16) < 16) {
                pen_setPos(pos & 0xFFFF, 16);
            }
            pen_move(0, -((g_hereIconTimer >> 2) % 3));
            poly = pen_addPolyFT4();
            poly->x1 = poly->x3 = poly->x0;
            x = poly->x0;
            if (g_hereIconFlipped) {
                nx = x + sprite->w;
            } else {
                nx = x - sprite->w;
            }
            poly->x2 = poly->x0 = nx;
            poly->y2 = poly->y3 = poly->y0;
            poly->y1 = poly->y0 = poly->y0 - sprite->h;
            poly->u0 = poly->u2 = sprite->u;
            poly->v0 = poly->v1 = sprite->v;
            poly->u1 = poly->u3 = sprite->u + sprite->w - (u8)g_hereIconFlipped;
            poly->v2 = poly->v3 = sprite->v + sprite->h;
            poly->tpage = getTPage(0, 2, VRAM_UI_SHEET_X, VRAM_UI_SHEET_Y);
            poly->clut = sprite->clut;
            pen_popPos();
            pen_end();
        }
    }
}

void msg_drawPlayerBubble(void) {
    Unk80068788 *sprite = &g_bubbleSprite;
    s32 shade = g_bubbleColor;
    s32 hw = g_bubbleHalfWidth;
    s32 h = g_bubbleHeight;
    s32 spriteId;
    POLY_FT4 *bubble;
    POLY_FT4 *tail;
    s32 y;
    s32 x;
    s32 tailHw;

    if (shade == 0x80) {
        spriteId = g_bubbleVariant + 0x19;
        pen_pushPos();
        pen_move(-4, -0x10 - g_bubbleLift);
        pen_drawSprite(spriteId);
        pen_popPos();
    }
    bubble = pen_addPolyFT4();
    tail = pen_addPolyFT4();
    setSemiTrans(bubble, 1);
    setSemiTrans(tail, 1);
    bubble->r0 = bubble->g0 = bubble->b0 = shade;
    tail->r0 = tail->g0 = tail->b0 = shade;
    y = bubble->y0 - g_bubbleLift;
    x = bubble->x0;
    bubble->y2 = bubble->y3 = y;
    bubble->y0 = y - g_bubbleLift;
    bubble->x1 = bubble->x3 = x + hw;
    bubble->x2 = bubble->x0 = x - hw;
    bubble->y1 = bubble->y0 = y - h;
    tail->y0 = tail->y1 = y;
    tail->y2 = tail->y3 = y + h / 2;
    tailHw = hw / 3;
    tail->x0 = tail->x2 = x - tailHw;
    tail->x1 = tail->x3 = x + tailHw;
    bubble->u0 = bubble->u2 = sprite->u;
    bubble->v0 = bubble->v1 = sprite->v;
    bubble->u1 = bubble->u3 = sprite->u + 0x18;
    bubble->v2 = bubble->v3 = sprite->v + 0x10;
    tail->u0 = tail->u2 = sprite->u + 8;
    /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
    if (tail) {
        tail->u1 = tail->u3 = sprite->u + 0x10;
    } else {
        tail->u1 = tail->u3 = sprite->u + 0x10;
    }
    tail->v0 = tail->v1 = sprite->v + 0x10;
    tail->v2 = tail->v3 = sprite->v + 0x18;
    tail->tpage = bubble->tpage = getTPage(0, 2, VRAM_UI_SHEET_X, VRAM_UI_SHEET_Y);
    tail->clut = bubble->clut = sprite->clut;
}

void msg_updatePlayerBubble(void) {
    EvtObject *obj;

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
        obj = evt_getObject(g_evtState->playerUid);
        if (obj != NULL && obj->kind == EVT_KIND_ACTOR) {
            pen_begin();
            evt_setDrawPosAtObject(obj, -2, 0);
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
            pen_drawSprite(UI_SPRITE_ATE_TITLE);
            pen_move(2, 0);
            pen_drawSprite(UI_SPRITE_ATE_TITLE + 1);
            pen_move(2, 0);
            pen_drawSprite(UI_SPRITE_ATE_TITLE + 2);
        } else {
            pen_move(-2, 0);
            pen_drawSprite(UI_SPRITE_ATE_PRESS);
            pen_move(2, 0);
            pen_drawSprite(UI_SPRITE_SELECT);
            pen_drawSprite(UI_SPRITE_ATE_TO_SEE);
        }
    } else if ((g_atePromptTimer / 15) & 1) {
        pen_drawSprite(UI_SPRITE_ATE_TITLE_GREY);
        pen_move(2, 0);
        pen_drawSprite(UI_SPRITE_ATE_TITLE_GREY + 1);
        pen_move(2, 0);
        pen_drawSprite(UI_SPRITE_ATE_TITLE_GREY + 2);
    }
    pen_end();
}

void msg_startHereIcon(s32 request) {
    s32 mode;

    if (request == 0) {
        g_hereIconActive = 0;
        g_hereIconTimer = 0;
        return;
    }
    mode = (g_gameState->config >> CFG_BIT_HERE_ICON) & 1;
    mode = mode == 0;
    if (request >= 3 || (g_evtState->controlEnabled != 0 &&
                      (mode >= 2 || (mode == 1 && (request == 2 || g_evtMsgCount >= 3 || g_evtPlayerAnimCount != 0))))) {
        g_hereIconTimer = 60;
    }
}

void msg_resetPlayerBubble(void) {
    g_bubbleScript = 0;
    g_bubbleWait = 0;
    g_bubbleRequestPrev = 0;
    g_bubbleRequest = 0;
}

void msg_showPlayerBubble(s32 variant) {
    g_bubbleRequest = 1;
    g_bubbleVariant = variant;
}

void msg_setAtePrompt(s32 mode) {
    if (g_atePromptMode != mode) {
        g_atePromptMode = mode;
        g_atePromptTimer = 0x2C;
    }
}

s32 evt_modelOpField(EvtObject *obj, s32 op) {
    s32 operand0; /* script operands: their meaning depends on the opcode (per-case locals don't match) */
    s32 operand1;
    u32 *slotFlags;
    GameState *gs;

    switch (op) {
    case EVT_OP_SHADOW_ON:
        *(u32 *)game_getModel(obj->modelSlot) &= ~MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_OFF:
        *(u32 *)game_getModel(obj->modelSlot) |= MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_SIZE:
        operand0 = evt_getArg8();
        operand1 = evt_getArg8();
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[obj->modelSlot].sizeX = operand0 * 14;
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[obj->modelSlot].sizeZ = operand1 * 12;
        return 0;
    case EVT_OP_SHADOW_OFFSET:
        operand0 = evt_getArg16();
        operand1 = evt_getArg16();
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[obj->modelSlot].ofsX = operand0;
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[obj->modelSlot].ofsZ = operand1;
        return 0;
    case EVT_OP_SHADOW_FIXED:
        operand0 = evt_getArg8();
        slotFlags = (u32 *)game_getModel(obj->modelSlot);
        gs = g_gameState;
        *slotFlags |= MODEL_SLOT_SHADOW_FIXED;
        gs->gameData->ctx.ovl65->fieldArea->scene->shadows[obj->modelSlot].fixedAngle = operand0 * 16;
        return 0;
    case EVT_OP_SHADOW_UNFIX:
        *(u32 *)game_getModel(obj->modelSlot) &= ~MODEL_SLOT_SHADOW_FIXED;
        return 0;
    case 0x85:
        g_gameState->gameData->ctx.ovl65->fieldArea->scene->shadows[obj->modelSlot].unk0_0 = evt_getArg8();
        return 0;
    case EVT_OP_RAIN:
        fld_enableRain(0, 0, 1);
        operand0 = evt_getArg8();
        rain_set(operand0, evt_getArg8());
        g_gameState->gameData->rainCount = operand0;
        return 0;
    default:
        return 1;
    }
    return 0;
}

s32 evt_modelOpBattle(EvtObject *obj, s32 op) {
    s32 operand0; /* script operands: their meaning depends on the opcode (per-case locals don't match) */
    s32 operand1;
    u32 *slotFlags;
    GameState *gs;

    switch (op) {
    case EVT_OP_SHADOW_ON:
        *(u32 *)game_getModel(obj->modelSlot) &= ~MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_OFF:
        *(u32 *)game_getModel(obj->modelSlot) |= MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_SIZE:
        operand0 = evt_getArg8();
        operand1 = evt_getArg8();
        g_gameState->gameData->ctx.ovl65->work->shadows[obj->modelSlot].sizeX = operand0 * 14;
        g_gameState->gameData->ctx.ovl65->work->shadows[obj->modelSlot].sizeZ = operand1 * 12;
        return 0;
    case EVT_OP_SHADOW_OFFSET:
        operand0 = evt_getArg16();
        operand1 = evt_getArg16();
        g_gameState->gameData->ctx.ovl65->work->shadows[obj->modelSlot].ofsX = operand0;
        g_gameState->gameData->ctx.ovl65->work->shadows[obj->modelSlot].ofsZ = operand1;
        return 0;
    case EVT_OP_SHADOW_FIXED:
        operand0 = evt_getArg8();
        slotFlags = (u32 *)game_getModel(obj->modelSlot);
        gs = g_gameState;
        *slotFlags |= MODEL_SLOT_SHADOW_FIXED;
        gs->gameData->ctx.ovl65->work->shadows[obj->modelSlot].fixedAngle = operand0 * 16;
        return 0;
    case EVT_OP_SHADOW_UNFIX:
        *(u32 *)game_getModel(obj->modelSlot) &= ~MODEL_SLOT_SHADOW_FIXED;
        return 0;
    case 0x85:
        g_gameState->gameData->ctx.ovl65->work->shadows[obj->modelSlot].unk0_0 = evt_getArg8();
        return 0;
    case EVT_OP_RAIN:
        operand0 = evt_getArg8();
        evt_getArg8();
        g_gameState->gameData->rainCount = operand0;
        return 0;
    default:
        return 1;
    }
    return 0;
}

s32 evt_modelOpWorld(EvtObject *obj, s32 op) {
    s32 operand0; /* script operands: their meaning depends on the opcode (per-case locals don't match) */
    s32 operand1;
    u32 *slotFlags;
    GameState *gs;

    switch (op) {
    case EVT_OP_SHADOW_ON:
        *(u32 *)game_getModel(obj->modelSlot) &= ~MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_OFF:
        *(u32 *)game_getModel(obj->modelSlot) |= MODEL_SLOT_NO_SHADOW;
        return 0;
    case EVT_OP_SHADOW_SIZE:
        operand0 = evt_getArg8();
        operand1 = evt_getArg8();
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[obj->modelSlot].sizeX = operand0 * 14;
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[obj->modelSlot].sizeZ = operand1 * 12;
        return 0;
    case EVT_OP_SHADOW_OFFSET:
        operand0 = evt_getArg16();
        operand1 = evt_getArg16();
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[obj->modelSlot].ofsX = operand0;
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[obj->modelSlot].ofsZ = operand1;
        return 0;
    case EVT_OP_SHADOW_FIXED:
        operand0 = evt_getArg8();
        slotFlags = (u32 *)game_getModel(obj->modelSlot);
        gs = g_gameState;
        *slotFlags |= MODEL_SLOT_SHADOW_FIXED;
        gs->gameData->ctx.ovl65->worldObj->shadows[obj->modelSlot].fixedAngle = operand0 * 16;
        return 0;
    case EVT_OP_SHADOW_UNFIX:
        *(u32 *)game_getModel(obj->modelSlot) &= ~MODEL_SLOT_SHADOW_FIXED;
        return 0;
    case 0x85:
        g_gameState->gameData->ctx.ovl65->worldObj->shadows[obj->modelSlot].unk0_0 = evt_getArg8();
        return 0;
    case EVT_OP_RAIN:
        operand0 = evt_getArg8();
        wrain_set(operand0, evt_getArg8());
        g_gameState->gameData->rainCount = operand0;
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
    u8 *primBuf = g_fieldRainPrims;
    GameData *gd;
    s32 dropCount;

    if (g_gameState->gameData->drawBuf != 0) {
        primBuf += 0x22B0;
    }
    if (g_fieldRain.stopping < 2) {
        rain_spawn(g_fieldRain.speed == 0xFF0 ? -0x1770 : -0xFA0);
    }
    if (g_fieldRain.stopping != 0) {
        g_fieldRain.stopping++;
    }
    dropCount = g_fieldRain.active;
    if (dropCount != 0) {
        fldbg_applyGeomOffset();
        gd = g_gameState->gameData;
        gte_SetRotMatrix(&gd->viewMatrix);
        gte_SetTransMatrix(&gd->viewMatrix);
        primBuf = (u8 *)rain_drawDrops(dropCount, gd->drawBuf, gd->ot[gd->drawBuf] + 3, (s32)primBuf);
        fldbg_resetGeomOffset();
    }
    return (s32)primBuf;
}

u8 *rain_init(RainNode *nodes) {
    RainNode *node = nodes;
    RainNode *end;
    g_fieldRain.freeList = nodes;
    if (nodes != NULL) {
        end = nodes + 222;
        for (; node < end; node++) {
            node->next = node + 1;
        }
        node[-1].next = NULL;
    }
    g_fieldRain.active = 0;
    SetDrawTPage(&g_fieldRain.tpage, 0, 0, GetTPage(0, 3, 0, 0));
    g_fieldRain.tpage2 = g_fieldRain.tpage;
    g_fieldRain.count = 0;
    g_fieldRain.speed = 0;
    g_fieldRain.stopping = 0;
    g_fieldRainPrims = (u8 *)(nodes + 222);
    return (u8 *)node + 0x4560;
}

void rain_set(s32 count, u32 speed) {
    if (count != 0 && speed != 0) {
        RainState *rain = &g_fieldRain;

        if (count < 0x83) {
            rain->count = count;
        } else {
            rain->count = 0x82;
        }
    }
    g_fieldRain.speed = speed * 16;
    g_fieldRain.stopping = count == 0 && speed != 0;
}
