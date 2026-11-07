#include "fxmod/fx_1b9.h"

s32 fx_1b9_op80(s32 mode, Fx1b9Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1b9Work);
    } else if (mode == 1) {
        fx_1b9_env = env;
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->pos10);
        work->pos18.vx = work->pos10.vx;
        work->pos18.vy = work->pos10.vy;
        work->pos18.vz = work->pos10.vz;
        work->pos20.vx = work->pos10.vx;
        work->pos20.vy = work->pos10.vy;
        work->pos20.vz = work->pos10.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_APPEARANCE_TEAL_GLOWS), (s32)&work->pos20);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_APPEARANCE_WATER_SPOUT), (s32)&work->pos20);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_APPEARANCE_WATER_WAVE), (s32)&work->pos18);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x20);
            }
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x20);
            }
            break;
        case FX_APPEARANCE_RIPPLES_FRAME:
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_APPEARANCE_RIPPLES), (s32)&work->pos18);
            break;
        }
        return frame >= FX_APPEARANCE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1b9/nonmatchings/fx_1b9", fx_1b9_headerPtr);
