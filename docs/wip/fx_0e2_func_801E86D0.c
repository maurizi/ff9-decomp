/* Draft of func_801E86D0 (module fx_0e2). Status: 63 aligned mismatches; same instructions and spills as the
 * original except: loop.c hoists the 0x100 constant of the ua/ub clamps (life 5, threshold 29 with a call in the
 * loop, 135 insns), which then gets no register and is rematerialised as li t0,256; the original loads it in the loop.
 * That needs the loop to have >= 146 RTL insns or one more invariant moved before it; with the constant in the loop
 * (tested with asm("") padding) the rest matches up to sched barriers. Two addPrim calls (one per branch) give ex
 * a register and spill n like the original; the i >= 0x35 branch names its values the other way round (cb first),
 * otherwise jump cross-jumps the two clamp tails. Same body in fx_1fd (func_801E7E18). */
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
            cb = 0x80 - (i - 0x34) * 16;
            ca = 0x80 - (i - 0x33) * 16;
            if (c < cb) {
                cb = c;
            }
            if (c < ca) {
                ca = c;
            }
            setRGB0(p, cb, cb, cb);
            setRGB1(p, cb, cb, cb);
            setRGB2(p, ca, ca, ca);
            setRGB3(p, ca, ca, ca);
        } else {
            setRGB0(p, c, c, c);
            setRGB1(p, c, c, c);
            setRGB2(p, c, c, c);
            setRGB3(p, c, c, c);
        }
        if (e[0].unk0.vx < e[1].unk0.vx) {
            ex->btlfx_addPrim(0x0C000000, 0, p);
        } else {
            ex->btlfx_addPrim(0x0C000000, (e[0].unk8 + e[1].unk8) >> 1, p);
        }
    }
}
