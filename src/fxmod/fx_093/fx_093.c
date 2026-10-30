#include "fxmod/fx_093.h"

s32 fx_093_op80(s32 mode, Fx093Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone;
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x2C;
    } else if (mode == 1) {
        D_801E79F4 = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk0);
        ex->btlfx_getUnitBoneMatrix(0x10, env->targets->fxArgs[0], &bone);
        ex->btlfx_matrixPointAhead((MATRIX *)&bone, 0x100, (s32)&work->unk8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk18);
        ex->btlfx_aimAngles((s16 *)&work->unk0, (s16 *)&work->unk8, (s16 *)&work->unk20);
        work->unk28 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->unk0);
        if (work->unk28 != NULL) {
            work->unk28->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x40);
        }
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk0);
        if (p != NULL) {
            p->unk22 = ex->btlfx_addRadiusSixth(0x10, 0x40);
        }
    } else {
        frame = entry->frame;
        if (work->unk28 != NULL) {
            if (work->unk28->state == -1) {
                work->unk28 = NULL;
            }
            if (work->unk28 != NULL) {
                work->unk28->unk5C.vx = work->unk20.vx;
                work->unk28->unk5C.vy = work->unk20.vy;
                work->unk28->unk5C.vz = work->unk20.vz;
            }
        }
        if (frame == 8) {
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->unk10);
            if (p != NULL) {
                p->unk22 = ex->btlfx_addRadiusSixth(0, 0x40);
            }
            p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk18);
            if (p != NULL) {
                p->unk22 = ex->btlfx_addRadiusSixth(0, -0x40);
            }
        }
        return frame >= 0x43;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_093/nonmatchings/fx_093", fx_093_headerPtr);
