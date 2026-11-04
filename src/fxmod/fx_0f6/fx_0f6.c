#include "fxmod/fx_0f6.h"

s32 fx_0f6_op80(s32 mode, Fx0f6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    Fx0f6Layer *line;
    Fx0f6Layer *lineRef;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        work->pos8.vz = env->targets->centerZ;
        work->linePos10.vx = env->targets->centerX;
        work->linePos10.vy = env->targets->centerY;
        work->linePos10.vz = env->targets->centerZ;
        fx_0f6_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MEO_TWISTER_WIND_RINGS), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = 0x100;
        }
    } else {
        frame = entry->frame;
        lineRef = fx_0f6_lines;
        line = fx_0f6_lines;
        for (i = 0; i < 4; i++, line++, lineRef++) {
            if (line->done2 == 0 && frame >= line->start4 && frame < line->end6) {
                ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, lineRef->block0), &work->linePos10, line->frame8,
                                  line->alphaA);
                line->alphaA -= 0x1000 / (line->end6 - line->start4);
                if (frame == line->end6 - 1) {
                    line->done2 = 1;
                }
                line->frame8++;
            }
        }
        if (frame == 0x14) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_MEO_TWISTER_FIRE_ORBS), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x100;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f6/nonmatchings/fx_0f6", fx_0f6_headerPtr);
