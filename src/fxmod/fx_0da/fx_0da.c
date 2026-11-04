#include "fxmod/fx_0da.h"

s32 fx_0da_op80(s32 mode, Fx0daWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 pos[3];
    s32 scale[3];
    SVECTOR rot;
    Pool6CEntry *emitter;
    s32 frame;
    s32 size;
    s32 t;
    s32 mm;
    s32 b;
    s32 morphT;
    s32 rgbRB;
    s32 model;
    s32 ang;
    s32 k;
    s32 rgbG;

    if (mode == 0) {
        *args = 0x28;
    } else if (mode == 1) {
        fx_0da_env = env;
        ex->btlfx_initEffModels(fx_0da_effModels, 4);
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_ICE_BLOCK), 0xBA, 0x3DC0, 0, 0);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_ICE_CRACKING), 0xBA, 0x3DC0, 0, 0);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_ICE_SHARDS), 0xBA, 0x3DC0, 0, 0);
        work->model4 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_ICE_REFLECTION), 0xB8, 0x3D40, 0x80,
                                                    0x80);
        work->state0 = 1;
        work->frame24 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos1C);
        return 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 1:
            t = frame << 10;
            size = ((0x1000 - ex->math_cos(t >> 2)) * 0xC00) >> 12;
            scale[0] = scale[1] = scale[2] = size;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            pos[0] = work->pos14.vx;
            pos[1] = work->pos14.vy;
            pos[2] = work->pos14.vz;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, (VECTOR *)pos, (s32)scale, work->model8);
            ex->Hi_ModifyEffModelAbr(work->model8, 0);
            ex->Hi_ModifyEffModelRGB(work->model8, 0, 0, 0);
            if (frame >= 4) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            t = frame << 8;
            scale[0] = scale[1] = scale[2] = 0xC00;
            rot.vx = 0;
            rot.vy = frame << 5;
            rot.vz = 0;
            pos[0] = work->pos14.vx;
            pos[1] = work->pos14.vy;
            pos[2] = work->pos14.vz;
            ex->Hi_SetEffModelOffset(work->model8, 2);
            ex->Hi_DrawEffModel(&rot, (VECTOR *)pos, (s32)scale, work->model8);
            ex->Hi_ModifyEffModelAbr(work->model8, 0);
            ex->Hi_ModifyEffModelRGB(work->model8, 0, 0, 0);
            ex->Hi_DrawEffModel(&rot, (VECTOR *)pos, (s32)scale, work->model4);
            ex->Hi_ModifyEffModelAbr(work->model4, 1);
            ang = t >> 1;
            ex->Hi_ModifyEffModelRGB(work->model4, (ex->math_sin(ang) >> 5) - 0x80, (ex->math_sin(ang) >> 5) - 0x80,
                                     (ex->math_sin(ang) >> 5) - 0x80);
            ex->btlfx_drawScrollMove(((0x1000 - ex->math_cos(ang)) >> 9) + frame * 5, 0x240, 0x180, 0x40, 0x80, 0x240, 0x100);
            if (frame >= 0x10) {
                work->state0 = 3;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_FREEZE_BURST), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_FROST_RING), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_ICE_FLARES), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_ICE_FLARES_2), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x20, 0x46);
            }
            break;
        case 3:
            scale[0] = scale[1] = scale[2] = 0xC00;
            rot.vx = 0;
            rot.vy = 0x200;
            rot.vz = 0;
            pos[0] = work->pos14.vx;
            pos[1] = work->pos14.vy;
            pos[2] = work->pos14.vz;
            if (frame < 2) {
                morphT = 0x2000 - (frame << 11);
            } else {
                morphT = 0x1000 - ((frame - 2) << 12) / 14;
            }
            if (morphT > 0x1000) {
                {
                    register s32 mc REG_ASM("$3") = work->modelC; /* the original passes it in v1 */
                    ex->Hi_DrawMorphEffModel(&rot, (VECTOR *)pos, (s32)scale, work->model8, mc, morphT - 0x1000);
                }
                rgbRB = -0x40;
                rgbG = -0x40;
                model = work->model8;
            } else {
                ex->Hi_DrawMorphEffModel(&rot, (VECTOR *)pos, (s32)scale, work->modelC, work->model10, morphT);
                rgbRB = morphT - 0x800;
                rgbRB >>= 5;
                rgbRB -= 0x80;
                rgbG = rgbRB;
                model = work->modelC;
            }
            ex->Hi_ModifyEffModelRGB(model, rgbRB, rgbG, rgbRB);
            if (frame == 1) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_SHATTER_WAVE), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0xC) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->frame24++;
    }
    return 0;
}

s32 fx_0da_op84(s32 mode, Fx0daWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 alpha;
    s32 scale0;
    s32 scale1;
    s32 ang;
    s32 t;
    s32 flicker;
    s32 grow;
    u16 ry;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_0da_env = env;
        work->state0 = 0;
        work->frame4 = 0;
    } else {
        alpha = 0;
        scale0 = 0;
        scale1 = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
        ry = 0;
        switch (work->state0) {
        case 0:
            ry = 0x200;
            alpha = (frame << 12) / 3;
            t = ex->math_sin(alpha >> 2);
            scale0 = t >> 1;
            scale1 = ((0x1000 - alpha) >> 1) + 0x800;
            if (frame >= 3) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            t = (frame << 12) / 9;
            flicker = (frame & 1) << 8;
            scale1 = flicker + 0x1000;
            grow = (t >> 1) + 0x800;
            alpha = flicker + grow;
            ry = (t >> 3) + 0x200;
            scale0 = scale1;
            if (frame >= 9) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            t = (frame << 12) / 9;
            ang = t >> 2;
            ry = 0x400 - ang;
            alpha = ex->math_cos(ang);
            scale0 = ex->math_sin(ang) + 0x1000;
            scale1 = ex->math_sin(ang) * 3 / 2 + 0x1000;
            if (frame >= 9) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_SWORD_GLINT), &work->pos8, NULL, scale0 / 3, 0, 1, alpha / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_SWDMAG_BLIZZARD_SWORD_FLASH), &work->pos8, &rot, scale1 / 2, 0, 1, alpha * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0da/nonmatchings/fx_0da", fx_0da_headerPtr);
