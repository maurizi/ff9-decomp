#include "fxmod/fx_0d9.h"

INCLUDE_ASM("asm/us/fx_0d9/nonmatchings/fx_0d9", fx_0d9_op80);

INCLUDE_ASM("asm/us/fx_0d9/nonmatchings/fx_0d9", func_801E8188);

INCLUDE_ASM("asm/us/fx_0d9/nonmatchings/fx_0d9", func_801E83F0);

INCLUDE_ASM("asm/us/fx_0d9/nonmatchings/fx_0d9", func_801E8674);

s32 func_801E8768(s32 mode, Fx0d9Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0d9Strip *strip;
    Fx0d9Strip *br;
    s32 n;
    s32 c;
    s32 s;
    s32 t;

    if (mode == 1) {
        it->unk11 = 0;
    } else if (it->unk11 == 0) {
        if (it->unk14 < 2) {
            n = 8;
            if (D_801EAB88 != 0) {
                n = 6;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &D_801EA048[it->unk10];
            br = (Fx0d9Strip *)&D_801EA808[it->unk10];
            func_801E8188(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame * 0x97, 0x186, frame, c,
                          ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 12, it->unk14);
            func_801E8674(entry, strip, br, it->unk18, it->unk12, c, ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 13,
                          it->unk14);
            func_801E8674(entry, strip, br, it->unk18 + 3, it->unk13, c,
                          ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 13, it->unk14);
            if (frame >= n) {
                it->unk11 = 1;
                return 1;
            }
        } else {
            n = 6;
            if (D_801EAB88 != 0) {
                n = 4;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &D_801EA048[it->unk10];
            br = (Fx0d9Strip *)&D_801EA808[it->unk10];
            func_801E8188(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame << 6, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->unk14);
            func_801E8674(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E8674(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
            if (frame >= n) {
                it->unk11 = 1;
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0d9/nonmatchings/fx_0d9", func_801E8B10);

INCLUDE_ASM("asm/us/fx_0d9/nonmatchings/fx_0d9", fx_0d9_op83);

INCLUDE_ASM("asm/us/fx_0d9/nonmatchings/fx_0d9", fx_0d9_op84);

INCLUDE_RODATA("asm/us/fx_0d9/nonmatchings/fx_0d9", fx_0d9_headerPtr);
