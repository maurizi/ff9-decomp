#include "fxmod/fx_091.h"

s32 fx_091_op80(s32 mode, Fx091Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx091Work);
    } else if (mode == 1) {
        work->bgFrame0 = 0;
        fx_091_env = env;
        ex->btlfx_getUnitArgBonePos(BTLFX_UNIT_CASTER, 0, (u16 *)&work->pos8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_ENTICE_VIOLET_HALO), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0xC8);
        }
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->aim18);
    } else {
        frame = entry->frame;
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim18.vx;
                work->emit4->rot.vy = work->aim18.vy;
                work->emit4->rot.vz = work->aim18.vz;
            }
        }
        ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_ENTICE_BACKDROP), work->bgFrame0);
        work->bgFrame0++;
        return frame >= FX_ENTICE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_091/nonmatchings/fx_091", fx_091_headerPtr);
