#include "fxmod/fx_031.h"

s32 fx_031_op80(s32 mode, Fx031Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    MATRIX boneMat0;
    MATRIX boneMat1;
    MATRIX camBoneMat;
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = sizeof(Fx031Work);
    } else if (mode == 1) {
        fx_031_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        work->pos8.vy += FX_GRADUAL_PETRIFY_DUST_2_DROP;
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, env->targets->fxArgs[0], (BtlBone *)&boneMat0);
        ex->btlfx_matrixPointAhead(&boneMat0, (s16)env->targets->fxParam, (s32)&work->pos10);
        work->emit30 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GRADUAL_PETRIFY_GOLD_RINGS), (s32)&work->pos10);
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, env->targets->fxArgs[1], (BtlBone *)&boneMat1);
        ex->btlfx_matrixPointAhead(&boneMat1, (s16)env->targets->fxParam, (s32)&work->pos20);
        work->emit34 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GRADUAL_PETRIFY_GOLD_RINGS), (s32)&work->pos20);
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, env->targets->fxArgs[1], (BtlBone *)&camBoneMat);
        ex->btlfx_matrixPointAhead(&camBoneMat, (s16)env->targets->fxParam, (s32)env->camera);
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, env->targets->fxArgs[0], (BtlBone *)&boneMat0);
        ex->btlfx_matrixPointAhead(&boneMat0, (s16)env->targets->fxParam, (s32)&work->pos18);
        if (work->emit30 != NULL && work->emit30->state == BTLFX_EMIT_DONE) {
            work->emit30 = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)work->emit30, (s32)&work->pos18);
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, env->targets->fxArgs[1], (BtlBone *)&boneMat1);
        ex->btlfx_matrixPointAhead(&boneMat1, (s16)env->targets->fxParam, (s32)&work->pos28);
        if (work->emit34 != NULL && work->emit34->state == BTLFX_EMIT_DONE) {
            work->emit34 = NULL;
        }
        ex->btlfx_moveEmitterTo((s32)work->emit34, (s32)&work->pos28);
        if (frame < FX_GRADUAL_PETRIFY_FLASH_FRAMES) {
            s32 tint = (FX_GRADUAL_PETRIFY_FLASH_FRAMES - frame) * 0x10;

            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint, tint, tint);
        }
        if (frame == FX_GRADUAL_PETRIFY_FLASH_FRAMES) {
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GRADUAL_PETRIFY_STONE_DUST_2), (s32)&work->pos8);
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GRADUAL_PETRIFY_STONE_DUST), (s32)&work->pos0);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_GRADUAL_PETRIFY_GOLD_RINGS_2), (s32)&work->pos0);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= FX_GRADUAL_PETRIFY_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_031/nonmatchings/fx_031", fx_031_headerPtr);
