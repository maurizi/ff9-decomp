#include "fxmod/fx_0a9.h"

s32 fx_0a9_op80(s32 mode, Fx0a9Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Pool6CEntry *emitter;
    s32 frame;

    if (mode == 0) {
        *args = sizeof(Fx0a9Work80);
    } else if (mode == 1) {
        work->state0 = FX_LIGHTNING_STATE_ARCS;
        fx_0a9_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_LIGHTNING_ELECTRIC_ARCS), (s32)&work->pos4);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x18);
        }
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case FX_LIGHTNING_STATE_ARCS:
            if (frame >= FX_LIGHTNING_ARCS_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

static s32 fx_0a9_op83PoolTask(s32 mode, Fx0a9Part *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0a9Work83 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR zero;
    SVECTOR *pt;
    s32 fade;

    if (mode == 1) {
        part->drag12 = ex->btlfx_randCentered(0x20) + 0xE00;
        part->kind17 = (ex->btlfx_rand() % 3) / 2;
    } else {
        pt = part->point18;
        pt->pad = FX_LIGHTNING_BOLT_WIDTH;
        fade = ex->math_cos(frame << 7);
        ((s32 *)&zero)[0] = 0;
        ((s32 *)&zero)[1] = 0;
        ex->btlfx_stepModuleParticle((Pool6CEntry *)part, 0, &zero, pt);
        if (part->fromEnd16 == 0) {
            ((Fx0a9Bolt *)(part->point18 - 2))->lastSlot0 = ((Fx0a9Bolt *)(part->point18 - 2))->count2 = FX_LIGHTNING_BOLT_POINTS - 1;
            ((Fx0a9Bolt *)(part->point18 - 2))->head1 = 0;
            if (part->kind17 == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_LIGHTNING_BOLT_PILLAR), 0, NULL, 4, 0x1000, fade, BTLFX_ABR_ADD,
                                  (u8 *)part->point18 - 4, 0);
            } else {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_LIGHTNING_TRAIL_2), 0, NULL, 4, 0x1000, fade, BTLFX_ABR_ADD,
                                  (u8 *)part->point18 - 4, 2);
            }
        }
        if (frame >= FX_LIGHTNING_BOLT_FRAMES) {
            return 1;
        }
    }
    return 0;
}

static void fxm_resetBolts(void) {
    fx_0a9_nextBolt = 0;
}

static void fxm_spawnBolt(Unk80100C78Entry *entry, SVECTOR *from, SVECTOR *to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_0a9_env);
    SVECTOR pos;
    SVECTOR ang;
    s32 rollBase;
    Fx0a9Bolt *bolt;
    SVECTOR *pt;
    Fx0a9Part *part;
    s32 i;
    s32 t;
    s32 sinv;
    s32 roll;

    ex->btlfx_aimAngles((s16 *)from, (s16 *)to, (s16 *)&ang);
    i = 0;
    rollBase = ex->btlfx_rand();
    bolt = &fx_0a9_bolts[fx_0a9_nextBolt];
    pt = &bolt->pts4[FX_LIGHTNING_BOLT_POINTS - 1];
    bolt->lastSlot0 = bolt->count2 = FX_LIGHTNING_BOLT_POINTS - 1;
    bolt->head1 = 0;
    do {
        t = 0x1000 - (i << 12) / (FX_LIGHTNING_BOLT_POINTS - 1);
        sinv = ex->math_sin(t >> 1);
        t += (sinv * ex->btlfx_randCentered(0x100)) >> 12;
        ex->btlfx_lerpVec((s16 *)from, (s16 *)to, t, (s16 *)&pos);
        roll = ((t * 3) >> 1) + rollBase + ex->btlfx_randCentered(0x100);
        ex->btlfx_offsetPoint(&pos, (s32)pt, 0, (XY32 *)&ang, (sinv * ((ex->btlfx_rand() & 0x7F) + 0x40)) >> 12, roll);
        part = ex->btlfx_poolSpawn((s16 *)entry->pool);
        if (part != NULL) {
            part->bolt14 = fx_0a9_nextBolt;
            part->fromEnd16 = FX_LIGHTNING_BOLT_POINTS - 1 - i;
            part->point18 = pt;
            part->pos0[0] = pt->vx << 12;
            part->pos0[1] = pt->vy << 12;
            part->pos0[2] = pt->vz << 12;
            part->velC[0] = ((pt->vx - pos.vx) >> 1) + ((ex->btlfx_randCentered(0x18) * sinv) >> 12);
            part->velC[1] = ((pt->vy - pos.vy) >> 1) + ((ex->btlfx_randCentered(0x18) * sinv) >> 12);
            part->velC[2] = ((pt->vz - pos.vz) >> 1) + ((ex->btlfx_randCentered(0x18) * sinv) >> 12);
        }
        i++;
        pt--;
    } while (i < FX_LIGHTNING_BOLT_POINTS);
    fx_0a9_nextBolt++;
    fx_0a9_nextBolt = fx_0a9_nextBolt % FX_LIGHTNING_BOLT_SLOTS;
}

s32 fx_0a9_op83(s32 mode, Fx0a9Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR target;
    s32 frame;
    s32 i;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx0a9Work83);
        ((EffInit *)args)->elemSize = sizeof(Fx0a9Part);
        ((EffInit *)args)->elemFn = (s32)fx_0a9_op83PoolTask;
        ((EffInit *)args)->poolCount = FX_LIGHTNING_BOLT_SLOTS * FX_LIGHTNING_BOLT_POINTS;
    } else if (mode == 1) {
        fx_0a9_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos0);
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        fxm_resetBolts();
        return 0;
    } else {
        frame = entry->frame;
        ex->btlfx_getUnitArgBonePos(0x10, 0, (u16 *)&work->pos8);
        if (frame < FX_LIGHTNING_BOLTS_FRAMES) {
            for (i = 0; i < FX_LIGHTNING_BOLTS_PER_FRAME; i++) {
                BTLFX_COPY_SVEC(&target, &work->pos0);
                target.vx += ex->btlfx_randCentered(0x180);
                target.vy += ex->btlfx_randCentered(0x180);
                target.vz += ex->btlfx_randCentered(0x180);
                fxm_spawnBolt(entry, &work->pos8, &target);
            }
        }
        env->otZ = 0x40;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_LIGHTNING_BLUE_RING), &work->pos8, NULL,
                          ex->math_sin((frame << 11) / FX_LIGHTNING_BOLTS_FRAMES) * 2 / 3, 0, BTLFX_ABR_ADD, 0x1000 - ((frame & 1) << 11));
        env->otZ = 0;
        if (frame >= FX_LIGHTNING_BOLTS_FRAMES) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0a9/nonmatchings/fx_0a9", fx_0a9_headerPtr);
