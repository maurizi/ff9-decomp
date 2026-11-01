#include "fxmod/fx_1ec.h"

static s32 fx_1ec_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    SVECTOR v;

    fxm_posPathGet(&fx_1ec_posPath0[(s16)(e->slot - 1)], frame, &v);
    v.vy = v.vy * 4 / 5;
    v.vz = v.vz * 3 / 4;
    v.vx += fx_1ec_op80Origin.vx;
    v.vy += fx_1ec_op80Origin.vy;
    v.vz += fx_1ec_op80Origin.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

static s32 fx_1ec_op81ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 frames) {
    SVECTOR v;

    fxm_posPathGet(&fx_1ec_posPath1[(s16)(e->slot - 1)], frame, &v);
    v.vx += fx_1ec_op81Origin.vx;
    v.vy += fx_1ec_op81Origin.vy;
    v.vz += fx_1ec_op81Origin.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_1ec_op80(s32 mode, Fx1ecWork80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0x64;
    } else if (mode == 1) {
        fx_1ec_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos24);
        work->pos2C.vx = env->targets->centerX;
        work->pos2C.vy = env->targets->centerY;
        work->pos2C.vz = env->targets->centerZ;
        work->pos34.vx = work->pos24.vx;
        work->pos34.vy = work->pos24.vy;
        work->pos34.vz = work->pos24.vz;
        work->unk3C.vx = work->pos24.vx;
        work->unk3C.vy = work->pos24.vy;
        work->unk3C.vz = work->pos24.vz;
        fx_1ec_op80Origin.vx = work->pos24.vx;
        fx_1ec_op80Origin.vy = work->pos24.vy;
        fx_1ec_op80Origin.vz = work->pos24.vz;
        work->pos34.vy = 0;
        for (i = 0; i < 0x12; i++) {
            fxm_posPathInit(&fx_1ec_posPath0[i], fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x1E), i));
        }
        work->camPos4C.vx = work->pos2C.vx;
        work->camPos4C.vy = work->pos2C.vy;
        work->camPos4C.vz = work->pos2C.vz;
        work->camPos54.vx = work->pos2C.vx;
        work->camPos54.vy = work->pos2C.vy;
        work->camPos54.vz = work->pos2C.vz;
        work->camPos5C.vx = work->pos2C.vx;
        work->camPos5C.vy = work->pos2C.vy;
        work->camPos5C.vz = work->pos2C.vz;
        work->camPos4C.vy -= 0x2AF8;
        work->camPos4C.vz -= 0xFA0;
        work->camPos54.vy -= 0xC80;
        work->camPos54.vz -= 0xFA0;
        work->camPos5C.vy -= 0x2AF8;
        env->camera->points[0].vx = work->camPos4C.vx;
        env->camera->points[0].vy = work->camPos4C.vy;
        env->camera->points[0].vz = work->camPos4C.vz;
        env->camera->points[1].vx = work->camPos54.vx;
        env->camera->points[1].vy = work->camPos54.vy;
        env->camera->points[1].vz = work->camPos54.vz;
        env->camera->points[2].vx = work->camPos5C.vx;
        env->camera->points[2].vy = work->camPos5C.vy;
        env->camera->points[2].vz = work->camPos5C.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x8:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 1), (s32)&work->pos24);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos24);
            if (work->emit0[3] != NULL) {
                work->emit0[3]->trails = (s32)fx_1ec_op80Trails0;
                work->emit0[3]->trailSlots = 0x14;
                work->emit0[3]->trailStride = 0x50;
            }
            if (work->emit0[0] != NULL) {
                work->emit0[0]->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x12:
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos24);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos34);
            break;
        case 0x30:
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos24);
            work->emit0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->pos24);
            work->emit0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos34);
            work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&fx_1ec_emitPos0);
            if (work->emit0[7] != NULL) {
                work->emit0[7]->trails = (s32)fx_1ec_op80Trails1;
                work->emit0[7]->trailSlots = 0x12;
                work->emit0[7]->trailStride = 0x70;
                if (work->emit0[7] != NULL) {
                    work->emit0[7]->file->m.module.update = fx_1ec_op80ParticleMotion;
                }
            }
            if (work->emit0[5] != NULL) {
                work->emit0[5]->otZ = 0x600;
            }
            if (work->emit0[6] != NULL) {
                work->emit0[6]->otZ = 0x600;
            }
            if (work->emit0[7] != NULL) {
                work->emit0[7]->otZ = 0x600;
            }
            break;
        case 0x4E:
            ex->btlfx_stopEmitter(work->emit0[7]);
            break;
        case 0x34:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x26), frame);
        return frame >= 0x64;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_1ec/nonmatchings/fx_1ec", fx_1ec_headerPtr);

s32 fx_1ec_op81(s32 mode, Fx1ecWork81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0x54;
    } else if (mode == 1) {
        work->pos2C.vx = env->targets->centerX;
        work->pos2C.vy = env->targets->centerY;
        work->pos2C.vz = env->targets->centerZ;
        fx_1ec_env = env;
        work->pos34.vx = work->pos2C.vx; work->pos34.vy = work->pos2C.vy; work->pos34.vz = work->pos2C.vz;
        work->pos3C.vx = work->pos2C.vx; work->pos3C.vy = work->pos2C.vy; work->pos3C.vz = work->pos2C.vz;
        work->pos44.vx = work->pos2C.vx; work->pos44.vy = work->pos2C.vy; work->pos44.vz = work->pos2C.vz;
        work->pos34.vy -= 0xC80;
        work->pos34.vz -= 0xFA0;
        work->pos3C.vy -= 0x2AF8;
        work->pos3C.vz -= 0xFA0;
        work->pos44.vy -= 0x2AF8;
        fx_1ec_op81Origin.vx = work->pos3C.vx;
        fx_1ec_op81Origin.vy = work->pos3C.vy;
        fx_1ec_op81Origin.vz = work->pos3C.vz;
        for (i = 0; i < 0x1E; i++) {
            fxm_posPathInit(&fx_1ec_posPath1[i], fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0xB), i));
        }
    } else {
    frame = entry->frame;
    switch (frame) {
    case 0x0:
        work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x25), (s32)&work->pos34);
        if (work->emit0[0] != NULL) {
            work->emit0[0]->trails = (s32)fx_1ec_op81Trails0;
            work->emit0[0]->trailSlots = 0x17;
            work->emit0[0]->trailStride = 0x90;
        }
        break;
    case 0x14:
        work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos3C);
        break;
    case 0x18:
        work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos3C);
        if (work->emit0[2] != NULL) {
            work->emit0[2]->trails = (s32)fx_1ec_op81Trails2;
            work->emit0[2]->trailSlots = 0x20;
            work->emit0[2]->trailStride = 0x50;
        }
        break;
    case 0x1C:
        work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos3C);
        break;
    case 0x30:
        work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos3C);
        work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&work->pos3C);
        work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos3C);
        work->emit0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->pos3C);
        break;
    case 0x33:
        ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        break;
    case 0x38:
        work->emit0[9] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&fx_1ec_emitPos1);
        if (work->emit0[9] != NULL) {
            work->emit0[9]->trails = (s32)fx_1ec_op81Trails3;
            work->emit0[9]->trailSlots = 0x2A;
            work->emit0[9]->trailStride = 0x90;
            if (work->emit0[9] != NULL) {
                work->emit0[9]->file->m.module.update = fx_1ec_op81ParticleMotion;
            }
        }
        break;
    case 0x45:
        work->pos4C.vx = work->pos44.vx;
        work->pos4C.vy = work->pos44.vy;
        work->pos4C.vz = work->pos44.vz;
        work->pos4C.vy = -0xFA0;
        work->emit0[10] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->pos4C);
        if (work->emit0[10] != NULL) {
            work->emit0[10]->trails = (s32)fx_1ec_op81Trails1;
            work->emit0[10]->trailSlots = 0x16;
            work->emit0[10]->trailStride = 0x90;
        }
        break;
    case 0x4B:
        ex->btlfx_stopEmitter(work->emit0[9]);
        break;
    }
    ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x26), frame);
    return frame >= 0x64;
    }
    return 0;
}

s32 fx_1ec_op82(s32 mode, Fx1ecWork82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 w;
    s32 i;

    if (mode == 0) {
        *args = 0x70;
    } else if (mode == 1) {
        fx_1ec_env = env;
        ex->btlfx_initEffModels(fx_1ec_effModels, 2);
        work->pos4C.vx = env->targets->centerX;
        work->pos4C.vy = env->targets->centerY;
        work->pos4C.vz = env->targets->centerZ;
        work->rot20.vx = 0;
        work->rot20.vy = 0;
        work->rot20.vz = 0;
        work->pos28.vx = work->pos4C.vx;
        work->pos28.vy = 0;
        work->pos28.vz = work->pos4C.vz;
        work->scale38.vx = 0x4000;
        work->scale38.vy = 0x4000;
        work->scale38.vz = 0x4000;
        work->model6C = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xD), 1);
        work->model6E = ex->Hi_RegisterGouEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 0xE));
        work->timer64 = -1;
        work->timer68 = -1;
        work->flag6A = 0;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->pos54.vx = work->pos4C.vx;
            work->pos54.vy = work->pos4C.vy;
            work->pos54.vz = work->pos4C.vz;
            work->pos54.vy = -0x157C;
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos54);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->otZ = 0x500;
            }
            break;
        case 4:
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->pos4C);
            if (work->emit0[1] != NULL) {
                work->emit0[1]->otZ = 0x500;
            }
            break;
        case 16:
            work->pos5C.vx = work->pos4C.vx;
            work->pos5C.vy = work->pos4C.vy;
            work->pos5C.vz = work->pos4C.vz;
            work->pos5C.vy = -0x1F40;
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x24), (s32)&work->pos5C);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&work->pos4C);
            if (work->emit0[2] != NULL) {
                work->emit0[2]->otZ = 0x500;
            }
            break;
        case 30:
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x23), (s32)&work->pos4C);
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&work->pos4C);
            work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x27), (s32)&work->pos4C);
            if (work->emit0[4] != NULL) {
                work->emit0[4]->trails = (s32)fx_1ec_op82Trails0;
                work->emit0[4]->trailSlots = 0x26;
                work->emit0[4]->trailStride = 0x90;
            }
            if (work->emit0[5] != NULL) {
                work->emit0[5]->trails = (s32)fx_1ec_op82Trails1;
                work->emit0[5]->trailSlots = 0x14;
                work->emit0[5]->trailStride = 0x90;
            }
            if (work->emit0[7] != NULL) {
                work->emit0[7]->otZ = 0x200;
            }
            break;
        case 32:
            work->timer68 = 0x3C;
            for (i = 0; i < env->targets->targetCount; i++) {
                fxm_startHeavyHitMotionIfActive(i);
            }
            break;
        case 42:
            work->timer64 = 8;
            break;
        }
        t = frame - 0x1E;
        if ((u32)t < 0x14) {
            if (t < 0) {
                w = 0x1000;
            } else if (t >= 0x14) {
                w = 0;
            } else {
                w = 0x1000 - (t << 12) / 20;
            }
            ex->Hi_DrawMorphEffModel(&work->rot20, &work->pos28, (s32)&work->scale38, work->model6C, work->model6E, w);
            if (work->timer64 >= 0) {
                work->rgb48 = fxm_lerpRatio(0, -0xFF, 8 - work->timer64, 8);
                work->timer64--;
                ex->Hi_ModifyEffModelRGB(work->model6C, work->rgb48, work->rgb48, work->rgb48);
            }
        }
        if (work->timer68 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, 0x3C - work->timer68, 0x3C, 1, 0);
            work->timer68--;
            if (work->timer68 == -1) {
                work->flag6A = 1;
                ex->btlfx_signalScriptCue();
            }
        } else if (work->flag6A != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        if (frame >= 0x20) {
            for (i = 0; i < env->targets->targetCount; i++) {
                if (fxm_isMotionDone(i) != 0) {
                    fxm_startUnitMotion(i, 4);
                    ex->btlfx_holdMotion(i);
                }
            }
        }
        if (frame >= 0x78) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static u16 *fxm_offsetTableEntry(u8 *base, u16 idx) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)idx + 1]);
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_1ec_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_1ec_env)->btlfx_drawScreenTint((u32 *)(fx_1ec_env->ot + ot), abr, bright, bright, bright);
}

static void fxm_posPathInit(Fx1ecPath *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx1ecPoint *)data;
}

static s32 fxm_posPathGet(Fx1ecPath *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx1ecPoint *key;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    out->vx = key->x;
    out->vy = key->y;
    out->vz = key->z;
    return done;
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ec_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_1ec_env);

    s32 done;

    if (ex->btlfx_isUnitActive(unit)) {
        done = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        done = 0;
    }
    return done;
}

static void fxm_startHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1ec_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT);
    }
}

static void fxm_startHeavyHitMotionIfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_1ec_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, BTL_MOTION_HIT_HEAVY);
    }
}
