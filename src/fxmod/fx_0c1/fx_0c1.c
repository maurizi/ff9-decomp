#include "fxmod/fx_0c1.h"

static s32 fx_0c1_op80ParticleMotion(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    BTLFX_EXPORTS(fx_0c1_env)->btlfx_lerpVec((s16 *)&fx_0c1_pathFrom, (s16 *)&fx_0c1_pathTo, (frame << 12) / 9, (s16 *)pos);
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_0c1_op80(s32 mode, Fx0c1Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone bone; /* also holds angles at frame 0x24 */
    s32 frame;

    if (mode == 0) {
        *args = 0x50;
    } else if (mode == 1) {
        fx_0c1_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos10);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot18);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos20);
        ex->btlfx_getUnitBoneMatrix(0x10, 0x17, &bone);
        ex->btlfx_matrixPointAhead((MATRIX *)&bone, 0x64, (s32)&work->pos28);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos28);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x20);
        }
    } else {
        frame = entry->frame;
        if (frame == 0x24) {
            ex->btlfx_getUnitBonePos(0x10, 0x17, (u16 *)&work->pos38);
            fx_0c1_pathFrom.vx = work->pos38.vx;
            fx_0c1_pathFrom.vy = work->pos38.vy;
            fx_0c1_pathFrom.vz = work->pos38.vz;
            ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot18);
            ex->btlfx_pointAhead((s16 *)&work->pos10, (s16 *)&work->rot18, 0x12C, (s16 *)&work->pos30);
            ex->btlfx_aimAngles((s16 *)&work->pos10, (s16 *)&work->pos20, (s16 *)&bone);
            ex->btlfx_pointAhead((s16 *)&work->pos20, (s16 *)&bone, 0, (s16 *)&fx_0c1_pathTo);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos30);
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&fx_0c1_emitPos);
            if (work->emit8 != NULL) {
                work->emit8->file->m.module.update = fx_0c1_op80ParticleMotion;
            }
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            ex->btlfx_aimAngles((s16 *)&work->pos38, (s16 *)&work->pos20, (s16 *)&work->rot48);
            work->rot40.vx = work->rot48.vx;
            work->rot40.vy = work->rot48.vy;
            work->rot40.vz = work->rot48.vz;
        }
        if (frame == 0x2D) {
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos20);
            if (work->emitC != NULL) {
                work->emitC->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
            }
        }
        if (frame >= 0x24) {
            if (work->emit4 != NULL) {
                if (work->emit4->state == BTLFX_EMIT_DONE) {
                    work->emit4 = NULL;
                }
                if (work->emit4 != NULL) {
                    work->emit4->rot.vx = work->rot40.vx;
                    work->emit4->rot.vy = work->rot40.vy;
                    work->emit4->rot.vz = work->rot40.vz;
                }
            }
            if (work->emit8 != NULL) {
                if (work->emit8->state == BTLFX_EMIT_DONE) {
                    work->emit8 = NULL;
                }
                if (work->emit8 != NULL) {
                    work->emit8->rot.vx = work->rot48.vx;
                    work->emit8->rot.vy = work->rot48.vy;
                    work->emit8->rot.vz = work->rot48.vz;
                }
            }
        }
        return frame >= 0x46;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0c1/nonmatchings/fx_0c1", fx_0c1_headerPtr);
