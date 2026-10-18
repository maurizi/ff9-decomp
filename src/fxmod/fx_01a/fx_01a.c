#include "fxmod/fx_01a.h"

s32 fx_01a_op80(s32 mode, Fx01aWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x14;
    } else if (mode == 1) {
        D_801E7910 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unkC);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unkC);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7920;
            work->unk0->unk12 = 0xE;
            work->unk0->unkC = 0x50;
        }
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0x13:
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unkC);
            if (work->unk4 != NULL) {
                work->unk4->unk14 = (s32)D_801E7D80;
                work->unk4->unk12 = 0x1E;
                work->unk4->unkC = 0x50;
            }
            break;
        case 0x27:
            work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unkC);
            break;
        }
        if (frame >= 0x13) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->unk8, 1), frame - 0x13);
        }
        return frame >= 0x50;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01a/nonmatchings/fx_01a", fx_01a_headerPtr);
