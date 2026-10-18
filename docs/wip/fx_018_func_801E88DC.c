/* Draft of fx_018 func_801E88DC (same body in fx_085 and fx_0d9). Status: 18 aligned mismatches: the original keeps
 * the constant 1 in fp for the whole function (mode == 1 test, then it->unk11 = 1 after the calls); here CSE does
 * not carry it to the final store. */
#include "fxmod/fx_018.h"

s32 func_801E88DC(s32 mode, Fx018Bolt *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx018Strip *strip;
    Fx018Strip *br;
    s32 n;
    s32 c;
    s32 s;
    s32 t;

    if (mode == 1) {
        it->unk11 = 0;
    } else if (it->unk11 == 0) {
        if (it->unk14 < 2) {
            n = 8;
            if (D_801EA9D0 != 0) {
                n = 6;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &D_801E9E90[it->unk10];
            br = (Fx018Strip *)&D_801EA650[it->unk10];
            func_801E82FC(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame * 0x97, 0x186, frame, c,
                          ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 12, it->unk14);
            func_801E87E8(entry, strip, br, it->unk18, it->unk12, c, ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 13,
                          it->unk14);
            func_801E87E8(entry, strip, br, it->unk18 + 3, it->unk13, c,
                          ((0x1000 - t) * (it->unk15 * 2 + 0x97)) >> 13, it->unk14);
        } else {
            n = 6;
            if (D_801EA9D0 != 0) {
                n = 4;
            }
            t = (frame << 12) / n;
            c = ex->math_cos(t >> 2);
            strip = &D_801E9E90[it->unk10];
            br = (Fx018Strip *)&D_801EA650[it->unk10];
            func_801E82FC(entry, strip, it->unk18, &it->unk0, &it->unk8, 0, 0x200, frame << 6, 0x186, frame, c,
                          ((0x1000 - t) * 0xAA) >> 12, it->unk14);
            func_801E87E8(entry, strip, br, it->unk18, it->unk12, c, (0x1000 - t) >> 6, it->unk14);
            func_801E87E8(entry, strip, br, it->unk18 + 3, it->unk13, c, (0x1000 - t) >> 6, it->unk14);
        }
        if (frame >= n) {
            it->unk11 = 1;
            return 1;
        }
    }
    return 0;
}
