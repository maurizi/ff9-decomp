#include "fxmod/fx_017.h"

s32 fx_017_op80(s32 mode, Fx017Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        D_801E78CC = env;
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk14);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unkC);
        work->unk1C.vx = work->unkC.vx;
        work->unk1C.vy = work->unkC.vy;
        work->unk1C.vz = work->unkC.vz;
        work->unk1C.vy = 0;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk14);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x10);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0x14) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk1C);
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unkC);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_017/nonmatchings/fx_017", fx_017_headerPtr);
