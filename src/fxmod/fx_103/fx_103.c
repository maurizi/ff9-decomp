#include "fxmod/fx_103.h"

s32 fx_103_op80(s32 mode, Fx103Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x40;
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX + 0x1F4;
        work->pos8.vy = 0;
        work->pos8.vz = env->targets->centerZ;
        work->pos0.vx = env->targets->centerX - 0x1F4;
        work->pos0.vy = 0;
        work->pos0.vz = env->targets->centerZ;
        work->pos30.vx = env->targets->centerX;
        work->pos30.vy = 0;
        work->pos30.vz = env->targets->centerZ;
        fx_103_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos10);
        ex->btlfx_getUnitArgBonePos(0x10, 1, (u16 *)&work->pos18);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos0, (s16 *)&work->rot20);
        ex->btlfx_aimAngles((s16 *)&work->pos18, (s16 *)&work->pos8, (s16 *)&work->rot28);
        work->rot28.vx = -0x200;
        work->rot20.vx = -0x200;
        work->emit38 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MIST_CLOUD), (s32)&work->pos10);
        if (work->emit38 != NULL) {
            work->emit38->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
        }
        work->emit3C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MIST_CLOUD), (s32)&work->pos18);
        if (work->emit3C != NULL) {
            work->emit3C->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
        }
    } else {
        frame = entry->frame;
        if (work->emit38 != NULL && work->emit38->state == BTLFX_EMIT_DONE) {
            work->emit38 = NULL;
        }
        if (work->emit3C != NULL && work->emit3C->state == BTLFX_EMIT_DONE) {
            work->emit3C = NULL;
        }
        if (work->emit38 != NULL) {
            work->emit38->rot.vx = work->rot20.vx;
            work->emit38->rot.vy = work->rot20.vy;
            work->emit38->rot.vz = work->rot20.vz;
        }
        if (work->emit3C != NULL) {
            work->emit3C->rot.vx = work->rot28.vx;
            work->emit3C->rot.vy = work->rot28.vy;
            work->emit3C->rot.vz = work->rot28.vz;
        }
        if (frame == 0x10) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MIST_SWIRL), (s32)&work->pos30);
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_103/nonmatchings/fx_103", fx_103_headerPtr);
