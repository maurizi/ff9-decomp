#include "fxmod/fx_128.h"

static s32 fx_128_op81ParticleMotion0(Fx128Particle *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    BTLFX_EXPORTS(fx_128_env)->btlfx_lerpVec((s16 *)&fx_128_lerpFrom, (s16 *)&fx_128_lerpTo, (frame << 12) / 12, (s16 *)pos);
    out->pos0.vx = pos->vx << 12;
    out->pos0.vy = pos->vy << 12;
    out->pos0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

static s32 fx_128_op81ParticleMotion1(Fx128Particle *out, s32 owner, SVECTOR *pos, s32 trail, s32 frame, s32 n) {
    BTLFX_EXPORTS(fx_128_env)->btlfx_lerpVec((s16 *)&fx_128_lerpFrom, (s16 *)((u8 *)fx_128_lerpToRand + (out->index1F - 1) * 8),
                                             (frame << 12) / 12, (s16 *)pos);
    out->pos0.vx = pos->vx << 12;
    out->pos0.vy = pos->vy << 12;
    out->pos0.vz = pos->vz << 12;
    if (frame >= n) {
        return -1;
    }
    return 0;
}

s32 fx_128_op80(s32 mode, Fx128Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x18;
    } else if (mode == 1) {
        fx_128_env = env;
        ex->btlfx_getUnitBodyPos(0x10, 0, (s16 *)&work->pos8);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot10);
        work->emit0 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_LIGHT_STREAK), (s32)&work->pos8);
        work->emit4 = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_SPARKLE_CIRCLE), (s32)&work->pos8);
        if (work->emit0 != NULL) {
            work->emit0->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
        if (work->emit4 != NULL) {
            work->emit4->otZ = ex->btlfx_addRadiusSixth(0x10, 0x80);
        }
    } else {
        frame = entry->frame;
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

s32 fx_128_op81(s32 mode, Fx128Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    SVECTOR ang;
    SVECTOR bodyPos;
    SVECTOR slicePos;
    MATRIX m;
    VECTOR lv;
    SVECTOR offset;
    s32 morph;
    s32 frame;
    s32 i;
    s32 j;
    s32 pathFrame;
    SVECTOR *unused;

    if (mode == 0) {
        *args = 0xB0;
    } else if (mode == 1) {
        fx_128_env = env;
        ex->btlfx_initEffModels(fx_128_effModels, 7);
        ex->btlfx_getUnitPos(0x10, (s16 *)&work->pos50);
        ex->btlfx_getUnitRot(0x10, (s16 *)&work->rot58);
        if (env->targets->targetCount == mode) {
            ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->pos60);
            ex->btlfx_getUnitPos(0, (s16 *)&work->pos68);
        } else {
            work->pos60.vx = env->targets->centerX;
            work->pos60.vy = env->targets->centerY;
            work->pos60.vz = env->targets->centerZ;
            work->pos68.vx = env->targets->centerX;
            work->pos68.vy = env->targets->centerY;
            work->pos68.vz = env->targets->centerZ;
            work->pos68.vy = 0;
        }
        work->rot24.vx = 0;
        work->rot24.vy = 0;
        work->rot24.vz = 0;
        work->pos2C.vx = 0;
        work->pos2C.vy = 0;
        work->pos2C.vz = 0;
        work->scale3C.vx = 0x1000;
        work->scale3C.vy = 0x1000;
        work->scale3C.vz = 0x1000;
        work->modelA0[0] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_DRAGON), 1);
        work->modelA0[1] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_DRAGON_2), 1);
        fxm_rotPosPathInit((Fx128Path *)&fx_128_rotPosPath0, (u16 *)ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_ROT_POS_PATH));
        fxm_posPathInit(&fx_128_posPath, (u16 *)ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_POS_PATH));
        fxm_valuePathInit((Fx128Path *)&fx_128_valuePath, (u16 *)ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_VALUE_PATH));
        for (j = 0; j < 5; j++) {
            fxm_rotPosPathInit((Fx128Path *)&fx_128_rotPosPath1[j], (u16 *)ex->btlfx_getFileData(entry->file, fx_128_pathFileIds[j]));
            work->modelA0[j + 2] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_128_modelFileIds[j]), 1);
        }
        work->timer98 = -1;
        work->timer9C = -1;
        ex->btlfx_pointAhead((s16 *)&work->pos50, (s16 *)&work->rot58, -0x1DE, (s16 *)&work->pos70);
        work->emit0[1] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_VIOLET_RINGS), (s32)&work->pos70);
    } else {
        frame = entry->frame;
        switch (frame) {
        case 0x19:
            work->emit0[0] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_BUBBLE_RINGS), (s32)&fx_128_emitPos0);
            break;
        case 0x2D:
            work->pos78.vx = work->pos90.vx;
            work->pos78.vy = work->pos90.vy;
            work->pos78.vz = work->pos90.vz;
            work->pos78.vy += 0x50;
            fx_128_lerpFrom.vx = work->pos78.vx;
            fx_128_lerpFrom.vy = work->pos78.vy;
            fx_128_lerpFrom.vz = work->pos78.vz;
            fx_128_lerpTo.vx = work->pos60.vx;
            fx_128_lerpTo.vy = work->pos60.vy;
            fx_128_lerpTo.vz = work->pos60.vz;
            for (i = 0; i < 10; i++) {
                fx_128_lerpToRand[i].vx = work->pos60.vx;
                fx_128_lerpToRand[i].vy = work->pos60.vy;
                fx_128_lerpToRand[i].vz = work->pos60.vz;
                if (ex->btlfx_rand() & 1) {
                    fx_128_lerpToRand[i].vx = fx_128_lerpToRand[i].vx + (0xC8 + ex->btlfx_rand() % 400);
                } else {
                    fx_128_lerpToRand[i].vx = fx_128_lerpToRand[i].vx - (0xC8 + ex->btlfx_rand() % 400);
                }
                if (ex->btlfx_rand() & 1) {
                    fx_128_lerpToRand[i].vy = fx_128_lerpToRand[i].vy + (0xC8 + ex->btlfx_rand() % 400);
                } else {
                    fx_128_lerpToRand[i].vy = fx_128_lerpToRand[i].vy - (0xC8 + ex->btlfx_rand() % 400);
                }
            }
            ex->btlfx_aimAngles((s16 *)&work->pos78, (s16 *)&work->pos60, (s16 *)&work->rot80);
            work->emit0[2] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_BUBBLES), (s32)&fx_128_emitPos1);
            work->emit0[3] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_ORB_2), (s32)&fx_128_emitPos1);
            work->emit0[4] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_GREEN_TRAIL_2), (s32)&work->pos78);
            if (work->emit0[4] != NULL) {
                work->emit0[4]->trails = (s32)fx_128_op81Trails0;
                work->emit0[4]->trailSlots = 9;
                work->emit0[4]->trailStride = 0x48;
            }
            if (work->emit0[2] != NULL) {
                work->emit0[2]->file->m.module.update = fx_128_op81ParticleMotion0;
            }
            if (work->emit0[3] != NULL) {
                work->emit0[3]->file->m.module.update = fx_128_op81ParticleMotion1;
            }
            break;
        case 0x3C:
            work->emit0[5] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_BUBBLES_2), (s32)&work->pos60);
            work->emit0[6] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_ORB), (s32)&work->pos60);
            work->emit0[7] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_GREEN_TRAIL), (s32)&work->pos60);
            work->emit0[8] = ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, FX_DRAGON_BREATH_WHITE_RINGS), (s32)&work->pos68);
            if (work->emit0[7] != NULL) {
                work->emit0[7]->trails = (s32)fx_128_op81Trails1;
                work->emit0[7]->trailSlots = 8;
                work->emit0[7]->trailStride = 0x48;
            }
            if (work->emit0[5] != NULL) {
                work->emit0[5]->otZ = 0x80;
            }
            break;
        case 8:
            work->timer98 = 4;
            break;
        case 0x39:
            work->timer9C = 8;
            break;
        }
        if (frame >= 8 && frame < 0x41) {
            pathFrame = frame - 8;
            if (work->timer98 >= 0 || work->timer9C >= 0) {
                if (work->timer98 >= 0) {
                    work->fade4C = fxm_lerpRatio(-0x80, 0, 4 - work->timer98, 4);
                    work->timer98--;
                }
                if (work->timer9C >= 0) {
                    work->fade4C = fxm_lerpRatio(0, -0x80, 8 - work->timer9C, 8);
                    work->timer9C--;
                }
            }
            fxm_rotPosPathGet(&fx_128_rotPosPath0, pathFrame, &work->rot24, &bodyPos);
            work->pos2C.vx = bodyPos.vx;
            work->pos2C.vy = bodyPos.vy;
            work->pos2C.vz = bodyPos.vz;
            work->pos2C.vx += work->pos50.vx;
            work->pos2C.vy += work->pos50.vy;
            work->pos2C.vz += work->pos50.vz;
            work->pos88.vx = work->pos2C.vx;
            work->pos88.vy = work->pos2C.vy;
            work->pos88.vz = work->pos2C.vz;
            ex->btlfx_aimAngles((s16 *)&work->pos70, (s16 *)&work->pos60, (s16 *)&ang);
            work->rot24.vy = ang.vy;
            if (env->targets->targetCount == 1 && pathFrame >= 0x25 && work->rot24.vx <= ang.vx) {
                work->rot24.vx = ang.vx;
            }
            fxm_valuePathGet(&fx_128_valuePath, pathFrame, &morph);
            ex->Hi_DrawMorphEffModel(&work->rot24, (VECTOR *)&work->pos2C, (s32)&work->scale3C, work->modelA0[0],
                                     work->modelA0[1], morph);
            if (work->timer98 >= 0 || work->timer9C >= 0) {
                ex->Hi_ModifyEffModelRGB(work->modelA0[0], work->fade4C, work->fade4C, work->fade4C);
            }
            for (i = 0; i < 5; i++) {
                fxm_rotPosPathGet(&fx_128_rotPosPath1[i], pathFrame, &work->rot24, &slicePos);
                work->pos2C.vx = slicePos.vx;
                work->pos2C.vy = slicePos.vy;
                work->pos2C.vz = slicePos.vz;
                work->pos2C.vx += work->pos50.vx;
                work->pos2C.vy += work->pos50.vy;
                work->pos2C.vz += work->pos50.vz;
                ex->Hi_SetEffModelSlice(work->modelA0[i + 2], work->pos2C.vy);
                ex->Hi_DrawSliceEffModel(&work->rot24, (VECTOR *)&work->pos2C, (s32)&work->scale3C, work->modelA0[i + 2]);
                if (work->timer98 >= 0 || work->timer9C >= 0) {
                    ex->Hi_ModifyEffModelRGB(work->modelA0[i + 2], work->fade4C, work->fade4C, work->fade4C);
                }
            }
        }
        if (frame >= 0x2D && work->emit0[4] != NULL) {
            if (work->emit0[4]->state == BTLFX_EMIT_DONE) {
                work->emit0[4] = NULL;
            }
            if (work->emit0[4] != NULL) {
                work->emit0[4]->rot.vx = work->rot80.vx;
                work->emit0[4]->rot.vy = work->rot80.vy;
                work->emit0[4]->rot.vz = work->rot80.vz;
            }
        }
        if (frame >= 0x19) {
            fxm_posPathGet(&fx_128_posPath, frame - 8, &offset);
            rot.vx = 0;
            rot.vy = work->rot24.vy;
            rot.vz = 0;
            ex->math_rotMatrixYXZ(&rot, &m);
            gte_SetRotMatrix(&m);
            gte_ldtr0();
            gte_ldv0(&offset);
            gte_rtv0tr();
            gte_stlvnl(&lv);
            work->pos90.vx = lv.vx;
            work->pos90.vy = lv.vy;
            work->pos90.vz = lv.vz;
            work->pos90.vx += work->pos50.vx;
            work->pos90.vy += work->pos50.vy;
            work->pos90.vz += work->pos50.vz;
            if (work->emit0[0] != NULL && work->emit0[0]->state == BTLFX_EMIT_DONE) {
                work->emit0[0] = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)work->emit0[0], (s32)&work->pos90);
        }
        if (frame >= 0x82) {
            ex->btlfx_clearEffModels();
            return 1;
        }
    }
    return 0;
}

static void fxm_lerpSvec(SVECTOR *from, SVECTOR *to, s32 t, SVECTOR *out) {
    BTLFX_EXPORTS(fx_128_env)->LoadAverageShort12(from, to, 0x1000 - t, t, out);
}

static s32 fxm_lerpRatio(s32 from, s32 to, s32 t, s32 n) {
    return (from * (n - t) + to * t) / n;
}

static void fxm_rotPosPathInit(Fx128Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx128Point *)data;
}

static s32 fxm_rotPosPathGet(Fx128Path_0b4 *path, s32 t, SVECTOR *rot, SVECTOR *pos) {
    Fx128Key *key;
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

static void fxm_posPathInit(Fx128Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx128Point *)data;
}

static s32 fxm_posPathGet(Fx128Path *path, s32 t, SVECTOR *out) {
    s32 last = path->count2 - 1;
    s32 done = 1;
    Fx128Point *key;

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

static void fxm_valuePathInit(Fx128Path *path, u16 *data) {
    data++;
    path->count2 = *data++;
    path->keys4 = (Fx128Point *)data;
}

static s32 fxm_valuePathGet(Fx128Keys *keys, s32 t, s32 *out) {
    s32 done = 1;

    if (t > keys->count2 - 1) {
        t = keys->count2 - 1;
    } else {
        done = 0;
    }
    *out = *(s16 *)((u8 *)keys->values4 + (t << 1));
    return done;
}

INCLUDE_RODATA("asm/us/fx_128/nonmatchings/fx_128", fx_128_headerPtr);
