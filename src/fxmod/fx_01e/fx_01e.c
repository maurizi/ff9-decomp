#include "fxmod/fx_01e.h"

s32 fx_01e_op80(s32 mode, Fx01eWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        work->unk0 = 0;
        fx_01e_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_01e_trails;
            work->emit4->trailSlots = 0x64;
            work->emit4->trailStride = 0x1C;
        }
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->aim18);
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim18.vx;
                work->emit4->rot.vy = work->aim18.vy;
                work->emit4->rot.vz = work->aim18.vz;
            }
            if (frame == 8) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0xC);
                }
            }
            if (frame >= 0x3C) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_01e/nonmatchings/fx_01e", fx_01e_headerPtr);
