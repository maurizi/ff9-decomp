#include "fxmod/fx_001.h"

INCLUDE_RODATA("asm/us/fx_001/nonmatchings/fx_001", fx_001_headerPtr);

s32 fx_001_op80(s32 mode, Fx001Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT rect;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx001Work);
    } else if (mode == 1) {
        fx_001_env = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos4C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos5C);
        work->pos54.vx = work->pos5C.vx;
        work->pos54.vy = work->pos5C.vy;
        work->pos54.vz = work->pos5C.vz;
        work->pos54.vy -= FX_DAGGERS_MAGIC_SPARKLES_HEIGHT;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DAGGERS_MAGIC_STAR_FLARES), (s32)&work->pos5C);
        work->rot20.vx = 0;
        work->rot20.vy = 0;
        work->rot20.vz = 0;
        work->scale38.vx = 0x800;
        work->scale38.vy = 0x1000;
        work->scale38.vz = 0x800;
        work->pos28.vx = work->pos5C.vx;
        work->pos28.vy = work->pos5C.vy;
        work->pos28.vz = work->pos5C.vz;
        ex->btlfx_initEffModels(fx_001_effModels, FX_DAGGERS_MAGIC_MODEL_COUNT);
        work->model6C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DAGGERS_MAGIC_LIGHT_FOUNTAIN), FX_DAGGERS_MAGIC_LIGHT_TPAGE, FX_DAGGERS_MAGIC_LIGHT_CLUT, 0, 0);
        work->model6E = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_DAGGERS_MAGIC_LIGHT_RING), FX_DAGGERS_MAGIC_LIGHT_TPAGE, FX_DAGGERS_MAGIC_LIGHT_CLUT, 0, 0);
        work->fadeOut68 = work->fadeIn64 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_DAGGERS_MAGIC_BLUE_SWIRL_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DAGGERS_MAGIC_BLUE_SWIRL), (s32)&work->pos5C);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DAGGERS_MAGIC_GOLD_RINGS), (s32)&work->pos5C);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_001_trails;
                work->emit4->trailSlots = FX_DAGGERS_MAGIC_BLUE_SWIRL_TRAIL_SLOTS;
                work->emit4->trailStride = FX_DAGGERS_MAGIC_BLUE_SWIRL_TRAIL_STRIDE;
            }
            break;
        case FX_DAGGERS_MAGIC_FOUNTAIN_FRAME:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DAGGERS_MAGIC_WHITE_RINGS), (s32)&work->pos5C);
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DAGGERS_MAGIC_SPARKLE_BURST), (s32)&work->pos5C);
            work->fadeIn64 = FX_DAGGERS_MAGIC_FADE_IN_FRAMES;
            break;
        case FX_DAGGERS_MAGIC_SPARKLES_FRAME:
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DAGGERS_MAGIC_SPARKLES), (s32)&work->pos54);
            break;
        case FX_DAGGERS_MAGIC_SPARKLE_SHOWER_FRAME:
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DAGGERS_MAGIC_SPARKLE_SHOWER), (s32)&work->pos4C);
            break;
        case FX_DAGGERS_MAGIC_FADE_OUT_FRAME:
            work->fadeOut68 = FX_DAGGERS_MAGIC_FADE_OUT_FRAMES;
            break;
        }
        if (work->fadeIn64 >= 0) {
            work->rgb48 = fxm_lerpRatio(-0x80, 0, FX_DAGGERS_MAGIC_FADE_IN_FRAMES - work->fadeIn64, FX_DAGGERS_MAGIC_FADE_IN_FRAMES);
            work->fadeIn64--;
        }
        if (work->fadeOut68 >= 0) {
            work->rgb48 = fxm_lerpRatio(0, -0x80, FX_DAGGERS_MAGIC_FADE_OUT_FRAMES - work->fadeOut68, FX_DAGGERS_MAGIC_FADE_OUT_FRAMES);
            work->fadeOut68--;
        }
        if (frame >= FX_DAGGERS_MAGIC_FOUNTAIN_FRAME && frame < FX_DAGGERS_MAGIC_FOUNTAIN_FRAME + FX_DAGGERS_MAGIC_FOUNTAIN_GROW_FRAMES) {
            s32 t = ((frame - FX_DAGGERS_MAGIC_FOUNTAIN_FRAME) << 12) / FX_DAGGERS_MAGIC_FOUNTAIN_GROW_FRAMES;
            work->scale38.vy = t / 2;
            ex->Hi_DrawEffModel(&work->rot20, &work->pos28, (s32)&work->scale38, work->model6C);
            ex->Hi_ModifyEffModelAbr(work->model6C, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model6C, work->rgb48, work->rgb48, work->rgb48);
        }
        if (frame >= FX_DAGGERS_MAGIC_FOUNTAIN_FRAME + FX_DAGGERS_MAGIC_FOUNTAIN_GROW_FRAMES &&
            frame < FX_DAGGERS_MAGIC_FOUNTAIN_FRAME + FX_DAGGERS_MAGIC_FOUNTAIN_GROW_FRAMES + FX_DAGGERS_MAGIC_RING_MORPH_FRAMES) {
            ex->Hi_DrawMorphEffModel(&work->rot20, &work->pos28, (s32)&work->scale38, work->model6C, work->model6E,
                                     0x1000 - ((frame - (FX_DAGGERS_MAGIC_FOUNTAIN_FRAME + FX_DAGGERS_MAGIC_FOUNTAIN_GROW_FRAMES)) << 12) /
                                                FX_DAGGERS_MAGIC_RING_MORPH_FRAMES);
            ex->Hi_ModifyEffModelAbr(work->model6C, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model6C, work->rgb48, work->rgb48, work->rgb48);
        }
        if (frame >= FX_DAGGERS_MAGIC_FOUNTAIN_FRAME) {
            s16 *texXY = fx_001_texAnimXY[frame % FX_DAGGERS_MAGIC_LIGHT_ANIM_FRAMES];
            rect.x = texXY[0];
            rect.y = texXY[1];
            rect.w = FX_DAGGERS_MAGIC_LIGHT_VRAM_W;
            rect.h = FX_DAGGERS_MAGIC_LIGHT_VRAM_H;
            ex->MoveImage(&rect, FX_DAGGERS_MAGIC_LIGHT_VRAM_X, FX_DAGGERS_MAGIC_LIGHT_VRAM_Y);
        }
        if (frame >= FX_DAGGERS_MAGIC_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_001_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
