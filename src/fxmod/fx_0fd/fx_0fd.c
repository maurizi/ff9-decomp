#include "fxmod/fx_0fd.h"

s32 fx_0fd_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        D_801E778C = env;
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0fd/nonmatchings/fx_0fd", fx_0fd_headerPtr);
