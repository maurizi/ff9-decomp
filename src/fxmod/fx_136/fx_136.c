#include "fxmod/fx_136.h"

s32 fx_136_op80(s32 mode, Fx136Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx136Work);
    } else if (mode == 1) {
        fx_136_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, 2, (BtlBone *)&m);
        ex->btlfx_matrixPointAhead(&m, FX_ABSORB_SPARKLE_REACH, (s32)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ABSORB_SPARKLE), (s32)&work->pos4);
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->target = (s32)&work->posC;
            }
        }
        return frame >= FX_ABSORB_SPARKLE_FRAMES;
    }
    return 0;
}

s32 fx_136_op81(s32 mode, Fx136Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx136Work);
    } else if (mode == 1) {
        fx_136_env = env;
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_ABSORB_SMOKE_BONE, (u16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ABSORB_SMOKE_SWIRL), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x20);
        }
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->pos4);
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->pos4.vx;
                work->emit0->rot.vy = work->pos4.vy;
                work->emit0->rot.vz = work->pos4.vz;
            }
        }
        return frame >= FX_ABSORB_SMOKE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_136/nonmatchings/fx_136", fx_136_headerPtr);
