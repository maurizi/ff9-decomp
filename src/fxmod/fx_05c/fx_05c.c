#include "fxmod/fx_05c.h"

s32 fx_05c_op80(s32 mode, Fx05cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx05cWork);
    } else if (mode == 1) {
        fx_05c_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARMOR_BREAK_BLUE_ORB), (s32)&work->posC);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARMOR_BREAK_SPARKLES), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(BTLFX_UNIT_CASTER, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_ARMOR_BREAK_TRIANGLE_FRAME) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ARMOR_BREAK_BLUE_TRIANGLE), (s32)&work->pos14);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame >= FX_ARMOR_BREAK_FRAMES) {
            ex->btlfx_stopEmitter(work->emit8);
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_05c/nonmatchings/fx_05c", fx_05c_headerPtr);
