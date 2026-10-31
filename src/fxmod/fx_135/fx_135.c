#include "fxmod/fx_135.h"

s32 fx_135_op80(s32 mode, Fx135Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->pos8.vx = env->targets->centerX;
        work->pos8.vy = env->targets->centerY;
        work->pos8.vz = env->targets->centerZ;
        work->pos8.vy -= 0xC8;
        fx_135_env = env;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos10);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos8, (s16 *)&work->rot18);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos10);
    } else {
        frame = entry->frame;
        if (frame == 8) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->rot18.vx;
                work->emit0->rot.vy = work->rot18.vy;
                work->emit0->rot.vz = work->rot18.vz;
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_135/nonmatchings/fx_135", fx_135_headerPtr);
