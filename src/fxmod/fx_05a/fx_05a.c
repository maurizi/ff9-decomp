#include "fxmod/fx_05a.h"

s32 fx_05a_op80(s32 mode, Fx05aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 size;
    s32 height;
    s32 radius;
    s32 elapsed;

    if (mode == 0) {
        *args = sizeof(Fx05aWork);
    } else if (mode == 1) {
        fx_05a_env = env;
        ex->btlfx_initEffModels(fx_05a_effModels, FX_MIGHT_MODEL_COUNT);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->pos10.vx = work->pos8.vx;
        work->pos10.vy = work->pos8.vy;
        work->pos10.vz = work->pos8.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MIGHT_LIGHT_TRAIL), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_05a_trails;
            work->emit0->trailSlots = FX_MIGHT_LIGHT_TRAIL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_MIGHT_LIGHT_TRAIL_TRAIL_STRIDE;
        }
        work->rot18.vx = 0;
        work->rot18.vy = 0;
        work->rot18.vz = 0;
        work->pos20.vx = work->pos8.vx;
        work->pos20.vy = work->pos8.vy;
        work->pos20.vz = work->pos8.vz;
        if (ex->btlfx_isEnemyUnit(0)) {
            size = FX_MIGHT_STAR_SCALE;
            height = ex->btlfx_getUnitHeight(0);
            radius = ex->btlfx_getUnitRadius(0);
            if (height > FX_MIGHT_BIG_HEIGHT) {
                size += (height - FX_MIGHT_BIG_HEIGHT) / 4;
            }
            if (radius > FX_MIGHT_BIG_RADIUS) {
                size += (radius - FX_MIGHT_BIG_RADIUS) / 4;
            }
            work->pos10.vy -= (size - FX_MIGHT_STAR_SCALE) * 3;
            if (size > FX_MIGHT_STAR_MAX_SCALE) {
                size = FX_MIGHT_STAR_MAX_SCALE;
            }
            work->scale30.vx = size;
            work->scale30.vy = size;
            work->scale30.vz = size;
        } else {
            work->scale30.vx = FX_MIGHT_STAR_SCALE;
            work->scale30.vy = FX_MIGHT_STAR_SCALE;
            work->scale30.vz = FX_MIGHT_STAR_SCALE;
        }
        work->model44 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_MIGHT_STAR), FX_MIGHT_STAR_TPAGE, 0, 0, FX_MIGHT_STAR_V);
        work->rgb40 = -0x14;
        work->tint42 = FX_MIGHT_FLASH_LEVEL;
    } else {
        frame = entry->frame;
        if (frame == FX_MIGHT_STAR_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MIGHT_SWIRL), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame >= FX_MIGHT_STAR_FRAME) {
            ex->Hi_DrawEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model44);
            ex->Hi_ModifyEffModelAbr(work->model44, BTLFX_ABR_ADD);
            work->rot18.vy += 0x40;
            elapsed = frame - FX_MIGHT_STAR_FRAME;
            if (frame >= FX_MIGHT_FADE_FRAME) {
                ex->Hi_ModifyEffModelRGB(work->model44, work->rgb40, work->rgb40, work->rgb40);
                work->rgb40 -= 4;
            }
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_MIGHT_AURA_TEXTURE), elapsed);
        }
        if (frame >= FX_MIGHT_STAR_FRAME && frame < FX_MIGHT_STAR_FRAME + FX_MIGHT_FLASH_FRAMES) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_ADD, work->tint42, work->tint42, work->tint42);
            work->tint42 -= 0x1F;
        }
        if (frame >= FX_MIGHT_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_05a/nonmatchings/fx_05a", fx_05a_headerPtr);
