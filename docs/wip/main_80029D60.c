/* 10 rows differ: kana/hi get t4/t5 swapped, and the p+0xA giv increment is scheduled before the u/v/clut stores in the original. The q = p copy and (u8)c compares were needed. */
#include "ff9.h"

typedef struct {
    u32 tag;
    u32 rgbc;
    s16 x;
    s16 y;
    u8 u;
    u8 v;
    u16 clut;
} Sprt8Fields;

u8 *func_80029D60(s32 x, s32 y, u32 *ot, u8 *s, s32 pal, u8 *pkt) {
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
            p->rgbc = 0x74808080;
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
            s++;
            q->clut = D_8006BBA0[pal] | hi;
            addPrim(ot, p);
            p++;
        }
    }
    setlen(p, 1);
    ((DrTpageWords *)p)->code = 0xE100061F;
    addPrim(ot, p);
    return (u8 *)p + 8;
}
