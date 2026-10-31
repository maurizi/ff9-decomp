#include "fxmod/fx_044.h"

s32 fx_044_op80(s32 mode, Fx044Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x20;
    } else if (mode == 1) {
        fx_044_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
        ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->unk8, (s16 *)&work->unk18);
    } else {
        frame = entry->frame;
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->unk18.vx;
                work->emit4->rot.vy = work->unk18.vy;
                work->emit4->rot.vz = work->unk18.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_044_op81(s32 mode, Fx044Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x24;
    } else if (mode == 1) {
        fx_044_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos14);
        work->pos1C.vx = work->posC.vx;
        work->pos1C.vy = work->posC.vy;
        work->pos1C.vz = work->posC.vz;
        work->pos1C.vy -= 0xC8;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos1C);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos14);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->unk14 = (s32)D_801E7A74;
            work->emit0->unk12 = 0xD;
            work->emit0->unkC = 0x90;
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_044/nonmatchings/fx_044", fx_044_headerPtr);
