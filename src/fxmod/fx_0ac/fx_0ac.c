#include "fxmod/fx_0ac.h"

s32 fx_0ac_op80(s32 mode, Fx0acWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x30;
    } else if (mode == 1) {
        fx_0ac_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk18);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 2:
            ex->btlfx_getUnitArgBoneMatrix(0x10, 0, &bone);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone, -0x32, (s32)&work->unk20);
            work->unk20.vy -= 0x96;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk20);
            if (work->emit0 != NULL) {
                work->emit0->unk14 = (s32)D_801E7A38;
                work->emit0->unk12 = 0x3B;
                work->emit0->unkC = 0xBC;
            }
            ex->btlfx_aimAngles((s16 *)&work->unk20, (s16 *)&work->unk10, (s16 *)&work->unk28);
            break;
        case 8:
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk18);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk18);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            break;
        }
        if (frame >= 2) {
            if (work->emit0 != NULL) {
                if (work->emit0->state == BTLFX_EMIT_DONE) {
                    work->emit0 = NULL;
                }
                if (work->emit0 != NULL) {
                    work->emit0->rot.vx = work->unk28.vx;
                    work->emit0->rot.vy = work->unk28.vy;
                    work->emit0->rot.vz = work->unk28.vz;
                }
            }
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0ac/nonmatchings/fx_0ac", fx_0ac_headerPtr);
