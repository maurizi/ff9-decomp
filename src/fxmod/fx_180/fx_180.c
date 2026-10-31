#include "fxmod/fx_180.h"

static s32 fx_180_op80ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    fxm_posPathGet(&D_801E93F8[(s16)(e->slot - 1)], frame, &v);
    v.vy = v.vy * 4 / 5;
    v.vz = v.vz * 3 / 4;
    v.vx += D_801F0828.vx;
    v.vy += D_801F0828.vy;
    v.vz += D_801F0828.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

static s32 fx_180_op81ParticleMotion(Pool30Entry *e, Pool6CEntry *owner, SVECTOR *pos, s32 arg3, s32 frame, s32 frames) {
    SVECTOR v;

    fxm_posPathGet(&D_801E9488[(s16)(e->slot - 1)], frame, &v);
    v.vx += D_801F0830.vx;
    v.vy += D_801F0830.vy;
    v.vz += D_801F0830.vz;
    pos->vx = v.vx;
    pos->vy = v.vy;
    pos->vz = v.vz;
    if (frame >= frames) {
        return -1;
    }
    return 0;
}

s32 fx_180_op80(s32 mode, Fx180Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    MATRIX m;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x74;
    } else if (mode == 1) {
        fx_180_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos24);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos2C);
        work->unk34.vx = env->targets->centerX;
        work->unk34.vy = env->targets->centerY;
        work->unk34.vz = env->targets->centerZ;
        work->pos44.vx = work->pos24.vx;
        work->pos44.vy = work->pos24.vy;
        work->pos44.vz = work->pos24.vz;
        work->unk4C.vx = work->pos24.vx;
        work->unk4C.vy = work->pos24.vy;
        work->unk4C.vz = work->pos24.vz;
        D_801F0828.vx = work->pos24.vx;
        D_801F0828.vy = work->pos24.vy;
        D_801F0828.vz = work->pos24.vz;
        work->pos44.vy = 0;
        for (i = 0; i < 0x12; i++) {
            fxm_posPathInit(&D_801E93F8[i], fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x22), i));
        }
        work->unk54.vx = work->unk34.vx;
        work->unk54.vy = work->unk34.vy;
        work->unk54.vz = work->unk34.vz;
        work->unk5C.vx = work->unk34.vx;
        work->unk5C.vy = work->unk34.vy;
        work->unk5C.vz = work->unk34.vz;
        work->unk64.vx = work->unk34.vx;
        work->unk64.vy = work->unk34.vy;
        work->unk64.vz = work->unk34.vz;
        work->unk6C.vx = work->pos2C.vx;
        work->unk6C.vy = work->pos2C.vy;
        work->unk6C.vz = work->pos2C.vz;
        work->unk54.vy -= 0x2AF8;
        work->unk54.vz -= 0xFA0;
        work->unk5C.vy -= 0xC80;
        work->unk5C.vz -= 0xFA0;
        work->unk64.vy -= 0x2AF8;
        work->unk6C.vy -= 0x3E8;
        env->camera->points[0].vx = work->unk54.vx;
        env->camera->points[0].vy = work->unk54.vy;
        env->camera->points[0].vz = work->unk54.vz;
        env->camera->points[1].vx = work->unk5C.vx;
        env->camera->points[1].vy = work->unk5C.vy;
        env->camera->points[1].vz = work->unk5C.vz;
        env->camera->points[2].vx = work->unk64.vx;
        env->camera->points[2].vy = work->unk64.vy;
        env->camera->points[2].vz = work->unk64.vz;
        env->camera->points[3].vx = work->unk6C.vx;
        env->camera->points[3].vy = work->unk6C.vy;
        env->camera->points[3].vz = work->unk6C.vz;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x8:
            ex->btlfx_getUnitBoneMatrix(0x10, 0x29, (BtlBone *)&m);
            ex->btlfx_matrixPointAhead(&m, 0, (s32)&work->pos3C);
            work->pos3C.vy += 0x64;
            work->pos3C.vz -= 0xC8;
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x18), (s32)&work->pos3C);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos24);
            if (work->emit0[3] != NULL) {
                work->emit0[3]->trails = (s32)D_801E9578;
                work->emit0[3]->trailSlots = 0x14;
                work->emit0[3]->trailStride = 0x50;
            }
            if (work->emit0[0] != NULL) {
                work->emit0[0]->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
            }
            break;
        case 0x12:
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 2), (s32)&work->pos24);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 3), (s32)&work->pos44);
            break;
        case 0x30:
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos24);
            work->emit0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&work->pos24);
            work->emit0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&work->pos44);
            work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&D_801F0838);
            if (work->emit0[7] != NULL) {
                work->emit0[7]->trails = (s32)D_801E9BB8;
                work->emit0[7]->trailSlots = 0x12;
                work->emit0[7]->trailStride = 0x70;
            }
            if (work->emit0[7] != NULL) {
                ((EffMotion *)work->emit0[7]->file)->update = fx_180_op80ParticleMotion;
            }
            if (work->emit0[7] != NULL) {
                work->emit0[7]->otZ = 0x600;
            }
            break;
        case 0x4E:
            ex->btlfx_stopEmitter(work->emit0[7]);
            break;
        case 0x14:
            ex->btlfx_signalScriptCue();
            break;
        case 0x34:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x19), frame);
        return frame >= 0x64;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_180/nonmatchings/fx_180", fx_180_headerPtr);

s32 fx_180_op81(s32 mode, Fx180Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x54;
    } else if (mode == 1) {
        work->unk2C.vx = env->targets->centerX;
        work->unk2C.vy = env->targets->centerY;
        work->unk2C.vz = env->targets->centerZ;
        fx_180_env = env;
        work->pos34.vx = work->unk2C.vx; work->pos34.vy = work->unk2C.vy; work->pos34.vz = work->unk2C.vz;
        work->pos3C.vx = work->unk2C.vx; work->pos3C.vy = work->unk2C.vy; work->pos3C.vz = work->unk2C.vz;
        work->unk44.vx = work->unk2C.vx; work->unk44.vy = work->unk2C.vy; work->unk44.vz = work->unk2C.vz;
        work->pos34.vy -= 0xC80;
        work->pos34.vz -= 0xFA0;
        work->pos3C.vy -= 0x2AF8;
        work->pos3C.vz -= 0xFA0;
        work->unk44.vy -= 0x2AF8;
        D_801F0830.vx = work->pos3C.vx;
        D_801F0830.vy = work->pos3C.vy;
        D_801F0830.vz = work->pos3C.vz;
        for (i = 0; i < 0x1E; i++) {
            fxm_posPathInit(&D_801E9488[i], fxm_offsetTableEntry((u8 *)ex->btlfx_getFileData(entry->file, 0x1A), i));
        }
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x0:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos34);
            if (work->emit0[0] != NULL) {
                work->emit0[0]->trails = (s32)D_801EA398;
                work->emit0[0]->trailSlots = 0x17;
                work->emit0[0]->trailStride = 0x90;
            }
            break;
        case 0x14:
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos3C);
            break;
        case 0x18:
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&work->pos3C);
            if (work->emit0[2] != NULL) {
                work->emit0[2]->trails = (s32)D_801EBCE8;
                work->emit0[2]->trailSlots = 0x20;
                work->emit0[2]->trailStride = 0x50;
            }
            break;
        case 0x1C:
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos3C);
            break;
        case 0x30:
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos3C);
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos3C);
            work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x10), (s32)&work->pos3C);
            work->emit0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x11), (s32)&work->pos3C);
            break;
        case 0x33:
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
            break;
        case 0x38:
            work->emit0[9] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x12), (s32)&D_801F0840);
            if (work->emit0[9] != NULL) {
                work->emit0[9]->trails = (s32)D_801EC6E8;
                work->emit0[9]->trailSlots = 0x2A;
                work->emit0[9]->trailStride = 0x90;
            }
            if (work->emit0[9] != NULL) {
                ((EffMotion *)work->emit0[9]->file)->update = fx_180_op81ParticleMotion;
            }
            break;
        case 0x45:
            work->pos4C.vx = work->unk44.vx;
            work->pos4C.vy = work->unk44.vy;
            work->pos4C.vz = work->unk44.vz;
            work->pos4C.vy = -0xFA0;
            work->emit0[10] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x13), (s32)&work->pos4C);
            if (work->emit0[10] != NULL) {
                work->emit0[10]->trails = (s32)D_801EB088;
                work->emit0[10]->trailSlots = 0x16;
                work->emit0[10]->trailStride = 0x90;
            }
            break;
        case 0x4B:
            ex->btlfx_stopEmitter(work->emit0[9]);
            break;
        }
        ex->btlfx_drawBackdrop((BtlBgLayers *)ex->btlfx_getFileData(entry->file, 0x19), frame);
        return frame >= 0x64;
    }
    return 0;
}

s32 fx_180_op82(s32 mode, Fx180Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 t;
    s32 w;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x6C;
    } else if (mode == 1) {
        fx_180_env = env;
        ex->btlfx_initEffModels(fx_180_effModels, 2);
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
        work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x17), 1);
        work->model6A = ex->Hi_RegisterGouEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 1));
        work->unk64 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->pos54.vx = work->pos4C.vx;
            work->pos54.vy = work->pos4C.vy;
            work->pos54.vz = work->pos4C.vz;
            work->pos54.vy = -0x157C;
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x14), (s32)&work->pos54);
            break;
        case 4:
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x16), (s32)&work->pos4C);
            break;
        case 16:
            work->pos5C.vx = work->pos4C.vx;
            work->pos5C.vy = work->pos4C.vy;
            work->pos5C.vz = work->pos4C.vz;
            work->pos5C.vy = -0x1F40;
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x15), (s32)&work->pos5C);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->pos4C);
            break;
        case 30:
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1D), (s32)&work->pos4C);
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&work->pos4C);
            work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->pos4C);
            if (work->emit0[4] != NULL) {
                work->emit0[4]->trails = (s32)D_801EDE88;
                work->emit0[4]->trailSlots = 0x26;
                work->emit0[4]->trailStride = 0x90;
            }
            if (work->emit0[5] != NULL) {
                work->emit0[5]->trails = (s32)D_801EF3E8;
                work->emit0[5]->trailSlots = 0x14;
                work->emit0[5]->trailStride = 0x90;
            }
            break;
        case 32:
            for (i = 0; i < env->targets->targetCount; i++) {
                fxm_startMotion3IfActive(i);
            }
            break;
        case 42:
            work->unk64 = 8;
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
            ex->Hi_DrawMorphEffModel(&work->rot20, &work->pos28, (s32)&work->scale38, work->model68, work->model6A, w);
            if (work->unk64 >= 0) {
                work->unk48 = fxm_lerpRatio(0, -0xFF, 8 - work->unk64, 8);
                work->unk64--;
                ex->Hi_ModifyEffModelRGB(work->model68, work->unk48, work->unk48, work->unk48);
            }
        }
        if (frame >= 0x20) {
            for (i = 0; i < env->targets->targetCount; i++) {
                if (fxm_isMotionDone(i) != 0) {
                    fxm_startUnitMotion(i, 4);
                    ex->btlfx_holdMotion(i);
                }
            }
        }
        if (frame >= 0x3C) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_180_op83(s32 mode, Fx180Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        fx_180_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        work->unk38 = -1;
        work->unk3A = 0;
        work->pos30.vx = work->pos8.vx;
        work->pos30.vy = work->pos8.vy;
        work->pos30.vz = work->pos8.vz;
        work->pos30.vy += 0x190;
        work->pos30.vz -= 0x3E8;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 3:
            work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1E), (s32)&work->pos30);
            if (work->emit0 != NULL) {
                work->emit0->trails = (s32)D_801EFF28;
                work->emit0->trailSlots = 0x10;
                work->emit0->trailStride = 0x90;
            }
            break;
        case 0x11:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos10);
            work->unk18.vx = work->pos10.vx;
            work->unk18.vy = work->pos10.vy;
            work->unk18.vz = work->pos10.vz + 0x1770;
            break;
        case 0x25:
            ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos20);
            break;
        case 0x14:
            work->unk38 = 0x1E;
            break;
        }
        if (frame >= 0x11 && frame < 0x25) {
            fxm_moveUnitArc(0x10, &work->unk18, frame - 0x11, 0x14, 0, &work->unk28);
        }
        if (frame >= 0x25) {
            work->pos20.vy += 0xC8;
            ex->btlfx_setUnitPos(0x10, (s16 *)&work->pos20);
        }
        if (work->unk38 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, 0x1E - work->unk38, 0x1E, 1, 0);
            work->unk38--;
            if (work->unk38 == -1) {
                work->unk3A = 1;
                ex->btlfx_signalScriptCue();
            }
        } else if (work->unk3A != 0) {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x50;
    }
    return 0;
}

static u16 *fxm_offsetTableEntry(u8 *base, u16 i) {
    return (u16 *)((s32)base + ((s32 *)base)[(u16)i + 1]);
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_180_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_180_env)->btlfx_drawScreenTint((u32 *)(fx_180_env->ot + ot), arg4, c, c, c);
}

static void fxm_posPathInit(Fx180Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx180Point *)data;
}

static s32 fxm_posPathGet(Fx180Path *path, s32 t, SVECTOR *out) {
    s32 last = path->unk2 - 1;
    s32 done = 1;
    Fx180Point *p;

    if (last < t) {
        t = last;
    } else {
        done = 0;
    }
    p = &path->unk4[t];
    out->vx = p->x;
    out->vy = p->y;
    out->vz = p->z;
    return done;
}

static void fxm_startUnitMotion(s32 unit, s32 motion) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_180_env);

    ex->btlfx_setUnitMotion(unit, ex->btlfx_getUnitMotion(unit, motion));
    ex->btlfx_setMotionFrame(unit, 0);
}

static s32 fxm_isMotionDone(s32 unit) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_180_env);

    s32 r;

    if (ex->btlfx_isUnitActive(unit)) {
        r = ex->btlfx_getMotionFrame(unit) == ex->btlfx_getMotionLength(unit) - 1;
    } else {
        r = 0;
    }
    return r;
}

static void fxm_startMotion2IfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_180_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, 2);
    }
}

static void fxm_startMotion3IfActive(s32 unit) {
    if (BTLFX_EXPORTS(fx_180_env)->btlfx_isUnitActive(unit)) {
        fxm_startUnitMotion(unit, 3);
    }
}

static s32 fxm_moveUnitArc(s32 unit, SVECTOR *dst, s32 frame, s32 n, s32 height, SVECTOR *start) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_180_env);
    SVECTOR pos;
    s32 t;

    if (frame == 0) {
        ex->btlfx_getUnitPos(unit, (s16 *)start);
    }
    t = (frame << 12) / n;
    fxm_lerpSvec(start, dst, t, &pos);
    if (height) {
        pos.vy -= (ex->math_sin(t >> 1) * height) >> 12;
    }
    ex->btlfx_setUnitPos(unit, (s16 *)&pos);
    return frame >= n;
}
