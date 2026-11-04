#include "fxmod/fx_00e.h"

s32 fx_00e_op80(s32 mode, Fx00eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;
    SVECTOR rot;
    VECTOR scale;
    s32 ang;
    s32 scaleXZ;
    s32 bright;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_00e_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos8);
        work->pos18[0] = work->pos8.vx;
        work->pos18[1] = work->pos8.vy;
        work->pos18[2] = work->pos8.vz;
        ex->btlfx_initEffModels(fx_00e_effModels, 2);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BERSERK_CONE), 0x138, 0xFFFF, 0, 0x80);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BERSERK_FIRE_RINGS), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BERSERK_RED_FLASH), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            ang = (frame << 7) >> 1;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            bright = ex->math_sin(ang);
            scaleXZ = (ang * 2 + 0x1800) * 7 / 4;
            scale.vx = scaleXZ;
            scale.vy = (frame << 8) + 0x1000;
            scale.vz = scaleXZ;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)work->pos18, (s32)&scale, work->model24);
            ex->Hi_ModifyEffModelAbr(work->model24, 1);
            ex->Hi_ModifyEffModelRGB(work->model24, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
            scale.vy >>= 1;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)work->pos18, (s32)&scale, work->model24);
            ex->Hi_ModifyEffModelAbr(work->model24, 1);
            ex->Hi_ModifyEffModelRGB(work->model24, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
            if (frame >= 0x20) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_BERSERK_BACKDROP), work->frame4);
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_00e/nonmatchings/fx_00e", fx_00e_headerPtr);
