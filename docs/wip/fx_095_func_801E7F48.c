/* Draft of fx_095 func_801E7F48. Status: 3 aligned mismatches ignoring registers (one load scheduled a slot
 * later), 91 with registers: global-alloc order. The original gives the angle a s0 and the prim bases
 * (&gt4->code, &g4->code, &ft3->code) s1..s3, here the bases get s0..s2 and a s3 (a: 21 refs / 241, gt4 base:
 * 31 / 235). tools/equiv.py: equivalent (23/23 trials). Levers that got here: declaration order of the spilled
 * locals (ft3, g4, f3, i, tpage, env, r1: spill slots follow pseudo numbers), each prim's code store last
 * among its stores except gt4's (the base giv is the last address giv in the loop), c reused for c * 2. */
#include "fxmod/fx_095.h"

void func_801E7F48(s32 ot, s32 x, s32 y, s32 r, s32 c, RECT *rect) {
    BtlfxExports *ex;
    POLY_GT4 *gt4;
    POLY_FT3 *ft3;
    POLY_G4 *g4;
    POLY_F3 *f3;
    s32 i;
    s32 tpage;
    Unk800F78A4 *env;
    s32 r1;
    SVECTOR v[5];
    SVECTOR uv[5];
    CVECTOR col;
    CVECTOR col2;
    s32 a;
    s32 c2;
    s32 cu;
    s32 cv;

    env = D_801E8904;
    gt4 = (POLY_GT4 *)env->unk24;
    ft3 = (POLY_FT3 *)(gt4 + 16);
    g4 = (POLY_G4 *)(ft3 + 16);
    f3 = (POLY_F3 *)(g4 + 16);
    ex = BTLFX_EXPORTS(env);
    env->unk24 = (u8 *)(f3 + 16);
    col.r = col.g = col.b = c;
    c *= 2;
    if (c >= 0x100) {
        c = 0xFF;
    }
    a = 0;
    col2.r = col2.g = col2.b = c;
    r1 = r - 0x18;
    tpage = getTPage(2, 1, rect->x, rect->y) << 16;
    cu = (rect->w >> 1) + (rect->x & 0x3F);
    cv = (rect->h >> 1) + (rect->y & 0xFF);
    v[4].vx = x;
    v[4].vy = y;
    for (i = 0; i < 16; i++) {
        v[0].vx = (ex->math_cos(a) * r1) >> 12;
        v[0].vy = (ex->math_sin(a) * r1) >> 12;
        v[2].vx = (ex->math_cos(a) * r) >> 12;
        v[2].vy = (ex->math_sin(a) * r) >> 12;
        a += 0x100;
        v[1].vx = (ex->math_cos(a) * r1) >> 12;
        v[1].vy = (ex->math_sin(a) * r1) >> 12;
        v[3].vx = (ex->math_cos(a) * r) >> 12;
        v[3].vy = (ex->math_sin(a) * r) >> 12;
        uv[4].vx = cu;
        uv[4].vy = cv;
        uv[0].vx = v[0].vx + cu;
        uv[0].vy = v[0].vy + cv;
        uv[1].vx = v[1].vx + cu;
        uv[1].vy = v[1].vy + cv;
        uv[2].vx = v[2].vx + cu;
        uv[2].vy = v[2].vy + cv;
        uv[3].vx = v[3].vx + cu;
        uv[3].vy = v[3].vy + cv;
        v[0].vx += x;
        v[0].vy += y;
        v[1].vx += x;
        v[1].vy += y;
        v[2].vx += x;
        v[2].vy += y;
        v[3].vx += x;
        v[3].vy += y;
        *(s32 *)&ft3->x0 = *(s32 *)&f3->x0 = *(s32 *)&v[4];
        *(s32 *)&ft3->x1 = *(s32 *)&f3->x1 = *(s32 *)&v[0];
        *(s32 *)&ft3->x2 = *(s32 *)&f3->x2 = *(s32 *)&v[1];
        *(s32 *)&ft3->u0 = uv[4].vx | (uv[4].vy << 8);
        *(s32 *)&ft3->u1 = uv[0].vx | (uv[0].vy << 8) | tpage;
        *(s32 *)&ft3->u2 = uv[1].vx | (uv[1].vy << 8);
        *(s32 *)&ft3->r0 = *(s32 *)&col;
        *(s32 *)&f3->r0 = *(s32 *)&col2;
        ft3->code = 0x26;
        f3->code = 0x22;
        *(s32 *)&gt4->x0 = *(s32 *)&g4->x0 = *(s32 *)&v[0];
        *(s32 *)&gt4->x1 = *(s32 *)&g4->x1 = *(s32 *)&v[2];
        *(s32 *)&gt4->x2 = *(s32 *)&g4->x2 = *(s32 *)&v[1];
        *(s32 *)&gt4->x3 = *(s32 *)&g4->x3 = *(s32 *)&v[3];
        *(s32 *)&gt4->u0 = uv[0].vx | (uv[0].vy << 8);
        *(s32 *)&gt4->u1 = uv[2].vx | (uv[2].vy << 8) | tpage;
        *(s32 *)&gt4->u2 = uv[1].vx | (uv[1].vy << 8);
        *(s32 *)&gt4->u3 = uv[3].vx | (uv[3].vy << 8);
        *(s32 *)&gt4->r0 = *(s32 *)&gt4->r2 = *(s32 *)&col;
        *(s32 *)&g4->r1 = *(s32 *)&g4->r3 = 0;
        *(s32 *)&g4->r0 = *(s32 *)&g4->r2 = *(s32 *)&col2;
        *(s32 *)&gt4->r1 = *(s32 *)&gt4->r3 = 0;
        gt4->code = 0x3E;
        g4->code = 0x3A;
        if ((u32)ot < 0x1000) {
            addPrimLenIn(&env->unk20[ot], gt4, 0x0C000000);
            addPrimLenIn(&env->unk20[ot], ft3, 0x07000000);
            ex->btlfx_linkPrimAbr(0x08000000, (u32 *)&env->unk20[ot], g4, 2);
            ex->btlfx_linkPrimAbr(0x04000000, (u32 *)&env->unk20[ot], f3, 2);
        }
        ft3++;
        gt4++;
        f3++;
        g4++;
    }
}
