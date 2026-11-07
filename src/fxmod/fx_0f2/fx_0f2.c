#include "fxmod/fx_0f2.h"

s32 fx_0f2_op80(s32 mode, Fx0f2Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 elapsed;

    if (mode == 0) {
        *args = sizeof(Fx0f2Work);
    } else if (mode == 1) {
        fx_0f2_env = env;
        ex->btlfx_initEffModels(fx_0f2_effModels, 1);
        work->pos38.vx = env->targets->centerX;
        work->pos38.vy = env->targets->centerY;
        work->pos38.vz = env->targets->centerZ;
        if (ex->btlfx_isEnemyUnit(0)) {
            work->isEnemy50 = 1;
        } else {
            work->isEnemy50 = 0;
        }
        switch (work->isEnemy50) {
        case 1:
            work->speed34 = FX_TIDAL_FLAME_SPEED;
            work->rot8.vx = 0;
            work->rot8.vy = 0;
            work->rot8.vz = 0;
            break;
        case 0:
            work->speed34 = -FX_TIDAL_FLAME_SPEED;
            work->rot8.vx = 0;
            work->rot8.vy = 0x800;
            work->rot8.vz = 0;
            break;
        }
        work->pos10.vz = 0x1F4;
        work->pos10.vx = 0;
        work->pos10.vy = 0;
        work->scale20.vx = 0xAAA;
        work->scale20.vy = 0xAAA;
        work->scale20.vz = 0xAAA;
        work->model58 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_TIDAL_FLAME_FIRE_DOME), FX_TIDAL_FLAME_FIRE_DOME_TPAGE, FX_TIDAL_FLAME_FIRE_DOME_CLUT, FX_TIDAL_FLAME_FIRE_DOME_U, FX_TIDAL_FLAME_FIRE_DOME_V);
        work->startPos40.vx = work->pos10.vx;
        work->startPos40.vy = work->pos10.vy;
        work->startPos40.vz = work->pos10.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TIDAL_FLAME_RUNE_BURST), (s32)&work->pos38);
        work->fadeInTimer54 = -1;
        work->fadeOutTimer56 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_TIDAL_FLAME_SWEEP_FRAME:
            work->pos48.vx = work->pos10.vx;
            work->pos48.vy = work->pos10.vy;
            work->pos48.vz = work->pos10.vz;
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_TIDAL_FLAME_FLAME_CLOUD), (s32)&work->pos48);
            work->fadeInTimer54 = FX_TIDAL_FLAME_FADE_IN_FRAMES;
            break;
        case FX_TIDAL_FLAME_FADE_OUT_FRAME:
            work->fadeOutTimer56 = FX_TIDAL_FLAME_FADE_OUT_FRAMES;
            break;
        }
        if (frame >= FX_TIDAL_FLAME_SWEEP_FRAME && frame < FX_TIDAL_FLAME_SWEEP_FRAME + FX_TIDAL_FLAME_SWEEP_FRAMES) {
            elapsed = frame - FX_TIDAL_FLAME_SWEEP_FRAME;
            ex->Hi_DrawEffModel(&work->rot8, &work->pos10, (s32)&work->scale20, work->model58);
            ex->Hi_ModifyEffModelAbr(work->model58, BTLFX_ABR_ADD);
            if (work->fadeInTimer54 >= 0 || work->fadeOutTimer56 >= 0) {
                if (work->fadeInTimer54 >= 0) {
                    work->rgb30 = fxm_lerpRatio(-0x80, 0, FX_TIDAL_FLAME_FADE_IN_FRAMES - work->fadeInTimer54, FX_TIDAL_FLAME_FADE_IN_FRAMES);
                    work->fadeInTimer54--;
                }
                if (work->fadeOutTimer56 >= 0) {
                    work->rgb30 = fxm_lerpRatio(0, -0xC0, FX_TIDAL_FLAME_FADE_OUT_FRAMES - work->fadeOutTimer56, FX_TIDAL_FLAME_FADE_OUT_FRAMES);
                    work->fadeOutTimer56--;
                }
                ex->Hi_ModifyEffModelRGB(work->model58, work->rgb30, work->rgb30, work->rgb30);
            }
            work->pos10.vz += work->speed34;
            work->scale20.vz += 0x100;
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_TIDAL_FLAME_BACKDROP), elapsed);
        }
        if (frame >= FX_TIDAL_FLAME_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0f2_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0f2/nonmatchings/fx_0f2", fx_0f2_headerPtr);
