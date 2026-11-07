#include "fxmod/fx_07f.h"

s32 fx_07f_op80(s32 mode, Fx07fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR scale;
    Pool6CEntry *emitter;
    s32 frame;
    s32 fade;
    s32 t;
    s32 sxz;
    s32 sy;
    s32 tint;

    if (mode == 0) {
        *args = sizeof(Fx07fWork);
    } else if (mode == 1) {
        fx_07f_env = env;
        BTLFX_COPY_SVEC(&work->pos8, &env->targets->centerX);
        BTLFX_COPY_SVEC(&work->pos10, &work->pos8);
        work->modelPos18[0] = work->pos8.vx;
        work->modelPos18[1] = work->pos8.vy;
        work->modelPos18[2] = work->pos8.vz;
        work->rot24.vx = 0;
        work->rot24.vy = 0;
        work->rot24.vz = 0;
        work->state0 = FX_SILENCE_STATE_DOME;
        work->bgFrame4 = 0;
        work->pos10.vy -= FX_SILENCE_KANJI_HEIGHT;
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SILENCE_PINK_PUFF), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = 0x20;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SILENCE_PINK_RING), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = -0x20;
        }
        ex->btlfx_initEffModels(fx_07f_effModels, FX_SILENCE_MODEL_COUNT);
        work->model2C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SILENCE_KANJI_DOME), FX_SILENCE_KANJI_DOME_TPAGE, FX_SILENCE_KANJI_DOME_CLUT, 0, 0);
        work->model30 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SILENCE_VIOLET_RING), FX_SILENCE_VIOLET_RING_TPAGE, FX_SILENCE_VIOLET_RING_CLUT, FX_SILENCE_VIOLET_RING_U, FX_SILENCE_VIOLET_RING_V);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_SILENCE_STATE_DOME:
            t = (frame << 12) / FX_SILENCE_FRAMES;
            fade = ex->math_sin(t >> 1);
            sy = ex->math_sin(t >> 2) * 3;
            sxz = t + 0x1800;
            scale.vx = sxz;
            scale.vy = sy;
            scale.vz = sxz;
            if (frame < FX_SILENCE_TURN_FRAMES) {
                work->rot24.vy = ex->math_sin((frame << 10) / FX_SILENCE_TURN_FRAMES) >> 2;
            } else {
                work->rot24.vy = 0x400;
            }
            ex->Hi_DrawEffModel(&work->rot24, (VECTOR *)work->modelPos18, (s32)&scale, work->model2C);
            ex->Hi_ModifyEffModelAbr(work->model2C, BTLFX_ABR_ADD);
            tint = (fade >> 5) - 0x80;
            ex->Hi_ModifyEffModelRGB(work->model2C, tint, tint, tint);
            scale.vx = sxz;
            scale.vy = sy;
            scale.vz = sxz;
            work->rot24.vy = frame * 0x18;
            ex->Hi_DrawEffModel(&work->rot24, (VECTOR *)work->modelPos18, (s32)&scale, work->model30);
            ex->Hi_ModifyEffModelAbr(work->model30, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model30, tint, tint, tint);
            if (frame == FX_SILENCE_KANJI_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SILENCE_KANJI), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x46;
                }
            }
            if (frame >= FX_SILENCE_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SILENCE_BACKDROP), work->bgFrame4);
        work->bgFrame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07f/nonmatchings/fx_07f", fx_07f_headerPtr);
