#include "fxmod/fx_123.h"

static s32 fxm_pinParticle0(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[0], frame, n);
}

static s32 fxm_pinParticle1(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[1], frame, n);
}

static s32 fxm_pinParticle2(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[2], frame, n);
}

static s32 fxm_pinParticle3(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[3], frame, n);
}

static s32 fxm_pinParticle4(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[4], frame, n);
}

static s32 fxm_pinParticle5(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos1[0], frame, n);
}

s32 fxm_pinParticle6(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[0], frame, n);
}

s32 fxm_pinParticle7(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[1], frame, n);
}

s32 fxm_pinParticle8(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[2], frame, n);
}

s32 fxm_pinParticle9(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[3], frame, n);
}

s32 fxm_pinParticle10(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos0[4], frame, n);
}

s32 fxm_pinParticle11(Pool30Entry *pt, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    return fxm_pinParticle(pt, pos, &fx_123_pinPos1[0], frame, n);
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

s32 fx_123_op80(s32 mode, Fx123Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    BtlBone m;
    Fx123Bone *bone;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;

    if (mode == 0) {
        *args = 0x8C;
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
            work->emit0[i] = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&fx_123_emitPos0[i]);
            if (emitter != NULL) {
                emitter->otZ = 0x20;
            }
        }
        break;
    case 20:
        for (i = 0; i < 6; i++) {
            work->emit18[i] = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 9), (s32)&fx_123_emitPos0[i]);
            if (emitter != NULL) {
                emitter->otZ = 0x20;
            }
        }
        break;
    case 30:
        for (i = 0; i < 6; i++) {
            work->emit30[i] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xB), (s32)&fx_123_emitPos1[i]);
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
        bone = fx_123_op80Bones;
        for (i = 0; i < 6; i++, bone++) {
            ex->btlfx_getUnitBoneMatrix(0x10, bone->bone0, &m);
            ex->btlfx_matrixPointAhead((MATRIX *)&m, bone->dist2, (s32)&work->pos5C[i]);
            if (work->emit0[i] != NULL && work->emit0[i]->state == BTLFX_EMIT_DONE) {
                work->emit0[i] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0[i], (s32)&work->pos5C[i]);
        }
    }
    if (frame >= 0x14 && frame < 0x26) {
        bone = fx_123_op80Bones;
        for (i = 0; i < 6; i++, bone++) {
            ex->btlfx_getUnitBoneMatrix(0x10, bone->bone0, &m);
            ex->btlfx_matrixPointAhead((MATRIX *)&m, bone->dist2, (s32)&work->pos5C[i]);
            if (work->emit18[i] != NULL && work->emit18[i]->state == BTLFX_EMIT_DONE) {
                work->emit18[i] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit18[i], (s32)&work->pos5C[i]);
        }
    }
    if (frame >= 0x1E && frame < 0x37) {
        bone = fx_123_op80Bones;
        for (i = 0; i < 6; i++, bone++) {
            ex->btlfx_getUnitBoneMatrix(0x10, bone->bone0, &m);
            ex->btlfx_matrixPointAhead((MATRIX *)&m, bone->dist2, (s32)&work->pos5C[i]);
            if (work->emit30[i] != NULL && work->emit30[i]->state == BTLFX_EMIT_DONE) {
                work->emit30[i] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit30[i], (s32)&work->pos5C[i]);
        }
    }
    return frame >= 0x50;
    }
    return 0;
}

s32 fx_123_op81(s32 mode, Fx123Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    Pool6CEntry *emitter;
    s32 frame;
    Fx123Shot *shot;
    s32 j;
    s32 i;
    s32 shotFrame;
    s32 flyFrame;
    s32 pathFrame;
    s32 path;

    if (mode == 0) {
        *args = 0x188;
    } else if (mode == 1) {
        fx_123_env = env;
        ex->btlfx_initEffModels(fx_123_effModels, 7);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos64);
        for (path = 0; path < 6; path++) {
            fxm_rotPosPathInit((Fx123Path *)&fx_123_rotPosPath[path], (u16 *)ex->btlfx_getFileData(entry->file, fx_123_pathFileIds[path]));
            setVector(&work->rot6C[path], 0, 0, 0);
            setVector(&work->pos9C[path], 0, 0, 0);
            work->model17A[path] = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 7), 0x97, 0x3D40, 0x80, 0);
        }
        setVector(&work->scaleFC, 0x1000, 0x1000, 0x1000);
        work->model186 = ex->Hi_RegisterTexEffModel((BtlModelHeader *)ex->btlfx_getFileData(entry->file, 5), 0x97, 0x3D40, 0x80, 0);
        work->unk178 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->pos140.vx = work->pos64.vx;
            work->pos140.vy = work->pos64.vy - 0x1013;
            work->pos140.vz = work->pos64.vz - 0x5F5;
            work->emit60 = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xF), (s32)&work->pos140);
            if (emitter != NULL) {
                emitter->otZ = 0x80;
            }
            break;
        case 11:
            for (j = 0; j < 6; j++) {
                work->emit0[j] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 8), (s32)&fx_123_emitPos2[j]);
            }
            break;
        case 19:
            for (shot = fx_123_op81Shots, j = 0; j < 6; j++, shot++) {
                shot->pos0.vx = env->targets->centerX;
                shot->pos0.vy = work->pos9C[j].vy;
                shot->pos0.vz = env->targets->centerZ;
                shot->pos0.vx += ex->math_sin(shot->angle20) * 350 / 4096;
                shot->pos0.vz += ex->math_cos(shot->angle20) * 350 / 4096;
                shot->rot8.vx = work->rot6C[j].vx;
                shot->rot8.vy = work->rot6C[j].vy;
                shot->rot8.vz = work->rot6C[j].vz;
                shot->pos10.vx = ex->math_sin(shot->angle20) * 1400 / 4096;
                shot->pos10.vy = 500;
                shot->pos10.vz = ex->math_cos(shot->angle20) * 1400 / 4096;
                shot->pos10.vx += env->targets->centerX;
                shot->pos10.vy += env->targets->centerY;
                shot->pos10.vz += env->targets->centerZ;
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
            shotFrame = frame;
            if (frame >= 3 && frame < 11) {
                pos.vx = work->pos64.vx;
                pos.vy = work->pos64.vy - 0x1013;
                pos.vz = work->pos64.vz - 0x5F5;
                setVector(&rot, 0, 0, 0);
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&work->scaleFC, work->model186);
                for (i = 0; i < 6; i++) {
                    fxm_rotPosPathGet(&fx_123_rotPosPath[i], 3, &work->rot6C[i], &work->pos148[i]);
                    addVector(&work->pos148[i], &work->pos64);
                }
            } else {
                if (frame >= 11) {
                    shotFrame = frame - 8;
                }
                for (shot = fx_123_op81Shots, i = 0; i < 6; i++, shot++) {
                    if (shot->active18 == 0) {
                        continue;
                    }
                    if (shotFrame >= 0 && shotFrame < 11) {
                        pathFrame = frame >= 11 ? frame - 8 : frame;
                        fxm_rotPosPathGet(&fx_123_rotPosPath[i], pathFrame, &work->rot6C[i], &work->pos148[i]);
                        copyVector(&work->pos9C[i], &work->pos148[i]);
                        addVector(&work->pos9C[i], &work->pos64);
                        addVector(&work->pos148[i], &work->pos64);
                    } else if (shotFrame >= shot->frame1A + 13 && shotFrame < shot->frame1A + 17) {
                        flyFrame = shotFrame - 13;
                        if (flyFrame < shot->frame1A) {
                            continue;
                        }
                        if (shot->frame1A == flyFrame) {
                            work->emit30[i] = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, fx_123_emitFileIds[i]), (s32)&fx_123_emitPos3[i]);
                            if (emitter != NULL) {
                                emitter->otZ = 0x80;
                                if (work->emit30[i] != NULL) {
                                    work->emit30[i]->trails = (s32)fx_123_op81Trails[i];
                                    work->emit30[i]->trailSlots = 2;
                                    work->emit30[i]->trailStride = 0x70;
                                    if (work->emit30[i] != NULL) {
                                        *(s32 *)&work->emit30[i]->file->m.module.update = fx_123_pinParticles[i];
                                    }
                                }
                            }
                        }
                        flyFrame -= shot->frame1A;
                        copyVector(&work->rot6C[i], &shot->rot8);
                        ex->btlfx_lerpVec((s16 *)&shot->pos0, (s16 *)&shot->pos10, flyFrame << 10, (s16 *)&work->pos148[i]);
                        copyVector(&work->pos9C[i], &work->pos148[i]);
                    }
                    if (shotFrame == shot->frame1A + 17) {
                        copyVector(&work->pos110[i], &work->pos148[i]);
                        work->pos110[i].vy += 0x154;
                        work->emit48[i] = emitter = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA), (s32)&work->pos110[i]);
                        if (emitter != NULL) {
                            emitter->otZ = 0x80;
                        }
                    }
                    copyVector(&fx_123_pinPos0[i], &work->pos9C[i]);
                    if (shotFrame == shot->frame1A + 21) {
                        shot->timer1E = 4;
                    }
                    if (shotFrame >= shot->frame1A + 16) {
                        ex->Hi_SetEffModelSlice(work->model17A[i], work->pos9C[i].vy - 100);
                        ex->Hi_DrawSliceEffModel(&work->rot6C[i], &work->pos9C[i], (s32)&work->scaleFC, work->model17A[i]);
                    } else {
                        ex->Hi_DrawEffModel(&work->rot6C[i], &work->pos9C[i], (s32)&work->scaleFC, work->model17A[i]);
                    }
                    if (shot->timer1E >= 0) {
                        work->fade10C = fxm_lerpRatio(0, -0x80, 4 - shot->timer1E, 4);
                        if (shot->timer1E == 0) {
                            shot->active18 = 0;
                        } else {
                            shot->timer1E--;
                        }
                        ex->Hi_ModifyEffModelAbr(work->model17A[i], 1);
                        ex->Hi_ModifyEffModelRGB(work->model17A[i], work->fade10C, work->fade10C, work->fade10C);
                    }
                }
            }
        }
        if (frame >= 11 && frame < 29) {
            for (j = 0; j < 6; j++) {
                if (work->emit0[j] != NULL && work->emit0[j]->state == BTLFX_EMIT_DONE) {
                    work->emit0[j] = NULL;
                }
                ex->btlfx_moveEmitterTo((s32)work->emit0[j], (s32)&work->pos148[j]);
            }
        }
        if (frame >= 0x50) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

s32 fx_123_op82(s32 mode, Fx123Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x1C;
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

s32 fx_123_op83(s32 mode, Fx123Work83 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 4;
    } else if (mode == 1) {
        fx_123_env = env;
        work->timer2 = -1;
        work->timer0 = -1;
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0:
            work->timer0 = 0x10;
            break;
        case 20:
            work->timer2 = 0x10;
            break;
        }
        if (work->timer0 >= 0) {
            fxm_tintScreenLerp(0, 0xFF, 0x10 - work->timer0, 0x10, 1, 0);
            work->timer0--;
        } else if (work->timer2 >= 0) {
            fxm_tintScreenLerp(0xFF, 0, 0x10 - work->timer2, 0x10, 1, 0);
            work->timer2--;
        } else {
            ex->btlfx_drawScreenTint((u32 *)env->ot, 1, 0xFF, 0xFF, 0xFF);
        }
        return frame >= 0x20;
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_123_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_rotPosPathInit(Fx123Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx123Point *)data;
}

static s32 fxm_rotPosPathGet(Fx123Path_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos) {
    Fx123Key *key;
    s32 done = 1;

    if (t > path->count2 - 1) {
        t = path->count2 - 1;
    } else {
        done = 0;
    }
    key = &path->keys4[t];
    if (rot != NULL) {
        rot->vx = key->rot6[0];
        rot->vy = key->rot6[1];
        rot->vz = key->rot6[2];
    }
    if (pos != NULL) {
        pos->vx = key->pos0[0];
        pos->vy = key->pos0[1];
        pos->vz = key->pos0[2];
    }
    return done;
}

static void fxm_tintScreenLerp(s32 from, s32 to, s32 t, s32 n, s32 abr, s32 ot) {
    s32 bright = (from * (n - t) + to * t) / n;

    BTLFX_EXPORTS(fx_123_env)->btlfx_drawScreenTint((u32 *)(fx_123_env->ot + ot), abr, bright, bright, bright);
}
