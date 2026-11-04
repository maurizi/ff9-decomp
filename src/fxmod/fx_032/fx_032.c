#include "fxmod/fx_032.h"

s32 fx_032_op80(s32 mode, Fx032Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    MATRIX m0;
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    MATRIX m4;
    MATRIX m5;
    MATRIX m6;
    MATRIX m7;

    if (mode == 0) {
        *args = 0x58;
    } else if (mode == 1) {
        fx_032_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[1], (BtlBone *)&m0);
        ex->btlfx_matrixPointAhead(&m0, (s16)env->targets->fxParam, (s32)env->camera);
    } else {
        frame = entry->frame;
        if (frame == 4) {
            ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[1], (BtlBone *)&m0);
            ex->btlfx_matrixPointAhead(&m0, (s16)env->targets->fxParam, (s32)&work->pos8);
            work->emit48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GLOWING_EYES_SPARK_DOTS), (s32)&work->pos8);
            ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[1], (BtlBone *)&m1);
            ex->btlfx_matrixPointAhead(&m1, (s16)env->targets->fxParam, (s32)&work->pos18);
            work->emit4C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GLOWING_EYES_GLOW_RINGS), (s32)&work->pos18);
            ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], (BtlBone *)&m2);
            ex->btlfx_matrixPointAhead(&m2, (s16)env->targets->fxParam, (s32)&work->pos28);
            work->emit50 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GLOWING_EYES_SPARK_DOTS), (s32)&work->pos28);
            ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], (BtlBone *)&m3);
            ex->btlfx_matrixPointAhead(&m3, (s16)env->targets->fxParam, (s32)&work->pos38);
            work->emit54 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GLOWING_EYES_GLOW_RINGS_2), (s32)&work->pos38);
        }
        if (frame >= 5) {
            if (work->emit48 != NULL) {
                if (work->emit48->state == BTLFX_EMIT_DONE) {
                    work->emit48 = NULL;
                }
                if (work->emit48 != NULL) {
                    ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[1], (BtlBone *)&m4);
                    ex->btlfx_matrixPointAhead(&m4, (s16)env->targets->fxParam, (s32)&work->pos10);
                    ex->btlfx_moveEmitterTo((s32)work->emit48, (s32)&work->pos10);
                }
            }
            if (work->emit4C != NULL) {
                if (work->emit4C->state == BTLFX_EMIT_DONE) {
                    work->emit4C = NULL;
                }
                if (work->emit4C != NULL) {
                    ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[1], (BtlBone *)&m5);
                    ex->btlfx_matrixPointAhead(&m5, (s16)env->targets->fxParam, (s32)&work->pos20);
                    ex->btlfx_moveEmitterTo((s32)work->emit4C, (s32)&work->pos20);
                }
            }
            if (work->emit50 != NULL) {
                if (work->emit50->state == BTLFX_EMIT_DONE) {
                    work->emit50 = NULL;
                }
                if (work->emit50 != NULL) {
                    ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], (BtlBone *)&m6);
                    ex->btlfx_matrixPointAhead(&m6, (s16)env->targets->fxParam, (s32)&work->pos30);
                    ex->btlfx_moveEmitterTo((s32)work->emit50, (s32)&work->pos30);
                }
            }
            if (work->emit54 != NULL) {
                if (work->emit54->state == BTLFX_EMIT_DONE) {
                    work->emit54 = NULL;
                }
                if (work->emit54 != NULL) {
                    ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], (BtlBone *)&m7);
                    ex->btlfx_matrixPointAhead(&m7, (s16)env->targets->fxParam, (s32)&work->pos40);
                    ex->btlfx_moveEmitterTo((s32)work->emit54, (s32)&work->pos40);
                }
            }
        }
        if (frame == 0x10) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GLOWING_EYES_SMOKE), (s32)&work->pos0);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GLOWING_EYES_SWIRL), (s32)&work->pos0);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_032/nonmatchings/fx_032", fx_032_headerPtr);
