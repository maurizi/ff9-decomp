#include "fxmod/fx_038.h"

static s32 fx_038_op80PoolTask(s32 mode, Fx038Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx038Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR fallPos;
    SVECTOR trailPos;
    SVECTOR ringRot;
    s32 t;
    s32 fade;
    s32 spinAng;
    s32 growAng;
    s32 size;

    if (mode == 1) {
        it->state0 = FX_COMET_STATE_FALL;
        it->emit14 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COMET_ROCK_DEBRIS), (s32)&it->pos4);
        if (it->emit14 != NULL) {
            it->emit14->otZ = 0x10;
        }
        return 0;
    }
    switch (it->state0) {
    case FX_COMET_STATE_FALL:
        t = (frame << 12) / FX_COMET_FALL_FRAMES;
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_COMET_TAIL), 0, NULL, -1, 0x1000, 0, 1,
                              (u8 *)&fx_038_strips[it->strip20], 0);
        }
        BTLFX_COPY_SVEC(&fallPos, &it->pos18);
        fallPos.vy -= (0x1000 - t) * 2;
        fallPos.vz += 0x1000 - t;
        BTLFX_COPY_SVEC(&it->pos4, &fallPos);
        pos.vx = it->pos4.vx;
        pos.vy = it->pos4.vy;
        pos.vz = it->pos4.vz;
        it->rotC.vx += 0x46;
        it->rotC.vy += 0x40;
        it->rotC.vz += 8;
        scale.vz = 0x199;
        scale.vy = 0x199;
        scale.vx = 0x199;
        ex->Hi_DrawEffModel(&it->rotC, &pos, (s32)&scale, work->model10);
        env->otZ = 0x100;
        BTLFX_COPY_SVEC(&trailPos, &it->pos4);
        trailPos.vy += 0x140;
        trailPos.vz -= 0xA0;
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_COMET_TAIL), (s32)&trailPos, NULL, 0, 0x6000, ex->math_cos(t >> 2) * 2 / 3 + 0x555, 1,
                          (u8 *)&fx_038_strips[it->strip20], 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_COMET_BLUE_GLOW), &it->pos4, NULL, 0x1000, 0, 1, -1, 0, 0);
        env->otZ = 0;
        if (it->strip20 == 0) {
            env->camera->points[0].vx = it->pos4.vx;
            env->camera->points[0].vy = it->pos4.vy;
            env->camera->points[0].vz = it->pos4.vz;
        }
        if (it->emit14 != NULL && it->emit14->state == BTLFX_EMIT_DONE) {
            it->emit14 = NULL;
        }
        if (frame >= FX_COMET_FALL_FRAMES) {
            it->state0 = FX_COMET_STATE_IMPACT;
            ((s16 *)it)[-1] = -1;
            if (it->emit14 != NULL) {
                it->emit14->frame = it->emit14->tail->anim.duration;
            }
            it->pos4.vy = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_SUB, FX_COMET_IMPACT_DARKEN_RGB);
            it->emit14 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_COMET_IMPACT_SMOKE), (s32)&it->pos4);
            if (it->emit14 != NULL) {
                it->emit14->otZ = 0x46;
            }
            it->rotC.vx = 0;
            it->rotC.vy = ex->btlfx_rand();
            it->rotC.vz = 0;
        }
        break;
    case FX_COMET_STATE_IMPACT:
        t = (frame << 12) / FX_COMET_IMPACT_FRAMES;
        if (frame < FX_COMET_FLASH_FRAMES) {
            s32 tint = FX_COMET_FLASH_LEVEL - frame * FX_COMET_FLASH_LEVEL / FX_COMET_FLASH_FRAMES;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), BTLFX_ABR_ADD, tint, tint, tint);
        }
        spinAng = t >> 2;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_COMET_BLUE_GLOW), &it->pos4, NULL, (t << 1) + 0x1800, 0, 1, ex->math_cos(spinAng), 0, 0);
        BTLFX_COPY_SVEC(&trailPos, &it->pos4);
        trailPos.vy += 0x140;
        trailPos.vz -= 0xA0;
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, FX_COMET_TAIL), (s32)&it->pos4, NULL, 0, 0, 0x555, 1,
                          (u8 *)&fx_038_strips[it->strip20], 0);
        it->rotC.vy += ex->math_cos(spinAng) >> 7;
        pos.vx = it->pos4.vx;
        pos.vy = it->pos4.vy;
        pos.vz = it->pos4.vz;
        if (frame < FX_COMET_GLOW_DISH_FRAMES) {
            s32 flashAng;

            t = frame << 9;
            flashAng = t >> 2;
            fade = ex->math_cos(flashAng);
            size = (ex->math_sin(flashAng) / 2 + 0x800) / 3;
            scale.vz = size;
            scale.vx = size;
            scale.vy = (size * 9) >> 3;
            ex->Hi_DrawEffModel(&it->rotC, &pos, (s32)&scale, work->model14);
            ex->Hi_ModifyEffModelRGB(work->model14, ((fade * 2 - 0x1000) >> 5) - 0x80, ((fade * 2 - 0x1000) >> 5) - 0x80,
                                     ((fade * 2 - 0x1000) >> 5) - 0x80);
        }
        t = (frame << 12) / FX_COMET_IMPACT_FRAMES;
        fade = 0x1000 - t;
        growAng = t >> 2;
        scale.vx = scale.vz = (ex->math_sin(growAng) / 3 + 0xAAA) * 3 / 4;
        scale.vy = ex->math_cos(growAng) * 2 / 3 + 0x800;
        ex->Hi_DrawEffModel(&it->rotC, &pos, (s32)&scale, work->model18);
        ex->Hi_ModifyEffModelRGB(work->model18, (fade >> 5) - 0x80, (fade >> 5) - 0x80, (fade >> 5) - 0x80);
        ringRot.pad = 1;
        ringRot.vx = 0x400;
        ringRot.vz = 0;
        ringRot.vy = 0;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, FX_COMET_BLUE_RING), &it->pos4, &ringRot, ex->math_sin(growAng) + 0xAAA, 8, 1, fade);
        if (frame < FX_COMET_FIRE_GLOW_FRAMES) {
            s32 burstT;

            ringRot.pad = 1;
            ringRot.vx = 0x400;
            ringRot.vz = 0;
            ringRot.vy = 0;
            burstT = (frame << 12) / FX_COMET_FIRE_GLOW_FRAMES;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, FX_COMET_FIRE_GLOW), &it->pos4, &ringRot, ex->math_cos(burstT >> 2) * 3, 6, 1, ex->math_cos(burstT >> 2), 0, 0);
        }
        if (frame >= FX_COMET_IMPACT_FRAMES) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 fx_038_op80(s32 mode, Fx038Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    u8 spawn;
    Fx038Item *it;

    if (mode == 0) {
        init->workSize = sizeof(Fx038Work);
        init->elemSize = sizeof(Fx038Item);
        init->elemFn = (s32)fx_038_op80PoolTask;
        init->poolCount = FX_COMET_ROCK_COUNT;
    } else if (mode == 1) {
        work->hold0 = 0;
        work->countC = 0;
        fx_038_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos4);
        ex->btlfx_initEffModels(fx_038_effModels, FX_COMET_MODEL_COUNT);
        work->model10 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_COMET_ROCK), FX_COMET_ROCK_TPAGE, FX_COMET_ROCK_CLUT, 0, 0);
        work->model18 =
            ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_COMET_SHOCK_RING), FX_COMET_SHOCK_RING_TPAGE, FX_COMET_SHOCK_RING_CLUT,
                                       FX_COMET_SHOCK_RING_U, FX_COMET_SHOCK_RING_V);
        work->model14 = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, FX_COMET_GLOW_DISH));
    } else {
        frame = entry->frame;
        if (work->hold0 == 0) {
            spawn = 0;
            if (frame < FX_COMET_SPAWN_FRAMES) {
                spawn = fx_038_spawnFrames[frame];
            }
            if (spawn != 0 && work->countC < FX_COMET_ROCK_COUNT) {
                work->countC++;
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    it->pos18.vx = work->pos4.vx + ex->btlfx_randCentered(0x258);
                    it->pos18.vz = work->pos4.vz + ex->btlfx_randCentered(0x258);
                    it->pos18.vy = 0;
                    it->strip20 = work->countC - 1;
                    it->rotC.vx = ex->btlfx_rand();
                    it->rotC.vy = ex->btlfx_rand();
                    it->rotC.vz = ex->btlfx_rand();
                }
            }
            ex->btlfx_drawLayerTexture((BtlBgLayers *)ex->btlfx_getFileData(entry->file, FX_COMET_HAZE_TEXTURE), frame);
            if (work->countC >= FX_COMET_ROCK_COUNT && frame >= FX_COMET_FRAMES) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_038/nonmatchings/fx_038", fx_038_headerPtr);
