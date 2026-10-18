#include "fxmod/fx_1ec.h"

s32 func_801E78E0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8C60(&D_801E9034[(s16)(e->unk1F - 1)], frame, &v);
    v.vy = v.vy * 4 / 5;
    v.vz = v.vz * 3 / 4;
    v.vx += D_801EFB64.vx;
    v.vy += D_801EFB64.vy;
    v.vz += D_801EFB64.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 func_801E79DC(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    func_801E8C60(&D_801E90C4[(s16)(e->unk1F - 1)], frame, &v);
    v.vx += D_801EFB6C.vx;
    v.vy += D_801EFB6C.vy;
    v.vz += D_801EFB6C.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_op80);

INCLUDE_RODATA("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_headerPtr);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_op81);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_op82);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8AFC);

void func_801E8B14(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(D_801E8E94)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

s32 func_801E8B58(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8BB0);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8C48);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8C60);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8CC4);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8D30);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8DB4);

INCLUDE_ASM("asm/us/fx_1ec/nonmatchings/fx_1ec", func_801E8E00);
