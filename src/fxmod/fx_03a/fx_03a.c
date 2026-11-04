#include "fxmod/fx_03a.h"

s32 fx_03a_op80(s32 mode, Fx03aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 t;
    s32 height;
    Pool6CEntry *emitter;
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    s32 bright;

    if (mode == 0) {
        *args = 0x30;
    } else if (mode == 1) {
        fx_03a_env = env;
        ex->btlfx_initEffModels(fx_03a_effModels, 0xA);
        for (i = 7; i >= 0; i--) {
            ((s32 *)work)[i] = 0;
        }
        work->pos20.vx = 0;
        work->pos20.vy = 0;
        work->pos20.vz = -0x550;
        fx_03a_models[0].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_0), 0xFF);
        fx_03a_models[1].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_1), 0xFF);
        fx_03a_models[2].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_2), 0xFF);
        fx_03a_models[3].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_3), 0xFF);
        fx_03a_models[4].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_4), 0xFF);
        fx_03a_models[5].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_5), 0xFF);
        fx_03a_models[6].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_6), 0xFF);
        fx_03a_models[7].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_7), 0xFF);
        fx_03a_models[8].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_8), 0xFF);
        fx_03a_models[9].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_GROUND_PIECE_9), 0xFF);
        for (i = 0; i < 9; i++) {
            setVector(&fx_03a_models[i].rot24, 0, 0, 0);
            setVector(&fx_03a_models[i].scale4, 0xF00, 0, 0xF00);
            setVector(&fx_03a_models[i].pos14, 0, 0, -0x550);
            fx_03a_models[i].speed2C = ex->btlfx_rand() & 0x3F;
            fx_03a_models[i].time30 = -(ex->btlfx_rand() & 7);
        }
        fx_03a_bouncePower = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x100, 0x100, fx_03a_bgOt, 0, -0x550, -0x1000);
            break;
        case 1:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_RUBBLE), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            work->gridC = 0x40;
            work->bounce0 = 0x40;
            work->powerUp4 = 0x10;
            break;
        case 33:
            work->powerDown8 = 0x20;
            break;
        }
        if (work->powerUp4 != 0) {
            work->powerUp4--;
            fx_03a_bouncePower += 0x3C;
        }
        if (work->powerDown8 != 0) {
            work->powerDown8--;
            fx_03a_bouncePower -= 0x1E;
        }
        if (work->bounce0 != 0) {
            work->bounce0--;
            for (i = 0; i < 9; i++) {
                t = ++fx_03a_models[i].time30;
                if (t >= 0) {
                    height = fx_03a_models[i].speed2C * t - ((t * (t << 8)) >> 1);
                    if (height < 0) {
                        height = 0;
                        fx_03a_models[i].speed2C = ex->btlfx_rand() % (fx_03a_bouncePower + 1);
                        if (work->bounce0 < 8) {
                            fx_03a_models[i].time30 = -100;
                        } else {
                            fx_03a_models[i].time30 = 0;
                        }
                    }
                    fx_03a_models[i].scale4.vy = height;
                    ex->Hi_DrawEffModel(&fx_03a_models[i].rot24, &fx_03a_models[i].pos14, (s32)&fx_03a_models[i].scale4,
                                        fx_03a_models[i].model0);
                }
            }
        }
        if (work->gridC != 0) {
            work->gridC--;
            /* the assignment inside the sum keeps 0x1000 in one register for all three fields */
            scale.vy = work->gridC * 2 + (scale.vx = 0x1000);
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            bright = fx_03a_bouncePower / 15;
            col.b = bright;
            col.g = bright;
            col.r = bright;
            ex->btlfx_drawScreenGrid(5, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, NULL);
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 4), 2, bright, bright, bright);
        }
        return frame >= 0x58;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03a/nonmatchings/fx_03a", fx_03a_headerPtr);
