#include "ff9.h"

void msg_init(void) {
    GameState *s;
    Unk7D4 *f;

    if (D_8007B950 != 0) {
        D_8007B950 = 0;
    } else {
        msg_resetWindows();
        win_closeAll();
    }
    s = g_gameState;
    D_8007BBD4 = 0;
    f = s->unk1C->unk7D4;
    D_8007BB98.unk0 = 0;
    D_8007BBCC = f->unkC;
    D_8007BB98.unk4 = *(s32 *)&f->unk4;
    D_8007B94C = f->unk8;
    D_8007BB98.unk8 = -1;
    switch (s->mode) {
    case 1:
        g_msgTable = s->unk1C->unk8E4.ovl65->unkC->unk8;
        break;
    case 3:
        g_msgTable = (Unk8007B954 *)s->unk1C->unk8E4.ovl65->unk8->unk14;
        break;
    case 2:
        g_msgTable = (Unk8007B954 *)s->unk1C->unk8E4.ovl65->unk11C0->unk14;
        break;
    }
    D_8007BBD4 = 1;
    msg_resetChoices();
}

static inline void msg_readPad_set(s32 type, u32 bits) {
    D_8007B988.unk0 = type;
    D_8007B988.unk4 = bits;
}

void msg_readPad(void) {
    s32 f = 0;
    u32 cur = g_padPortHeld[0] & 0x3FFFFFF;
    u32 x;

    x = ~cur & D_8007B978;
    D_8007B97C = cur & ~D_8007B978;
    D_8007B980 = x;
    if (x != 0) {
        msg_readPad_set(4, x);
    }
    if (D_8007B978 != cur) {
        D_8007B990 = 16;
    }
    D_8007B990 = g_gameState->vsyncMode != 0 ? D_8007B990 - 2 : D_8007B990 - 1;
    if (D_8007B990 < 0 && !(D_8007B990 & 3)) {
        f = 1;
        D_8007B978 &= ~0xF0;
    }
    x = cur & ~D_8007B978;
    if (x != 0) {
        s32 v = 1;
        if (f) {
            v = 2;
        }
        msg_readPad_set(v, x);
    }
    D_8007B978 = cur;
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
        tbl = &D_8007B9B0[s->unk1C->unk8];
        D_8007BCA8 = pp;
        D_8007B9A4 = D_8007B9A8 = (u8 *)tbl;
    } else {
        D_8007B9A0 = 0x4000;
        cur = g_primPtr;
        D_8007BCA8 = &g_primPtr;
        D_8007B9A4 = cur;
    }
    g = g_gameState->unk1C;
    D_8007BCA4 = g->unk10[g->unk8];
}

/* width of an embedded string, up to 0xF7 / 0xFF */
static inline s32 msg_measureLine_width(u8 *s) {
    s32 w = 0;
    s32 multi = 0;
    s32 c;
    u32 g;

    D_8007BB98.unk0 = 0;
    c = *s++;
    while (c != 0xF7 && c != 0xFF) {
        do {
            g = func_80050F68(c);
            if (g != 0) {
                w += 1 + (g >> 28);
            }
            multi = multi ? 0 : (c >= 0xFA ? c < 0xFE : 0);
            c = *s++;
        } while (multi);
    }
    return w;
}

/* copy character ch's name into D_8007B930, converting the case */
static inline void msg_measureLine_name(s32 ch) {
    u8 *name = g_gameState->unk1C->characters[ch].unk0;
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
        D_8007B930[k] = c;
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
                w += msg_measureLine_width(g_gameState->unk1C->characters[c - 0x10].unk0);
                break;
            case 0x18:
            case 0x19:
            case 0x1A:
            case 0x1B:
                w += msg_measureLine_width(g_gameState->unk1C->characters[func_8004B120(c - 0x18)].unk0);
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
                w += msg_measureLine_width(D_8007B930);
                break;
            case 0xE:
                n = g_msgNumbers[*s++];
                if (n < 0x100) {
                    str = g_itemNames + g_itemTable[n].unk0;
                } else if (n < 0x200) {
                    str = D_8006ADAC + D_8006B374[n - 0x100].unk0;
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
                if (g < 4 && i < 8 && (tbl = D_8007BB80[g]) != NULL) {
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

/* skip the choices hidden by D_8007B964 once the window reaches the choice line */
static inline void msg_renderText_skip(s32 id, u8 **pp) {
    s32 c;

    if (g_msgChoiceLine >= 0 && g_msgChoiceWindow == id && D_8007BB7C >= g_msgChoiceLine) {
        if (!(D_8007B964 & 1)) {
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
                D_8007B964 >>= 1;
            } while (!(D_8007B964 & 1));
        }
        D_8007B964 >>= 1;
    }
}

/* typewriter: advance the shown character count of an embedded value of n characters */
static inline void msg_renderText_type(MsgWindow *w, s32 n) {
    w->unk24 += w->speed;
    w->unk2A += w->unk24 >> 3;
    w->unk24 &= 7;
    if (w->unk2A >= n) {
        w->unk2A = 0;
    }
}

/* text b of message bank a (D_8007BB80) */
static inline u8 *msg_renderText_text(u32 a, u32 b) {
    u8 *t = NULL;
    u8 *tbl;
    u8 *q;
    s32 k;

    if (a < 4 && b < 8 && (tbl = D_8007BB80[a]) != NULL) {
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
    if (last && w->unk2A != 0) {
        msg_renderText_type(w, fn(arg, w->unk2A));
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

    D_8007BB7C = 0;
    end = s + w->textPos;
    p = s + w->pageStart;
    msg_renderText_skip(w->unkA, &p);
    while (p < end) {
        c = *p++;
        switch (c) {
        case 0xF8:
            func_800512C8(1, 0);
            break;
        case 0xF7:
            func_80051380();
            func_800512C8(0, 14);
            func_80051300();
            D_8007BB7C++;
            msg_renderText_skip(w->unkA, &p);
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
                func_800512C8(0x12, 0);
                break;
            case 0x44:
                func_800512B4(D_8007BC98[-1].vx);
            case 0x43:
                func_800512C8(*p++, 0);
                break;
            case 0x49:
                func_800512C8(0, *p++);
                break;
            case 0x4A:
                func_800512C8(0, -*p++);
                break;
            case 0x48:
                c = *p++;
                c |= *p++ << 8;
                msg_measureLine(&p, &c);
                func_800512C8((w->w - c) >> 1, 0);
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
                func_80050F4C(c - 0x20);
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
                func_80050F4C(4);
                msg_renderText_value(w, g_msgNumbers[*p++], msg_emitItemName, last);
                func_80050F4C(0);
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
                msg_renderText_value(w, func_8004B120(c - 0x18), msg_emitCharName, last);
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
                func_800512C8(0, 1);
                func_800513CC(D_8006E67C[c - 0x30]);
                func_800512C8(1, -1);
                break;
            case 0x60:
            case 0x61:
            case 0x62:
            case 0x63:
            case 0x64:
            case 0x65:
            case 0x66:
            case 0x67:
                func_800512C8(0, 1);
                if (c < 0x62) {
                    c ^= 1;
                }
                c = (g_gameState->config >> 1) & 1 ? (g_gameState->padMap >> (c * 4)) & 0xF : c & 0xF;
                if (c < 2) {
                    c ^= 1;
                }
                func_800513CC(D_8006E67C[c]);
                func_800512C8(1, -1);
                break;
            case 0x3A:
                func_800512C8(4, 2);
                func_800513CC(13);
                func_800512C8(-8, -1);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x3B:
                func_800512C8(4, 7);
                func_800513CC(14);
                func_800512C8(-8, -6);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x3C:
                func_800512C8(1, 5);
                func_800513CC(15);
                func_800512C8(-5, -4);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x3D:
                func_800512C8(6, 5);
                func_800513CC(16);
                func_800512C8(-10, -4);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x2F:
                func_800512C8(0, 1);
                func_800513CC(12);
                func_800512C8(1, -1);
                break;
            case 0x4B:
                func_800513CC(*p++);
                break;
            case 0x70:
                if ((u32)g_msgNumbers[0] & (1 << *p++)) {
                    func_800512C8(6, 3);
                    func_800513CC(0xB6);
                    if (VSync(-1) % 48 >= 16) {
                        func_80051444();
                    }
                    func_800512C8(-24, -3);
                    func_800513CC(0xB5);
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
            func_80051004(c);
            c = *p++;
        default:
            func_80051004(c);
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
    while (w->unk26 == 0 && w->unk2A == 0 && w->unk24 >= 8) {
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
                    w->unk26 = *p++;
                } else {
                    p++;
                }
                break;
            case 0:
            case 1:
                w->flags = (w->flags & ~4) | 8;
            set26:
                w->unk26 = *p++;
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
                n = D_8007BB7C;
                pad_getAnalogThresholds(&D_8007B99C);
                pad_setAnalogThresholds(&tmp);
                g_msgChoiceLine = n;
                w->flags |= 2;
                break;
            case 9:
                w->unk26 = 0xFF;
            case 0x45:
                w->flags |= 0x40;
                break;
            case 0xA:
                w->flags |= 8;
                break;
            case 0xF:
                D_8007BBD8 = *p++;
                break;
            case 0x46:
                D_8007BBD8++;
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
                    w->unk2A = 1;
                    w->unk24 = 0;
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
            w->unk26 = 0xFF;
            break;
        case 0xF7:
            D_8007BB7C++;
            break;
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
            p++;
        default:
            if (!(w->flags & 4)) {
                w->unk24 -= 8;
            }
            break;
        }
    }
    w->textPos = (w->unk26 != 0 ? start : p) - e;
    if (w->unk26 == 0xFF) {
        w->flags &= ~4;
    }
    w->unk24 += w->speed;
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
        func_800512C8(((w->w - w->unk1C) >> 1) + 4, 2);
        func_80051300();
        if (w->textPos < w->pageStart) {
            w->textPos = w->pageStart;
        }
        D_8007BB98.unk0 = 0;
        D_8007B964 = D_8007B960;
        msg_renderText(w, e);
        if (w->unk26 == 0) {
            if (w->unk2A == 0) {
                msg_advanceText(w, e);
            } else if (w->flags & 4) {
                w->unk2A = 0;
                w->unk24 = 8;
            }
        } else {
            w->unk26 -= (w->unk26 != 0xFF);
            if (w->flags & 4) {
                w->unk26 = 0;
            }
            if (w->unk26 == 0) {
                msg_stepControl(w, e);
            }
        }
        func_80051380();
        func_80050F4C(0);
    }
}

/* The sound callback returns s32 (GameData.unk83C), so each call is a call_value insn even with the
   result ignored. That gives the function pointer a v0 preference in local-alloc, which the
   0x10000 path needs (pointer in v0, %hi(g_msgChoice) in v1). The 0x10000 path must come
   last in the chain so its call falls into the shared `return 1` and jump2 cross-jumps the
   0x10/0x40 calls onto its jalr. */
s32 msg_input(MsgWindow *w, s32 pad) {
    u8 f;

    if ((g_evtMode == 1 && (pad & 0xA0000)) || (g_evtMode != 1 && (pad & 0x20000))) {
        f = w->flags;
        if (!(f & 0x40)) {
            if (w->unk26 == 0xFF) {
                w->unk26 = 1;
                if (g_msgChoiceLine >= 0) {
                    g_gameState->unk1C->unk83C(0xD000, 0x67, 0x800000, 0x80, 0x7F);
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
        if (g_msgChoiceWindow != w->unkA) {
            return 0;
        }
        if (pad & 0x10) {
            if (g_msgChoice > 0) {
                g_msgChoice--;
            } else {
                g_msgChoice = g_msgChoiceCount - 1;
            }
            g_gameState->unk1C->unk83C(0xD000, 0x67, 0x800000, 0x80, 0x7F);
        } else if (pad & 0x40) {
            if (g_msgChoice < g_msgChoiceCount - 1) {
                g_msgChoice++;
            } else {
                g_msgChoice = 0;
            }
            g_gameState->unk1C->unk83C(0xD000, 0x67, 0x800000, 0x80, 0x7F);
        } else if (!(pad & 0x10000) || g_msgChoiceCancel < 0) {
            return 0;
        } else {
            g_msgChoice = g_msgChoiceCancel;
            g_gameState->unk1C->unk83C(0xD000, 0x65, 0x800000, 0x80, 0x7F);
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
    w->unkA = 0;
    w->parent = parent;
    if (l->unk0 != 0) {
        w->x = l->unk0;
        w->y = (u8)l->unk8;
    } else {
        w->x = 0xA0 - (l->unk2 >> 1);
        w->y = 0x78 - (l->unk9 >> 1);
    }
    w->w = l->unk2;
    w->h = l->unk9;
    w->flags = 0;
    w->msgId = 0;
    w->textPos = 0;
    w->unk1A = 0;
    w->unk2B = 0;
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
    w->unk26 = 0;
    w->unkA = id;
    w->msgId = msg;
    w->speed = D_8006E688[(g_gameState->config >> 14) & 7];
    w->pageStart = 0;
    w->unk24 = 8;
    w->unk2A = 0;
    w->unk1A = l->unk4;
    w->unk2B = l->unk6;
    w->unk1C = l->unk2;
    w->flags |= 0x10;
    if (!(l->unkB & 1)) {
        g_msgFocus = w;
    }
    r.x = w->x;
    r.y = w->y;
    r.w = w->w + 3;
    r.h = w->h + 3;
    style = 4;
    if (!(l->unk6 & 0x10)) {
        style = 3;
        if (l->unk4 == 0 && !(flags & 4)) {
            style = 2;
        }
    }
    w->window = win_open(&r, 2, style, id);
    if (w->window != NULL) {
        w->window->unk50 = (s32)w;
        win_setDrawContent(w->window, (void (*)(Window *))msg_drawWindowContent);
        win_setDrawOverlay(w->window, msg_drawChoiceCursor);
        if (l->unk4 != 0 && !(flags & 4)) {
            win_setTail(w->window, l->unk6 & 3, l->unk4);
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
    if (!(l->unkB & 1)) {
        g_msgChoiceLine = -1;
        n = D_8007BB94;
        D_8007BB94 = 0;
        g_msgChoiceWindow = id;
        g_msgChoice = n;
    }
    D_8007BBC8++;
    msg_startHereIcon(0);
}

void msg_resetChoices(void) {
    s32 i;
    u8 *e;

    g_msgChoice = 0;
    D_8007B974 = -1;
    g_msgChoiceLine = -1;
    for (i = 0; i < 4; i++) {
        e = msg_drawText_msg(i);
        if (e == NULL || e[0] != 0xF9 || e[1] != 4) {
            break;
        }
        D_8007BB80[i] = e + 2;
    }
    D_8007BB94 = 0;
    D_8007BBC8 = 0;
}

/* Inlined copy of msg_getEntry (message i, its size into out). */
static inline u8 *msg_getEntry_inline(s32 i, WinLayout *out) {
    u8 *e = NULL;
    s32 v;

    if (i >= 0 && i < g_msgTable->unk0) {
        e = (u8 *)g_msgTable->unk4;
        v = ((u32 *)e)[g_evtMode == 4 ? i : i * 2];
        if (out != NULL) {
            out->unk2 = ((v >> 16) & 0xFFF) + 9;
            out->unk9 = (g_evtMode == 4 ? (u32)v >> 28 : ((MsgEntry8 *)e)[i].unk6) * 14 + 6;
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
        g_msgLayout.unkA = 0;
        g_msgLayout.unkB = 0;
        *x = 0;
        *y = 0;
        *z = 0;
        while (!done) {
            if (*p++ != 0xF9) {
                break;
            }
            switch (*p++) {
            case 0x7:
                D_8007B960 = -1;
                g_msgChoiceCount = *p++;
                g_msgChoiceCancel = *p++ - 1;
                break;
            case 0x4F:
                D_8007B960 = D_8007B974;
                n = i = *p;
                mask = D_8007B974;
                while (--i != -1) {
                    n -= (mask ^ 1) & 1;
                    mask >>= 1;
                }
                g_msgChoiceCount = n;
                g_msgLayout.unk9 -= (*p++ - n) * 14;
                n = -1;
                mask = D_8007B960;
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
                g_msgLayout.unk0 = *p++;
                g_msgLayout.unk0 |= *p++ << 8;
                g_msgLayout.unk8 = *p++;
                g_msgLayout.unkA = 0xFF;
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
                g_msgLayout.unkA = p[-1];
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
                g_msgLayout.unkB = 1;
                break;
            case 0xD:
                for (;;) {
                    v = *p++;
                    v |= *p++ << 8;
                    if (v == 0xFFFF) break;
                    t = (v >> 12) & 0xF;
                    v &= 0xFFF;
                    if (t == 0 || ((D_8007B960 >> (t - 1)) & 1)) {
                        msg_measureLine(&p, &v);
                        if (g_msgLayout.unk2 < v) {
                            g_msgLayout.unk2 = v;
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
    GameData *d = g_gameState->unk1C;
    D_8007BCA4 = d->unk10[d->unk8] + a;
}

s32 msg_getPartyMemberId(s32 i) {
    Character *p = g_gameState->unk1C->party[i];
    if (p == NULL) {
        return -1;
    }
    return p->unk34.b.id;
}

void msg_resetPad(void) {
    D_8007B978 = 0;
    D_8007B988.unk0 = 0;
    D_8007B988.unk4 = 0;
}

void msg_update(void) {
    MsgWindow *p = msg_getFocus();
    s32 a;
    msg_readPad();
    if (D_8007B988.unk0 < 3) {
        if (D_8007B988.unk0 > 0) {
            while (p != NULL) {
                a = D_8007B988.unk4;
                if (D_8007B988.unk0 == 2) {
                    a |= 0x80000000;
                }
                if (p->onInput(p, a) != 0) {
                    break;
                }
                p = p->parent;
            }
        }
    }
    D_8007B988.unk0 = 0;
    D_8007B988.unk4 = 0;
    D_8007B998 = D_8007B994;
}

u32 msg_getPadHeld(void) {
    return g_padPortHeld[0] & 0x3FFFFFF;
}

u32 msg_getPadPressed(void) {
    return D_8007B97C & 0x3FFFFFF;
}

u32 msg_getPadReleased(void) {
    return D_8007B980 & 0x3FFFFFF;
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

s32 msg_overlapsRect(Unk8004EBC0 *r) {
    MsgWindow *p;
    for (p = g_msgWindows; p < g_msgWindows + 8; p++) {
        if (p->open != 0 && r->unk0 < p->x + p->w && p->x < r->unk0 + r->unk2 &&
            r->unk8 < p->y + p->h && p->y < r->unk8 + r->unk9) {
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
    s32 b = D_8007B960;
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
    D_8007B974 = a;
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
    D_8007BB94 = r;
}

void func_8004ED3C(void) {
    MsgWindow *p;
    for (p = g_msgWindows; p < g_msgWindows + 10; p++) {
        if (p->open != 0 && p->window != NULL) {
            p->window->unk18 = 0;
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

MsgWindow *msg_allocWindow(Unk8004EBC0 *r, MsgWindow *a, s32 idx, s32 flag) {
    MsgWindow *e = &g_msgWindows[idx];
    e->open = 1;
    e->parent = a;
    e->onInput = 0;
    e->unk10 = 0;
    e->y = 0;
    e->x = 0;
    e->unkA = 0;
    if (r->unk0 != 0) {
        e->x = r->unk0;
        e->y = r->unk8;
    } else {
        e->x = 160 - (r->unk2 >> 1);
        e->y = 120 - (r->unk9 >> 1);
    }
    e->w = r->unk2;
    e->h = r->unk9;
    if (flag != 0) {
        e->flags = 2;
    } else {
        e->flags = 0;
    }
    e->msgId = 0;
    e->textPos = 0;
    e->unk1A = 0;
    e->unk2B = 0;
    e->window = NULL;
    return e;
}

void msg_closeWindow(MsgWindow *p) {
    msg_closeWindow_inline(p);
}

void msg_keepWindowsOnInit(void) {
    D_8007B950 = 1;
}

u8 *msg_getEntry(s32 i, MsgInfo *out) {
    u8 *e = NULL;
    u16 *t;
    s32 v;
    u32 k;

    if (i >= 0 && i < g_msgTable->unk0) {
        e = (u8 *)g_msgTable->unk4;
        v = ((u32 *)e)[g_evtMode == 4 ? i : i * 2];
        if (out != NULL) {
            out->unk2 = ((v >> 16) & 0xFFF) + 9;
            out->unk9 = (g_evtMode == 4 ? (u32)v >> 28 : ((MsgEntry8 *)e)[i].unk6) * 14 + 6;
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
        func_80051004(ch);
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
            func_80051004(*p++);
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
        t = g_itemNames + g_itemTable[id].unk0;
    } else if (id < 0x200) {
        t = D_8006ADAC + D_8006B374[id - 0x100].unk0;
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
        func_80051004(s[i]);
    }
    return len;
}

s32 msg_emitCharName(s32 arg0, s32 n) {
    u8 *name = g_gameState->unk1C->characters[arg0].unk0;
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
        func_80051004(name[i]);
    }
    return len;
}

s32 msg_emitCharNameCaps(s32 ch, s32 n) {
    s32 i;
    s32 j;
    s32 k;
    u8 *name = g_gameState->unk1C->characters[ch].unk0;
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
        D_8007B930[k] = c;
    }
    buf = D_8007B930;
    for (i = 0; i < 8; i++) {
        if (buf[i] == 0xFF) {
            break;
        }
    }
    if (i < n) {
        n = i;
    }
    for (j = 0; j < n; j++) {
        func_80051004(buf[j]);
    }
    return i;
}

void msg_drawWindowContent(Unk8004F5D4 *p) {
    GameData *g = g_gameState->unk1C;

    D_8007BCA4 = g->unk10[g->unk8] + p->unk18;
    func_800515B8();
    func_80051300();
    func_800512B8(p->unk0 + 2, p->unk2 + 2);
    msg_drawText(p->unk50);
    func_80051380();
    func_80051600();
}

void msg_drawChoiceCursor(Window *w) {
    GameData *g;
    s32 ofs;

    if (g_msgChoiceLine >= 0 && w->unk1C == 3 && (((Window *)w->unk50)->unk28 & 2)) {
        g = g_gameState->unk1C;
        D_8007BCA4 = g->unk10[g->unk8] + w->unk18;
        func_800515B8();
        func_80051300();
        ofs = 8;
        func_800512B8(w->unk0 - 3, w->unk2 + ((g_msgChoiceLine + g_msgChoice) * 14 + ofs));
        func_800513CC(1);
        func_80051380();
        func_800515EC();
    }
}

void msg_drawHereIcon(Unk8004AB64 *o) {
    POLY_FT4 *p;
    Unk80068790 *s;
    s32 pos;
    s32 n;
    s32 x;
    s32 nx;

    if (g_evtState->controlEnabled != 0 && (msg_getPadPressed() & PAD_SELECT)) {
        D_8007BBE8 = 60;
        D_8007BBE0 = 1;
    }
    if (D_8007BBE8 > 0 && D_8007AEC8 == 0) {
        n = --D_8007BBE8;
        if (n == 0) {
            D_8007BBE0 = 0;
        }
        if (g_evtMode == 1 && !(g_gameState->unk1C->unk0 & GD_HIDE_ALL) && n < 58) {
            s = &D_80068790;
            func_800515B8();
            func_80051300();
            evt_setDrawPosAtObject(o, 0, 0);
            if (D_8007BBE8 == 57) {
                D_8007BBE4 = D_8007BC9C.s.x < 24;
            }
            pos = D_8007BC9C.w;
            if ((pos >> 16) < 16) {
                func_800512B8(pos & 0xFFFF, 16);
            }
            func_800512C8(0, -((D_8007BBE8 >> 2) % 3));
            p = func_80051530();
            p->x1 = p->x3 = p->x0;
            x = p->x0;
            if (D_8007BBE4) {
                nx = x + s->unk4;
            } else {
                nx = x - s->unk4;
            }
            p->x2 = p->x0 = nx;
            p->y2 = p->y3 = p->y0;
            p->y1 = p->y0 = p->y0 - s->unk5;
            p->u0 = p->u2 = s->unk0;
            p->v0 = p->v1 = s->unk1;
            p->u1 = p->u3 = s->unk0 + s->unk4 - (u8)D_8007BBE4;
            p->v2 = p->v3 = s->unk1 + s->unk5;
            p->tpage = 0x5F;
            p->clut = s->unk2;
            func_80051380();
            func_80051610();
        }
    }
}

void msg_drawPlayerBubble(void) {
    Unk80068788 *s = &D_80068788;
    s32 c = D_8007BC00;
    s32 hw = D_8007BBF8;
    s32 h = D_8007BBFC;
    s32 n;
    POLY_FT4 *p;
    POLY_FT4 *q;
    s32 y;
    s32 x;
    s32 d;

    if (c == 0x80) {
        n = D_8007BBF4 + 0x19;
        func_80051300();
        func_800512C8(-4, -0x10 - D_8007BC04);
        func_800513CC(n);
        func_80051380();
    }
    p = func_80051530();
    q = func_80051530();
    setSemiTrans(p, 1);
    setSemiTrans(q, 1);
    p->r0 = p->g0 = p->b0 = c;
    q->r0 = q->g0 = q->b0 = c;
    y = p->y0 - D_8007BC04;
    x = p->x0;
    p->y2 = p->y3 = y;
    p->y0 = y - D_8007BC04;
    p->x1 = p->x3 = x + hw;
    p->x2 = p->x0 = x - hw;
    p->y1 = p->y0 = y - h;
    q->y0 = q->y1 = y;
    q->y2 = q->y3 = y + h / 2;
    d = hw / 3;
    q->x0 = q->x2 = x - d;
    q->x1 = q->x3 = x + d;
    p->u0 = p->u2 = s->unk0;
    p->v0 = p->v1 = s->unk1;
    p->u1 = p->u3 = s->unk0 + 0x18;
    p->v2 = p->v3 = s->unk1 + 0x10;
    q->u0 = q->u2 = s->unk0 + 8;
    /* identical branches (jump2 merges them), as in math_buildRotMatrixZXY */
    if (q) {
        q->u1 = q->u3 = s->unk0 + 0x10;
    } else {
        q->u1 = q->u3 = s->unk0 + 0x10;
    }
    q->v0 = q->v1 = s->unk1 + 0x10;
    q->v2 = q->v3 = s->unk1 + 0x18;
    q->tpage = p->tpage = 0x5F;
    q->clut = p->clut = s->unk2;
}

void msg_updatePlayerBubble(void) {
    Unk8004AB64 *o;

    if (D_8007BBF0 == 0 && D_8007BBEC != 0) {
        D_8007BC0C = D_8007BBF4 ? D_8006E6D8 : D_8006E690;
    } else if (D_8007BBF0 != 0 && D_8007BBEC == 0) {
        D_8007BC0C = D_8006E6F0;
    }
    D_8007BBF0 = D_8007BBEC;
    D_8007BBEC = 0;
    if (D_8007BC0C == NULL || D_8007AEC8 != 0) {
        return;
    }
    if (D_8007BC08 > 0) {
        D_8007BC08--;
    } else {
        switch (*D_8007BC0C) {
        case 0xFD:
            D_8007BC0C = NULL;
            return;
        case 0xFE:
            D_8007BC08 = D_8007BC0C[1];
            D_8007BC0C += 4;
            goto draw;
        case 0xFF:
            D_8007BC0C -= D_8007BC0C[1] * 4;
            break;
        }
        D_8007BC00 = D_8007BC0C[0];
        D_8007BBF8 = D_8007BC0C[1];
        D_8007BBFC = D_8007BC0C[2];
        D_8007BC04 = D_8007BC0C[3];
        D_8007BC0C += 4;
    }
    draw:
    if (!(g_gameState->unk1C->unk0 & GD_HIDE_ALL)) {
        o = evt_getObject(g_evtState->playerUid);
        if (o != NULL && o->unk5 == 4) {
            func_800515B8();
            evt_setDrawPosAtObject(o, -2, 0);
            msg_drawPlayerBubble();
            func_80051610();
        }
    }
    if (D_8007BBE0 == 0) {
        D_8007BBE8 = 0;
    }
}

void msg_drawAtePrompt(void) {
    if (D_8007BC10 == 0) {
        return;
    }
    if (!(D_8007BC10 & 4) && !evt_isControlEnabled()) {
        return;
    }
    D_8007BC14++;
    if (g_gameState->unk1C->unk0 & GD_HIDE_ALL) {
        return;
    }
    func_800515B8();
    func_800512B8(0x10, 200);
    if ((D_8007BC10 & 3) != 2) {
        if ((D_8007BC14 / 44) & 1) {
            func_800513CC(0x80);
            func_800512C8(2, 0);
            func_800513CC(0x81);
            func_800512C8(2, 0);
            func_800513CC(0x82);
        } else {
            func_800512C8(-2, 0);
            func_800513CC(0x53);
            func_800512C8(2, 0);
            func_800513CC(0x12);
            func_800513CC(0x54);
        }
    } else if ((D_8007BC14 / 15) & 1) {
        func_800513CC(0x1F);
        func_800512C8(2, 0);
        func_800513CC(0x20);
        func_800512C8(2, 0);
        func_800513CC(0x21);
    }
    func_80051610();
}

void msg_startHereIcon(s32 arg0) {
    s32 mode;

    if (arg0 == 0) {
        D_8007BBE0 = 0;
        D_8007BBE8 = 0;
        return;
    }
    mode = (g_gameState->config >> 17) & 1;
    mode = mode == 0;
    if (arg0 >= 3 || (g_evtState->controlEnabled != 0 &&
                      (mode >= 2 || (mode == 1 && (arg0 == 2 || D_8007BBC8 >= 3 || D_8007B710 != 0))))) {
        D_8007BBE8 = 60;
    }
}

void msg_resetPlayerBubble(void) {
    D_8007BC0C = 0;
    D_8007BC08 = 0;
    D_8007BBF0 = 0;
    D_8007BBEC = 0;
}

void msg_showPlayerBubble(s32 a) {
    D_8007BBEC = 1;
    D_8007BBF4 = a;
}

void msg_setAtePrompt(s32 a) {
    if (D_8007BC10 != a) {
        D_8007BC10 = a;
        D_8007BC14 = 0x2C;
    }
}

s32 evt_modelOpField(Unk8004AB64 *o, s32 op) {
    s32 a;
    s32 b;
    u32 *p;
    GameState *gs;

    switch (op) {
    case 0x7F:
        *(u32 *)game_getModel(o->unk3B) &= ~0x10;
        return 0;
    case 0x80:
        *(u32 *)game_getModel(o->unk3B) |= 0x10;
        return 0;
    case 0x81:
        a = evt_getArg8();
        b = evt_getArg8();
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk0_8 = a * 14;
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk0_20 = b * 12;
        return 0;
    case 0x82:
        a = evt_getArg16();
        b = evt_getArg16();
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk4_12 = a;
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk4_22 = b;
        return 0;
    case 0x83:
        a = evt_getArg8();
        p = (u32 *)game_getModel(o->unk3B);
        gs = g_gameState;
        *p |= 0x20;
        gs->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk4_0 = a * 16;
        return 0;
    case 0x84:
        *(u32 *)game_getModel(o->unk3B) &= ~0x20;
        return 0;
    case 0x85:
        g_gameState->unk1C->unk8E4.ovl65->unkC->unk14->unk2C[o->unk3B].unk0_0 = evt_getArg8();
        return 0;
    case 0xD8:
        func_800B3444(0, 0, 1);
        a = evt_getArg8();
        func_80050CA8(a, evt_getArg8());
        g_gameState->unk1C->unk570 = a;
        return 0;
    default:
        return 1;
    }
    return 0;
}

s32 evt_modelOpBattle(Unk8004AB64 *o, s32 op) {
    s32 a;
    s32 b;
    u32 *p;
    GameState *gs;

    switch (op) {
    case 0x7F:
        *(u32 *)game_getModel(o->unk3B) &= ~0x10;
        return 0;
    case 0x80:
        *(u32 *)game_getModel(o->unk3B) |= 0x10;
        return 0;
    case 0x81:
        a = evt_getArg8();
        b = evt_getArg8();
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk0_8 = a * 14;
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk0_20 = b * 12;
        return 0;
    case 0x82:
        a = evt_getArg16();
        b = evt_getArg16();
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk4_12 = a;
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk4_22 = b;
        return 0;
    case 0x83:
        a = evt_getArg8();
        p = (u32 *)game_getModel(o->unk3B);
        gs = g_gameState;
        *p |= 0x20;
        gs->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk4_0 = a * 16;
        return 0;
    case 0x84:
        *(u32 *)game_getModel(o->unk3B) &= ~0x20;
        return 0;
    case 0x85:
        g_gameState->unk1C->unk8E4.ovl65->unk11C0->unk28[o->unk3B].unk0_0 = evt_getArg8();
        return 0;
    case 0xD8:
        a = evt_getArg8();
        evt_getArg8();
        g_gameState->unk1C->unk570 = a;
        return 0;
    default:
        return 1;
    }
    return 0;
}

s32 evt_modelOpWorld(Unk8004AB64 *o, s32 op) {
    s32 a;
    s32 b;
    u32 *p;
    GameState *gs;

    switch (op) {
    case 0x7F:
        *(u32 *)game_getModel(o->unk3B) &= ~0x10;
        return 0;
    case 0x80:
        *(u32 *)game_getModel(o->unk3B) |= 0x10;
        return 0;
    case 0x81:
        a = evt_getArg8();
        b = evt_getArg8();
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk0_8 = a * 14;
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk0_20 = b * 12;
        return 0;
    case 0x82:
        a = evt_getArg16();
        b = evt_getArg16();
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk4_12 = a;
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk4_22 = b;
        return 0;
    case 0x83:
        a = evt_getArg8();
        p = (u32 *)game_getModel(o->unk3B);
        gs = g_gameState;
        *p |= 0x20;
        ((Unk11C0Slot *)gs->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk4_0 = a * 16;
        return 0;
    case 0x84:
        *(u32 *)game_getModel(o->unk3B) &= ~0x20;
        return 0;
    case 0x85:
        ((Unk11C0Slot *)g_gameState->unk1C->unk8E4.ovl65->unk8->unk1C)[o->unk3B].unk0_0 = evt_getArg8();
        return 0;
    case 0xD8:
        a = evt_getArg8();
        func_80050EFC(a, evt_getArg8());
        g_gameState->unk1C->unk570 = a;
        return 0;
    default:
        return 1;
    }
    return 0;
}

void evt_clearGlobalVars(void) {
    bzero(&g_evtGlobalVars, 0x800);
}

s32 func_80050AAC(void) {
    u8 *r = D_8007BC18;
    GameData *gd;
    s32 n;

    if (g_gameState->unk1C->unk8 != 0) {
        r += 0x22B0;
    }
    if (D_8007BC20.unk24 < 2) {
        func_8005338C(D_8007BC20.unkC == 0xFF0 ? -0x1770 : -0xFA0);
    }
    if (D_8007BC20.unk24 != 0) {
        D_8007BC20.unk24++;
    }
    n = D_8007BC20.unk4;
    if (n != 0) {
        fldbg_applyGeomOffset();
        gd = g_gameState->unk1C;
        gte_SetRotMatrix(&gd->unk18);
        gte_SetTransMatrix(&gd->unk18);
        r = (u8 *)func_80053498(n, gd->unk8, gd->unk10[gd->unk8] + 3, (s32)r);
        fldbg_resetGeomOffset();
    }
    return (s32)r;
}

u8 *func_80050BE0(Unk8007BC20Node *buf) {
    Unk8007BC20Node *p = buf;
    Unk8007BC20Node *end;
    D_8007BC20.unk0 = buf;
    if (buf != NULL) {
        end = buf + 222;
        for (; p < end; p++) {
            p->unk0 = p + 1;
        }
        p[-1].unk0 = NULL;
    }
    D_8007BC20.unk4 = 0;
    SetDrawTPage(&D_8007BC20.unk10, 0, 0, GetTPage(0, 3, 0, 0));
    D_8007BC20.unk18 = D_8007BC20.unk10;
    D_8007BC20.unk8 = 0;
    D_8007BC20.unkC = 0;
    D_8007BC20.unk24 = 0;
    D_8007BC18 = (u8 *)(buf + 222);
    return (u8 *)p + 0x4560;
}

void func_80050CA8(s32 a, u32 b) {
    if (a != 0 && b != 0) {
        Unk8007BC20 *p = &D_8007BC20;

        if (a < 0x83) {
            p->unk8 = a;
        } else {
            p->unk8 = 0x82;
        }
    }
    D_8007BC20.unkC = b * 16;
    D_8007BC20.unk24 = a == 0 && b != 0;
}
