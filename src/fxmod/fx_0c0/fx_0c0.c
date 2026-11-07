#include "fxmod/fx_0c0.h"

s32 fx_0c0_op80(s32 mode, Fx0c0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0c0Work);
    } else if (mode == 1) {
        fx_0c0_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHOCK_ENERGY_ORB), (s32)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHOCK_SPARKLES), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_SHOCK_FLAME_RING_FRAME) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHOCK_FLAME_RING), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_SHOCK_SPARKLES_2), (s32)&work->pos18);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        return frame >= FX_SHOCK_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c0/nonmatchings/fx_0c0", fx_0c0_headerPtr);
