#include "fxmod/fx_01e.h"

s32 fx_01e_op80(s32 mode, Fx01eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx01eWork);
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_01e_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_1000_NEEDLES_NEEDLE_TRAILS), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_01e_trails;
            work->emit4->trailSlots = FX_1000_NEEDLES_NEEDLE_TRAILS_TRAIL_SLOTS;
            work->emit4->trailStride = FX_1000_NEEDLES_NEEDLE_TRAILS_TRAIL_STRIDE;
        }
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->aim18);
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim18.vx;
                work->emit4->rot.vy = work->aim18.vy;
                work->emit4->rot.vz = work->aim18.vz;
            }
            if (frame == FX_1000_NEEDLES_BURST_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_1000_NEEDLES_NEEDLE_BURST), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0xC);
                }
            }
            if (frame >= FX_1000_NEEDLES_FRAMES) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01e/nonmatchings/fx_01e", fx_01e_headerPtr);
