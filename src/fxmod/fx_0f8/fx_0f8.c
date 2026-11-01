#include "fxmod/fx_0f8.h"

s32 fx_0f8_op80(s32 mode, Fx0f8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        fx_0f8_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos4);
        work->centerC.vx = env->targets->centerX;
        work->centerC.vy = env->targets->centerY;
        work->centerC.vz = env->targets->centerZ;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos4);
        ex->btlfx_aimAngles((s16 *)&work->pos4, (s16 *)&work->centerC, (s16 *)&work->aimRot14);
        work->aimRot14.vy -= 0x400;
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aimRot14.vx;
                work->emit0->rot.vy = work->aimRot14.vy;
                work->emit0->rot.vz = work->aimRot14.vz;
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

s32 fx_0f8_op81(s32 mode, Fx0f8Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 i;
    s32 j;
    Fx0f8Spawn *initSpawn;
    Fx0f8Spawn *spawn;

    if (mode == 0) {
        *args = 0x50;
    } else if (mode == 1) {
        work->pos28.vx = env->targets->centerX;
        work->pos28.vy = env->targets->centerY;
        work->pos28.vz = env->targets->centerZ;
        fx_0f8_env = env;
        for (initSpawn = fx_0f8_spawns, i = 0; i < 4; i++, initSpawn++) {
            (work->emitPos30 + i)->vx = work->pos28.vx;
            (work->emitPos30 + i)->vy = work->pos28.vy;
            (work->emitPos30 + i)->vz = work->pos28.vz;
            initSpawn->frame2 = i * 4;
            rot.vx = 0;
            rot.vy = i << 10;
            rot.vz = 0;
            ex->btlfx_pointAhead((s16 *)&work->pos28, (s16 *)&rot, ex->btlfx_rand() % 400 + 0x578, (s16 *)&work->emitPos30[i]);
        }
    } else {
        frame = entry->frame;
        if (frame == 0xF) {
            work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos28);
            work->emit24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos28);
        }
        for (spawn = fx_0f8_spawns, j = 0; j < 4; j++, spawn++) {
            if (frame == spawn->frame2) {
                work->emit0[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, spawn->block0), (s32)&work->emitPos30[j]);
                work->emit10[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->emitPos30[j]);
            }
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 1), frame);
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 2), frame);
        return frame >= 0x2D;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f8/nonmatchings/fx_0f8", fx_0f8_headerPtr);
