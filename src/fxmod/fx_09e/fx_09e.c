#include "fxmod/fx_09e.h"

s32 fx_09e_op80(s32 mode, Fx09eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E7880 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk0);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk0);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x18) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk0);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x38;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09e/nonmatchings/fx_09e", fx_09e_headerPtr);
