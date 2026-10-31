#include "fxmod/fx_0f4.h"

s32 fx_0f4_op80(s32 mode, Fx0f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x70;
    } else if (mode == 1) {
        fx_0f4_env = env;
        ex->btlfx_initEffModels(fx_0f4_effModels, 2);
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
        work->model6C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xA), 0x11A, 0, 0, 0x80);
        work->model6E = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xB), 0x11A, 0, 0, 0x80);
        work->startPos60.vx = work->pos1C.vx;
        work->startPos60.vy = work->pos1C.vy;
        work->startPos60.vz = work->pos1C.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos58);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos58);
        work->fadeInTimer68 = -1;
        work->fadeOutTimer6A = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0xF:
            p = work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos58);
            if (p != NULL) {
                p->otZ = -0x100;
            }
            break;
        case 0x32:
            break;
        case 0x36:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos58);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos58);
            if (work->emitC != NULL) {
                work->emitC->otZ = 0x80;
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x100;
            }
            break;
        case 0x52:
            work->fadeOutTimer6A = 0x10;
            break;
        }
        if ((u32)(frame - 0x32) < 0x30) {
            t = frame - 0x32;
            if (t >= 4) {
                ex->Hi_DrawEffModel(&work->rot14, &work->pos1C, (s32)&work->scale2C, work->model6C);
                ex->Hi_ModifyEffModelAbr(work->model6C, 1);
            }
            ex->Hi_DrawEffModel(&work->rot40, &work->pos1C, (s32)&work->scale48, work->model6E);
            ex->Hi_ModifyEffModelAbr(work->model6E, 1);
            if (work->fadeInTimer68 >= 0 || work->fadeOutTimer6A >= 0) {
                if (work->fadeInTimer68 >= 0) {
                    work->rgb3C = fxm_lerpRatio(-0x80, 0, 3 - work->fadeInTimer68, 3);
                    work->fadeInTimer68--;
                }
                if (work->fadeOutTimer6A >= 0) {
                    work->rgb3C = fxm_lerpRatio(0, -0x100, 0x10 - work->fadeOutTimer6A, 0x10);
                    work->fadeOutTimer6A--;
                }
                if (t >= 4) {
                    ex->Hi_ModifyEffModelRGB(work->model6C, work->rgb3C, work->rgb3C, work->rgb3C);
                }
                ex->Hi_ModifyEffModelRGB(work->model6E, work->rgb3C, work->rgb3C, work->rgb3C);
            }
            if (t >= 4) {
                work->scale2C.vx += 0x40;
                work->scale2C.vz += 0x40;
                if (t >= 0x10) {
                    work->scale2C.vy -= 8;
                } else {
                    work->scale2C.vy += 0x40;
                }
            }
            work->scale48.vx += 0x40;
            work->scale48.vz += 0x40;
            if (t >= 0xC) {
                work->scale48.vy -= 8;
            } else {
                work->scale48.vy += 0x55;
            }
            work->scale2C.vx = work->scale2C.vx > 0x666 ? 0x666 : work->scale2C.vx;
            work->scale2C.vy = work->scale2C.vy > 0xCCC ? 0xCCC : work->scale2C.vy;
            work->scale2C.vz = work->scale2C.vz > 0x666 ? 0x666 : work->scale2C.vz;
            work->scale48.vx = work->scale48.vx > 0x666 ? 0x666 : work->scale48.vx;
            work->scale48.vy = work->scale48.vy > 0xCCC ? 0xCCC : work->scale48.vy;
            work->scale48.vz = work->scale48.vz > 0x666 ? 0x666 : work->scale48.vz;
        }
        if (frame >= 0x32) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 8), frame);
        } else {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xE), frame);
        }
        if (frame >= 0x78) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_0f4_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_0f4/nonmatchings/fx_0f4", fx_0f4_headerPtr);
