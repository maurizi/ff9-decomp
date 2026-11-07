#include "fxmod/fx_04a.h"

s32 fx_04a_op80(s32 mode, Fx04aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx04aWork);
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_04a_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GOBLIN_PUNCH_GOLD_RINGS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0 && frame >= FX_GOBLIN_PUNCH_FRAMES) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04a/nonmatchings/fx_04a", fx_04a_headerPtr);
