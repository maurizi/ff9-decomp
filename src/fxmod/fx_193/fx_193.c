#include "fxmod/fx_193.h"

static s32 fx_193_op80ParticleMotion0(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_wobbleParticle(e, pos, &fx_193_op80From0, &fx_193_op80To0, -0x320, frame, frames);
}

static s32 fx_193_op80ParticleMotion1(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    return fxm_wobbleParticle(e, pos, &fx_193_op80From1, &fx_193_op80To1, 0x320, frame, frames);
}

static s32 fxm_wobbleParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *from, SVECTOR *to, s32 amp, s32 frame, s32 frames) {
    s32 t = (frame << 12) / 7;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_193_env);

    if (t > 0x1000) {
        t = 0x1000;
    }
    ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)pos);
    if (frame >= 4) {
        amp /= 2;
    }
    pos->vx += amp * ex->math_sin(t) / 4096;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_193_op80(s32 mode, Fx193Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_193_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos8);
        fx_193_op80To0.vx = work->pos8.vx;
        fx_193_op80To0.vy = work->pos8.vy;
        fx_193_op80To0.vz = work->pos8.vz;
        fx_193_op80To1.vx = work->pos8.vx;
        fx_193_op80To1.vy = work->pos8.vy;
        fx_193_op80To1.vz = work->pos8.vz;
        ex->btlfx_getUnitBonePos(0x10, 0xE, (u16 *)&fx_193_op80From0);
        ex->btlfx_getUnitBonePos(0x10, 0x10, (u16 *)&fx_193_op80From1);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&fx_193_emitPos);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&fx_193_emitPos);
        if (work->emit0 != NULL) {
            work->emit0->trails = (s32)fx_193_op80Trails0;
            work->emit0->trailSlots = 4;
            work->emit0->trailStride = 0xF0;
        }
        if (work->emit4 != NULL) {
            work->emit4->trails = (s32)fx_193_op80Trails1;
            work->emit4->trailSlots = 4;
            work->emit4->trailStride = 0xF0;
        }
        if (work->emit0 != NULL) {
            work->emit0->file->m.module.update = fx_193_op80ParticleMotion0;
        }
        if (work->emit4 != NULL) {
            work->emit4->file->m.module.update = fx_193_op80ParticleMotion1;
        }
    } else {
        return entry->frame >= 0x1E;
    }
    return 0;
}

s32 fx_193_op81(s32 mode, Fx193Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x20;
    } else if (mode == 1) {
        fx_193_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos10);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos18);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos10);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos10);
        work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos18);
    } else {
        frame = entry->frame;
        if (frame == 4) {
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos18);
            if (work->emit8 != NULL) {
                work->emit8->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_193/nonmatchings/fx_193", fx_193_headerPtr);
