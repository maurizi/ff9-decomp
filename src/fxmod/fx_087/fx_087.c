#include "fxmod/fx_087.h"

s32 fx_087_op80(s32 mode, Fx087Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x34;
    } else if (mode == 1) {
        D_801E7958 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unkC);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk14);
        ex->func_800DC818(0x10, 0x17, (u16 *)&work->unk1C);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 6), (s32)&work->unk1C);
        if (work->unk4 != NULL) {
            work->unk4->unk14 = (s32)D_801E7968;
            work->unk4->unk12 = 0x19;
            work->unk4->unkC = 0x30;
        }
        ex->btlfx_aimAngles((s16 *)&work->unkC, (s16 *)&work->unk14, (s16 *)&work->unk2C);
    } else {
        frame = entry->unk0;
        if (work->unk4 != NULL) {
            if (work->unk4->unk30 == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = work->unk2C.vx;
                work->unk4->unk5C.vy = work->unk2C.vy;
                work->unk4->unk5C.vz = work->unk2C.vz;
            }
        }
        if (frame == 8) {
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk14);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_087/nonmatchings/fx_087", fx_087_headerPtr);
