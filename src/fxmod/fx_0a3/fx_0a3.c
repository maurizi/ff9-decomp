#include "fxmod/fx_0a3.h"

s32 fx_0a3_op80(s32 mode, Fx0a3Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0a3Work);
    } else if (mode == 1) {
        fx_0a3_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->pos10);
        ex->btlfx_getUnitRot(BTLFX_UNIT_CASTER, (s16 *)&work->rot18);
        ex->btlfx_pointAhead((s16 *)&work->pos8, (s16 *)&work->rot18, 0x190, (s16 *)&work->pos8);
        ex->btlfx_pointAhead((s16 *)&work->pos10, (s16 *)&work->rot18, FX_ELECTROCUTE_REACH, (s16 *)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ELECTROCUTE_BLUE_BOLTS), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_0a3_trails;
            work->emit0->trailSlots = FX_ELECTROCUTE_BLUE_BOLTS_TRAIL_SLOTS;
            work->emit0->trailStride = FX_ELECTROCUTE_BLUE_BOLTS_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        if (frame == FX_ELECTROCUTE_BOLTS_2_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ELECTROCUTE_BLUE_BOLTS_2), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_0a3_trails2;
                work->emit4->trailSlots = FX_ELECTROCUTE_BLUE_BOLTS_2_TRAIL_SLOTS;
                work->emit4->trailStride = FX_ELECTROCUTE_BLUE_BOLTS_2_TRAIL_STRIDE;
            }
        }
        return frame >= FX_ELECTROCUTE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a3/nonmatchings/fx_0a3", fx_0a3_headerPtr);
