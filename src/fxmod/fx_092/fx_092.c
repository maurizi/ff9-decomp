#include "fxmod/fx_092.h"

s32 fx_092_op80(s32 mode, Fx092Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx092Work);
    } else if (mode == 1) {
        fx_092_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot18);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos20);
        work->pos30.vx = work->pos20.vx;
        work->pos30.vy = work->pos20.vy;
        work->pos30.vz = work->pos20.vz;
        work->pos38.vx = work->pos20.vx;
        work->pos38.vy = work->pos20.vy;
        work->pos38.vz = work->pos20.vz;
        work->pos30.vy -= FX_HYPNOTIZE_HALO_HEIGHT;
        work->pos38.vy -= FX_HYPNOTIZE_DOTS_HEIGHT;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos28);
        ex->btlfx_pointAhead((s16 *)&work->pos28, (s16 *)&work->rot18, FX_HYPNOTIZE_REACH, (s16 *)&work->pos28);
        env->camera->points[0].vx = work->pos28.vx;
        env->camera->points[0].vy = work->pos28.vy;
        env->camera->points[0].vz = work->pos28.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HYPNOTIZE_MAGENTA_RINGS), (s32)&work->pos28);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
        }
    } else {
        frame = entry->frame;
        ex->btlfx_aimAngles(&env->camera->eye.vx, (s16 *)&work->pos28, (s16 *)&work->aim40);
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim40.vx;
                work->emit0->rot.vy = work->aim40.vy;
                work->emit0->rot.vz = work->aim40.vz;
            }
        }
        if (frame == FX_HYPNOTIZE_RINGS_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HYPNOTIZE_HALO_RINGS), (s32)&work->pos30);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HYPNOTIZE_VIOLET_RING), (s32)&work->pos20);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HYPNOTIZE_SPARKLE_DOTS), (s32)&work->pos38);
        }
        return frame >= FX_HYPNOTIZE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_092/nonmatchings/fx_092", fx_092_headerPtr);
