#include "fxmod/fx_034.h"

INCLUDE_ASM("asm/us/fx_034/nonmatchings/fx_034", func_801E7744);

INCLUDE_RODATA("asm/us/fx_034/nonmatchings/fx_034", fx_034_headerPtr);

INCLUDE_ASM("asm/us/fx_034/nonmatchings/fx_034", func_801E7810);

INCLUDE_ASM("asm/us/fx_034/nonmatchings/fx_034", func_801E7A00);

void func_801E7B1C(s32 kind, s32 count, SVECTOR *v) {
    SVECTOR sum;
    s32 n;
    s32 k;
    s32 m;

    switch (kind) {
    case 0:
    case 2:
    case 4:
    case 6:
        n = 4;
        break;
    case 1:
    case 3:
    case 5:
    case 7:
    default:
        n = 3;
        break;
    }
    for (k = 0; k < count; k++) {
        sum.vx = 0;
        sum.vy = 0;
        sum.vz = 0;
        for (m = 0; m < n; m++, v++) {
            sum.vx += v->vx;
            sum.vy += v->vy;
            sum.vz += v->vz;
        }
        D_801E82E0[k].x = sum.vx / n;
        D_801E82E0[k].y = sum.vy / n;
        D_801E82E0[k].z = sum.vz / n;
    }
}

INCLUDE_ASM("asm/us/fx_034/nonmatchings/fx_034", fx_034_op80);
