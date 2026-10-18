#include "fxmod/fx_086.h"

INCLUDE_ASM("asm/us/fx_086/nonmatchings/fx_086", func_801E7704);

INCLUDE_ASM("asm/us/fx_086/nonmatchings/fx_086", func_801E7E18);

void func_801E7F6C(u32 *ot, s32 n, s32 h, s32 dz, void *src, SVECTOR *pt, CVECTOR *col, s32 p, s32 abr) {
    Unk800F78A4 *env = D_801E9D6C;
    CVECTOR c;
    MATRIX m;
    s32 hsave;
    u32 otz;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    LINE_G2 *prim;
    s32 *prev;
    SVECTOR *pts;
    s32 live;
    s32 i;

    if (p < 0) {
        p = 0x1000;
    }
    prim = (LINE_G2 *)env->unk24;
    env->unk24 = (u8 *)(prim + n);
    if (col == NULL) {
        c.r = c.g = c.b = 0;
    } else {
        gte_DpqColorFar_nst(col, p);
        gte_strgb(&c);
    }
    gte_SetRotMatrix(env->unk14);
    gte_SetTransMatrix(env->unk14);
    gte_ReadGeomScreen(&hsave);
    gte_SetGeomScreen(h);
    ex->func_800316D8(src, &m);
    m.t[1] = 0;
    m.t[0] = 0;
    m.t[2] = h;
    gte_SetRotMatrix(&m);
    gte_SetTransMatrix(&m);
    pts = pt;
    prev = (s32 *)&pts[n];
    for (i = 0; i < n; i++, prev++, pts++, prim++) {
        live = *prev != 0x7F7F7F7F;
        gte_ldv0(pts);
        gte_rtps();
        pts->vz -= dz;
        if (dz > 0 && pts->vz < -0x100) {
            pts->vz = 0x400;
            *prev = 0x7F7F7F7F;
        } else if (dz < 0 && pts->vz > 0x300) {
            pts->vz = -0x200;
            *prev = 0x7F7F7F7F;
        } else {
            gte_stsxy(&prim->x0);
            gte_stszotz(&otz);
            if (otz < 0x200 && live) {
                gte_DpqColorFar_nst(&c, ((0x200 - otz) << 12) >> 9);
                gte_strgb(&prim->r0);
                *(s32 *)&prim->x1 = *prev;
                *(s32 *)&prim->r1 = 0;
                prim->code = abr != 0xFF ? 0x52 : 0x50;
                ex->btlfx_linkPrimAbr(0x4000000, ot, prim, abr);
            }
            *prev = *(s32 *)&prim->x0;
        }
    }
    gte_SetGeomScreen(hsave);
}

INCLUDE_ASM("asm/us/fx_086/nonmatchings/fx_086", fx_086_op80);

INCLUDE_RODATA("asm/us/fx_086/nonmatchings/fx_086", fx_086_headerPtr);
