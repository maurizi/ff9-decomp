#include "fxmod/fx_1b8.h"

s32 fx_1b8_op80(s32 mode, Fx1b8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7A24 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk4);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        return entry->unk0 >= 0x3C;
    }
    return 0;
}

s32 fx_1b8_op81(s32 mode, Fx1b8Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E7A24 = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk10);
        work->unk18.vx = work->unk10.vx;
        work->unk18.vy = work->unk10.vy;
        work->unk18.vz = work->unk10.vz;
        work->unk20.vx = work->unk10.vx;
        work->unk20.vy = work->unk10.vy;
        work->unk20.vz = work->unk10.vz;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 8:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk20);
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk20);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk18);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x20);
            }
            break;
        case 10:
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk18);
            break;
        }
        return frame >= 0x5A;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b8/nonmatchings/fx_1b8", fx_1b8_headerPtr);
