#include "fxmod/fx_0bf.h"

s32 fx_0bf_op80(s32 mode, Fx0bfWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0bfWork80);
    } else if (mode == 1) {
        fx_0bf_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        work->tintC = FX_THUNDER_SLASH_FLASH_GREY;
    } else {
        frame = entry->frame;
        env->otZ = 0x80;
        ex->btlfx_drawSprite((u8 *)ex->btlfx_getFileData(entry->file, FX_THUNDER_SLASH_LIGHTNING), &work->pos4, frame, BTLFX_ABR_ADD, -1);
        if (frame == FX_THUNDER_SLASH_SPARKS_FRAME) {
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_THUNDER_SLASH_SPARKS), (s32)&work->pos4);
            if (work->emit0 != NULL) {
                work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame == FX_THUNDER_SLASH_FLASH_FRAME) {
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, work->tintC, work->tintC, work->tintC);
        }
        return frame >= FX_THUNDER_SLASH_FRAMES;
    }
    return 0;
}

s32 fx_0bf_op81(s32 mode, Fx0bfWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0bfWork81);
    } else if (mode == 1) {
        fx_0bf_env = env;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitPos(BTLFX_UNIT_CASTER, (s16 *)&work->pos0);
        env->camera->points[0].vx = work->pos0.vx;
        env->camera->points[0].vy = work->pos0.vy;
        env->camera->points[0].vz = work->pos0.vz;
        return frame >= FX_THUNDER_SLASH_CAMERA_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0bf/nonmatchings/fx_0bf", fx_0bf_headerPtr);
