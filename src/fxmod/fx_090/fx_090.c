#include "fxmod/fx_090.h"

s32 fx_090_op80(s32 mode, Fx090Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT rect;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx090Work);
    } else if (mode == 1) {
        work->pos50.vx = env->targets->centerX;
        work->pos50.vy = env->targets->centerY;
        work->pos50.vz = env->targets->centerZ;
        work->pos58.vx = env->targets->centerX;
        work->pos58.vy = env->targets->centerY;
        work->pos58.vz = env->targets->centerZ;
        work->pos60.vx = env->targets->centerX;
        work->pos60.vy = env->targets->centerY;
        work->pos60.vz = env->targets->centerZ;
        work->pos58.vy -= FX_CURAGA_SPARKLES_HEIGHT;
        work->pos60.vy -= 0x4B0;
        fx_090_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_BLUE_SWIRL), (s32)&work->pos50);
        work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_TEAL_RING), (s32)&work->pos50);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_090_trails;
            work->emit0->trailSlots = FX_CURAGA_BLUE_SWIRL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_CURAGA_BLUE_SWIRL_TRAIL_STRIDE;
        }
        if (work->emitC != NULL) {
            work->emitC->otZ = 0x180;
        }
        work->rot24.vx = 0;
        work->rot24.vy = 0;
        work->rot24.vz = 0;
        work->pos2C.vx = env->targets->centerX;
        work->pos2C.vy = env->targets->centerY;
        work->pos2C.vz = env->targets->centerZ;
        work->scale3C.vx = 0x1000;
        work->scale3C.vy = 0x1000;
        work->scale3C.vz = 0x1000;
        ex->btlfx_initEffModels(fx_090_effModels, FX_CURAGA_MODEL_COUNT);
        work->model70 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_CURAGA_LIGHT_FOUNTAIN), FX_CURAGA_FOUNTAIN_TPAGE, FX_CURAGA_FOUNTAIN_CLUT, 0, 0);
        work->model72 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_CURAGA_LIGHT_RING), FX_CURAGA_FOUNTAIN_TPAGE, FX_CURAGA_FOUNTAIN_CLUT, 0, 0);
        work->fadeOut6C = work->fadeIn68 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_CURAGA_FOUNTAIN_FRAME:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_WHITE_RINGS), (s32)&work->pos50);
            work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_SPARKLE_BURST), (s32)&work->pos50);
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x180;
            }
            work->fadeIn68 = FX_CURAGA_FADE_IN_FRAMES;
            break;
        case FX_CURAGA_SPARKLES_FRAME:
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_SPARKLES), (s32)&work->pos58);
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_STAR_FLARES), (s32)&work->pos58);
            break;
        case FX_CURAGA_SHOWER_FRAME:
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_SPARKLE_SHOWER), (s32)&work->pos58);
            break;
        case FX_CURAGA_FADE_OUT_FRAME:
            work->fadeOut6C = FX_CURAGA_FADE_OUT_FRAMES;
            break;
        }
        if (work->fadeIn68 >= 0) {
            work->bright4C = fxm_lerpRatio(-0x80, 0, FX_CURAGA_FADE_IN_FRAMES - work->fadeIn68, FX_CURAGA_FADE_IN_FRAMES);
            work->fadeIn68--;
        }
        if (work->fadeOut6C >= 0) {
            work->bright4C = fxm_lerpRatio(0, -0x80, FX_CURAGA_FADE_OUT_FRAMES - work->fadeOut6C, FX_CURAGA_FADE_OUT_FRAMES);
            work->fadeOut6C--;
        }
        if (frame >= FX_CURAGA_FOUNTAIN_FRAME && frame < FX_CURAGA_FOUNTAIN_FRAME + FX_CURAGA_FOUNTAIN_GROW_FRAMES) {
            work->scale3C.vy = ((frame - FX_CURAGA_FOUNTAIN_FRAME) << 12) / FX_CURAGA_FOUNTAIN_GROW_FRAMES;
            ex->Hi_DrawEffModel(&work->rot24, &work->pos2C, (s32)&work->scale3C, work->model70);
            ex->Hi_ModifyEffModelAbr(work->model70, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model70, work->bright4C, work->bright4C, work->bright4C);
        }
        if (frame >= FX_CURAGA_FOUNTAIN_FRAME + FX_CURAGA_FOUNTAIN_GROW_FRAMES &&
            frame < FX_CURAGA_FOUNTAIN_FRAME + FX_CURAGA_FOUNTAIN_GROW_FRAMES + FX_CURAGA_FOUNTAIN_MORPH_FRAMES) {
            ex->Hi_DrawMorphEffModel(&work->rot24, &work->pos2C, (s32)&work->scale3C, work->model70, work->model72,
                                     0x1000 - ((frame - (FX_CURAGA_FOUNTAIN_FRAME + FX_CURAGA_FOUNTAIN_GROW_FRAMES)) << 12) /
                                                FX_CURAGA_FOUNTAIN_MORPH_FRAMES);
            ex->Hi_ModifyEffModelAbr(work->model70, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model70, work->bright4C, work->bright4C, work->bright4C);
        }
        if (frame >= FX_CURAGA_FOUNTAIN_FRAME) {
            s16 *texXY = fx_090_texAnimSrc[frame % FX_CURAGA_FOUNTAIN_ANIM_FRAMES];
            rect.x = texXY[0];
            rect.y = texXY[1];
            rect.w = FX_CURAGA_FOUNTAIN_VRAM_W;
            rect.h = FX_CURAGA_FOUNTAIN_VRAM_H;
            ex->MoveImage(&rect, FX_CURAGA_FOUNTAIN_VRAM_X, FX_CURAGA_FOUNTAIN_VRAM_Y);
        }
        if (frame >= FX_CURAGA_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_090_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_090/nonmatchings/fx_090", fx_090_headerPtr);
