#include "fxmod/fx_030.h"

s32 fx_030_op80(s32 mode, Fx030Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx030Work);
    } else if (mode == 1) {
        fx_030_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos0);
        work->pos8.vx = env->targets->centerX;
        work->pos8.vz = env->targets->centerZ + FX_POLLEN_AIM_DEPTH;
        work->pos8.vy = 0;
        ex->btlfx_aimAngles((s16 *)&work->pos0, (s16 *)&work->pos8, (s16 *)&work->aim10);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos0, (s16 *)&work->aim18);
        work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POLLEN_POLLEN_PUFF), (s32)&work->pos0);
        work->emit24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POLLEN_POLLEN_DOTS), (s32)&work->pos0);
        work->emit28 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POLLEN_POLLEN_DOTS_2), (s32)&work->pos0);
    } else {
        frame = entry->frame;
        if (work->emit20 != NULL && work->emit20->state == BTLFX_EMIT_DONE) {
            work->emit20 = NULL;
        }
        if (work->emit24 != NULL && work->emit24->state == BTLFX_EMIT_DONE) {
            work->emit24 = NULL;
        }
        if (work->emit28 != NULL && work->emit28->state == BTLFX_EMIT_DONE) {
            work->emit28 = NULL;
        }
        if (work->emit20 != NULL) {
            work->emit20->rot.vx = work->aim10.vx;
            work->emit20->rot.vy = work->aim10.vy;
            work->emit20->rot.vz = work->aim10.vz;
        }
        if (work->emit24 != NULL) {
            work->emit24->rot.vx = work->aim18.vx;
            work->emit24->rot.vy = work->aim18.vy;
            work->emit24->rot.vz = work->aim18.vz;
        }
        if (work->emit28 != NULL) {
            work->emit28->rot.vx = work->aim10.vx;
            work->emit28->rot.vy = work->aim10.vy;
            work->emit28->rot.vz = work->aim10.vz;
        }
        if (frame == FX_POLLEN_DUST_PUFF_FRAME) {
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&work->pos2C[i]);
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_POLLEN_DUST_PUFF), (s32)&work->pos2C[i]);
            }
        }
        return frame >= FX_POLLEN_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_030/nonmatchings/fx_030", fx_030_headerPtr);
