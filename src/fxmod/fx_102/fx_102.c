#include "fxmod/fx_102.h"

s32 fx_102_op80(s32 mode, Fx102Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = sizeof(Fx102Work);
    } else if (mode == 1) {
        fx_102_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos28);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RISE_RUNE_CIRCLE), (s32)&work->pos28);
        for (i = 0; i < FX_RISE_EMITTER_COUNT; i++) {
            (work->pos30 + i)->vx = work->pos28.vx;
            (work->pos30 + i)->vy = work->pos28.vy;
            (work->pos30 + i)->vz = work->pos28.vz;
            work->pos30[i].vx += ex->math_sin(i << 9) * FX_RISE_RING_RADIUS / 4096;
            work->pos30[i].vz += ex->math_cos(i << 9) * FX_RISE_RING_RADIUS / 4096;
            work->emit4[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_102_emitFileIds[i]), (s32)&work->pos30[i]);
        }
    } else {
        frame = entry->frame;
        if (frame == FX_RISE_GLOW_RING_FRAME) {
            work->emit24 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_RISE_GLOW_RING), (s32)&work->pos28);
        }
        return frame >= FX_RISE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_102/nonmatchings/fx_102", fx_102_headerPtr);
