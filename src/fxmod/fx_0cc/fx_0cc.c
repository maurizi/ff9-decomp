#include "fxmod/fx_0cc.h"

static s32 fx_0cc_op80PoolTask(s32 mode, Fx0ccPart *part, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx0ccWork *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 mpos[3];
    s32 scale[3];
    SVECTOR rot;
    SVECTOR pos;
    s32 t;

    if (mode == 1) {
        part->state0 = 0;
        part->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_METEORITE_EMBERS), (s32)&part->pos4);
        if (part->emitC != NULL) {
            part->emitC->otZ = 0;
        }
    } else {
        switch (part->state0) {
        case FX_METEORITE_STATE_FALL:
            BTLFX_COPY_SVEC(&pos, &part->pos10);
            t = (frame << 12) / FX_METEORITE_FALL_FRAMES;
            pos.vy -= FX_METEORITE_FALL_HEIGHT;
            pos.vy += t;
            pos.vz = pos.vz + ((0x1000 - t) >> 1);
            BTLFX_COPY_SVEC(&part->pos4, &pos);
            mpos[0] = part->pos4.vx;
            mpos[1] = part->pos4.vy;
            mpos[2] = part->pos4.vz;
            scale[0] = scale[1] = scale[2] = 0x1C7;
            rot.vx = frame * 0x18;
            rot.vy = frame * 0x28;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, (VECTOR *)mpos, (s32)scale, work->model10);
            env->otZ = 0x80;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_METEORITE_RED_GLOW), &part->pos4, NULL, 0x1000, 0, 1, -1, 0, 0);
            env->otZ = 0;
            if (frame >= FX_METEORITE_FALL_FRAMES) {
                part->state0 = FX_METEORITE_STATE_IMPACT;
                ((s16 *)part)[-1] = -1;
                if (part->emitC != NULL) {
                    part->emitC->frame = part->emitC->tail->anim.duration;
                }
                part->pos4.vy = 0;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, FX_METEORITE_IMPACT_TINT_RGB);
            }
            break;
        case FX_METEORITE_STATE_IMPACT:
            rot.vx = 0x400;
            rot.vy = 0;
            rot.vz = frame << 5;
            rot.pad = 1;
            BTLFX_COPY_SVEC(&pos, &part->pos4);
            pos.vy -= frame * 4;
            env->otZ = 0x10;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_METEORITE_IMPACT_SMOKE), &pos, NULL, 0x3000, frame, 1, -1, 0, 0);
            if (frame < FX_METEORITE_FLASH_FRAMES) {
                env->otZ = 0x64;
                ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_METEORITE_IMPACT_RING), &pos, NULL, -1, frame, 1, -1);
                env->otZ = 0;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_METEORITE_IMPACT_FLASH), &part->pos4, &rot, 0x1000, frame, 1,
                                  -1, 0, 0);
            }
            env->otZ = 0;
            if (frame >= FX_METEORITE_IMPACT_FRAMES) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_0cc_op80(s32 mode, Fx0ccWork *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx0ccPart *part;
    s32 frame;

    if (mode == 0) {
        ((EffInit *)args)->workSize = sizeof(Fx0ccWork);
        ((EffInit *)args)->elemSize = sizeof(Fx0ccPart);
        ((EffInit *)args)->elemFn = (s32)fx_0cc_op80PoolTask;
        ((EffInit *)args)->poolCount = FX_METEORITE_SLOTS;
    } else if (mode == 1) {
        work->state0 = 0;
        work->spawnCountC = 0;
        fx_0cc_env = env;
        ex->btlfx_initEffModels(fx_0cc_effModels, 1);
        work->model10 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_METEORITE_ROCK), 0);
        BTLFX_COPY_SVEC(&work->pos4, &env->targets->centerX);
        work->pos4.vy = 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            if (frame & 1) {
                if (work->spawnCountC < FX_METEORITE_ROCKS) {
                    work->spawnCountC++;
                    part = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (part != NULL) {
                        part->pos10.vx = work->pos4.vx + ex->btlfx_randCentered(0x800);
                        part->pos10.vz = work->pos4.vz + ex->btlfx_randCentered(0x800);
                        part->pos10.vy = 0;
                    }
                }
            }
            if (work->spawnCountC >= FX_METEORITE_ROCKS) {
                if (frame >= FX_METEORITE_FRAMES) {
                    ex->btlfx_clearEffModels();
                }
                return 1;
            }
            break;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0cc/nonmatchings/fx_0cc", fx_0cc_headerPtr);
