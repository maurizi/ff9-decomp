#include "fxmod/fx_0a8.h"

s32 fx_0a8_op80(s32 mode, Fx0a8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E7B68 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk8);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        frame = entry->unk0;
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk10.vx;
                work->unk0->unk5C.vy = work->unk10.vy;
                work->unk0->unk5C.vz = work->unk10.vz;
            }
        }
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk10.vx;
                work->unk4->unk5C.vy = work->unk10.vy;
                work->unk4->unk5C.vz = work->unk10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0a8_op81(s32 mode, Fx0a8Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk20.vx = env->unkC->unk38;
        work->unk20.vy = env->unkC->unk3A;
        work->unk20.vz = env->unkC->unk3C;
        D_801E7B68 = env;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk20);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk20);
        work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xC), (s32)&work->unk20);
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E7B78;
            work->unk4->unk12 = 6;
            work->unk4->unkC = 0x60;
        }
    } else {
        frame = entry->unk0;
        if (frame == 8) {
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk20);
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk20);
            work->unk18 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk20);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a8/nonmatchings/fx_0a8", fx_0a8_headerPtr);
