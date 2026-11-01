#include "fxmod/fx_0db.h"

static s32 fx_0db_op80PoolTask(s32 mode, Fx0dbItem *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx0dbWork80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR d;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *r;
    Pool6CEntry *emitter;
    s32 a;
    s32 k;
    s32 c;
    s32 t;
    s32 fade;

    if (mode == 1) {
        it->state4 = 0;
        it->frame8 = 0;
        emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos14);
        it->emit40 = emitter;
        if (emitter != NULL) {
            emitter->otZ = 0x40;
        }
        return 0;
    }
    fade = 0;
    r = &fx_0db_shardAngles[it->shard0];
    switch (it->state4) {
    case 0:
        t = it->frame8 << 10;
        a = t >> 2;
        scale.vx = scale.vy = ((0x1000 - ex->math_cos(a)) * (it->size3C * 5 / 2)) >> 12;
        scale.vz = (ex->math_sin(a) * it->size3C) >> 12;
        BTLFX_COPY_SVEC(&rot, r);
        rot.vx -= 0x800;
        fade = 0x1000;
        if (it->emit40 != NULL) {
            if (it->emit40->state == BTLFX_EMIT_DONE) {
                it->emit40 = NULL;
            }
            if (it->emit40 != NULL) {
                it->emit40->rot.vx = r->vx;
                it->emit40->rot.vy = r->vy;
                it->emit40->rot.vz = r->vz;
            }
        }
        it->frame8++;
        if (it->frame8 >= 4) {
            it->state4 = 1;
            it->frame8 = 0;
        }
        break;
    case 1:
        scale.vx = scale.vy = it->size3C * 5 / 2;
        scale.vz = it->size3C;
        BTLFX_COPY_SVEC(&rot, r);
        rot.vx -= 0x800;
        fade = 0x1000;
        if (it->emit40 != NULL) {
            if (it->emit40->state == BTLFX_EMIT_DONE) {
                it->emit40 = NULL;
            }
            if (it->emit40 != NULL) {
                it->emit40->rot.vx = rot.vx;
                it->emit40->rot.vy = rot.vy;
                it->emit40->rot.vz = rot.vz;
            }
        }
        if (work->split24 != 0) {
            it->state4 = 2;
            it->frame8 = 0;
            it->spinX38 += -25 + ex->btlfx_rand() % 50;
            it->spinY3A += -16 + (ex->btlfx_rand() & 0x1F);
            BTLFX_COPY_SVEC(&it->rot30, r);
            it->posC[1].vx = it->posC[0].vx;
            it->posC[1].vy = it->posC[0].vy;
            it->posC[1].vz = it->posC[0].vz;
            it->posC[2].vx = it->posC[0].vx;
            it->posC[2].vy = it->posC[0].vy;
            it->posC[2].vz = it->posC[0].vz;
        }
        break;
    case 2:
        t = (it->frame8 << 12) / 20;
        a = t >> 2;
        scale.vx = scale.vy = (ex->math_cos(a) * (it->size3C * 5 / 2)) >> 12;
        scale.vz = it->size3C;
        fade = ex->math_cos(a);
        k = ex->math_cos(a) >> 7;
        ex->btlfx_rotateOffset(r, k, (s32)&d);
        it->posC[0].vx -= d.vx;
        it->posC[0].vy -= d.vy - (it->frame8 - 6);
        it->posC[0].vz -= d.vz;
        ex->btlfx_rotateOffset(r, k * 2, (s32)&d);
        it->posC[1].vx -= d.vx;
        it->posC[1].vy -= d.vy - (it->frame8 - 6) * 4;
        it->posC[1].vz -= d.vz;
        ex->btlfx_rotateOffset(r, k * 2 + k, (s32)&d);
        r = &it->rot30;
        it->posC[2].vx -= d.vx;
        it->posC[2].vy -= d.vy - (it->frame8 - 6) * 8;
        it->posC[2].vz -= d.vz;
        it->rot30.vx += it->spinX38;
        it->rot30.vy += it->spinY3A;
        it->frame8++;
        if (it->frame8 >= 20) {
            return 1;
        }
        break;
    }
    scale.vx >>= 1;
    scale.vy >>= 1;
    if (it->state4 < 2) {
        ex->Hi_SetEffModelOffset(work->model4, 2);
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->posC[0], (s32)&scale, work->model4);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->model4, 1);
            c = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->model4, 0);
            c = fade * 2;
            c += fade;
            c >>= 6;
        }
        c -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model4, c, c, c);
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->posC[0], (s32)&scale, work->model10);
    } else {
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->posC[0], (s32)&scale, work->model10);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->model10, 1);
            c = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->model10, 0);
            c = fade * 2;
            c += fade;
            c >>= 6;
        }
        c -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model10, c, c, c);
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->posC[1], (s32)&scale, work->modelC);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->modelC, 1);
            c = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->modelC, 0);
            c = fade * 2;
            c += fade;
            c >>= 6;
        }
        c -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->modelC, c, c, c);
        ex->Hi_DrawEffModel(r, (VECTOR *)&it->posC[2], (s32)&scale, work->model8);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->model8, 1);
            c = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->model8, 0);
            c = fade * 2;
            c += fade;
            c >>= 6;
        }
        c -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model8, c, c, c);
    }
    return 0;
}

s32 fx_0db_op80(s32 mode, Fx0dbWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    Fx0dbItem *part;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 c;
    s16 *a;
    s16 *b;

    if (mode == 0) {
        ((EffInit *)args)->workSize = 0x34;
        ((EffInit *)args)->elemSize = 0x44;
        ((EffInit *)args)->elemFn = (s32)fx_0db_op80PoolTask;
        ((EffInit *)args)->poolCount = 0xA;
    } else if (mode == 1) {
        fx_0db_env = env;
        ex->btlfx_initEffModels(fx_0db_effModels, 6);
        work->model4 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x13), 0x9A, 0x3DC0, 0x80, 0);
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x15), 0x9A, 0x3DC0, 0x80, 0);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x16), 0x9A, 0x3DC0, 0x80, 0);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x17), 0x9A, 0x3DC0, 0x80, 0);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xB), 0xBA, 0x3D40, 0, 0x80);
        work->state0 = 0;
        work->spawnCount26 = 0;
        work->frame28 = 0;
        work->split24 = 0;
        work->scroll2C = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos14);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos1C);
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos14);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x46);
        }
        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->pos14);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x46);
        }
        a = &fx_0db_shardAngles[0].vx;
        for (i = 0, b = a + 1; i < 10; i++) {
            *a += 0x5C;
            a += 4;
            *b += 0x200;
            b += 4;
        }
        return 0;
    } else {
        frame = entry->frame;
        switch (work->state0) {
        case 0:
            part = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (part != NULL) {
                part->shard0 = work->spawnCount26;
                work->spawnCount26++;
                ex->btlfx_pointAhead((s16 *)&work->pos14, (s16 *)&fx_0db_shardAngles[part->shard0], -0x10, (s16 *)&v);
                part->posC[0].vx = v.vx;
                part->posC[0].vy = v.vy;
                part->posC[0].vz = v.vz;
                part->size3C = fx_0db_shardAngles[part->shard0].pad * 3;
            }
            work->scroll2C += 0x20;
            if (frame >= 9) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            work->scroll2C += (ex->math_cos((frame << 10) / 14) << 5) >> 12;
            if (frame >= 0xE) {
                work->state0 = 2;
                entry->frame = -1;
                work->split24 = 1;
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            break;
        case 2:
            if (frame < 5) {
                c = (4 - frame) * 0xFA / 4;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, c >> 2, c >> 1, c);
            }
            if (frame == 0) {
                emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&work->pos14);
                if (emitter != NULL) {
                    emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
                }
            }
            if (frame >= 0x15) {
                ex->btlfx_clearEffModels();
                return 1;
            }
            break;
        }
        ex->btlfx_drawScrollMove(work->scroll2C, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
        work->frame28++;
    }
    return 0;
}

s32 fx_0db_op84(s32 mode, Fx0dbWork84 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    s32 frame;
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 u;
    s32 k;
    s32 v;
    u16 ry;

    if (mode == 0) {
        *args = 0x10;
    } else if (mode == 1) {
        fx_0db_env = env;
        work->state0 = 0;
        work->frame4 = 0;
    } else {
        a = 0;
        b = 0;
        c = 0;
        frame = entry->frame;
        ex->btlfx_getWeaponPos(0x10, 1, (s16 *)&work->pos8);
        ry = 0;
        switch (work->state0) {
        case 0:
            ry = 0x200;
            a = (frame << 12) / 3;
            u = ex->math_sin(a >> 2);
            b = u >> 1;
            c = ((0x1000 - a) >> 1) + 0x800;
            if (frame >= 3) {
                work->state0 = 1;
                entry->frame = -1;
            }
            break;
        case 1:
            u = (frame << 12) / 9;
            k = (frame & 1) << 8;
            c = k + 0x1000;
            v = (u >> 1) + 0x800;
            a = k + v;
            ry = (u >> 3) + 0x200;
            b = c;
            if (frame >= 9) {
                work->state0 = 2;
                entry->frame = -1;
            }
            break;
        case 2:
            u = (frame << 12) / 9;
            t = u >> 2;
            ry = 0x400 - t;
            a = ex->math_cos(t);
            b = ex->math_sin(t) + 0x1000;
            c = ex->math_sin(t) * 3 / 2 + 0x1000;
            if (frame >= 9) {
                return 1;
            }
            break;
        }
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = ry;
        rot.pad = 0;
        env->otZ = 0x80;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1B), &work->pos8, NULL, b / 3, 0, 1, a / 2);
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), &work->pos8, &rot, c / 2, 0, 1, a * 2 / 3);
        env->otZ = 0;
        work->frame4++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_0db/nonmatchings/fx_0db", fx_0db_headerPtr);
