#include "fxmod/fx_1f4.h"

s32 fx_1f4_op80(s32 mode, Fx1f4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1f4Work);
    } else if (mode == 1) {
        fx_1f4_env = env;
        work->fadeOut0 = 0;
        work->fadeIn4 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VANISH_WHITE_STREAK), (s32)&work->pos8);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x200);
            emitter->trails = (s32)fx_1f4_op80Trails0;
            emitter->trailSlots = FX_VANISH_STREAK_TRAIL_SLOTS;
            emitter->trailStride = FX_VANISH_STREAK_TRAIL_STRIDE;
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case FX_VANISH_STREAK_2_FRAME:
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VANISH_WHITE_STREAK_2), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x200);
                emitter->trails = (s32)fx_1f4_op80Trails1;
                emitter->trailSlots = FX_VANISH_STREAK_TRAIL_SLOTS;
                emitter->trailStride = FX_VANISH_STREAK_TRAIL_STRIDE;
            }
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_VANISH_GOLD_GLOW), (s32)&work->pos8);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x200);
            }
            break;
        case FX_VANISH_FADE_OUT_FRAME:
            work->fadeOut0 = FX_VANISH_FADE_FRAMES;
            break;
        case FX_VANISH_FADE_IN_FRAME:
            work->fadeIn4 = FX_VANISH_FADE_FRAMES;
            break;
        }
        if (work->fadeOut0 != 0) {
            work->fadeOut0--;
            ex->btlfx_fadeUnitById(0, work->fadeOut0 * 8);
        }
        if (work->fadeIn4 != 0) {
            work->fadeIn4--;
            ex->btlfx_fadeUnitById(0, 0x80 - work->fadeIn4 * 8);
        }
        return frame >= FX_VANISH_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f4/nonmatchings/fx_1f4", fx_1f4_headerPtr);
