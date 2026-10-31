#include "fxmod/fx_04c.h"

s32 fx_04c_op80(s32 mode, Fx04cWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->stop0 = 0;
        fx_04c_env = env;
        ((s32 *)&work->pos4)[0] = ((s32 *)&env->targets->centerX)[0];
        t = ((s32 *)&env->targets->centerX)[1];
        work->pos4.vy = 0;
        ((s32 *)&work->pos4)[1] = t;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos4);
        if (p != NULL) {
            p->otZ = -0x12C;
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
        if (p != NULL) {
            p->otZ = -0x258;
        }
    } else {
        frame = entry->frame;
        if (work->stop0 == 0) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 5), frame);
            if (frame >= 0x3C) {
                return 1;
            }
        }
    }
    return 0;
}

s32 fx_04c_op81(s32 mode, Fx04cWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x3C;
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
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos4[work->target38]);
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

s32 fx_04c_op82(s32 mode, Fx04cWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x3C;
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
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos4[work->target38]);
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
