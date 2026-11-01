#include "fxmod/fx_18e.h"

s32 fx_18e_op80(s32 mode, Fx18eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    s32 frame;

    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        fx_18e_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos4);
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
    } else {
        frame = entry->frame;
        if (frame == 8) {
            ex->btlfx_getUnitBoneMatrix(0x10, 0x10, (BtlBone *)&m);
            ex->btlfx_matrixPointAhead(&m, 0x12C, (s32)&work->pos14);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos14);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
            }
        }
        return frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_18e/nonmatchings/fx_18e", fx_18e_headerPtr);
