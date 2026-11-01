#include "fxmod/fx_1f3.h"

static void fxm_tintScreenRatio(s32 t, s32 n, s32 from, s32 to, s32 abr, s32 ot) {
    Unk800F78A4 *env = fx_1f3_env;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 c = ex->btlfx_lerpRatio(from, to, t, n);

    ex->btlfx_drawScreenTint((u32 *)(env->ot + ot), abr, c, c, c);
}

static s32 fxm_easeLerp(s32 curve, s32 n, s32 t, s32 from, s32 to) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1f3_env);
    s32 invSq = 0x20000000 / (n * n);
    s32 ratio = 0;

    switch (curve) {
    case 0:
        ratio = 0x1000 - (((0x10000000 / n) * t) >> 16);
        break;
    case 1:
        ratio = (invSq * (n - t) * (n - t)) >> 17;
        break;
    case 2:
        ratio = 0x1000 - ((invSq * t * t) >> 17);
        break;
    }
    return ex->btlfx_lerp(from, to, ratio);
}

s32 fx_1f3_op80(s32 mode, Fx1f3Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    VECTOR out;
    SVECTOR cam;
    SVECTOR cam2;
    s32 flag;
    Pool6CEntry *emitter;
    Fx1f3Ring *g;
    Fx1f3RingDef *def;
    SVECTOR *dst;
    Fx1f3Line *l;
    s32 n1;
    s32 n2;
    s32 frame;
    s32 i;
    s32 j;
    s32 c;
    s32 a;
    s32 b;
    s32 *q;

    if (mode == 0) {
        *args = 0x120;
    } else if (mode == 1) {
        work->emit40[0] = NULL;
        work->emit40[1] = NULL;
        work->emit40[2] = NULL;
        work->emit40[3] = NULL;
        work->emit40[4] = NULL;
        work->emit40[5] = NULL;
        work->emit40[6] = NULL;
        fx_1f3_env = env;
        ex->btlfx_initEffModels(fx_1f3_effModels, 0x20);
        for (i = 15, q = &work->timer0[15]; i >= 0; i--, q--) {
            *q = 0;
        }
        work->pos5C.vx = env->targets->centerX;
        work->pos5C.vz = env->targets->centerZ;
        work->pos5C.vy = -0x200;
        for (i = 0; i < 16; i++) {
            fx_1f3_rings[i].timer12C = 0;
            fx_1f3_rings[i].bone130 = fx_1f3_ringDefs[i].bone0;
            fx_1f3_rings[i].radius13C = fx_1f3_ringDefs[i].radius4;
            fx_1f3_rings[i].angle134 = ex->btlfx_rand() & 0xFFF;
            fx_1f3_rings[i].spin138 = (ex->btlfx_rand() & 0xFF) + 0x40;
            fx_1f3_rings[i].angleStep140 = 0x40;
            ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x11), 0, NULL, -1, 0x1000, 0x1000, 1,
                                (u8 *)&fx_1f3_rings[i].trail20, 2);
        }
        for (i = 0, b = 0x600, a = 0x400; i < 8; i++, b += 0x200, a += 0x200) {
            fx_1f3_spinModels[i].model0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD), 1);
            fx_1f3_spinModels[i].scale10.vy = fx_1f3_spinModels[i].scale10.vx = fx_1f3_spinModels[i].scale10.vz = ex->btlfx_randRange(a, b);
            fx_1f3_spinModels[i].pos20.vx = work->pos5C.vx;
            fx_1f3_spinModels[i].pos20.vz = work->pos5C.vz;
            fx_1f3_spinModels[i].pos20.vy = work->pos5C.vy + ex->btlfx_randRange(-0x200, 0x200);
            fx_1f3_spinModels[i].rot8.vy = (i << 10) + (ex->btlfx_rand() & 0x3FF);
            fx_1f3_spinModels[i].rot8.vz = 0;
            fx_1f3_spinModels[i].rot8.vx = -0x400;
            fx_1f3_spinModels[i].spin4 = (ex->btlfx_rand() & 0x1FF) + 0x100;
        }
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 1), 1);
        work->pos70.vx = env->targets->centerX;
        work->pos70.vz = env->targets->centerZ;
        work->pos70.vy = -0x200;
        work->rot90.vx = 0;
        work->rot90.vy = 0;
        work->rot90.vz = 0;
        work->scale80.vz = work->scale80.vy = work->scale80.vx = 0x3000;
        work->model98 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        work->model9C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 9), 1);
        work->rotA0.vx = 0;
        work->rotA0.vy = 0;
        work->rotA0.vz = 0;
        work->rotA8.vx = 0x800;
        work->rotA8.vy = 0;
        work->rotA8.vz = 0;
        work->scaleB0.vz = work->scaleB0.vy = work->scaleB0.vx = 0x400;
        work->modelC0 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 8), 1);
        work->modelC4 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 8), 1);
        work->rotC8.vx = 0;
        work->rotC8.vy = 0;
        work->rotC8.vz = 0;
        work->rotD0.vx = 0;
        work->rotD0.vy = 0;
        work->rotD0.vz = 0;
        work->scaleD8.vz = work->scaleD8.vy = work->scaleD8.vx = 0x400;
        work->modelE8 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x13), 1);
        work->modelEC = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x14), 1);
        work->rotF0.vx = 0;
        work->rotF0.vy = 0;
        work->rotF0.vz = 0;
        work->scaleF8.vz = work->scaleF8.vy = work->scaleF8.vx = 0x400;
        ex->btlfx_getUnitHomePos(0x10, (s16 *)&work->pos108);
        ex->btlfx_getUnitHomePos(0x10, (s16 *)&work->pos110);
        ex->btlfx_getUnitHomePos(0x10, (s16 *)&work->pos118);
        work->pos110.vy -= 0x400;
        work->pos118.vy -= 0x800;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->timer0[7] = 0x4A;
            work->timer0[9] = 0x20;
            break;
        case 0x22:
            ex->btlfx_getUnitBonePos(0x10, 0x2E, (u16 *)&work->pos64);
            work->emit40[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos64);
            if (work->emit40[0] != NULL) {
                work->emit40[0]->otZ = 0x200;
            }
            break;
        case 0x2A:
            emitter = work->emit40[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos64);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0x52:
            emitter = work->emit40[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0x62:
            work->emit40[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos5C);
            if (work->emit40[2] != NULL) {
                work->emit40[2]->trails = (s32)fx_1f3_op80Trails;
                work->emit40[2]->trailSlots = 0x24;
                work->emit40[2]->trailStride = 0xCC;
                if (work->emit40[2] != NULL) {
                    work->emit40[2]->otZ = 0x200;
                }
            }
            emitter = work->emit40[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0x72:
            work->timer0[0] = 0x80;
            break;
        case 0x7E:
            emitter = work->emit40[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0x92:
            ex->btlfx_stopEmitter(work->emit40[6]);
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            work->timer0[1] = 0x10;
            break;
        case 0x9A:
            work->timer0[2] = 4;
            break;
        case 0x9D:
            work->timer0[0] = 0;
            ex->btlfx_stopEmitter(work->emit40[2]);
            ex->btlfx_stopEmitter(work->emit40[3]);
            ex->btlfx_stopEmitter(work->emit40[4]);
            ex->btlfx_stopEmitter(work->emit40[6]);
            break;
        case 0x9E:
            work->timer0[3] = 8;
            work->timer0[5] = 0x20;
            work->timer0[6] = 0x20;
            work->timer0[8] = 0x20;
            work->timer0[4] = 0x60;
            emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos5C);
            if (emitter != NULL) {
                emitter->otZ = 0x200;
            }
            break;
        case 0xBE:
            work->timer0[10] = 0x20;
            break;
        }
        if (work->emit40[0] != NULL && work->emit40[0]->state == BTLFX_EMIT_DONE) {
            work->emit40[0] = NULL;
        }
        if (work->emit40[1] != NULL && work->emit40[1]->state == BTLFX_EMIT_DONE) {
            work->emit40[1] = NULL;
        }
        if (work->emit40[2] != NULL && work->emit40[2]->state == BTLFX_EMIT_DONE) {
            work->emit40[2] = NULL;
        }
        if (work->emit40[3] != NULL && work->emit40[3]->state == BTLFX_EMIT_DONE) {
            work->emit40[3] = NULL;
        }
        if (work->emit40[4] != NULL && work->emit40[4]->state == BTLFX_EMIT_DONE) {
            work->emit40[4] = NULL;
        }
        if (work->emit40[5] != NULL && work->emit40[5]->state == BTLFX_EMIT_DONE) {
            work->emit40[5] = NULL;
        }
        if (work->emit40[6] != NULL && work->emit40[6]->state == BTLFX_EMIT_DONE) {
            work->emit40[6] = NULL;
        }
        if (work->emit40[1] != NULL) {
            ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x10), frame);
        }
        if (work->timer0[1] != 0) {
            work->timer0[1]--;
            work->scale80.vx = work->scale80.vy = work->scale80.vz = fxm_easeLerp(2, 0x10, work->timer0[1], 0x3000, 0);
        }
        if (work->timer0[0] != 0) {
            work->timer0[0]--;
            ex->btlfx_aimAngles((s16 *)&env->camera->eye, (s16 *)&work->pos5C, (s16 *)&work->rot90);
            ex->Hi_DrawEffModel(&work->rot90, (VECTOR *)&work->pos70, (s32)&work->scale80, work->model6C);
            n1 = 0x80 - work->timer0[0];
            if (n1 < 8) {
                c = (n1 << 5) - 0x100;
                ex->Hi_ModifyEffModelRGB(work->model6C, c, c, c);
            }
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, 0x240, 0x100, 0x40, 0x80, 0x240, 0x180);
        }
        if (work->timer0[5] != 0) {
            work->timer0[5]--;
            work->scaleB0.vx = work->scaleB0.vy = work->scaleB0.vz = fxm_easeLerp(2, 0x20, work->timer0[5], 0x100, 0x400);
            ex->Hi_DrawEffModel(&work->rotA0, (VECTOR *)&work->pos70, (s32)&work->scaleB0, work->model98);
            ex->Hi_DrawEffModel(&work->rotA8, (VECTOR *)&work->pos70, (s32)&work->scaleB0, work->model9C);
            if (work->timer0[5] < 0x10) {
                c = -((0x10 - work->timer0[5]) * 0x10);
                ex->Hi_ModifyEffModelRGB(work->model98, c, c, c);
                ex->Hi_ModifyEffModelRGB(work->model9C, c, c, c);
            }
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, 0x240, 0x100, 0x40, 0x80, 0x240, 0x180);
        }
        if (work->timer0[8] != 0) {
            work->timer0[8]--;
            work->scaleF8.vx = work->scaleF8.vy = work->scaleF8.vz = fxm_easeLerp(2, 0x20, work->timer0[8], 0x400, 0x1000);
            ex->Hi_DrawEffModel(&work->rotF0, (VECTOR *)&work->pos70, (s32)&work->scaleF8, work->modelE8);
            ex->Hi_DrawEffModel(&work->rotF0, (VECTOR *)&work->pos70, (s32)&work->scaleF8, work->modelEC);
            if (work->timer0[8] < 0x10) {
                c = -((0x10 - work->timer0[8]) * 0x10);
                ex->Hi_ModifyEffModelRGB(work->modelE8, c, c, c);
                ex->Hi_ModifyEffModelRGB(work->modelEC, c, c, c);
            }
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, 0x200, 0x100, 0x40, 0x80, 0x200, 0x180);
        }
        if (work->timer0[6] != 0) {
            work->timer0[6]--;
            work->scaleD8.vx = work->scaleD8.vy = fxm_easeLerp(2, 0x20, work->timer0[6], 0x400, 0xC00);
            work->scaleD8.vz = fxm_easeLerp(2, 0x20, work->timer0[6], 0x400, 0x800);
            ex->Hi_DrawEffModel(&work->rotC8, (VECTOR *)&work->pos70, (s32)&work->scaleD8, work->modelC0);
            ex->Hi_DrawEffModel(&work->rotD0, (VECTOR *)&work->pos70, (s32)&work->scaleD8, work->modelC4);
            if (work->timer0[6] < 0x10) {
                c = -((0x10 - work->timer0[6]) * 0x10);
                ex->Hi_ModifyEffModelRGB(work->modelC0, c, c, c);
                ex->Hi_ModifyEffModelRGB(work->modelC4, c, c, c);
            }
            work->rotC8.vy = 0x400;
            work->rotC8.vx = 0x200;
            work->rotD0.vy = -0x400;
            work->rotD0.vx = 0x200;
            work->rotC8.vz += 0x40;
            work->rotD0.vz += 0x40;
            ex->btlfx_drawScrollMove(0x2710 - frame * 8, 0x2C0, 0x100, 0x20, 0x80, 0x2E0, 0x100);
        }
        if (work->timer0[4] != 0) {
            work->timer0[4]--;
            for (i = 0; i < 8; i++) {
                fx_1f3_spinModels[i].rot8.vz += fxm_easeLerp(2, 0x60, work->timer0[4], fx_1f3_spinModels[i].spin4, 0);
                n2 = fxm_easeLerp(2, 0x60, work->timer0[4], 0x40, 0);
                fx_1f3_spinModels[i].scale10.vz = 0x800;
                fx_1f3_spinModels[i].scale10.vy += n2;
                fx_1f3_spinModels[i].scale10.vx += n2;
                ex->Hi_DrawEffModel(&fx_1f3_spinModels[i].rot8, (VECTOR *)&fx_1f3_spinModels[i].pos20, (s32)&fx_1f3_spinModels[i].scale10,
                                    fx_1f3_spinModels[i].model0);
                if (work->timer0[4] < 0x40) {
                    c = -((0x40 - work->timer0[4]) * 4);
                    ex->Hi_ModifyEffModelRGB(fx_1f3_spinModels[i].model0, c, c, c);
                }
            }
        }
        if (work->timer0[7] != 0) {
            work->timer0[7]--;
            env->otZ = 0x80;
            if (work->timer0[7] >= 9) {
                for (i = 0; i < 8; i++) {
                    if (fx_1f3_rings[i].timer12C == 0) {
                        ex->btlfx_getUnitBoneMatrix(0x10, fx_1f3_rings[i].bone130, (BtlBone *)&fx_1f3_rings[i]);
                        fx_1f3_rings[i].timer12C = 8;
                        break;
                    }
                }
                for (i = 8; i < 16; i++) {
                    if (fx_1f3_rings[i].timer12C == 0) {
                        ex->btlfx_getUnitBoneMatrix(0x10, fx_1f3_rings[i].bone130, (BtlBone *)&fx_1f3_rings[i]);
                        fx_1f3_rings[i].timer12C = 8;
                        break;
                    }
                }
            }
            for (i = 0; i < 16; i++) {
                if (fx_1f3_rings[i].timer12C != 0) {
                    fx_1f3_rings[i].timer12C--;
                    ex->btlfx_getUnitBonePos(0x10, fx_1f3_rings[i].bone130, (u16 *)&v);
                    fx_1f3_rings[i].matrix0.t[0] = v.vx;
                    fx_1f3_rings[i].matrix0.t[1] = v.vy;
                    fx_1f3_rings[i].matrix0.t[2] = v.vz;
                    gte_SetRotMatrix(&fx_1f3_rings[i].matrix0);
                    gte_SetTransMatrix(&fx_1f3_rings[i].matrix0);
                    l = &fx_1f3_rings[i].trail20;
                    l->lastSlot0 = l->count2 = 0x20;
                    l->head1 = 0;
                    dst = l->points4;
                    for (j = 0; j < 0x21; j++, dst++) {
                        v.vx = (ex->math_sin(j * fx_1f3_rings[i].angleStep140 + fx_1f3_rings[i].angle134) * fx_1f3_rings[i].radius13C) >> 12;
                        v.vz = (ex->math_cos(j * fx_1f3_rings[i].angleStep140 + fx_1f3_rings[i].angle134) * fx_1f3_rings[i].radius13C) >> 12;
                        v.vy = 0;
                        gte_ldv0(&v);
                        gte_rtv0tr();
                        gte_stlvnl(&out);
                        gte_stflg(&flag);
                        dst->vx = out.vx;
                        dst->vy = out.vy;
                        dst->vz = out.vz;
                        dst->pad = 0xC0;
                    }
                    if (fx_1f3_rings[i].timer12C < 4) {
                        fx_1f3_rings[i].angle134 += fxm_easeLerp(2, 8, fx_1f3_rings[i].timer12C, fx_1f3_rings[i].spin138, 0);
                        c = fx_1f3_rings[i].timer12C << 10;
                    } else {
                        fx_1f3_rings[i].angle134 += fx_1f3_rings[i].spin138;
                        c = (8 - fx_1f3_rings[i].timer12C) << 10;
                    }
                    ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x11), 0, NULL, 0, 0x1000, c, 1,
                                        (u8 *)&fx_1f3_rings[i].trail20, 2);
                }
            }
            env->otZ = 0;
        }
        if (work->timer0[2] != 0) {
            work->timer0[2]--;
            fxm_tintScreenRatio(work->timer0[2], 4, 0xFF, 0, 1, 0);
        }
        if (work->timer0[3] != 0) {
            work->timer0[3]--;
            fxm_tintScreenRatio(work->timer0[3], 8, 0, 0xFF, 1, 0);
        }
        if (work->timer0[9] != 0) {
            work->timer0[9]--;
            cam.vx = work->pos108.vx;
            cam.vz = work->pos108.vz;
            cam.vy = fxm_easeLerp(1, 0x20, work->timer0[9], work->pos108.vy, work->pos118.vy);
            ex->btlfx_setUnitPos(0x10, (s16 *)&cam);
        }
        if (work->timer0[10] != 0) {
            work->timer0[10]--;
            if (work->timer0[10] != 0) {
                cam2.vx = work->pos108.vx;
                cam2.vz = work->pos108.vz;
                cam2.vy = fxm_easeLerp(2, 0x20, work->timer0[10], work->pos118.vy, work->pos108.vy);
            } else {
                ex->btlfx_getUnitHomePos(0x10, (s16 *)&cam2);
            }
            ex->btlfx_setUnitPos(0x10, (s16 *)&cam2);
        }
        return frame >= 0xFE;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1f3/nonmatchings/fx_1f3", fx_1f3_headerPtr);
