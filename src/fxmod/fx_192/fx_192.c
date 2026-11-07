#include "fxmod/fx_192.h"

s32 fx_192_op80(s32 mode, u16 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(SVECTOR);
    } else if (mode == 1) {
        fx_192_env = env;
        ex->btlfx_getUnitBonePos(BTLFX_UNIT_CASTER, 0, work);
        ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DEATH_GREEN_SMOKE), (s32)work);
    } else {
        return entry->frame >= FX_DEATH_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_192/nonmatchings/fx_192", fx_192_headerPtr);
