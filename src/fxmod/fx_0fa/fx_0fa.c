#include "fxmod/fx_0fa.h"

void func_801E7704(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out) {
    s32 w[4];
    s32 k;
    s32 s;
    s32 r;
    s32 i;
    s32 *wp;

    k = 0x2000000 / (len * len);
    s = 0;
    switch (mode) {
    case 0:
        s = ((0x1000000 / len) * t) >> 16;
        break;
    case 1:
        s = 0x100 - ((k * (len - t) * (len - t)) >> 17);
        break;
    case 2:
        s = (k * t * t) >> 17;
        break;
    }
    r = 0x100 - s;
    w[0] = (s * s * s) >> 12;
    w[1] = (r * s * s * 6) >> 12;
    w[2] = (r * r * s * 6) >> 12;
    w[3] = (r * r * r) >> 12;
    out->vx = 0;
    out->vy = 0;
    out->vz = 0;
    wp = w;
    for (i = 0; i < 4; i++) {
        out->vx += (ctrl[i].vx * *wp) >> 12;
        out->vy += (ctrl[i].vy * *wp) >> 12;
        out->vz += (ctrl[i].vz * *wp) >> 12;
        wp++;
    }
}

INCLUDE_ASM("asm/us/fx_0fa/nonmatchings/fx_0fa", fx_0fa_op80);

INCLUDE_RODATA("asm/us/fx_0fa/nonmatchings/fx_0fa", fx_0fa_headerPtr);
