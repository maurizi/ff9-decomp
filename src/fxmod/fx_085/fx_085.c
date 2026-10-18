#include "fxmod/fx_085.h"

INCLUDE_ASM("asm/us/fx_085/nonmatchings/fx_085", fx_085_op80);

INCLUDE_ASM("asm/us/fx_085/nonmatchings/fx_085", func_801E82A0);

INCLUDE_ASM("asm/us/fx_085/nonmatchings/fx_085", func_801E8508);

INCLUDE_ASM("asm/us/fx_085/nonmatchings/fx_085", func_801E878C);

s32 func_801E8880(s32 mode, Fx085Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx085Strip *strip;
    Fx085Strip *br;
    s32 n;
    s32 c;
    s32 s;
    s32 t;

    if (mode == 1) {
        it->unk11 = 0;
    } else if (it->unk11 == 0) {
        if (it->unk14 < 2) {
            n = 8;
            if (D_801EAADC != 0) {
                n = 6;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &D_801E9F9C[it->unk10];
            br = (Fx085Strip *)&D_801EA75C[it->unk10];
            func_801E82A0(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame * 0x97, 0x186, frame, c,
                          ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 12, it->unk14);
            func_801E878C(entry, strip, br, it->unk18, it->unk12, c, ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 13,
                          it->unk14);
            func_801E878C(entry, strip, br, it->unk18 + 3, it->unk13, c,
                          ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 13, it->unk14);
            if (frame >= n) {
                it->unk11 = 1;
                return 1;
            }
        } else {
            n = 6;
            if (D_801EAADC != 0) {
                n = 4;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &D_801E9F9C[it->unk10];
            br = (Fx085Strip *)&D_801EA75C[it->unk10];
            func_801E82A0(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame << 6, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->unk14);
            func_801E878C(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E878C(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
            if (frame >= n) {
                it->unk11 = 1;
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_085/nonmatchings/fx_085", func_801E8C28);

INCLUDE_ASM("asm/us/fx_085/nonmatchings/fx_085", fx_085_op83);

INCLUDE_RODATA("asm/us/fx_085/nonmatchings/fx_085", fx_085_headerPtr);
