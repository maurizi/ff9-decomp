#include "fxmod/fx_0ef.h"

INCLUDE_ASM("asm/us/fx_0ef/nonmatchings/fx_0ef", fx_0ef_op80);

s32 fx_0ef_op81(s32 mode, Fx0efWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E7B4C = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk8);
        work->unk8.vy += 0x64;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk8);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk8);
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

INCLUDE_ASM("asm/us/fx_0ef/nonmatchings/fx_0ef", func_801E7A4C);

INCLUDE_ASM("asm/us/fx_0ef/nonmatchings/fx_0ef", func_801E7A64);

INCLUDE_RODATA("asm/us/fx_0ef/nonmatchings/fx_0ef", fx_0ef_headerPtr);
