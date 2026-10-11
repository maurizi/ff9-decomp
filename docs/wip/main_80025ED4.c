/* STATUS: 146 rows (cmp.py), same length (342). Structure and the lerp/divide sequences match; most rows are register naming. Open: x/y params land in s2/s1 (original s1/s2); the original keeps max in a1 and k in a2 (we use t0/a3), so every div/lerp row differs; the POLY_G4 field stores are in a different order (tried all 48 orders of the four xy stores with rgb1/rgb3 before or after: this one is best). */
#include "ff9.h"

typedef struct {
    CVECTOR c[4];
} GaugeColors;
extern GaugeColors D_80068FD8[];

static inline void func_80025ED4_lerp(CVECTOR *out, CVECTOR *from, CVECTOR *to, s32 n, s32 d) {
    out->r = from->r + (to->r - from->r) * n / d;
    out->g = from->g + (to->g - from->g) * n / d;
    out->b = from->b + (to->b - from->b) * n / d;
}

static inline void func_80025ED4_lerp7(CVECTOR *out, CVECTOR *from, CVECTOR *to, s32 n) {
    out->r = from->r + (to->r - from->r) * n / 7;
    out->g = from->g + (to->g - from->g) * n / 7;
    out->b = from->b + (to->b - from->b) * n / 7;
}

void func_80025ED4(s32 x, s32 y, s32 otz, s32 cur, s32 max, s32 style, u8 blink) {
    CVECTOR c1;
    CVECTOR c2;
    GaugeColors *e;
    CVECTOR *f1, *f2, *t1, *t2;
    s32 w;
    s32 k;
    PolyG4Words *p;
    TileWords *t;

    w = cur * 36 / max;
    e = &D_80068FD8[style];
    if (cur != 0) {
        if (blink && cur >= max) {
            k = D_80077150 & 0x1F;
            if (k < 16) {
                f1 = &e->c[2];
                if (k >= 8) {
                    k = 15 - k;
                }
                f2 = &e->c[3];
                t1 = &e->c[0];
                t2 = &e->c[0];
            } else {
                k -= 16;
                f1 = &e->c[2];
                if (k >= 8) {
                    k = 15 - k;
                }
                f2 = &e->c[3];
                t1 = &e->c[1];
                t2 = t1;
            }
            func_80025ED4_lerp7(&c1, f1, t1, k);
            func_80025ED4_lerp7(&c2, f2, t2, k);
        } else {
            c1 = e->c[2];
            func_80025ED4_lerp(&c2, &c1, &e->c[3], cur, max);
        }
        setlen(D_80077170, 8);
        p = (PolyG4Words *)D_80077170;
        p->rgbc0 = p->rgb2 = c1.r | (c1.g << 8) | (c1.b << 16) | 0x38000000;
        p->rgb1 = p->rgb3 = c2.r | (c2.g << 8) | (c2.b << 16);
        p->xy1 = (x + (w + 2)) | ((y + 2) << 16);
        p->xy0 = (x + 2) | ((y + 2) << 16);
        p->xy2 = (x + 2) | ((y + 4) << 16);
        p->xy3 = (x + (w + 2)) | ((y + 4) << 16);
        addPrim(&D_8007716C[otz], p);
        D_80077170 = (u8 *)(p + 1);
    }
    func_800275DC(x, y, otz, 2, 0);
    setlen(D_80077170, 3);
    t = (TileWords *)D_80077170;
    t->rgbc = *(u32 *)&e->c[0];
    t->xy = (x + 1) | ((y + 1) << 16);
    t->wh = (w < 36 ? 0x30000 : 0x10000) | 0x26;
    addPrim(&D_8007716C[otz], t);
    D_80077170 = (u8 *)(t + 1);
}
