#include "fxmod/fx_094.h"

s32 fx_094_op80(s32 mode, Fx094Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx094Work);
    } else if (mode == 1) {
        fx_094_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DELTA_ATTACK_COLOR_RINGS), (s32)&work->pos0);
        ex->btlfx_aimAngles((s16 *)&work->pos0, (s16 *)&work->pos8, (s16 *)&work->aim18);
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_DELTA_ATTACK_WHITE_RING_2_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DELTA_ATTACK_WHITE_RING_2), (s32)&work->pos10);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_DELTA_ATTACK_VIOLET_RING_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DELTA_ATTACK_VIOLET_RING), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_DELTA_ATTACK_RAINBOW_STREAK_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DELTA_ATTACK_RAINBOW_STREAK), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_DELTA_ATTACK_WHITE_RING_FRAME:
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DELTA_ATTACK_WHITE_RING), (s32)&work->pos10);
            break;
        }
        if (work->emit20 != NULL) {
            if (work->emit20->state == BTLFX_EMIT_DONE) {
                work->emit20 = NULL;
            }
            if (work->emit20 != NULL) {
                work->emit20->rot.vx = work->aim18.vx;
                work->emit20->rot.vy = work->aim18.vy;
                work->emit20->rot.vz = work->aim18.vz;
            }
        }
        return frame >= FX_DELTA_ATTACK_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_094/nonmatchings/fx_094", fx_094_headerPtr);
