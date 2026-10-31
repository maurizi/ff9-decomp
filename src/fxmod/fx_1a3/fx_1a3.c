#include "fxmod/fx_1a3.h"

static s32 fx_1a3_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    s32 t = (frame - 4) << 9;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1a3_env);

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)&D_801E93F4, (s16 *)&D_801E93FC, t, (s16 *)pos);
    pos->vy += ex->math_sin(t >> 1) * 0;
    e->pos.vx = pos->vx << 12;
    e->pos.vy = pos->vy << 12;
    e->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1a3_op80(s32 mode, Fx1a3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x28;
    } else if (mode == 1) {
        fx_1a3_env = env;
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot8);
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        D_801E93FC.vx = work->pos10.vx;
        D_801E93FC.vy = work->pos10.vy;
        D_801E93FC.vz = work->pos10.vz;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos18);
        ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&work->rot8, 0x190, (s16 *)&work->pos20);
        D_801E93F4.vx = work->pos20.vx;
        D_801E93F4.vy = work->pos20.vy;
        D_801E93F4.vz = work->pos20.vz;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&fx_1a3_emitPos);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos20);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_1a3_op80Trails;
            work->emit0->trailSlots = 0x23;
            work->emit0->trailStride = 0x20;
        }
        if (work->emit0 != NULL) {
            ((EffMotion *)work->emit0->file)->update = fx_1a3_op80ParticleMotion;
        }
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

s32 fx_1a3_op81(s32 mode, Fx1a3Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        fx_1a3_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->posC);
        work->pos14.vx = work->posC.vx;
        work->pos14.vy = work->posC.vy;
        work->pos14.vz = work->posC.vz;
        work->pos14.vy -= 0x190;
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos14);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->posC);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_1a3_op81Trails0;
            work->emit0->trailSlots = 0x33;
            work->emit0->trailStride = 0x20;
        }
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->posC);
            if (work->emit8 != NULL) {
                work->emit8->trails = (s32)fx_1a3_op81Trails1;
                work->emit8->trailSlots = 0x1E;
                work->emit8->trailStride = 0x70;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1a3/nonmatchings/fx_1a3", fx_1a3_headerPtr);
