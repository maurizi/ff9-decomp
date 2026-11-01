#include "fxmod/fx_0a1.h"

s32 fx_0a1_op80(s32 mode, Fx0a1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos;
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *args = 0x30;
    } else if (mode == 1) {
        work->state0 = 0;
        work->unk4 = 0;
        fx_0a1_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos10);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos18);
        ex->btlfx_aimAngles((s16 *)&work->pos8, (s16 *)&work->pos18, (s16 *)&work->aim28);
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            t = (frame << 12) / 10;
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos20);
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos20);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x40);
                }
            }
            ex->btlfx_lerpVec((s16 *)&work->pos18, (s16 *)&work->pos8, 0x1000 - ex->math_cos(t >> 2),
                              (s16 *)&pos);
            if (frame == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x10), 0, NULL, -1, 0x1000, 0, 1, fx_0a1_trail, 0);
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x10), (s32)&pos, NULL, 0, 0x1000, 0x1000, 1,
                              fx_0a1_trail, 0);
            if (frame >= 0xA) {
                work->state0 = 1;
                entry->frame = -1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            break;
        case 1:
            if (frame == 7) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                    emitter->trails = (s32)fx_0a1_trails;
                    emitter->trailSlots = 0x2D;
                    emitter->trailStride = 0x4C;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame == 0xC) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                }
            }
            if (frame == 0x11) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x40);
                    emitter->trails = (s32)fx_0a1_trails2;
                    emitter->trailSlots = 0x1E;
                    emitter->trailStride = 0x4C;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos10);
            }
            if (frame >= 0x20) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a1/nonmatchings/fx_0a1", fx_0a1_headerPtr);
