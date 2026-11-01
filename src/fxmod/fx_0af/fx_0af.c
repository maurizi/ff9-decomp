#include "fxmod/fx_0af.h"

s32 fx_0af_op80(s32 mode, Fx0afWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *p;
    s32 frame;

    if (mode == 0) {
        *args = 0x20;
    } else if (mode == 1) {
        work->state0 = 0;
        fx_0af_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos10, (s16 *)&work->aim18);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos8);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_0af_trails;
            work->emit4->trailSlots = 0x1E;
            work->emit4->trailStride = 0x4C;
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = 0x20;
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim18.vx;
                work->emit4->rot.vy = work->aim18.vy;
                work->emit4->rot.vz = work->aim18.vz;
            }
            if (frame == 0x10) {
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos10);
                if (p != NULL) {
                    p->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos10);
                if (p != NULL) {
                    p->trails = (s32)fx_0af_trails2;
                    p->trailSlots = 0x1E;
                    p->trailStride = 0x4C;
                    p->otZ = 0x20;
                }
            }
            if (frame >= 0x3C) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0af/nonmatchings/fx_0af", fx_0af_headerPtr);
