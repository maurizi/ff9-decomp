#include "fxmod/fx_0c3.h"

s32 fx_0c3_op80(s32 mode, Fx0c3Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_0c3_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos8);
    } else {
        return entry->frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c3/nonmatchings/fx_0c3", fx_0c3_headerPtr);
