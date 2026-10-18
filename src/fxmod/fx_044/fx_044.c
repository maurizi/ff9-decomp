#include "fxmod/fx_044.h"

s32 fx_044_op80(s32 mode, Fx044Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E7A64 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk8, (s16 *)&work->unk18);
    } else {
        frame = entry->unk0;
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk18.vx;
                work->unk4->unk5C.vy = work->unk18.vy;
                work->unk4->unk5C.vz = work->unk18.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_044_op81(s32 mode, Fx044Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E7A64 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk14);
        work->unk1C.vx = work->unkC.vx;
        work->unk1C.vy = work->unkC.vy;
        work->unk1C.vz = work->unkC.vz;
        work->unk1C.vy -= 0xC8;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk1C);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk14);
        work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 5), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7A74;
            work->unk0->unk12 = 0xD;
            work->unk0->unkC = 0x90;
        }
    } else {
        return entry->unk0 >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_044/nonmatchings/fx_044", fx_044_headerPtr);
