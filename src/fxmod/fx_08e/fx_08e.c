#include "fxmod/fx_08e.h"

INCLUDE_RODATA("asm/us/fx_08e/nonmatchings/fx_08e", fx_08e_headerPtr);

s32 fx_08e_op80(s32 mode, Fx08eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT rect;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx08eWork);
    } else if (mode == 1) {
        fx_08e_env = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos48);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos58);
        work->pos50.vx = work->pos58.vx;
        work->pos50.vy = work->pos58.vy;
        work->pos50.vz = work->pos58.vz;
        work->pos50.vy -= FX_CURAGA_SPARKLES_HEIGHT;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_STAR_FLARES), (s32)&work->pos58);
        work->rot1C.vx = 0;
        work->rot1C.vy = 0;
        work->rot1C.vz = 0;
        work->scale34.vx = 0x800;
        work->scale34.vy = 0x1000;
        work->scale34.vz = 0x800;
        work->pos24.vx = work->pos58.vx;
        work->pos24.vy = work->pos58.vy;
        work->pos24.vz = work->pos58.vz;
        ex->btlfx_initEffModels(fx_08e_effModels, FX_CURAGA_MODEL_COUNT);
        work->model68 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_CURAGA_LIGHT_FOUNTAIN), FX_CURAGA_FOUNTAIN_TPAGE, FX_CURAGA_FOUNTAIN_CLUT, 0, 0);
        work->model6A = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_CURAGA_LIGHT_RING), FX_CURAGA_FOUNTAIN_TPAGE, FX_CURAGA_FOUNTAIN_CLUT, 0, 0);
        work->fadeOut64 = work->fadeIn60 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_CURAGA_SWIRL_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_BLUE_SWIRL), (s32)&work->pos58);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_GOLD_RING), (s32)&work->pos58);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_08e_trails;
                work->emit4->trailSlots = FX_CURAGA_BLUE_SWIRL_TRAIL_SLOTS;
                work->emit4->trailStride = FX_CURAGA_BLUE_SWIRL_TRAIL_STRIDE;
            }
            break;
        case FX_CURAGA_FOUNTAIN_FRAME:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_WHITE_RINGS), (s32)&work->pos58);
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_SPARKLE_BURST), (s32)&work->pos58);
            work->fadeIn60 = FX_CURAGA_FADE_IN_FRAMES;
            break;
        case FX_CURAGA_SPARKLES_FRAME:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_SPARKLES), (s32)&work->pos50);
            break;
        case FX_CURAGA_SHOWER_FRAME:
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_SPARKLE_SHOWER), (s32)&work->pos48);
            break;
        case FX_CURAGA_FADE_OUT_FRAME:
            work->fadeOut64 = FX_CURAGA_FADE_OUT_FRAMES;
            break;
        }
        if (work->fadeIn60 >= 0) {
            work->bright44 = fxm_lerpRatio(-0x80, 0, FX_CURAGA_FADE_IN_FRAMES - work->fadeIn60, FX_CURAGA_FADE_IN_FRAMES);
            work->fadeIn60--;
        }
        if (work->fadeOut64 >= 0) {
            work->bright44 = fxm_lerpRatio(0, -0x80, FX_CURAGA_FADE_OUT_FRAMES - work->fadeOut64, FX_CURAGA_FADE_OUT_FRAMES);
            work->fadeOut64--;
        }
        if (frame >= FX_CURAGA_FOUNTAIN_FRAME && frame < FX_CURAGA_FOUNTAIN_FRAME + FX_CURAGA_FOUNTAIN_GROW_FRAMES) {
            s32 t = ((frame - FX_CURAGA_FOUNTAIN_FRAME) << 12) / FX_CURAGA_FOUNTAIN_GROW_FRAMES;
            work->scale34.vy = t / 2;
            ex->Hi_DrawEffModel(&work->rot1C, &work->pos24, (s32)&work->scale34, work->model68);
            ex->Hi_ModifyEffModelAbr(work->model68, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model68, work->bright44, work->bright44, work->bright44);
        }
        if (frame >= FX_CURAGA_FOUNTAIN_FRAME + FX_CURAGA_FOUNTAIN_GROW_FRAMES &&
            frame < FX_CURAGA_FOUNTAIN_FRAME + FX_CURAGA_FOUNTAIN_GROW_FRAMES + FX_CURAGA_FOUNTAIN_MORPH_FRAMES) {
            ex->Hi_DrawMorphEffModel(&work->rot1C, &work->pos24, (s32)&work->scale34, work->model68, work->model6A,
                                     0x1000 - ((frame - (FX_CURAGA_FOUNTAIN_FRAME + FX_CURAGA_FOUNTAIN_GROW_FRAMES)) << 12) /
                                                FX_CURAGA_FOUNTAIN_MORPH_FRAMES);
            ex->Hi_ModifyEffModelAbr(work->model68, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model68, work->bright44, work->bright44, work->bright44);
        }
        if (frame >= FX_CURAGA_FOUNTAIN_FRAME) {
            s16 *texXY = fx_08e_texAnimSrc[frame % FX_CURAGA_FOUNTAIN_ANIM_FRAMES];
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
    BTLFX_EXPORTS(fx_08e_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
