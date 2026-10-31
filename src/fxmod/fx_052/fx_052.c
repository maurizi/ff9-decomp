#include "fxmod/fx_052.h"

s32 fx_052_op80(s32 mode, Fx052Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x58;
    } else if (mode == 1) {
        fx_052_env = env;
        ex->btlfx_initEffModels(fx_052_effModels, 3);
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
        work->model52 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        work->model54 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xB), 1);
        work->model56 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xE));
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos48);
        work->fadeIn50 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x1E:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos48);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_052_trails;
                work->emit4->trailSlots = 0x14;
                work->emit4->trailStride = 0xB0;
            }
            break;
        case 0x37:
            work->fadeIn50 = 0x18;
            break;
        case 0x50:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos48);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_052_trails2;
                work->emit8->trailSlots = 0x14;
                work->emit8->trailStride = 0xB0;
            }
            break;
        }
        t = frame - 0x37;
        if (t >= 0 && t < 0x79) {
            if (t >= 0x23) {
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
                work->rgb3C = fxm_lerpRatio(-0x80, 0, 0x18 - work->fadeIn50, 0x18);
                work->fadeIn50--;
                ex->Hi_ModifyEffModelRGB(work->model52, work->rgb3C, work->rgb3C, work->rgb3C);
                ex->Hi_ModifyEffModelRGB(work->model56, work->rgb3C, work->rgb3C, work->rgb3C);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xC), frame);
        if (frame >= 0xC8) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_052_op81(s32 mode, Fx052Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        fx_052_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 15:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->posC);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        case 40:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->posC);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        return frame >= 0x5A;
    }
    return 0;
}

s32 fx_052_op82(s32 mode, Fx052Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        fx_052_env = env;
        work->fadeOut2 = -1;
        work->fadeIn0 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->fadeIn0 = 0x10;
            break;
        case 20:
            work->fadeOut2 = 0x10;
            break;
        }
        if (work->fadeIn0 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, 0x10 - work->fadeIn0, 0x10, 1, 0);
            work->fadeIn0--;
        } else if (work->fadeOut2 >= 0) {
            fxm_tintScreenLerp(0xFF, 0, 0x10 - work->fadeOut2, 0x10, 1, 0);
            work->fadeOut2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x20;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_052_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_052_env)->btlfx_drawScreenTint((u32 *)(fx_052_env->ot + ot), arg4, c, c, c);
}

INCLUDE_RODATA("asm/us/fx_052/nonmatchings/fx_052", fx_052_headerPtr);
