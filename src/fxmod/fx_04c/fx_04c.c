#include "fxmod/fx_04c.h"

s32 fx_04c_op80(s32 mode, Fx04cWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 zWord;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_04c_env = env;
        ((s32 *)&work->pos4)[0] = ((s32 *)&env->targets->centerX)[0];
        zWord = ((s32 *)&env->targets->centerX)[1];
        work->pos4.vy = 0;
        ((s32 *)&work->pos4)[1] = zWord;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLOAT_GLOW), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = -0x12C;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLOAT_GLOW_2), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = -0x258;
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_FLOAT_BACKDROP), frame);
            if (frame >= 0x3C) {
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_04c_op81(s32 mode, Fx04cWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0x3C;
    } else if (mode == 1) {
        fx_04c_env = env;
        work->count34 = env->targets->targetCount;
        env->state->signals[2] = 0;
        for (i = 0; i < work->count34; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&work->pos4[i]);
            work->emit24[i] = NULL;
        }
        work->state0 = 0;
        work->target38 = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            work->emit24[work->target38] =
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLOAT_STAR_FLASH), (s32)&work->pos4[work->target38]);
            if (work->emit24[work->target38] != NULL) {
                work->emit24[work->target38]->otZ = ex->btlfx_addRadiusSixth(work->target38, 0x40);
            }
            work->state0 = 1;
            entry->frame = -1;
            break;
        case 1:
            if (frame > 0) {
                if (++work->target38 < work->count34) {
                    work->state0 = 0;
                    entry->frame = -1;
                } else {
                    work->state0 = 2;
                    entry->frame = -1;
                }
            }
            break;
        case 2:
            if (frame >= 0xA) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_04c_op82(s32 mode, Fx04cWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0x3C;
    } else if (mode == 1) {
        fx_04c_env = env;
        work->count34 = env->targets->targetCount;
        env->state->signals[2] = 0;
        for (i = 0; i < work->count34; i++) {
            ex->btlfx_getUnitPos(i, (s16 *)&work->pos4[i]);
            work->emit24[i] = NULL;
        }
        work->state0 = 0;
        work->target38 = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            work->emit24[work->target38] =
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FLOAT_SPECK_RING), (s32)&work->pos4[work->target38]);
            if (work->emit24[work->target38] != NULL) {
                work->emit24[work->target38]->otZ = ex->btlfx_addRadiusSixth(work->target38, 0x40);
            }
            work->state0 = 1;
            entry->frame = -1;
            break;
        case 1:
            if (frame > 0) {
                if (++work->target38 < work->count34) {
                    work->state0 = 0;
                    entry->frame = -1;
                } else {
                    work->state0 = 2;
                    env->state->signals[2] = 1;
                    entry->frame = -1;
                }
            }
            break;
        case 2:
            if (frame >= 0xA) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_04c/nonmatchings/fx_04c", fx_04c_headerPtr);
