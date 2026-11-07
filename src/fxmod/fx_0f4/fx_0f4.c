#include "fxmod/fx_0f4.h"

s32 fx_0f4_op80(s32 mode, Fx0f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    s32 elapsed;

    if (mode == 0) {
        *args = sizeof(Fx0f4Work);
    } else if (mode == 1) {
        fx_0f4_env = env;
        ex->btlfx_initEffModels(fx_0f4_effModels, FX_SHIFT_BREAK_MODEL_SLOTS);
        work->pos58.vx = env->targets->centerX;
        work->pos58.vy = env->targets->centerY;
        work->pos58.vz = env->targets->centerZ;
        work->pos58.vx = 0;
        work->rot14.vx = 0;
        work->rot14.vy = 0;
        work->rot14.vz = 0;
        work->scale2C.vx = 0;
        work->scale2C.vy = 0;
        work->scale2C.vz = 0;
        work->rot40.vx = 0;
        work->rot40.vz = 0;
        work->scale48.vx = 0;
        work->scale48.vy = 0;
        work->scale48.vz = 0;
        work->pos1C.vy = work->pos58.vy;
        work->rot40.vy = 0x200;
        work->pos1C.vx = work->pos58.vx;
        work->pos1C.vz = work->pos58.vz;
        work->model6C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SHIFT_BREAK_DOME), FX_SHIFT_BREAK_DOME_TPAGE, FX_SHIFT_BREAK_DOME_CLUT, FX_SHIFT_BREAK_DOME_U, FX_SHIFT_BREAK_DOME_V);
        work->model6E = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SHIFT_BREAK_DOME_2), FX_SHIFT_BREAK_DOME_TPAGE, FX_SHIFT_BREAK_DOME_CLUT, FX_SHIFT_BREAK_DOME_U, FX_SHIFT_BREAK_DOME_V);
        work->startPos60.vx = work->pos1C.vx;
        work->startPos60.vy = work->pos1C.vy;
        work->startPos60.vz = work->pos1C.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIFT_BREAK_WHITE_RINGS), (s32)&work->pos58);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIFT_BREAK_RAINBOW_HALO), (s32)&work->pos58);
        work->fadeInTimer68 = -1;
        work->fadeOutTimer6A = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_SHIFT_BREAK_VIOLET_HALO_FRAME:
            emitter = work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIFT_BREAK_VIOLET_HALO), (s32)&work->pos58);
            if (emitter != NULL) {
                emitter->otZ = -0x100;
            }
            break;
        case FX_SHIFT_BREAK_DOME_FRAME:
            break;
        case FX_SHIFT_BREAK_LIGHTNING_FRAME:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIFT_BREAK_LIGHTNING), (s32)&work->pos58);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHIFT_BREAK_WHITE_GLOW), (s32)&work->pos58);
            if (work->emitC != NULL) {
                work->emitC->otZ = 0x80;
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x100;
            }
            break;
        case FX_SHIFT_BREAK_FADE_OUT_FRAME:
            work->fadeOutTimer6A = FX_SHIFT_BREAK_FADE_OUT_FRAMES;
            break;
        }
        if ((u32)(frame - FX_SHIFT_BREAK_DOME_FRAME) < FX_SHIFT_BREAK_DOME_FRAMES) {
            elapsed = frame - FX_SHIFT_BREAK_DOME_FRAME;
            if (elapsed >= FX_SHIFT_BREAK_DOME_DELAY) {
                ex->Hi_DrawEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, work->model6C);
                ex->Hi_ModifyEffModelAbr(work->model6C, BTLFX_ABR_ADD);
            }
            ex->Hi_DrawEffModel(&work->rot40, &work->pos1C, (s32)&work->scale48, work->model6E);
            ex->Hi_ModifyEffModelAbr(work->model6E, BTLFX_ABR_ADD);
            if (work->fadeInTimer68 >= 0 || work->fadeOutTimer6A >= 0) {
                if (work->fadeInTimer68 >= 0) {
                    work->rgb3C = fxm_lerpRatio(-0x80, 0, FX_SHIFT_BREAK_FADE_IN_FRAMES - work->fadeInTimer68, FX_SHIFT_BREAK_FADE_IN_FRAMES);
                    work->fadeInTimer68--;
                }
                if (work->fadeOutTimer6A >= 0) {
                    work->rgb3C = fxm_lerpRatio(0, -0x100, FX_SHIFT_BREAK_FADE_OUT_FRAMES - work->fadeOutTimer6A, FX_SHIFT_BREAK_FADE_OUT_FRAMES);
                    work->fadeOutTimer6A--;
                }
                if (elapsed >= FX_SHIFT_BREAK_DOME_DELAY) {
                    ex->Hi_ModifyEffModelRGB(work->model6C, work->rgb3C, work->rgb3C, work->rgb3C);
                }
                ex->Hi_ModifyEffModelRGB(work->model6E, work->rgb3C, work->rgb3C, work->rgb3C);
            }
            if (elapsed >= FX_SHIFT_BREAK_DOME_DELAY) {
                work->scale2C.vx += 0x40;
                work->scale2C.vz += 0x40;
                if (elapsed >= FX_SHIFT_BREAK_DOME_RISE_FRAMES) {
                    work->scale2C.vy -= 8;
                } else {
                    work->scale2C.vy += 0x40;
                }
            }
            work->scale48.vx += 0x40;
            work->scale48.vz += 0x40;
            if (elapsed >= FX_SHIFT_BREAK_DOME_2_RISE_FRAMES) {
                work->scale48.vy -= 8;
            } else {
                work->scale48.vy += 0x55;
            }
            work->scale2C.vx = work->scale2C.vx > FX_SHIFT_BREAK_DOME_MAX_WIDTH ? FX_SHIFT_BREAK_DOME_MAX_WIDTH : work->scale2C.vx;
            work->scale2C.vy = work->scale2C.vy > FX_SHIFT_BREAK_DOME_MAX_HEIGHT ? FX_SHIFT_BREAK_DOME_MAX_HEIGHT : work->scale2C.vy;
            work->scale2C.vz = work->scale2C.vz > FX_SHIFT_BREAK_DOME_MAX_WIDTH ? FX_SHIFT_BREAK_DOME_MAX_WIDTH : work->scale2C.vz;
            work->scale48.vx = work->scale48.vx > FX_SHIFT_BREAK_DOME_MAX_WIDTH ? FX_SHIFT_BREAK_DOME_MAX_WIDTH : work->scale48.vx;
            work->scale48.vy = work->scale48.vy > FX_SHIFT_BREAK_DOME_MAX_HEIGHT ? FX_SHIFT_BREAK_DOME_MAX_HEIGHT : work->scale48.vy;
            work->scale48.vz = work->scale48.vz > FX_SHIFT_BREAK_DOME_MAX_WIDTH ? FX_SHIFT_BREAK_DOME_MAX_WIDTH : work->scale48.vz;
        }
        if (frame >= FX_SHIFT_BREAK_DOME_FRAME) {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SHIFT_BREAK_BACKDROP), frame);
        } else {
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_SHIFT_BREAK_BACKDROP_2), frame);
        }
        if (frame >= FX_SHIFT_BREAK_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0f4_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0f4/nonmatchings/fx_0f4", fx_0f4_headerPtr);
