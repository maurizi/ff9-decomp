#include "fxmod/fx_02c.h"

s32 fx_02c_op80(s32 mode, Fx02cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx02cWork);
    } else if (mode == 1) {
        fx_02c_env = env;
        ex->btlfx_getUnitBodyPos(BTLFX_UNIT_CASTER, 0, (s16 *)&work->pos8);
        work->pos10.vx = env->targets->centerX;
        work->pos10.vy = env->targets->centerY;
        work->pos10.vz = env->targets->centerZ;
        work->pos10.vy -= FX_BAD_BREATH_AIM_HEIGHT;
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->aim18);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAD_BREATH_GAS_CLOUD), (s32)&work->pos8);
    } else {
        frame = entry->frame;
        if (frame == FX_BAD_BREATH_GAS_BURST_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_BAD_BREATH_GAS_BURST), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->aim18.vx;
                work->emit0->rot.vy = work->aim18.vy;
                work->emit0->rot.vz = work->aim18.vz;
            }
        }
        return frame >= FX_BAD_BREATH_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02c/nonmatchings/fx_02c", fx_02c_headerPtr);
