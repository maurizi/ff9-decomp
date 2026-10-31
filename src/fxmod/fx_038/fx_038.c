#include "fxmod/fx_038.h"

static s32 fx_038_op80PoolTask(s32 mode, Fx038Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx038Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR a;
    SVECTOR b;
    SVECTOR d;
    s32 t;
    s32 k;
    s32 k1;
    s32 q;
    s32 v;

    if (mode == 1) {
        it->unk0 = 0;
        it->emit14 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&it->pos4);
        if (it->emit14 != NULL) {
            it->emit14->otZ = 0x10;
        }
        return 0;
    }
    switch (it->unk0) {
    case 0:
        t = (frame << 12) / 15;
        if (frame == 0) {
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 4), 0, NULL, -1, 0x1000, 0, 1,
                              (u8 *)&D_801E857C[it->unk20], 0);
        }
        BTLFX_COPY_SVEC(&a, &it->unk18);
        a.vy -= (0x1000 - t) * 2;
        a.vz += 0x1000 - t;
        BTLFX_COPY_SVEC(&it->pos4, &a);
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
        BTLFX_COPY_SVEC(&b, &it->pos4);
        b.vy += 0x140;
        b.vz -= 0xA0;
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 4), (s32)&b, NULL, 0, 0x6000, ex->math_cos(t >> 2) * 2 / 3 + 0x555, 1,
                          (u8 *)&D_801E857C[it->unk20], 0);
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &it->pos4, NULL, 0x1000, 0, 1, -1, 0, 0);
        env->otZ = 0;
        if (it->unk20 == 0) {
            env->camera->points[0].vx = it->pos4.vx;
            env->camera->points[0].vy = it->pos4.vy;
            env->camera->points[0].vz = it->pos4.vz;
        }
        if (it->emit14 != NULL && it->emit14->state == BTLFX_EMIT_DONE) {
            it->emit14 = NULL;
        }
        if (frame >= 15) {
            it->unk0 = 1;
            ((s16 *)it)[-1] = -1;
            if (it->emit14 != NULL) {
                it->emit14->frame = it->emit14->tail->anim.duration;
            }
            it->pos4.vy = 0;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x18, 0x18, 0x18);
            it->emit14 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&it->pos4);
            if (it->emit14 != NULL) {
                it->emit14->otZ = 0x46;
            }
            it->rotC.vx = 0;
            it->rotC.vy = ex->btlfx_rand();
            it->rotC.vz = 0;
        }
        break;
    case 1:
        t = (frame << 12) / 18;
        if (frame < 4) {
            s32 c = 0x46 - frame * 0x46 / 4;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c, c, c);
        }
        k1 = t >> 2;
        ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 1), &it->pos4, NULL, (t << 1) + 0x1800, 0, 1, ex->math_cos(k1), 0, 0);
        BTLFX_COPY_SVEC(&b, &it->pos4);
        b.vy += 0x140;
        b.vz -= 0xA0;
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 4), (s32)&it->pos4, NULL, 0, 0, 0x555, 1,
                          (u8 *)&D_801E857C[it->unk20], 0);
        it->rotC.vy += ex->math_cos(k1) >> 7;
        pos.vx = it->pos4.vx;
        pos.vy = it->pos4.vy;
        pos.vz = it->pos4.vz;
        if (frame < 8) {
            s32 w;

            t = frame << 9;
            w = t >> 2;
            k = ex->math_cos(w);
            v = (ex->math_sin(w) / 2 + 0x800) / 3;
            scale.vz = v;
            scale.vx = v;
            scale.vy = (v * 9) >> 3;
            ex->Hi_DrawEffModel(&it->rotC, &pos, (s32)&scale, work->model14);
            ex->Hi_ModifyEffModelRGB(work->model14, ((k * 2 - 0x1000) >> 5) - 0x80, ((k * 2 - 0x1000) >> 5) - 0x80,
                                     ((k * 2 - 0x1000) >> 5) - 0x80);
        }
        t = (frame << 12) / 18;
        k = 0x1000 - t;
        q = t >> 2;
        scale.vx = scale.vz = (ex->math_sin(q) / 3 + 0xAAA) * 3 / 4;
        scale.vy = ex->math_cos(q) * 2 / 3 + 0x800;
        ex->Hi_DrawEffModel(&it->rotC, &pos, (s32)&scale, work->model18);
        ex->Hi_ModifyEffModelRGB(work->model18, (k >> 5) - 0x80, (k >> 5) - 0x80, (k >> 5) - 0x80);
        d.pad = 1;
        d.vx = 0x400;
        d.vz = 0;
        d.vy = 0;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xD), &it->pos4, &d, ex->math_sin(q) + 0xAAA, 8, 1, k);
        if (frame < 12) {
            s32 r;

            d.pad = 1;
            d.vx = 0x400;
            d.vz = 0;
            d.vy = 0;
            r = (frame << 12) / 12;
            ex->btlfx_drawParticles((u8 *)ex->btlfx_getFileData(entry->file, 5), &it->pos4, &d, ex->math_cos(r >> 2) * 3, 6, 1, ex->math_cos(r >> 2), 0, 0);
        }
        if (frame >= 18) {
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
        init->workSize = 0x1C;
        init->elemSize = 0x24;
        init->elemFn = (s32)fx_038_op80PoolTask;
        init->poolCount = 3;
    } else if (mode == 1) {
        work->unk0 = 0;
        work->unkC = 0;
        fx_038_env = env;
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk4);
        ex->btlfx_initEffModels(fx_038_effModels, 3);
        work->model10 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 2), 0x97, 0x3C80, 0, 0);
        work->model18 =
            ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xB), 0x137, 0xFFFF, -0x40, 0x80);
        work->model14 = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 0xA));
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            spawn = 0;
            if (frame < 0x18) {
                spawn = D_801E82AC[frame];
            }
            if (spawn != 0 && work->unkC < 3) {
                work->unkC++;
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    it->unk18.vx = work->unk4.vx + ex->btlfx_randCentered(0x258);
                    it->unk18.vz = work->unk4.vz + ex->btlfx_randCentered(0x258);
                    it->unk18.vy = 0;
                    it->unk20 = work->unkC - 1;
                    it->rotC.vx = ex->btlfx_rand();
                    it->rotC.vy = ex->btlfx_rand();
                    it->rotC.vz = ex->btlfx_rand();
                }
            }
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0xC), frame);
            if (work->unkC >= 3 && frame >= 0x3E) {
                ex->btlfx_clearEffModels();
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_038/nonmatchings/fx_038", fx_038_headerPtr);
