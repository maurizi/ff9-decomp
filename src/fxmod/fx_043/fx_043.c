#include "fxmod/fx_043.h"

static s32 fx_043_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_043_env);
    s32 ratio = frame << 10;

    if (ratio > 0x1000) {
        ratio = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&fx_043_particleFrom, (s16 *)&fx_043_particleTo, ratio, (s16 *)pos);
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_043_op80(s32 mode, Fx043Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx043Work80);
    } else if (mode == 1) {
        fx_043_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        work->pos14.vy -= FX_INK_BLOT_HEIGHT;
        fx_043_particleTo.vx = work->pos14.vx;
        fx_043_particleTo.vy = work->pos14.vy;
        fx_043_particleTo.vz = work->pos14.vz;
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->posC);
        fx_043_particleFrom.vx = work->posC.vx;
        fx_043_particleFrom.vy = work->posC.vy;
        fx_043_particleFrom.vz = work->posC.vz;
        env->camera->points[0].vx = work->posC.vx;
        env->camera->points[0].vy = work->posC.vy;
        env->camera->points[0].vz = work->posC.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_INK_BLOTS_2), (s32)&fx_043_emitPos);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_043_trails;
            work->emit0->trailSlots = FX_INK_BLOTS_2_TRAIL_SLOTS;
            work->emit0->trailStride = FX_INK_BLOTS_2_TRAIL_STRIDE;
        }
        if (work->emit0 != NULL) {
            ((s32 *)work->emit0->file)[2] = (s32)fx_043_op80ParticleMotion;
        }
    } else {
        return entry->frame >= FX_INK_FRAMES;
    }
    return 0;
}

s32 fx_043_op81(s32 mode, Fx043Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx043Work81);
    } else if (mode == 1) {
        fx_043_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&fx_043_particleFrom, (s16 *)&work->aim20);
        ex->btlfx_pointAhead((s16 *)&work->pos8, (s16 *)&work->aim20, FX_INK_BLOTS_REACH, (s16 *)&work->pos18);
        work->pos18.vy -= FX_INK_BLOT_HEIGHT;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_INK_BLOTS), (s32)&work->pos18);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_043_trails2;
            work->emit0->trailSlots = FX_INK_BLOTS_TRAIL_SLOTS;
            work->emit0->trailStride = FX_INK_BLOTS_TRAIL_STRIDE;
        }
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_INK_GREY_RINGS), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim20.vx;
                work->emit0->rot.vy = work->aim20.vy;
                work->emit0->rot.vz = work->aim20.vz;
            }
        }
        return frame >= FX_INK_SPLAT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_043/nonmatchings/fx_043", fx_043_headerPtr);
