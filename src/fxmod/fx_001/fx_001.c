#include "fxmod/fx_001.h"

INCLUDE_RODATA("asm/us/fx_001/nonmatchings/fx_001", fx_001_headerPtr);

s32 fx_001_op80(s32 mode, Fx001Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    RECT r;
    s32 frame;

    if (mode == 0) {
        *args = 0x70;
    } else if (mode == 1) {
        fx_001_env = env;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->pos4C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos5C);
        work->pos54.vx = work->pos5C.vx;
        work->pos54.vy = work->pos5C.vy;
        work->pos54.vz = work->pos5C.vz;
        work->pos54.vy -= 0x1C2;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos5C);
        work->rot20.vx = 0;
        work->rot20.vy = 0;
        work->rot20.vz = 0;
        work->scale38.vx = 0x800;
        work->scale38.vy = 0x1000;
        work->scale38.vz = 0x800;
        work->pos28.vx = work->pos5C.vx;
        work->pos28.vy = work->pos5C.vy;
        work->pos28.vz = work->pos5C.vz;
        ex->btlfx_initEffModels(fx_001_effModels, 2);
        work->model6C = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xE), 0x1B, 0x3D09, 0, 0);
        work->model6E = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xF), 0x1B, 0x3D09, 0, 0);
        work->fadeOut68 = work->fadeIn64 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 6:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos5C);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos5C);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_001_trails;
                work->emit4->trailSlots = 9;
                work->emit4->trailStride = 0xA0;
            }
            break;
        case 22:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos5C);
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos5C);
            work->fadeIn64 = 0x12;
            break;
        case 28:
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos54);
            break;
        case 42:
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos4C);
            break;
        case 36:
            work->fadeOut68 = 0x18;
            break;
        }
        if (work->fadeIn64 >= 0) {
            work->rgb48 = fxm_lerpRatio(-0x80, 0, 0x12 - work->fadeIn64, 0x12);
            work->fadeIn64--;
        }
        if (work->fadeOut68 >= 0) {
            work->rgb48 = fxm_lerpRatio(0, -0x80, 0x18 - work->fadeOut68, 0x18);
            work->fadeOut68--;
        }
        if (frame >= 0x16 && frame < 0x20) {
            s32 t = ((frame - 0x16) << 12) / 10;
            work->scale38.vy = t / 2;
            ex->Hi_DrawEffModel(&work->rot20, &work->pos28, (s32)&work->scale38, work->model6C);
            ex->Hi_ModifyEffModelAbr(work->model6C, 1);
            ex->Hi_ModifyEffModelRGB(work->model6C, work->rgb48, work->rgb48, work->rgb48);
        }
        if (frame >= 0x20 && frame < 0x38) {
            ex->Hi_DrawMorphEffModel(&work->rot20, &work->pos28, (s32)&work->scale38, work->model6C, work->model6E,
                                     0x1000 - ((frame - 0x20) << 12) / 24);
            ex->Hi_ModifyEffModelAbr(work->model6C, 1);
            ex->Hi_ModifyEffModelRGB(work->model6C, work->rgb48, work->rgb48, work->rgb48);
        }
        if (frame >= 0x16) {
            s16 *p = fx_001_texAnimXY[frame % 15];
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
    BTLFX_EXPORTS(fx_001_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}
