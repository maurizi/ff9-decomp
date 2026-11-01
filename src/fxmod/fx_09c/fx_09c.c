#include "fxmod/fx_09c.h"

s32 fx_09c_op80(s32 mode, Fx09cWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR tip;
    SVECTOR bonePos;
    BtlBone bone;
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = 0x40;
    } else if (mode == 1) {
        work->emit20 = NULL;
        work->emit3C = NULL;
        fx_09c_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        work->unk0.vx = work->pos10.vx + ex->btlfx_randRange(-0x100, 0x100);
        work->unk0.vy = work->pos10.vy + ex->btlfx_randRange(-0x100, 0x100);
        work->unk0.vz = work->pos10.vz + ex->btlfx_randRange(-0x100, 0x100);
        work->unk8.vx = work->pos10.vx + ex->btlfx_randRange(-0x100, 0x100);
        work->unk8.vy = work->pos10.vy + ex->btlfx_randRange(-0x100, 0x100);
        work->unk8.vz = work->pos10.vz + ex->btlfx_randRange(-0x100, 0x100);
    } else {
        frame = entry->frame;
        if (frame == 4) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos10);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame == 0xA) {
            ex->btlfx_getUnitBoneMatrix(0x10, 0x11, &bone);
            ex->btlfx_matrixPointAhead((MATRIX *)&bone, 0x800, (s32)&tip);
            ex->btlfx_getUnitBonePos(0x10, 0x11, (u16 *)&bonePos);
            ex->btlfx_aimAngles((s16 *)&tip, (s16 *)&bonePos, (s16 *)&work->aim34);
            work->aim34.vy -= 0xB4;
            ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&tip);
            work->pos2C.vx = tip.vx - ((ex->math_sin(work->aim34.vy) * 0x1E0) >> 12);
            work->pos2C.vz = tip.vz - ((ex->math_cos(work->aim34.vy) * 0x1E0) >> 12);
            work->pos2C.vy = tip.vy - ((ex->math_sin(work->aim34.vx) * 0x1E0) >> 12);
            work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos2C);
            work->emit3C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos2C);
        }
        if (frame == 0x12) {
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos18);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (work->emit20 != NULL) {
            if (work->emit20->state == BTLFX_EMIT_DONE) {
                work->emit20 = NULL;
            }
            if (work->emit20 != NULL) {
                work->emit20->rot.vx = work->aim34.vx;
                work->emit20->rot.vy = work->aim34.vy;
                work->emit20->rot.vz = work->aim34.vz;
            }
        }
        if (work->emit3C != NULL) {
            if (work->emit3C->state == BTLFX_EMIT_DONE) {
                work->emit3C = NULL;
            }
            if (work->emit3C != NULL) {
                work->emit3C->rot.vx = work->aim34.vx;
                work->emit3C->rot.vy = work->aim34.vy;
                work->emit3C->rot.vz = work->aim34.vz;
            }
        }
        return frame >= 0x2D;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_09c/nonmatchings/fx_09c", fx_09c_headerPtr);
