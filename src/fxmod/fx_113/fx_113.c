#include "fxmod/fx_113.h"

s32 fx_113_op80(s32 mode, Fx113Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        work->unk18 = *arg4;
        D_801E7A74 = env;
        if (work->unk18 >= 3) {
            return 1;
        }
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->unk10.vx = work->unk8.vx;
        work->unk10.vy = work->unk8.vy;
        work->unk10.vz = work->unk8.vz;
        switch (work->unk18) {
        case 0:
            work->unk10.vx += 0x64;
            work->unk10.vy -= 0x190;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk10);
            break;
        case 1:
            work->unk10.vx -= 0x32;
            work->unk10.vy -= 0x32;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk10);
            break;
        case 2:
            work->unk10.vy -= 0xC8;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk10);
            break;
        }
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        return 0;
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_113_op81(s32 mode, Fx113Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0xC;
    } else if (mode == 1) {
        D_801E7A74 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk4);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk4);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7A84;
            work->unk0->unk12 = 0xD;
            work->unk0->unkC = 0x50;
            if (work->unk0 != NULL) {
                work->unk0->unk22 = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return 0;
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_113/nonmatchings/fx_113", fx_113_headerPtr);
