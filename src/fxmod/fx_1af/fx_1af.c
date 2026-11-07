#include "fxmod/fx_1af.h"

s32 fx_1af_op80(s32 mode, Fx1afWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR unitPos;
    VECTOR pos;
    Pool6CEntry *emitter;
    s32 frame;
    s32 state;
    s32 t;
    s32 ang;
    s32 i;
    s32 j;
    s32 sliceY;

    if (mode == 0) {
        *args = 0x54;
    } else if (mode == 1) {
        fx_1af_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&unitPos);
        work->pos8[0] = unitPos.vx;
        work->pos8[1] = unitPos.vy;
        work->pos8[2] = unitPos.vz;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot14);
        ex->btlfx_getUnitScale(0x10, (VECTOR *)work->scale1C);
        fx_1af_task = entry;
        work->state0 = 0;
        work->frame4 = 0;
        work->pos28.vx = work->pos8[0];
        work->pos28.vy = work->pos8[1];
        work->pos28.vz = work->pos8[2];
        work->pos28.vy = 0;
        work->rot14.vy += 0x800;
        work->pos38.vx = work->pos28.vx;
        work->pos38.vy = work->pos28.vy;
        work->pos38.vz = work->pos28.vz;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame > 0) {
                work->state0 = 1;
                entry->frame = -1;
                ex->btlfx_fadeUnitById(0x10, 0);
                ex->btlfx_moveSummonToHeap(0x50);
                ex->btlfx_initEffModels(fx_1af_effModels, 5);
                for (i = 0; i < 5; i++) {
                    work->model40[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_1af_modelFileIds[i]), 0);
                }
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0x80, 0x80, 0xAA);
            }
            break;
        case 1:
            t = (frame << 12) / 180;
            if (frame == 0) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0x40, 0x40, 0x55);
            }
            if (frame == 1) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0x2A, 0x2A, 0x38);
            }
            if (frame == 0x14) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0x2A, 0x2A, 0x38);
            }
            if (frame == 0x28) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 2), BTLFX_ABR_ADD, 0x2A, 0x2A, 0x38);
            }
            if (frame >= 0) {
                sliceY = t;
                if (frame < 4) {
                    work->rot14.vz += 0xC;
                    work->pos38.vx += 0xE;
                    work->pos28.vx += 0xC;
                } else {
                    work->rot14.vz += 2;
                    work->pos38.vx += 8;
                    work->pos28.vx += 6;
                }
                pos.vx = work->pos8[0];
                pos.vy = work->pos8[1];
                pos.vz = work->pos8[2];
                ang = t >> 2;
                pos.vy = sliceY;
                pos.vx += ex->btlfx_randCentered(ex->math_sin(ang) >> 6);
                pos.vz += ex->btlfx_randCentered(ex->math_sin(ang) >> 6);
                work->pos30.vx = pos.vx;
                work->pos30.vy = pos.vy;
                work->pos30.vz = pos.vz;
                work->pos30.vx += t >> 4;
                for (j = 0; j < 5; j++) {
                    ex->Hi_SetEffModelSlice(work->model40[j], sliceY);
                    ex->Hi_DrawSliceEffModel(&work->rot14, &pos, (s32)work->scale1C, work->model40[j]);
                }
            }
            if (frame == 0xA) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_SPECKS), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_ASH_COLUMN), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_ASH_COLUMN_2), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_SMOKE), (s32)&work->pos38);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x1E) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_ASH_COLUMN_3), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_ASH_COLUMN_4), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x45) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_DARK_SMOKE), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_SPECKS_2), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_OLIVE_RINGS), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame == 0x4B) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_PINK_RINGS), (s32)&work->pos28);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_1af_op80Trails;
                    emitter->trailSlots = 0x2C;
                    emitter->trailStride = 0x4C;
                    emitter->otZ = 0x140;
                }
            }
            if (frame >= 0xB4) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        work->frame4++;
    }
    return 0;
}

s32 fx_1af_op83(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0;
    } else if (mode == 1) {
        fx_1af_env = env;
        ex->btlfx_clearUnitStatus(0x10);
    } else {
        return 1;
    }
    return 0;
}

s32 fx_1af_op87(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR unitPos;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        fx_1af_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&unitPos);
        env->camera->points[3].vx = unitPos.vx;
        env->camera->points[3].vy = unitPos.vy;
        env->camera->points[3].vz = unitPos.vz;
        return 1;
    } else {
        return 0;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1af/nonmatchings/fx_1af", fx_1af_headerPtr);
