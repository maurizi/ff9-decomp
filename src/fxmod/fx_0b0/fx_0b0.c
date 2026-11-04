#include "fxmod/fx_0b0.h"

s32 fx_0b0_op80(s32 mode, Fx0b0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR pos0;
    SVECTOR pos1;
    Pool6CEntry *emitter;
    s32 frame;
    s32 t;

    if (mode == 0) {
        *args = 0x4C;
    } else if (mode == 1) {
        work->state0 = 0;
        fx_0b0_env = env;
        BTLFX_COPY_SVEC(&work->posC, &env->targets->centerX);
        BTLFX_COPY_SVEC(&work->pos14, &work->posC);
        work->posC.vy -= 0xF0;
        BTLFX_COPY_SVEC(&work->pos2C, &work->posC);
        BTLFX_COPY_SVEC(&work->pos34, &work->posC);
        work->pos2C.vx -= 0x300;
        work->pos34.vx += 0x300;
        ex->btlfx_getUnitBonePos(0x10, fx_0b0_bones[0], (u16 *)&work->pos1C);
        ex->btlfx_getUnitBonePos(0x10, fx_0b0_bones[1], (u16 *)&work->pos24);
        work->emit4 = work->emit8 = NULL;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame < 0x10) {
                t = ex->math_cos(frame << 7);
                t = (0x1000 - t) >> 1;
            } else {
                t = 0x1000;
            }
            ex->btlfx_getUnitBonePos(0x10, fx_0b0_bones[0], (u16 *)&work->pos1C);
            ex->btlfx_getUnitBonePos(0x10, fx_0b0_bones[1], (u16 *)&work->pos24);
            ex->btlfx_lerpVec((s16 *)&work->pos1C, (s16 *)&work->pos2C, t, (s16 *)&pos0);
            ex->btlfx_lerpVec((s16 *)&work->pos24, (s16 *)&work->pos34, t, (s16 *)&pos1);
            if (frame == 0) {
                BTLFX_COPY_SVEC(&work->pos3C, &pos0);
                work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEAF_SWIRL_LEAF_TRAIL), (s32)&work->pos3C);
                if (work->emit4 != NULL) {
                    work->emit4->trails = (s32)fx_0b0_trails;
                    work->emit4->trailSlots = 0x96;
                    work->emit4->trailStride = 0x14;
                }
                if (work->emit4 != NULL) {
                    work->emit4->otZ = 0x18;
                }
                BTLFX_COPY_SVEC(&work->pos44, &pos1);
                work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEAF_SWIRL_LEAF_TRAIL_2), (s32)&work->pos44);
                if (work->emit8 != NULL) {
                    work->emit8->trails = (s32)fx_0b0_trails2;
                    work->emit8->trailSlots = 0x96;
                    work->emit8->trailStride = 0x14;
                }
                if (work->emit8 != NULL) {
                    work->emit8->otZ = 0x18;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEAF_SWIRL_LEAF_TRAIL_3), (s32)&work->pos1C);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0b0_trails4;
                    emitter->trailSlots = 0x12;
                    emitter->trailStride = 0x14;
                    emitter->otZ = 0x18;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEAF_SWIRL_LEAF_TRAIL_4), (s32)&work->pos24);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0b0_trails5;
                    emitter->trailSlots = 0x12;
                    emitter->trailStride = 0x14;
                    emitter->otZ = 0x18;
                }
            } else {
                if (work->emit4 != NULL && work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit4, (s32)&pos0);
                if (work->emit8 != NULL && work->emit8->state == BTLFX_EMIT_DONE) {
                    work->emit8 = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit8, (s32)&pos1);
            }
            if (frame == 0xE) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEAF_SWIRL_DUST_SPECKS), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
                ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEAF_SWIRL_OLIVE_RING), (s32)&work->pos14);
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEAF_SWIRL_WIND_TRAIL), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0b0_trails3;
                    emitter->trailSlots = 0x10;
                    emitter->trailStride = 0x6C;
                    emitter->otZ = 0x80;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEAF_SWIRL_LEAF_TRAIL_3), (s32)&work->pos2C);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0b0_trails6;
                    emitter->trailSlots = 0x12;
                    emitter->trailStride = 0x14;
                    emitter->otZ = 0x18;
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LEAF_SWIRL_LEAF_TRAIL_4), (s32)&work->pos34);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_0b0_trails7;
                    emitter->trailSlots = 0x12;
                    emitter->trailStride = 0x14;
                    emitter->otZ = 0x18;
                }
            }
            if (frame >= 0x37) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0b0/nonmatchings/fx_0b0", fx_0b0_headerPtr);
