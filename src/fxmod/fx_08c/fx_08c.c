#include "fxmod/fx_08c.h"

s32 fx_08c_op80(s32 mode, Fx08cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR scale;
    Pool6CEntry *emitter;
    s32 frame;
    s32 ang;
    s32 bright;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        work->state0 = 0;
        work->unk10.vx = env->targets->centerX;
        work->unk10.vy = env->targets->centerY;
        work->unk10.vz = env->targets->centerZ;
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        work->pos8.vz = env->targets->centerZ;
        work->unk10.vy -= 0x100;
        work->modelPos18[0] = work->pos8.vx;
        work->modelPos18[1] = work->pos8.vy;
        work->modelPos18[2] = work->pos8.vz;
        fx_08c_env = env;
        ex->btlfx_initEffModels(fx_08c_effModels, 2);
        work->model24 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_BERSERK_FUNNEL), 0x138, 0xFFFF, 0, 0x80);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BERSERK_FIRE_RING), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = 0x80;
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BERSERK_STAR_FLASH), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = 0x80;
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            ang = (frame << 7) >> 1;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            bright = ex->math_sin(ang);
            scale.vx = ((ang + 0x2800) * 4) / 3;
            scale.vz = scale.vx;
            scale.vy = ((frame << 9) + 0x2000) / 3;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)work->modelPos18, (s32)&scale, work->model24);
            ex->Hi_ModifyEffModelAbr(work->model24, 1);
            ex->Hi_ModifyEffModelRGB(work->model24, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
            scale.vy >>= 1;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)work->modelPos18, (s32)&scale, work->model24);
            ex->Hi_ModifyEffModelAbr(work->model24, 1);
            ex->Hi_ModifyEffModelRGB(work->model24, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
            if (frame >= 0x20) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_BERSERK_BACKDROP), work->bgFrame4);
        work->bgFrame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_08c/nonmatchings/fx_08c", fx_08c_headerPtr);
