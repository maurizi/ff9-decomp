#include "fxmod/fx_135.h"

s32 fx_135_op80(s32 mode, Fx135Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk8.vx = env->targets->centerX;
        work->unk8.vy = env->targets->centerY;
        work->unk8.vz = env->targets->centerZ;
        work->unk8.vy -= 0xC8;
        D_801E7918 = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->unk10);
        ex->btlfx_aimAngles((s16 *)&work->unk10, (s16 *)&work->unk8, (s16 *)&work->unk18);
        work->unk0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk10);
    } else {
        frame = entry->frame;
        if (frame == 8) {
            work->unk4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->unk8);
            if (work->unk4 != NULL) {
                work->unk4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (work->unk0 != NULL) {
            if (work->unk0->state == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->rot.vx = work->unk18.vx;
                work->unk0->rot.vy = work->unk18.vy;
                work->unk0->rot.vz = work->unk18.vz;
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_135/nonmatchings/fx_135", fx_135_headerPtr);
