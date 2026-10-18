/* Draft of fx_1be func_801E89A8 (8 x 4 POLY_GT4 colour-ramp grid). Status: 2 instructions off (build/sdiff.py
 * --aligned): the original loads the table pointer with `lui s5; addiu s5,s5` (a rematerialized spilled pseudo, like
 * the `lui t1` for the i + 1 row), ours with `lui v0; addiu s5,v0` (combine folds the lo_sum into the init).
 * tools/equiv.py: 21/21 trials agree, 99% coverage. What got it here: clut taken as s32 (the original reads the low
 * half of the stack argument at use), chained assignments written in the original's store order (`a = b = x` stores
 * b first), i / z0 / z1 set in the for-init in that order (global-alloc priority: i -> t0, z0 -> a3, z1 -> fp),
 * `for (j = 0, x1 = .., x0 = ..` likewise, and gte_rtv0tr_b (no nops) for the mvmva. */
#include "fxmod/fx_1be.h"

extern SVECTOR D_801E8F38[9];

void func_801E89A8(SVECTOR *pos, void *rot, s32 step, s32 tpage, s32 clut, s32 abr) {
    SVECTOR v[4];
    SVECTOR col;
    MATRIX m;
    u8 uv0[2];
    u8 uv1[2];
    u8 uv2[2];
    u8 uv3[2];
    CVECTOR c0;
    CVECTOR c1;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8FD8);
    POLY_GT4 *prim;
    s32 i;
    s32 j;
    s32 a;
    s32 b;
    s32 z0;
    s32 z1;
    s32 x0;
    s32 x1;
    SVECTOR *t;

    uv1[0] = uv3[0] = 0x5F;
    uv0[1] = uv1[1] = 0x40;
    uv0[0] = uv2[0] = 0;
    uv2[1] = uv3[1] = 0x9F;
    v[0].vy = v[1].vy = v[2].vy = v[3].vy = 0;
    gte_SetRotMatrix(D_801E8FD8->unk14);
    gte_SetTransMatrix(D_801E8FD8->unk14);
    gte_ldv0(pos);
    prim = (POLY_GT4 *)D_801E8FD8->unk24;
    tpage |= (abr & 3) << 5;
    D_801E8FD8->unk24 = (u8 *)(prim + 32);
    gte_rtv0tr_b();
    ex->func_800316D8(rot, &m);
    gte_stlvnl(m.t);
    ex->MulRotMatrix(&m);
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    step /= 8;
    a = 0;
    b = step;
    for (i = 0, z0 = -0x1E00, z1 = -0x2800, t = D_801E8F38; i < 8; i++, t++) {
        col.vx = t->vx;
        col.vy = t->vy;
        col.vz = t->vz;
        c0.r = (col.vx * a) >> 12;
        c0.g = (col.vy * a) >> 12;
        c0.b = (col.vz * a) >> 12;
        col.vx = D_801E8F38[i + 1].vx;
        col.vy = D_801E8F38[i + 1].vy;
        col.vz = D_801E8F38[i + 1].vz;
        c1.r = (col.vx * b) >> 12;
        c1.g = (col.vy * b) >> 12;
        c1.b = (col.vz * b) >> 12;
        a += step;
        b += step;
        v[0].vz = v[1].vz = z1;
        v[2].vz = v[3].vz = z0;
        for (j = 0, x1 = -0x1400, x0 = -0x2800; j < 4; j++) {
            v[0].vx = v[2].vx = x0;
            v[1].vx = v[3].vx = x1;
            gte_ldv3c(v);
            gte_rtpt();
            if (abr == 0xFF) {
                prim->code = 0x3C;
            } else {
                prim->code = 0x3E;
            }
            prim->r0 = prim->r1 = c0.r;
            prim->g0 = prim->g1 = c0.g;
            prim->b0 = prim->b1 = c0.b;
            prim->r2 = prim->r3 = c1.r;
            prim->g2 = prim->g3 = c1.g;
            prim->b2 = prim->b3 = c1.b;
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_ldv0(&v[3]);
            gte_rtps();
            *(u16 *)&prim->u0 = *(u16 *)uv0;
            *(u16 *)&prim->u1 = *(u16 *)uv1;
            *(u16 *)&prim->u2 = *(u16 *)uv2;
            *(u16 *)&prim->u3 = *(u16 *)uv3;
            prim->clut = clut;
            prim->tpage = tpage;
            gte_stsxy(&prim->x3);
            ex->btlfx_addPrim(0xC000000, 0, prim);
            prim++;
            x1 += 0x1400;
            x0 += 0x1400;
        }
        z0 += 0xA00;
        z1 += 0xA00;
    }
}
