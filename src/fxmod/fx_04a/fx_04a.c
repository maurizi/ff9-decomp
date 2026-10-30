#include "fxmod/fx_04a.h"

s32 fx_04a_op80(s32 mode, Fx04aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk0 = 0;
        D_801E7834 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk4);
        if (p != NULL) {
            p->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->unk0 == 0 && frame >= 0x20) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04a/nonmatchings/fx_04a", fx_04a_headerPtr);
