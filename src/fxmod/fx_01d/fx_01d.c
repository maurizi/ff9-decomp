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
        *args = 0x50;
    } else if (mode == 1) {
        fx_01d_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos14);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot1C);
        work->pos34.vx = work->pos14.vx;
        work->pos34.vy = work->pos14.vy;
        work->pos34.vz = work->pos14.vz;
        ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&work->rot1C, 0x5DC, (s16 *)&work->pos34);
        work->pos34.vy = -0xC8;
        ex->btlfx_getUnitBoneMatrix(0x10, 7, (BtlBone *)&u.m);
        ex->btlfx_matrixPointAhead(&u.m, 0x190, (s32)&work->pos24);
        ex->btlfx_getUnitBoneMatrix(0x10, 0xF, (BtlBone *)&boneMat);
        ex->btlfx_matrixPointAhead(&boneMat, 0x190, (s32)&work->pos2C);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_FAN_EMITTER), (s32)&work->pos24);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_FAN_EMITTER_2), (s32)&work->pos2C);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos24, (s16 *)&work->pos34, (s16 *)&work->aim3C);
        ex->btlfx_aimAngles((s16 *)&work->pos2C, (s16 *)&work->pos34, (s16 *)&work->aim44);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 11:
            work->tint4C = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->tint4C, work->tint4C, 0x80);
            break;
        case 10:
            work->tint4C = 0x80;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, work->tint4C, work->tint4C, 0x80);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_WIND_RINGS), (s32)&work->pos34);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_DUST_RING), (s32)&work->pos34);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_FAN_EMITTER_3), (s32)&work->pos34);
            break;
        }
        if (frame >= 0xD) {
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
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_01d_op81(s32 mode, Fx01dWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x10;
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
        return frame >= 0x28;
    }
    return 0;
}

static void fxm_setSignal(Unk800F78A4 *env, s32 signal, s32 value) {
    env->state->signals[signal] = value;
}

INCLUDE_RODATA("asm/us/fx_01d/nonmatchings/fx_01d", fx_01d_headerPtr);
