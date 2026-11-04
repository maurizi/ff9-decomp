#include "fxmod/fx_08d.h"

s32 fx_08d_op80(s32 mode, Fx08dWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u16 animFrame;

    if (mode == 0) {
        *args = 0x20;
    } else if (mode == 1) {
        fx_08d_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos14);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURA_STAR_TRAIL), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_08d_trails;
            work->emit0->trailSlots = 3;
            work->emit0->trailStride = 0xA0;
        }
        work->frame1C = work->alpha1E = 0;
    } else {
        frame = entry->frame;
        if (frame == 0x12) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURA_CYAN_GLOW), (s32)&work->pos14);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame == 0x14) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_CURA_CRESCENT_BURST), (s32)&work->posC);
        }
        if (frame >= 0x25 && frame < 0x31) {
            ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, FX_CURA_STAR_FLARES), &work->posC, work->frame1C, work->alpha1E);
            if (frame - 0x25 < 6) {
                work->alpha1E += 0x333;
            } else if (work->alpha1E >= 0x249) {
                work->alpha1E -= 0x249;
            } else {
                work->alpha1E = 0;
            }
            animFrame = ++work->frame1C;
            if (animFrame > 5) {
                animFrame = 5;
            }
            work->frame1C = animFrame;
        }
        return frame >= 0x32;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_08d/nonmatchings/fx_08d", fx_08d_headerPtr);
