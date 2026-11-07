#include "fxmod/fx_0d1.h"

s32 fx_0d1_op80(s32 mode, Fx0d1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0d1Work);
    } else if (mode == 1) {
        fx_0d1_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos1C);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MENTAL_BREAK_ENERGY_ORB), (s32)&work->pos14);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MENTAL_BREAK_SPARKLES), (s32)&work->pos14);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_MENTAL_BREAK_GLOW_FRAME) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MENTAL_BREAK_PINK_GLOW), (s32)&work->pos1C);
            if (work->emit8 != NULL) {
                work->emit8->otZ = 0x100;
            }
        }
        if (frame == FX_MENTAL_BREAK_CROSS_FRAME) {
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MENTAL_BREAK_GLOW_CROSS), (s32)&work->pos1C);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MENTAL_BREAK_RINGS), (s32)&work->pos1C);
            if (work->emitC != NULL) {
                work->emitC->otZ = 0x100;
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = 0x100;
            }
        }
        return frame >= FX_MENTAL_BREAK_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0d1/nonmatchings/fx_0d1", fx_0d1_headerPtr);
