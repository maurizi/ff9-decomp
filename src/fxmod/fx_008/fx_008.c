#include "fxmod/fx_008.h"

s32 fx_008_op80(s32 mode, Fx008Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 elapsed;
    u16 animFrame;

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
        elapsed = frame - 0x18;
        if (elapsed >= 0 && elapsed < 0x10) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, 3), &work->posC, work->frame14, work->alpha16);
            if (elapsed >= 4) {
                work->alpha16 -= 0x155;
            }
            animFrame = work->frame14 + 1;
            work->frame14 = animFrame;
            if (animFrame >= 6) {
                animFrame = 5;
            }
            work->frame14 = animFrame;
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_008/nonmatchings/fx_008", fx_008_headerPtr);
