#include "fxmod/fx_020.h"

s32 fx_020_op80(s32 mode, Fx020Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        fx_020_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E78F4;
            work->emit0->trailSlots = 0x14;
            work->emit0->trailStride = 0x90;
        }
    } else {
        frame = entry->frame;
        if (frame == 0x14) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos8);
        }
        if (frame >= 0x14 && work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = D_801E7894.vx;
                work->emit4->rot.vy = D_801E7894.vy;
                work->emit4->rot.vz = D_801E7894.vz;
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_020/nonmatchings/fx_020", fx_020_headerPtr);
