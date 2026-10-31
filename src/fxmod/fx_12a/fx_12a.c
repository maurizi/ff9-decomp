#include "fxmod/fx_12a.h"

s32 fx_12a_op80(s32 mode, Fx12aWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        fx_12a_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos20);
        ex->btlfx_getUnitArgBonePos(0x10, 1, (u16 *)&work->pos28);
        work->pos30.vx = work->pos20.vx;
        work->pos30.vy = work->pos20.vy;
        work->pos30.vz = work->pos20.vz;
        work->pos38.vx = work->pos28.vx;
        work->pos38.vy = work->pos28.vy;
        work->pos38.vz = work->pos28.vz;
        work->pos20.vy += 0x64;
        work->pos28.vy += 0x64;
        work->pos20.vz -= 0x64;
        work->pos28.vz -= 0x64;
        work->pos40.vx = env->targets->centerX;
        work->pos40.vy = env->targets->centerY;
        work->pos40.vz = env->targets->centerZ;
        work->pos48.vx = env->targets->centerX;
        work->pos48.vy = env->targets->centerY;
        work->pos48.vz = env->targets->centerZ;
        work->pos40.vx = work->pos20.vx;
        work->pos48.vx = work->pos28.vx;
        work->pos40.vy -= 0x190;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos20);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos20);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos28);
        work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos28);
        if (work->emit8 != NULL) {
            work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emitC != NULL) {
            work->emitC->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos20, (s16 *)&work->pos40, (s16 *)&work->rot50);
        ex->btlfx_aimAngles((s16 *)&work->pos28, (s16 *)&work->pos48, (s16 *)&work->rot58);
    } else {
        frame = entry->frame;
        if (frame == 6) {
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos30);
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos20);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos38);
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos28);
            if (work->emit18 != NULL) {
                work->emit18->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emit1C != NULL) {
                work->emit1C->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
        }
        if (frame >= 6) {
            if (work->emit10 != NULL) {
                if (work->emit10->state == BTLFX_EMIT_DONE) {
                    work->emit10 = NULL;
                }
                if (work->emit10 != NULL) {
                    work->emit10->rot.vx = work->rot50.vx;
                    work->emit10->rot.vy = work->rot50.vy;
                    work->emit10->rot.vz = work->rot50.vz;
                }
            }
            if (work->emit18 != NULL) {
                if (work->emit18->state == BTLFX_EMIT_DONE) {
                    work->emit18 = NULL;
                }
                if (work->emit18 != NULL) {
                    work->emit18->rot.vx = work->rot50.vx;
                    work->emit18->rot.vy = work->rot50.vy;
                    work->emit18->rot.vz = work->rot50.vz;
                }
            }
            if (work->emit14 != NULL) {
                if (work->emit14->state == BTLFX_EMIT_DONE) {
                    work->emit14 = NULL;
                }
                if (work->emit14 != NULL) {
                    work->emit14->rot.vx = work->rot58.vx;
                    work->emit14->rot.vy = work->rot58.vy;
                    work->emit14->rot.vz = work->rot58.vz;
                }
            }
            if (work->emit1C != NULL) {
                if (work->emit1C->state == BTLFX_EMIT_DONE) {
                    work->emit1C = NULL;
                }
                if (work->emit1C != NULL) {
                    work->emit1C->rot.vx = work->rot58.vx;
                    work->emit1C->rot.vy = work->rot58.vy;
                    work->emit1C->rot.vz = work->rot58.vz;
                }
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

s32 fx_12a_op81(s32 mode, Fx12aWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->pos4.vx = env->targets->centerX;
        work->pos4.vy = env->targets->centerY;
        work->pos4.vz = env->targets->centerZ;
        fx_12a_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos4);
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_12a_op82(s32 mode, Fx12aWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 n;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        n = *arg4;
        work->unit10 = n;
        fx_12a_env = env;
        if (n >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_getUnitPos(work->unit10, (s16 *)&work->pos8);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->unit10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 4) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= 0x28;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 i, s32 v) {
    env->state->signals[i] = v;
}

INCLUDE_RODATA("asm/us/fx_12a/nonmatchings/fx_12a", fx_12a_headerPtr);
