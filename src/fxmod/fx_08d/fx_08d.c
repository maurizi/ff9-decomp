#include "fxmod/fx_08d.h"

s32 fx_08d_op80(s32 mode, Fx08dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u16 animFrame;

    if (mode == 0) {
        *args = sizeof(Fx08dWork);
    } else if (mode == 1) {
        fx_08d_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURA_STAR_TRAIL), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_08d_trails;
            work->emit0->trailSlots = FX_CURA_STAR_TRAIL_TRAIL_SLOTS;
            work->emit0->trailStride = FX_CURA_STAR_TRAIL_TRAIL_STRIDE;
        }
        work->frame1C = work->alpha1E = 0;
    } else {
        frame = entry->frame;
        if (frame == FX_CURA_CYAN_GLOW_FRAME) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURA_CYAN_GLOW), (s32)&work->pos14);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame == FX_CURA_CRESCENT_FRAME) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURA_CRESCENT_BURST), (s32)&work->posC);
        }
        if (frame >= FX_CURA_STAR_FLARES_FRAME && frame < FX_CURA_STAR_FLARES_FRAME + FX_CURA_STAR_FLARES_FRAMES) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_CURA_STAR_FLARES), &work->posC, work->frame1C, work->alpha1E);
            if (frame - FX_CURA_STAR_FLARES_FRAME < FX_CURA_STAR_FLARES_FADE_IN_FRAMES) {
                work->alpha1E += 0x333;
            } else if (work->alpha1E >= 0x249) {
                work->alpha1E -= 0x249;
            } else {
                work->alpha1E = 0;
            }
            animFrame = ++work->frame1C;
            if (animFrame > FX_CURA_STAR_FLARES_ANIM_FRAMES - 1) {
                animFrame = FX_CURA_STAR_FLARES_ANIM_FRAMES - 1;
            }
            work->frame1C = animFrame;
        }
        return frame >= FX_CURA_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_08d/nonmatchings/fx_08d", fx_08d_headerPtr);
