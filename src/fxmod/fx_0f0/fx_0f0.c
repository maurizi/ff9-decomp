#include "fxmod/fx_0f0.h"

s32 fx_0f0_op80(s32 mode, Fx0f0Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x2C;
    } else if (mode == 1) {
        fx_0f0_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos1C);
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos1C);
            break;
        case 0x26:
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos24);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos24);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos24);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos24);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos24);
            if (work->emit4 != NULL) {
                work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emit10 != NULL) {
                work->emit10->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            if (work->emit4 != NULL) {
                work->emit4->trails = (s32)fx_0f0_op80Trails0;
                work->emit4->trailSlots = 0x1C;
                work->emit4->trailStride = 0x9C;
            }
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_0f0_op80Trails1;
                work->emit8->trailSlots = 0x1C;
                work->emit8->trailStride = 0x9C;
            }
            if (work->emit10 != NULL) {
                work->emit10->trails = (s32)fx_0f0_op80Trails2;
                work->emit10->trailSlots = 0x3A;
                work->emit10->trailStride = 0x70;
            }
            break;
        }
        if (frame >= 0x26) {
            if (work->emit10 != NULL) {
                if (work->emit10->state == BTLFX_EMIT_DONE) {
                    work->emit10 = NULL;
                }
                if (work->emit10 != NULL) {
                    work->emit10->target = (s32)&work->pos14;
                }
            }
        }
        return frame >= 0x50;
    }
    return 0;
}

s32 fx_0f0_op81(s32 mode, Fx0f0Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x14;
    } else if (mode == 1) {
        fx_0f0_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->posC);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->emit8 != NULL) {
            work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->posC);
        }
        return frame >= 0x3C;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0f0/nonmatchings/fx_0f0", fx_0f0_headerPtr);
