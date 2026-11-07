#include "fxmod/fx_03c.h"

s32 fx_03c_op80(s32 mode, Fx03cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = sizeof(Fx03cWork);
    } else if (mode == 1) {
        fx_03c_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos4);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_WIND_WIND_TRAILS), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_03c_trails;
            work->emit0->trailSlots = FX_WIND_WIND_TRAILS_TRAIL_SLOTS;
            work->emit0->trailStride = FX_WIND_WIND_TRAILS_TRAIL_STRIDE;
        }
    } else {
        return entry->frame >= FX_WIND_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_03c/nonmatchings/fx_03c", fx_03c_headerPtr);
