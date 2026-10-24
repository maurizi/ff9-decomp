#include "fxmod/fx_02c.h"

s32 fx_02c_op80(s32 mode, Fx02cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        D_801E7928 = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        work->unk10.vx = env->targets->centerX;
        work->unk10.vy = env->targets->centerY;
        work->unk10.vz = env->targets->centerZ;
        work->unk10.vy -= 0xC8;
        ex->btlfx_aimAngles((s16 *)&work->unk8, (s16 *)&work->unk10, (s16 *)&work->unk18);
        work->unk0 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk8);
    } else {
        frame = entry->unk0;
        if (frame == 8) {
            work->unk4 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk10);
            if (work->unk4 != NULL) {
                work->unk4->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        if (work->unk0 != NULL) {
            if (work->unk0->unk30 == -1) {
                work->unk0 = NULL;
            }
            if (work->unk0 != NULL) {
                work->unk0->unk5C.vx = work->unk18.vx;
                work->unk0->unk5C.vy = work->unk18.vy;
                work->unk0->unk5C.vz = work->unk18.vz;
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02c/nonmatchings/fx_02c", fx_02c_headerPtr);
