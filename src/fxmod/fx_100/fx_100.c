#include "fxmod/fx_100.h"

s32 fx_100_op80(s32 mode, Fx100Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x34;
    } else if (mode == 1) {
        D_801E7D5C = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk1C);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 4:
            ex->btlfx_getUnitBonePos(0x10, 0x22, (u16 *)&work->unk24);
            work->unk24.vy = 0;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk24);
            if (work->unk0 != NULL) {
                work->unk0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0xC:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk1C);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E7D6C;
                work->unk4->unk12 = 9;
                work->unk4->unkC = 0x90;
                if (work->unk4 != NULL) {
                    work->unk4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
                }
            }
            break;
        case 0xE:
            func_801E7C7C(0);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk1C);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk1C);
            work->unk10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk1C);
            if (work->unkC != NULL) {
                work->unkC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        if (frame >= 0xE) {
            if (func_801E7BF8(0)) {
                func_801E7B8C(0, 0);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_100_op81(s32 mode, Fx100Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        D_801E7D5C = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkC);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_getUnitBoneMatrix(0x10, 0x11, (BtlBone *)&m);
            ex->btlfx_matrixPointAhead(&m, 0, (s32)&work->unk14);
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk14);
            if (work->unk0 != NULL) {
                work->unk0->unk14 = (s32)D_801E827C;
                work->unk0->unk12 = 0x1A;
                work->unk0->unkC = 0x60;
            }
            break;
        case 4:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unkC);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unkC);
            if (work->unk4 != NULL) {
                work->unk4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            break;
        }
        return frame >= 0x28;
    }
    return 0;
}

void func_801E7B8C(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7D5C);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

s32 func_801E7BF8(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(D_801E7D5C);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

void func_801E7C7C(s32 unit) {
    if (BTLFX_EXPORTS(D_801E7D5C)->btlfx_isUnitActive(unit)) {
        func_801E7B8C(unit, 2);
    }
}

void func_801E7CC8(s32 unit) {
    if (BTLFX_EXPORTS(D_801E7D5C)->btlfx_isUnitActive(unit)) {
        func_801E7B8C(unit, 3);
    }
}

INCLUDE_RODATA("asm/us/fx_100/nonmatchings/fx_100", fx_100_headerPtr);
