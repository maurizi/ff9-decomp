#include "fxmod/fx_07b.h"

s32 fx_07b_op80(s32 mode, Fx07bWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        D_801E7A18 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk8);
        if (work->unk0 != NULL) {
            work->unk0->unk14 = (s32)D_801E7A28;
            work->unk0->unk12 = 6;
            work->unk0->unkC = 0xA0;
        }
        ex->btlfx_getUnitRot(0, (s16 *)&work->unk10);
        D_801E79C8.vy = work->unk10.vy;
        work->unk8.vx -= ex->math_sin(work->unk10.vy) * 0x190 / 4096;
        work->unk8.vz -= ex->math_cos(work->unk10.vy) * 0x190 / 4096;
    } else {
        frame = entry->frame;
        if (frame == 0xE) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk8);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = D_801E79C8.vx;
                work->unk0->unk5C.vy = D_801E79C8.vy;
                work->unk0->unk5C.vz = D_801E79C8.vz;
            }
        }
        if (frame >= 0xE && work->unk4 != NULL) {
            if (work->unk4->state == -1) {
                work->unk4 = NULL;
            }
            if (work->unk4 != NULL) {
                work->unk4->unk5C.vx = D_801E79C8.vx;
                work->unk4->unk5C.vy = D_801E79C8.vy;
                work->unk4->unk5C.vz = D_801E79C8.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_07b/nonmatchings/fx_07b", fx_07b_headerPtr);
