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
    D_8007BB98.lead = 0;
    D_8007BBCC = f->glyphs;
    D_8007BB98.extRange = *(s32 *)&f->extCode;
    D_8007B94C = f->unk8;
    D_8007BB98.kanjiCode = -1;
    switch (s->mode) {
    case 1:
        g_msgTable = s->gameData->ctx.ovl65->unkC->unk8;
        break;
    case 3:
        g_msgTable = (Unk8007B954 *)s->gameData->ctx.ovl65->unk8->unk14;
        break;
    case 2:
        g_msgTable = (Unk8007B954 *)s->gameData->ctx.ovl65->work->unk14;
        break;
    }
    g_msgReady = 1;
    msg_resetChoices();
}

static inline void msg_readPad_set(s32 type, u32 bits) {
    g_msgPadEvent.unk0 = type;
    g_msgPadEvent.unk4 = bits;
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
        g_msgPadPrev &= ~0xF0;
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

    if (s->mode == 2 || s->mode == 6) {
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

/* width of an embedded string, up to 0xF7 / 0xFF */
static inline s32 msg_measureLine_width(u8 *s) {
    s32 w = 0;
    s32 multi = 0;
    s32 c;
    u32 g;

    D_8007BB98.lead = 0;
    c = *s++;
    while (c != 0xF7 && c != 0xFF) {
        do {
            g = pen_getGlyph(c);
            if (g != 0) {
                w += 1 + (g >> 28);
            }
            multi = multi ? 0 : (c >= 0xFA ? c < 0xFE : 0);
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
        while (c != 0xFF) {
            switch (c) {
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
                w += msg_measureLine_width(g_gameState->gameData->characters[c - 0x10].name);
                break;
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
                w += msg_measureLine_width(g_gameState->gameData->characters[evt_getPartyChrIndex(c - 0x18)].name);
                break;
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
                msg_measureLine_name(c - 0x80);
                w += msg_measureLine_width(g_msgNameBuf);
                break;
            case 0xE:
                n = g_msgNumbers[*s++];
                if (n < 0x100) {
                    str = g_itemNames + g_itemTable[n].name;
                } else if (n < 0x200) {
                    str = g_keyItemNames + g_keyItemTable[n - 0x100].name;
                } else {
                    str = (u8 *)card_getName(n - 0x200);
                }
                w += msg_measureLine_width(str);
                break;
            case 0x40:
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
            case 0x6:
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
            case 0x70:
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

INCLUDE_RODATA("asm/us/main/nonmatchings/msg", D_80011A6C);

/* skip the choices hidden by g_msgChoiceDrawMask once the window reaches the choice line */
static inline void msg_renderText_skip(s32 id, u8 **pp) {
    s32 c;

    if (g_msgChoiceLine >= 0 && g_msgChoiceWindow == id && g_msgLine >= g_msgChoiceLine) {
        if (!(g_msgChoiceDrawMask & 1)) {
            do {
                while ((c = *(*pp)++) != 0xF7 && c != 0xFF) {
                    if (c < 0xFE) {
                        if (c >= 0xF9) {
                            (*pp)++;
                        }
                    }
                }
                if (c == 0xFF) {
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
        case 0xF8:
            pen_move(1, 0);
            break;
        case 0xF7:
            pen_popPos();
            pen_move(0, 14);
            pen_pushPos();
            g_msgLine++;
            msg_renderText_skip(w->id, &p);
            if (p >= end) {
                w->textPos = p - s;
            }
            break;
        case 0xF9:
            /* padding between the two jump tables in the original rodata */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            c = *p++;
            switch (c) {
            case 0x71:
                pen_move(0x12, 0);
                break;
            case 0x44:
                func_800512B4(g_penStackPtr[-1].vx);
            case 0x43:
                pen_move(*p++, 0);
                break;
            case 0x49:
                pen_move(0, *p++);
                break;
            case 0x4A:
                pen_move(0, -*p++);
                break;
            case 0x48:
                c = *p++;
                c |= *p++ << 8;
                msg_measureLine(&p, &c);
                pen_move((w->w - c) >> 1, 0);
                break;
            case 0x20:
            case 0x21:
            case 0x22:
            case 0x23:
            case 0x24:
            case 0x25:
            case 0x26:
            case 0x27:
            case 0x28:
                pen_setPalette(c - 0x20);
                break;
            case 0x40:
                last = end - p == 1;
                v = g_msgNumbers[*p++];
                msg_renderText_value(w, v, msg_emitNumber, last);
                break;
            case 0x6:
                c = *p++;
                last = end - p == 1;
                t = msg_renderText_text(c, *p++);
                msg_renderText_value(w, (s32)t, msg_emitString, last);
                break;
            case 0xE:
                last = end - p == 1;
                pen_setPalette(4);
                msg_renderText_value(w, g_msgNumbers[*p++], msg_emitItemName, last);
                pen_setPalette(0);
                break;
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
                msg_renderText_value(w, c - 0x10, msg_emitCharName, end == p);
                break;
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
                last = end - p == 0;
                msg_renderText_value(w, evt_getPartyChrIndex(c - 0x18), msg_emitCharName, last);
                break;
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
                msg_renderText_value(w, c - 0x80, msg_emitCharNameCaps, end == p);
                break;
            case 0x30:
            case 0x31:
            case 0x32:
            case 0x33:
            case 0x34:
            case 0x35:
            case 0x36:
            case 0x37:
            case 0x38:
            case 0x39:
                pen_move(0, 1);
                pen_drawSprite(D_8006E67C[c - 0x30]);
                pen_move(1, -1);
                break;
            case 0x60:
            case 0x61:
            case 0x62:
            case 0x63:
            case 0x64:
            case 0x65:
            case 0x66:
            case 0x67:
                pen_move(0, 1);
                if (c < 0x62) {
                    c ^= 1;
                }
                c = (g_gameState->config >> 1) & 1 ? (g_gameState->padMap >> (c * 4)) & 0xF : c & 0xF;
                if (c < 2) {
                    c ^= 1;
                }
                pen_drawSprite(D_8006E67C[c]);
                pen_move(1, -1);
                break;
            case 0x3A:
                pen_move(4, 2);
                pen_drawSprite(13);
                pen_move(-8, -1);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case 0x3B:
                pen_move(4, 7);
                pen_drawSprite(14);
                pen_move(-8, -6);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case 0x3C:
                pen_move(1, 5);
                pen_drawSprite(15);
                pen_move(-5, -4);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case 0x3D:
                pen_move(6, 5);
                pen_drawSprite(16);
                pen_move(-10, -4);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case 0x2F:
                pen_move(0, 1);
                pen_drawSprite(12);
                pen_move(1, -1);
                break;
            case 0x4B:
                pen_drawSprite(*p++);
                break;
            case 0x70:
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
            case 0x2:
            case 0xF:
            case 0x41:
                p++;
                break;
            }
            break;
        case 0xFE:
        case 0xFF:
            break;
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
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
        case 0xF9:
            /* padding between the two jump tables in the original rodata */
            __asm__(".section .rodata\n\t.word 0\n\t.section .text");
            c = *p++;
            switch (c) {
            case 0x48:
                p += 2;
                msg_measureLine(&p, &c);
                break;
            case 0x41:
                if (!(w->flags & 4)) {
                    w->waitFrames = *p++;
                } else {
                    p++;
                }
                break;
            case 0:
            case 1:
                w->flags = (w->flags & ~4) | 8;
            set26:
                w->waitFrames = *p++;
                break;
            case 2:
                w->speed = *p++;
                break;
            case 0x42:
                w->speed = D_8006E688[(g_gameState->config >> 14) & 7];
                break;
            case 0xC:
                w->flags |= 4;
                break;
            case 7:
                p += 2;
                break;
            case 8:
                tmp = D_80011A6C;
                n = g_msgLine;
                pad_getAnalogThresholds(&D_8007B99C);
                pad_setAnalogThresholds(&tmp);
                g_msgChoiceLine = n;
                w->flags |= 2;
                break;
            case 9:
                w->waitFrames = 0xFF;
            case 0x45:
                w->flags |= 0x40;
                break;
            case 0xA:
                w->flags |= 8;
                break;
            case 0xF:
                g_msgSyncTag = *p++;
                break;
            case 0x46:
                g_msgSyncTag++;
                break;
            case 6:
                p++;
            case 0xE:
            case 0x40:
                p++;
            case 0x10:
            case 0x11:
            case 0x12:
            case 0x13:
            case 0x14:
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
                if (!(w->flags & 4)) {
                    w->pendingChars = 1;
                    w->typeAccum = 0;
                }
                break;
            case 0x43:
            case 0x44:
            case 0x49:
            case 0x4A:
            case 0x4B:
            case 0x70:
            skip1:
                p++;
                break;
            }
            break;
        case 0xFE:
        case 0xFF:
            w->waitFrames = 0xFF;
            break;
        case 0xF7:
            g_msgLine++;
            break;
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
            p++;
        default:
            if (!(w->flags & 4)) {
                w->typeAccum -= 8;
            }
            break;
        }
    }
    w->textPos = (w->waitFrames != 0 ? start : p) - e;
    if (w->waitFrames == 0xFF) {
        w->flags &= ~4;
    }
    w->typeAccum += w->speed;
}

void msg_stepControl(MsgWindow *w, u8 *e) {
    u8 *p = e + w->textPos;
    s32 c;

    c = *p++;
    switch (c) {
    case 0xF9:
        c = *p++;
        switch (c) {
        case 0x41:
            w->textPos = p + 1 - e;
            break;
        case 1:
            msg_closeWindow_inline(w);
            break;
        case 0:
            w->pageStart = p + 1 - e;
            w->flags &= ~8;
            break;
        }
        break;
    case 0xFF:
        msg_closeWindow_inline(w);
        break;
    case 0xFE:
        w->pageStart = p - e;
        break;
    }
}

void msg_drawText(MsgWindow *w) {
    u8 *e = NULL;
    s32 i = w->msgId;

    if (i >= 0 && i < g_msgTable->unk0) {
        e = (u8 *)g_msgTable->unk4;
        e += *(u16 *)&((u32 *)e)[(g_evtMode == 2 || g_evtMode == 4) ? i : i * 2 + 1];
    }
    if (e != NULL) {
        pen_move(((w->w - w->textWidth) >> 1) + 4, 2);
        pen_pushPos();
        if (w->textPos < w->pageStart) {
            w->textPos = w->pageStart;
        }
        D_8007BB98.lead = 0;
        g_msgChoiceDrawMask = g_msgChoiceEnabled;
        msg_renderText(w, e);
        if (w->waitFrames == 0) {
            if (w->pendingChars == 0) {
                msg_advanceText(w, e);
            } else if (w->flags & 4) {
                w->pendingChars = 0;
                w->typeAccum = 8;
            }
        } else {
            w->waitFrames -= (w->waitFrames != 0xFF);
            if (w->flags & 4) {
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

    if ((g_evtMode == 1 && (pad & 0xA0000)) || (g_evtMode != 1 && (pad & 0x20000))) {
        f = w->flags;
        if (!(f & 0x40)) {
            if (w->waitFrames == 0xFF) {
                w->waitFrames = 1;
                if (g_msgChoiceLine >= 0) {
                    g_gameState->gameData->soundHook(0xD000, 0x67, 0x800000, 0x80, 0x7F);
                }
                return 1;
            }
            if (!(f & 8)) {
                w->flags = f | 4;
                return 1;
            }
        }
    }
    if (g_msgChoiceLine >= 0) {
        if (g_msgChoiceWindow != w->id) {
            return 0;
        }
        if (pad & 0x10) {
            if (g_msgChoice > 0) {
                g_msgChoice--;
            } else {
                g_msgChoice = g_msgChoiceCount - 1;
            }
            g_gameState->gameData->soundHook(0xD000, 0x67, 0x800000, 0x80, 0x7F);
        } else if (pad & 0x40) {
            if (g_msgChoice < g_msgChoiceCount - 1) {
                g_msgChoice++;
            } else {
                g_msgChoice = 0;
            }
            g_gameState->gameData->soundHook(0xD000, 0x67, 0x800000, 0x80, 0x7F);
        } else if (!(pad & 0x10000) || g_msgChoiceCancel < 0) {
            return 0;
        } else {
            g_msgChoice = g_msgChoiceCancel;
            g_gameState->gameData->soundHook(0xD000, 0x65, 0x800000, 0x80, 0x7F);
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
INCLUDE_RODATA("asm/us/main/nonmatchings/msg", D_80011EFC);

INCLUDE_RODATA("asm/us/main/nonmatchings/msg", D_80011F10);

void msg_open(s32 msg, s32 id, WinLayout *l, s32 flags) {
    MsgWindow *w;
    RECT r;
    s32 style;
    s32 n;
    u8 *e;

    e = NULL;
    if (msg >= 0 && msg < g_msgTable->unk0) {
        e = (u8 *)g_msgTable->unk4;
        e += *(u16 *)&((u32 *)e)[(g_evtMode == 2 || g_evtMode == 4) ? msg : msg * 2 + 1];
    }
    if (e == NULL) {
        return;
    }
    w = msg_open_init(id, l);
    w->onInput = msg_input;
    w->waitFrames = 0;
    w->id = id;
    w->msgId = msg;
    w->speed = D_8006E688[(g_gameState->config >> 14) & 7];
    w->pageStart = 0;
    w->typeAccum = 8;
    w->pendingChars = 0;
    w->tailOffset = l->tailOffset;
    w->tailMode = l->tailMode;
    w->textWidth = l->w;
    w->flags |= 0x10;
    if (!(l->flags & 1)) {
        g_msgFocus = w;
    }
    r.x = w->x;
    r.y = w->y;
    r.w = w->w + 3;
    r.h = w->h + 3;
    style = 4;
    if (!(l->tailMode & 0x10)) {
        style = 3;
        if (l->tailOffset == 0 && !(flags & 4)) {
            style = 2;
        }
    }
    w->window = win_open(&r, 2, style, id);
    if (w->window != NULL) {
        w->window->userData = (s32)w;
        win_setDrawContent(w->window, msg_drawWindowContent);
        win_setDrawOverlay(w->window, msg_drawChoiceCursor);
        if (l->tailOffset != 0 && !(flags & 4)) {
            win_setTail(w->window, l->tailMode & 3, l->tailOffset);
        }
        if (D_8007BB90 != 0) {
            win_setOpenAnim(w->window, 3);
        }
        if (flags & 0x40) {
            win_setTitle(w->window, (s32)D_80011EFC);
        } else if (flags & 8) {
            win_setTitle(w->window, (s32)D_80011F10);
        }
    }
    if (!(l->flags & 1)) {
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
        if (e == NULL || e[0] != 0xF9 || e[1] != 4) {
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

    if (i >= 0 && i < g_msgTable->unk0) {
        e = (u8 *)g_msgTable->unk4;
        v = ((u32 *)e)[g_evtMode == 4 ? i : i * 2];
        if (out != NULL) {
            out->w = ((v >> 16) & 0xFFF) + 9;
            out->h = (g_evtMode == 4 ? (u32)v >> 28 : ((MsgEntry8 *)e)[i].lines) * 14 + 6;
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
        D_8007BB90 = 0;
        g_msgLayout.anchor = 0;
        g_msgLayout.flags = 0;
        *x = 0;
        *y = 0;
        *z = 0;
        while (!done) {
            if (*p++ != 0xF9) {
                break;
            }
            switch (*p++) {
            case 0x7:
                g_msgChoiceEnabled = -1;
                g_msgChoiceCount = *p++;
                g_msgChoiceCancel = *p++ - 1;
                break;
            case 0x4F:
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
            case 0x3:
                g_msgLayout.x = *p++;
                g_msgLayout.x |= *p++ << 8;
                g_msgLayout.y = *p++;
                g_msgLayout.anchor = 0xFF;
                break;
            case 0x50:
            case 0x51:
            case 0x52:
            case 0x53:
            case 0x54:
            case 0x55:
            case 0x58:
            case 0x59:
            case 0x5A:
            case 0x5B:
            case 0x5C:
                g_msgLayout.anchor = p[-1];
                break;
            case 0x5F:
                *x = *p;
                *x |= *(s8 *)++p << 8;
                *y = *++p;
                *y |= *(s8 *)++p << 8;
                *z = *++p;
                *z |= *(s8 *)++p << 8;
                p++;
                break;
            case 0xB:
                D_8007BB90 = 1;
                break;
            case 0x45:
                g_msgLayout.flags = 1;
                break;
            case 0xD:
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
                        while (*p++ != 0xFF) {
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
    g_msgPadEvent.unk0 = 0;
    g_msgPadEvent.unk4 = 0;
}

void msg_update(void) {
    MsgWindow *p = msg_getFocus();
    s32 a;
    msg_readPad();
    if (g_msgPadEvent.unk0 < 3) {
        if (g_msgPadEvent.unk0 > 0) {
            while (p != NULL) {
                a = g_msgPadEvent.unk4;
                if (g_msgPadEvent.unk0 == 2) {
                    a |= 0x80000000;
                }
                if (p->onInput(p, a) != 0) {
                    break;
                }
                p = p->parent;
            }
        }
    }
    g_msgPadEvent.unk0 = 0;
    g_msgPadEvent.unk4 = 0;
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
        if (p->open != 0 && (p->flags & 2)) {
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

void func_8004ED3C(void) {
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
        e->flags = 2;
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

    if (i >= 0 && i < g_msgTable->unk0) {
        e = (u8 *)g_msgTable->unk4;
        v = ((u32 *)e)[g_evtMode == 4 ? i : i * 2];
        if (out != NULL) {
            out->w = ((v >> 16) & 0xFFF) + 9;
            out->h = (g_evtMode == 4 ? (u32)v >> 28 : ((MsgEntry8 *)e)[i].lines) * 14 + 6;
        }
        e += v & 0xFFFF;
    }
    return e;
}

void *msg_getText(s32 i) {
    u8 *e = NULL;

    if (i >= 0 && i < g_msgTable->unk0) {
        e = (u8 *)g_msgTable->unk4;
        e += *(u16 *)&((u32 *)e)[(g_evtMode == 2 || g_evtMode == 4) ? i : i * 2 + 1];
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
        while (c != 0xF7) {
            if (c == 0xFF) {
                break;
            }
            if (c - 0xF9 < 5) {
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

    if (id < 0x100) {
        t = g_itemNames + g_itemTable[id].name;
    } else if (id < 0x200) {
        t = g_keyItemNames + g_keyItemTable[id - 0x100].name;
    } else {
        t = (u8 *)card_getName(id - 0x200);
    }
    s = t;
    for (len = 0; len < 32; len++) {
        if (!esc && s[len] == 0xFF) {
            break;
        }
        esc = !esc && s[len] == 0xFA;
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
        if (name[len] == 0xFF) {
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
        if (buf[i] == 0xFF) {
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

    if (g_msgChoiceLine >= 0 && w->state == 3 && (((Window *)w->userData)->animMode & 2)) {
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
        if (g_evtMode == 1 && !(g_gameState->gameData->flags & GD_HIDE_ALL) && n < 58) {
            s = &D_80068790;
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
    Unk80068788 *s = &D_80068788;
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
        g_bubbleScript = g_bubbleVariant ? D_8006E6D8 : D_8006E690;
    } else if (g_bubbleRequestPrev != 0 && g_bubbleRequest == 0) {
        g_bubbleScript = D_8006E6F0;
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
        if (o != NULL && o->kind == 4) {
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
    if (g_atePromptMode == 0) {
        return;
    }
    if (!(g_atePromptMode & 4) && !evt_isControlEnabled()) {
        return;
    }
    g_atePromptTimer++;
    if (g_gameState->gameData->flags & GD_HIDE_ALL) {
        return;
    }
    pen_begin();
    pen_setPos(0x10, 200);
    if ((g_atePromptMode & 3) != 2) {
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
    mode = (g_gameState->config >> 17) & 1;
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
        *(u32 *)game_getModel(o->modelSlot) &= ~0x10;
        return 0;
    case EVT_OP_SHADOW_OFF:
        *(u32 *)game_getModel(o->modelSlot) |= 0x10;
        return 0;
    case EVT_OP_SHADOW_SIZE:
        a = evt_getArg8();
        b = evt_getArg8();
        g_gameState->gameData->ctx.ovl65->unkC->unk14->unk2C[o->modelSlot].unk0_8 = a * 14;
        g_gameState->gameData->ctx.ovl65->unkC->unk14->unk2C[o->modelSlot].unk0_20 = b * 12;
        return 0;
    case 0x82:
        a = evt_getArg16();
        b = evt_getArg16();
        g_gameState->gameData->ctx.ovl65->unkC->unk14->unk2C[o->modelSlot].unk4_12 = a;
        g_gameState->gameData->ctx.ovl65->unkC->unk14->unk2C[o->modelSlot].unk4_22 = b;
        return 0;
    case EVT_OP_SHADOW_FIXED:
        a = evt_getArg8();
        p = (u32 *)game_getModel(o->modelSlot);
        gs = g_gameState;
        *p |= 0x20;
        gs->gameData->ctx.ovl65->unkC->unk14->unk2C[o->modelSlot].unk4_0 = a * 16;
        return 0;
    case EVT_OP_SHADOW_UNFIX:
        *(u32 *)game_getModel(o->modelSlot) &= ~0x20;
        return 0;
    case 0x85:
        g_gameState->gameData->ctx.ovl65->unkC->unk14->unk2C[o->modelSlot].unk0_0 = evt_getArg8();
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
        *(u32 *)game_getModel(o->modelSlot) &= ~0x10;
        return 0;
    case EVT_OP_SHADOW_OFF:
        *(u32 *)game_getModel(o->modelSlot) |= 0x10;
        return 0;
    case EVT_OP_SHADOW_SIZE:
        a = evt_getArg8();
        b = evt_getArg8();
        g_gameState->gameData->ctx.ovl65->work->unk28[o->modelSlot].unk0_8 = a * 14;
        g_gameState->gameData->ctx.ovl65->work->unk28[o->modelSlot].unk0_20 = b * 12;
        return 0;
    case 0x82:
        a = evt_getArg16();
        b = evt_getArg16();
        g_gameState->gameData->ctx.ovl65->work->unk28[o->modelSlot].unk4_12 = a;
        g_gameState->gameData->ctx.ovl65->work->unk28[o->modelSlot].unk4_22 = b;
        return 0;
    case EVT_OP_SHADOW_FIXED:
        a = evt_getArg8();
        p = (u32 *)game_getModel(o->modelSlot);
        gs = g_gameState;
        *p |= 0x20;
        gs->gameData->ctx.ovl65->work->unk28[o->modelSlot].unk4_0 = a * 16;
        return 0;
    case EVT_OP_SHADOW_UNFIX:
        *(u32 *)game_getModel(o->modelSlot) &= ~0x20;
        return 0;
    case 0x85:
        g_gameState->gameData->ctx.ovl65->work->unk28[o->modelSlot].unk0_0 = evt_getArg8();
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
        *(u32 *)game_getModel(o->modelSlot) &= ~0x10;
        return 0;
    case EVT_OP_SHADOW_OFF:
        *(u32 *)game_getModel(o->modelSlot) |= 0x10;
        return 0;
    case EVT_OP_SHADOW_SIZE:
        a = evt_getArg8();
        b = evt_getArg8();
        ((Unk11C0Slot *)g_gameState->gameData->ctx.ovl65->unk8->unk1C)[o->modelSlot].unk0_8 = a * 14;
        ((Unk11C0Slot *)g_gameState->gameData->ctx.ovl65->unk8->unk1C)[o->modelSlot].unk0_20 = b * 12;
        return 0;
    case 0x82:
        a = evt_getArg16();
        b = evt_getArg16();
        ((Unk11C0Slot *)g_gameState->gameData->ctx.ovl65->unk8->unk1C)[o->modelSlot].unk4_12 = a;
        ((Unk11C0Slot *)g_gameState->gameData->ctx.ovl65->unk8->unk1C)[o->modelSlot].unk4_22 = b;
        return 0;
    case EVT_OP_SHADOW_FIXED:
        a = evt_getArg8();
        p = (u32 *)game_getModel(o->modelSlot);
        gs = g_gameState;
        *p |= 0x20;
        ((Unk11C0Slot *)gs->gameData->ctx.ovl65->unk8->unk1C)[o->modelSlot].unk4_0 = a * 16;
        return 0;
    case EVT_OP_SHADOW_UNFIX:
        *(u32 *)game_getModel(o->modelSlot) &= ~0x20;
        return 0;
    case 0x85:
        ((Unk11C0Slot *)g_gameState->gameData->ctx.ovl65->unk8->unk1C)[o->modelSlot].unk0_0 = evt_getArg8();
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
