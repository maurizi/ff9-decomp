#include "fxmod/fx_009.h"

s32 fx_009_op80(s32 mode, Fx009Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        fx_009_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&env->targets->centerX);
        if (work->emit0 != NULL) {
            work->emit0->unk14 = (s32)D_801E7828;
            work->emit0->unk12 = 0x17;
            work->emit0->unkC = 0x70;
        }
    } else {
        return entry->frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_009/nonmatchings/fx_009", fx_009_headerPtr);
