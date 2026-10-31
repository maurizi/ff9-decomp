#include "fxmod/fx_1eb.h"

s32 fx_1eb_op80(s32 mode, Fx1ebWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x18;
    } else if (mode == 1) {
        fx_1eb_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        s32 frame = entry->frame;

        if (work->emit0 != NULL) {
            if (work->emit0->state == BTLFX_EMIT_DONE) {
                work->emit0 = NULL;
            }
            if (work->emit0 != NULL) {
                work->emit0->rot.vx = work->rot10.vx;
                work->emit0->rot.vy = work->rot10.vy;
                work->emit0->rot.vz = work->rot10.vz;
            }
        }
        if (work->emit4 != NULL) {
            if (work->emit4->state == BTLFX_EMIT_DONE) {
                work->emit4 = NULL;
            }
            if (work->emit4 != NULL) {
                work->emit4->rot.vx = work->rot10.vx;
                work->emit4->rot.vy = work->rot10.vy;
                work->emit4->rot.vz = work->rot10.vz;
            }
        }
        return frame >= 0x28;
    }
    return 0;
}

s32 fx_1eb_op81(s32 mode, void *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    if (mode == 0) {
        *arg4 = 0;
    } else if (mode == 1) {
        fx_1eb_env = env;
    } else {
        return entry->frame >= 0xA;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1eb/nonmatchings/fx_1eb", fx_1eb_headerPtr);

s32 fx_1eb_op82(s32 mode, Fx1ebWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR v;
    s32 m;
    s32 frame;
    s32 i;
    s32 j;
    s32 t;

    if (mode == 0) {
        *arg4 = 0x60;
    } else if (mode == 1) {
        fx_1eb_env = env;
        ex->btlfx_initEffModels(fx_1eb_effModels, 6);
        work->pos44.vx = env->targets->centerX;
        work->pos44.vy = env->targets->centerY;
        work->pos44.vz = env->targets->centerZ;
        work->rot18.vx = 0;
        work->rot18.vy = 0;
        work->rot18.vz = 0;
        work->pos20.vx = 0;
        work->pos20.vy = 0;
        work->pos20.vz = 0;
        work->scale30.vx = 0xAAA;
        work->scale30.vy = 0xAAA;
        work->scale30.vz = 0xAAA;
        fxm_valuePathInit(&fx_1eb_valuePath, (u16 *)ex->btlfx_getFileData(entry->file, 7));
        for (j = 0; j < 3; j++) {
            fxm_rotPosPathInit(&fx_1eb_rotPosPath[j], (u16 *)ex->btlfx_getFileData(entry->file, fx_1eb_pathFileIds[j]));
            work->model52[j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_1eb_modelFileIds[j]), 1);
        }
        work->model58 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 6), 1);
        work->timer4C = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->alpha50 = 0x1000;
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1D), (s32)&work->pos44);
            work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->pos44);
            break;
        case 30:
            work->timer4C = 0xA;
            break;
        case 10:
            work->emit8 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos44);
            work->emitC = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos44);
            break;
        case 20:
            work->emit10 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos44);
            break;
        case 22:
            work->emit14 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos44);
            if (work->emit14 != NULL) {
                work->emit14->trails = (s32)fx_1eb_op82Trails;
                work->emit14->trailSlots = 6;
                work->emit14->trailStride = 0x60;
            }
            break;
        }
        if (frame >= 0 && frame < 0x28) {
            t = frame;
            if (work->timer4C >= 0) {
                work->rgb40 = fxm_lerpRatio(0, -0x80, 0xA - work->timer4C, 0xA);
                work->timer4C--;
            }
            for (i = 0; i < 3; i++) {
                fxm_rotPosPathGet(&fx_1eb_rotPosPath[i], t, &work->rot18, &v);
                work->pos20.vx = v.vx;
                work->pos20.vy = v.vy;
                work->pos20.vz = v.vz;
                work->pos20.vx += work->pos44.vx;
                work->pos20.vy += work->pos44.vy;
                work->pos20.vz += work->pos44.vz;
                if (i == 0 && t >= 0x16) {
                    fxm_valuePathGet(&fx_1eb_valuePath, t - 0x16, &m);
                    ex->Hi_DrawMorphEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model52[i],
                                             work->model58, m);
                } else {
                    ex->Hi_SetEffModelSlice(work->model52[i], work->pos20.vy);
                    ex->Hi_DrawSliceEffModel(&work->rot18, &work->pos20, (s32)&work->scale30, work->model52[i]);
                }
                if (work->timer4C >= 0) {
                    ex->Hi_ModifyEffModelAbr(work->model52[i], 1);
                    ex->Hi_ModifyEffModelRGB(work->model52[i], work->rgb40, work->rgb40, work->rgb40);
                } else {
                    ex->Hi_ModifyEffModelAbr(work->model52[i], 0);
                }
            }
        }
        if (frame >= 0x18) {
            work->alpha50 -= 0x200;
            if (work->emit0 != NULL) {
                work->emit0->alpha = work->alpha50;
            }
            if (work->alpha50 == 0) {
                ex->btlfx_stopEmitter(work->emit0);
            }
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_1eb_op83(s32 mode, Fx1ebWork83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);

    if (mode == 0) {
        *arg4 = 0x10;
    } else if (mode == 1) {
        s32 unit = *arg4;
        work->unitC = unit;
        fx_1eb_env = env;
        if (unit >= env->targets->targetCount) {
            return 1;
        }
        ex->btlfx_getUnitPos(unit, (s16 *)&work->pos4);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos4);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(work->unitC, 0x80);
        }
    } else {
        return entry->frame >= 0x28;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1eb_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_rotPosPathInit(Fx1ebKeyPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1ebKey *)data;
}

static s32 fxm_rotPosPathGet(Fx1ebKeyPath *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx1ebKey *k;
    s32 r = 1;

    if (i > path->count2 - 1) {
        i = path->count2 - 1;
    } else {
        r = 0;
    }
    k = &path->keys4[i];
    if (rot != NULL) {
        rot->vx = k->rot6[0];
        rot->vy = k->rot6[1];
        rot->vz = k->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = k->pos0[0];
        pos->vy = k->pos0[1];
        pos->vz = k->pos0[2];
    }
    return r;
}

static void fxm_valuePathInit(Fx1ebPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1ebPoint *)data;
}

static s32 fxm_valuePathGet(Fx1ebPath *keys, s32 i, s32 *out) {
    s32 r = 1;

    if (i > keys->count2 - 1) {
        i = keys->count2 - 1;
    } else {
        r = 0;
    }
    *out = *(s16 *)((u8 *)keys->keys4 + (i << 1));
    return r;
}
