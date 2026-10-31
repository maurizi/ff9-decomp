#include "fxmod/fx_088.h"

s32 fx_088_op80(s32 mode, Fx088Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        fx_088_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_088_trails;
            work->emit0->trailSlots = 0x19;
            work->emit0->trailStride = 0x30;
        }
        ex->btlfx_aimAngles((s16 *)&work->pos8, &env->targets->centerX, (s16 *)&work->aim18);
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim18.vx;
                work->emit0->rot.vy = work->aim18.vy;
                work->emit0->rot.vz = work->aim18.vz;
            }
        }
        if (frame == 0xC) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&env->targets->centerX);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x20;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_088_op81(s32 mode, Fx088Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        fx_088_env = env;
        work->pos8.vz = env->targets->centerZ;
    } else {
        frame = entry->frame;
        return fxm_turnUnitBy(0x10, frame, 0xF, &work->pos8, &work->turn0) != 0;
    }
    return 0;
}

static s32 fxm_turnUnitBy(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_088_env);
    SVECTOR rot;
    SVECTOR pos;

    ex->btlfx_getUnitPos(unit, (s16 *)&pos);
    if (frame == 0) {
        ex->btlfx_getUnitRot(unit, (s16 *)&rot);
    }
    ex->btlfx_getUnitRot(unit, (s16 *)&rot);
    rot.vx += delta->vx / n;
    rot.vy += delta->vy / n;
    rot.vz += delta->vz / n;
    ex->btlfx_setUnitRot(unit, (s16 *)&rot);
    return frame >= n - 1;
}

static s32 fxm_turnUnitToward(s32 unit, s32 target, s32 frame, s32 n, s32 turnBack, s32 relative, SVECTOR *ang) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_088_env);
    SVECTOR from;
    SVECTOR rot;
    SVECTOR to;

    if (frame == 0) {
        ex->btlfx_getUnitPos(unit, (s16 *)&from);
        ex->btlfx_getUnitPos(target, (s16 *)&to);
        ex->btlfx_aimAngles((s16 *)&from, (s16 *)&to, (s16 *)ang);
        if (relative) {
            ex->btlfx_getUnitRot(unit, (s16 *)&rot);
            ang->vx -= rot.vx;
            ang->vy -= rot.vy;
            ang->vz -= rot.vz;
            if (ang->vy >= 0x800) {
                ang->vy = 0x1000 - ang->vy;
            } else if (ang->vy < -0x7FF) {
                ang->vy = ang->vy + 0x1000;
            }
        }
        if (turnBack == 1) {
            ang->vy += 0x800;
        }
    }
    ex->btlfx_getUnitRot(unit, (s16 *)&rot);
    rot.vx += ang->vx / n;
    rot.vy += ang->vy / n;
    rot.vz += ang->vz / n;
    ex->btlfx_setUnitRot(unit, (s16 *)&rot);
    return frame >= n - 1;
}

INCLUDE_RODATA("asm/us/fx_088/nonmatchings/fx_088", fx_088_headerPtr);
