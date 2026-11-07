#include "fxmod/fx_1f1.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1f1_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

s32 fx_1f1_op80(s32 mode, s32 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx1f1Unit *unit;
    Pool6CEntry *emitter;
    SVECTOR unitPos;
    s32 frame;
    s32 i;
    s32 shade;
    s32 delay;
    Fx1f1Model *models;
    VECTOR *modelPos;
    VECTOR *modelScale;

    if (mode == 0) {
        *args = 0x40;
    } else if (mode == 1) {
        fx_1f1_env = env;
        ex->btlfx_clearUnitStatus(BTLFX_UNIT_CASTER);
        ex->btlfx_initEffModels(fx_1f1_effModels, 8);
        for (i = 15; i >= 0; i--) {
            work[i] = 0;
        }
        for (i = 0; i < env->targets->targetCount; i++) {
            unit = &fx_1f1_units[i];
            unit->model18 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_DEATH3_RUNE_CYLINDER), 1);
            unit->model1C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_DEATH3_RUNE_CYLINDER_2), 1);
            ex->btlfx_getUnitPos(i, (s16 *)&fx_1f1_units[i].posC);
            setVector(&fx_1f1_units[i].rot20, 0, 0, 0);
            setVector(&fx_1f1_units[i].rot28, 0, 0, 0);
            setVector(&fx_1f1_units[i].scale30, 0x1100, 0x1000, 0x1100);
            setVector(&fx_1f1_units[i].scale40, 0x1000, 0x1000, 0x1000);
            setVector(&fx_1f1_units[i].pos50, unit->posC.vx, -0x800, unit->posC.vz);
            delay = fx_1f1_unitDelays[i];
            unit->timer4 = 0x20;
            unit->unk14 = 0;
            unit->delay0 = delay;
        }
    } else {
        frame = entry->frame;
        if (frame == 0) {
            work[0] = 0x50;
        }
        if (work[0] != 0) {
            work[0]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                unit = &fx_1f1_units[i];
                if (unit->delay0 == 0) {
                    if (unit->timer4 != 0) {
                        unit->timer4--;
                        if (unit->timer4 == 0x1F) {
                            ex->btlfx_getUnitPos(i, (s16 *)&fx_1f1_units[i].posC);
                            unit->height8 = unit->posC.vy;
                            unit->posC.vy = 0;
                            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH3_MAGIC_CIRCLE), (s32)&fx_1f1_units[i].posC);
                            if (emitter != NULL) {
                                emitter->otZ = ex->btlfx_addRadiusSixth(i, 0x100);
                            }
                            ex->btlfx_playFileSound(6, entry->file, -1);
                            ex->btlfx_playFileSound(7, entry->file, -1);
                            ex->btlfx_playFileSound(8, entry->file, -1);
                        }
                        /* models holds the base so modelPos and modelScale are computed from it (base + 0x30, + 0x10) while its last
                         * use is the &models[i] giv: loop.c then hoists the base without dragging + 0x10 along */
                        models = fx_1f1_unitModels;
                        modelPos = &fx_1f1_unitModels[i].pos30;
                        modelScale = fx_1f1_unitModels[i].scale10;
                        ex->Hi_DrawEffModel(models[i].rot0, modelPos, (s32)modelScale, unit->model18);
                        ex->Hi_DrawEffModel(models[i].rot0 + 1, modelPos, (s32)(modelScale + 1), unit->model1C);
                        if (unit->timer4 >= 0x10) {
                            shade = (0x10 - unit->timer4) * 0x10;
                            ex->Hi_ModifyEffModelRGB(unit->model18, shade, shade, shade);
                            ex->Hi_ModifyEffModelRGB(unit->model1C, shade, shade, shade);
                            unit->pos50.vy = fxm_easeLerp(2, 0x10, unit->timer4 - 0x10, -0x800, 0);
                            unitPos.vy = fxm_easeLerp(2, 0x10, unit->timer4 - 0x10, unit->height8, 0);
                            unitPos.vx = unit->posC.vx;
                            unitPos.vz = unit->posC.vz;
                            ex->btlfx_setUnitPos(i, (s16 *)&unitPos);
                        }
                        if (unit->timer4 < 0x10) {
                            shade = (unit->timer4 - 0x10) * 0x10;
                            ex->Hi_ModifyEffModelRGB(unit->model18, shade, shade, shade);
                            ex->Hi_ModifyEffModelRGB(unit->model1C, shade, shade, shade);
                            ex->btlfx_fadeUnitById(i, unit->timer4 * 8);
                            unitPos.vy = unit->pos50.vy = fxm_easeLerp(1, 0x10, unit->timer4, 0, -0x800);
                            unitPos.vx = unit->pos50.vx;
                            unitPos.vz = unit->pos50.vz;
                            ex->btlfx_setUnitPos(i, (s16 *)&unitPos);
                        }
                        if (unit->timer4 == 0) {
                            unit->posC.vy = 0x3000;
                            ex->btlfx_setUnitPos(i, (s16 *)&fx_1f1_units[i].posC);
                            ex->btlfx_fadeUnitById(i, 0x80);
                            ex->btlfx_clearUnitStatus(i);
                        }
                        unit->rot28.vy += 0x13;
                    }
                } else {
                    unit->delay0--;
                }
            }
        }
        ex->btlfx_drawScrollMove(frame * 8, 0x1C0, 0x100, 0x20, 0x80, 0x1E0, 0x100);
        ex->btlfx_drawScrollMove(frame * 4, 0x1C0, 0x180, 0x20, 0x80, 0x1E0, 0x180);
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f1/nonmatchings/fx_1f1", fx_1f1_headerPtr);
