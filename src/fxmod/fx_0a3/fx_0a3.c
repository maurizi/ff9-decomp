#include "fxmod/fx_0a3.h"

s32 fx_0a3_op80(s32 mode, Fx0a3Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E791C = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->unk10);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk18);
        ex->btlfx_pointAhead((s16 *)&work->unk8, (s16 *)&work->unk18, 0x190, (s16 *)&work->unk8);
        ex->btlfx_pointAhead((s16 *)&work->unk10, (s16 *)&work->unk18, 0xC8, (s16 *)&work->unk10);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E792C;
            work->unk0->unk12 = 0x2C;
            work->unk0->unkC = 0x28;
        }
    } else {
        frame = entry->unk0;
        if (frame == 0xA) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E800C;
                work->unk4->unk12 = 0x14;
                work->unk4->unkC = 0x28;
            }
        }
        return frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a3/nonmatchings/fx_0a3", fx_0a3_headerPtr);
