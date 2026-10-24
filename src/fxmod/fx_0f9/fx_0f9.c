#include "fxmod/fx_0f9.h"

s32 fx_0f9_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        work->vx = env->targets->centerX;
        work->vy = 0;
        work->vz = env->targets->centerZ;
        D_801E7820 = env;
        p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->unk8, 1), (s32)work);
        if (p != NULL) {
            p->unk22 = ex->func_800E7CC4(0x10, 0x80);
        }
    } else {
        return entry->unk0 >= 0x40;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f9/nonmatchings/fx_0f9", fx_0f9_headerPtr);
