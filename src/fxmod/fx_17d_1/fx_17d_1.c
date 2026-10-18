#include "fxmod/fx_17d_1.h"

INCLUDE_ASM("asm/us/fx_17d_1/nonmatchings/fx_17d_1", func_801EC704);

void func_801EC944(u32 *ot, s32 tpage, s32 x, s32 r, s32 g, s32 b) {
    BtlfxExports *ex;
    POLY_G4 *prim;
    POLY_G4 *p;
    s32 i;
    s32 k;

    prim = (POLY_G4 *)D_801ED420->unk24;
    ex = BTLFX_EXPORTS(D_801ED420);
    p = prim;
    D_801ED420->unk24 = (u8 *)(prim + 4);
    for (i = 0; i < 4; i++) {
        setlen(p, 8);
        p->code = 0x3A;
        p->x1 = x + (s16)(i * 16 + 0x80);
        p->r0 = p->r2 = r;
        p->g0 = p->g2 = g;
        p->b0 = p->b2 = b;
        p->r1 = p->r3 = 0;
        p->g1 = p->g3 = 0;
        p->b1 = p->b3 = 0;
        p->x0 = p->x2 = x;
        k = i + 1;
        p->x3 = x + (s16)(k * 16 + 0x80);
        p->y0 = p->y1 = i * 60;
        p->y2 = p->y3 = i * 60 + 60;
        p++;
        ex->btlfx_linkPrimAbr(0x08000000, ot, prim, tpage);
        prim++;
    }
}

INCLUDE_ASM("asm/us/fx_17d_1/nonmatchings/fx_17d_1", fx_17d_1_op80);

INCLUDE_RODATA("asm/us/fx_17d_1/nonmatchings/fx_17d_1", fx_17d_1_headerPtr);
