#include "fxmod/fx_07b.h"

s32 fx_07b_op80(s32 mode, Fx07bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_07b_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_07b_trails;
            work->emit0->trailSlots = 6;
            work->emit0->trailStride = 0xA0;
        }
        ex->btlfx_getUnitRot(0, (s16 *)&work->rot10);
        fx_07b_emitRot.vy = work->rot10.vy;
        work->pos8.vx -= ex->math_sin(work->rot10.vy) * 0x190 / 4096;
        work->pos8.vz -= ex->math_cos(work->rot10.vy) * 0x190 / 4096;
    } else {
        frame = entry->frame;
        if (frame == 0xE) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = fx_07b_emitRot.vx;
                work->emit0->rot.vy = fx_07b_emitRot.vy;
                work->emit0->rot.vz = fx_07b_emitRot.vz;
            }
        }
        if (frame >= 0xE && work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = fx_07b_emitRot.vx;
                work->emit4->rot.vy = fx_07b_emitRot.vy;
                work->emit4->rot.vz = fx_07b_emitRot.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07b/nonmatchings/fx_07b", fx_07b_headerPtr);
