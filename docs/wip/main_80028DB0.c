/* 15 rows differ: odd/even pixel branches. The original computes &buf[k] before px>>1 on the odd path and keeps the v<<4 temp separate (a1). This draft gets the separate shift but not the register choice. */
#include "ff9.h"

extern u8 D_80010BD0[];

s32 func_80028DB0(s32 x, s32 hi, s32 c) {
    u8 buf[16];
    u8 w = D_80010BD0[c];
    u8 start = w >> 4;
    u8 width = (w & 0xF) + 1;
    u8 *dst = (u8 *)D_80077144;
    u8 *src = (u8 *)D_80077140 + (c << 6);
    s32 row;
    s32 i;
    s32 j;
    u8 b;
    u8 *p;
    s32 px;
    u8 v, o, r;
    u8 t;

    func_8001D898(buf, 0, 0x10);
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
                if (hi) { r = o | (v << 2); } else { r = o | v; }
            } else {
                v = buf[start + i];
                p = dst + (px >> 1);
                o = *p;
                t = v << 4; if (hi) { r = o | (v << 6); } else { r = o | t; }
            }
            *p = r;
        }
        dst += 0x54;
    }
    return width + x;
}
