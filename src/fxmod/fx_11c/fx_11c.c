#include "fxmod/fx_11c.h"

s32 fx_11c_op80(s32 mode, Fx11cWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx11cWork80);
    } else if (mode == 1) {
        fx_11c_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        work->pos10.vx = work->pos8.vx;
        work->pos10.vy = work->pos8.vy;
        work->pos10.vz = work->pos8.vz;
        work->pos10.vy += 0x12C;
        work->pos10.vz -= 0x12C;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_WIND_TRAIL_2), (s32)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_WIND_TRAIL), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_11c_op80Trails0;
            work->emit0->trailSlots = FX_HIGH_WIND_WIND_TRAIL_SLOTS;
            work->emit0->trailStride = FX_HIGH_WIND_WIND_TRAIL_STRIDE;
        }
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_11c_op80Trails1;
            work->emit4->trailSlots = FX_HIGH_WIND_WIND_TRAIL_SLOTS;
            work->emit4->trailStride = FX_HIGH_WIND_WIND_TRAIL_STRIDE;
        }
    } else {
        return entry->frame >= FX_HIGH_WIND_GUST_FRAMES;
    }
    return 0;
}

s32 fx_11c_op81(s32 mode, Fx11cWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx11cBurst *initBurst;
    Fx11cBurst *burst;
    s32 frame;
    s32 i;
    s32 j;

    if (mode == 0) {
        *args = sizeof(Fx11cWork81);
    } else if (mode == 1) {
        work->pos20.vx = env->targets->centerX;
        work->pos20.vy = env->targets->centerY;
        work->pos20.vz = env->targets->centerZ;
        fx_11c_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_TEAL_SWIRL), (s32)&work->pos20);
        work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_CLOUD), (s32)&work->pos20);
        work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_TEAL_RINGS), (s32)&work->pos20);
        initBurst = fx_11c_op81Bursts;
        for (i = 0; i < FX_HIGH_WIND_BURST_COUNT; i++, initBurst++) {
            initBurst->active10 = 0;
            initBurst->frame12 = 0;
            initBurst->startFrame14 = i * FX_HIGH_WIND_BURST_INTERVAL;
            initBurst->pos0.vx = work->pos20.vx;
            initBurst->pos0.vy = work->pos20.vy;
            initBurst->pos0.vz = work->pos20.vz;
            initBurst->pos8.vx = work->pos20.vx;
            initBurst->pos8.vy = work->pos20.vy;
            initBurst->pos8.vz = work->pos20.vz;
            if (ex->btlfx_rand() & 1) {
                initBurst->pos8.vx += ex->btlfx_rand() % 800;
            } else {
                initBurst->pos8.vx -= ex->btlfx_rand() % 800;
            }
            if (ex->btlfx_rand() & 1) {
                initBurst->pos8.vz += ex->btlfx_rand() % 800;
            } else {
                initBurst->pos8.vz -= ex->btlfx_rand() % 800;
            }
        }
    } else {
        frame = entry->frame;
        burst = fx_11c_op81Bursts;
        for (j = 0; j < FX_HIGH_WIND_BURST_COUNT; j++, burst++) {
            if (burst->startFrame14 == frame) {
                work->emit4[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, burst->fileId16),
                                                  (s32)&burst->pos0);
                burst->active10 = 1;
            } else if (burst->active10 != 0) {
                burst->frame12++;
            }
        }
        return frame >= FX_HIGH_WIND_BURSTS_FRAMES;
    }
    return 0;
}

s32 fx_11c_op82(s32 mode, Fx11cWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx11cWork82);
    } else if (mode == 1) {
        work->pos18.vx = env->targets->centerX;
        work->pos18.vy = env->targets->centerY;
        work->pos18.vz = env->targets->centerZ;
        work->pos20.vx = env->targets->centerX;
        work->pos20.vy = env->targets->centerY;
        work->pos20.vz = env->targets->centerZ;
        work->pos28.vx = env->targets->centerX;
        work->pos28.vy = env->targets->centerY;
        work->pos28.vz = env->targets->centerZ;
        work->pos20.vx -= FX_HIGH_WIND_SIDE_CLOUD_OFFSET;
        work->pos28.vx += FX_HIGH_WIND_SIDE_CLOUD_OFFSET;
        fx_11c_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_WIND_TRAIL_3), (s32)&work->pos18);
        work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_GREY_SWIRL), (s32)&work->pos18);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_11c_op82Trails;
            work->emit0->trailSlots = FX_HIGH_WIND_WIND_TRAIL_3_SLOTS;
            work->emit0->trailStride = FX_HIGH_WIND_WIND_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        if (frame == FX_HIGH_WIND_STREAKS_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_WIND_STREAKS), (s32)&work->pos18);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_CLOUD_2), (s32)&work->pos18);
        }
        if (frame == FX_HIGH_WIND_SIDE_CLOUDS_FRAME) {
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_CLOUD_2), (s32)&work->pos20);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HIGH_WIND_CLOUD_2), (s32)&work->pos28);
        }
        return frame >= FX_HIGH_WIND_STORM_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_11c/nonmatchings/fx_11c", fx_11c_headerPtr);
