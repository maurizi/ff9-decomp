#include "fxmod/fx_021.h"

s32 fx_021_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 heightArg;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        fx_021_env = env;
        ex->btlfx_getUnitArgBonePos(0, 1, (u16 *)work);
        heightArg = env->targets->fxArgs[0];
        if (heightArg < 0x80) {
            work->vy += heightArg * 4;
        } else {
            work->vy -= (0x100 - heightArg) * 4;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_OPEN_BOOK_DUST_PUFF), (s32)work);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_021/nonmatchings/fx_021", fx_021_headerPtr);
