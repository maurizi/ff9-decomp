#include "fxmod/fx_084.h"

static s32 fx_084_op80ParticleMotion(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_084_env);
    s32 t = frame << 8;
    s32 arcAng;

    ex->btlfx_lerpVec((s16 *)&fx_084_pathFrom, (s16 *)&fx_084_pathTo, t, (s16 *)pos);
    arcAng = t >> 1;
    pos->vy -= (ex->math_sin(arcAng) * FX_DRAIN_ORB_ARC) >> 12;
    switch (pt->slot) {
    case 1:
        pos->vy -= (ex->math_sin(arcAng) << 9) >> 12;
        break;
    case 3:
        pos->vy += (ex->math_sin(arcAng) << 9) >> 12;
        break;
    }
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= FX_DRAIN_ORB_FRAMES) {
        return -1;
    }
    return 0;
}

s32 fx_084_op80(s32 mode, Fx084Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx084Work);
    } else if (mode == 1) {
        fx_084_env = env;
        work->state0 = FX_DRAIN_STATE_START;
        fx_084_task = entry;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        BTLFX_COPY_SVEC(&fx_084_pathFrom, &work->pos4);
    } else {
        frame = entry->frame;
        if (ex->btlfx_isEnemyUnit(0x10) == 0) {
            ex->btlfx_getUnitBodyPos(0x10, 1, (s16 *)&work->posC);
        } else {
            ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->posC);
        }
        BTLFX_COPY_SVEC(&fx_084_pathTo, &work->posC);
        switch (work->state0) {
        case FX_DRAIN_STATE_START:
            work->state0 = FX_DRAIN_STATE_ORB;
            entry->frame = -1;
            work->emit1C = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAIN_PINK_ORB), (s32)&fx_084_pathFrom);
            if (work->emit1C != NULL) {
                work->emit1C->otZ = 0x10;
            }
            if (work->emit1C != NULL) {
                work->emit1C->file->m.module.update = fx_084_op80ParticleMotion;
            }
            break;
        case FX_DRAIN_STATE_ORB:
            if (frame >= FX_DRAIN_ORB_FRAMES) {
                work->state0 = FX_DRAIN_STATE_RING;
                entry->frame = -1;
                work->emit20 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAIN_RED_RING), (s32)&work->posC);
                if (work->emit20 != NULL) {
                    work->emit20->otZ = ex->btlfx_addRadiusSixth(0x10, 0x18);
                }
                ex->btlfx_aimAngles((s16 *)&work->posC, (s16 *)&work->pos4, (s16 *)&work->aim14);
            }
            break;
        case FX_DRAIN_STATE_RING:
            if (work->emit20 != NULL) {
                if (work->emit20->state == BTLFX_EMIT_DONE) {
                    work->emit20 = NULL;
                }
                if (work->emit20 != NULL) {
                    work->emit20->rot.vx = work->aim14.vx;
                    work->emit20->rot.vy = work->aim14.vy;
                    work->emit20->rot.vz = work->aim14.vz;
                }
            }
            if (frame == FX_DRAIN_SPECKS_FRAME) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAIN_PINK_SPECKS), (s32)&work->posC);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0x10, 0x18);
                }
            }
            if (frame >= FX_DRAIN_RING_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_084/nonmatchings/fx_084", fx_084_headerPtr);
