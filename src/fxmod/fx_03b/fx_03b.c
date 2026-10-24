#include "fxmod/fx_03b.h"

s32 fx_03b_op80(s32 mode, Fx03bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        D_801E7970 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        work->unk18.vx = work->unk10.vx;
        work->unk18.vy = work->unk10.vy;
        work->unk18.vz = work->unk10.vz;
        work->unk18.vy -= 0xB4;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->unk20);
        ex->func_800DC858(0x10, 0, (u16 *)&work->unk28);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk28);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        frame = entry->unk0;
        if (frame == 5) {
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 0xA), (s32)&work->unk20);
            if (work->unkC != NULL) {
                work->unkC->unk14 = (s32)D_801E7E80;
                work->unkC->unk12 = 0x28;
                work->unkC->unkC = 0x18;
            }
        }
        if (frame == 8) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk10);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk18);
            if (work->unk8 != NULL) {
                work->unk8->unk14 = (s32)D_801E7980;
                work->unk8->unk12 = 0x28;
                work->unk8->unkC = 0x20;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03b/nonmatchings/fx_03b", fx_03b_headerPtr);
