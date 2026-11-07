#include "fxmod/fx_047.h"

s32 fx_047_op80(s32 mode, Fx047Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 zWord;

    if (mode == 0) {
        *args = sizeof(Fx047Work);
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_047_env = env;
        ((s32 *)&work->posC)[0] = ((s32 *)&env->targets->centerX)[0];
        zWord = ((s32 *)&env->targets->centerX)[1];
        work->posC.vy = 0;
        ((s32 *)&work->posC)[1] = zWord;
        ((s32 *)&work->pos4)[0] = ((s32 *)&work->posC)[0];
        ((s32 *)&work->pos4)[1] = ((s32 *)&work->posC)[1];
        work->pos4.vy -= FX_FIRA_BURST_HEIGHT;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_FLAME_BURST), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = 0x80;
        }
        work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_HEAT_RING), (s32)&work->posC);
        if (work->emit14 != NULL) {
            work->emit14->otZ = 0x80;
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            if (frame == FX_FIRA_SMOKE_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_SMOKE), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame == FX_FIRA_FLAME_SWIRL_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_FLAME_SWIRL), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                    emitter->trails = (s32)fx_047_trails;
                    emitter->trailSlots = FX_FIRA_FLAME_SWIRL_TRAIL_SLOTS;
                    emitter->trailStride = FX_FIRA_FLAME_SWIRL_TRAIL_STRIDE;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FIRA_FLAME_BURST_2), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            if (frame >= FX_FIRA_FRAMES) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_047/nonmatchings/fx_047", fx_047_headerPtr);
