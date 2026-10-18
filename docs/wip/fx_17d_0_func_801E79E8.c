/* Draft of func_801E79E8 (module fx_17d_0). Status: 98/206 aligned instructions with ccprobe; the loop's induction variables (last/vz/q/xy/prim) are ordered differently.
 * Declarations it needs that are not yet in include/fxmod/fx_17d_0.h are repeated below. */
#include "fxmod/fx_17d_0.h"

void func_801E79E8(u32 *ot, s32 n, s32 h, s32 dz, SVECTOR *rot, SVECTOR *pts, CVECTOR *rgb, s32 p, s32 abr);
/* the PsyQ gte_ReadGeomScreen: store the projection distance H */
#define gte_ReadGeomScreen(r0) __asm__ volatile( \
    "cfc2\t$12, $26;" \
    "nop;" \
    "sw\t$12, 0(%0)" \
    : : "r"(r0) : "$12", "memory")

void func_801E79E8(u32 *ot, s32 n, s32 h, s32 dz, SVECTOR *rot, SVECTOR *pts, CVECTOR *rgb, s32 p, s32 abr) {
    Unk800F78A4 *env = D_801E9470;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    CVECTOR c;
    MATRIX m;
    s32 oldh;
    u32 z;
    u32 *last;
    SVECTOR *q;
    s16 *vz;
    s32 flag;
    s32 i;

    if (p < 0) {
        p = 0x1000;
    }
    prim = (LINE_G2 *)env->unk24;
    env->unk24 = (u8 *)(prim + n);
    if (rgb == NULL) {
        c.b = 0;
        c.g = 0;
        c.r = 0;
    } else {
        gte_DpqColorFar_nst(rgb, p);
        gte_strgb(&c);
    }
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ReadGeomScreen(&oldh);
    gte_SetGeomScreen(h);
    ex->func_800316D8(rot, &m);
    m.t[0] = m.t[1] = 0;
    m.t[2] = h;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    q = pts;
    last = (u32 *)(q + n);
    vz = &q->vz;
    for (i = 0; i < n; i++, prim++, q++, vz += 4) {
        flag = last[i] != 0x7F7F7F7F;
        gte_ldv0(q);
        gte_rtps();
        *vz -= dz;
        if (dz > 0 && *vz < -0x80) {
            *vz = 0x200;
            last[i] = 0x7F7F7F7F;
        } else if (dz < 0 && *vz > 0x180) {
            *vz = -0x100;
            last[i] = 0x7F7F7F7F;
        } else {
            gte_stsxy(&prim->x0);
            gte_stszotz(&z);
            if (z < 0x80 && flag) {
                gte_DpqColorFar_nst(&c, ((0x80 - z) << 12) >> 7);
                gte_strgb(&prim->r0);
                *(s32 *)&prim->r1 = 0;
                *(s32 *)&prim->x1 = last[i];
                prim->code = abr == 0xFF ? 0x50 : 0x52;
                ex->btlfx_linkPrimAbr(0x04000000, ot, prim, abr);
            }
            last[i] = *(s32 *)&prim->x0;
        }
    }
    gte_SetGeomScreen(oldh);
}
