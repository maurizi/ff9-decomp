#include "fxmod/fx_090.h"

s32 fx_090_op80(s32 mode, Fx090Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT r;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        work->pos50.vx = env->targets->centerX;
        work->pos50.vy = env->targets->centerY;
        work->pos50.vz = env->targets->centerZ;
        work->pos58.vx = env->targets->centerX;
        work->pos58.vy = env->targets->centerY;
        work->pos58.vz = env->targets->centerZ;
        work->unk60.vx = env->targets->centerX;
        work->unk60.vy = env->targets->centerY;
        work->unk60.vz = env->targets->centerZ;
        work->pos58.vy -= 0x320;
        work->unk60.vy -= 0x4B0;
        fx_090_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos50);
        work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos50);
        if (work->emit0 != NULL) {
            work->emit0->unk14 = (s32)D_801E7FD8;
            work->emit0->unk12 = 0xC;
            work->emit0->unkC = 0xA0;
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
        ex->btlfx_initEffModels(fx_090_effModels, 2);
        work->model70 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xB), 0x1B, 0x3D09, 0, 0);
        work->model72 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xD), 0x1B, 0x3D09, 0, 0);
        work->unk6C = work->unk68 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 18:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos50);
            work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos50);
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x180;
            }
            work->unk68 = 0x12;
            break;
        case 22:
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos58);
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos58);
            break;
        case 39:
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos58);
            break;
        case 28:
            work->unk6C = 0x1A;
            break;
        }
        if (work->unk68 >= 0) {
            work->unk4C = fxm_lerpRatio(-0x80, 0, 0x12 - work->unk68, 0x12);
            work->unk68--;
        }
        if (work->unk6C >= 0) {
            work->unk4C = fxm_lerpRatio(0, -0x80, 0x1A - work->unk6C, 0x1A);
            work->unk6C--;
        }
        if (frame >= 0x12 && frame < 0x18) {
            work->scale3C.vy = ((frame - 0x12) << 12) / 6;
            ex->Hi_DrawEffModel(&work->rot24, &work->pos2C, (s32)&work->scale3C, work->model70);
            ex->Hi_ModifyEffModelAbr(work->model70, 1);
            ex->Hi_ModifyEffModelRGB(work->model70, work->unk4C, work->unk4C, work->unk4C);
        }
        if (frame >= 0x18 && frame < 0x32) {
            ex->Hi_DrawMorphEffModel(&work->rot24, &work->pos2C, (s32)&work->scale3C, work->model70, work->model72,
                                     0x1000 - ((frame - 0x18) << 12) / 26);
            ex->Hi_ModifyEffModelAbr(work->model70, 1);
            ex->Hi_ModifyEffModelRGB(work->model70, work->unk4C, work->unk4C, work->unk4C);
        }
        if (frame >= 0x12) {
            s16 *p = D_801E7DB4[frame % 15];
            r.x = p[0];
            r.y = p[1];
            r.w = 0x20;
            r.h = 0x80;
            ex->MoveImage(&r, 0x2C0, 0x100);
        }
        if (frame >= 0x46) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_090_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_090/nonmatchings/fx_090", fx_090_headerPtr);
