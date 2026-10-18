#include "fxmod/fx_1ea.h"

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E7804);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E79D0);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_op80);

INCLUDE_RODATA("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_headerPtr);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_op81);

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", fx_1ea_op82);

void func_801E8B00(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8DD8)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8B44(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

void func_801E8B9C(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(D_801E8DD8)->btlfx_drawScreenTint((u32 *)(D_801E8DD8->unk20 + ot), arg4, c, c, c);
}

void func_801E8C34(Fx1eaPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1eaPoint *)data;
}

s32 func_801E8C4C(Fx1eaPath *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx1eaPoint *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

void func_801E8CB0(Fx1eaPath *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx1eaPoint *)data;
}

INCLUDE_ASM("asm/us/fx_1ea/nonmatchings/fx_1ea", func_801E8CC8);
