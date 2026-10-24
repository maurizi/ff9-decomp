#include "fxmod/fx_0ac.h"

s32 fx_0ac_op80(s32 mode, Fx0acWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        D_801E7A28 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk18);
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 2:
            ex->btlfx_getUnitArgBoneMatrix(0x10, 0, &bone);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone, -0x32, (s32)&work->unk20);
            work->unk20.vy -= 0x96;
            work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)&work->unk20);
            if (work->unk0 != NULL) {
                work->unk0->unk14 = (s32)D_801E7A38;
                work->unk0->unk12 = 0x3B;
                work->unk0->unkC = 0xBC;
            }
            ex->btlfx_aimAngles((s16 *)&work->unk20, (s16 *)&work->unk10, (s16 *)&work->unk28);
            break;
        case 8:
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 3), (s32)&work->unk18);
            work->unk8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 4), (s32)&work->unk18);
            work->unkC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 2), (s32)&work->unk18);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x20);
            }
            if (work->unk8 != NULL) {
                work->unk8->unk22 = ex->func_800E7CC4(0, 0x20);
            }
            break;
        }
        if (frame >= 2) {
            if (work->unk0 != NULL) {
                if (work->unk0->unk30 == -1) {
                    work->unk0 = NULL;
                }
                if (work->unk0 != NULL) {
                    work->unk0->unk5C.vx = work->unk28.vx;
                    work->unk0->unk5C.vy = work->unk28.vy;
                    work->unk0->unk5C.vz = work->unk28.vz;
                }
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ac/nonmatchings/fx_0ac", fx_0ac_headerPtr);
