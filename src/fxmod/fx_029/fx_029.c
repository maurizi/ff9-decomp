#include "fxmod/fx_029.h"

s32 fx_029_op80(s32 mode, Fx029Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 zWord;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx029Work);
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_029_env = env;
        ((s32 *)&work->pos8)[0] = ((s32 *)&env->targets->centerX)[0];
        zWord = ((s32 *)&env->targets->centerX)[1];
        work->pos8.vy -= FX_WHITE_WIND_HEIGHT;
        ((s32 *)&work->pos8)[1] = zWord;
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WHITE_WIND_GLOW), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->otZ = 0x80;
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0 && frame >= FX_WHITE_WIND_FRAMES) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_029/nonmatchings/fx_029", fx_029_headerPtr);
