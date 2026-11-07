#include "fxmod/fx_020.h"

s32 fx_020_op80(s32 mode, Fx020Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx020Work);
    } else if (mode == 1) {
        fx_020_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REGEN_LIGHT_TRAIL), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_020_trails;
            work->emit0->trailSlots = FX_REGEN_LIGHT_TRAIL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_REGEN_LIGHT_TRAIL_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        if (frame == FX_REGEN_GLINTS_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_REGEN_GLINTS), (s32)&work->pos8);
        }
        if (frame >= FX_REGEN_GLINTS_FRAME && work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = fx_020_emitRot.vx;
                work->emit4->rot.vy = fx_020_emitRot.vy;
                work->emit4->rot.vz = fx_020_emitRot.vz;
            }
        }
        return frame >= FX_REGEN_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_020/nonmatchings/fx_020", fx_020_headerPtr);
