#include "fxmod/fx_105.h"

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801E7704);

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801E7B74);

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801E7BA8);

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801E7BF8);

void func_801E7D9C(s32 z, s32 tpage, s32 y0, s32 y1, CVECTOR *rgb) {
    POLY_G4 *p = (POLY_G4 *)D_801EAB18->unk24;
    BtlfxExports *ex = BTLFX_EXPORTS(D_801EAB18);

    D_801EAB18->unk24 = (u8 *)(p + 1);
    p->code = (tpage != 0xFF) ? 0x3A : 0x38;
    p->r0 = rgb[0].r;
    p->g0 = rgb[0].g;
    p->b0 = rgb[0].b;
    p->r1 = rgb[1].r;
    p->g1 = rgb[1].g;
    p->b1 = rgb[1].b;
    p->r2 = rgb[2].r;
    p->g2 = rgb[2].g;
    p->b2 = rgb[2].b;
    p->r3 = rgb[3].r;
    p->g3 = rgb[3].g;
    p->b3 = rgb[3].b;
    p->x2 = 0;
    p->x0 = 0;
    p->x3 = 0x140;
    p->x1 = 0x140;
    p->y1 = y0;
    p->y0 = y0;
    p->y3 = y1;
    p->y2 = y1;
    ex->btlfx_addPrim(0x08000000, z * 4, p);
    ex->btlfx_addPrim(0x01000000, z * 4, ex->btlfx_allocDrMode(0, tpage));
}

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", fx_105_op80);

void func_801E9E1C(void) {
    Unk800F78A4 *env = D_801EAB18;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx105Spark *p;
    s32 t;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        t = ex->btlfx_randRange(-((i + 1) * 0x180), -(i * 0x180));
        p = &D_801EC168[i];
        p->vx = t;
        p->vz = ex->btlfx_randRange(-0x1000, 0x2000);
        p->vy = p->vx >> 4;
        p->vx += env->unk2C->unk0[2].vx;
        p->vz += env->unk2C->unk0[2].vz;
        p->vy += env->unk2C->unk0[2].vy;
    }
}

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801E9F28);

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801EA298);

INCLUDE_ASM("asm/us/fx_105/nonmatchings/fx_105", func_801EA6D4);

INCLUDE_RODATA("asm/us/fx_105/nonmatchings/fx_105", fx_105_headerPtr);
