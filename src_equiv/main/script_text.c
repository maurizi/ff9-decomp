/* Equivalent C for the hand-written module at main 0x80050F4C-0x80053924 (63 routines).
 *
 * The original keeps state in registers across calls, including calls that pass through ordinary
 * gcc-compiled C (which just happens not to touch them). Those registers become the file-static variables
 * below, with the same lifetimes: a routine that saves and restores a register in the original saves and
 * restores the variable here.
 *
 *   $t4  r_pkt   next free primitive while a pen session is open (see the begin/end pairs below)
 *   $t5  r_pen   pen position, x in the low half, y in the high half (PS1 screen coordinates)
 *   $t6  r_code  sprite command word: 0x64000000 | colour (0xBBGGRR)
 *   $t7  r_link  address (low 24 bits) of the last primitive emitted; each new primitive's tag points at
 *                the previous one, so the chain is drawn newest first once linked into the OT
 *   $t8  r_glyph glyph sink used by the number/format printers (selected by func_8005179C)
 *   $s6  r_ip    script instruction pointer
 *   $s7  r_sp    expression stack pointer (grows up; g_evtExprSp is its published copy)
 *   $s5  r_args  "argument is an expression" bits for the scheduler's own opcodes
 *
 * Pen sessions: pen_begin / pen_beginBuffered load r_pen, r_code, r_pkt and r_link from globals and
 * pen_end / pen_endSmall / pen_endFont / pen_endBuffered store them back. C callers (the window text
 * renderer around main.c:14100, the menus) call the primitive emitters in between.
 *
 * Verification: math_atan2, math_length3 (with math_isqrt) and math_distSq2 were replayed against
 * 85 argument/result pairs captured in the emulator (field, walking): no differences. evt_isInRegion matched
 * 29 captured calls (all "outside"). Everything else is checked by reading against the asm only.
 *
 * pen_drawSmallString (unreachable: no callers, and it expects the return address in $t1) is given a normal body.
 * Labels that are not functions (D_8005115C and pen_putSmallChar + 0x68, glyph sinks reached only through
 * func_8005179C) are static functions here.
 */
#include <stdarg.h>
#include "ff9.h"
#include "../src_equiv.h"

/* ---- globals the headers don't declare yet (types from their use in this module) ---- */
extern u32 D_8007BCB4;           /* CLUT/texture word added to glyph sprites (pen_setPalette) */
extern u32 *D_8007BBD0;          /* second glyph table (pen_getGlyph) */
extern u32 g_penColor;           /* pen colour, 0xBBGGRR */
extern u8 *D_8007BCAC;           /* packet pointer of the pen_beginBuffered sessions */
extern DVECTOR D_8007BC78[];     /* pen stack storage */
extern void (*D_8007BD00)(s32);  /* glyph sink chosen by func_8005179C */
extern s32 g_evtExprStack[];         /* expression stack storage */
extern u8 *g_evtObjVars;           /* current object's local variables */
extern u8 D_8006E700[];          /* "divide by zero"-style message printed by the evaluator */
extern u16 D_8006EAD0[];         /* atan table, 0x401 entries, 0 .. 0x200 (45 degrees) */

/* ---- callees without prototypes in the headers ---- */
s32 evt_getObjectProp(EvtObject *o, s32 prop);
u16 func_8004B404(s32 arg0);
EvtObject *evt_resolveObjectId(s32 id);
void evt_setObjectProp(Unk800219D8 *arg0, s32 arg1, s32 arg2);
void func_8004B4A8(s32 i, s16 v);
s32 evt_exprSpecial(s32 op);
u32 msg_getPadPressed(void);
u32 msg_getPadReleased(void);
u32 msg_getPadHeld(void);
s32 func_8004B85C(s32 slot);
s32 evt_joinParty(s32 slot);
s32 evt_exprUnitCompare(s32 op);
s32 evt_popRandomBit(void);
s32 evt_exprUnitAssign(s32 op);
s32 evt_popBitCount(void);
s32 evt_exprUnitFirst(void);
s16 msg_isOpen(s32 i);
void func_8004BB30(void *p);
void math_rotMatrixZYX(void *a, void *b);
void func_8004E91C(void);
s32 evt_execInstr(void);

/* ---- the register file ---- */
static u8 *r_pkt;
static u32 r_pen;
static u32 r_code;
static u32 r_link;
static void (*r_glyph)(s32);
static u8 *r_ip;
static s32 *r_sp;
static u32 r_args;

#define PEN_X(p) ((p) & 0xFFFF)
#define PEN_Y(p) ((p) >> 16)

/* Start a primitive of `len` words at r_pkt: tag = previous primitive, link = this one. */
static void emit_tag(u32 *p, s32 len) {
    p[0] = ((u32)len << 24) | r_link;
    r_link = (u32)p & 0xFFFFFF;
}

/* ============================================================ glyphs and the pen */

void pen_setPalette(s32 clut) {
    D_8007BCB4 = (u32)((clut << 7) + 0x3827) << 16;
}

/* Glyph lookup for the dialogue font. 0xFA-0xFF are lead bytes: they are remembered and 0 is returned;
 * the next byte is then looked up as (lead << 8) | byte. Returns the glyph word, 0 for none. */
u32 pen_getGlyph(s32 c) {
    Unk8007BB98 *g = &D_8007BB98;
    u32 range;
    s32 d;
    s32 e;

    if (g->unk0 != 0) {
        c |= g->unk0 << 8;
        g->unk0 = 0;
    } else if (c - 0xFA >= 0) {
        g->unk0 = c;
        return 0;
    }
    range = g->unk4;
    d = c - (s32)(range & 0xFFFF);
    if (d < 0) {
        return D_8007BBCC[c];
    }
    e = c - (s32)(u16)g->unk8;
    if (e < 0) {
        return D_8007BBCC[d + (range >> 16)];
    }
    return D_8007BBD0[e];
}

/* Dialogue-font glyph: one sprite, then the pen advances by the glyph's width. */
void pen_putGlyph(s32 c) {
    u32 g = pen_getGlyph(c);
    u32 *p;

    if (g == 0) {
        return;
    }
    p = (u32 *)r_pkt;
    emit_tag(p, 4);
    p[1] = r_code;
    p[2] = r_pen + ((g >> 20) & 3);
    p[3] = (g & 0x40FFFF) + D_8007BCB4;
    p[4] = ((g >> 16) & 0xF) | 0xD0000;
    r_pkt += 0x14;
    r_pen += ((g >> 28) & 0xF) + 1;
}

/* Tab: next multiple of 48 pixels from the line start. Shared by both debug fonts. */
static void pen_tab48(void) {
    u32 org = (u32)D_8007BCB0;
    u32 rel = r_pen - org;
    u32 q = rel / 6;

    r_pen = (((q + 8) & 0xFFF8) * 6 + org) | (rel & 0xFFFF0000);
}

static void pen_newline(s32 dy) {
    r_pen = ((PEN_Y(r_pen) + dy) << 16) | (u32)D_8007BCB0;
}

/* 6x6 debug font sprite (u,v from the character, CLUT 0x7FFC). */
static void glyph_debug6(s32 c) {
    u32 *p;
    s32 i;

    if (c == ' ') {
        r_pen += 6;
        return;
    }
    p = (u32 *)r_pkt;
    emit_tag(p, 4);
    p[1] = r_code;
    p[2] = r_pen;
    p[4] = 0x60006;
    i = (c & 0x1F) | ((c << 3) & 0x300);
    p[3] = (i * 6 + 0xE600) | 0x7FFC0000;
    r_pkt += 0x14;
    r_pen += 6;
}

/* Debug-font glyph sink for the field: tab, newline (6 lines), else the 6x6 font. */
void func_80051084(s32 c) {
    if (c == 9) {
        pen_tab48();
        return;
    }
    if (c == 0xA) {
        pen_newline(6);
        return;
    }
    glyph_debug6(c);
}

void pen_drawSmallChar(s32 c) {
    glyph_debug6(c);
}

void pen_addTpage(s32 mode);

/* Second debug font (CLUT 0x368C, v 0xDA): the sprite is followed by a texpage set for its 32-character row. */
void pen_putSmallChar(s32 c) {
    u32 *p;

    if (c == ' ') {
        r_pen += 6;
        return;
    }
    p = (u32 *)r_pkt;
    emit_tag(p, 4);
    p[1] = r_code;
    p[2] = r_pen;
    r_pen += 6;
    p[4] = 0x60006;
    p[3] = ((c & 0x1F) * 6 + 0xDA00) | 0x368C0000;
    r_pkt += 0x14;
    pen_addTpage((c >> 5) - 1);
}

/* D_8005115C: battle glyph sink. Tab is a fixed 24 pixels, newline 6 lines, else the second debug font. */
static void glyph_sink_battle(s32 c) {
    if (c == 9) {
        r_pen += 0x18;
        return;
    }
    if (c == 0xA) {
        pen_newline(6);
        return;
    }
    pen_putSmallChar(c);
}

/* pen_putSmallChar + 0x68: world map glyph sink. Tab 24 pixels, newline 7 lines, else the 6x6 font. */
static void glyph_sink_world(s32 c) {
    if (c == 9) {
        r_pen += 0x18;
        return;
    }
    if (c == 0xA) {
        pen_newline(7);
        return;
    }
    glyph_debug6(c);
}

/* func_80051274 and func_80053384 (`jr ra; nop`, the "print nothing" sinks) are matched C in src/main/main.c. */
void func_80051274(); /* void f(void) in src; called through a void (*)(s32) sink pointer */
void func_80053384();

/* Pen to text cell (col, row): 6 pixels per cell, origin (10, 20); also sets the line start. */
void pen_setCell(s32 col, s32 row) {
    r_pen = ((u32)col | ((u32)row << 16)) * 6 + 0x14000A;
    g_penPos.w = r_pen;
    *(s16 *)&D_8007BCB0 = r_pen;
}

/* Pen x = x, keep y. (Falls into pen_setPos in the original.) */
void func_800512B4(s32 x) {
    r_pen = (x & 0xFFFF) | (PEN_Y(r_pen) << 16);
}

void pen_setPos(s32 x, s32 y) {
    r_pen = (x & 0xFFFF) | ((u32)y << 16);
}

/* Move the pen by (dx, dy). */
void pen_move(s32 dx, s32 dy) {
    u32 x = (r_pen + dx) & 0xFFFF;

    r_pen = ((PEN_Y(r_pen) + dy) << 16) | x;
}

/* Pen colour (0xBBGGRR). */
void pen_setColor(s32 rgb) {
    r_code = rgb & 0xFFFFFF;
    g_penColor = r_code;
    r_code |= 0x64000000;
}

/* Push / pop the pen position. */
void pen_pushPos(void) {
    *(u32 *)g_penStackPtr = r_pen;
    g_penStackPtr++;
}

void pen_popPos(void) {
    g_penStackPtr--;
    r_pen = *(u32 *)g_penStackPtr;
}

/* Print a NUL-terminated string in the second debug font, then return the pen to where it started. */
void pen_putSmallString(u8 *s) {
    s32 c;

    pen_pushPos();
    while ((c = *s++) != 0) {
        pen_putSmallChar(c);
    }
    pen_popPos();
}

/* Same with the 6x6 font. Unreachable in the original (no callers). */
void pen_drawSmallString(u8 *s) {
    s32 c;

    pen_pushPos();
    while ((c = *s++) != 0) {
        pen_drawSmallChar(c);
    }
    pen_popPos();
}

/* DR_TPAGE-style draw mode word 0xE1000000 | mode. */
void pen_addTpage(s32 mode) {
    u32 *p = (u32 *)r_pkt;

    emit_tag(p, 1);
    p[1] = 0xE1000000 | (u32)mode;
    r_pkt += 8;
}

/* Icon from g_uiSprites[i] (8 bytes: uv/CLUT word, w, h, texpage), bracketed by texpage 0x1E and its own. */
void pen_drawSprite(s32 i) {
    u8 *e = (u8 *)&g_uiSprites[0] + i * 8;
    u32 w0 = *(u32 *)e;
    u32 w1 = *(u32 *)(e + 4);
    u32 *p;
    u32 wd;

    pen_addTpage(0x1E);
    p = (u32 *)r_pkt;
    emit_tag(p, 4);
    p[1] = r_code;
    p[2] = r_pen;
    p[3] = w0;
    wd = w1 & 0xFF;
    p[4] = ((w1 & 0xFF00) << 8) | wd;
    r_pen += wd;
    r_pkt += 0x14;
    pen_addTpage(*(s16 *)(e + 6));
}

/* Make the last 0x14-byte primitive semi-transparent. */
void pen_setLastSemiTrans(void) {
    r_pkt[-0x15] |= 2;
}

/* Line from the pen to pen + (dx, dy); the pen moves there. */
void pen_line(s32 dx, s32 dy) {
    u32 *p = (u32 *)r_pkt;

    emit_tag(p, 3);
    p[1] = (r_code & 0xFFFFFF) | 0x40000000;
    p[2] = r_pen;
    pen_move(dx, dy);
    p[3] = r_pen;
    r_pkt += 0x10;
}

/* Rectangle outline w x h from the pen (four lines; the pen ends where it started). */
void pen_rect(s32 w, s32 h) {
    pen_line(w, 0);
    pen_line(0, h);
    pen_line(-w, 0);
    pen_line(0, -h);
}

/* Filled tile w x h at the pen (code 0x60 | flags), followed by texpage `mode`. */
void pen_tile(s32 w, s32 h, s32 flags, s32 mode) {
    u32 *p = (u32 *)r_pkt;

    emit_tag(p, 3);
    p[1] = (r_code & 0xFFFFFF) | ((u32)(flags | 0x60) << 24);
    p[2] = r_pen;
    p[3] = (u32)w | ((u32)h << 16);
    r_pkt += 0x10;
    pen_addTpage(mode);
}

/* Start a 10-word textured quad (code 0x2C) at the pen; the caller fills in the rest. */
POLY_FT4 *pen_addPolyFT4(void) {
    u32 *p = (u32 *)r_pkt;

    emit_tag(p, 9);
    p[1] = (r_code & 0xFFFFFF) | 0x2C000000;
    p[2] = r_pen;
    r_pkt += 0x28;
    return (POLY_FT4 *)p;
}

/* Reset the pen state. */
void pen_init(void) {
    g_penPos.w = 0;
    g_penPktPtr = NULL;
    g_penStackPtr = D_8007BC78;
    g_penColor = 0x808080;
    D_8007BCB0 = 0xA;
    D_8007BCB4 = 0x38270000;
}

/* Begin a pen session on *g_penPktPtr (a packet-pointer variable), linking from *g_penOt. */
void pen_begin(void) {
    r_pen = g_penPos.w;
    r_code = g_penColor;
    r_pkt = *g_penPktPtr;
    r_link = *g_penOt;
    r_code |= 0x64000000;
}

/* End a session: store the packet pointer and the chain head back, remember the pen. */
void pen_end(void) {
    *g_penPktPtr = r_pkt;
    *g_penOt = r_link;
    g_penPos.w = r_pen;
}

/* End with texpage 0x1F / 0x1E first. */
void pen_endSmall(void) {
    pen_addTpage(0x1F);
    pen_end();
}

void pen_endFont(void) {
    pen_addTpage(0x1E);
    pen_end();
}

/* Begin a session on D_8007BCAC (the debug print session). */
void pen_beginBuffered(void) {
    r_pen = g_penPos.w;
    r_code = g_penColor;
    r_pkt = D_8007BCAC;
    r_link = *g_penOt;
    r_code |= 0x64000000;
}

/* End it (nothing if there is no packet buffer). */
void pen_endBuffered(void) {
    if (r_pkt == NULL) {
        return;
    }
    pen_addTpage(0x1F);
    D_8007BCAC = r_pkt;
    *g_penOt = r_link;
    g_penPos.w = r_pen;
}

/* ============================================================ debug printing */

/* Print v in base `base` through the glyph sink, padded to `width`. flags: 1 = pad with '0' (sign first),
 * 2 = unsigned. Digits above 9 are 'A'.. (uppercase). */
void func_800516A0(s32 v, s32 width, s32 base, s32 flags) {
    void (*out)(s32) = D_8007BD00;
    u8 buf[0x24];
    u32 u = (u32)v;
    s32 neg = 0;
    s32 n = 0;
    s32 pad;
    u32 d;

    if (!(flags & 2) && v < 0) {
        u = (u32)-v;
        neg = 1;
    }
    do {
        d = u % (u32)base;
        u /= (u32)base;
        buf[++n] = d > 9 ? d + '0' + 7 : d + '0';
    } while (u != 0);
    pad = width - neg - n;
    if (pad > 0) {
        if (flags & 1) {
            if (neg) {
                out('-');
            }
            do {
                out('0');
            } while (--pad != 0);
            neg = 0;
        } else {
            do {
                out(' ');
            } while (--pad != 0);
        }
    }
    if (neg) {
        out('-');
    }
    while (n > 0) {
        out(buf[n--]);
    }
}

/* Choose the glyph sink: none without a packet buffer, nothing if `quiet`, else per gMode (g_evtMode):
 * battle (2) and world map (3) have their own, everything else uses the field's. */
void func_8005179C(s32 quiet) {
    void (*f)(s32);

    if (r_pkt == NULL) {
        f = func_80051274;
    } else if (quiet != 0) {
        f = func_80053384;
    } else if (g_evtMode == 2) {
        f = glyph_sink_battle;
    } else if (g_evtMode == 3) {
        f = glyph_sink_world;
    } else {
        f = func_80051084;
    }
    D_8007BD00 = f;
    r_glyph = f;
}

void evt_runExpr(void);
s32 evt_popValue(void);

/* evt_runExpr with the pen registers (t4-t9) preserved. */
void func_80051830(void) {
    u8 *pkt = r_pkt;
    u32 pen = r_pen;
    u32 code = r_code;
    u32 link = r_link;
    void (*glyph)(s32) = r_glyph;

    evt_runExpr();
    r_pkt = pkt;
    r_pen = pen;
    r_code = code;
    r_link = link;
    r_glyph = glyph;
}

/* Script debug print (scheduler opcodes 0x30 / 0x6C): a list of items ending in a 0 byte; '"' starts a
 * string literal ending in '"', any other non-zero byte is followed by an expression printed in decimal.
 * quiet = 0 ends the line and closes the session. */
void func_80051878(s32 quiet) {
    s32 c;

    pen_beginBuffered();
    func_8005179C(quiet);
    while ((c = *r_ip++) != 0) {
        if (c == '"') {
            while ((c = *r_ip++) != '"') {
                r_glyph(c);
            }
        } else {
            func_80051830();
            func_800516A0(evt_popValue(), 0, 10, 0);
        }
    }
    if (quiet == 0) {
        func_80051084(0xA);
        pen_endBuffered();
    }
}

/* Script printf (scheduler opcodes 0x31 / 0x6D): one byte, then a "..."-quoted format whose %-conversions
 * take expressions from the script after it. %[0][w[w]]d / x / b (base 10, 16, 2), %c (glyph), \n, \t,
 * \cN (colour: N = '8' grey 0x505050, else 0x3F3F3F plus 0x40 in R/G/B for bits 1/2/0 of N). */
void func_80051920(s32 quiet) {
    u8 *f;
    s32 c;
    s32 v;
    u32 fl;
    s32 base;

    pen_beginBuffered();
    r_ip++;
    func_8005179C(quiet);
    f = r_ip;
    while (*r_ip++ != '"') {
    }
    while ((c = *f++) != '"') {
        if (c == '%') {
            c = *f++;
            fl = 0;
            if (c == '0') {
                fl = 1;
                c = *f++;
            }
            if (c >= '1' && c <= '9') {
                v = c - '0';
                c = *f++;
                if (c >= '0' && c <= '9') {
                    v = v * 10 + (c - '0');
                    c = *f++;
                }
                fl |= v << 8;
            }
            if (c == 'd' || c == 'x' || c == 'b') {
                base = c == 'd' ? 10 : c == 'x' ? 16 : 2;
                func_80051830();
                func_800516A0(evt_popValue(), fl >> 8, base, fl & 1);
            } else if (c == 'c') {
                func_80051830();
                r_glyph(evt_popValue());
            } else {
                r_glyph(c);
            }
        } else if (c == '\\') {
            c = *f++;
            if (c == 'n') {
                r_glyph(0xA);
            } else if (c == 't') {
                r_glyph(9);
            } else if (c == 'c') {
                c = *f++;
                if (c == '8') {
                    pen_setColor(0x505050);
                } else {
                    pen_setColor(0x3F3F3F | ((c & 1) << 22) | ((c & 4) << 12) | ((c & 2) << 5));
                }
            } else {
                r_glyph(c);
            }
        } else {
            r_glyph(c);
        }
    }
    if (quiet == 0) {
        pen_endBuffered();
    }
}

/* Debug printf on screen: %c, %[0][w]d, %[0][w]x (unsigned), %s; anything else is printed. */
void func_80051B58(const char *fmt, ...) {
    va_list ap;
    s32 c;
    u32 fl;
    const u8 *s;

    va_start(ap, fmt);
    pen_beginBuffered();
    func_8005179C(0);
    while ((c = (u8)*fmt++) != 0) {
        if (c != '%') {
            r_glyph(c);
            continue;
        }
        c = (u8)*fmt++;
        if (c == 'c') {
            r_glyph(va_arg(ap, s32));
            continue;
        }
        fl = 0;
        if (c == '0') {
            fl = 1;
            c = (u8)*fmt++;
        }
        if (c >= '1' && c <= '9') {
            fl |= (c - '0') << 8;
            c = (u8)*fmt++;
        }
        if (c == 'd') {
            func_800516A0(va_arg(ap, s32), fl >> 8, 10, fl & 1);
        } else if (c == 'x') {
            func_800516A0(va_arg(ap, s32), fl >> 8, 16, (fl & 1) | 2);
        } else if (c == 's') {
            for (s = va_arg(ap, const u8 *); *s != 0; s++) {
                r_glyph(*s);
            }
        } else {
            r_glyph(c);
        }
    }
    pen_endBuffered();
    func_8004E91C();
    va_end(ap);
}

/* ============================================================ script variables
 *
 * A stack entry is a 32-bit word with a 6-bit tag in bits 26-31. Bits 0-2 of the tag (bits 26-28) select
 * the space and bits 3-5 (bits 29-31) the width:
 *   space 0: global variables (g_evtGlobalVars)       space 1: the event context's map variables
 *   space 2/3: the current object's locals (g_evtObjVars)
 *   space 4: (uid << 8) | index on another object (evt_resolveObjectId + evt_getObjectProp), read-only
 *   space 5: system variable `index` (func_8004B404 / func_8004B4A8)
 *   space 6: property `index` of g_evtExprObj (evt_getObjectProp / evt_setObjectProp)
 *   space 7: an immediate (26-bit, sign-extended)
 *   width (spaces 0-3): 0/1 bit, 2/3 24-bit signed, 4 s8, 5 u8, 6 s16, 7 u16
 */

/* Read `off` of `base` with the tag's width. */
s32 evt_readVar(u8 *base, s32 off, s32 tag) {
    u8 *p;

    if (tag & 0x20) {
        p = base + off;
        if (tag & 0x10) {
            if (tag & 8) {
                return (u16)(p[0] | (p[1] << 8));
            }
            return (s16)(p[0] | (p[1] << 8));
        }
        if (tag & 8) {
            return p[0];
        }
        return (s8)p[0];
    }
    if (tag & 0x10) {
        p = base + off;
        return ((s8)p[2] << 16) | (p[1] << 8) | p[0];
    }
    return ((s8)base[(u32)off >> 3] >> (off & 7)) & 1;
}

/* Pop an entry and return its value. */
s32 evt_popValue(void) {
    u32 t = (u32)*--r_sp;
    u32 tag = t >> 26;

    if (tag & 4) {
        if (tag & 2) {
            if (tag & 1) {
                return (s32)(t << 6) >> 6;
            }
            return evt_getObjectProp((EvtObject *)g_evtExprObj, (s32)(t << 6) >> 6);
        }
        if (tag & 1) {
            return func_8004B404(t & 0xFF);
        }
        return evt_getObjectProp(evt_resolveObjectId((t >> 8) & 0xFF), t & 0xFF);
    }
    if (tag & 2) {
        return evt_readVar(g_evtObjVars, t & 0xFFFF, tag);
    }
    if (!(tag & 1)) {
        return evt_readVar((u8 *)&g_evtGlobalVars, t & 0xFFFF, tag);
    }
    return evt_readVar((u8 *)g_evtState, t & 0xFFFF, tag);
}

/* Write `v` to `off` of `base` with the tag's width. */
void evt_writeVar(u8 *base, s32 off, s32 tag, s32 v) {
    u8 *p;
    s32 bit;

    if (tag & 0x20) {
        p = base + off;
        p[0] = v;
        if (tag & 0x10) {
            p[1] = v >> 8;
        }
        return;
    }
    if (tag & 0x10) {
        p = base + off;
        p[0] = v;
        p[1] = v >> 8;
        p[2] = v >> 16;
        return;
    }
    p = base + ((u32)off >> 3);
    bit = 1 << (off & 7);
    if (v != 0) {
        *p = *p | bit;
    } else {
        *p = *p & (bit ^ 0xFF);
    }
}

/* Pop a variable reference and store `v` in it. Space 4 and immediates are ignored. */
void evt_popAssign(s32 v) {
    u32 t = (u32)*--r_sp;
    u32 tag = t >> 26;

    if (tag & 4) {
        if (tag & 2) {
            evt_setObjectProp((Unk800219D8 *)g_evtExprObj, t & 0xFF, v);
        } else if (tag & 1) {
            func_8004B4A8(t & 0xFF, v);
        }
        return;
    }
    if (tag & 2) {
        evt_writeVar(g_evtObjVars, t & 0xFFFF, tag, v);
    } else if (!(tag & 1)) {
        evt_writeVar((u8 *)&g_evtGlobalVars, t & 0xFFFF, tag, v);
    } else {
        evt_writeVar((u8 *)g_evtState, t & 0xFFFF, tag, v);
    }
}

/* Decode a variable-reference opcode (a negative byte: bits 0-1 space, 2-4 width, 5 = 16-bit index)
 * and the index bytes after it. */
u32 func_80051F18(s32 op) {
    u32 v = ((u32)(op & 3) << 26) | ((u32)(op & 0x1C) << 27);
    u32 i = *r_ip++;

    if (op & 0x20) {
        v |= i;
        i = (u32)*r_ip++ << 8;
    }
    return v | i;
}

/* ============================================================ the expression evaluator */

#define IMM(v) ((s32)((u32)(v) | 0x1C000000))

static void push(s32 v) {
    *r_sp++ = v;
}

/* Call a C helper with the stack published in g_evtExprSp (as the original does). */
#define SYNCED(call) (g_evtExprSp = r_sp, v = (call), r_sp = g_evtExprSp)

/* Evaluate the postfix expression at r_ip. On return g_evtExprSp holds the stack pointer and the result is
 * on top. Division by zero prints D_8006E700 and pushes printf's return value instead. */
void evt_runExpr(void) {
    s32 *base = g_evtExprStack;
    s32 v = 0;
    s32 t;
    s32 op;
    EvtObject *o;
    EvtObject *q;

    r_sp = base;
    for (;;) {
        op = (s8)*r_ip;
        r_ip++;
        if (op < 0) {
            v = (s32)func_80051F18(op);
            push(v);
            continue;
        }
        switch (op) {
        case 0x04: /* x++ */
            t = evt_popValue();
            r_sp++;
            evt_popAssign(t + 1);
            v = t;
            break;
        case 0x05: /* x-- */
            t = evt_popValue();
            r_sp++;
            evt_popAssign(t - 1);
            v = t;
            break;
        case 0x06: /* ++x */
            t = evt_popValue() + 1;
            r_sp++;
            evt_popAssign(t);
            v = t;
            break;
        case 0x07: /* --x */
            t = evt_popValue() - 1;
            r_sp++;
            evt_popAssign(t);
            v = t;
            break;
        case 0x0C: /* push the previous result again */
            break;
        case 0x0D:
            v = -evt_popValue();
            break;
        case 0x0E: /* "not" is !(0 < v), so negative values count as false too */
            v = !(0 < evt_popValue());
            break;
        case 0x10:
            v = ~evt_popValue();
            break;
        case 0x11:
            t = evt_popValue();
            v = evt_popValue() * t;
            break;
        case 0x12:
        case 0x13:
            t = evt_popValue();
            v = evt_popValue();
            if (t == 0) {
                printf((char *)D_8006E700);
                v = 0; /* the original pushes BIOS printf's return value here */
                break;
            }
            v = op == 0x12 ? v / t : v % t;
            break;
        case 0x14:
            t = evt_popValue();
            v = evt_popValue() + t;
            break;
        case 0x15:
            t = evt_popValue();
            v = evt_popValue() - t;
            break;
        case 0x16:
            t = evt_popValue();
            v = evt_popValue() << (t & 31);
            break;
        case 0x17:
            t = evt_popValue();
            v = evt_popValue() >> (t & 31);
            break;
        case 0x18:
            t = evt_popValue();
            v = evt_popValue() < t;
            break;
        case 0x19:
            t = evt_popValue();
            v = t < evt_popValue();
            break;
        case 0x1A:
            t = evt_popValue();
            v = !(t < evt_popValue());
            break;
        case 0x1B:
            t = evt_popValue();
            v = !(evt_popValue() < t);
            break;
        case 0x20:
            t = evt_popValue();
            v = evt_popValue() == t;
            break;
        case 0x21:
            t = evt_popValue();
            v = evt_popValue() != t;
            break;
        case 0x24:
            t = evt_popValue();
            v = evt_popValue() & t;
            break;
        case 0x25:
            t = evt_popValue();
            v = evt_popValue() ^ t;
            break;
        case 0x26:
            t = evt_popValue();
            v = evt_popValue() | t;
            break;
        case 0x27: /* && (the other operand is dropped unread when this one is 0) */
            v = evt_popValue();
            if (v == 0) {
                r_sp--;
                break;
            }
            v = evt_popValue() != 0;
            break;
        case 0x28: /* || */
            v = evt_popValue() != 0;
            if (v != 0) {
                r_sp--;
                break;
            }
            v = evt_popValue() != 0;
            break;
        case 0x29: /* property reference (the previous result is left as it was) */
            push(*r_ip++ | 0x18000000);
            continue;
        case 0x2C: /* = */
            v = evt_popValue();
            evt_popAssign(v);
            break;
        case 0x2F: case 0x30: case 0x31: case 0x32: case 0x33: case 0x34: case 0x35:
        case 0x3D: case 0x3E: case 0x3F: /* compound assignment: the reference stays below the value */
            t = evt_popValue();
            v = evt_popValue();
            if ((op == 0x30 || op == 0x31) && t == 0) {
                r_sp++;
                printf((char *)D_8006E700);
                v = 0; /* the original pushes BIOS printf's return value here */
                break;
            }
            switch (op) {
            case 0x2F: v = v * t; break;
            case 0x30: v = v / t; break;
            case 0x31: v = v % t; break;
            case 0x32: v = v + t; break;
            case 0x33: v = v - t; break;
            case 0x34: v = v << (t & 31); break;
            case 0x35: v = v >> (t & 31); break;
            case 0x3D: v = v & t; break;
            case 0x3E: v = v ^ t; break;
            default:   v = v | t; break;
            }
            r_sp++;
            evt_popAssign(v);
            break;
        case 0x4F:
            t = msg_getPadPressed();
            v = (evt_popValue() & t) != 0;
            break;
        case 0x58:
            t = msg_getPadReleased();
            v = (evt_popValue() & t) != 0;
            break;
        case 0x59:
            t = msg_getPadHeld();
            v = (evt_popValue() & t) != 0;
            break;
        case 0x50:
            v = math_sin(evt_popValue());
            break;
        case 0x51:
            v = math_cos(evt_popValue());
            break;
        case 0x62:
            v = math_sin(evt_popValue() << 4);
            break;
        case 0x63:
            v = math_cos(evt_popValue() << 4);
            break;
        case 0x5D: /* angle from the current object to (x, z) */
        case 0x5E: /* distance from the current object to (x, z) */
            t = evt_popValue();
            v = evt_popValue();
            o = g_evtActor;
            if (op == 0x5D) {
                v = (s32)((u32)math_atan2(v - o->pos.vx, t - o->pos.vz) << 20) >> 24;
            } else {
                v = math_length3(v - o->pos.vx, 0, t - o->pos.vz);
            }
            break;
        case 0x60: /* angle / distance from the current object to the object at address v */
            /* v comes from opcode 0x5F (an object pointer pushed as a 26-bit immediate); see src_equiv.h. */
        case 0x61:
            q = (EvtObject *)SCRIPT_PTR_RESTORE(evt_popValue());
            o = g_evtActor;
            if (op == 0x60) {
                v = (s32)((u32)math_atan2(q->pos.vx - o->pos.vx, q->pos.vz - o->pos.vz) << 20) >> 24;
            } else {
                v = math_length3(q->pos.vx - o->pos.vx, 0, q->pos.vz - o->pos.vz);
            }
            break;
        case 0x5F:
            v = (s32)evt_resolveObjectId(*r_ip++);
            break;
        case 0x66:
            t = evt_popValue();
            v = math_atan2(evt_popValue(), t) >> 4;
            break;
        case 0x6A:
            t = evt_popValue();
            v = evt_getAnimFrameCount(g_evtActor->modelId, t);
            break;
        case 0x6B:
            v = func_8004B85C(evt_popValue());
            break;
        case 0x6D:
            v = evt_joinParty(evt_popValue());
            break;
        case 0x1C: case 0x1D: case 0x1E: case 0x1F: case 0x22: case 0x23:
        case 0x54: case 0x55: case 0x56: case 0x57:
            SYNCED(evt_exprUnitCompare(op));
            break;
        case 0x2D: case 0x36: case 0x37: case 0x38: case 0x39: case 0x3A: case 0x3B: case 0x3C:
        case 0x40: case 0x41: case 0x42:
            SYNCED(evt_exprUnitAssign(op));
            break;
        case 0x2A:
            SYNCED(evt_popBitCount());
            break;
        case 0x2B:
            SYNCED(evt_exprUnitFirst());
            break;
        case 0x4D:
            SYNCED(evt_popRandomBit());
            break;
        case 0x78: /* reference to another object's variable: uid, index */
            push(((r_ip[0] << 8) | r_ip[1]) | 0x10000000);
            r_ip += 2;
            continue;
        case 0x79: /* system variable reference */
            push(*r_ip++ | 0x14000000);
            continue;
        case 0x7A: /* pushes the masked value but keeps the full one as the previous result */
            v = evt_getSysVar(*r_ip++);
            push((v & 0x3FFFFFF) | 0x1C000000);
            continue;
        case 0x7D: /* 16-bit immediate */
            push(IMM(r_ip[0] | ((s8)r_ip[1] << 8)));
            r_ip += 2;
            continue;
        case 0x7E: /* 26-bit immediate */
            push(IMM(r_ip[0] | (r_ip[1] << 8) | (r_ip[2] << 16) | (((s8)r_ip[3] & 3) << 24)));
            r_ip += 4;
            continue;
        case 0x7F: /* end */
            g_evtExprSp = r_sp;
            return;
        default:
            SYNCED(evt_exprSpecial(op));
            break;
        }
        v = IMM(v);
        push(v);
    }
}

/* ============================================================ the scheduler */

/* Next scheduler operand: an expression if the low bit of r_args is set, else a byte. */
s32 func_80052834(void) {
    s32 e = r_args & 1;

    r_args >>= 1;
    if (!e) {
        return *r_ip++;
    }
    evt_runExpr();
    return evt_popValue();
}

#define U16AT(p) ((p)[0] | ((p)[1] << 8))

/* Run every object in the list `n` (ObjList-style nodes {next, obj}) until each yields. Returns 0, or the
 * status that stopped the run: 3, 4, 5, 7, 8, or 6 (also saved in D_8007B6FC with the node). Opcodes handled
 * here: 1 jump, 2 jump if false, 3 jump if true, 4 evt_returnFromFunc, 5 expression, 6 / 0xB / 0xD switches,
 * 0x1C, 0x22 wait, 0x30/0x6C print, 0x31/0x6D printf, 0x32 print position. Everything else (including all
 * opcodes >= 0x6E) goes to evt_execInstr. */
s32 evt_runScripts(Unk16D8 *n) {
    u32 save_args = r_args;
    u8 *save_ip = r_ip;
    s32 *save_sp = r_sp;
    EvtObject *o;
    s32 st;
    s32 op;
    s32 v;
    s32 k;
    u8 *e;
    s32 ret = 0;

    while (n != NULL) {
        o = n->unk4;
        if (o->state == 0) {
            o->state = 2;
            goto next;
        }
        if (o->state == 3) {
            goto next;
        }
        st = 0;
        r_ip = o->ip;
        if (r_ip == NULL) {
            goto next;
        }
        if (o->wait != 0) {
            if (o->wait == 0xFE) {
                if (o->windowId != 0xFF) {
                    if (msg_isOpen(o->windowId) != 0) {
                        goto next;
                    }
                    o->windowId = 0xFF;
                }
                o->wait = 0;
            } else if (o->wait != 0xFF) {
                o->wait = (s8)o->wait - 1;
            }
            goto next;
        }
        g_evtCurObj = o;
        g_evtObjVars = (u8 *)o + o->varsOfs * 4;
        g_evtActor = o->kind == 1 ? evt_getObject(o->uid - 0x40) : o;
        if (g_evtMode == 2) {
            func_8004BB30(o);
        }
        while (st == 0) {
            op = *r_ip;
            if (op >= 0x6E) {
                goto exec;
            }
            r_ip++;
            switch (op) {
            case EVT_OP_JMP:
                r_ip += 2;
                r_ip += (s16)U16AT(r_ip - 2);
                break;
            case EVT_OP_JMP_IFNOT:
                v = evt_popValue();
                r_ip += 2;
                if (v == 0) {
                    r_ip += (s16)U16AT(r_ip - 2);
                }
                break;
            case EVT_OP_JMP_IF:
                v = evt_popValue();
                r_ip += 2;
                if (v != 0) {
                    r_ip += (s16)U16AT(r_ip - 2);
                }
                break;
            case EVT_OP_RETURN:
                evt_returnFromFunc(o);
                goto status; /* the instruction pointer is not stored back */
            case EVT_OP_EXPR:
                evt_runExpr();
                break;
            case EVT_OP_SWITCH: /* count, default, {value, offset} x count; offsets are from r_ip + 3 */
                k = r_ip[0];
                v = (u16)evt_popValue();
                for (e = r_ip + 3; k > 0; k--, e += 4) {
                    if (U16AT(e) == v) {
                        r_ip += U16AT(e + 2) + 3;
                        goto switched;
                    }
                }
                r_ip += U16AT(r_ip + 1) + 3;
            switched:
                break;
            case EVT_OP_SWITCH_TABLE: /* range switch, 8-bit count */
            case EVT_OP_SWITCH_TABLE16: /* range switch, 16-bit count */
                if (op == 0x0D) {
                    k = U16AT(r_ip);
                    r_ip++;
                } else {
                    k = r_ip[0];
                }
                v = evt_popValue() - r_ip[1] - ((s8)r_ip[2] << 8);
                if (v >= 0 && v - k < 0) {
                    r_ip += U16AT(r_ip + 5 + v * 2);
                } else {
                    r_ip += U16AT(r_ip + 3);
                }
                break;
            case EVT_OP_DELETE_OBJ:
                k = r_ip[1];
                r_ip += 2;
                if (k != 0xFF) {
                    EvtObject *t = evt_resolveObjectId(k);
                    if (t != o) {
                        evt_deleteObject(t);
                        break;
                    }
                }
                n = evt_deleteObject(o);
                st = 2;
                break;
            case EVT_OP_WAIT: /* wait: frames, then the object sleeps */
                r_args = *r_ip++;
                v = func_80052834();
                if (v == 0) {
                    break;
                }
                o->wait = v - 0xFE > 0 ? 0xFD : v - 1;
                st = 1;
                break;
            case 0x30:
                func_80051878(0);
                break;
            case 0x6C:
                func_80051878(1);
                break;
            case 0x31:
                func_80051920(0);
                break;
            case 0x6D:
                func_80051920(1);
                break;
            case EVT_OP_PEN_CELL:
                r_args = *r_ip++;
                k = func_80052834();
                pen_setCell(k, func_80052834());
                break;
            default:
                r_ip--;
            exec:
                o->ip = r_ip;
                st = evt_execInstr();
                r_ip = o->ip;
                if (g_evtArgRead <= 0) {
                    r_ip--;
                }
                break;
            }
        }
        o->ip = r_ip;
    status:
        if (st == 4 || st == 5 || st == 3 || st == 7 || st == 8) {
            ret = st;
            goto out;
        }
        if (st == 6) {
            D_8007B6FC = (s32)n;
            ret = 6;
            goto out;
        }
        if (st == 2) {
            continue;
        }
    next:
        n = n->unk0;
    }
out:
    r_args = save_args;
    r_ip = save_ip;
    r_sp = save_sp;
    return ret;
}

/* ============================================================ C entry points into the evaluator */

/* Base of variable space 0 (globals), 1 (event context) or 2 (current object). */
u8 *evt_getVarBank(s32 space) {
    if (space == 0) {
        return (u8 *)&g_evtGlobalVars;
    }
    if (space == 1) {
        return (u8 *)g_evtState;
    }
    return g_evtObjVars;
}

/* Skip a variable reference at *pp (the original also returns it in v0; the C prototype is void). */
void func_80052D0C(u8 **pp) {
    u8 *save = r_ip;

    r_ip = *pp;
    func_80051F18((s8)*r_ip++);
    *pp = r_ip;
    r_ip = save;
}

/* Pop a value from the published stack (the evaluator's result). */
s32 evt_popResult(void) {
    s32 *save = r_sp;
    s32 v;

    r_sp = g_evtExprSp;
    v = evt_popValue();
    g_evtExprSp = r_sp;
    r_sp = save;
    return v;
}

/* Pop a reference from the published stack and store v in it. */
void evt_assignResult(s32 v) {
    s32 *save = r_sp;

    r_sp = g_evtExprSp;
    evt_popAssign(v);
    g_evtExprSp = r_sp;
    r_sp = save;
}

/* Evaluate the expression at the executing object's instruction pointer (g_evtCurObj) and advance it. */
void evt_evalExpr(void) {
    u8 *save_ip = r_ip;
    s32 *save_sp = r_sp;
    EvtObject *x = g_evtCurObj;

    r_ip = x->ip;
    evt_runExpr();
    x->ip = r_ip;
    r_ip = save_ip;
    r_sp = save_sp;
}

/* ============================================================ geometry */

/* x*x + y*y - z*z, or 0x7FFFFFFF if x*x, y*y or their sum overflows. */
s32 math_distSq2(s32 x, s32 y, s32 z) {
    long long a = (long long)x * x;
    long long b = (long long)y * y;
    s32 s;

    if ((s32)(a >> 32) != 0 || (s32)a < 0) {
        return 0x7FFFFFFF;
    }
    if ((s32)(b >> 32) != 0 || (s32)b < 0) {
        return 0x7FFFFFFF;
    }
    s = (s32)a + (s32)b;
    if (s < 0) {
        return 0x7FFFFFFF;
    }
    return s - (s32)((long long)z * z);
}

/* x*x + y*y + z*z, 0x7FFFFFFF on overflow. */
s32 math_dist2(s32 x, s32 y, s32 z) {
    long long a = (long long)x * x;
    long long b = (long long)y * y;
    long long c = (long long)z * z;
    s32 s;

    if ((s32)(a >> 32) != 0 || (s32)a < 0) {
        return 0x7FFFFFFF;
    }
    if ((s32)(b >> 32) != 0 || (s32)b < 0) {
        return 0x7FFFFFFF;
    }
    s = (s32)a + (s32)b;
    if (s < 0) {
        return 0x7FFFFFFF;
    }
    if ((s32)(c >> 32) != 0 || (s32)c < 0) {
        return 0x7FFFFFFF;
    }
    s += (s32)c;
    if (s < 0) {
        return 0x7FFFFFFF;
    }
    return s;
}

/* Integer square root (bit by bit, rounding down). */
s32 math_isqrt(s32 v) {
    s32 r = 0;
    s32 sh;
    s32 b;

    for (sh = 30; sh >= 0; sh -= 2) {
        b = (s32)((u32)(r + 1) << sh);
        r <<= 1;
        if (!(v < b)) {
            v -= b;
            r |= 4;
        }
    }
    return r >> 2;
}

/* Length of (x, y, z): each component is taken as 16 bits (the GTE IR registers), squared with SQR, and
 * the sum (wrapping) goes to math_isqrt. */
s32 math_length3(s32 x, s32 y, s32 z) {
    s32 a = (s16)x;
    s32 b = (s16)y;
    s32 c = (s16)z;

    return math_isqrt((s32)((u32)(a * a) + (u32)(b * b) + (u32)(c * c)));
}

/* Angle of (dx, dz) in 4096 units, from the u16 table D_8006EAD0 (one octant). 0 for (0, 0). */
s32 math_atan2(s32 dx, s32 dz) {
    s32 q;

    if ((dx | dz) == 0) {
        return 0;
    }
    if (dz < 0) {
        if (dx < 0) {
            if (dx - dz < 0) {
                q = (s32)((u32)dz << 10) / dx;
                return 0x400 - D_8006EAD0[q];
            }
            q = (s32)((u32)dx << 10) / dz;
            return D_8006EAD0[q];
        }
        if (dx + dz < 0) {
            q = (s32)((u32)dx << 10) / dz;
            return 0 - D_8006EAD0[-q];
        }
        q = (s32)((u32)dz << 10) / dx;
        return -0x400 + D_8006EAD0[-q];
    }
    if (dx >= 0) {
        if (dx - dz >= 0) {
            q = (s32)((u32)dz << 10) / dx;
            return -0x400 - D_8006EAD0[q];
        }
        q = (s32)((u32)dx << 10) / dz;
        return -0x800 + D_8006EAD0[q];
    }
    if (-dx - dz < 0) {
        q = (s32)((u32)dx << 10) / dz;
        return 0x800 - D_8006EAD0[-q];
    }
    q = (s32)((u32)dz << 10) / dx;
    return 0x400 + D_8006EAD0[-q];
}

/* Rotate the SVECTOR v in place by the angles (rx, ry, 0); TR is cleared and RT left set (as the original). */
void func_800530A8(SVECTOR *v, s32 rx, s32 ry) {
    SVECTOR a;
    MATRIX m;

    if (v == NULL) {
        return;
    }
    a.vx = rx;
    a.vy = ry;
    a.vz = 0;
    a.pad = 0;
    math_rotMatrixZYX(&a, &m);
    SetRotMatrix(&m);
    gte_ldtr0();
    gte_ldv0(v);
    gte_rtv0tr();
    gte_stsv(v);
}

/* A vector of length len along -z, rotated by (rx, ry). (Falls into func_800530A8 in the original.) */
void func_8005309C(s16 *out, s32 rx, s32 ry, s32 len) {
    SVECTOR *v = (SVECTOR *)out;

    *(u32 *)v = 0;
    v->vz = -len;
    func_800530A8(v, rx, ry);
}

/* Is (x, z) inside the convex polygon p? p+0x14: vertex count, p+0x18: vertices as words (x low, z high).
 * For each edge the GTE OP gives (edge.z * (x - v.x) - edge.x * (z - v.z)); inside means all >= 0 or all <= 0.
 * Returns 1 if inside (on an edge counts). The original computes the cross products with the GTE (OP), which
 * leaves R11, R12, R22, R23 and R33 of the rotation matrix overwritten; this version doesn't touch the GTE. */
s32 evt_isInRegion(EvtObject *obj, s32 x, s32 z) {
    u8 *p = (u8 *)obj;
    s32 n = *(s32 *)(p + 0x14);
    u32 *v = (u32 *)(p + 0x18);
    s32 pos = 1;
    s32 negs = 1;
    s32 i;
    u32 a;
    u32 b;
    s32 ex;
    s32 ez;
    s32 ix;
    s32 iz;
    s32 c;

    for (i = 0; i < n; i++) {
        a = v[i];
        b = v[i + 1 < n ? i + 1 : 0];
        ex = (s16)(b - a);
        ez = (s16)((s32)(b - a) >> 16);
        ix = (s16)(x - a);
        iz = (s16)(z - ((s32)a >> 16));
        c = ez * ix - ex * iz;
        pos &= !(c < 0);
        negs &= !(0 < c);
    }
    return pos | negs;
}

/* Signed difference (-0x800..0x7FF) between angle `ang` and the direction from a to b. */
s32 evt_angleToObject(EvtObject *a, EvtObject *b, s32 ang) {
    s32 d = math_atan2(b->pos.vx - a->pos.vx, b->pos.vz - a->pos.vz);

    return (s32)((u32)(ang - d) << 20) >> 20;
}

/* atan2 in 4096 units, from the s16 table D_8006EAD0. */
long ratan2(long y, long x) {
    s32 xneg = 0;
    s32 yneg = 0;
    s32 q;
    s32 r;

    if (x < 0) {
        xneg = 1;
        x = -x;
    }
    if (y < 0) {
        yneg = 1;
        y = -y;
    }
    if (x == 0 && y == 0) {
        return 0;
    }
    if (y < x) {
        if (y & 0x7FE00000) {
            q = y / (x >> 10);
        } else {
            q = (s32)((u32)y << 10) / x;
        }
        r = (s16)D_8006EAD0[q];
    } else {
        if (x & 0x7FE00000) {
            q = x / (y >> 10);
        } else {
            q = (s32)((u32)x << 10) / y;
        }
        r = 0x400 - (s16)D_8006EAD0[q];
    }
    if (xneg) {
        r = 0x800 - r;
    }
    if (yneg) {
        r = -r;
    }
    return r;
}

/* ============================================================ streak particles (two pools)
 *
 * Pool layout (RainState): +0 free list, +4 live list, +8 particles to spawn per call, +0xC fall
 * offset per frame (applied to y, in the high half), +0x10/+0x18 the draw-mode word per buffer, +0x20 set
 * when the free list ran out, +0x24 nonzero = don't move (draw only). A node is {next, pos.xy, pos.z}.
 */

typedef struct Streak {
    struct Streak *next;
    u32 xy;
    s32 z;
} Streak;

/* Spawn pool->unk8 particles from the free list at random positions, y = random byte + y0. The random
 * pointer is aligned down to a word and consumes one word per particle (rand_refill first if needed). */
static void streak_spawn(RainState *pool, s32 y0) {
    Streak *head = (Streak *)pool->freeList;
    Streak *f = head;
    Streak *last = NULL;
    s32 k = pool->count;
    u32 *r;
    u32 w;

    *(s32 *)((u8 *)pool + 0x20) = 0;
    r = (u32 *)((u32)g_randPtr & ~3);
    if ((u8 *)(r + k) - (g_randTable + 0x208) >= 0) {
        rand_refill();
        r = (u32 *)g_randTable;
    }
    while (k != 0) {
        k--;
        last = f; /* NULL when the free list runs out: then nothing moves to the live list */
        if (f == NULL) {
            *(s32 *)((u8 *)pool + 0x20) = 1;
            break;
        }
        f = f->next;
        w = *r++;
        last->xy = ((u32)((w & 0xFF) + y0) << 16) | ((u32)((s32)(w << 16) >> 19) & 0xFFFF);
        last->z = (s32)w >> 19;
    }
    g_randPtr = (u8 *)r;
    if (last != NULL) {
        pool->freeList = (RainNode *)last->next;
        last->next = (Streak *)pool->active;
        pool->active = (s32)head;
    }
}

void rain_spawn(s32 y0) {
    streak_spawn(&g_fieldRain, y0);
}

void wrain_spawn(s32 y0) {
    streak_spawn(&g_worldRain, y0);
}

/* Load a streak's three vertices (p, p + dy, p + 2dy) into V0-V2; unless `still`, move p by dy first. */
static void streak_load(Streak *s, s32 dy, s32 still) {
    SVECTOR v[3];
    u32 xy = s->xy;

    dy <<= 16;
    *(u32 *)&v[0] = xy;
    *(s32 *)&v[0].vz = s->z;
    xy += dy;
    if (!still) {
        s->xy = xy;
    }
    *(u32 *)&v[1] = xy;
    *(s32 *)&v[1].vz = s->z;
    xy += dy;
    *(u32 *)&v[2] = xy;
    *(s32 *)&v[2].vz = s->z;
    gte_ldv3(&v[0], &v[1], &v[2]);
}

void rain_stepDrop(Streak *s, s32 dy, s32 still) {
    streak_load(s, dy, still);
}

void wrain_stepDrop(Streak *s, s32 dy, s32 still) {
    streak_load(s, dy, still);
}

/* Draw the live list `s` as pairs of gouraud lines (black to a depth-faded grey), moving each streak down.
 * Streaks whose word at +6 (the y half of xy) has become positive go back to the free list. The chain is
 * closed with the pool's draw-mode word for buffer `buf` and linked into *ot. Returns the next free
 * primitive. */
static u8 *streak_draw(RainState *pool, Streak *s, s32 buf, u32 *ot, u8 *pkt) {
    Streak *prev = NULL;
    Streak *freel = (Streak *)pool->freeList;
    Streak *live = (Streak *)pool->active;
    s32 dy = pool->speed;
    s32 still = pool->stopping;
    u32 link = *ot;
    u32 *mode = (u32 *)((u8 *)pool + buf * 8 + 0x10);
    u32 *p;
    u32 sxy1;
    s32 sz3;
    s32 c;
    Streak *nx;

    streak_load(s, dy, still);
    gte_rtpt();
    for (;;) {
        p = (u32 *)pkt;
        gte_stsxy3(&p[2], &p[4], &p[7]);
        sxy1 = p[4];
        gte_stsz(&sz3);
        if (*(s16 *)((u8 *)s + 6) > 0) {
            if (prev != NULL) {
                nx = s->next;
                prev->next = nx;
                s->next = freel;
                freel = s;
                s = nx;
            } else {
                nx = s->next;
                live = nx;
                s->next = freel;
                freel = s;
                s = nx;
            }
        } else {
            prev = s;
            s = s->next;
        }
        p[9] = sxy1 + 0x10000;
        if (s != NULL) {
            streak_load(s, dy, still);
            gte_rtpt();
        }
        c = 0xF0 - (sz3 >> 6);
        if (c - 0x40 < 0) {
            c = 0x40;
        } else if (c - 0xFF > 0) {
            c = 0xFF;
        }
        c = c | (c << 8) | (c << 16);
        p[3] = c;
        p[8] = c;
        p[1] = 0x52000000;
        p[6] = 0x52000000;
        p[0] = 0x04000000 | link;
        link = (u32)p & 0xFFFFFF;
        p[5] = 0x04000000 | link;
        link = (u32)&p[5] & 0xFFFFFF;
        pkt += 0x28;
        if (s == NULL) {
            break;
        }
    }
    mode[0] = 0x01000000 | link;
    link = (u32)mode & 0xFFFFFF;
    pool->freeList = (RainNode *)freel;
    pool->active = (s32)live;
    *ot = link;
    return pkt;
}

/* Prototype as in ff9.h: (list, buffer, ot, packet) with pointers passed as s32. */
s32 rain_drawDrops(s32 s, u8 buf, u32 *ot, s32 pkt) {
    return (s32)streak_draw(&g_fieldRain, (Streak *)s, buf, ot, (u8 *)pkt);
}

s32 wrain_drawDrops(s32 s, u8 buf, u32 *ot, s32 pkt) {
    return (s32)streak_draw(&g_worldRain, (Streak *)s, buf, ot, (u8 *)pkt);
}

/* ============================================================ func_80064C24 (main, outside the module) */

/* Pen colour preset n: 0 grey, 1 red-ish 0x2020A0, 2 green-ish 0x20A020, 3 blue-ish 0xA02020, 4 black;
 * other values do nothing. Only ovl_0c calls it (with 0). */
void func_80064C24(s32 arg) {
    static const u32 c[5] = { 0x808080, 0x2020A0, 0x20A020, 0xA02020, 0 };
    u8 n = arg; /* the original compares the low byte */

    if (n < 5) {
        pen_setColor(c[n]);
    }
}
