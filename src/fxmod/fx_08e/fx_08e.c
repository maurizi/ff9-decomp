#include "fxmod/fx_08e.h"

INCLUDE_RODATA("asm/us/fx_08e/nonmatchings/fx_08e", fx_08e_headerPtr);

s32 fx_08e_op80(s32 mode, Fx08eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT rect;
    s32 frame;

    if (mode == 0) {
        *args = 0x6C;
    } else if (mode == 1) {
        fx_08e_env = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos48);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos58);
        work->pos50.vx = work->pos58.vx;
        work->pos50.vy = work->pos58.vy;
        work->pos50.vz = work->pos58.vz;
        work->pos50.vy -= 0x1C2;
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
        ex->btlfx_initEffModels(fx_08e_effModels, 2);
        work->model68 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_CURAGA_LIGHT_FOUNTAIN), 0x1B, 0x3D09, 0, 0);
        work->model6A = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_CURAGA_LIGHT_RING), 0x1B, 0x3D09, 0, 0);
        work->fadeOut64 = work->fadeIn60 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 6:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_BLUE_SWIRL), (s32)&work->pos58);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_GOLD_RING), (s32)&work->pos58);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_08e_trails;
                work->emit4->trailSlots = 9;
                work->emit4->trailStride = 0xA0;
            }
            break;
        case 22:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_WHITE_RINGS), (s32)&work->pos58);
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_SPARKLE_BURST), (s32)&work->pos58);
            work->fadeIn60 = 0x12;
            break;
        case 28:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_SPARKLES), (s32)&work->pos50);
            break;
        case 42:
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURAGA_SPARKLE_SHOWER), (s32)&work->pos48);
            break;
        case 36:
            work->fadeOut64 = 0x18;
            break;
        }
        if (work->fadeIn60 >= 0) {
            work->bright44 = fxm_lerpRatio(-0x80, 0, 0x12 - work->fadeIn60, 0x12);
            work->fadeIn60--;
        }
        if (work->fadeOut64 >= 0) {
            work->bright44 = fxm_lerpRatio(0, -0x80, 0x18 - work->fadeOut64, 0x18);
            work->fadeOut64--;
        }
        if (frame >= 0x16 && frame < 0x20) {
            s32 t = ((frame - 0x16) << 12) / 10;
            work->scale34.vy = t / 2;
            ex->Hi_DrawEffModel(&work->rot1C, &work->pos24, (s32)&work->scale34, work->model68);
            ex->Hi_ModifyEffModelAbr(work->model68, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model68, work->bright44, work->bright44, work->bright44);
        }
        if (frame >= 0x20 && frame < 0x38) {
            ex->Hi_DrawMorphEffModel(&work->rot1C, &work->pos24, (s32)&work->scale34, work->model68, work->model6A,
                                     0x1000 - ((frame - 0x20) << 12) / 24);
            ex->Hi_ModifyEffModelAbr(work->model68, BTLFX_ABR_ADD);
            ex->Hi_ModifyEffModelRGB(work->model68, work->bright44, work->bright44, work->bright44);
        }
        if (frame >= 0x16) {
            s16 *texXY = fx_08e_texAnimSrc[frame % 15];
            rect.x = texXY[0];
            rect.y = texXY[1];
            rect.w = 0x20;
            rect.h = 0x80;
            ex->MoveImage(&rect, 0x2C0, 0x100);
        }
        if (frame >= 0x46) {
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
