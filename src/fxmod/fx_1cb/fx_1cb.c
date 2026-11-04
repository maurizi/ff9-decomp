#include "fxmod/fx_1cb.h"

s32 fx_1cb_op80(s32 mode, Fx1cbWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        work->pos4.vx = env->targets->centerX;
        work->pos4.vy = env->targets->centerY;
        work->pos4.vz = env->targets->centerZ;
        fx_1cb_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_STOCK_BREAK_SLASH_ARC), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = 0x80;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1cb/nonmatchings/fx_1cb", fx_1cb_headerPtr);
