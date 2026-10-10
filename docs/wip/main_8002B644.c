/* 1 row differs: c*8 is computed from the raw lbu value (sll v0,a0,3) in the original, from the andi'd copy here. The do{}while(0) and the (u & 0xFF | 0x4000) grouping fixed everything else. */
#include "ff9.h"
extern u32 D_8006BBB4[];
u8 *func_8002B644(s32 x, s32 y, u32 *ot, u8 *s, s32 style, void *pkt) {
    SprtWords *p = pkt;
    DrTpageWords *t;
    u32 rgbc = 0x64808080;
    u8 c;
    s32 u;

    if (style == 2) {
        rgbc = 0x66606060;
    }
    while ((c = *s) != 0) {
        if (c != ' ') {
            do { } while (0);
            if (c != '/') {
                u = c * 8 - 0x1E8;
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
    t->code = 0xE100061F;
    addPrim(ot, t);
    return (u8 *)(t + 1);
}
