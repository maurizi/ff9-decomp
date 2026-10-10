/* 27 rows differ, registers only: m gets t4 and sy t5 (the original has them the other way round). Same template as func_80031718 (matched): ISIN macro from ff9.h, GTE IR/MAC macros from psyq.h, do { } while (0) around the second gpf. The permuter reached 0 only by wrapping sy = -sy in a fake if (D_8006C1C4 || m) {...} else {...}, i.e. m needs one more early reference. */
#include "ff9.h"

MATRIX *func_80031284(SVECTOR *r, MATRIX *m, s32 mode) {
    s16 sx, cx, sy, cy, sz, cz;
    s32 x, y, z;
    s32 t;
    s32 tx, ty, tz;
    s16 t1, t2;
    s32 a, b, c, d, e, f;

    x = r->vx & 0xFFF;
    y = r->vy & 0xFFF;
    z = r->vz & 0xFFF;
    ISIN(sx, r->vx, x);
    tx = 0x400 - x;
    x = tx & 0xFFF;
    ISIN(cx, tx, x);
    ISIN(sy, y, y);
    ty = 0x400 - y;
    y = ty & 0xFFF;
    ISIN(cy, ty, y);
    ISIN(sz, z, z);
    tz = 0x400 - z;
    z = tz & 0xFFF;
    ISIN(cz, tz, z);
    if (mode == 0) {
        t1 = (sy * cz) >> 12;
        t2 = (sy * sz) >> 12;
        m->m[0][0] = (cy * cz) >> 12;
        m->m[0][1] = (-sz * cx + t1 * sx) >> 12;
        m->m[0][2] = (sz * sx + t1 * cx) >> 12;
        m->m[1][0] = (cy * sz) >> 12;
        m->m[1][1] = (cz * cx + t2 * sx) >> 12;
        m->m[1][2] = (-cz * sx + t2 * cx) >> 12;
        m->m[2][0] = -sy;
        m->m[2][1] = (cy * sx) >> 12;
        m->m[2][2] = (cy * cx) >> 12;
    } else {
        gte_ldir0123(cy, cz, sz, sx);
        m->m[2][0] = -sy;
        gte_gpf12_n1();
        a = cz * sx;
        gte_stmac123(d, e, f);
        gte_ldir0123(cx, cy, sz, cz);
        do {
            b = sz * sx;
            m->m[0][0] = d;
            gte_gpf12_b();
            a >>= 12;
            b >>= 12;
            m->m[1][0] = e;
            m->m[2][1] = f;
        } while (0);
        gte_stmac123(d, c, t);
        gte_ldir0123(sy, a, b, t);
        m->m[2][2] = d;
        gte_gpf12_n1();
        gte_stmac123(d, e, f);
        gte_ldir12(t, c);
        d -= c;
        m->m[0][1] = d;
        gte_gpf12_b();
        e += t;
        m->m[1][1] = e;
        gte_stmac12(d, e);
        d += b;
        e -= a;
        m->m[0][2] = d;
        m->m[1][2] = e;
    }
    return m;
}
