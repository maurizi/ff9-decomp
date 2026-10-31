#include "fxmod/fx_02e.h"

s32 fx_02e_op80(s32 mode, Fx02eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        fx_02e_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x24) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos18);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos18);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame >= 0x24) {
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->pos18, (s16 *)&work->unk20);
            if (work->emit8 != NULL) {
                if (work->emit8->state == BTLFX_EMIT_DONE) {
                    work->emit8 = NULL;
                }
                if (work->emit8 != NULL) {
                    work->emit8->rot.vx = work->unk20.vx;
                    work->emit8->rot.vy = work->unk20.vy;
                    work->emit8->rot.vz = work->unk20.vz;
                }
            }
            if (work->emitC != NULL) {
                if (work->emitC->state == BTLFX_EMIT_DONE) {
                    work->emitC = NULL;
                }
                if (work->emitC != NULL) {
                    work->emitC->rot.vx = work->unk20.vx;
                    work->emitC->rot.vy = work->unk20.vy;
                    work->emitC->rot.vz = work->unk20.vz;
                }
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_02e/nonmatchings/fx_02e", fx_02e_headerPtr);
