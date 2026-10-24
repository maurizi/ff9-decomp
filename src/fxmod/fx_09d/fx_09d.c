#include "fxmod/fx_09d.h"

s32 fx_09d_op80(s32 mode, Fx09dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        work->unk0.vx = env->targets->centerX;
        work->unk0.vy = -0x190;
        work->unk0.vz = env->targets->centerZ;
        D_801E7880 = env;
        p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 8), (s32)&work->unk0);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0, 0x80);
        }
    } else {
        frame = entry->unk0;
        if (frame == 0x18) {
            p = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 7), (s32)&work->unk0);
            if (p != NULL) {
                p->unk22 = ex->func_800E7CC4(0, 0x80);
            }
        }
        return frame >= 0x38;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09d/nonmatchings/fx_09d", fx_09d_headerPtr);
