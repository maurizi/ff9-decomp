#include "fxmod/fx_1fa.h"

s32 fx_1fa_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E80E4.vx = 0;
        D_801E80E4.vy = 0;
        D_801E80E4.vz = -0x618;
        D_801E80DC = env;
        ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x1E), (s32)&D_801E80E4);
    } else {
        return entry->unk0 >= 0x40;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_1fa/nonmatchings/fx_1fa", fx_1fa_op81);

INCLUDE_RODATA("asm/us/fx_1fa/nonmatchings/fx_1fa", fx_1fa_headerPtr);
