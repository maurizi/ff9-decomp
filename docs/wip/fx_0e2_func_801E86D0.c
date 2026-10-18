/* Draft of func_801E86D0 (module fx_0e2). Status: 11/176 aligned instructions with ccprobe; ex and n swap between a callee-saved register and the stack (same body in fx_1fd).
 * Declarations it needs that are not yet in include/fxmod/fx_0e2.h are repeated below. */
#include "fxmod/fx_0e2.h"

void func_801E86D0(s32 n, s32 c, s32 tpage, s32 clut, u16 v0, u16 v1) {
    Fx0e2Edge *e = D_801EE62C;
    POLY_GT4 *p;
    BtlfxExports *ex;
    u16 tp;
    u16 cl;
    s32 i;
    s32 k;
    s32 ua;
    s32 ub;
    s32 ca;
    s32 cb;
    s32 z;

    tp = tpage | 0x20;
    p = (POLY_GT4 *)D_801ECC0C->unk24;
    D_801ECC0C->unk24 = (u8 *)(p + n);
    cl = clut;
    ex = BTLFX_EXPORTS(D_801ECC0C);
    for (i = 0; i < n - 1; i++, p++, e++) {
        p->tpage = tp;
        p->code = 0x3E;
        p->clut = cl;
        p->x0 = e[0].unk0.vx;
        p->y0 = e[0].unk0.vy;
        p->x1 = e[0].unk4.vx;
        p->y1 = e[0].unk4.vy;
        p->x2 = e[1].unk0.vx;
        p->y2 = e[1].unk0.vy;
        p->x3 = e[1].unk4.vx;
        p->y3 = e[1].unk4.vy;
        p->v0 = p->v2 = v0;
        p->v1 = p->v3 = v1;
        k = i % 8;
        ua = k << 5;
        ub = (k + 1) << 5;
        if (ua == 0x100) {
            ua = 0xFF;
        }
        if (ub == 0x100) {
            ub = 0xFF;
        }
        p->u2 = p->u3 = ub;
        p->u0 = p->u1 = ua;
        if (i < 8) {
            ca = i * 16;
            cb = (i + 1) * 16;
            if (c < ca) {
                ca = c;
            }
            if (c < cb) {
                cb = c;
            }
            setRGB0(p, ca, ca, ca);
            setRGB1(p, ca, ca, ca);
            setRGB2(p, cb, cb, cb);
            setRGB3(p, cb, cb, cb);
        } else if (i >= 0x35) {
            ca = 0x80 - (i - 0x34) * 16;
            cb = 0x80 - (i - 0x33) * 16;
            if (c < ca) {
                ca = c;
            }
            if (c < cb) {
                cb = c;
            }
            setRGB0(p, ca, ca, ca);
            setRGB1(p, ca, ca, ca);
            setRGB2(p, cb, cb, cb);
            setRGB3(p, cb, cb, cb);
        } else {
            setRGB0(p, c, c, c);
            setRGB1(p, c, c, c);
            setRGB2(p, c, c, c);
            setRGB3(p, c, c, c);
        }
        if (e[0].unk0.vx < e[1].unk0.vx) {
            z = 0;
        } else {
            z = (e[0].unk8 + e[1].unk8) >> 1;
        }
        ex->btlfx_addPrim(0x0C000000, z, p);
    }
}
