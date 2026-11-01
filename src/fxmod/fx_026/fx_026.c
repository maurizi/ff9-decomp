#include "fxmod/fx_026.h"

static void fxm_keyGetVec(Fx026Track *track, s32 t, SVECTOR *out) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    track = (Fx026Track *)&track->keys4[t * 3];
    out->vx = ((s16 *)track)[0];
    out->vy = ((s16 *)track)[1];
    out->vz = ((s16 *)track)[2];
}

static void fxm_keyGetVec2(Fx026Track *track, s32 t, SVECTOR *out0, SVECTOR *out1) {
    s16 *key;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    key = &track->keys4[t * 6];
    if (out0 != NULL) {
        out0->vx = key[0];
        out0->vy = key[1];
        out0->vz = key[2];
    }
    if (out1 != NULL) {
        out1->vx = key[3];
        out1->vy = key[4];
        out1->vz = key[5];
    }
}

static void fxm_keyGetVec3(Fx026Track *track, s32 t, SVECTOR *out0, SVECTOR *out1, SVECTOR *out2) {
    s16 *key;

    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    key = &track->keys4[t * 9];
    if (out0 != NULL) {
        out0->vx = key[0];
        out0->vy = key[1];
        out0->vz = key[2];
    }
    if (out1 != NULL) {
        out1->vx = key[3];
        out1->vy = key[4];
        out1->vz = key[5];
    }
    if (out2 != NULL) {
        out2->vx = key[6];
        out2->vy = key[7];
        out2->vz = key[8];
    }
}

static s32 fxm_keyGetValue(Fx026Track *track, s32 t) {
    if (t >= track->count2) {
        t = track->count2 - 1;
    }
    return (track->keys4 + t)[0];
}

static void fxm_drawScrollMoveX(s32 x, s32 dx, s32 dy, s32 w, s32 h, s32 sx, s32 sy) {
    s32 wrapW = x % w;
    Unk800F78A4 *env = fx_026_env;
    u32 *move = (u32 *)env->prim;
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 restW;
    RECT rectRest;
    RECT rectWrap;

    env->prim = (u8 *)(move + 12);
    restW = w - wrapW;
    if (wrapW != 0) {
        rectWrap.x = sx;
        rectWrap.y = sy;
        rectWrap.w = wrapW;
        rectWrap.h = h;
        ex->SetDrawMove(move, &rectWrap, dx + restW, dy);
        addPrim(env->ot + OT_LAST, move);
    }
    move += 6;
    if (restW != 0) {
        rectRest.x = sx + wrapW;
        rectRest.y = sy;
        rectRest.w = restW;
        rectRest.h = h;
        ex->SetDrawMove(move, &rectRest, dx, dy);
        addPrim(env->ot + OT_LAST, move);
    }
}

static s32 fx_026_op80PoolTask(s32 mode, Fx026Item *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame, Unk800DA1B8 *pool,
                  Fx026Work80 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR trackPos;
    s32 n;
    s32 t;

    if (mode == 1) {
        it->done0 = 0;
        it->emit8 = NULL;
    } else {
        if (it->done0 != 0) {
            return 0;
        }
        n = ((Fx026Track *)ex->btlfx_getFileData(entry->file, fx_026_op80TrackBlocks[it->track4]))->count2;
        if (n == 0) {
            n = 1;
        }
        t = (frame << 12) / n;
        fxm_keyGetVec((Fx026Track *)ex->btlfx_getFileData(entry->file, fx_026_op80TrackBlocks[it->track4]), frame, &trackPos);
        pos.vx = trackPos.vx;
        pos.vy = trackPos.vy;
        pos.vz = trackPos.vz;
        scale.vx = scale.vy = 0x1400 - ex->math_cos(t >> 2);
        scale.vz = t;
        rot.vx = -0xEE;
        rot.vy = 0;
        rot.vz = it->spin2;
        it->spin2 += 0x20;
        if (frame == 4) {
            it->posC.vx = pos.vx;
            it->posC.vy = pos.vy;
            it->posC.vz = pos.vz;
            it->emit8 = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x86), (s32)&it->posC);
            if (it->emit8 != NULL) {
                it->emit8->otZ = 0x80;
            }
        }
        if (it->emit8 != NULL) {
            if (it->emit8->state == BTLFX_EMIT_DONE) {
                it->emit8 = NULL;
            }
            ex->btlfx_moveEmitterTo((s32)it->emit8, (s32)&trackPos);
        }
        if (frame >= 4) {
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model68);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model6C);
        }
        if (frame >= n) {
            return 1;
        }
    }
    return 0;
}

s32 fx_026_op80(s32 mode, Fx026Work80 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR trackPos;
    Pool6CEntry *emitter;
    Fx026Item *it;
    s32 frame;
    s32 ringAlpha;
    s32 ringScale;
    s32 t;

    if (mode == 0) {
        init->workSize = 0x70;
        init->elemSize = 0x14;
        init->elemFn = (s32)fx_026_op80PoolTask;
        init->poolCount = 6;
    } else {
    if (mode == 1) {
        work->state0 = 0;
        work->frame4 = 0;
        fx_026_env = env;
        ex->btlfx_initEffModels(fx_026_effModels, 0x20);
        work->rot58.vy = 0x800;
        work->scale4C.vx = 0x1000;
        work->scale4C.vy = 0x1000;
        work->scale4C.vz = 0x1000;
        work->rot58.vx = 0;
        work->rot58.vz = 0;
        work->pos40.vx = 0;
        work->pos40.vy = 0x2000;
        work->pos40.vz = 0;
        fxm_keyGetVec((Fx026Track *)ex->btlfx_getFileData(entry->file, 0xB), 2, &trackPos);
        trackPos.vy = 0;
        work->pos10.vx = trackPos.vx;
        work->pos10.vy = trackPos.vy;
        work->pos10.vz = trackPos.vz;
        work->unk64 = 0;
    } else {
    ringAlpha = 0;
    frame = entry->frame;
    ringScale = 0;
    switch (work->state0) {
    case 0:
        if (frame == 0) {
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x19), (s32)&work->pos10);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x17), (s32)&work->pos10);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 4), (s32)&work->pos10);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
        }
        if (frame >= 40) {
            work->state0 = 1;
            entry->frame = -1;
            ex->Hi_SetSummonMotion(((Fx026Summon *)env->loader)->summon54->motion180, ((Fx026Summon *)env->loader)->model60);
            ex->Hi_SetSummonMotFrame(((Fx026Summon *)env->loader)->model60, 0);
            ex->Hi_StartSummonTexAnim(((Fx026Summon *)env->loader)->model60, 0, 0);
        }
        break;
    case 1:
        work->pos40.vx = env->camera->points[2].vx;
        work->pos40.vy = env->camera->points[2].vy;
        work->pos40.vz = env->camera->points[2].vz;
        if (frame >= 2) {
            ex->Hi_DrawSummonModel(&work->rot58, (VECTOR *)&work->pos40, (s32)&work->scale4C,
                                   ((Fx026Summon *)env->loader)->model60, 0);
            ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->model60, 0, (u16 *)&work->pos28);
        }
        fxm_keyGetVec((Fx026Track *)ex->btlfx_getFileData(entry->file, 0xB), work->frame4, &trackPos);
        work->pos40.vx = trackPos.vx;
        work->pos40.vy = trackPos.vy;
        work->pos40.vz = trackPos.vz;
        env->camera->points[2].vx = trackPos.vx;
        env->camera->points[2].vy = trackPos.vy;
        env->camera->points[2].vz = trackPos.vz;
        if (frame == 38) {
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA2), (s32)&work->pos28);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
        }
        if (frame == 42) {
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA3), (s32)&work->pos28);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
        }
        if (frame >= 66) {
            work->state0 = 2;
            entry->frame = -1;
            fx_026_op80Model = work->model68 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x85), 0);
            work->model6C = fx_026_op80Model2 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0x87), 1);
        }
        break;
    case 2:
        work->pos40.vx = env->camera->points[2].vx;
        work->pos40.vy = env->camera->points[2].vy;
        work->pos40.vz = env->camera->points[2].vz;
        t = (frame << 12) / 50;
        ex->Hi_DrawSummonModel(&work->rot58, (VECTOR *)&work->pos40, (s32)&work->scale4C,
                               ((Fx026Summon *)env->loader)->model60, 0);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->model60, 0, (u16 *)&work->pos28);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->model60, 0x19, (u16 *)&work->pos30);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->model60, 0x12, (u16 *)&work->pos38);
        fxm_keyGetVec((Fx026Track *)ex->btlfx_getFileData(entry->file, 0xE), frame, &trackPos);
        work->pos40.vx = trackPos.vx;
        work->pos40.vy = trackPos.vy;
        work->pos40.vz = trackPos.vz;
        env->camera->points[2].vx = trackPos.vx;
        env->camera->points[2].vy = trackPos.vy;
        env->camera->points[2].vz = trackPos.vz;
        if (frame == 0) {
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA1), (s32)&work->pos28);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
        }
        if (frame == 29) {
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA6), (s32)&work->pos30);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
        }
        if (frame == 32) {
            ex->Hi_StartSummonTexAnim(((Fx026Summon *)env->loader)->model60, 1, 0);
        }
        ringAlpha = t;
        ringScale = (ringAlpha >> 1) + 0x800;
        if (frame >= 50) {
            work->state0 = 3;
            entry->frame = -1;
            ex->btlfx_initParticleSystem();
            ex->btlfx_poolClear(entry->pool);
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA4), (s32)&work->pos28);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
        }
        break;
    case 3:
        if (frame == 0) {
            emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0xA5), (s32)&work->pos30);
            if (emitter != NULL) {
                emitter->otZ = 0x40;
            }
        }
        if (frame >= 4 && frame < 10) {
            it = ex->btlfx_poolSpawn((s16 *)entry->pool);
            if (it != NULL) {
                it->track4 = frame - 4;
                it->spin2 = ex->btlfx_rand();
            }
        }
        work->pos40.vx = env->camera->points[2].vx;
        work->pos40.vy = env->camera->points[2].vy;
        work->pos40.vz = env->camera->points[2].vz;
        ex->Hi_DrawSummonModel(&work->rot58, (VECTOR *)&work->pos40, (s32)&work->scale4C,
                               ((Fx026Summon *)env->loader)->model60, 0);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->model60, 0, (u16 *)&work->pos28);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->model60, 0x19, (u16 *)&work->pos30);
        ex->Hi_GetSummonBonePos(((Fx026Summon *)env->loader)->model60, 0x12, (u16 *)&work->pos38);
        fxm_keyGetVec((Fx026Track *)ex->btlfx_getFileData(entry->file, 0x11), frame, &trackPos);
        work->pos40.vx = trackPos.vx;
        work->pos40.vy = trackPos.vy;
        work->pos40.vz = trackPos.vz;
        env->camera->points[2].vx = trackPos.vx;
        ringAlpha = 0x1000;
        env->camera->points[2].vy = trackPos.vy;
        env->camera->points[2].vz = trackPos.vz;
        ringScale = ringAlpha;
        if (frame >= 29) {
            ex->btlfx_poolClear(entry->pool);
            return 1;
        }
        break;
    }
    if (ringAlpha != 0) {
        ringScale = ringScale * 2 / 3;
        if (work->frame4 & 1) {
            ringScale = ringScale * 9 >> 3;
        }
        env->otZ = 0x40;
        ex->btlfx_drawRings((u8 *)ex->btlfx_getFileData(entry->file, 0xA0), &work->pos30, 0, ringScale, 0, 1, ringAlpha);
        env->otZ = 0;
    }
    work->frame4++;
    }
    }
    return 0;
}

static s32 fx_026_op82PoolTask(s32 mode, Fx026Item82 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work82 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR keyPos;
    SVECTOR morphPos;
    SVECTOR prev;
    s32 halfWidth;
    s32 blend;
    u8 *data;
    u8 *track;
    Fx026Strip32 *strip;
    Fx026Pt *pt;
    s32 i;
    s32 t;
    s32 fade;

    if (mode == 1) {
        it->unk0 = 0;
        it->unk8 = 0;
    } else {
    switch (it->kind4) {
    case 0:
        track = (u8 *)ex->btlfx_getFileData(entry->file, 2);
        halfWidth = 0x46;
        strip = &fx_026_strips0[it->strip2];
        t = frame * 2;
        track += ((s32 *)track)[it->strip2];
        strip->count2 = 0x1F;
        strip->lastSlot0 = 0x1F;
        strip->head1 = 0;
        pt = strip->pts4;
        for (i = 0; i < 32; i++, pt++, t--) {
            if (t >= 0) {
                fxm_keyGetVec((Fx026Track *)track, t, &keyPos);
                if (i == 0) {
                    prev.vx = fx_026_stripOrigin.vx + keyPos.vx;
                    prev.vy = fx_026_stripOrigin.vy + keyPos.vy;
                    prev.vz = fx_026_stripOrigin.vz + keyPos.vz;
                }
                pt->vx = fx_026_stripOrigin.vx + keyPos.vx;
                pt->vy = fx_026_stripOrigin.vy + keyPos.vy;
                pt->vz = fx_026_stripOrigin.vz + keyPos.vz;
                pt->halfWidth6 = halfWidth;
                BTLFX_COPY_SVEC(&prev, pt);
            } else {
                BTLFX_COPY_SVEC(pt, &prev);
                pt->halfWidth6 = 0;
            }
        }
        fade = 0x1000;
        if (frame >= 16) {
            fade = ex->math_cos(((frame - 16) << 10) >> 4);
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x36), 0, 0, 0, 0x1000, fade, 1, (u8 *)strip, 0);
        if (frame >= 32) {
            return 1;
        }
        break;
    case 1:
        track = (u8 *)ex->btlfx_getFileData(entry->file, 0x9D);
        halfWidth = 0x46;
        strip = &fx_026_strips1[it->strip2];
        t = frame * 4;
        track += ((s32 *)track)[it->strip2];
        strip->count2 = 0x1F;
        strip->lastSlot0 = 0x1F;
        strip->head1 = 0;
        pt = strip->pts4;
        if (t > 32) {
            t = 32;
        }
        for (i = 0; i < 32; i++, pt++, t--) {
            if (t >= 0) {
                fxm_keyGetVec((Fx026Track *)track, t, &keyPos);
                if (i == 0) {
                    prev.vx = fx_026_stripOrigin.vx + keyPos.vx;
                    prev.vy = fx_026_stripOrigin.vy + keyPos.vy;
                    prev.vz = fx_026_stripOrigin.vz + keyPos.vz;
                }
                pt->vx = fx_026_stripOrigin.vx + keyPos.vx;
                pt->vy = fx_026_stripOrigin.vy + keyPos.vy;
                pt->vz = fx_026_stripOrigin.vz + keyPos.vz;
                pt->halfWidth6 = halfWidth;
                BTLFX_COPY_SVEC(&prev, pt);
            } else {
                BTLFX_COPY_SVEC(pt, &prev);
                pt->halfWidth6 = 0;
            }
        }
        fade = 0x1000;
        if (frame >= 8) {
            fade = ex->math_cos(((frame - 8) << 10) >> 4);
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x35), 0, 0, 0, 0x1000, fade, 1, (u8 *)strip, 0);
        if (frame >= 24) {
            return 1;
        }
        break;
    case 2:
        data = (u8 *)ex->btlfx_getFileData(entry->file, 0x9D);
        halfWidth = 0x58;
        track = data + ((s32 *)data)[it->strip2 + 6];
        data += ((s32 *)data)[it->strip2 + 12];
        if (frame < 10) {
            blend = 0;
        } else {
            blend = 0x1000 - ex->math_cos(((frame - 10) << 10) / 30);
        }
        t = frame * 4;
        strip = (Fx026Strip32 *)&fx_026_strips2[it->strip2];
        strip->count2 = 0x27;
        strip->lastSlot0 = 0x27;
        strip->head1 = 0;
        pt = strip->pts4;
        if (t > 40) {
            t = 40;
        }
        for (i = 0; i < 40; i++, pt++, t--) {
            if (t >= 0) {
                fxm_keyGetVec((Fx026Track *)track, t, &keyPos);
                fxm_keyGetVec((Fx026Track *)data, t, &morphPos);
                ex->btlfx_lerpVec((s16 *)&keyPos, (s16 *)&morphPos, blend, (s16 *)&keyPos);
                if (i == 0) {
                    prev.vx = fx_026_stripOrigin.vx + keyPos.vx;
                    prev.vy = fx_026_stripOrigin.vy + keyPos.vy;
                    prev.vz = fx_026_stripOrigin.vz + keyPos.vz;
                }
                pt->vx = fx_026_stripOrigin.vx + keyPos.vx;
                pt->vy = fx_026_stripOrigin.vy + keyPos.vy;
                pt->vz = fx_026_stripOrigin.vz + keyPos.vz;
                pt->halfWidth6 = halfWidth;
                BTLFX_COPY_SVEC(&prev, pt);
            } else {
                BTLFX_COPY_SVEC(pt, &prev);
                pt->halfWidth6 = 0;
            }
        }
        fade = 0x1000;
        if (frame >= 24) {
            fade = ex->math_cos(((frame - 24) << 10) >> 4);
        }
        ex->btlfx_drawTrail((u8 *)ex->btlfx_getFileData(entry->file, 0x34), 0, 0, 0, 0x1000, fade, 1, (u8 *)strip, 0);
        if (frame >= 40) {
            return 1;
        }
        break;
    }
    }
    return 0;
}

s32 fx_026_op82(s32 mode, Fx026Work82 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    SVECTOR rot;
    VECTOR pos;
    VECTOR scale;
    Fx026Item82 *it;
    Pool6CEntry *emitter;
    s32 frame;
    s32 i;
    s32 t;
    s32 bright;

    if (mode == 0) {
        init->workSize = 0x14;
        init->elemSize = 0xC;
        init->elemFn = (s32)fx_026_op82PoolTask;
        init->poolCount = 0x18;
    } else if (mode == 1) {
        work->pos8.vx = 0;
        work->pos8.vz = -2500;
        work->unk0 = 0;
        work->frame4 = 0;
        work->pos8.vy = 0;
        fx_026_stripOrigin.vx = work->pos8.vx;
        fx_026_stripOrigin.vy = work->pos8.vy;
        fx_026_env = env;
        fx_026_stripOrigin.vz = work->pos8.vz;
    } else {
        frame = entry->frame;
        if (work->unk0 == 0) {
            if (frame == 13) {
                for (i = 0; i < 6; i++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        it->kind4 = 0;
                        it->strip2 = i;
                    }
                }
            }
            if (frame == 13) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 5), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x140;
                }
            }
            if (frame >= 39 && frame < 92) {
                if (frame == 39) {
                    work->model10 = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, 0xAB), 1);
                }
                rot.vz = 0;
                rot.vx = 0;
                rot.vy = frame * 16;
                t = ((frame - 39) << 12) / 53 >> 2;
                scale.vx = scale.vz = 0x2000 - ex->math_cos(t);
                scale.vy = 0x1800;
                bright = ex->math_cos(t);
                pos.vx = work->pos8.vx;
                pos.vy = work->pos8.vy;
                pos.vz = work->pos8.vz;
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model10);
                ex->Hi_ModifyEffModelRGB(work->model10, (bright >> 5) - 0x80, (bright >> 5) - 0x80, (bright >> 5) - 0x80);
                fxm_drawScrollMoveX(1000 - frame * 2, 0x200, 0x1C0, 0x80, 0x40, 0x200, 0x180);
            }
            if (frame == 39) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x9F), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x9E), (s32)&work->pos8);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                for (i = 0; i < 6; i++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        it->kind4 = 1;
                        it->strip2 = i;
                    }
                }
            }
            if (frame == 39) {
                for (i = 0; i < 6; i++) {
                    it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                    if (it != NULL) {
                        it->kind4 = 2;
                        it->strip2 = i;
                    }
                }
            }
            if (frame >= 100) {
                return 1;
            }
        }
        work->frame4++;
    }
    return 0;
}

static void fxm_shatterInit(s32 slot, s32 life, Fx026Shatter *buf, Fx026Tri *tris) {
    Fx026Shatter *set;

    fx_026_shatterSets[slot] = buf;
    fx_026_shatterShards[slot] = (Fx026Shard *)(buf + 1);
    set = fx_026_shatterSets[slot];
    set->life0 = life;
    set->triCount2 = 0;
    set->nextTriC = tris;
    set->tris8 = tris;
}

static s32 fxm_shatterAddModel(s32 slot, s32 model) {
    BtlfxExports *ex = BTLFX_EXPORTS(fx_026_env);
    Fx026Shatter *set = fx_026_shatterSets[slot];
    Fx026Tri *tri = set->nextTriC;
    Fx026Shard *shard;
    s32 n;
    s32 k;

    n = (u32)ex->Hi_GetSplitMdlVertex(model, -1) / 24;
    ex->Hi_SplitMdlVertex(model, -1, (s32)tri);
    set->lastTris10 = tri;
    set->lastCount4 = n;
    set->nextTriC = tri + n;
    set->triCount2 += n;
    for (k = 0; k < n; k++, tri++) {
        tri->vert0.pad = 0;
    }
    shard = fx_026_shatterShards[slot];
    for (k = 0; k < n; k++, shard++) {
        shard->frame18 = -1;
    }
    return set->triCount2;
}

static Fx026Shard *fxm_shatterFreeShard(s32 slot) {
    Fx026Shatter *set = fx_026_shatterSets[slot];
    Fx026Shard *shard = fx_026_shatterShards[slot];
    s32 k;

    for (k = 0; k < set->triCount2; k++, shard++) {
        if (shard->frame18 < 0) {
            return shard;
        }
    }
    return NULL;
}

static s32 fxm_shatterRelease(s32 slot) {
    s32 idx = 0;
    s32 k = 0;
    Fx026Shatter *set = fx_026_shatterSets[slot];
    Fx026Tri *tri = set->tris8;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_026_env);
    s32 n = 0;
    s32 lineY = fx_026_shatterLine;
    Fx026Shard *shard;
    SVECTOR center;

    for (; k < set->triCount2; k++, tri++, idx++) {
        if (lineY < tri->vert8.vy && tri->vert0.pad == 0) {
            shard = fxm_shatterFreeShard(slot);
            if (shard != NULL) {
                tri->vert0.pad = 1;
                shard->x0 = ((tri->vert0.vx + tri->vert8.vx + tri->vert10.vx) / 3) << 12;
                shard->y4 = ((tri->vert0.vy + tri->vert8.vy + tri->vert10.vy) / 3) << 12;
                shard->z8 = ((tri->vert0.vz + tri->vert8.vz + tri->vert10.vz) / 3) << 12;
                shard->frame18 = 0;
                shard->tri1A = idx;
                shard->drag12 = 0x1000;
                shard->spin14 = ex->btlfx_randCentered(0x100);
                shard->spin16 = ex->btlfx_randCentered(0x200);
                center.vx = shard->x0 >> 12;
                center.vy = shard->y4 >> 12;
                center.vz = shard->z8 >> 12;
                shard->velC = (center.vx >> 5) + ex->btlfx_randCentered(0x18);
                n++;
                shard->velE = (center.vy >> 5) + (ex->btlfx_rand() & 0x1F);
                shard->vel10 = (center.vz >> 5) + ex->btlfx_randCentered(0x18);
            }
        }
    }
    return n;
}

/* move the flying shards of shatter i and write their triangles back */
static void fxm_shatterUpdate(s32 slot) {
    SVECTOR moved;
    SVECTOR rot;
    SVECTOR center;
    SVECTOR rel0;
    SVECTOR rel1;
    SVECTOR rel2;
    SVECTOR zero;
    VECTOR out;
    VECTOR sv;
    MATRIX m;
    BtlfxExports *ex = BTLFX_EXPORTS(fx_026_env);
    Fx026Shard *shard;
    Fx026Shatter *set;
    Fx026Tri *tri;
    s32 k;

    sv.vx = sv.vy = sv.vz = 0xF9C;
    shard = fx_026_shatterShards[slot];
    set = fx_026_shatterSets[slot];
    rot.vz = 0;
    *(s32 *)&zero.vx = 0;
    *(s32 *)&zero.vz = 0;
    for (k = 0; k < set->triCount2; k++, shard++) {
        if (shard->frame18 >= 0) {
            tri = &set->tris8[shard->tri1A];
            center.vx = shard->x0 >> 12;
            center.vy = shard->y4 >> 12;
            center.vz = shard->z8 >> 12;
            rel0.vx = tri->vert0.vx - center.vx;
            rel0.vy = tri->vert0.vy - center.vy;
            rel0.vz = tri->vert0.vz - center.vz;
            rel1.vx = tri->vert8.vx - center.vx;
            rel1.vy = tri->vert8.vy - center.vy;
            rel1.vz = tri->vert8.vz - center.vz;
            rel2.vx = tri->vert10.vx - center.vx;
            rel2.vy = tri->vert10.vy - center.vy;
            rel2.vz = tri->vert10.vz - center.vz;
            ex->btlfx_stepModuleParticle((Pool6CEntry *)shard, 0, &zero, &moved);
            *(s32 *)&rot = *(s32 *)&shard->spin14;
            ex->math_rotMatrixYXZ(&rot, &m);
            ex->ScaleMatrix(&m, &sv);
            m.t[0] = shard->x0 >> 12;
            m.t[1] = shard->y4 >> 12;
            m.t[2] = shard->z8 >> 12;
            ex->SetRotMatrix(&m);
            ex->SetTransMatrix(&m);
            gte_ldv0(&rel0);
            gte_rtv0tr();
            gte_stlvnl(&out);
            gte_ldv0(&rel1);
            gte_rtv0tr();
            tri->vert0.vx = out.vx;
            tri->vert0.vy = out.vy;
            tri->vert0.vz = out.vz;
            gte_stlvnl(&out);
            gte_ldv0(&rel2);
            gte_rtv0tr();
            tri->vert8.vx = out.vx;
            tri->vert8.vy = out.vy;
            tri->vert8.vz = out.vz;
            gte_stlvnl(&out);
            tri->vert10.vx = out.vx;
            tri->vert10.vy = out.vy;
            tri->vert10.vz = out.vz;
            shard->frame18++;
            if (shard->frame18 >= set->life0) {
                shard->frame18 = -1;
                tri->vert10.vz = 0;
                *(s32 *)&tri->vert10 = 0;
                tri->vert8.vz = 0;
                *(s32 *)&tri->vert8 = 0;
                tri->vert0.vz = 0;
                *(s32 *)&tri->vert0 = 0;
            }
        }
    }
}

static s32 fx_026_op81PoolTask(s32 mode, Fx026Item81 *it, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 frame,
                  Unk800DA1B8 *pool, Fx026Work81 *work) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    VECTOR pos;
    VECTOR scale;
    SVECTOR rot;
    SVECTOR keyPos;
    SVECTOR unused; /* never used, but it takes a stack slot */
    s32 model;
    s32 model2;
    s32 bright;
    Pool6CEntry *emitter;
    s32 fade;
    s32 shatter;

    if (mode == 1) {
        it->state0 = 0;
        it->pad4[1] = 0;
    } else if (it->piece2 < 6) {
        switch (it->state0) {
        case 0:
            fxm_keyGetVec((Fx026Track *)ex->btlfx_getFileData(entry->file, fx_026_pieceTrackBlocks[it->piece2]), frame, &keyPos);
            keyPos.vx += work->pos8.vx;
            keyPos.vy += work->pos8.vy;
            keyPos.vz += work->pos8.vz;
            pos.vx = keyPos.vx;
            pos.vy = keyPos.vy;
            pos.vz = keyPos.vz;
            scale.vx = 0x2000;
            scale.vy = 0x2000;
            scale.vz = 0x3000;
            rot.vx = -0x200;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, fx_026_op80Model2);
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, fx_026_op80Model);
            if (frame >= 3) {
                it->state0 = 1;
                ((s16 *)it)[-1] = -1;
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x20, 0x46);
                it->posC.vx = fx_026_pieceOffsets[it->piece2].vx;
                it->posC.vy = fx_026_pieceOffsets[it->piece2].vy;
                it->posC.vz = fx_026_pieceOffsets[it->piece2].vz;
                it->posC.vx += work->pos8.vx;
                it->posC.vy += work->pos8.vy;
                it->posC.vz += work->pos8.vz;
                it->posC.vy = 0;
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1A), (s32)&it->posC);
                if (emitter != NULL) {
                    emitter->otZ = 0x80;
                }
            }
            return 0;
        case 1:
            if (frame + (it->piece2 - 5) <= 0) {
                model = work->pieceModels18[it->piece2][0];
                model2 = -1;
            } else {
                if (frame < 16) {
                    model = work->pieceModels18[it->piece2][fx_026_pieceStages[frame]];
                } else {
                    model = work->pieceModels18[it->piece2][2];
                }
                if (frame < 16) {
                    model2 = -1;
                } else {
                    model2 = work->model6C[it->piece2];
                }
            }
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            if (model2 >= 0) {
                ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, model2);
            }
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, model);
            if (it->piece2 < 5) {
                shatter = fx_026_shatterNow;
            } else {
                shatter = fx_026_shatterNow5;
            }
            if (shatter != 0) {
                it->state0 = 2;
                ((s16 *)it)[-1] = -1;
                fxm_shatterInit(it->piece2, 0x14, (Fx026Shatter *)((u8 *)env->workBuf + fx_026_shatterBufs[it->piece2].setOfs0),
                              (Fx026Tri *)((u8 *)env->workBuf + fx_026_shatterBufs[it->piece2].trisOfs4));
                fxm_shatterAddModel(it->piece2, work->pieceModels18[it->piece2][2]);
                return 0;
            }
            break;
        case 2:
            pos.vx = work->pos8.vx;
            pos.vy = work->pos8.vy;
            pos.vz = work->pos8.vz;
            scale.vx = 0x1000;
            scale.vy = 0x1000;
            scale.vz = 0x1000;
            rot.vx = 0;
            rot.vy = 0;
            rot.vz = 0;
            ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->pieceModels18[it->piece2][2]);
            if (frame >= 14) {
                fade = 0x1000 - ((frame - 14) << 12) / 6;
                ex->Hi_ModifyEffModelAbr(work->pieceModels18[it->piece2][2], 1);
                bright = (fade / 2 >> 5) - 0x80;
                ex->Hi_ModifyEffModelRGB(work->pieceModels18[it->piece2][2], bright, bright, bright);
            } else {
                ex->Hi_ModifyEffModelAbr(work->pieceModels18[it->piece2][2], 1);
                bright = -0x40;
                ex->Hi_ModifyEffModelRGB(work->pieceModels18[it->piece2][2], bright, bright, bright);
            }
            fxm_shatterRelease(it->piece2);
            fxm_shatterUpdate(it->piece2);
            if (frame >= 20) {
                return 1;
            }
            break;
        }
    } else {
    switch (it->state0) {
    case 0:
        pos.vx = work->pos8.vx;
        pos.vy = work->pos8.vy;
        pos.vz = work->pos8.vz;
        scale.vx = 0x1000;
        scale.vy = 0x1000;
        scale.vz = 0x1000;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model6C[6]);
        if (frame < 8) {
            ex->Hi_ModifyEffModelRGB(work->model6C[6], ((frame << 12) >> 9) - 0x80, ((frame << 12) >> 9) - 0x80,
                                     ((frame << 12) >> 9) - 0x80);
        }
        if (fx_026_shatterNow6 != 0) {
            it->state0 = 1;
            ((s16 *)it)[-1] = -1;
            fxm_shatterInit(it->piece2, 0x57, fx_026_shatterBuf6, fx_026_shatterTris6);
            fxm_shatterAddModel(it->piece2, work->model6C[6]);
            return 0;
        }
        break;
    case 1:
        pos.vx = work->pos8.vx;
        pos.vy = work->pos8.vy;
        pos.vz = work->pos8.vz;
        scale.vx = 0x1000;
        scale.vy = 0x1000;
        scale.vz = 0x1000;
        rot.vx = 0;
        rot.vy = 0;
        rot.vz = 0;
        ex->Hi_DrawEffModel(&rot, &pos, (s32)&scale, work->model6C[6]);
        if (frame >= 55) {
            ex->Hi_ModifyEffModelRGB(work->model6C[6], ((0x1000 - (((frame - 55) << 12) >> 5)) >> 5) - 0x80,
                                     ((0x1000 - (((frame - 55) << 12) >> 5)) >> 5) - 0x80,
                                     ((0x1000 - (((frame - 55) << 12) >> 5)) >> 5) - 0x80);
        }
        fxm_shatterRelease(6);
        fxm_shatterUpdate(6);
        if (frame >= 87) {
            return 1;
        }
        break;
    }
    }
    return 0;
}

s32 fx_026_op81(s32 mode, Fx026Work81 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, EffInit *init) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    Fx026Item81 *it;
    Fx026Item81 *it2;
    Pool6CEntry *emitter;
    s32 frame;
    s32 state;
    s32 i;
    s32 j;
    s32 tint;

    if (mode == 0) {
        init->workSize = 0x88;
        init->elemSize = 0x14;
        init->elemFn = (s32)fx_026_op81PoolTask;
        init->poolCount = 0x4A;
    } else if (mode == 1) {
        fx_026_env = env;
        work->state0 = 0;
        work->frame4 = 0;
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 3; j++) {
                work->pieceModels18[i][j] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_026_pieceModelBlocks[i][j]), 0);
            }
        }
        for (i = 0; i < 7; i++) {
            work->model6C[i] = ex->btlfx_registerEffectModel((Unk800E8F50 *)ex->btlfx_getFileData(entry->file, fx_026_op81ModelBlocks[i]), 1);
        }
        work->pos8.vx = fx_026_op81Pos.vx;
        work->pos8.vy = fx_026_op81Pos.vy;
        work->pos8.vz = fx_026_op81Pos.vz;
        work->pos10.vx = fx_026_op81Pos.vx;
        work->pos10.vy = fx_026_op81Pos.vy;
        work->pos10.vz = fx_026_op81Pos.vz;
    } else {
        state = work->state0;
        frame = entry->frame;
        switch (state) {
        case 0:
            if (frame < 10 && (frame & 1)) {
                it = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it != NULL) {
                    it->piece2 = fx_026_pieceOrder[frame >> 1];
                }
            }
            if (frame == 5) {
                it2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it2 != NULL) {
                    it2->piece2 = 6;
                }
            }
            if (frame == 4) {
                it2 = ex->btlfx_poolSpawn((s16 *)entry->pool);
                if (it2 != NULL) {
                    it2->piece2 = 5;
                }
            }
            if (frame >= 16) {
                work->state0 = 1;
                entry->frame = -1;
                fx_026_shatterLine = 0x400;
            }
            break;
        case 1:
            if (frame == 20) {
                fx_026_shatterNow = 1;
                fx_026_shatterNow6 = 1;
            } else {
                fx_026_shatterLine = 0x400 - (frame << 7);
            }
            if (frame == 24) {
                fx_026_shatterNow5 = 1;
            }
            if (frame == 20) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x21), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x20), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x1F), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->trails = (s32)fx_026_trails;
                    emitter->trailSlots = 0xC;
                    emitter->trailStride = 0x4C;
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 18) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 0x22), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 32) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 7), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 44) {
                emitter = (Pool6CEntry *)ex->btlfx_startEmitter(ex->btlfx_getFileData(entry->file, 6), (s32)&work->pos10);
                if (emitter != NULL) {
                    emitter->otZ = 0x40;
                }
            }
            if (frame == 19) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 2, 0x80, 0x80, 0x80);
            }
            if (frame == 20) {
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, 0x80, 0xFF);
            }
            if (frame >= 20 && frame < 36) {
                tint = 0x80 - (((frame - 20) << 7) >> 4);
                ex->btlfx_drawScreenTint((u32 *)(env->ot + 1), 1, 0, tint >> 2, tint);
            }
            ex->btlfx_drawScrollMove(frame * 2, 0x2C0, 0x180, 0x40, 0x80, 0x2C0, 0x100);
            if (frame >= 126) {
                return 1;
            }
            break;
        }
        ex->btlfx_drawScrollMove(frame, 0x280, 0x100, 0x40, 0x80, 0x280, 0x180);
        work->frame4++;
    }
    return 0;
}

s32 fx_026_op87(s32 mode, Fx026Work87 *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *args) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;

    if (mode == 0) {
        *args = 0x14;
    } else if (mode == 1) {
        work->path0 = *args;
        fx_026_env = env;
    } else {
        frame = entry->frame;
        fxm_keyGetVec((Fx026Track *)ex->btlfx_getFileData(entry->file, fx_026_camPaths[work->path0].targetTrack2), frame,
                      &work->target4);
        env->camera->points[1].vx = work->target4.vx;
        env->camera->points[1].vy = work->target4.vy;
        env->camera->points[1].vz = work->target4.vz;
        fxm_keyGetVec((Fx026Track *)ex->btlfx_getFileData(entry->file, fx_026_camPaths[work->path0].posTrack0), frame,
                      &work->posC);
        env->camera->points[0].vx = work->posC.vx;
        env->camera->points[0].vy = work->posC.vy;
        env->camera->points[0].vz = work->posC.vz;
        if (frame >= fx_026_camPaths[work->path0].length4) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_RODATA("asm/us/fx_026/nonmatchings/fx_026", fx_026_headerPtr);
