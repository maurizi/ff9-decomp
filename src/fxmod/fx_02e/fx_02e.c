#include "fxmod/fx_02e.h"

s32 fx_02e_op80(s32 mode, Fx02eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        D_801E7A64 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk18);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk10);
        work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk10);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x24) {
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk18);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk18);
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x80);
            }
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (frame >= 0x24) {
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->unk18, (s16 *)&work->unk20);
            if (work->unk8 != NULL) {
                if (work->unk8->state == -1) {
                    work->unk8 = NULL;
                }
                if (work->unk8 != NULL) {
                    work->unk8->unk5C.vx = work->unk20.vx;
                    work->unk8->unk5C.vy = work->unk20.vy;
                    work->unk8->unk5C.vz = work->unk20.vz;
                }
            }
            if (work->unkC != NULL) {
                if (work->unkC->state == -1) {
                    work->unkC = NULL;
                }
                if (work->unkC != NULL) {
                    work->unkC->unk5C.vx = work->unk20.vx;
                    work->unkC->unk5C.vy = work->unk20.vy;
                    work->unkC->unk5C.vz = work->unk20.vz;
                }
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02e/nonmatchings/fx_02e", fx_02e_headerPtr);
