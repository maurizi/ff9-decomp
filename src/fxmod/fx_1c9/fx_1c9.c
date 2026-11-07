#include "fxmod/fx_1c9.h"

s32 fx_1c9_op80(s32 mode, Fx1c9Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx1c9Work);
    } else if (mode == 1) {
        fx_1c9_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        work->colorC = FX_THUNDER_SLASH_FLASH_GREY;
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
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, work->colorC, work->colorC, work->colorC);
        }
        return frame >= FX_THUNDER_SLASH_FRAMES;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1c9/nonmatchings/fx_1c9", fx_1c9_headerPtr);
