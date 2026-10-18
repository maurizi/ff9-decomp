#include "fxmod/fx_1a9.h"

s32 fx_1a9_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E7820 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&D_801E7828);
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&D_801E7828);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        return entry->unk0 >= 0x38;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a9/nonmatchings/fx_1a9", fx_1a9_headerPtr);
