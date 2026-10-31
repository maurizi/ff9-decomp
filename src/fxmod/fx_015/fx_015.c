#include "fxmod/fx_015.h"

s32 fx_015_op80(s32 mode, Fx015Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_015_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->pos8.vx = work->pos10.vx;
        work->pos8.vy = work->pos10.vy;
        work->pos8.vz = work->pos10.vz;
        work->pos8.vy -= 0x190;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E78F0;
            work->emit0->trailSlots = 0x1E;
            work->emit0->trailStride = 0x30;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x19) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos10);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_015/nonmatchings/fx_015", fx_015_headerPtr);
