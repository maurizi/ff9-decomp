#include "fxmod/fx_1f0.h"

static void fxm_easeBlend4(s32 mode, s32 len, s32 t, SVECTOR *ctrl, SVECTOR *out) {
    s32 w[4];
    s32 k;
    s32 s;
    s32 r;
    s32 i;
    s32 *wp;

    k = 0x2000000 / (len * len);
    s = 0;
    switch (mode) {
    case 0:
        s = ((0x1000000 / len) * t) >> 16;
        break;
    case 1:
        s = 0x100 - ((k * (len - t) * (len - t)) >> 17);
        break;
    case 2:
        s = (k * t * t) >> 17;
        break;
    }
    r = 0x100 - s;
    w[0] = (s * s * s) >> 12;
    w[1] = (r * s * s * 6) >> 12;
    w[2] = (r * r * s * 6) >> 12;
    w[3] = (r * r * r) >> 12;
    out->vx = 0;
    out->vy = 0;
    out->vz = 0;
    wp = w;
    for (i = 0; i < 4; i++) {
        out->vx += (ctrl[i].vx * *wp) >> 12;
        out->vy += (ctrl[i].vy * *wp) >> 12;
        out->vz += (ctrl[i].vz * *wp) >> 12;
        wp++;
    }
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1f0_env);
    s32 k = 0x20000000 / (n * n);
    s32 r = 0;

    switch (curve) {
    case 0:
        r = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        r = (k * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        r = 0x1000 - ((k * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, r);
}

s32 fx_1f0_op80(s32 mode, Fx1f0Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    CVECTOR col;
    SVECTOR rot;
    VECTOR scale;
    Fx1f0Unit *u;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 t;
    s32 v;
    s32 n;

    if (mode == 0) {
        *args = 0xB8;
    } else if (mode == 1) {
        fx_1f0_env = env;
        ex->btlfx_initEffModels(fx_1f0_effModels, 4);
        for (i = 15; i >= 0; i--) {
            work->timer0[i] = 0;
        }
        work->model60 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 3), 1);
        work->model64 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 5), 1);
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 6), 1);
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 7), 1);
        work->scale80.vx = 0x19B0;
        work->scale80.vy = 0x19B0;
        work->scale80.vz = 0x19B0;
        work->scale90.vx = 0x1400;
        work->scale90.vy = 0x1400;
        work->scale90.vz = 0x1400;
        work->pos70.vx = 0;
        work->pos70.vy = 0;
        work->pos70.vz = 0;
        work->rotA0.vx = 0;
        work->rotA0.vy = 0;
        work->rotA0.vz = 0;
        work->rotA8.vx = 0;
        work->rotA8.vy = 0;
        work->rotA8.vz = 0;
        work->spinB0 = 0;
        work->spinB4 = 0;
        for (i = 0; i < 4; i++) {
            fx_1f0_units[i].delay0 = -1;
            fx_1f0_units[i].timer4 = 0x80;
            fx_1f0_units[i].emit28 = NULL;
        }
        for (i = 0; i < env->targets->targetCount; i++) {
            u = &fx_1f0_units[i];
            u->emit28 = NULL;
            ex->btlfx_fadeUnitById(i, -2);
            ex->btlfx_getUnitHomePos(i, (s16 *)&fx_1f0_units[i].homePos10);
            u->startPos8.vx = u->homePos10.vx;
            u->startPos8.vy = u->homePos10.vy + 0x3000;
            u->startPos8.vz = u->homePos10.vz - 0x1000;
            u->emitPos18.vx = u->pos20.vx = u->startPos8.vx;
            u->emitPos18.vy = u->pos20.vy = u->startPos8.vy;
            u->emitPos18.vz = u->pos20.vz = u->startPos8.vz;
            ex->btlfx_setUnitPos(i, (s16 *)&fx_1f0_units[i].startPos8);
            u->delay0 = fx_1f0_unitDelays[i];
        }
        fx_1f0_units[0].pos20.vy -= 0x100;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->timer0[0] = 0x32;
            break;
        case 0x32:
            work->timer0[1] = 0xA0;
            break;
        case 0xC0:
            work->timer0[7] = 0x18;
            work->timer0[8] = 0x18;
            work->timer0[6] = 0x18;
            break;
        case 0xC8:
            work->timer0[9] = 0x20;
            break;
        }
        if (work->timer0[0] != 0) {
            work->timer0[0]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                ex->btlfx_setUnitPos(i, (s16 *)&fx_1f0_units[i].startPos8);
            }
        }
        if (work->timer0[1] != 0) {
            work->timer0[1]--;
            for (i = 0; i < env->targets->targetCount; i++) {
                if (fx_1f0_units[i].emit28 != NULL && fx_1f0_units[i].emit28->state == BTLFX_EMIT_DONE) {
                    fx_1f0_units[i].emit28 = NULL;
                }
                if (fx_1f0_units[i].delay0 == 0) {
                    if (fx_1f0_units[i].timer4 != 0) {
                        fx_1f0_units[i].timer4--;
                        work->ctrlPts40[0].vx = fx_1f0_units[i].startPos8.vx;
                        work->ctrlPts40[0].vy = fx_1f0_units[i].startPos8.vy;
                        work->ctrlPts40[0].vz = fx_1f0_units[i].startPos8.vz;
                        work->ctrlPts40[1].vx = (fx_1f0_units[i].startPos8.vx + fx_1f0_units[i].homePos10.vx) >> 1;
                        work->ctrlPts40[1].vy = (fx_1f0_units[i].startPos8.vy + fx_1f0_units[i].homePos10.vy) >> 1;
                        work->ctrlPts40[1].vz = fx_1f0_units[i].homePos10.vz;
                        work->ctrlPts40[2].vx = fx_1f0_units[i].homePos10.vx;
                        work->ctrlPts40[2].vy = fx_1f0_units[i].homePos10.vy - 0x400;
                        work->ctrlPts40[2].vz = fx_1f0_units[i].homePos10.vz;
                        work->ctrlPts40[3].vx = fx_1f0_units[i].homePos10.vx;
                        work->ctrlPts40[3].vy = fx_1f0_units[i].homePos10.vy;
                        work->ctrlPts40[3].vz = fx_1f0_units[i].homePos10.vz;
                        fxm_easeBlend4(0, 0x80, fx_1f0_units[i].timer4, work->ctrlPts40, &fx_1f0_units[i].pos20);
                        ex->btlfx_moveEmitterTo((s32)fx_1f0_units[i].emit28, (s32)&fx_1f0_units[i].pos20);
                        if (fx_1f0_units[i].timer4 < 0x10 && fx_1f0_units[i].emit28 != NULL) {
                            fx_1f0_units[i].emit28->alpha = fx_1f0_units[i].timer4 << 8;
                        }
                        ex->btlfx_setUnitPos(i, (s16 *)&fx_1f0_units[i].pos20);
                        fx_1f0_units[i].pos20.vy -= 0x100;
                    } else {
                        fx_1f0_units[i].delay0 = -1;
                        ex->btlfx_getUnitHomePos(i, (s16 *)&fx_1f0_units[i].pos20);
                        ex->btlfx_setUnitPos(i, (s16 *)&fx_1f0_units[i].pos20);
                        ex->btlfx_stopEmitter(fx_1f0_units[i].emit28);
                    }
                }
                if (fx_1f0_units[i].delay0 > 0) {
                    ex->btlfx_setUnitPos(i, (s16 *)&fx_1f0_units[i].startPos8);
                    if (fx_1f0_units[i].delay0 == 8) {
                        fx_1f0_units[i].emit28 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&fx_1f0_units[i].emitPos18);
                        if (fx_1f0_units[i].emit28 != NULL) {
                            fx_1f0_units[i].emit28->otZ = 0x80;
                        }
                        emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&fx_1f0_units[i].pos20);
                        if (emitter != NULL) {
                            emitter->otZ = 0x80;
                        }
                    }
                    if (fx_1f0_units[i].delay0 < 8 && fx_1f0_units[i].emit28 != NULL) {
                        fx_1f0_units[i].emit28->alpha = (8 - fx_1f0_units[i].delay0) << 9;
                    }
                    fx_1f0_units[i].delay0--;
                }
            }
        }
        env->camera->points[0].vx = 0;
        env->camera->points[0].vy = fx_1f0_units[0].pos20.vy;
        env->camera->points[0].vz = fx_1f0_units[0].pos20.vz;
        work->rotA0.vy = work->rotA0.vy + work->spinB0 + 0x20;
        work->rotA8.vy = work->rotA8.vy - work->spinB4 - 0x20;
        if (work->timer0[4] | work->timer0[5] | work->timer0[6]) {
            ex->btlfx_drawScrollMove(frame, 0x200, 0x100, 0x40, 0x80, 0x200, 0x180);
            ex->btlfx_drawScrollMove(frame * 2, 0x1C0, 0x120, 0x38, 0x70, 0x1C0, 0x190);
        }
        if (work->timer0[6] != 0) {
            work->timer0[6]--;
            t = fxm_easeLerp(0, 0x18, work->timer0[6], 0, 0x1000);
            ex->Hi_DrawMorphEffModel(&work->rotA0, &work->pos70, (s32)&work->scale80, work->model64, work->model60, t);
            ex->Hi_DrawMorphEffModel(&work->rotA8, &work->pos70, (s32)&work->scale90, work->model6C, work->model68, t);
            if (work->timer0[6] > 0x10) {
                v = -((work->timer0[6] - 0x10) << 4);
                ex->Hi_ModifyEffModelRGB(work->model64, v, v, v);
                ex->Hi_ModifyEffModelRGB(work->model6C, v, v, v);
            }
            if (work->timer0[6] < 8) {
                v = -((8 - work->timer0[6]) << 4);
                ex->Hi_ModifyEffModelRGB(work->model64, v, v, v);
                ex->Hi_ModifyEffModelRGB(work->model6C, v, v, v);
            }
        }
        if (work->timer0[7] != 0) {
            work->timer0[7]--;
            work->scale80.vx = work->scale80.vy = work->scale80.vz = fxm_easeLerp(1, 0x18, work->timer0[7], 0x19B0, 0x3000);
            work->scale90.vx = work->scale90.vy = work->scale90.vz = fxm_easeLerp(1, 0x18, work->timer0[7], 0x1400, 0x2C00);
        }
        if (work->timer0[8] != 0) {
            work->timer0[8]--;
            work->spinB0 = fxm_easeLerp(1, 0x18, work->timer0[8], 0, 0x80);
            work->spinB4 = fxm_easeLerp(1, 0x18, work->timer0[8], 0, 0x80);
        }
        if (work->timer0[9] != 0) {
            n = work->timer0[9] - 1;
            work->timer0[9] = n;
            scale.vz = scale.vy = scale.vx = (n << 2) + 0x1000;
            col.r = col.g = col.b = n * 3;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->btlfx_drawScreenGrid(3, 0xA0, 0x78, 0xA0, 0x78, 2, 2, 1, 1, &rot, &scale, &col, 1, 0);
        }
        return frame >= 0x108;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f0/nonmatchings/fx_1f0", fx_1f0_headerPtr);
