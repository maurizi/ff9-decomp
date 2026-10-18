#include "fxmod/fx_138.h"

s32 func_801E7704(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[0], i);
    return 0;
}

s32 func_801E7728(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[1], i);
    return 0;
}

s32 func_801E774C(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[2], i);
    return 0;
}

s32 func_801E7770(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[3], i);
    return 0;
}

s32 func_801E7794(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[4], i);
    return 0;
}

s32 func_801E77B8(SVECTOR *out, s32 arg1, s32 i) {
    func_801E77DC(out, &D_801E8830[5], i);
    return 0;
}

void func_801E77DC(SVECTOR *out, Fx138Path *path, s32 i) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E8500);
    MATRIX m;
    VECTOR v;
    SVECTOR sv;

    func_801E81F0(path, i, &sv);
    sv.vx /= 2;
    sv.vy /= 2;
    sv.vz /= 2;
    ex->func_800316D8(&D_801E8E68, &m);
    gte_SetRotMatrix(&m);
    gte_ldtr0();
    gte_ldv0(&sv);
    gte_rtv0tr();
    gte_stlvnl(&v);
    v.vx += D_801E8E60.vx;
    v.vy += D_801E8E60.vy;
    v.vz += D_801E8E60.vz;
    out->vx = v.vx;
    out->vy = v.vy;
    out->vz = v.vz;
}

INCLUDE_ASM("asm/us/fx_138/nonmatchings/fx_138", fx_138_op80);

INCLUDE_ASM("asm/us/fx_138/nonmatchings/fx_138", fx_138_op81);

INCLUDE_ASM("asm/us/fx_138/nonmatchings/fx_138", func_801E813C);

INCLUDE_ASM("asm/us/fx_138/nonmatchings/fx_138", func_801E8180);

INCLUDE_ASM("asm/us/fx_138/nonmatchings/fx_138", func_801E81D8);

INCLUDE_ASM("asm/us/fx_138/nonmatchings/fx_138", func_801E81F0);

INCLUDE_ASM("asm/us/fx_138/nonmatchings/fx_138", func_801E8254);

INCLUDE_ASM("asm/us/fx_138/nonmatchings/fx_138", func_801E8268);

INCLUDE_RODATA("asm/us/fx_138/nonmatchings/fx_138", fx_138_headerPtr);
