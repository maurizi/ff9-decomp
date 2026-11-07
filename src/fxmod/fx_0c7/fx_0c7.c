#include "fxmod/fx_0c7.h"

s32 fx_0c7_op80(s32 mode, Fx0c7Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx0c7Work);
    } else if (mode == 1) {
        fx_0c7_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_FOCUS_GLOW_RING), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
    } else {
        return entry->frame >= FX_FOCUS_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c7/nonmatchings/fx_0c7", fx_0c7_headerPtr);
