/* ovl_0c world map helpers (hand-written asm in the original, standard C ABI). */
#include "ovl_0c.h"
#include "../src_equiv.h"

/* Is any of the object's points on screen inside the rectangle pair rects[idx] (near, SZ3 >= 0x2260) or
 * rects[idx + 1] (far, SZ3 < 0x2260)? Each rectangle is { top-left, bottom-right } as packed xy words.
 * o+0x04: points (8-byte SVECTORs), o+0x0C: s8 point indices, o+0x20: s16 count (>= 1).
 * Each point is transformed (MVMVA, current RT/TR), its y scaled by (4096 - MAC3/16)/4096 (the world's
 * curvature), then projected with RTPS. No callers in the game. */
s32 func_800B3844(u8 *o, u32 *rects, s32 idx) {
    u8 *pts = *(u8 **)(o + 4);
    s32 n = *(s16 *)(o + 0x20);
    s8 *ix = (s8 *)(o + 0xC);
    SVECTOR v;
    s32 m1, m2, m3;
    u32 sxy;
    s32 sz;
    s32 x, y, f;
    u32 *r;

    do {
        v = *(SVECTOR *)(pts + *ix++ * 8);
        gte_ldv0(&v);
        gte_rtv0tr();
        gte_stmac123(m1, m2, m3);
        f = 0x1000 - (m3 >> 4);
        v.vy = (v.vy * f) >> 12;
        gte_ldv0(&v);
        gte_rtps();
        gte_stsxy(&sxy);
        gte_stsz(&sz);
        r = rects + ((sz < 0x2260) + idx) * 2;
        x = (s16)sxy;
        y = (s32)sxy >> 16;
        if ((s32)r[0] >> 16 > y || (s16)r[0] > x) {
            continue;
        }
        if (y > (s32)r[1] >> 16 || x > (s16)r[1]) {
            continue;
        }
        return 1;
    } while (--n != 0);
    return 0;
}

/* Depth-cue a palette: dst[0] = src[0], then 15 colours (15-bit BGR + bit 15) go through DPCL with IR0 = depth
 * and IR1-3 = col[0..2]; bit 15 is kept. */
void func_800B3998(u16 *src, u16 *dst, s32 depth, s16 *col) {
    s32 i;
    u32 c;
    u32 rgb;

    *dst++ = *src++;
    for (i = 0; i < 15; i++) {
        c = *src++;
        gte_ldrgbc(((c & 0x7C00) << 9) | ((c & 0x3E0) << 6) | ((c & 0x1F) << 3));
        gte_ldir0123(depth, col[0], col[1], col[2]);
        gte_dpcl();
        gte_strgb2(rgb);
        *dst++ = (c & 0x8000) | ((rgb >> 9) & 0x7C00) | ((rgb >> 6) & 0x3E0) | ((rgb >> 3) & 0x1F);
    }
}
