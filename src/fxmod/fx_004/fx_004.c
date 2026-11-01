#include "fxmod/fx_004.h"

static s32 fx_004_op80PoolTask(s32 mode, Fx004Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx004Work *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR offset;
    SVECTOR rot;
    VECTOR scale;
    SVECTOR *pieceRot;
    Pool6CEntry *emitter;
    s32 ang;
    s32 step;
    s32 bright;
    s32 t;
    s32 fade;

    if (mode == 1) {
        it->state4 = 0;
        it->frame8 = 0;
        emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos18);
        it->emit40 = emitter;
        if (emitter != NULL) {
            emitter->otZ = 0x40;
        }
        return 0;
    }
    fade = 0;
    pieceRot = &fx_004_pieceRots[it->piece0];
    switch (it->state4) {
    case 0:
        t = it->frame8 << 10;
        ang = t >> 2;
        scale.vx = scale.vy = ((0x1000 - ex->math_cos(ang)) * (it->size3C * 5 / 2)) >> 12;
        scale.vz = (ex->math_sin(ang) * it->size3C) >> 12;
        BTLFX_COPY_SVEC(&rot, pieceRot);
        rot.vx -= 0x800;
        fade = 0x1000;
        if (it->emit40 != NULL) {
            if (it->emit40->state == BTLFX_EMIT_DONE) {
                it->emit40 = NULL;
            }
            if (it->emit40 != NULL) {
                it->emit40->rot.vx = pieceRot->vx;
                it->emit40->rot.vy = pieceRot->vy;
                it->emit40->rot.vz = pieceRot->vz;
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
        BTLFX_COPY_SVEC(&rot, pieceRot);
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
        if (work->split28 != 0) {
            it->state4 = 2;
            it->frame8 = 0;
            it->spin38 += -25 + ex->btlfx_rand() % 50;
            it->spin3A += -16 + (ex->btlfx_rand() & 0x1F);
            BTLFX_COPY_SVEC(&it->rot30, pieceRot);
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
        ang = t >> 2;
        scale.vx = scale.vy = (ex->math_cos(ang) * (it->size3C * 5 / 2)) >> 12;
        scale.vz = it->size3C;
        fade = ex->math_cos(ang);
        step = ex->math_cos(ang) >> 7;
        ex->btlfx_rotateOffset(pieceRot, step, (s32)&offset);
        it->posC[0].vx -= offset.vx;
        it->posC[0].vy -= offset.vy - (it->frame8 - 6);
        it->posC[0].vz -= offset.vz;
        ex->btlfx_rotateOffset(pieceRot, step * 2, (s32)&offset);
        it->posC[1].vx -= offset.vx;
        it->posC[1].vy -= offset.vy - (it->frame8 - 6) * 4;
        it->posC[1].vz -= offset.vz;
        ex->btlfx_rotateOffset(pieceRot, step * 2 + step, (s32)&offset);
        pieceRot = &it->rot30;
        it->posC[2].vx -= offset.vx;
        it->posC[2].vy -= offset.vy - (it->frame8 - 6) * 8;
        it->posC[2].vz -= offset.vz;
        it->rot30.vx += it->spin38;
        it->rot30.vy += it->spin3A;
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
        ex->Hi_DrawEffModel(pieceRot, (VECTOR *)&it->posC[0], (s32)&scale, work->model4);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->model4, 1);
            bright = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->model4, 0);
            bright = fade * 2;
            bright += fade;
            bright >>= 6;
        }
        bright -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model4, bright, bright, bright);
        ex->Hi_DrawEffModel(pieceRot, (VECTOR *)&it->posC[0], (s32)&scale, work->model14);
    } else {
        ex->Hi_DrawEffModel(pieceRot, (VECTOR *)&it->posC[0], (s32)&scale, work->model10);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->model10, 1);
            bright = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->model10, 0);
            bright = fade * 2;
            bright += fade;
            bright >>= 6;
        }
        bright -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model10, bright, bright, bright);
        ex->Hi_DrawEffModel(pieceRot, (VECTOR *)&it->posC[1], (s32)&scale, work->modelC);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->modelC, 1);
            bright = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->modelC, 0);
            bright = fade * 2;
            bright += fade;
            bright >>= 6;
        }
        bright -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->modelC, bright, bright, bright);
        ex->Hi_DrawEffModel(pieceRot, (VECTOR *)&it->posC[2], (s32)&scale, work->model8);
        if (fade < 4000) {
            ex->Hi_ModifyEffModelAbr(work->model8, 1);
            bright = fade >> 5;
        } else {
            ex->Hi_ModifyEffModelAbr(work->model8, 0);
            bright = fade * 2;
            bright += fade;
            bright >>= 6;
        }
        bright -= 0x80;
        ex->Hi_ModifyEffModelRGB(work->model8, bright, bright, bright);
    }
    return 0;
}

s32 fx_004_op80(s32 mode, Fx004Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR startPos;
    Fx004Item *it;
    Pool6CEntry *emitter;
    SVECTOR *pieceRot;
    s32 frame;
    s32 state;
    s32 i;
    s32 tint;

    if (mode == 0) {
        init->workSize = 0x38;
        init->elemSize = 0x44;
        init->elemFn = (s32)fx_004_op80PoolTask;
        init->poolCount = 0xA;
    } else if (mode == 1) {
        fx_004_env = env;
        ex->btlfx_initEffModels(fx_004_effModels, 6);
        work->model4 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x13), 0x9A, 0x3DC0, 0x80, 0);
        work->model8 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x15), 0x9A, 0x3DC0, 0x80, 0);
        work->modelC = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x16), 0x9A, 0x3DC0, 0x80, 0);
        work->model10 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0x17), 0x9A, 0x3DC0, 0x80, 0);
        work->model14 = (s16)ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xB), 0xBA, 0x3D40, 0, 0x80);
        work->state0 = 0;
        work->count2A = 0;
        work->frameCount2C = 0;
        work->split28 = 0;
        work->scroll30 = 0;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos18);
        ex->btlfx_getUnitPos(0, (s16 *)&work->pos20);
        emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos18);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x46);
        }
        emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->pos18);
        if (emitter != NULL) {
            emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x46);
        }
        pieceRot = fx_004_pieceRots;
        for (i = 0; i < 10; i++, pieceRot++) {
            pieceRot->vx += 0x5C;
            ((s16 *)pieceRot)[1] += 0x200;
        }
        return 0;
    } else {
    state = work->state0;
    frame = entry->frame;
    switch (state) {
    case 0:
        it = ex->btlfx_poolSpawn((s16 *)entry->pool);
        if (it != NULL) {
            it->piece0 = work->count2A;
            work->count2A++;
            ex->btlfx_pointAhead((s16 *)&work->pos18, (s16 *)&fx_004_pieceRots[it->piece0], -0x10, (s16 *)&startPos);
            it->posC[0].vx = startPos.vx;
            it->posC[0].vy = startPos.vy;
            it->posC[0].vz = startPos.vz;
            it->size3C = fx_004_pieceRots[it->piece0].pad * 3;
        }
        work->scroll30 += 0x20;
        if (frame >= 9) {
            work->state0 = 1;
            entry->frame = -1;
        }
        break;
    case 1:
        work->scroll30 += (ex->math_cos((frame << 10) / 14) << 5) >> 12;
        if (frame >= 14) {
            work->state0 = 2;
            entry->frame = -1;
            work->split28 = state;
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos18);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos18);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos18);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos18);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        break;
    case 2:
        if (frame < 5) {
            tint = (4 - frame) * 250 / 4;
            ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, tint >> 2, tint >> 1, tint);
        }
        if (frame == 0) {
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&work->pos18);
            if (emitter != NULL) {
                emitter->otZ = ex->btlfx_addRadiusSixth(0, 0x20);
            }
        }
        if (frame >= 21) {
            ex->btlfx_clearEffModels();
            return 1;
        }
        break;
    }
    ex->btlfx_drawScrollMove(work->scroll30, 0x280, 0x180, 0x40, 0x80, 0x280, 0x100);
    work->frameCount2C++;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_004/nonmatchings/fx_004", fx_004_headerPtr);
