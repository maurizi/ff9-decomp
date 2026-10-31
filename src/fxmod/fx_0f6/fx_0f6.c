#include "fxmod/fx_0f6.h"

s32 fx_0f6_op80(s32 mode, Fx0f6Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    Fx0f6Layer *p;
    Fx0f6Layer *q;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        work->pos8.vz = env->targets->centerZ;
        work->linePos10.vx = env->targets->centerX;
        work->linePos10.vy = env->targets->centerY;
        work->linePos10.vz = env->targets->centerZ;
        fx_0f6_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = 0x100;
        }
    } else {
        frame = entry->frame;
        q = fx_0f6_lines;
        p = fx_0f6_lines;
        for (i = 0; i < 4; i++, p++, q++) {
            if (p->done2 == 0 && frame >= p->start4 && frame < p->end6) {
                ex->btlfx_drawSpriteLine((u8 *)ex->btlfx_getFileData(entry->file, q->block0), &work->linePos10, p->frame8,
                                  p->alphaA);
                p->alphaA -= 0x1000 / (p->end6 - p->start4);
                if (frame == p->end6 - 1) {
                    p->done2 = 1;
                }
                p->frame8++;
            }
        }
        if (frame == 0x14) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = 0x100;
            }
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f6/nonmatchings/fx_0f6", fx_0f6_headerPtr);
