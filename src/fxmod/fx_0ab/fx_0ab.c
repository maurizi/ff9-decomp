#include "fxmod/fx_0ab.h"

s32 fx_0ab_op80(s32 mode, Fx0abWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E7918 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk10);
        work->unk18.vx = work->unk10.vx;
        work->unk18.vy = work->unk10.vy;
        work->unk18.vz = work->unk10.vz;
        work->unk20.vx = work->unk10.vx;
        work->unk20.vy = work->unk10.vy;
        work->unk20.vz = work->unk10.vz;
        work->unk18.vy -= 0xC8;
        work->unk20.vy -= 0x190;
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7928;
            work->unk0->unk12 = 0x11;
            work->unk0->unkC = 0x60;
        }
        work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk10);
    } else {
        frame = entry->frame;
        if (frame == 0xC) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk18);
        }
        if (frame == 0x10) {
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk20);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ab/nonmatchings/fx_0ab", fx_0ab_headerPtr);
