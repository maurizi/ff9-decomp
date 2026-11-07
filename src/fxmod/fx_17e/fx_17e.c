#include "fxmod/fx_17e.h"

s32 fx_17e_op80(s32 mode, Fx17eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    SVECTOR lerpPos;
    s32 frame;
    s32 i;
    s32 fade;

    if (mode == 0) {
        *args = sizeof(Fx17eWork);
    } else if (mode == 1) {
        fx_17e_env = env;
        for (i = FX_CHESTNUT_TIMERS - 1; i >= 0; i--) {
            work->timer40[i] = 0;
        }
        ex->btlfx_initEffModels(fx_17e_effModels, 1);
        work->rot28.vx = 0;
        work->rot28.vy = 0;
        work->rot28.vz = 0;
        work->scale8.vx = 0x1000;
        work->scale8.vy = 0x1000;
        work->scale8.vz = 0x1000;
        work->model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_CHESTNUT_SPIKY_BURR), 0xFF);
        work->fade4 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos38);
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_CHESTNUT_APPEAR_FRAME:
            work->timer40[FX_CHESTNUT_T_SHOW] = FX_CHESTNUT_SHOW_FRAMES;
            work->timer50 = FX_CHESTNUT_FADE_IN_FRAMES;
            work->timer40[FX_CHESTNUT_T_HOLD] = FX_CHESTNUT_HOLD_FRAMES;
            break;
        case FX_CHESTNUT_LIFT_FRAME:
            work->fade4 = 0x80;
            work->timer40[FX_CHESTNUT_T_LIFT] = FX_CHESTNUT_LIFT_FRAMES;
            break;
        case FX_CHESTNUT_THROW_FRAME:
            work->timer40[FX_CHESTNUT_T_THROW] = FX_CHESTNUT_THROW_FRAMES;
            break;
        case FX_CHESTNUT_IMPACT_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CHESTNUT_IMPACT_RINGS), (s32)&work->pos38);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (work->timer40[FX_CHESTNUT_T_HOLD] != 0) {
            work->timer40[FX_CHESTNUT_T_HOLD]--;
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_CHESTNUT_HOLD_BONE, (u16 *)&work->pos30);
            work->pos18.vx = work->pos30.vx;
            work->pos18.vy = work->pos30.vy - 0x64;
            work->pos18.vz = work->pos30.vz;
        }
        if (work->timer40[FX_CHESTNUT_T_LIFT] != 0) {
            work->timer40[FX_CHESTNUT_T_LIFT]--;
            ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, FX_CHESTNUT_LIFT_BONE, (u16 *)&work->pos30);
            work->pos18.vx = work->pos30.vx;
            work->pos18.vy = work->pos30.vy;
            work->pos18.vz = work->pos30.vz;
        }
        if (work->timer40[FX_CHESTNUT_T_THROW] != 0) {
            work->timer40[FX_CHESTNUT_T_THROW]--;
            ex->btlfx_lerpVec((s16 *)&work->pos30, (s16 *)&work->pos38, (FX_CHESTNUT_THROW_FRAMES - work->timer40[FX_CHESTNUT_T_THROW]) * (0x1000 / FX_CHESTNUT_THROW_FRAMES), (s16 *)&lerpPos);
            work->pos18.vx = lerpPos.vx;
            work->pos18.vy = lerpPos.vy;
            work->pos18.vz = lerpPos.vz;
        }
        if (work->timer50 != 0) {
            work->timer50--;
            work->fade4 = 0x80 - work->timer50 * (0x80 / FX_CHESTNUT_FADE_IN_FRAMES);
        }
        if (work->timer40[FX_CHESTNUT_T_SHOW] != 0) {
            work->timer40[FX_CHESTNUT_T_SHOW]--;
            ex->Hi_DrawEffModel(&work->rot28, &work->pos18, (s32)&work->scale8, work->model0);
            if (work->fade4 != 0x80) {
                fade = work->fade4 - 0x80;
                ex->Hi_ModifyEffModelAbr(work->model0, BTLFX_ABR_ADD);
                ex->Hi_ModifyEffModelRGB(work->model0, fade, fade, fade);
            }
        }
        return frame >= FX_CHESTNUT_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_17e/nonmatchings/fx_17e", fx_17e_headerPtr);
