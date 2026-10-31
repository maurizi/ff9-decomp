#include "fxmod/fx_0a3.h"

s32 fx_0a3_op80(s32 mode, Fx0a3Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        fx_0a3_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos10);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot18);
        ex->btlfx_pointAhead((s16 *)&work->pos8, (s16 *)&work->rot18, 0x190, (s16 *)&work->pos8);
        ex->btlfx_pointAhead((s16 *)&work->pos10, (s16 *)&work->rot18, 0xC8, (s16 *)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)D_801E792C;
            work->emit0->trailSlots = 0x2C;
            work->emit0->trailStride = 0x28;
        }
    } else {
        frame = entry->frame;
        if (frame == 0xA) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)D_801E800C;
                work->emit4->trailSlots = 0x14;
                work->emit4->trailStride = 0x28;
            }
        }
        return frame >= 0x1E;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a3/nonmatchings/fx_0a3", fx_0a3_headerPtr);
