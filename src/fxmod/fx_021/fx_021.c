#include "fxmod/fx_021.h"

s32 fx_021_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 v;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E7874 = env;
        ex->btlfx_getUnitArgBonePos(0, 1, (u16 *)work);
        v = env->targets->fxArgs[0];
        if (v < 0x80) {
            work->vy += v * 4;
        } else {
            work->vy -= (0x100 - v) * 4;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)work);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_021/nonmatchings/fx_021", fx_021_headerPtr);
