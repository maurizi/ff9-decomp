#include "fxmod/fx_033.h"

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_033_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case FX_SNORT_EASE_LINEAR:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case FX_SNORT_EASE_SQUARED:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case FX_SNORT_EASE_INV_SQUARED:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

s32 fx_033_op80(s32 mode, Fx033Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 dist;
    s32 dz;

    if (mode == 0) {
        *args = sizeof(Fx033Work);
    } else if (mode == 1) {
        work->timer0 = 0;
        fx_033_env = env;
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitHomePos(0, (s16 *)&work->pos14);
        work->emit2C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SNORT_SMOKE_STREAKS), (s32)&work->pos4);
        ex->btlfx_aimAngles((s16 *)&work->pos4, (s16 *)&work->posC, (s16 *)&work->aim24);
        if (work->emit2C != NULL) {
            work->emit2C->trails = (s32)fx_033_trails;
            work->emit2C->trailSlots = FX_SNORT_SMOKE_STREAKS_TRAIL_SLOTS;
            work->emit2C->trailStride = FX_SNORT_SMOKE_STREAKS_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        if (work->emit2C != NULL) {
            if (work->emit2C->state == BTLFX_EMIT_DONE) {
                work->emit2C = NULL;
            }
            if (work->emit2C != NULL) {
                work->emit2C->rot.vx = work->aim24.vx;
                work->emit2C->rot.vy = work->aim24.vy;
                work->emit2C->rot.vz = work->aim24.vz;
            }
        }
        if (frame == FX_SNORT_KNOCKBACK_FRAME) {
            work->timer0 = frame;
        }
        if (work->timer0 != 0) {
            work->timer0--;
            dist = fxm_easeLerp(FX_SNORT_EASE_SQUARED, FX_SNORT_KNOCKBACK_FRAMES, work->timer0, 0, FX_SNORT_PUSH_DIST);
            work->pos1C.vx = work->pos14.vx + ((ex->math_sin(work->aim24.vy) * dist) >> 12);
            dz = ex->math_cos(work->aim24.vy) * dist;
            work->pos1C.vy = work->pos14.vy - (dist >> 6);
            work->pos1C.vz = work->pos14.vz + (dz >> 12);
            ex->btlfx_setUnitPos(0, (s16 *)&work->pos1C);
        }
        return frame >= FX_SNORT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_033/nonmatchings/fx_033", fx_033_headerPtr);
