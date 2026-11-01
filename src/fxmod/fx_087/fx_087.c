#include "fxmod/fx_087.h"

s32 fx_087_op80(s32 mode, Fx087Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x34;
    } else if (mode == 1) {
        fx_087_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&work->pos1C);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos1C);
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_087_trails;
            work->emit4->trailSlots = 0x19;
            work->emit4->trailStride = 0x30;
        }
        ex->btlfx_aimAngles((s16 *)&work->posC, (s16 *)&work->pos14, (s16 *)&work->aim2C);
    } else {
        frame = entry->frame;
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->aim2C.vx;
                work->emit4->rot.vy = work->aim2C.vy;
                work->emit4->rot.vz = work->aim2C.vz;
            }
        }
        if (frame == 8) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos14);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_087/nonmatchings/fx_087", fx_087_headerPtr);
