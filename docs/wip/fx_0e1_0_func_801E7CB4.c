/* Draft of func_801E7CB4 (module fx_0e1_0). Status: 33/252 aligned instructions with ccprobe; the history shift loop uses two pointers in the original, and tpage/w/u/v live in caller-saved registers saved around the call.
 * Declarations it needs that are not yet in include/fxmod/fx_0e1_0.h are repeated below. */
#include "fxmod/fx_0e1_0.h"

typedef struct {
    s32 unk0; /* SVECTOR: x, y */
    s16 unk4; /* z */
    u16 unk6; /* point valid */
    s32 unk8; /* SVECTOR: x, y */
    s32 unkC; /* z, pad */
} Fx0e10TrailPt; /* 0x10 */
typedef struct {
    u8 r, g, b, code;
} Fx0e10Rgbc;
void func_801E7CB4(SVECTOR *p0, SVECTOR *p1, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e10TrailPt *pts, s32 abr, s32 phase);

void func_801E7CB4(SVECTOR *p0, SVECTOR *p1, s32 n, s32 tpage, s32 clut, s32 u, s32 v, s32 w, s32 h, s32 p,
                   Fx0e10TrailPt *pts, s32 abr, s32 phase) {
    Unk800F78A4 *env = D_801E9E38;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0e10Rgbc col;
    s32 z;
    POLY_GT4 *prim;
    Fx0e10TrailPt *q;
    Fx0e10TrailPt *r;
    s32 c;
    s32 i;

    if (p < 0) {
        p = 0x1000;
    }
    if (n > 1000) {
        n -= 999;
        for (i = 0; i < n; i++) {
            pts[i].unk6 = 0;
        }
        return;
    }
    q = &pts[n - 1];
    for (i = 0; i < n; i++, q--) {
        q[1] = q[0];
    }
    for (i = 0; i < n; i++) {
        if (!(pts[i].unk6 & pts[i + 1].unk6)) {
            break;
        }
    }
    pts->unk0 = *(s32 *)&p0->vx;
    *(s32 *)&pts->unk4 = *(s32 *)&p0->vz;
    pts->unk6 = 1;
    pts->unk8 = *(s32 *)&p1->vx;
    pts->unkC = *(s32 *)&p1->vz;
    n = i;
    prim = (POLY_GT4 *)env->unk24;
    env->unk24 = (u8 *)(prim + n);
    col.code = abr == 0xFF ? 0x3C : 0x3E;
    w--;
    h--;
    tpage |= (abr & 3) << 5;
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    for (i = 0; i < n; pts++, prim++, phase++) {
        gte_ldv3(&pts[0].unk0, &pts[0].unk8, &pts[1].unk0);
        c = (i << 12) / n;
        col.r = col.g = col.b = p;
        gte_rtpt_b();
        c = ((0x1000 - c) * p) >> 12;
        col.r = col.g = col.b = c;
        *(s32 *)&prim->r0 = *(s32 *)&prim->r1 = *(s32 *)&col;
        col.r = col.g = col.b = c;
        *(s32 *)&prim->r2 = *(s32 *)&prim->r3 = *(s32 *)&col;
        gte_stopz(&z);
        i++;
        if (z != 0) {
            gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
            gte_avsz3();
            gte_stszotz(&z);
            gte_ldv0(&pts[1].unk8);
            gte_rtps();
            ex->btlfx_addPrim(0x0C000000, z, prim);
            gte_stsxy(&prim->x3);
            prim->clut = clut;
            prim->tpage = tpage;
            prim->u0 = prim->u2 = u;
            prim->u1 = prim->u3 = u + w;
            prim->v0 = prim->v1 = v + (((phase & 0x1F) * h) >> 5);
            prim->v2 = prim->v3 = v + ((((phase & 0x1F) + 1) * h) >> 5);
        }
    }
}
