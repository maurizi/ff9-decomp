#include "fxmod/fx_100.h"

s32 fx_100_op80(s32 mode, Fx100Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x34;
    } else if (mode == 1) {
        fx_100_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos1C);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 4:
            ex->btlfx_getUnitBonePos(0x10, 0x22, (u16 *)&work->pos24);
            work->pos24.vy = 0;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos24);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0xC:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos1C);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_100_op80Trails;
                work->emit4->trailSlots = 9;
                work->emit4->trailStride = 0x90;
                if (work->emit4 != NULL) {
                    work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
            break;
        case 0xE:
            fxm_startHitMotionIfActive(0);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos1C);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos1C);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos1C);
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (frame >= 0xE) {
            if (fxm_isMotionDone(0)) {
                fxm_startUnitMotion(0, 0);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_100_op81(s32 mode, Fx100Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    s32 frame;

    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        fx_100_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_getUnitBoneMatrix(0x10, 0x11, (BtlBone *)&m);
            ex->btlfx_matrixPointAhead(&m, 0, (s32)&work->pos14);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos14);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)fx_100_op81Trails;
                work->emit0->trailSlots = 0x1A;
                work->emit0->trailStride = 0x60;
            }
            break;
        case 4:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->posC);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->posC);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        return frame >= 0x28;
    }
    return 0;
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_100_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_100_env);

    s32 done;

    if (ex->btlfx_isUnitActive(unit)) {
        done = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        done = 0;
    }
    return done;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_100_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_100_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}

INCLUDE_RODATA("asm/us/fx_100/nonmatchings/fx_100", fx_100_headerPtr);
