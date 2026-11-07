#include "fxmod/fx_008.h"

s32 fx_008_op80(s32 mode, Fx008Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 elapsed;
    u16 animFrame;

    if (mode == 0) {
        *args = sizeof(Fx008Work);
    } else if (mode == 1) {
        fx_008_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURE_SPARKLE_SWIRL), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_008_trails;
            work->emit0->trailSlots = FX_CURE_SPARKLE_SWIRL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_CURE_SPARKLE_SWIRL_TRAIL_STRIDE;
        }
        work->alpha16 = 0x1000;
        work->frame14 = 0;
        ex->btlfx_getUnitBodyPos(0, 1, (s16 *)&work->posC);
    } else {
        frame = entry->frame;
        elapsed = frame - FX_CURE_STAR_FLARES_FRAME;
        if (elapsed >= 0 && elapsed < FX_CURE_STAR_FLARES_FRAMES) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_CURE_STAR_FLARES), &work->posC, work->frame14, work->alpha16);
            if (elapsed >= FX_CURE_STAR_FLARES_FADE_FRAME) {
                work->alpha16 -= 0x1000 / (FX_CURE_STAR_FLARES_FRAMES - FX_CURE_STAR_FLARES_FADE_FRAME);
            }
            animFrame = work->frame14 + 1;
            work->frame14 = animFrame;
            if (animFrame >= FX_CURE_STAR_FLARES_ANIM_FRAMES) {
                animFrame = FX_CURE_STAR_FLARES_ANIM_FRAMES - 1;
            }
            work->frame14 = animFrame;
        }
        return frame >= FX_CURE_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_008/nonmatchings/fx_008", fx_008_headerPtr);
