#include "fxmod/fx_093.h"

s32 fx_093_op80(s32 mode, Fx093Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0x2C;
    } else if (mode == 1) {
        fx_093_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos0);
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], &bone);
        ex->btlfx_matrixPointAhead((MATRIX *)&bone, 0x100, (s32)&work->pos8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
        ex->btlfx_aimAngles((s16 *)&work->pos0, (s16 *)&work->pos8, (s16 *)&work->aim20);
        work->emit28 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos0);
        if (work->emit28 != NULL) {
            work->emit28->otZ = ex->btlfx_addRadiusSixth(0x10, 0x40);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos0);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x40);
        }
    } else {
        frame = entry->frame;
        if (work->emit28 != NULL) {
            if (work->emit28->state == BTLFX_EMIT_DONE) {
                work->emit28 = NULL;
            }
            if (work->emit28 != NULL) {
                work->emit28->rot.vx = work->aim20.vx;
                work->emit28->rot.vy = work->aim20.vy;
                work->emit28->rot.vz = work->aim20.vz;
            }
        }
        if (frame == 8) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos18);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, -0x40);
            }
        }
        return frame >= 0x43;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_093/nonmatchings/fx_093", fx_093_headerPtr);
