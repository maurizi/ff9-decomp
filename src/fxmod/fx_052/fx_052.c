#include "fxmod/fx_052.h"

s32 fx_052_op80(s32 mode, Fx052Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 elapsed;

    if (mode == 0) {
        *args = sizeof(Fx052Work80);
    } else if (mode == 1) {
        fx_052_env = env;
        ex->btlfx_initEffModels(fx_052_effModels, FX_LIMIT_GLOVE_MODEL_COUNT);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos40);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos48);
        work->rotC.vz = -0x200;
        work->rot14.vz = 0x200;
        work->rotC.vx = 0;
        work->rotC.vy = 0;
        work->rot14.vx = 0;
        work->rot14.vy = 0;
        work->scale2C.vx = 0x2000;
        work->scale2C.vy = 0x2000;
        work->scale2C.vz = 0x2000;
        work->pos1C.vx = work->pos40.vx;
        work->pos1C.vy = work->pos40.vy;
        work->pos1C.vz = work->pos40.vz;
        work->model52 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_PINK_ORB), 1);
        work->model54 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_GRID_SPHERE), 1);
        work->model56 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_GLOW_DOME));
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_GOLD_SPARKLES), (s32)&work->pos48);
        work->fadeIn50 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_LIMIT_GLOVE_GLOW_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_GOLD_GLOW), (s32)&work->pos48);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_052_trails;
                work->emit4->trailSlots = FX_LIMIT_GLOVE_GOLD_GLOW_TRAIL_SLOTS;
                work->emit4->trailStride = FX_LIMIT_GLOVE_GOLD_GLOW_TRAIL_STRIDE;
            }
            break;
        case FX_LIMIT_GLOVE_ORB_FRAME:
            work->fadeIn50 = FX_LIMIT_GLOVE_FADE_FRAMES;
            break;
        case FX_LIMIT_GLOVE_GLOW_2_FRAME:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_GOLD_GLOW_2), (s32)&work->pos48);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_052_trails2;
                work->emit8->trailSlots = FX_LIMIT_GLOVE_GOLD_GLOW_TRAIL_SLOTS;
                work->emit8->trailStride = FX_LIMIT_GLOVE_GOLD_GLOW_TRAIL_STRIDE;
            }
            break;
        }
        elapsed = frame - FX_LIMIT_GLOVE_ORB_FRAME;
        if (elapsed >= 0 && elapsed < FX_LIMIT_GLOVE_ORB_FRAMES) {
            if (elapsed >= FX_LIMIT_GLOVE_GRID_FRAME) {
                if (work->scale2C.vx > 0x400) {
                    work->scale2C.vx -= 0x100;
                    work->scale2C.vy -= 0x100;
                    work->scale2C.vz -= 0x100;
                }
                ex->Hi_DrawEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, work->model54);
                work->rot14.vy += 0x10;
            }
            ex->Hi_DrawEffModel(&work->rotC, &work->pos1C, (s32)&work->scale2C, work->model56);
            ex->Hi_DrawEffModel(&work->rotC, &work->pos1C, (s32)&work->scale2C, work->model52);
            work->rotC.vy -= 0x10;
            if (work->fadeIn50 >= 0) {
                work->rgb3C = fxm_lerpRatio(-0x80, 0, FX_LIMIT_GLOVE_FADE_FRAMES - work->fadeIn50, FX_LIMIT_GLOVE_FADE_FRAMES);
                work->fadeIn50--;
                ex->Hi_ModifyEffModelRGB(work->model52, work->rgb3C, work->rgb3C, work->rgb3C);
                ex->Hi_ModifyEffModelRGB(work->model56, work->rgb3C, work->rgb3C, work->rgb3C);
            }
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_BACKDROP), frame);
        if (frame >= FX_LIMIT_GLOVE_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_052_op81(s32 mode, Fx052Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx052Work81);
    } else if (mode == 1) {
        fx_052_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_LIGHT_BEAM), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_LIMIT_GLOVE_RINGS_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_BLUE_RINGS), (s32)&work->posC);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case FX_LIMIT_GLOVE_SPECKS_FRAME:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIMIT_GLOVE_PINK_SPECKS), (s32)&work->posC);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        return frame >= FX_LIMIT_GLOVE_BEAM_FRAMES;
    }
    return 0;
}

s32 fx_052_op82(s32 mode, Fx052Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx052Work82);
    } else if (mode == 1) {
        fx_052_env = env;
        work->fadeOut2 = -1;
        work->fadeIn0 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->fadeIn0 = FX_LIMIT_GLOVE_FLASH_FADE_FRAMES;
            break;
        case FX_LIMIT_GLOVE_FLASH_OUT_FRAME:
            work->fadeOut2 = FX_LIMIT_GLOVE_FLASH_FADE_FRAMES;
            break;
        }
        if (work->fadeIn0 >= 0) {
            fxm_tintScreenLerp(0, FX_LIMIT_GLOVE_FLASH_LEVEL, FX_LIMIT_GLOVE_FLASH_FADE_FRAMES - work->fadeIn0,
                               FX_LIMIT_GLOVE_FLASH_FADE_FRAMES, BTLFX_ABR_ADD, 0);
            work->fadeIn0--;
        } else if (work->fadeOut2 >= 0) {
            fxm_tintScreenLerp(FX_LIMIT_GLOVE_FLASH_LEVEL, 0, FX_LIMIT_GLOVE_FLASH_FADE_FRAMES - work->fadeOut2,
                               FX_LIMIT_GLOVE_FLASH_FADE_FRAMES, BTLFX_ABR_ADD, 0);
            work->fadeOut2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->ot, BTLFX_ABR_ADD, FX_LIMIT_GLOVE_FLASH_LEVEL, FX_LIMIT_GLOVE_FLASH_LEVEL,
                                     FX_LIMIT_GLOVE_FLASH_LEVEL);
        }
        return frame >= FX_LIMIT_GLOVE_FLASH_FRAMES;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_052_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_052_env)->btlfx_drawScreenTint((u32 *)(fx_052_env->ot + ot), abr, bright, bright, bright);
}

INCLUDE_RODATA("asm/us/fx_052/nonmatchings/fx_052", fx_052_headerPtr);
