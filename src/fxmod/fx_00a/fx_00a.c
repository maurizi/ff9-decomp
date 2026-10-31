#include "fxmod/fx_00a.h"

s32 fx_00a_op80(s32 mode, Fx00aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        fx_00a_env = env;
        work->unk0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            if (frame == 0) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos18);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0x28) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00a/nonmatchings/fx_00a", fx_00a_headerPtr);
