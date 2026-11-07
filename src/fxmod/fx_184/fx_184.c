#include "fxmod/fx_184.h"

s32 fx_184_op80(s32 mode, Fx184Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR dropPos;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx184Work80);
    } else if (mode == 1) {
        fx_184_env = env;
        ex->btlfx_initEffModels(fx_184_effModels, 1);
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos0);
        work->pos10.vx = work->pos0.vx;
        work->pos10.vy = work->pos0.vy;
        work->pos10.vz = work->pos0.vz;
        work->pos10.vy = FX_SPEAR_DROP_HEIGHT;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos8, (s16 *)&work->rot18);
        work->rot18.vx = ex->ratan2(work->pos8.vy - work->pos10.vy, work->pos10.vz - work->pos8.vz);
        work->scale30.vx = 0x1000;
        work->scale30.vy = 0x1000;
        work->scale30.vz = 0x1000;
        ex->btlfx_setWeaponVisible(BTLFX_UNIT_CASTER, 0);
        work->model40 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_SPEAR_SPEAR), 1);
    } else {
        frame = entry->frame;
        if (frame < 6) {
            ex->btlfx_lerpVec((s16 *)&work->pos10, (s16 *)&work->pos8, frame << 10, (s16 *)&dropPos);
            work->pos20.vx = dropPos.vx;
            work->pos20.vy = dropPos.vy;
            work->pos20.vz = dropPos.vz;
        }
        ex->Hi_SetEffModelSlice(work->model40, work->pos20.vy);
        ex->Hi_DrawSliceEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model40);
        if (frame >= FX_SPEAR_FRAMES) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_184_op81(s32 mode, Fx184Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx184Work81);
    } else if (mode == 1) {
        fx_184_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        work->pos4.vy = 0;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SPEAR_WHITE_RING), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        return entry->frame >= FX_SPEAR_RING_FRAMES;
    }
    return 0;
}

s32 fx_184_op82(s32 mode, Fx184Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx184Work82);
    } else if (mode == 1) {
        fx_184_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos0);
        work->unk8.vx = work->pos0.vx;
        work->unk8.vy = work->pos0.vy;
        work->unk8.vz = work->pos0.vz;
        work->pos0.vy = FX_SPEAR_JUMP_HEIGHT;
        work->unk8.vy = 0;
        ex->btlfx_setUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->pos0);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        work->rot20.vx = 0;
        work->rot20.vy = 0x800;
        work->rot20.vz = 0;
        ex->btlfx_pointAhead((s16 *)&work->pos10, (s16 *)&work->rot20, ex->btlfx_getUnitRadius(0) + FX_SPEAR_LAND_DISTANCE, (s16 *)&work->pos18);
        ex->btlfx_setUnitVisible(BTLFX_UNIT_CASTER, 1);
        work->state30 = FX_SPEAR_STATE_LAND;
        work->duration34 = FX_SPEAR_JUMP_FRAMES;
        work->frame38 = 0;
    } else {
        if (entry->frame == FX_SPEAR_WEAPON_FRAME) {
            ex->btlfx_setWeaponVisible(BTLFX_UNIT_CASTER, 1);
        }
        switch (work->state30) {
        case FX_SPEAR_STATE_LAND:
            if (fxm_moveUnitArc(BTLFX_UNIT_CASTER, &work->pos18, work->frame38, work->duration34, 0, &work->pos28)) {
                work->state30 = FX_SPEAR_STATE_WAIT;
                work->duration34 = FX_SPEAR_WAIT_FRAMES;
                work->frame38 = 0;
            } else {
                work->frame38++;
            }
            break;
        case FX_SPEAR_STATE_WAIT:
            if (work->frame38 == work->duration34) {
                return 1;
            }
            work->frame38++;
            break;
        }
        return 0;
    }
    return 0;
}

static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_184_env);
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

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_184_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

INCLUDE_RODATA("asm/us/fx_184/nonmatchings/fx_184", fx_184_headerPtr);
