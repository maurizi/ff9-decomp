#include "fxmod/fx_1f8.h"

s32 fx_1f8_op80(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1f8_emitPos0.vx = 0;
        fx_1f8_emitPos0.vy = 0;
        fx_1f8_emitPos0.vz = -0x618;
        fx_1f8_env = env;
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_ORBIT_GLOW_2), (s32)&fx_1f8_emitPos0);
    } else {
        return entry->frame >= 0x40;
    }
    return 0;
}

s32 fx_1f8_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 shade;

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1f8_timer[0] = 0;
        fx_1f8_env = env;
        ex->btlfx_initEffModels(fx_1f8_effModels, 4);
        for (i = 0; i < env->targets->targetCount; i++) {
            fx_1f8_models[i] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_STAR_CRYSTAL), 0x177,
                                                       0xFFFF, 0, 0x80);
            ex->btlfx_getUnitBodyPos(i, 0, (s16 *)&fx_1f8_emitPos1[i]);
            fx_1f8_modelPos[i].vx = fx_1f8_emitPos1[i].vx;
            fx_1f8_modelPos[i].vy = fx_1f8_emitPos1[i].vy;
            fx_1f8_modelPos[i].vz = fx_1f8_emitPos1[i].vz + 0x100;
            fx_1f8_emitPos1[i].vz += 0x200;
        }
        fx_1f8_modelRot.vx = 0;
        fx_1f8_modelRot.vy = 0x800;
        fx_1f8_modelRot.vz = 0;
        fx_1f8_modelScale.vx = 0x1800;
        fx_1f8_modelScale.vy = 0x1800;
        fx_1f8_modelScale.vz = 0x1800;
    } else {
        frame = entry->frame;
        if (frame == 0) {
            fx_1f8_timer[0] = 0x20;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_PINK_RING), (s32)&fx_1f8_emitPos1[i]);
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_COLOR_RINGS), (s32)&fx_1f8_emitPos1[i]);
            }
        }
        if (fx_1f8_timer[0] != 0) {
            fx_1f8_timer[0]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->Hi_DrawEffModel(&fx_1f8_modelRot, &fx_1f8_modelPos[i], (s32)&fx_1f8_modelScale, fx_1f8_models[i]);
                if (fx_1f8_timer[0] >= 0x19) {
                    shade = (0x18 - fx_1f8_timer[0]) * 0x10;
                    ex->Hi_ModifyEffModelRGB(fx_1f8_models[i], shade, shade, shade);
                }
                if (fx_1f8_timer[0] < 8) {
                    shade = (fx_1f8_timer[0] - 8) * 0x10;
                    ex->Hi_ModifyEffModelRGB(fx_1f8_models[i], shade, shade, shade);
                }
            }
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_CARBUNCLE_BACKDROP), frame);
        }
        return frame >= 0x30;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f8/nonmatchings/fx_1f8", fx_1f8_headerPtr);
