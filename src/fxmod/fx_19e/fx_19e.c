#include "fxmod/fx_19e.h"

s32 fx_19e_op80(s32 mode, Fx19eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        work->unk4.vx = env->targets->centerX;
        work->unk4.vy = env->targets->centerY;
        work->unk4.vz = env->targets->centerZ;
        fx_19e_env = env;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->otZ = 0x80;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_19e_op81(s32 mode, Fx19eWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        fx_19e_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk0);
        work->unk18.vx = env->targets->centerX;
        work->unk18.vy = env->targets->centerY;
        work->unk18.vz = env->targets->centerZ;
        ex->btlfx_aimAngles((s16 *)&work->unk0, (s16 *)&work->unk18, (s16 *)&work->unk8);
        work->unk8.vy += 0x800;
        ex->btlfx_pointAhead((s16 *)&work->unk18, (s16 *)&work->unk8, 0x514, (s16 *)&work->unk10);
    } else {
        s32 frame = entry->frame;
        return fxm_moveUnitArc(0x10, &work->unk10, frame, 7, 0, &work->unk20) != 0;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_19e_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_19e_env);
    SVECTOR pos;
    s32 t;

    if (frame == 0) {
        ex->btlfx_getUnitPos(unit, (s16 *)start);
    }
    t = (frame << 12) / n;
    fxm_lerpSvec(start, dst, t, &pos);
    if (height) {
        pos.vy -= (ex->math_sin(t >> 1) * height) >> 12;
    }
    ex->btlfx_setUnitPos(unit, (s16 *)&pos);
    return frame >= n;
}

static s32 fxm_turnUnitBy(s32 unit, s32 frame, s32 n, SVECTOR *arg3, SVECTOR *delta) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_19e_env);
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
    BtlfxExports *ex = BTLFX_EXPORTS(fx_19e_env);
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

INCLUDE_RODATA("asm/us/fx_19e/nonmatchings/fx_19e", fx_19e_headerPtr);
