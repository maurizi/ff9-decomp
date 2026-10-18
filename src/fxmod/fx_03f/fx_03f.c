#include "fxmod/fx_03f.h"

s32 fx_03f_op80(s32 mode, Fx03fWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        D_801E7A58 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk20);
        work->unk18.vx = work->unk20.vx;
        work->unk18.vy = work->unk20.vy;
        work->unk18.vz = work->unk20.vz;
        work->unk28.vx = work->unk20.vx;
        work->unk28.vy = work->unk20.vy;
        work->unk28.vz = work->unk20.vz;
        work->unk18.vy -= 0xFA;
        work->unk28.vy += 0xFA;
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk18);
        work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk28);
        work->unk8 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk20);
        if (work->unk0 != NULL) {
            work->unk0->unk22 = ex->func_800E7CC4(0, 0x20);
        }
        if (work->unk4 != NULL) {
            work->unk4->unk22 = ex->func_800E7CC4(0, 0x20);
        }
        if (work->unk8 != NULL) {
            work->unk8->unk22 = ex->func_800E7CC4(0, 0xC8);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0xA) {
            work->unkC = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk18);
            work->unk10 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk28);
            if (work->unkC != NULL) {
                work->unkC->unk22 = ex->func_800E7CC4(0, 0x20);
            }
            if (work->unk10 != NULL) {
                work->unk10->unk22 = ex->func_800E7CC4(0, 0x20);
            }
        }
        if (frame == 0x14) {
            work->unk14 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk20);
        }
        if (frame == 0xB) {
            ex->func_800E3D54(work->unk0);
            ex->func_800E3D54(work->unk4);
        }
        return frame >= 0x2D;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03f/nonmatchings/fx_03f", fx_03f_headerPtr);
