#include "fxmod/fx_1a9.h"

s32 fx_1a9_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1a9_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&fx_1a9_emitPos0);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&fx_1a9_emitPos0);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= 0x38;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a9/nonmatchings/fx_1a9", fx_1a9_headerPtr);
