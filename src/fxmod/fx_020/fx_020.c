#include "fxmod/fx_020.h"

s32 fx_020_op80(s32 mode, Fx020Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        D_801E78E4 = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk8);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E78F4;
            work->unk0->unk12 = 0x14;
            work->unk0->unkC = 0x90;
        }
    } else {
        frame = entry->frame;
        if (frame == 0x14) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk8);
        }
        if (frame >= 0x14 && work->unk4 != NULL) {
            if (work->unk4->state == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->rot.vx = D_801E7894.vx;
                work->unk4->rot.vy = D_801E7894.vy;
                work->unk4->rot.vz = D_801E7894.vz;
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_020/nonmatchings/fx_020", fx_020_headerPtr);
