#include "fxmod/fx_1ab.h"

INCLUDE_RODATA("asm/us/fx_1ab/nonmatchings/fx_1ab", fx_1ab_headerPtr);

s32 fx_1ab_op80(s32 mode, Fx1abWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 elapsed;

    if (mode == 0) {
        *args = 0x88;
    } else if (mode == 1) {
        fx_1ab_env = env;
        ex->btlfx_initEffModels(fx_1ab_effModels, 2);
        work->pos70.vx = env->targets->centerX;
        work->pos70.vy = env->targets->centerY;
        work->pos70.vz = env->targets->centerZ;
        work->rotC.vx = 0;
        work->rotC.vy = 0;
        work->rotC.vz = 0;
        work->pos14.vx = work->pos70.vx;
        work->pos14.vy = work->pos70.vy;
        work->pos14.vz = work->pos70.vz;
        work->scale24.vx = 0x400;
        work->scale24.vy = 0x800;
        work->scale24.vz = 0x400;
        work->pos14.vy = 0x4B0;
        work->rot38.vx = 0;
        work->rot38.vy = 0;
        work->rot38.vz = 0;
        work->pos40.vx = work->pos70.vx;
        work->pos40.vy = work->pos70.vy;
        work->pos40.vz = work->pos70.vz;
        work->scale50.vx = 0;
        work->scale50.vy = 0;
        work->scale50.vz = 0;
        work->unk60.vx = 0;
        work->unk60.vy = 0;
        work->unk60.vz = 0;
        work->model84 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_WATERGA_WATER_SPHERE), 1);
        work->model86 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_WATERGA_WATER_RING), 1);
        work->timer80 = -1;
        work->timer82 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATERGA_RINGS), (s32)&work->pos70);
            break;
        case 8:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATERGA_WATER_SPLASH), (s32)&work->pos70);
            break;
        case 32:
            work->pos78.vx = work->pos70.vx;
            work->pos78.vy = work->pos70.vy;
            work->pos78.vz = work->pos70.vz;
            work->pos78.vy = -0x258;
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WATERGA_WATER_SPLASH_2), (s32)&work->pos78);
            break;
        case 15:
            work->timer80 = 3;
            break;
        case 47:
            work->timer82 = 0x10;
            break;
        }
        if (frame >= 0xF && frame < 0x3F) {
            elapsed = frame - 0xF;
            ex->Hi_SetEffModelSlice(work->model84, work->pos14.vy);
            ex->Hi_DrawSliceEffModel(&work->rotC, &work->pos14, (s32)&work->scale24, work->model84);
            ex->Hi_DrawEffModel(&work->rot38, &work->pos40, (s32)&work->scale50, work->model86);
            if (work->timer80 >= 0 || work->timer82 >= 0) {
                if (work->timer80 >= 0) {
                    work->color34 = fxm_lerpRatio(-0x80, 0, 3 - work->timer80, 3);
                    work->timer80--;
                }
                if (work->timer82 >= 0) {
                    work->color34 = fxm_lerpRatio(0, -0x80, 0x10 - work->timer82, 0x10);
                    work->timer82--;
                }
                ex->Hi_ModifyEffModelRGB(work->model84, work->color34, work->color34, work->color34);
                ex->Hi_ModifyEffModelRGB(work->model86, work->color34, work->color34, work->color34);
            }
            if (elapsed < 8) {
                work->pos14.vy -= 0x145;
            } else {
                if (work->scale24.vy > 0x400) {
                    work->scale24.vy -= 0x40;
                }
                work->pos14.vy += 0x14;
            }
            work->scale50.vx += 0x40;
            work->scale50.vz += 0x40;
            if (elapsed >= 8) {
                work->scale50.vy -= 8;
            } else {
                work->scale50.vy += 0xAA;
            }
            work->scale50.vy = work->scale50.vy > 0x400 ? 0x400 : work->scale50.vy;
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_WATERGA_BACKDROP), frame);
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_WATERGA_BACKDROP_3), frame);
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_WATERGA_BACKDROP_2), frame);
        if (frame >= 0x64) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1ab_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
