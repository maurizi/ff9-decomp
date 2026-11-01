#include "fxmod/fx_008.h"

s32 fx_008_op80(s32 mode, Fx008Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    u16 n;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_008_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_008_trails;
            work->emit0->trailSlots = 0xB;
            work->emit0->trailStride = 0x8C;
        }
        work->alpha16 = 0x1000;
        work->frame14 = 0;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->posC);
    } else {
        frame = entry->frame;
        t = frame - 0x18;
        if (t >= 0 && t < 0x10) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 3), &work->posC, work->frame14, work->alpha16);
            if (t >= 4) {
                work->alpha16 -= 0x155;
            }
            n = work->frame14 + 1;
            work->frame14 = n;
            if (n >= 6) {
                n = 5;
            }
            work->frame14 = n;
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_008/nonmatchings/fx_008", fx_008_headerPtr);
