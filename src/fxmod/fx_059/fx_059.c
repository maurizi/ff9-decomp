#include "fxmod/fx_059.h"

static s32 fx_059_op80PoolTask(s32 mode, SVECTOR *pos, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx059Work80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode != 1) {
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, fx_059_tailBlocks[work->kindC]), pos, 0, 0x1000, frame, 1,
                          0x1000 - (frame << 12) / 12, 0, 0);
        if (frame >= 8) {
            return 1;
        }
    }
    return 0;
}

s32 fx_059_op80(s32 mode, Fx059Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx059Rot rot;
    SVECTOR pos;
    s32 frame;
    s32 radius;
    SVECTOR *trailPt;
    void (*fn)(SVECTOR *);

    if (mode == 0) {
        *args = 0x1C;
    } else if (mode == 1) {
        s32 kind = *args;
        work->state0 = 0;
        work->depthZ4 = -0x20;
        work->kindC = kind - 1;
        work->angle8 = (kind << 10) - 0x400;
        fx_059_env = env;
        work->angleStep10 = (ex->btlfx_rand() & 0x1F) + 0x100;
        work->viewRot18 = 0;
        work->pool14 = (Unk800DA1B8 *)ex->btlfx_allocTaskMem(entry, ex->btlfx_poolSize(8, 12));
        ex->btlfx_poolInit(work->pool14, entry, 8, 12, (s32)fx_059_op80PoolTask);
        return 0;
    } else {
        frame = entry->frame;
        rot.rotZ4 = work->viewRot18;
        work->viewRot18 += 0x10;
        fn = ex->btlfx_pushViewRotation;
        rot.rotXY0 = 0;
        fn((SVECTOR *)&rot);
        switch (work->state0) {
        case 0:
            radius = 0xA0 - frame * 0xA0 / 6;
            pos.vx = (ex->math_cos(work->angle8) * radius) >> 12;
            pos.vy = (ex->math_sin(work->angle8) * radius) >> 12;
            pos.vz = work->depthZ4;
            work->depthZ4 += 0xC8;
            work->angle8 += work->angleStep10;
            trailPt = ex->btlfx_poolSpawn((s16 *)work->pool14);
            if (trailPt != NULL) {
                ((s32 *)trailPt)[0] = ((s32 *)&pos)[0];
                ((s32 *)trailPt)[1] = ((s32 *)&pos)[1];
            }
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, fx_059_headBlocks[work->kindC]), &pos, 0, 0x1000, 0,
                              1, -1, 0, 0);
            if (frame >= 6) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            if (frame >= 7) {
                ex->btlfx_popViewMatrix();
                return 1;
            }
            break;
        }
        if (work->pool14 != NULL) {
            ex->btlfx_poolUpdate(work->pool14);
        }
        ex->btlfx_popViewMatrix();
    }
    return 0;
}

s32 fx_059_op81(s32 mode, Fx059Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        fx_059_env = env;
        work->hold0 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HASTE_YELLOW_RED_RINGS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HASTE_ORANGE_RINGS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HASTE_CLOCK_FACE), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        s32 frame = entry->frame;
        if (work->hold0 == 0 && frame >= 0x20) {
            return 1;
        }
    }
    return 0;
}

s32 fx_059_op82(s32 mode, Fx059Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;

    if (mode == 0) {
        *args = 0xC;
    } else if (mode == 1) {
        work->hold0 = 0;
        fx_059_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_HASTE_VIOLET_RINGS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
        }
    } else {
        s32 frame = entry->frame;
        if (work->hold0 == 0 && frame >= 0x18) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_059/nonmatchings/fx_059", fx_059_headerPtr);
