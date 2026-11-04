#include "fxmod/fx_198.h"

s32 fx_198_op80(s32 mode, Fx198Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    SVECTOR quadRot;
    SVECTOR quadPos;
    Fx198Key *key;
    Fx198Key *quadKey;
    s32 frame;
    s32 quadAlpha;
    s32 bright;
    s32 i;
    s32 t;
    s32 spinFrame;
    s32 j;
    Fx198Key *rowKey;

    if (mode == 0) {
        *args = 0x24;
    } else if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_198_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->posC);
        ex->btlfx_initEffModels(fx_198_effModels, 5);
        key = (Fx198Key *)ex->btlfx_getFileData(entry->file, FX_TSUNAMI_BOWL);
        work->model1C = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)key, 0x137, 0xFFFF, 0, 0x80);
        work->unk14 = 0;
        work->unk16 = 0;
        work->unk18 = 0;
        work->scale8 = 0x2000;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame < 0x1E) {
                key = (Fx198Key *)(ex->btlfx_getFileData(entry->file, FX_TSUNAMI_BOWL_KEYS) + 4);
                key += frame;
                pos.vx = work->posC.vx;
                pos.vy = work->posC.vy;
                pos.vz = work->posC.vz;
                pos.vz += (work->scale8 << 9) >> 12;
                rot.vx = key->y2 + 0x400;
                rot.vy = key->x0 + 0x800;
                rot.vz = key->z4;
                scale.vx = scale.vy = scale.vz = work->scale8;
                if (frame < 0x18) {
                    bright = 0x1000;
                } else {
                    bright = ex->math_cos(((frame - 0x18) << 10) / 6);
                }
                ex->Hi_SetEffModelSlice(work->model1C, 0);
                ex->Hi_DrawSliceEffModel(&rot, &pos, (s32)&scale, work->model1C);
                ex->Hi_ModifyEffModelRGB(work->model1C, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
                quadRot.vx = 0;
                quadRot.vy = 0;
                spinFrame = work->frame4;
                quadRot.vz = spinFrame << 5;
                quadRot.pad = 0;
                env->otZ = 0x180;
                if (frame < 0x14) {
                    quadAlpha = 0x1000;
                } else {
                    quadAlpha = ex->math_cos(((frame - 0x14) << 10) / 10);
                }
                for (j = 0; j < 0x11; j++) {
                    quadKey = (Fx198Key *)ex->btlfx_getFileData(entry->file, fx_198_quadPathFileIds[j]);
                    quadKey = (Fx198Key *)((u8 *)quadKey + 4);
                    if (frame < 0x15) {
                        quadKey += frame;
                    } else {
                        quadKey += 0x15;
                    }
                    quadPos.vx = work->posC.vx + ((quadKey->x0 * work->scale8) >> 12);
                    quadPos.vy = work->posC.vy + ((quadKey->y2 * work->scale8) >> 12);
                    quadPos.vz = work->posC.vz + ((quadKey->z4 * work->scale8) >> 12);
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, FX_TSUNAMI_WATER_SPLASH), &quadPos, NULL, 0x2000, work->frame4 + j * 4 + j, 1, quadAlpha);
                    quadRot.vz += 0x40;
                }
                if (frame < 0x14) {
                    quadAlpha = ex->math_cos((frame << 10) / 20);
                    j = 0;
                    for (; j < 0x11; j++) {
                        quadKey = (Fx198Key *)ex->btlfx_getFileData(entry->file, fx_198_quadPathFileIds[j]);
                        quadKey = (Fx198Key *)((u8 *)quadKey + 4);
                        quadPos.vx = work->posC.vx + ((quadKey->x0 * work->scale8) >> 12);
                        quadPos.vy = work->posC.vy + ((quadKey->y2 * work->scale8) >> 12);
                        quadPos.vz = work->posC.vz + ((quadKey->z4 * work->scale8) >> 12);
                        ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, FX_TSUNAMI_WATER_SPLASH), &quadPos, NULL, 0x1800, work->frame4 + j * 4 + j, 1, quadAlpha);
                        quadRot.vz += 0x40;
                    }
                }
                env->otZ = 0;
            }
            if (frame >= 0x14) {
                quadRot.vx = 0;
                quadRot.vy = 0;
                spinFrame = work->frame4;
                quadRot.vz = spinFrame << 5;
                quadRot.pad = 0;
                env->otZ = 0x180;
                t = ((frame - 0x14) << 12) / 20;
                rowKey = (Fx198Key *)(ex->btlfx_getFileData(entry->file, FX_TSUNAMI_SPLASH_ROW_KEYS) + 4);
                rowKey += frame;
                quadPos.vy = rowKey->y2;
                quadPos.vz = rowKey->z4;
                i = 0;
                for (; i < 0xA; i++) {
                    ex->btlfx_getFileData(entry->file, fx_198_quadPathFileIds[i]);
                    quadPos.vx = i * 0x100 - 0x500;
                    ex->btlfx_drawAnimQuads((u8 *)ex->btlfx_getFileData(entry->file, FX_TSUNAMI_WATER_SPLASH), &quadPos, NULL, 0x1800, work->frame4 + i * 4 + i, 1, ex->math_cos(t >> 2));
                    quadRot.vz += 0x40;
                }
                env->otZ = 0;
            }
            if (frame >= 0x28) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            if (frame > 0) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        rowKey = (Fx198Key *)ex->btlfx_getFileData(entry->file, FX_TSUNAMI_BACKDROP);
        ex->btlfx_drawBackdrop((BtlBgLayers *)rowKey, work->frame4);
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_198/nonmatchings/fx_198", fx_198_headerPtr);
