#include "fxmod/fx_05a.h"

s32 fx_05a_op80(s32 mode, Fx05aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 s;
    s32 h;
    s32 r;
    s32 t;

    if (mode == 0) {
        *args = 0x48;
    } else if (mode == 1) {
        fx_05a_env = env;
        ex->btlfx_initEffModels(fx_05a_effModels, 1);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->pos10.vx = work->pos8.vx;
        work->pos10.vy = work->pos8.vy;
        work->pos10.vz = work->pos8.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_05a_trails;
            work->emit0->trailSlots = 0xA;
            work->emit0->trailStride = 0xB0;
        }
        work->rot18.vx = 0;
        work->rot18.vy = 0;
        work->rot18.vz = 0;
        work->pos20.vx = work->pos8.vx;
        work->pos20.vy = work->pos8.vy;
        work->pos20.vz = work->pos8.vz;
        if (ex->btlfx_isEnemyUnit(0)) {
            s = 0x155;
            h = ex->btlfx_getUnitHeight(0);
            r = ex->btlfx_getUnitRadius(0);
            if (h > 0x190) {
                s += (h - 0x190) / 4;
            }
            if (r > 0x320) {
                s += (r - 0x320) / 4;
            }
            work->pos10.vy -= (s - 0x155) * 3;
            if (s > 0x400) {
                s = 0x400;
            }
            work->scale30.vx = s;
            work->scale30.vy = s;
            work->scale30.vz = s;
        } else {
            work->scale30.vx = 0x155;
            work->scale30.vy = 0x155;
            work->scale30.vz = 0x155;
        }
        work->model44 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xA), 0x117, 0, 0, 0xC0);
        work->rgb40 = -0x14;
        work->tint42 = 0xFF;
    } else {
        frame = entry->frame;
        if (frame == 0x19) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame >= 0x19) {
            ex->Hi_DrawEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model44);
            ex->Hi_ModifyEffModelAbr(work->model44, 1);
            work->rot18.vy += 0x40;
            t = frame - 0x19;
            if (frame >= 0x2D) {
                ex->Hi_ModifyEffModelRGB(work->model44, work->rgb40, work->rgb40, work->rgb40);
                work->rgb40 -= 4;
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 7), t);
        }
        if (frame >= 0x19 && frame < 0x21) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, work->tint42, work->tint42, work->tint42);
            work->tint42 -= 0x1F;
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_05a/nonmatchings/fx_05a", fx_05a_headerPtr);
