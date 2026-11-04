#include "fxmod/fx_12c.h"

INCLUDE_RODATA("asm/us/fx_12c/nonmatchings/fx_12c", fx_12c_headerPtr);

s32 fx_12c_op80(s32 mode, Fx12cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    CVECTOR col;
    SVECTOR rot;
    VECTOR sc;
    s32 *timer;
    s32 frame;
    s32 t;
    s32 height;
    s32 bright;
    s32 n;
    s32 i;

    if (mode == 0) {
        *args = 0x30;
    } else if (mode == 1) {
        fx_12c_env = env;
        ex->btlfx_initEffModels(fx_12c_effModels, 10);
        for (i = 7, timer = &work->timer0[7]; i >= 0; i--) {
            *timer-- = 0;
        }
        work->pos20.vx = 0;
        work->pos20.vy = 0;
        work->pos20.vz = -0x550;
        fx_12c_bounceModels[0].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_0), 0xFF);
        fx_12c_bounceModels[1].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_1), 0xFF);
        fx_12c_bounceModels[2].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_2), 0xFF);
        fx_12c_bounceModels[3].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_3), 0xFF);
        fx_12c_bounceModels[4].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_4), 0xFF);
        fx_12c_bounceModels[5].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_5), 0xFF);
        fx_12c_bounceModels[6].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_6), 0xFF);
        fx_12c_bounceModels[7].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_7), 0xFF);
        fx_12c_bounceModels[8].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_8), 0xFF);
        fx_12c_bounceModels[9].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_CHUNK_9), 0xFF);
        for (i = 0; i < 9; i++) {
            setVector(&fx_12c_bounceModels[i].rot24, 0, 0, 0);
            setVector(&fx_12c_bounceModels[i].scale4, 0xF00, 0, 0xF00);
            setVector(&fx_12c_bounceModels[i].pos14, 0, 0, -0x550);
            fx_12c_bounceModels[i].speed2C = ex->btlfx_rand() & 0x3F;
            fx_12c_bounceModels[i].frame30 = -(ex->btlfx_rand() & 7);
        }
        fx_12c_intensity = 0;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)env->camera);
        env->camera->points[0].vy = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_drawBgOffscreen(0x1C0, 0x100, 0x100, 0x100, fx_12c_bgOt, 0, -0x550, -0x1000);
            break;
        case 1:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos28);
            work->pos28.vy = 0;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_DUST_RING), (s32)&work->pos28);
            if (emitter != NULL) {
                emitter->otZ = -0x100;
            }
            ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_ROCK_DEBRIS), (s32)&work->pos28);
            break;
        case 9:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_EARTHQUAKE_RUBBLE), (s32)&work->pos20);
            if (emitter != NULL) {
                emitter->otZ = 0x100;
            }
            work->timer0[3] = 0x40;
            work->timer0[1] = 0x10;
            break;
        case 17:
            work->timer0[0] = 0x38;
            break;
        case 41:
            work->timer0[2] = 0x20;
            break;
        }
        if (work->timer0[1] != 0) {
            work->timer0[1]--;
            fx_12c_intensity += 0x3C;
        }
        if (work->timer0[2] != 0) {
            work->timer0[2]--;
            fx_12c_intensity -= 0x1E;
        }
        if (work->timer0[0] != 0) {
            work->timer0[0]--;
            for (i = 0; i < 9; i++) {
                if ((t = ++fx_12c_bounceModels[i].frame30) >= 0) {
                    height = fx_12c_bounceModels[i].speed2C * t - ((t * (t << 8)) >> 1);
                    if (height < 0) {
                        height = 0;
                        fx_12c_bounceModels[i].speed2C = ex->btlfx_rand() % (fx_12c_intensity + 1);
                        if (work->timer0[0] < 8) {
                            fx_12c_bounceModels[i].frame30 = -100;
                        } else {
                            fx_12c_bounceModels[i].frame30 = 0;
                        }
                    }
                    fx_12c_bounceModels[i].scale4.vy = height;
                    ex->Hi_DrawEffModel(&fx_12c_bounceModels[i].rot24, &fx_12c_bounceModels[i].pos14, (s32)&fx_12c_bounceModels[i].scale4,
                                        fx_12c_bounceModels[i].model0);
                }
            }
        }
        if (work->timer0[3] != 0) {
            n = --work->timer0[3];
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            sc.vx = 0x1000;
            sc.vy = (n << 1) + 0x1000;
            sc.vz = 0x1000;
            bright = fx_12c_intensity / 15;
            col.r = col.g = col.b = bright;
            ex->btlfx_drawScreenGrid(5, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &sc, &col, 1, NULL);
            ex->btlfx_drawScreenTint((u32 *)&env->ot[4], 2, bright, bright, bright);
        }
        return frame >= 0x80;
    }
    return 0;
}
