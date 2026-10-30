#include "fxmod/fx_015.h"

s32 fx_015_op80(s32 mode, Fx015Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E78E0 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        work->unk8.vx = work->unk10.vx;
        work->unk8.vy = work->unk10.vy;
        work->unk8.vz = work->unk10.vz;
        work->unk8.vy -= 0x190;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E78F0;
            work->unk0->unk12 = 0x1E;
            work->unk0->unkC = 0x30;
        }
        if (work->unk0 != NULL) {
            work->unk0->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x19) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk10);
            if (work->unk0 != NULL) {
                work->unk0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_015/nonmatchings/fx_015", fx_015_headerPtr);
