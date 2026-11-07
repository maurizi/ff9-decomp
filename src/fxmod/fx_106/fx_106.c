#include "fxmod/fx_106.h"

static s32 fx_106_op80ParticleMotion(VECTOR *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_106_env);
    s32 t = (frame - FX_MUCUS_ARC_START_FRAME) << 8;

    ex->btlfx_lerpVec((s16 *)&fx_106_pathStart, (s16 *)&fx_106_pathEnd, t, (s16 *)pos);
    pos->vy -= (ex->math_sin(t >> 1) * FX_MUCUS_ARC_HEIGHT) >> 12;
    out->vx = pos->vx << 12;
    out->vy = pos->vy << 12;
    out->vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_106_op80(s32 mode, Fx106Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx106Work80);
    } else if (mode == 1) {
        fx_106_env = env;
        ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->rot8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        fx_106_pathEnd.vx = work->pos10.vx;
        fx_106_pathEnd.vy = work->pos10.vy;
        fx_106_pathEnd.vz = work->pos10.vz;
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos18);
        ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&work->rot8, 0, (s16 *)&work->pos20);
        fx_106_pathStart.vx = work->pos20.vx;
        fx_106_pathStart.vy = work->pos20.vy;
        fx_106_pathStart.vz = work->pos20.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MUCUS_SLIME_BLOBS), (s32)&fx_106_emitPos);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MUCUS_BUBBLE_TRAIL), (s32)&work->pos20);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_106_op80Trails;
            work->emit0->trailSlots = FX_MUCUS_SLIME_TRAIL_SLOTS;
            work->emit0->trailStride = FX_MUCUS_SLIME_TRAIL_STRIDE;
            if (work->emit0 != NULL) {
                ((Fx106EffHdr *)work->emit0->file)->motion8 = fx_106_op80ParticleMotion;
                if (work->emit0 != NULL) {
                    work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
    } else {
        return entry->frame >= FX_MUCUS_SPIT_FRAMES;
    }
    return 0;
}

s32 fx_106_op81(s32 mode, Fx106Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx106Work81);
    } else if (mode == 1) {
        fx_106_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
        work->pos14.vx = work->posC.vx;
        work->pos14.vy = work->posC.vy;
        work->pos14.vz = work->posC.vz;
        work->pos14.vy -= 0x190;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MUCUS_BUBBLE_TRAIL_2), (s32)&work->pos14);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MUCUS_YELLOW_RINGS), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_106_op81Trails0;
            work->emit0->trailSlots = FX_MUCUS_BUBBLE_TRAIL_SLOTS;
            work->emit0->trailStride = FX_MUCUS_BUBBLE_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        if (frame == FX_MUCUS_SPLASH_FRAME) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MUCUS_SPLASH_STREAK), (s32)&work->posC);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_106_op81Trails1;
                work->emit8->trailSlots = FX_MUCUS_SPLASH_TRAIL_SLOTS;
                work->emit8->trailStride = FX_MUCUS_SPLASH_TRAIL_STRIDE;
            }
        }
        return frame >= FX_MUCUS_HIT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_106/nonmatchings/fx_106", fx_106_headerPtr);
