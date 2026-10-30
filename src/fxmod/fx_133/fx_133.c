#include "fxmod/fx_133.h"

s32 fx_133_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 dir;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E7874 = env;
        ex->btlfx_getUnitArgBonePos(0, 1, (u16 *)work);
        dir = env->targets->fxArgs[0];
        if (dir < 0x80) {
            work->vy += dir * 4;
        } else {
            work->vy -= (0x100 - dir) * 4;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)work);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_133/nonmatchings/fx_133", fx_133_headerPtr);
