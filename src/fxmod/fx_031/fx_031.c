#include "fxmod/fx_031.h"

s32 fx_031_op80(s32 mode, Fx031Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    MATRIX m1;
    MATRIX m2;
    MATRIX m3;
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 0x38;
    } else if (mode == 1) {
        fx_031_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        work->pos8.vy += 0x80;
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], (BtlBone *)&m1);
        ex->btlfx_matrixPointAhead(&m1, (s16)env->targets->fxParam, (s32)&work->pos10);
        work->emit30 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[1], (BtlBone *)&m2);
        ex->btlfx_matrixPointAhead(&m2, (s16)env->targets->fxParam, (s32)&work->pos20);
        work->emit34 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos20);
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[1], (BtlBone *)&m3);
        ex->btlfx_matrixPointAhead(&m3, (s16)env->targets->fxParam, (s32)env->camera);
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], (BtlBone *)&m1);
        ex->btlfx_matrixPointAhead(&m1, (s16)env->targets->fxParam, (s32)&work->pos18);
        if (work->emit30 != NULL && work->emit30->state == BTLFX_EMIT_DONE) {
            work->emit30 = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)work->emit30, (s32)&work->pos18);
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[1], (BtlBone *)&m2);
        ex->btlfx_matrixPointAhead(&m2, (s16)env->targets->fxParam, (s32)&work->pos28);
        if (work->emit34 != NULL && work->emit34->state == BTLFX_EMIT_DONE) {
            work->emit34 = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)work->emit34, (s32)&work->pos28);
        if (frame < 8) {
            s32 c = (8 - frame) * 0x10;

            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c, c, c);
        }
        if (frame == 8) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos8);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos0);
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos0);
            if (p != NULL) {
                p->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_031/nonmatchings/fx_031", fx_031_headerPtr);
