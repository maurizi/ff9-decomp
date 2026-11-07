#include "fxmod/fx_0aa.h"

static s32 fx_0aa_op80ParticleMotion0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_lerpParticle(pt, pos, &fx_0aa_pathFrom0, &fx_0aa_pathTo0, frame, n);
}

static s32 fx_0aa_op80ParticleMotion1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_lerpParticle(pt, pos, &fx_0aa_pathFrom1, &fx_0aa_pathTo1, frame, n);
}

static s32 fxm_lerpParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 frame, s32 n) {
    BTLFX_EXPORTS(fx_0aa_env)->btlfx_lerpVec((s16 *)from, (s16 *)to, frame << 9, (s16 *)pos);
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0aa_op80(s32 mode, Fx0aaWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0aaWork80);
    } else if (mode == 1) {
        fx_0aa_env = env;
    } else {
        frame = entry->frame;
        if (frame == FX_SNOWSTORM_STREAM_FRAME) {
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 0x12, (u16 *)&work->pos10);
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 0x2B, (u16 *)&work->pos18);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNOWSTORM_SNOW_BURST), (s32)&work->pos10);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNOWSTORM_SNOW_BURST_2), (s32)&work->pos18);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNOWSTORM_SNOW_STREAM), (s32)&fx_0aa_emitStartPos0);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNOWSTORM_SNOW_STREAM_2), (s32)&fx_0aa_emitStartPos1);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_0aa_trails;
                work->emit8->trailSlots = FX_SNOWSTORM_SNOW_STREAM_TRAIL_SLOTS;
                work->emit8->trailStride = FX_SNOWSTORM_SNOW_STREAM_TRAIL_STRIDE;
            }
            if (work->emitC != NULL) {
                work->emitC->trails = (s32)fx_0aa_trails2;
                work->emitC->trailSlots = FX_SNOWSTORM_SNOW_STREAM_TRAIL_SLOTS;
                work->emitC->trailStride = FX_SNOWSTORM_SNOW_STREAM_TRAIL_STRIDE;
            }
            if (work->emit8 != NULL) {
                work->emit8->file->m.module.update = fx_0aa_op80ParticleMotion0;
            }
            if (work->emitC != NULL) {
                work->emitC->file->m.module.update = fx_0aa_op80ParticleMotion1;
            }
            fx_0aa_pathFrom0.vx = work->pos10.vx;
            fx_0aa_pathFrom0.vy = work->pos10.vy;
            fx_0aa_pathFrom0.vz = work->pos10.vz;
            fx_0aa_pathFrom1.vx = work->pos18.vx;
            fx_0aa_pathFrom1.vy = work->pos18.vy;
            fx_0aa_pathFrom1.vz = work->pos18.vz;
            fx_0aa_pathTo0.vx = env->targets->centerX;
            fx_0aa_pathTo0.vy = env->targets->centerY;
            fx_0aa_pathTo0.vz = env->targets->centerZ;
            fx_0aa_pathTo1.vx = env->targets->centerX;
            fx_0aa_pathTo1.vy = env->targets->centerY;
            fx_0aa_pathTo1.vz = env->targets->centerZ;
            fx_0aa_pathTo0.vx = fx_0aa_pathFrom0.vx;
            fx_0aa_pathTo1.vx = fx_0aa_pathFrom1.vx;
        }
        return frame >= FX_SNOWSTORM_BLOW_FRAMES;
    }
    return 0;
}

s32 fx_0aa_op81(s32 mode, Fx0aaWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0aaWork81);
    } else if (mode == 1) {
        work->pos14.vx = env->targets->centerX;
        work->pos14.vy = env->targets->centerY;
        fx_0aa_env = env;
        work->pos14.vz = env->targets->centerZ;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNOWSTORM_FROST_RINGS), (s32)&work->pos14);
            break;
        case FX_SNOWSTORM_MIST_FRAME:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNOWSTORM_MIST), (s32)&work->pos14);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNOWSTORM_SNOW_TRAIL), (s32)&work->pos14);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)fx_0aa_trails3;
                work->emit0->trailSlots = FX_SNOWSTORM_SNOW_TRAIL_TRAIL_SLOTS;
                work->emit0->trailStride = FX_SNOWSTORM_SNOW_TRAIL_TRAIL_STRIDE;
            }
            break;
        case FX_SNOWSTORM_FLAKES_FRAME:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNOWSTORM_ICE_FLAKES), (s32)&work->pos14);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNOWSTORM_FLAKES), (s32)&work->pos14);
            break;
        }
        return frame >= FX_SNOWSTORM_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0aa/nonmatchings/fx_0aa", fx_0aa_headerPtr);
