#include "fxmod/fx_024.h"

s32 fx_024_op80(s32 mode, Fx024Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
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
        *args = sizeof(Fx024Work80);
    } else if (mode == 1) {
        fx_024_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos18);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot20);
        work->pos38.vx = work->pos18.vx;
        work->pos38.vy = work->pos18.vy;
        work->pos38.vz = work->pos18.vz;
        ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&work->rot20, FX_AERIAL_SLASH_GUST_REACH, (s16 *)&work->pos38);
        work->pos38.vy = -FX_AERIAL_SLASH_GUST_HEIGHT;
        ex->btlfx_getUnitBoneMatrix(0x10, 0xD, (BtlBone *)&u.m);
        ex->btlfx_matrixPointAhead(&u.m, FX_AERIAL_SLASH_BLADE_REACH, (s32)&work->pos28);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x1D, (BtlBone *)&boneMat);
        ex->btlfx_matrixPointAhead(&boneMat, FX_AERIAL_SLASH_BLADE_REACH, (s32)&work->pos30);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_CRESCENT), (s32)&work->pos28);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_CRESCENT_2), (s32)&work->pos30);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x100);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos28, (s16 *)&work->pos38, (s16 *)&work->aim40);
        ex->btlfx_aimAngles((s16 *)&work->pos30, (s16 *)&work->pos38, (s16 *)&work->aim48);
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_AERIAL_SLASH_FLASH_FRAME + 1:
            work->tint50 = FX_AERIAL_SLASH_FLASH_LEVEL;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, work->tint50, work->tint50, FX_AERIAL_SLASH_FLASH_LEVEL);
            break;
        case FX_AERIAL_SLASH_FLASH_FRAME:
            work->tint50 = FX_AERIAL_SLASH_FLASH_LEVEL;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, work->tint50, work->tint50, FX_AERIAL_SLASH_FLASH_LEVEL);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_WIND_RINGS), (s32)&work->pos38);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_DUST_RING), (s32)&work->pos38);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_STAR_FLASH), (s32)&work->pos38);
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_AERIAL_SLASH_BLUE_GUST), (s32)&work->pos38);
            break;
        }
        if (frame >= FX_AERIAL_SLASH_GRID_FRAME) {
            s32 gridScale = (frame << 7) + 0x980;

            u.d.scale.vx = gridScale;
            u.d.scale.vy = gridScale;
            u.d.scale.vz = gridScale;
            u.d.col.b = 0x8;
            u.d.col.g = 0x8;
            u.d.col.r = 0x8;
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
                work->emit0->rot.vx = work->aim40.vx;
                work->emit0->rot.vy = work->aim40.vy;
                work->emit0->rot.vz = work->aim40.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim48.vx;
                work->emit4->rot.vy = work->aim48.vy;
                work->emit4->rot.vz = work->aim48.vz;
            }
        }
        return frame >= FX_AERIAL_SLASH_FRAMES;
    }
    return 0;
}

s32 fx_024_op81(s32 mode, Fx024Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx024Work81);
    } else if (mode == 1) {
        s32 target = *args;

        work->targetC = target;
        fx_024_env = env;
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

INCLUDE_RODATA("asm/us/fx_024/nonmatchings/fx_024", fx_024_headerPtr);
