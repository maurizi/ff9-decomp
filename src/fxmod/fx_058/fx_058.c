#include "fxmod/fx_058.h"

static s32 fx_058_op80PoolTask(s32 mode, Fx058Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx058Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR ang;
    SVECTOR vec;
    VECTOR pos;
    VECTOR scale;

    if (mode == 1) {
        it->state0 = 0;
        it->speed4 = (ex->btlfx_rand() & 0x1F) + 0x27;
        it->turn8 = ex->btlfx_rand() & 0x1F;
        it->target20.pad = 0x249;
    } else {
        switch (it->state0) {
        case 0:
            if (frame == 0) {
                ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 2), (s32)&it->posC, NULL, -1, 0, 0, 0, it->trail1C,
                                  0);
            }
            it->speed4 += 0x20;
            if (it->speed4 > 0x300) {
                it->speed4 = 0x300;
            }
            it->turn8 += 0x66;
            if (it->turn8 >= 0x1000) {
                it->turn8 = 0x1000;
            }
            /* The empty loop's notes end cse's block here, so the 1 passed to the second drawTrail below is loaded
             * with li instead of reusing the register holding the mode test's 1. */
            do {
            } while (0);
            ex->btlfx_aimAngles((s16 *)&it->posC, (s16 *)&it->target20, (s16 *)&ang);
            ex->btlfx_turnToward((s16 *)&ang, (s16 *)&it->rot14, it->turn8);
            ang.vz = frame << 5;
            ex->btlfx_pointAhead((s16 *)&it->posC, (s16 *)&it->rot14, it->speed4, (s16 *)&it->posC);
            pos.vx = it->posC.vx;
            pos.vy = it->posC.vy;
            pos.vz = it->posC.vz;
            scale.vx = scale.vy = 0x249;
            scale.vz = it->target20.pad;
            it->target20.pad += 0x97;
            it->rot14.vx += 0x800;
            ex->Hi_DrawEffModel(&it->rot14, &pos, (s32)&scale, work->model24);
            it->rot14.vx -= 0x800;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 2), (s32)&it->posC, NULL, 0, 0x800, 0x1000, 1,
                              it->trail1C, 0);
            vec.vx = work->pos4.vx - it->posC.vx;
            vec.vy = work->pos4.vy - it->posC.vy;
            vec.vz = work->pos4.vz - it->posC.vz;
            if (vec.vx * vec.vx + vec.vy * vec.vy + vec.vz * vec.vz < 0x40000 || frame >= 0x11) {
                it->state0 = 1;
                ((s16 *)it)[-1] = -1;
                if (work->frame20 & 1) {
                    ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0x20, 0x10, 0);
                }
            }
            break;
        case 1:
            ang.vx = 0x400;
            ang.vy = 0;
            ang.vz = frame << 5;
            ang.pad = 1;
            BTLFX_COPY_SVEC(&vec, &it->posC);
            vec.vy -= frame * 4;
            env->otZ = 0x40;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 9), &vec, NULL, 0x4000, frame, 1, -1, 0, 0);
            env->otZ = 0;
            if (frame < 0x10) {
                BTLFX_COPY_SVEC(&vec, &it->posC);
                vec.vy = 0;
                ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 8), &vec, &ang, 0x1800, frame, 1, -1, 0, 0);
            }
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 2), (s32)&it->posC, NULL, 0, 0x800, 0x1000, 1,
                              it->trail1C, 0);
            if (frame >= 0x18) {
                return 1;
            }
            break;
        }
    }
    return 0;
}

s32 fx_058_op80(s32 mode, Fx058Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx058Item *it;
    s32 slot;
    s16 baseYaw;

    if (mode == 0) {
        init->workSize = 0x28;
        init->elemSize = 0x28;
        init->elemFn = (s32)fx_058_op80PoolTask;
        init->poolCount = 0x20;
    } else if (mode == 1) {
        work->hold0 = 0;
        work->count1C = 0;
        work->frame20 = 0;
        fx_058_env = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos4);
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->posC);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot14);
        work->rot14.vy += 0x800;
        ex->btlfx_initEffModels(fx_058_effModels, 1);
        work->model24 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xA), 1);
    } else if (work->hold0 == 0) {
        if (work->count1C < 0x20) {
            work->count1C++;
            it = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (it != NULL) {
                BTLFX_COPY_SVEC(&it->posC, &work->posC);
                it->target20.vx = work->pos4.vx;
                it->target20.vz = work->pos4.vz;
                it->target20.vy = work->pos4.vy;
                slot = work->count1C & 3;
                baseYaw = work->rot14.vy - 0x300;
                it->rot14.vy = baseYaw + slot * 0x200;
                slot = work->count1C >> 2;
                it->rot14.vx = work->rot14.vx + slot * 0xFF;
                it->rot14.vz = 0;
                it->trail1C = fx_058_trailBufs[work->count1C];
            }
        }
        work->frame20++;
        if (work->count1C >= 0x20) {
            return 1;
        }
    }
    return 0;
}

s32 fx_058_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 1) {
        fx_058_env = env;
        ex->btlfx_clearEffModels();
    }
    return 1;
}

INCLUDE_RODATA("asm/us/fx_058/nonmatchings/fx_058", fx_058_headerPtr);
