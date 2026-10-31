#include "fxmod/fx_123.h"

static s32 fxm_pinParticle0(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[0], frame, n);
}

static s32 fxm_pinParticle1(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[1], frame, n);
}

static s32 fxm_pinParticle2(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[2], frame, n);
}

static s32 fxm_pinParticle3(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[3], frame, n);
}

static s32 fxm_pinParticle4(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[4], frame, n);
}

static s32 fxm_pinParticle5(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E96B4[0], frame, n);
}

s32 fxm_pinParticle6(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[0], frame, n);
}

s32 fxm_pinParticle7(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[1], frame, n);
}

s32 fxm_pinParticle8(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[2], frame, n);
}

s32 fxm_pinParticle9(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[3], frame, n);
}

s32 fxm_pinParticle10(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E968C[4], frame, n);
}

s32 fxm_pinParticle11(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &D_801E96B4[0], frame, n);
}

static s32 fxm_pinParticle(Pool30Entry *pt, SVECTOR *pos, SVECTOR *src, s32 frame, s32 n) {
    pos->vx = src->vx;
    pos->vy = src->vy;
    pos->vz = src->vz;
    pt->pos.vx = pos->vx << 12;
    pt->pos.vy = pos->vy << 12;
    pt->pos.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_123/nonmatchings/fx_123", fx_123_headerPtr);

s32 fx_123_op80(s32 mode, Fx123Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone m;
    Fx123Bone *b;
    Pool6CEntry *p;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *arg4 = 0x8C;
    } else if (mode == 1) {
        fx_123_env = env;
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos4C);
        work->pos54.vx = work->pos4C.vx;
        work->pos54.vy = work->pos4C.vy;
        work->pos54.vz = work->pos4C.vz;
        work->pos54.vy -= 0xC8;
        work->pos54.vz -= 0x190;
    } else {
    frame = entry->frame;
    switch (frame) {
    case 0:
        for (i = 0; i < 6; i++) {
            work->emit0[i] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&D_801E9BFC[i]);
            if (p != NULL) {
                p->otZ = 0x20;
            }
        }
        break;
    case 20:
        for (i = 0; i < 6; i++) {
            work->emit18[i] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&D_801E9BFC[i]);
            if (p != NULL) {
                p->otZ = 0x20;
            }
        }
        break;
    case 30:
        for (i = 0; i < 6; i++) {
            work->emit30[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&D_801E9C2C[i]);
        }
        break;
    case 43:
        work->emit48 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos54);
        break;
    case 38:
        for (i = 0; i < 6; i++) {
            ex->btlfx_stopEmitter(work->emit18[i]);
        }
        break;
    }
    if (frame >= 0 && frame < 0x12) {
        b = D_801E8F64;
        for (i = 0; i < 6; i++, b++) {
            ex->btlfx_getUnitBoneMatrix(0x10, b->unk0, &m);
            ex->btlfx_matrixPointAhead((MATRIX *)&m, b->unk2, (s32)&work->unk5C[i]);
            if (work->emit0[i] != NULL && work->emit0[i]->state == BTLFX_EMIT_DONE) {
                work->emit0[i] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0[i], (s32)&work->unk5C[i]);
        }
    }
    if (frame >= 0x14 && frame < 0x26) {
        b = D_801E8F64;
        for (i = 0; i < 6; i++, b++) {
            ex->btlfx_getUnitBoneMatrix(0x10, b->unk0, &m);
            ex->btlfx_matrixPointAhead((MATRIX *)&m, b->unk2, (s32)&work->unk5C[i]);
            if (work->emit18[i] != NULL && work->emit18[i]->state == BTLFX_EMIT_DONE) {
                work->emit18[i] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit18[i], (s32)&work->unk5C[i]);
        }
    }
    if (frame >= 0x1E && frame < 0x37) {
        b = D_801E8F64;
        for (i = 0; i < 6; i++, b++) {
            ex->btlfx_getUnitBoneMatrix(0x10, b->unk0, &m);
            ex->btlfx_matrixPointAhead((MATRIX *)&m, b->unk2, (s32)&work->unk5C[i]);
            if (work->emit30[i] != NULL && work->emit30[i]->state == BTLFX_EMIT_DONE) {
                work->emit30[i] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit30[i], (s32)&work->unk5C[i]);
        }
    }
    return frame >= 0x50;
    }
    return 0;
}

s32 fx_123_op81(s32 mode, Fx123Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    Pool6CEntry *p;
    s32 frame;
    Fx123Shot *s;
    s32 j;
    s32 i;
    s32 k;
    s32 t;
    s32 f;
    s32 n;

    if (mode == 0) {
        *arg4 = 0x188;
    } else if (mode == 1) {
        fx_123_env = env;
        ex->btlfx_initEffModels(fx_123_effModels, 7);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos64);
        for (n = 0; n < 6; n++) {
            fxm_rotPosPathInit((Fx123Path *)&D_801E965C[n], (u16 *)ex->btlfx_getFileData(entry->file, D_801E9054[n]));
            setVector(&work->unk6C[n], 0, 0, 0);
            setVector(&work->unk9C[n], 0, 0, 0);
            work->model17A[n] = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 7), 0x97, 0x3D40, 0x80, 0);
        }
        setVector(&work->scaleFC, 0x1000, 0x1000, 0x1000);
        work->model186 = ex->Hi_RegisterTexEffModel((BtlUnit9C8 *)ex->btlfx_getFileData(entry->file, 5), 0x97, 0x3D40, 0x80, 0);
        work->unk178 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->pos140.vx = work->pos64.vx;
            work->pos140.vy = work->pos64.vy - 0x1013;
            work->pos140.vz = work->pos64.vz - 0x5F5;
            work->emit60 = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos140);
            if (p != NULL) {
                p->otZ = 0x80;
            }
            break;
        case 11:
            for (j = 0; j < 6; j++) {
                work->emit0[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&D_801E9C5C[j]);
            }
            break;
        case 19:
            for (s = D_801E8F7C, j = 0; j < 6; j++, s++) {
                s->unk0.vx = env->targets->centerX;
                s->unk0.vy = work->unk9C[j].vy;
                s->unk0.vz = env->targets->centerZ;
                s->unk0.vx += ex->math_sin(s->unk20) * 350 / 4096;
                s->unk0.vz += ex->math_cos(s->unk20) * 350 / 4096;
                s->unk8.vx = work->unk6C[j].vx;
                s->unk8.vy = work->unk6C[j].vy;
                s->unk8.vz = work->unk6C[j].vz;
                s->unk10.vx = ex->math_sin(s->unk20) * 1400 / 4096;
                s->unk10.vy = 500;
                s->unk10.vz = ex->math_cos(s->unk20) * 1400 / 4096;
                s->unk10.vx += env->targets->centerX;
                s->unk10.vy += env->targets->centerY;
                s->unk10.vz += env->targets->centerZ;
            }
            break;
        case 17:
            work->unk178 = 4;
            break;
        }
        env->camera->points[0].vx = work->pos64.vx;
        env->camera->points[0].vy = work->pos64.vy;
        env->camera->points[0].vz = work->pos64.vz - 0x5F5;
        if (frame >= 0 && frame < 0x32) {
            k = frame;
            if (frame >= 3 && frame < 11) {
                pos.vx = work->pos64.vx;
                pos.vy = work->pos64.vy - 0x1013;
                pos.vz = work->pos64.vz - 0x5F5;
                setVector(&rot, 0, 0, 0);
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&work->scaleFC, work->model186);
                for (i = 0; i < 6; i++) {
                    fxm_rotPosPathGet(&D_801E965C[i], 3, &work->unk6C[i], &work->unk148[i]);
                    addVector(&work->unk148[i], &work->pos64);
                }
            } else {
                if (frame >= 11) {
                    k = frame - 8;
                }
                for (s = D_801E8F7C, i = 0; i < 6; i++, s++) {
                    if (s->unk18 == 0) {
                        continue;
                    }
                    if (k >= 0 && k < 11) {
                        f = frame >= 11 ? frame - 8 : frame;
                        fxm_rotPosPathGet(&D_801E965C[i], f, &work->unk6C[i], &work->unk148[i]);
                        copyVector(&work->unk9C[i], &work->unk148[i]);
                        addVector(&work->unk9C[i], &work->pos64);
                        addVector(&work->unk148[i], &work->pos64);
                    } else if (k >= s->unk1A + 13 && k < s->unk1A + 17) {
                        t = k - 13;
                        if (t < s->unk1A) {
                            continue;
                        }
                        if (s->unk1A == t) {
                            work->emit30[i] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, D_801E9060[i]), (s32)&D_801E9C8C[i]);
                            if (p != NULL) {
                                p->otZ = 0x80;
                                if (work->emit30[i] != NULL) {
                                    work->emit30[i]->trails = (s32)D_801E96BC[i];
                                    work->emit30[i]->trailSlots = 2;
                                    work->emit30[i]->trailStride = 0x70;
                                    if (work->emit30[i] != NULL) {
                                        *(s32 *)&work->emit30[i]->file->unk8 = D_801E906C[i];
                                    }
                                }
                            }
                        }
                        t -= s->unk1A;
                        copyVector(&work->unk6C[i], &s->unk8);
                        ex->btlfx_lerpVec((s16 *)&s->unk0, (s16 *)&s->unk10, t << 10, (s16 *)&work->unk148[i]);
                        copyVector(&work->unk9C[i], &work->unk148[i]);
                    }
                    if (k == s->unk1A + 17) {
                        copyVector(&work->unk110[i], &work->unk148[i]);
                        work->unk110[i].vy += 0x154;
                        work->emit48[i] = p = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->unk110[i]);
                        if (p != NULL) {
                            p->otZ = 0x80;
                        }
                    }
                    copyVector(&D_801E968C[i], &work->unk9C[i]);
                    if (k == s->unk1A + 21) {
                        s->unk1E = 4;
                    }
                    if (k >= s->unk1A + 16) {
                        ex->Hi_SetEffModelSlice(work->model17A[i], work->unk9C[i].vy - 100);
                        ex->Hi_DrawSliceEffModel(&work->unk6C[i], &work->unk9C[i], (s32)&work->scaleFC, work->model17A[i]);
                    } else {
                        ex->Hi_DrawEffModel(&work->unk6C[i], &work->unk9C[i], (s32)&work->scaleFC, work->model17A[i]);
                    }
                    if (s->unk1E >= 0) {
                        work->unk10C = fxm_lerpRatio(0, -0x80, 4 - s->unk1E, 4);
                        if (s->unk1E == 0) {
                            s->unk18 = 0;
                        } else {
                            s->unk1E--;
                        }
                        ex->Hi_ModifyEffModelAbr(work->model17A[i], 1);
                        ex->Hi_ModifyEffModelRGB(work->model17A[i], work->unk10C, work->unk10C, work->unk10C);
                    }
                }
            }
        }
        if (frame >= 11 && frame < 29) {
            for (j = 0; j < 6; j++) {
                if (work->emit0[j] != NULL && work->emit0[j]->state == BTLFX_EMIT_DONE) {
                    work->emit0[j] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit0[j], (s32)&work->unk148[j]);
            }
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_123_op82(s32 mode, Fx123Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 0x1C;
    } else if (mode == 1) {
        work->pos14.vx = env->targets->centerX;
        work->pos14.vy = env->targets->centerY;
        fx_123_env = env;
        work->pos14.vz = env->targets->centerZ;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xE), (s32)&work->pos14);
            work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos14);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&work->pos14);
            break;
        case 0xC:
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xD), (s32)&work->pos14);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xC), (s32)&work->pos14);
            break;
        }
        return frame >= 0x1E;
    }
    return 0;
}

s32 fx_123_op83(s32 mode, Fx123Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *arg4 = 4;
    } else if (mode == 1) {
        fx_123_env = env;
        work->unk2 = -1;
        work->unk0 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->unk0 = 0x10;
            break;
        case 20:
            work->unk2 = 0x10;
            break;
        }
        if (work->unk0 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, 0x10 - work->unk0, 0x10, 1, 0);
            work->unk0--;
        } else if (work->unk2 >= 0) {
            fxm_tintScreenLerp(0xFF, 0, 0x10 - work->unk2, 0x10, 1, 0);
            work->unk2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x20;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *a, SVECTOR *b, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_123_env)->LoadAverageShort12(a, b, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_rotPosPathInit(Fx123Path *path, u16 *data) {
    data++;
    path->unk2 = *data++;
    path->unk4 = (Fx123Point *)data;
}

static s32 fxm_rotPosPathGet(Fx123Path_0b4 *path, s32 i, SVECTOR *rot, SVECTOR *pos) {
    Fx123Key *k;
    s32 r = 1;

    if (i > path->unk2 - 1) {
        i = path->unk2 - 1;
    } else {
        r = 0;
    }
    k = &path->unk4[i];
    if (rot != NULL) {
        rot->vx = k->unk6[0];
        rot->vy = k->unk6[1];
        rot->vz = k->unk6[2];
    }
    if (pos != NULL) {
        pos->vx = k->unk0[0];
        pos->vy = k->unk0[1];
        pos->vz = k->unk0[2];
    }
    return r;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 arg4, s32 ot) {
    s32 c = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_123_env)->btlfx_drawScreenTint((u32 *)(fx_123_env->ot + ot), arg4, c, c, c);
}
