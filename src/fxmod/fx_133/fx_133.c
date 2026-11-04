#include "fxmod/fx_133.h"

s32 fx_133_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 dir;

    if (mode == 0) {
        *args = 8;
    } else if (mode == 1) {
        fx_133_env = env;
        ex->btlfx_getUnitArgBonePos(0, 1, (u16 *)work);
        dir = env->targets->fxArgs[0];
        if (dir < 0x80) {
            work->vy += dir * 4;
        } else {
            work->vy -= (0x100 - dir) * 4;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DISAPPEAR_DUST_PUFF), (s32)work);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_133/nonmatchings/fx_133", fx_133_headerPtr);
