#include "fxmod/fx_01d.h"

s32 fx_01d_op80(s32 mode, Fx01dWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    union { /* one stack area: bone matrix in mode 1, draw arguments afterwards */
        MATRIX m;
        struct {
            VECTOR scale;
            SVECTOR rot;
            CVECTOR col;
        } d;
    } u;
    MATRIX boneMat;

    if (mode == 0) {
        *args = sizeof(Fx01dWork80);
    } else if (mode == 1) {
        fx_01d_env = env;
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->pos14);
        ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->rot1C);
        work->pos34.vx = work->pos14.vx;
        work->pos34.vy = work->pos14.vy;
        work->pos34.vz = work->pos14.vz;
        ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&work->rot1C, FX_AERIAL_SLASH_GUST_REACH, (s16 *)&work->pos34);
        work->pos34.vy = -FX_AERIAL_SLASH_GUST_HEIGHT;
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, 7, (BtlBone *)&u.m);
        ex->btlfx_matrixPointAhead(&u.m, FX_AERIAL_SLASH_BLADE_REACH, (s32)&work->pos24);
        ex->btlfx_getUnitBoneMatrix(BTLFX_UNIT_CASTER, 0xF, (BtlBone *)&boneMat);
        ex->btlfx_matrixPointAhead(&boneMat, FX_AERIAL_SLASH_BLADE_REACH, (s32)&work->pos2C);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_CRESCENT), (s32)&work->pos24);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_CRESCENT_2), (s32)&work->pos2C);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x100);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x100);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos24, (s16 *)&work->pos34, (s16 *)&work->aim3C);
        ex->btlfx_aimAngles((s16 *)&work->pos2C, (s16 *)&work->pos34, (s16 *)&work->aim44);
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_AERIAL_SLASH_FLASH_FRAME + 1:
            work->tint4C = FX_AERIAL_SLASH_FLASH_LEVEL;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, work->tint4C, work->tint4C, FX_AERIAL_SLASH_FLASH_LEVEL);
            break;
        case FX_AERIAL_SLASH_FLASH_FRAME:
            work->tint4C = FX_AERIAL_SLASH_FLASH_LEVEL;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, work->tint4C, work->tint4C, FX_AERIAL_SLASH_FLASH_LEVEL);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_WIND_RINGS), (s32)&work->pos34);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_DUST_RING), (s32)&work->pos34);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_BLUE_GUST), (s32)&work->pos34);
            break;
        }
        if (frame >= FX_AERIAL_SLASH_GRID_FRAME) {
            s32 gridScale = (frame << 7) + 0x980;

            u.d.scale.vx = gridScale;
            u.d.scale.vy = gridScale;
            u.d.scale.vz = gridScale;
            u.d.col.b = 0x28;
            u.d.col.g = 0x28;
            u.d.col.r = 0x28;
            u.d.rot.vx = 0;
            u.d.rot.vy = 0;
            u.d.rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &u.d.rot, &u.d.scale, &u.d.col, 1, NULL);
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim3C.vx;
                work->emit0->rot.vy = work->aim3C.vy;
                work->emit0->rot.vz = work->aim3C.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim44.vx;
                work->emit4->rot.vy = work->aim44.vy;
                work->emit4->rot.vz = work->aim44.vz;
            }
        }
        return frame >= FX_AERIAL_SLASH_FRAMES;
    }
    return 0;
}

s32 fx_01d_op81(s32 mode, Fx01dWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx01dWork81);
    } else if (mode == 1) {
        s32 target = *args;

        work->targetC = target;
        fx_01d_env = env;
        if (target >= env->targets->targetCount) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
            return 1;
        }
        fxm_setSignal(env, BTLFX_SIG_WAIT, 0);
        ex->btlfx_getUnitPos(work->targetC, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_DUST_CLOUD), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->targetC, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 1) {
            fxm_setSignal(env, BTLFX_SIG_WAIT, 1);
        }
        return frame >= FX_AERIAL_SLASH_DUST_FRAMES;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

INCLUDE_RODATA("asm/us/fx_01d/nonmatchings/fx_01d", fx_01d_headerPtr);
