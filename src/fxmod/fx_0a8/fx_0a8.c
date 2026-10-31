#include "fxmod/fx_0a8.h"

s32 fx_0a8_op80(s32 mode, Fx0a8Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_0a8_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->unk8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->unk10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->unk8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->unk8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->unk10.vx;
                work->emit0->rot.vy = work->unk10.vy;
                work->emit0->rot.vz = work->unk10.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->unk10.vx;
                work->emit4->rot.vy = work->unk10.vy;
                work->emit4->rot.vz = work->unk10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_0a8_op81(s32 mode, Fx0a8Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        work->unk20.vx = env->targets->centerX;
        work->unk20.vy = env->targets->centerY;
        work->unk20.vz = env->targets->centerZ;
        fx_0a8_env = env;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->unk20);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->unk20);
        work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->unk20);
        if (work->emit4 != NULL) {
            work->emit4->unk14 = (s32)D_801E7B78;
            work->emit4->unk12 = 6;
            work->emit4->unkC = 0x60;
        }
    } else {
        frame = entry->frame;
        if (frame == 8) {
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->unk20);
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->unk20);
            work->emit18 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->unk20);
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a8/nonmatchings/fx_0a8", fx_0a8_headerPtr);
