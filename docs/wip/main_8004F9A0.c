/* 81 rows differ, almost all from one register: the original keeps q (second func_80051530 result) in a2, i.e. q is a global pseudo allocated after the block-local temps; here q is block-local and takes v0, which shifts every temp. The permuter only fixed it with a fake read of q inside the c == 0x80 block (25 rows). Also the original keeps %hi(D_80068788) in s4 and reads s->unk0 as %lo(D_80068788)(s4), s->unk1/unk2 through s5 = &D_80068788 (cse of the lo_sum). Chained stores are written in the opposite order of the asm (the scheduler reverses them). */
#include "ff9.h"

void func_8004F9A0(void) {
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
    q->u1 = q->u3 = s->unk0 + 0x10;
    q->v0 = q->v1 = s->unk1 + 0x10;
    q->v2 = q->v3 = s->unk1 + 0x18;
    q->tpage = p->tpage = 0x5F;
    q->clut = p->clut = s->unk2;
}
