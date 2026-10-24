#include "fxmod/fx_0fc.h"

s32 fx_0fc_op80(s32 mode, SVECTOR *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 8;
    } else if (mode == 1) {
        D_801E77F4 = env;
        ex->btlfx_getUnitHomePos(0, (s16 *)work);
        return 0;
    } else {
        frame = entry->unk0;
        work->vz += 0xC0;
        ex->btlfx_setUnitPos(0, (s16 *)work);
        if (frame >= 0x12) {
            ex->btlfx_fadeUnitById(0, -1);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0fc/nonmatchings/fx_0fc", fx_0fc_headerPtr);
